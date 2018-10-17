#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>

#define CMD_STARTBYTE 0xA5

typedef enum cmd_msg_e {
    CMD_NAK = 0,
    CMD_ACK,
    CMD_PING,
    CMD_PONG,
    CMD_TAG, //4
    CMD_TIME_GET,
    CMD_TIME_SET,
    CMD_DEQUEUE_LOG,
    CMD_WIPE_LOG, //8
    CMD_FLASH_PEEK,
    CMD_FLASH_READ,
    CMD_ADC_READ,
    CMD_TEMP_READ, //c
    CMD_CENDEN_SET,
    CMD_ADC_SCAN,
    CMD_RESERVED = CMD_STARTBYTE
} cmd_msg_e;

// commands are all the same size, have the following structure
// {0xA5, cmd_msg_e, payload (8 bytes), checksum (1 byte)}

void commands_init(void);
void commands_process(void);

#endif // COMMANDS_H
