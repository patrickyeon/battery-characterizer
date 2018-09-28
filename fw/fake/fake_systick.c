#include "./fake_systick.h"

#include <libopencm3/cm3/systick.h>

void systick_set_clocksource(uint8_t clocksource) { }
void systick_set_reload(uint32_t value) { }
void systick_clear(void) { }
void systick_interrupt_enable(void) { }
void systick_counter_enable(void) { }
