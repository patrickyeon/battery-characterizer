#ifndef STM32_ADC_H
#define STM32_ADC_H

#include <libopencm3/stm32/adc.h>

void stm_adc_init(uint8_t nChan, uint8_t channels[], enum adc_opmode opmode);
void stm_adc_scan(void);
uint16_t stm_adc_read(uint8_t channel);

#endif // STM32_ADC_H
