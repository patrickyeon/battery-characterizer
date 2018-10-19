#ifndef FAKE_TIME_H
#define FAKE_TIME_H

#include <stdint.h>

void fake_time_init(uint32_t clk_hz);
void fake_time_run(uint32_t nticks);
uint32_t us_to_ticks(uint32_t us);
uint32_t ms_to_ticks(uint32_t ms);

#endif // FAKE_TIME_H
