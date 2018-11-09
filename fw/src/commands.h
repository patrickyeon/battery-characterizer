#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>

#define CMD_STARTBYTE 0xA5

typedef enum cmd_msg_e {
    CMD_NAK = 0,
    CMD_PING,
    CMD_PONG,
    CMD_CREATE_TAG,
    CMD_TAGGED, //4
    CMD_TIME_GET,
    CMD_TIME_SET,
    CMD_DEQUEUE_LOG,
    CMD_LOGLINE, //8
    CMD_WIPE_LOG, 
    CMD_FLASH_PEEK,
    CMD_FLASH_READ,
    CMD_FLASH_DATA, //c
    CMD_ADC_READ,
    CMD_TEMP_READ,
    CMD_CENDEN_SET,
    CMD_ADC_SCAN, //10
    CMD_CURRENT_SET,
    CMD_LOG_PERIOD_SET,
    CMD_LOG_EN_DIS,
    CMD_DEBUG, //14
    CMD_RESERVED = CMD_STARTBYTE
} cmd_msg_e;

// commands are all the same size, have the following structure
// {0xA5, cmd_msg_e, payload (8 bytes), checksum (1 byte)}

void commands_init(void);
void commands_process(void);

#endif // COMMANDS_H
