#ifndef LOGGER_H
#define LOGGER_H

#include "./timers.h"

#include <stdint.h>

// the logger will have a very short list of things it can do, to reduce
// complexity:
// - log a thing (a logline)
// - read an arbitrary logline, indexed by seqnum
// - read and delete the oldest logline
// - wipe the whole thing and start over

typedef enum log_type_e {
    LOG_ERASED = 0,

    LOG_IV_CHG_BAT0,
    LOG_IV_DCH_BAT0,
    LOG_STAT_BAT0,
    LOG_TEMPSTAT_BAT0,

    LOG_IV_CHG_BAT1,
    LOG_IV_DCH_BAT1,
    LOG_STAT_BAT1,
    LOG_TEMPSTAT_BAT1,

    LOG_IV_CHG_BAT2,
    LOG_IV_DCH_BAT2,
    LOG_STAT_BAT2,
    LOG_TEMPSTAT_BAT2,

    LOG_IV_CHG_BAT3,
    LOG_IV_DCH_BAT3,
    LOG_STAT_BAT3,
    LOG_TEMPSTAT_BAT3,

    LOG_USER_MSG,
    LOG_TIME_SYNC,
    LOG_ERR,

    LOG_UNWRITTEN = 0xff
} log_type_e;

typedef struct log_msg_t {
    uint16_t seqnum;
    uint32_t timestamp;
    uint8_t type;
    uint8_t payload[4];
} log_msg_t;

void logger_init(void);

int32_t logger_log_iv(abs_time_t *when, log_type_e what,
                      uint16_t ma, uint16_t mv);
int32_t logger_log_stat(abs_time_t *when, log_type_e what, uint8_t stat_flags);
int32_t logger_log_temp_stat(abs_time_t *when, log_type_e what, int16_t temp,
                             uint8_t stat_flags);
int32_t logger_log_user(abs_time_t *when, uint32_t msg);
int32_t logger_log_timesync(abs_time_t *when, int32_t delta_ms);
int32_t logger_log_error(abs_time_t *when, uint8_t err);

int logger_read(uint16_t seqnum, log_msg_t *buffer);
int logger_dequeue(log_msg_t *buffer);

#endif // LOGGER_H
