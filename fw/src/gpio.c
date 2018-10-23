#include "./gpio.h"
#include "./pindefs.h"

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>

#include <assert.h>

void gpio_init(void) {
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    
    gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
            (SET_ID_A | SET_ID_B | SET_IC_A | SET_IC_B));
    gpio_clear(GPIOB, (SET_ID_A | SET_ID_B | SET_IC_A | SET_IC_B));

    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, (DIR_A | CEN_B));
    gpio_clear(GPIOA, (DIR_A | CEN_B));

    gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
            (DIR_B | CEN_A | DEN_A | DEN_B));
    gpio_clear(GPIOB, (DIR_B | CEN_A | DEN_A | DEN_B));

    gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
                    (SET_ID_A | SET_ID_B | SET_IC_A | SET_IC_B));
    gpio_clear(GPIOB, (SET_ID_A | SET_ID_B | SET_IC_A | SET_IC_B));

    rcc_periph_clock_enable(RCC_TIM3);
    nvic_enable_irq(NVIC_TIM3_IRQ);
    rcc_periph_reset_pulse(RST_TIM3);
    timer_disable_preload(TIM3);
    timer_continuous_mode(TIM3);
    timer_direction_up(TIM3);
    timer_set_counter(TIM3, 0);
    timer_set_oc_value(TIM3, TIM_OC1, 0xffff);
    timer_enable_irq(TIM3, TIM_DIER_CC1IE);
    timer_set_oc_value(TIM3, TIM_OC2, 0xffff);
    timer_enable_irq(TIM3, TIM_DIER_CC2IE);
    timer_set_oc_value(TIM3, TIM_OC3, 0xffff);
    timer_enable_irq(TIM3, TIM_DIER_CC3IE);
    timer_set_oc_value(TIM3, TIM_OC4, 0xffff);
    timer_enable_irq(TIM3, TIM_DIER_CC4IE);
    timer_enable_counter(TIM3);
}

void pwm_out(uint32_t pin, uint16_t duty_frac) {
    static uint16_t duties[4] = {0, 0, 0, 0};
    int idx;
    enum tim_oc_id oc;
    switch (pin) {
    case SET_ID_A:
        idx = 0;
        oc = TIM_OC1;
        break;
    case SET_ID_B:
        idx = 1;
        oc = TIM_OC2;
        break;
    case SET_IC_A:
        idx = 2;
        oc = TIM_OC3;
        break;
    case SET_IC_B:
        idx = 3;
        oc = TIM_OC4;
        break;
    default:
        assert(0);
        return;
    }
    duties[idx] = 0xffff - duty_frac;
    timer_set_oc_value(TIM3, oc, duties[idx]);
}

void tim3_isr(void) {
    if (timer_get_flag(TIM3, TIM_SR_CC1IF)) {
        timer_clear_flag(TIM3, TIM_SR_CC1IF);
        gpio_set(GPIOB, SET_ID_A);
    }
    if (timer_get_flag(TIM3, TIM_SR_CC2IF)) {
        timer_clear_flag(TIM3, TIM_SR_CC2IF);
        gpio_set(GPIOB, SET_ID_B);
    }
    if (timer_get_flag(TIM3, TIM_SR_CC3IF)) {
        timer_clear_flag(TIM3, TIM_SR_CC3IF);
        gpio_set(GPIOB, SET_IC_A);
    }
    if (timer_get_flag(TIM3, TIM_SR_CC4IF)) {
        timer_clear_flag(TIM3, TIM_SR_CC4IF);
        gpio_set(GPIOB, SET_IC_B);
    }
    // if it overflowed, clear them all
    if (timer_get_flag(TIM3, TIM_SR_UIF)) {
        timer_clear_flag(TIM3, TIM_SR_UIF);
        gpio_clear(GPIOB, (SET_ID_A | SET_ID_B | SET_IC_A | SET_IC_B));
    }
}
