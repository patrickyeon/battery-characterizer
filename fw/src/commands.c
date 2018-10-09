#include "./commands.h"
#include "../driver/usb.h"

static uint8_t crc(uint8_t *buff, int len) {
    //FIXME real crc
    return 0;
}

void commands_process(void) {
    while (usb_readlen() > 0 && usb_peek() != CMD_STARTBYTE) {
        // get it out of there
        uint8_t devnull;
        usb_read(&devnull, 1);
    }
    // do we have enough bytes to have a command?
    if (usb_readlen() < 8) {
        return;
    }
    // so we've seen the start signal, is it actually a command?
    uint8_t buff[8];
    usb_read(buff, 8);
    if (crc(buff + 1, 6) != buff[7]) {
        // CRC error, or it wasn't a real start byte.
        // TODO deal with the mis-identified start problem
        return;
    }
    uint8_t resp[8] = {0xA5, 0, 0, 0, 0, 0, 0, 0};
    switch (buff[1]) {
    case CMD_PING:
        resp[1] = RESP_PONG;
        usb_write(resp, 8);
        break;
    default:
        resp[1] = RESP_NAK;
        resp[2] = buff[1];
        usb_write(resp, 8);
    }
}
