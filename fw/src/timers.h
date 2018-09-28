#ifndef TIMERS_H
#define TIMERS_H

#include <stdint.h>

// gives us over 49.7K days before overflow, but with ms precision.
typedef struct abs_time_t {
    uint32_t sec;
    uint16_t ms;
} abs_time_t;

void timers_init(void);
abs_time_t systime(void);
uint32_t ms_elapsed(abs_time_t *from, abs_time_t *to);

#endif // TIMERS_H
