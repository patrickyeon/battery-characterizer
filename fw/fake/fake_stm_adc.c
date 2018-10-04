#include "./fake_stm_adc.h"

#include <assert.h>
#include <stdlib.h>

static uint16_t values[18];
static uint32_t valid_chans;

void stm_adc_init(uint8_t nChan, uint8_t channels[], enum adc_opmode opmode) {
    // very narrow range of capabilities implemented
    assert(opmode == ADC_MODE_SCAN_INFINITE);
    assert(channels != NULL);
    assert(1 <= nChan && nChan <= 18);
    for (int i = 0; i < nChan; i++) {
        assert(0 <= channels[i] && channels[i] <= 15);
        // temp, vref, vbat are 16, 17, 18 but not implemented
        valid_chans |= 1 << channels[i];
    }
}

void fake_stm_adc_set(uint8_t channel, uint16_t val) {
    assert(valid_chans & (1 << channel));
    // only a 12-bit adc
    assert((val & 0xf000) == 0);
    values[channel] = val;
}

uint16_t stm_adc_read(uint8_t channel) {
    assert(valid_chans & (1 << channel));
    return values[channel];
}
