#ifndef AT30TS74_H
#define AT30TS74_H

#include <stdint.h>

uint8_t at30ts74_init(uint8_t addr);
int16_t at30ts74_read(uint8_t addr);
uint8_t at30ts74_read_err(int16_t val);

uint32_t *at_timings(void);

#endif // AT30TS74_H
