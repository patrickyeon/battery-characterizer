#include "./logger.h"
#include "../driver/flash.h"

#include <assert.h>
#include <stdbool.h>

//FIXME real addresses
#define LOG_BASE 0x08004000
#define N_LOG_PAGES 3

// A log line is stored in 8 bytes of memory, laid out like so:
//   uint16_t timestamp;
//   log_type_e msg_type;
//   uint8_t payload[4];
//   uint8_t checksum;

// There are 127 log lines per 1KB page, which has an accompanying header:
enum header_status_e {
    HEADER_INIT = 1,
    HEADER_OVF = 1 << 1,
    HEADER_TIMEOUT = 1 << 2,
    HEADER_RESET = 1 << 3
};

typedef struct header_t {
     uint16_t seqnum_base;
     uint32_t timestamp_offset;
     uint8_t status; // bitmask of header_status_e flags
     uint8_t last_seqnum;
} header_t;

// Reconstructing the seqnum and timestamp of a log item is:
//   uint32_t seconds_since_epoch = timestame_offset + timestamp;
//   uint16_t seqnum = seqnum_base + (&logline - &header) / 8 - 1;
// seqnum_base can be from 0..(65535-127), because we're not going to handle
// wrapping that. Furthermore, a seqnum_base of 0xffff is interpreted as "this
// page is erased and ready for use".
//
// We treat the log pages in flash as a big circular buffer, refusing to write
// on overflow.

static uint16_t _seqnum;

static void write_header(header_t *h, int npage);
static void finalize_header(header_t *h, int npage);
static void read_header(int npage, header_t *h);

static header_t _log[N_LOG_PAGES];

//FIXME real crc.
static uint8_t crc(uint8_t *buff, uint8_t len) {
    return 0;
}

static struct log_ptr_t {
    int page;
    uint8_t offset;
} n_write = {-1, 0xff}, n_read = {-1, 0};

static inline uint32_t header_addr(uint8_t npage) {
    assert(npage < N_LOG_PAGES);
    return LOG_BASE + npage * FLASH_PAGESIZE;
}

static inline uint32_t log_addr(uint8_t npage, uint8_t nline) {
    assert(npage < N_LOG_PAGES);
    assert(nline <= 126);
    return LOG_BASE + npage * FLASH_PAGESIZE + (nline + 1) * 8;
}

void logger_init(void) {
    n_write = (struct log_ptr_t){-1, 0xff};
    n_read = (struct log_ptr_t){-1, 0};
    // read in all the pages' headers, noting the first empty one for writing
    for (int i = 0; i < N_LOG_PAGES; i++) {
        read_header(i, _log + i);
        if (n_write.page < 0 && _log[i].seqnum_base == 0xffff) {
            n_write.page = i;
        }
        if (_log[i].seqnum_base != 0xffff && _log[i].status == 0x00) {
            // overflowed-via-reset, fix the header
            _log[i].status = HEADER_INIT | HEADER_RESET;
            // also, calculate the last written logline
            int lo = 0, hi = 126;
            while (hi - lo > 1) {
                int mid = (hi + lo) / 2;
                if (flash_peek(log_addr(i, mid) + 2) != LOG_UNWRITTEN) {
                    lo = mid;
                } else {
                    hi = mid;
                }
            }
            _log[i].last_seqnum = lo;
            finalize_header(_log + i, i);
        }
    }

    if (n_write.page >= 0) {
        // we've found an empty page we can init and write to
        if (n_write.page == 0) {
            //  go backwards to find the last written page, from where we can
            // correct for wrap issues if they exist
            for (int i = N_LOG_PAGES - 1; i >= 0; i--) {
                if (_log[i].status != 0) {
                    n_write.page = (i + 1) % N_LOG_PAGES;
                    break;
                }
            }
        }
        //  Find the next non-empty page after our empty one. That's where we
        // start reading from.
        for (int i = n_write.page + 1; i < n_write.page + N_LOG_PAGES; i++) {
            if (_log[i % N_LOG_PAGES].status != 0) {
                n_read.page = i % N_LOG_PAGES;
                break;
            }
        }
        //  If there's a previous page, we continue seqnum from there
        header_t *h = _log + ((n_write.page + N_LOG_PAGES - 1) % N_LOG_PAGES);
        if (h->status != 0) {
            _seqnum = h->seqnum_base + h->last_seqnum + 1;
        } else {
            _seqnum = 0;
        }
        // init the write header properly
        h = _log + n_write.page;
        h->seqnum_base = _seqnum;
        h->timestamp_offset = systime().sec;
        h->status = HEADER_INIT;
        h->last_seqnum = 0xff;
    }
    // n_write.page = page for next write, or -1 if full

    if (n_write.page < 0) {
        //  in the case that our log was full, prepare to start reading out at
        // the lowest seqnum we've logged.
        uint16_t oldest = 0xffff;
        for (int i = 0; i < N_LOG_PAGES; i++) {
            if ((_log[i].status & HEADER_INIT)
                && (_log[i].seqnum_base < oldest)) {
                oldest = _log[i].seqnum_base;
                n_read.page = i;
            }
        }
    }
    // n_read.page = page for next read (oldest logline), or -1 if empty
}

