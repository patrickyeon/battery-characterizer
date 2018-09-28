#ifndef AT30TS74_H
#define AT30TS74_H

#include <stdint.h>

void at30ts74_init(uint8_t addr);
int16_t at30ts74_read(uint8_t addr);

#endif // AT30TS74_H
