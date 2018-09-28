#include "./timers.h"

#include <assert.h>

#include <libopencm3/stm32/rcc.h>
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
    systick_interrupt_enable();
    systick_counter_enable();
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
    //  _ms counts from 0 to 2000, _sec just counts up. This way we can handle
    // the edge case where _ms and _sec get incremented in between us reading
    // the two.
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
