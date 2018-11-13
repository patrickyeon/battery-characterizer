#ifndef TIMERS_H
#define TIMERS_H

#include <stdint.h>

// Timer assignments:
// - SYSTICK is system time (surprise.)
// - TIM3 controls PWM on 4 channels for current setting
//   (CH 1/2/3/4 --> PB4/5/0/1 --> ICA/ICB/IDA/IDB
// - TIM14 for tick/tock time tracking
//   With a 48MHz clock, prescaler = 96, we can run it for ~131ms before
//   overflow and have 2us precision

// gives us over 49.7K days before overflow, but with ms precision.
typedef struct abs_time_t {
    uint32_t sec;
    uint16_t ms;
} abs_time_t;

void timers_init(void);
void timers_set_systime(uint32_t sec, uint16_t ms);
abs_time_t systime(void);
uint32_t ms_elapsed(abs_time_t *from, abs_time_t *to);
abs_time_t time_add(abs_time_t *when, uint32_t delta);

typedef enum ticker_e {
    TIMER_LOOP, // used for main loop timing, do not use
    TIMER_GEN // general purpose, use wherever but only one "wherever" at a time
} ticker_e;

void tick(ticker_e ticker);
uint32_t tock(ticker_e ticker);

void watchdog_start(void);
void watchdog_pet(void);

#endif // TIMERS_H
