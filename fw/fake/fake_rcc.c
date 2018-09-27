#include "./fake_rcc.h"

#include <libopencm3/stm32/rcc.h>
#include <stdint.h>

void rcc_periph_clock_enable(enum rcc_periph_clken clken) {
    return;
}
