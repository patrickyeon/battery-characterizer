#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

void gpio_init(void);
void pwm_out(uint32_t pin, uint16_t duty_frac);

#endif // GPIO_H
