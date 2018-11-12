#include "./at30ts74.h"

#include <stdint.h>
#include <stdlib.h>

#include <libopencm3/stm32/i2c.h>

#include "./i2c.h"

int16_t at30ts74_read(uint8_t addr) {
    uint8_t retval[2];
    uint8_t tempreg = 0;
    // timeout of 0.1ms
    int err = i2c_transfer(I2C1, addr, &tempreg, 1, retval, 2, 50);
    if (err) {
        return ((uint16_t)err << 8) | 0xff;
    }
    return ((int16_t)retval[0] << 8) | ((int16_t)retval[1] & 0xf0);
}

uint8_t at30ts74_init(uint8_t addr) {
    // register 1, reset to 0x0000
    uint8_t config[] = {0x01, 0x00, 0x00};
    return i2c_transfer(I2C1, addr, config, 3, NULL, 0, 50);
}

uint8_t at30ts74_read_err(int16_t val) {
    if (val & 0xf) {
        return val >> 8;
    }
    return 0;
}
