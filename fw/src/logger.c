#include "./logger.h"
#include "../driver/flash.h"

#include <assert.h>

//FIXME real addresses
#define LOG_BASE 0
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
static header_t _header;

// Reconstructing the seqnum and timestamp of a log item is:
//   uint32_t seconds_since_epoch = timestame_offset + timestamp;
//   uint16_t seqnum = seqnum_base + (&logline - &header) / 8 - 1;
// seqnum_base can be from 0..(65535-127), because we're not going to handle
// wrapping that. Furthermore, a seqnum_base of 0xffff is interpreted as "this
// page is erased and ready for use".
//
// We treat the log pages in flash as a big circular buffer, refusing to write
// on overflow.

static uint32_t _seqnum;
static int _page = -1;

static void write_header(header_t *h, int npage);
static void read_header(int npage, header_t *h);

static header_t _log[N_LOG_PAGES];

//FIXME real crc.
static uint8_t crc(uint8_t *buff, uint8_t len) {
    return 0;
}

static struct log_ptr_t {
    int page;
    uint8_t offset;
} n_write={-1, 0}, n_read = {-1, 0};

void logger_init(void) {
    n_write = (struct log_ptr_t){-1, 0};
    n_read = (struct log_ptr_t){-1, 0};
    for (int i = 0; i < N_LOG_PAGES; i++) {
        read_header(i, _log + i);
        if (n_write.page < 0 && _log[i].status == 0) {
            n_write.page = i;
        }
        if (_log[i].status == HEADER_INIT) {
            // overflowed-via-reset, fix the header
            _log[i].status |= HEADER_RESET;
            // also, calculate the last written logline
            int lo = 0, hi = 126;
            while (hi - lo > 1) {
                int mid = (hi + lo) / 2;
                if (flash_peek(LOG_BASE + i * FLASH_PAGESIZE + (mid + 1) * 8 + 3) != 0xff) {
                    lo = mid;
                } else {
                    hi = mid;
                }
            }
            _log[i].last_seqnum = lo;
            write_header(_log + i, i);
        }
    }
    if (n_write.page >= 0) {
        // we've found an empty page we can init and write to
        if (n_write.page == 0) {
            //  go backwards to find the last written page, from where we can
            // correct for wrap issues if they exist
            for (int i = N_LOG_PAGES - 1; i <= 0; i--) {
                if (_log[i].status != 0) {
                    n_write.page = (i + N_LOG_PAGES - 1) % N_LOG_PAGES;
                    break;
                }
            }
        }
        for (int i = n_write.page + 1; i < n_write.page + N_LOG_PAGES; i++) {
            if (_log[i % N_LOG_PAGES].status != 0) {
                n_read.page = i;
                break;
            }
        }
        header_t *h = _log + ((n_write.page + N_LOG_PAGES - 1) % N_LOG_PAGES);
        if (h->status != 0) {
            _seqnum = h->seqnum_base + h->last_seqnum;
        } else {
            _seqnum = 0;
        }
        h = _log + n_write.page;
        // init the header properly
        h->seqnum_base = _seqnum;
        h->timestamp_offset = systime().sec;
        h->status = HEADER_INIT;
        h->last_seqnum = 0xff;
    }
    // n_write.page = page for next write, or -1 if full

    uint16_t oldest = 0xffff;
    for (int i = 0; i < N_LOG_PAGES; i++) {
        if ((_log[i].status | HEADER_INIT) && (_log[i].seqnum_base < oldest)) {
            oldest = _log[i].seqnum_base;
            n_read.page = i;
        }
    }
    // n_read.page = page for next read (oldest logline), or -1 if empty
}

static void write_header(header_t *h, int npage) {
    assert(0 <= npage && npage < N_LOG_PAGES);
    //TODO just make header packed properly?
    uint8_t buff[] = {h->seqnum_base >> 8, h->seqnum_base & 0xff,
                      h->timestamp_offset >> 24, h->timestamp_offset >> 16,
                      h->timestamp_offset >> 8, h->timestamp_offset & 0xff,
                      ~h->status,
                      h->last_seqnum};
    flash_write(buff, LOG_BASE + npage * FLASH_PAGESIZE, 8);
}

