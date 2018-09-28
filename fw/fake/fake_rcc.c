#include "./fake_rcc.h"

#include <libopencm3/stm32/rcc.h>
#include <stdint.h>

void rcc_periph_clock_enable(enum rcc_periph_clken clken) {
    return;
}
void rcc_clock_setup_in_hse_8mhz_out_48mhz(void) { }
