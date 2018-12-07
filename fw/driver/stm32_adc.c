#include "./stm32_adc.h"

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/dma.h>

#include <assert.h>

//  This is a maximum. The f0 supports 18, but if we're not using them all, we
// may as well save a bit of running time
#define N_CHANS 10

//  Code should be re-worked (namely, averaging taken out of the ISR) if you
// want something not power-of-two. Also, buffers would need to be larger
// datatypes to safely handle averaging of >16 samples (at 12 bit resolution).
static const int averaging = 8;

static enum adc_opmode _mode;
static volatile uint16_t chanbuff[N_CHANS]; // for DMA'ing straight in
static volatile uint16_t accumbuff[N_CHANS];
static volatile uint16_t resultbuff[N_CHANS];
static volatile int n_accum;

void stm_adc_init(uint8_t nChan, uint8_t channels[], enum adc_opmode opmode) {
    assert(nChan <= N_CHANS);
    _mode = opmode;
    for (int i = 0; i < N_CHANS; i++) {
        accumbuff[i] = resultbuff[i] = 0;
    }
    n_accum = 0;

    adc_power_off(ADC1);
    adc_set_clk_source(ADC1, ADC_CLKSOURCE_ADC); // I think that means 14MHz
    adc_set_sample_time_on_all_channels(ADC1, ADC_SMPTIME_239DOT5);
    // that's ~17us per sample, plenty fast enough for me
    adc_calibrate(ADC1);
    adc_set_operation_mode(ADC1, opmode);
    adc_disable_discontinuous_mode(ADC1);
    adc_disable_external_trigger_regular(ADC1);
    adc_disable_temperature_sensor();
    adc_set_right_aligned(ADC1);
    adc_set_regular_sequence(ADC1, nChan, channels);
    adc_set_resolution(ADC1, ADC_RESOLUTION_12BIT);
    adc_disable_analog_watchdog(ADC1);

    adc_enable_temperature_sensor();
    adc_enable_vrefint();

    // These are really just me guessing. They seem to work fine though.
    dma_channel_reset(DMA1, 1);
    dma_enable_circular_mode(DMA1, 1);
    dma_set_memory_size(DMA1, 1, DMA_CCR_MSIZE_16BIT);
    dma_set_peripheral_size(DMA1, 1, DMA_CCR_PSIZE_16BIT);
    dma_enable_memory_increment_mode(DMA1, 1);
    dma_disable_peripheral_increment_mode(DMA1, 1);
    dma_set_read_from_peripheral(DMA1, 1);
    dma_set_priority(DMA1, 1, DMA_CCR_PL_LOW);
    dma_set_memory_address(DMA1, 1, (uint32_t)chanbuff);
    dma_set_peripheral_address(DMA1, 1, (uint32_t)&ADC_DR(ADC1));
    dma_set_number_of_data(DMA1, 1, nChan);
    dma_enable_channel(DMA1, 1);

    adc_enable_eoc_sequence_interrupt(ADC1);
    nvic_set_priority(NVIC_ADC_COMP_IRQ, 0);
    nvic_enable_irq(NVIC_ADC_COMP_IRQ);

    adc_power_on(ADC1);
    adc_enable_dma(ADC1);
}

void stm_adc_scan(void) {
    adc_start_conversion_regular(ADC1);
    return;
}

uint16_t stm_adc_read(uint8_t channel) {
    if (averaging == 1) {
        return chanbuff[channel];
    }
    return resultbuff[channel];
}

void adc_comp_isr(void) {
    if (adc_eos(ADC1)) {
        ADC_ISR(ADC1) = ADC_ISR_EOSEQ;
        if (averaging == 1) {
            // we'll just serve reads out of the chanbuff
            return;
        }
        for (int i = 0; i < N_CHANS; i++) {
            accumbuff[i] += chanbuff[i];
        }
        if (++n_accum >= averaging) {
            for (int i = 0; i < N_CHANS; i++) {
                resultbuff[i] = accumbuff[i] / averaging;
                accumbuff[i] = 0;
            }
            n_accum = 0;
        } else {
            adc_start_conversion_regular(ADC1);
        }
    }
}