static void read_header(int npage, header_t *h) {
    assert(0 <= npage && npage < N_LOG_PAGES);
    uint8_t buff[8];
    flash_read(LOG_BASE + npage * FLASH_PAGESIZE, buff, 8);
    h->seqnum_base = ((uint16_t)buff[0] << 8) | buff[1];
    h->timestamp_offset = (((uint32_t)buff[2] << 24) 
                           | ((uint32_t)buff[3] << 16)
                           | ((uint32_t) buff[4] << 8)
                           | buff[5]);
    h->status = ~buff[6];
    h->last_seqnum = buff[7];
}

uint16_t logger_len(void) {
    return 0;
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
        // sanity check: we're not going to serve time travellers
        if (when->sec > systime().sec + 1) {
            return -2;
        }
        if (n_write.offset == 0xff) {
            // we never actually used this header. re-init it
            h->seqnum_base = _seqnum;
            h->timestamp_offset = systime().sec;
            h->status = HEADER_INIT;
            h->last_seqnum = 0xff;
        } else {
            // we've timed out and need to flush this
            h->status |= HEADER_TIMEOUT;
            write_header(h, n_write.page);
        }
    }
    if (_seqnum > h->seqnum_base + 127) {
        // we're overflowing the current page, set up the next one
        h->status |= HEADER_OVF;
        write_header(h, n_write.page);
    }
    if (_header.status != HEADER_INIT) {
        //  page has been closed out for some reason. What the reason is doesn't
        // matter to us, only that we can't write here.
        //FIXME advance to next page
        _page = (_page + 1) % N_LOG_PAGES;
        read_header(_page, &_header);
        int err = 0;
        if (err) {
            // we assume we've filled up the logs entirely
            return err * 4;
        }
    }
    if (n_write.offset == 0xff) {
        // first line into this page, let's write the header first
        //  last_seqnum written as 0xff, so that we can overwrite it when we
        // actually finalize the page.
        h->last_seqnum = 0xff;
        write_header(h, n_write.page);
        h->last_seqnum = 0;
    }

    // ok we're in a good spot now
    uint32_t tstamp = when->sec - h->timestamp_offset;
    uint8_t buff[] = {(tstamp >> 8) & 0xff, tstamp & 0xff,
                      what,
                      details[0], details[1], details[2], details[3],
                      0};
    buff[7] = crc(buff, 7);
    uint32_t addr = LOG_BASE + _page * FLASH_PAGESIZE + n_write.offset++;
    flash_write(buff, addr, 8);
    return _seqnum++;
}

int32_t logger_log_iv(abs_time_t *when, log_type_e what,
                       uint16_t ma, uint16_t mv) {
    uint8_t payload[4] = {ma >> 8, ma & 0xff, mv >> 8, mv & 0xff};
    return _logline(when, what, payload);
}

int logger_read(uint16_t seqnum, log_msg_t *buffer) {
    // TODO maybe cache headers so that we don't re-read every time
    for (int i = 0; i < N_LOG_PAGES; i++) {
        header_t h;
        read_header(i, &h);
        if (h.status & HEADER_INIT) {
            uint16_t offset = seqnum - h.seqnum_base;
            if (seqnum >= h.seqnum_base && offset <= 127) {
                // that seqnum could conceivably be in this page
                uint8_t buff[8];
                uint32_t addr = LOG_BASE + i * FLASH_PAGESIZE + (offset + 1)* 8;
                flash_read(addr, buff, 8);
                if (buff[3] != LOG_UNWRITTEN) {
                    buffer->seqnum = seqnum;
                    buffer->timestamp = (h.timestamp_offset
                                         + ((uint32_t)buff[0] << 8) + buff[1]);
                    buffer->type = buff[2];
                    for (int j = 0; j < 4; j++) {
                        buffer->payload[j] = buff[3 + j];
                    }
                    return 0;
                }
            }
        }
    }
    return -1;
}

void logger_payload_to_ma_mv(uint8_t *payload, uint16_t *ma, uint16_t *mv) {
    *ma = ((uint16_t)(payload[0]) << 8) | payload[1];
    *mv = ((uint16_t)(payload[2]) << 8) | payload[3];
}
