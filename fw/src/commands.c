#include "./commands.h"
#include "../driver/usb.h"

static uint8_t crc(uint8_t *buff, int len) {
    //FIXME real crc
    return 0;
}

static uint8_t cmdbuff[8];
static unsigned int cmdidx;

void commands_init(void) {
    for (int i = 0; i < 8; i++) {
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
    if ((usb_readlen() + cmdidx) < 8) {
        return;
    }
    // so we've seen the start signal, is it actually a command?
    usb_read(cmdbuff + cmdidx, 8 - cmdidx);
    cmdidx = 0;
    if (crc(cmdbuff + 1, 6) != cmdbuff[7]) {
        //  CRC error, or it wasn't a real start byte. Look for another start
        // byte.
        for (int i = 1; i < 8; i++) {
            if (cmdbuff[i] == CMD_STARTBYTE) {
                for (int j = i; j < 8; j++) {
                    cmdbuff[j - i] = cmdbuff[j];
                }
                cmdidx = 8 - i;
                return;
            }
        }
        return;
    }
    uint8_t resp[8] = {0xA5, 0, 0, 0, 0, 0, 0, 0};
    switch (cmdbuff[1]) {
    case CMD_PING:
        resp[1] = RESP_PONG;
        usb_write(resp, 8);
        break;
    case CMD_TAG:
        // TODO log a tag
        resp[1] = RESP_ACK;
        resp[2] = CMD_TAG;
        usb_write(resp, 8);
        break;
    default:
        resp[1] = RESP_NAK;
        resp[2] = cmdbuff[1];
        usb_write(resp, 8);
    }
}
