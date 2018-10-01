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

static struct header_t {
     uint16_t seqnum_base;
     uint32_t timestamp_offset;
     uint8_t status; // bitmask of header_status_e flags
     uint8_t reserved;
} _header;

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

static void write_header(struct header_t *h, int npage);
static void read_header(int npage, struct header_t *h);

//FIXME real crc.
static uint8_t crc(uint8_t *buff, uint8_t len) {
    return 0;
}

static int first_empty_logpage(void) {
    //  go forward until you hit an empty one, then back up to get the last full
    // one. If you don't hit an empty one, sorry?
    struct header_t head;
    for (int i = 0; i < N_LOG_PAGES; i++) {
        read_header(i, &head);
        if (head.status == 0) {
            // found an empty one
            if (i > 0) {
                return i;
            } else {
                for (int j = N_LOG_PAGES - 1; j > 0; j--) {
                    read_header(j, &head);
                    if (head.status & HEADER_INIT) {
                        // now we're at the last filled one
                        // FIXME set the buffer_filled_with_reset bit
                        return (j == N_LOG_PAGES - 1 ? 0 : j + 1);
                    }
                }
                // they are all empty
                return 0;
            }
        }
    }
    return -1;
}

int logger_init(void) {
    int npage = first_empty_logpage();
    if (npage < 0) {
        return npage;
    }
    _page = npage;
    _seqnum = 0;
    read_header(_page, &_header);
    return 0;
}

int logger_initpage(uint16_t start_seqnum) {
    if (_header.status & HEADER_INIT) {
        return -1;
    }
    if (start_seqnum > 0xffff - 127) {
        return -2;
    }
    _seqnum = start_seqnum;
    abs_time_t now = systime();

    _header.seqnum_base = _seqnum;
    _header.timestamp_offset = now.sec;
    _header.status = HEADER_INIT;
    write_header(&_header, _page);

    return 0;
}

static void write_header(struct header_t *h, int npage) {
    assert(0 <= npage && npage < N_LOG_PAGES);
    //TODO just make header packed properly?
    uint8_t buff[] = {h->seqnum_base >> 8, h->seqnum_base & 0xff,
                      h->timestamp_offset >> 24, h->timestamp_offset >> 16,
                      h->timestamp_offset >> 8, h->timestamp_offset & 0xff,
                      ~h->status,
                      0xff};
    flash_write(buff, LOG_BASE + npage * FLASH_PAGESIZE, 8);
}

static void read_header(int npage, struct header_t *h) {
    assert(0 <= npage && npage < N_LOG_PAGES);
    uint8_t buff[8];
    flash_read(LOG_BASE + npage * FLASH_PAGESIZE, buff, 8);
    h->seqnum_base = ((uint16_t)buff[0] << 8) | buff[1];
    h->timestamp_offset = (((uint32_t)buff[2] << 24) 
                           | ((uint32_t)buff[3] << 16)
                           | ((uint32_t) buff[4] << 8)
                           | buff[5]);
    h->status = ~buff[6];
    h->reserved = 0;
}

uint16_t logger_len(void) {
    return 0;
}

static int32_t _logline(abs_time_t *when, log_type_e what, uint8_t *details) {
    if (_header.status == 0) {
        // header needs to be init'd
        return -1;
    }
    if (_header.status == HEADER_INIT
        && (when->sec < _header.timestamp_offset
            || (when->sec + _header.timestamp_offset) > 0xffff)) {
        // that first situation reeks of someone messing with timekeeping
        // the second would be we've overtimed this page
        // sanity check: we're not going to serve time travellers
        if (when->sec > systime().sec + 1) {
            return -2;
        }
        _header.status |= HEADER_TIMEOUT;
        write_header(&_header, _page);
    }
    if (_header.status != HEADER_INIT) {
        //  page has been closed out for some reason. What the reason is doesn't
        // matter to us, only that we can't write here.
        // advance to next page
        _page = (_page + 1) % N_LOG_PAGES;
        read_header(_page, &_header);
        int err = logger_initpage(_seqnum);
        if (err) {
            // we assume we've filled up the logs entirely
            return err * 4;
        }
    }

    // ok we're in a good spot now
    uint32_t tstamp = when->sec - _header.timestamp_offset;
    uint8_t buff[] = {(tstamp >> 8) & 0xff, tstamp & 0xff,
                      what,
                      details[0], details[1], details[2], details[3],
                      0};
    buff[7] = crc(buff, 7);
    uint32_t offset = (_seqnum - _header.seqnum_base + 1) * 8;
    uint32_t addr = LOG_BASE + _page * FLASH_PAGESIZE + offset;
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
        struct header_t h;
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
