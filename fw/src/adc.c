#include "./adc.h"
#include "./pindefs.h"
#include "../driver/stm32_adc.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

void adc_init(uint8_t nChan, uint8_t channels[], enum adc_opmode opmode) {
    rcc_periph_clock_enable(RCC_ADC);
    gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE,
            (MEAS_ID_A | MEAS_ID_B | MEAS_IC_A | MEAS_IC_B
             | VBAT_0 | VBAT_1 | VBAT_2 | VBAT_3));
    // let the driver do the most of the work
    stm_adc_init(nChan, channels, opmode);
}

uint16_t adc_read(uint8_t chan) {
    //  non-blocking, because we're continuously sampling and just going to pull
    // the most recent value that's been DMA'd into our buffer
    return stm_adc_read(chan);
}

uint16_t adc_code_to_mv(uint16_t code, uint16_t fullscale_mv) {
    uint32_t retval = (uint32_t)code * (uint32_t)fullscale_mv;
    return (uint16_t)(retval >> 12);
}

uint16_t adc_mv_to_code(uint16_t mv, uint16_t fullscale_mv) {
    uint32_t retval = ((uint32_t)mv << 12) / (uint32_t)fullscale_mv;
    // clip retval like an ADC would, not mask it
    retval = retval > 0xfff ? 0xfff : retval;
    return (uint16_t)retval;
}
