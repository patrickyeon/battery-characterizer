#include "./stm32_adc.h"

#include <libopencm3/stm32/dma.h>

static enum adc_opmode _mode;
static uint16_t chanbuff[18]; // for DMA'ing straight in

void stm_adc_init(uint8_t nChan, uint8_t channels[], enum adc_opmode opmode) {
    _mode = opmode;
    // starting off with just a guess at what we'll want
    adc_power_off(ADC1);
    adc_set_clk_source(ADC1, ADC_CLKSOURCE_ADC); // I think that means 14MHz
    adc_set_sample_time_on_all_channels(ADC1, ADC_SMPTIME_239DOT5);
    // that's ~17us per sample, plenty fast enough for me
    adc_calibrate(ADC1);
    adc_set_operation_mode(ADC1, ADC_MODE_SCAN_INFINITE);
    adc_disable_discontinuous_mode(ADC1);
    adc_disable_external_trigger_regular(ADC1);
    adc_set_right_aligned(ADC1);
    adc_set_regular_sequence(ADC1, nChan, channels);
    adc_set_resolution(ADC1, ADC_RESOLUTION_12BIT);
    adc_disable_analog_watchdog(ADC1);

    // these are really just me guessing
    //  there is a note in the reference manual, pp202, about the "corresponding
    // mapping bit in SYSCFG". I dunno, if it doesn't work look at that.
    dma_channel_reset(DMA1, 1);
    dma_enable_circular_mode(DMA1, 1);
    dma_set_memory_size(DMA1, 1, DMA_CCR_MSIZE_16BIT);
    dma_set_peripheral_size(DMA1, 1, DMA_CCR_MSIZE_16BIT);
    dma_enable_memory_increment_mode(DMA1, 1);
    dma_disable_peripheral_increment_mode(DMA1, 1);
    dma_set_read_from_peripheral(DMA1, 1);
    dma_set_priority(DMA1, 1, DMA_CCR_PL_LOW);
    dma_set_memory_address(DMA1, 1, chanbuff);
    // this address is a guess.
    dma_set_peripheral_address(DMA1, 1, &ADC_DR(ADC1));
    dma_set_number_of_data(DMA1, 1, nChan);
    dma_enable_channel(DMA1, 1);

    adc_power_on(ADC1);
    adc_enable_dma(ADC1);
}

uint16_t stm_adc_read(uint8_t channel) {
    return chanbuff[channel];
}
