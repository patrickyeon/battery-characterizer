#include "./fake_time.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <assert.h>
#include <stdbool.h>

#define MAXINT_32 (0xffffffff)
#define NTIMERS 6

typedef struct timer_t {
    uint32_t count;
    uint32_t prescaler;
    uint32_t prescaler_count;
    uint32_t compare_chan[4];
    bool compare_chan_en[4];
    bool enabled;
} timer_t;

static timer_t timers[NTIMERS];

static int _idx(uint32_t timer_peripheral) {
    switch (timer_peripheral) {
    case TIM3:
        return 2;
    case TIM14:
        return 3;
    // TODO skipping a bunch because they aren't implemented yet
    case TIM1:
        //return 0;
    case TIM2:
        //return 1;
    case TIM16:
        //return 4;
    case TIM17:
        //return 5;
    default:
        assert(0);
    }
}

static uint32_t _hz = 0;

static void _timer_init(int idx) {
    timers[idx].count = 0;
    timers[idx].prescaler = 1;
    timers[idx].prescaler_count = 0;
    for (int i = 0; i < 4; i++) {
        timers[idx].compare_chan[i] = 0;
        timers[idx].compare_chan_en[i] = false;
    }
    timers[idx].enabled = false;
}

void fake_time_init(uint32_t clk_hz) {
    _hz = clk_hz;
    for (int i = 0; i < NTIMERS; i++) {
        _timer_init(i);
    }
}

void fake_time_run(uint32_t nticks) {
    // TODO handle when count hits the compare, if it should re-load, and
    //      one-shot mode.
    for (int i = 0; i < NTIMERS; i++) {
        timer_t *t = timers + i;
        uint32_t ticks = nticks;
        if (!t->enabled) {
            continue;
        }
        if (t->prescaler > 1 && t->prescaler - t->prescaler_count < ticks) {
            ticks -= t->prescaler - t->prescaler_count;
            t->prescaler_count = 0;
            t->count++;
        }
        t->count += ticks / t->prescaler;
        ticks %= t->prescaler;
        t->prescaler_count += ticks;
    }
}

uint32_t us_to_ticks(uint32_t us) {
    assert(_hz);
    assert(us < (MAXINT_32 / (_hz / (1000 * 1000))));
    return us * (_hz / (1000 * 1000));
}

uint32_t ms_to_ticks(uint32_t ms) {
    assert(_hz);
    assert(ms < (MAXINT_32 / (_hz / 1000)));
    return ms * (_hz / 1000);
}

void rcc_periph_reset_pulse(enum rcc_periph_rst rst) {
    switch (rst) {
        case RST_TIM1:
            _timer_init(_idx(TIM1));
            break;
        case RST_TIM2:
            _timer_init(_idx(TIM2));
            break;
        case RST_TIM3:
            _timer_init(_idx(TIM3));
            break;
        case RST_TIM14:
            _timer_init(_idx(TIM14));
            break;
        case RST_TIM16:
            _timer_init(_idx(TIM16));
            break;
        case RST_TIM17:
            _timer_init(_idx(TIM17));
            break;
        default:
            assert(0);
    }
}

void timer_set_prescaler(uint32_t timer_peripheral, uint32_t value) {
    assert(value >= 1);
    assert(value <= 65536);
    timers[_idx(timer_peripheral)].prescaler = value;
    timers[_idx(timer_peripheral)].prescaler_count = 0;
}

void timer_disable_preload(uint32_t timer_peripheral) {
}

void timer_continuous_mode(uint32_t timer_peripheral) {
}

void timer_one_shot_mode(uint32_t timer_peripheral) {
}

void timer_enable_counter(uint32_t timer_peripheral) {
    timers[_idx(timer_peripheral)].enabled = true;
}

void timer_set_counter(uint32_t timer_peripheral, uint32_t count) {
    // limit to 16b where applicable
    if (timer_peripheral != TIM2) {
        assert(count <= 0xffff);
    }
    timers[_idx(timer_peripheral)].count = count;
}

uint32_t timer_get_counter(uint32_t timer_peripheral) {
    return timers[_idx(timer_peripheral)].count;
}

void timer_direction_up(uint32_t timer_peripheral) {
}
void timer_enable_irq(uint32_t timer_peripheral, uint32_t irq) {
}
void timer_set_oc_value(uint32_t timer_peripheral, enum tim_oc_id oc_id,
                        uint32_t value) {
}

bool timer_get_flag(uint32_t timer_peripheral, uint32_t flag) {
    return false;
}

void timer_clear_flag(uint32_t timer_peripheral, uint32_t flag) {
}

// I'm not making a fake_nvic until it's actually needed.
void nvic_enable_irq(uint8_t irqn) {
}