static void write_header(header_t *h, int npage) {
    assert(0 <= npage && npage < N_LOG_PAGES);
    //TODO just make header packed properly?
    uint8_t buff[] = {h->seqnum_base >> 8, h->seqnum_base & 0xff,
                      h->timestamp_offset >> 24, h->timestamp_offset >> 16,
                      h->timestamp_offset >> 8, h->timestamp_offset & 0xff};
    // don't do status or last_seqnum, that's handled by finalize_header
    flash_write(buff, header_addr(npage), 6);
}

static void finalize_header(header_t *h, int npage) {
    assert(0 <= npage && npage < N_LOG_PAGES);
    uint8_t buff[] = {~h->status, h->last_seqnum};
    flash_write(buff, header_addr(npage) + 6, 2);
}

static void read_header(int npage, header_t *h) {
    assert(0 <= npage && npage < N_LOG_PAGES);
    uint8_t buff[8];
    flash_read(header_addr(npage), buff, 8);
    h->seqnum_base = ((uint16_t)buff[0] << 8) | buff[1];
    h->timestamp_offset = (((uint32_t)buff[2] << 24) 
                           | ((uint32_t)buff[3] << 16)
                           | ((uint32_t) buff[4] << 8)
                           | buff[5]);
    h->status = ~buff[6];
    h->last_seqnum = buff[7];
}

static int32_t _logline(abs_time_t *when, log_type_e what, uint8_t *details) {
    if (n_write.page < 0) {
        // we've got nowhere to write
        return -1;
    }
    header_t *h = _log + n_write.page;
        
    if (when->sec < h->timestamp_offset
        || (when->sec - h->timestamp_offset) > 0xffff) {
        // that first situation reeks of someone messing with timekeeping
        // the second would be we've overtimed this page
        if (when->sec > systime().sec + 1) {
            // sanity check: we're not going to serve time travellers
            return -2;
        }
        if (n_write.offset == 0xff) {
            // we never actually used this header. re-init and use it
            h->seqnum_base = _seqnum;
            h->timestamp_offset = systime().sec;
            h->status = HEADER_INIT;
            h->last_seqnum = 0xff;
        } else {
            // we've timed out and need to flush this header
            h->status |= HEADER_TIMEOUT;
            finalize_header(h, n_write.page);
        }
    }
    if (_seqnum > h->seqnum_base + 126) {
        // we're overflowing the current page, set up the next one
        h->status |= HEADER_OVF;
        // FIXME not sure what to do with both OVF and TIMEOUT
        finalize_header(h, n_write.page);
    }
    if (h->status != HEADER_INIT) {
        //  page has been closed out for some reason. What the reason is doesn't
        // matter to us, only that we can't write here.
        if (_log[(n_write.page + 1 ) % N_LOG_PAGES].status == 0) {
            n_write.page = (n_write.page + 1) % N_LOG_PAGES;
            n_write.offset = 0xff;
            h = _log + n_write.page;
            h->seqnum_base = _seqnum;
            h->timestamp_offset = when->sec;
            h->status = HEADER_INIT;
            h->last_seqnum = 0xff;
        } else {
            // The next page has already been used, we're probably full up
            return -1;
        }
    }
    if (n_write.offset == 0xff) {
        // first line into this page, let's write the header first
        write_header(h, n_write.page);
        h->last_seqnum = 0;
        n_write.offset = 0;
    }

    // ok we're in a good spot now
    uint32_t tstamp = when->sec - h->timestamp_offset;
    uint8_t buff[] = {(tstamp >> 8) & 0xff, tstamp & 0xff,
                      what,
                      details[0], details[1], details[2], details[3],
                      0};
    buff[7] = crc(buff, 7);
    h->last_seqnum = n_write.offset;
    flash_write(buff, log_addr(n_write.page, n_write.offset), 8);
    if (n_read.page < 0) {
        n_read.page = n_write.page;
        n_read.offset = n_write.offset;
    }
    n_write.offset++;
    return _seqnum++;
}

int32_t logger_log_iv(abs_time_t *when, log_type_e what,
                       uint16_t ma, uint16_t mv) {
    uint8_t payload[4] = {ma >> 8, ma & 0xff, mv >> 8, mv & 0xff};
    return _logline(when, what, payload);
}

int32_t logger_log_stat(abs_time_t *when, log_type_e what, uint8_t stat_flags) {
    uint8_t payload[4] = {0, 0, 0, stat_flags};
    return _logline(when, what, payload);
}

int32_t logger_log_temp_stat(abs_time_t *when, log_type_e what, int16_t temp,
                             uint8_t stat_flags) {
    uint8_t payload[4] = {temp >> 8, temp & 0xff, 0, stat_flags};
    return _logline(when, what, payload);
}

