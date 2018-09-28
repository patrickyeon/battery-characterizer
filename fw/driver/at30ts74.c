#include "./at30ts74.h"

#include <stdint.h>
#include <stdlib.h>

#include <libopencm3/stm32/i2c.h>

// FIXME handle timeouts, NACKs

int16_t at30ts74_read(uint8_t addr) {
    uint8_t retval[2];
    uint8_t tempreg = 0;
    i2c_transfer7(I2C1, addr, &tempreg, 1, retval, 2);
    return ((int16_t)retval[0] << 8) | ((int16_t)retval[1] & 0xff);
}

void at30ts74_init(uint8_t addr) {
    // register 1, reset to 0x0000
    uint8_t config[] = {0x01, 0x00, 0x00};
    i2c_transfer7(I2C1, addr, config, 3, NULL, 0);
}
