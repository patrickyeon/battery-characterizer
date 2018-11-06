#include "../driver/at30ts74.h"
#include "./adc.h"
#include "./commands.h"
#include "./director.h"
#include "./gpio.h"
#include "./logger.h"
#include "./pindefs.h"
#include "./timers.h"
#include "../driver/usb.h"
#include "../driver/flash.h"

static uint8_t crc(uint8_t *buff, int len) {
    //FIXME real crc
    return 0;
}

#define RLEN 10
#define CLEN 11

#define UNPACK4(buffer, start) ((buffer[start] << 24)       \
                                | (buffer[start + 1] << 16) \
                                | (buffer[start + 2] << 8)  \
                                | (buffer[start + 3]))
#define UNPACK2(buffer, start) ((buffer[start] << 8) \
                                | (buffer[start+1] & 0xff))

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
    int err;
    uint32_t u32;
    int32_t i32;
    uint16_t u16;
    int16_t i16;
    log_msg_t log_msg;
    abs_time_t now = systime();
    switch (cmdbuff[1]) {

    case CMD_PING:
        resp[1] = CMD_PONG;
        usb_write(resp, RLEN);
        break;

    case CMD_TAG:
        u32 = UNPACK4(cmdbuff, 2);
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

    case CMD_TIME_SET:
        u32 = UNPACK4(cmdbuff, 2);
        u16 = UNPACK2(cmdbuff, 6);
        timers_set_systime(u32, u16);
        now = systime();
        // falling through on purpose
    case CMD_TIME_GET:
        resp[1] = CMD_TIME_SET;
        resp[2] = (now.sec >> 24) & 0xff;
        resp[3] = (now.sec >> 16) & 0xff;
        resp[4] = (now.sec >> 8) & 0xff;
        resp[5] = now.sec & 0xff;
        resp[6] = (now.ms >> 8) & 0xff;
        resp[7] = now.ms & 0xff;
        usb_write(resp, RLEN);
        break;

    case CMD_DEQUEUE_LOG:
        err = logger_dequeue(&log_msg);
        if (err < 0) {
            resp[1] = CMD_NAK;
            resp[2] = err & 0xff;
        } else {
            resp[1] = CMD_ACK;
            resp[2] = log_msg.seqnum >> 8;
            resp[3] = log_msg.seqnum & 0xff;
            resp[4] = log_msg.type;
            resp[5] = log_msg.payload[0];
            resp[6] = log_msg.payload[1];
            resp[7] = log_msg.payload[2];
            resp[8] = log_msg.payload[3];
        }
        usb_write(resp, RLEN);
        break;

    case CMD_WIPE_LOG:
        for (int i = 0; i < 3; i++) {
            flash_erase(i);
        }
        resp[1] = CMD_ACK;
        usb_write(resp, RLEN);
        break;

    case CMD_FLASH_PEEK:
        u32 = UNPACK4(cmdbuff, 2);
        if (cmdbuff[6] > 7 || u32 < 0x0800000 || u32 >= 0x08008000) {
            resp[1] = CMD_NAK;
            resp[2] = CMD_FLASH_PEEK;
        } else {
            resp[1] = CMD_FLASH_PEEK;
            for (int i = 0; i < cmdbuff[6]; i++) {
                resp[i + 2] = flash_peek(u32 + i);
            }
        }
        usb_write(resp, RLEN);
        break;

    case CMD_FLASH_READ:
        u32 = UNPACK4(cmdbuff, 2);
        if (cmdbuff[6] > 7 || u32 < 0x0800000 || u32 >= 0x08008000) {
            resp[1] = CMD_NAK;
            resp[2] = CMD_FLASH_READ;
        } else {
            resp[1] = CMD_FLASH_READ;
            flash_read(u32, resp + 2, cmdbuff[6]);
        }
        usb_write(resp, RLEN);
        break;

    case CMD_ADC_SCAN:
        adc_scan();
        resp[1] = CMD_ADC_SCAN;
        usb_write(resp, RLEN);
        break;

    case CMD_ADC_READ:
        u16 = adc_read(cmdbuff[2]);
        resp[1] = CMD_ADC_READ;
        resp[2] = (uint8_t)(u16 >> 8);
        resp[3] = u16 & 0xff;
        usb_write(resp, RLEN);
        break;

    case CMD_TEMP_READ:
        i16 = at30ts74_read(cmdbuff[2]);
        resp[1] = CMD_TEMP_READ;
        resp[2] = (uint8_t)((i16 >> 8) & 0xff);
        resp[3] = (uint8_t)(i16 & 0xff);
        usb_write(resp, RLEN);
        break;

    case CMD_CENDEN_SET:
        director_direction(cmdbuff[2] & 0x1 ? CHG_DISCHG : DISCHG_CHG,
                           cmdbuff[2] & 0x2 ? CHG_DISCHG : DISCHG_CHG);
        if (cmdbuff[3] & 0x1) {
            director_enable(CENA);
        } else {
            director_disable(CENA);
        }
        if (cmdbuff[3] & 0x2) {
            director_enable(DENA);
        } else {
            director_disable(DENA);
        }
        if (cmdbuff[3] & 0x4) {
            director_enable(CENB);
        } else {
            director_disable(CENB);
        }
        if (cmdbuff[3] & 0x8) {
            director_enable(DENB);
        } else {
            director_disable(DENB);
        }
        resp[1] = CMD_CENDEN_SET;
        usb_write(resp, RLEN);
        break;

    case CMD_CURRENT_SET:
        u16 = UNPACK2(cmdbuff, 3);
        switch (cmdbuff[2]) {
        case 0:
            u32 = SET_ID_A;
            break;
        case 1:
            u32 = SET_ID_B;
            break;
        case 2:
            u32 = SET_IC_A;
            break;
        case 3:
            u32 = SET_IC_B;
            break;
        default:
            u32 = 0;
        }
        if (u32) {
            pwm_out(u32, u16);
            resp[1] = CMD_CURRENT_SET;
        } else {
            resp[1] = CMD_NAK;
            resp[2] = CMD_CURRENT_SET;
        }
        usb_write(resp, RLEN);
        break;

    case CMD_SET_LOG_PERIOD:
        u16 = UNPACK2(cmdbuff, 2);
        if (u16 == 0) {
            resp[1] = CMD_NAK;
        } else {
            director_log_rate(u16);
            resp[1] = CMD_SET_LOG_PERIOD;
            resp[2] = (u16 >> 8) & 0xff;
            resp[3] = u16 & 0xff;
        }
        usb_write(resp, RLEN);
        break;

    case CMD_LOG_EN_DIS:
        if (cmdbuff[2] == 1) {
            director_log_start();
            resp[1] = CMD_ACK;
        } else if (cmdbuff[2] == 2) {
            director_log_stop();
            resp[1] = CMD_ACK;
        } else {
            resp[1] = CMD_NAK;
        }
        usb_write(resp, RLEN);
        break;

    default:
        resp[1] = CMD_NAK;
        resp[2] = cmdbuff[1];
        usb_write(resp, RLEN);
    }
}