int32_t logger_log_user(abs_time_t *when, uint32_t msg) {
    uint8_t payload[4] = {(msg >> 24) & 0xff, (msg >> 16) & 0xff,
                          (msg >> 8) & 0xff, msg & 0xff};
    return _logline(when, LOG_USER_MSG, payload);
}

int32_t logger_log_timesync(abs_time_t *when, int32_t delta_ms) {
    uint8_t payload[4] = {(delta_ms >> 24) & 0xff, (delta_ms >> 16) & 0xff,
                          (delta_ms >> 8) & 0xff, delta_ms & 0xff};
    return _logline(when, LOG_TIME_SYNC, payload);
}

int32_t logger_log_error(abs_time_t *when, uint8_t err) {
    uint8_t payload[4] = {0, 0, 0, err};
    return _logline(when, LOG_ERR, payload);
}

static int logline_read(uint8_t page, uint8_t offset, log_msg_t *buffer) {
    header_t *h = _log + page;
    uint8_t buff[8];
    flash_read(log_addr(page, offset), buff, 8);
    if (buff[2] != LOG_UNWRITTEN && buff[2] != LOG_ERASED) {
        buffer->seqnum = h->seqnum_base + offset;
        buffer->timestamp = (h->timestamp_offset + ((uint32_t)buff[0] << 8)
                             + ((uint32_t)buff[1] & 0xff));
        buffer->type = buff[2];
        for (int j = 0; j < 4; j++) {
            buffer->payload[j] = buff[3 + j];
        }
        return 0;
    }
    return -1;
}

int logger_read(uint16_t seqnum, log_msg_t *buffer) {
    for (int i = 0; i < N_LOG_PAGES; i++) {
        header_t *h = _log + i;
        if (h->status & HEADER_INIT) {
            uint16_t offset = seqnum - h->seqnum_base;
            if (seqnum >= h->seqnum_base && offset <= h->last_seqnum) {
                // that seqnum should be in this page
                return logline_read(i, offset, buffer);
            }
        }
    }
    return -1;
}

int logger_dequeue(log_msg_t *buffer) {
    if (n_read.page < 0) {
        return -1;
    }

    //  Find the next logline to read, skipping over unwritten or erased
    // loglines, stopping when we've caught up to the spot for the next logline
    // that will be written.
    while ((n_read.page != n_write.page)
           || (n_read.page == n_write.page && n_read.offset < n_write.offset)) {
        uint8_t type = flash_peek(log_addr(n_read.page, n_read.offset) + 2);
        if (type != LOG_UNWRITTEN && type != LOG_ERASED) {
            break;
        }
        if (++n_read.offset >= 127) {
            // We've scanned through a page, erase it and update header in _log
            flash_erase(n_read.page);
            _log[n_read.page] = (header_t){0xffff, 0xffffffff, 0, 0xff};
            if (n_write.page < 0) {
                //  We were blocking writes, because we had nowhere to go. Open
                // up for writes again.
                n_write.page = n_read.page;
                n_write.offset = 0xff;
            }

            // look for the next page we can read from
            bool found = false;
            for (int i = 0; i < N_LOG_PAGES; i++) {
                int npage = (n_read.page + i) % N_LOG_PAGES;
                if (_log[npage].status & HEADER_INIT) {
                    n_read.page = npage;
                    n_read.offset = 0;
                    found = true;
                    break;
                }
            }
            if (!found) {
                // flag that we've got nothing left to read
                n_read.page = -1;
                n_read.offset = 0;
                return -1;
            }
        }
    }

    int retval = logline_read(n_read.page, n_read.offset, buffer);
    if (retval == 0) {
        // erase that line from the log
        uint8_t zeros[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        flash_write(zeros, log_addr(n_read.page, n_read.offset), 8);
    }
    n_read.offset++;
    if (n_read.page != n_write.page) {
        if (n_read.offset >= 127
            || n_read.offset > _log[n_read.page].last_seqnum) {
            // clear out that log entry, erase the page
            flash_erase(n_read.page);
            _log[n_read.page] = (header_t){0xffff, 0xffffffff, 0, 0xff};
            if (n_write.page < 0) {
                n_write.page = n_read.page;
                n_write.offset = 0xff;
            }
            // advance to the next page
            bool found = false;
            for (int i = 1; i < N_LOG_PAGES; i++) {
                int npage = (n_read.page + i) % N_LOG_PAGES;
                if (_log[npage].status & HEADER_INIT) {
                    n_read.page = npage;
                    found = true;
                    break;
                }
            }
            if (!found) {
                n_read.page = -1;
            }
            n_read.offset = 0;
        }
    } else {
        if (n_read.offset == n_write.offset) {
            n_read.page = -1;
            n_read.offset = 0;
        }
    }
    return retval;
}
