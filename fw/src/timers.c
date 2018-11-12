#include "./timers.h"

#include <assert.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

static volatile uint32_t _sec;
static volatile uint16_t _ms;

void timers_init(void) {
    rcc_clock_setup_in_hse_8mhz_out_48mhz();
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB); // ~48MHz
    systick_set_reload(48000); // systick at 1kHz
    _ms = 0;
    _sec = 0;
    systick_clear();

    // TIM14 for loop tick/tock
    rcc_periph_clock_enable(RCC_TIM14);
    rcc_periph_reset_pulse(RST_TIM14);
    timer_set_prescaler(TIM14, 96);
    timer_disable_preload(TIM14);
    timer_one_shot_mode(TIM14);
    //  Calling tick() the first time doesn't work unless the timer has already
    // been started here. I assume it's because of calling timer_disable_counter
    // before calling timer_enable_counter, but don't know why it works the next
    // time around. We'll start it off but force it to roll-over right away so
    // that we still need a valid tick before calling tock.
    timer_set_counter(TIM14, 0xff);
    timer_enable_counter(TIM14);

    // TIM16 for general purpose tick/tock
    rcc_periph_clock_enable(RCC_TIM16);
    rcc_periph_reset_pulse(RST_TIM16);
    timer_set_prescaler(TIM16, 96);
    timer_disable_preload(TIM16);
    timer_one_shot_mode(TIM16);
    timer_set_counter(TIM16, 0xff);
    timer_enable_counter(TIM16);

    systick_interrupt_enable();
    systick_counter_enable();
}

void timers_set_systime(uint32_t sec, uint16_t ms) {
    sec += ms / 1000;
    ms %= 1000;
    _sec = sec;
    _ms = ms;
}

void sys_tick_handler(void) {
    if (_ms == 999) {
        _ms = 0;
        _sec++;
    } else {
        _ms++;
    }
}

abs_time_t systime(void) {
    uint16_t ms = _ms;
    uint32_t sec = _sec;
    if (ms != _ms) {
        //  very unlucky, the sys_tick_handler fired. There's the edge case
        // where (sec, ms) is (t+1, 999), but should be either (t, 999) or
        // (t+1, 0), depending on whether you want before or after the handler
        // fired (doesn't matter to me). We really shouldn't have ISRs stacked
        // up to take up a whole ms of processor time, so we'll just re-do
        // systime() and trust that we're not going to recurse more than once.
        return systime();
    }
    return (abs_time_t){sec, ms};
}

uint32_t ms_elapsed(abs_time_t *from, abs_time_t *to) {
    assert(to->sec >= from->sec);
    assert(to->ms >= from->ms || from->sec < to->sec);
    uint32_t retval = to->sec - from->sec;
    if (retval >= 0xffffffff / 1001) {
        return 0xffffffff;
    } else {
        return retval * 1000 + to->ms - from->ms;
    }
}

static inline uint32_t timer_for_ticker(ticker_e ticker) {
    switch (ticker) {
    case TIMER_LOOP:
        return TIM14;
    case TIMER_GEN:
        return TIM16;
    default:
        return 0;
    }
}

void tick(ticker_e ticker) {
    uint32_t tim = timer_for_ticker(ticker);
    if (tim == 0) {
        return;
    }
    timer_disable_counter(tim);
    timer_set_counter(tim, 0);
    timer_enable_counter(tim);
}

uint32_t tock(ticker_e ticker) {
    uint32_t tim = timer_for_ticker(ticker);
    if (tim == 0) {
        return 0;
    }
    return timer_get_counter(tim) * 2;
}
