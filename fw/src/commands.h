#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>

#define CMD_STARTBYTE 0xA5

typedef enum cmd_msg_e {
    CMD_PING = 0,
    CMD_TAG,
    CMD_RESERVED = CMD_STARTBYTE
} cmd_msg_e;

typedef enum resp_msg_e {
    RESP_PONG = 0,
    RESP_NAK,
    RESP_ACK,
    RESP_RESERVED = CMD_STARTBYTE
} resp_msg_e;

// commands are all the same size, have the following structure
typedef struct cmd_t {
    cmd_msg_e type;
    uint8_t payload[6];
    uint8_t checksum;
} cmd_t;

void commands_init(void);
void commands_process(void);

#endif // COMMANDS_H
