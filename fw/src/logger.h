#ifndef LOGGER_H
#define LOGGER_H

#include "./timers.h"

#include <stdint.h>

typedef enum log_type_e {
    LOG_ERASED = 0,
    LOG_IV_CHG_BAT0,
    // ...
    LOG_UNWRITTEN = 0xff
} log_type_e;

//TODO make this packed
typedef struct log_msg_t {
    uint16_t seqnum;
    uint32_t timestamp;
    uint8_t type;
    uint8_t payload[4];
} log_msg_t;

void logger_init(void);
uint16_t logger_len(void);
int32_t logger_log_iv(abs_time_t *when, log_type_e what,
                      uint16_t ma, uint16_t mv);
int logger_read(uint16_t seqnum, log_msg_t *buffer);
void logger_payload_to_ma_mv(uint8_t *payload, uint16_t *ma, uint16_t *mv);

#endif // LOGGER_H
