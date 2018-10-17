#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <stdint.h>

typedef enum {
    TSENS0 = 0x48,
    TSENS1 = 0x49,
    TSENS2 = 0x4c,
    TSENS3 = 0x4d
} tsens_e;

int16_t temperature_read(tsens_e which);

#endif // TEMPERATURE_H
