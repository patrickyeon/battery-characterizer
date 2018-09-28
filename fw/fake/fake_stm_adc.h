#ifndef FAKE_STM_ADC_H
#define FAKE_STM_ADC_H

#include <stdint.h>
#include <libopencm3/stm32/adc.h>

void fake_stm_adc_set(uint8_t channel, uint16_t val);

#endif // FAKE_STM_ADC_H
