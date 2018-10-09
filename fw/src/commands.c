#include "./commands.h"
#include "./logger.h"
#include "./timers.h"
#include "../driver/usb.h"

static uint8_t crc(uint8_t *buff, int len) {
    //FIXME real crc
    return 0;
}

#define RLEN 10
#define CLEN 8

static uint8_t cmdbuff[CLEN];
static unsigned int cmdidx;

void commands_init(void) {
    for (int i = 0; i < CLEN; i++) {
        cmdbuff[i] = 0;
    }
    cmdidx = 0;
}

void commands_process(void) {
    while (cmdidx == 0 && usb_readlen() > 0 && usb_peek() != CMD_STARTBYTE) {
        // get it out of there
        uint8_t devnull;
        usb_read(&devnull, 1);
    }
    // do we have enough bytes to have a command?
    if ((usb_readlen() + cmdidx) < CLEN) {
        return;
    }
    // so we've seen the start signal, is it actually a command?
    usb_read(cmdbuff + cmdidx, CLEN - cmdidx);
    cmdidx = 0;
    if (crc(cmdbuff + 1, CLEN - 2) != cmdbuff[CLEN - 1]) {
        //  CRC error, or it wasn't a real start byte. Look for another start
        // byte.
        for (int i = 1; i < CLEN; i++) {
            if (cmdbuff[i] == CMD_STARTBYTE) {
                for (int j = i; j < CLEN; j++) {
                    cmdbuff[j - i] = cmdbuff[j];
                }
                cmdidx = CLEN - i;
                return;
            }
        }
        return;
    }
    uint8_t resp[RLEN] = {CMD_STARTBYTE};
    uint32_t u32;
    int32_t i32;
    abs_time_t now = systime();
    switch (cmdbuff[1]) {
    case CMD_PING:
        resp[1] = CMD_PONG;
        usb_write(resp, RLEN);
        break;
    case CMD_TAG:
        u32 = ((cmdbuff[2] << 24) | (cmdbuff[3] << 16) | (cmdbuff[4] << 8)
               | (cmdbuff[5] & 0xff));
        i32 = logger_log_user(&now, u32);
        if (i32 >= 0) {
            resp[1] = CMD_TAG;
            resp[2] = (i32 >> 8) & 0xff;
            resp[3] = i32 & 0xff;
        } else {
            resp[1] = CMD_NAK;
            resp[2] = CMD_TAG;
        }
        usb_write(resp, RLEN);
        break;
    case CMD_TIME:
        resp[1] = CMD_TIME;
        resp[2] = (now.sec >> 24) & 0xff;
        resp[3] = (now.sec >> 16) & 0xff;
        resp[4] = (now.sec >> 8) & 0xff;
        resp[5] = now.sec & 0xff;
        resp[6] = (now.ms >> 8) & 0xff;
        resp[7] = now.ms & 0xff;
        usb_write(resp, RLEN);
        break;
    default:
        resp[1] = CMD_NAK;
        resp[2] = cmdbuff[1];
        usb_write(resp, RLEN);
    }
}
