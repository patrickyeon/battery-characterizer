#ifndef FAKE_GPIO_H
#define FAKE_GPIO_H

#include <libopencm3/stm32/gpio.h>

uint16_t fake_gpio_get(uint32_t gpioport, uint16_t mask);

#endif // FAKE_GPIO_H
