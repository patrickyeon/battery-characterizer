#include "./gpio.h"
#include "./pindefs.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

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
}
