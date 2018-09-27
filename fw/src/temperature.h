#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <stdint.h>

typedef enum {
    TSENS0 = 0x40,
    TSENS1 = 0x41,
    TSENS2 = 0x44,
    TSENS3 = 0x45
} tsens_e;

int16_t temperature_read(tsens_e which);

#endif // TEMPERATURE_H
