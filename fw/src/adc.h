#ifndef ADC_H
#define ADC_H

#include <libopencm3/stm32/adc.h>

void adc_init(uint8_t nChan, uint8_t channels[], enum adc_opmode opmode);
uint16_t adc_read(uint8_t chan);
uint16_t adc_code_to_mv(uint16_t code, uint16_t fullscale_mv);
uint16_t adc_mv_to_code(uint16_t mv, uint16_t fullscale_mv);

#endif // ADC_H
