#ifndef I2C_H
#define I2C_H

#include <stdint.h>

enum i2c_err_e {
    I2C_NO_ERR = 0,
    I2C_ERR_NACK,
    I2C_ERR_TX_TIMEOUT,
    I2C_ERR_TX_TIMEOUT2,
    I2C_ERR_RX_TIMEOUT
};

void i2c_init(void);
uint8_t i2c_transfer(uint32_t i2c, uint8_t addr, uint8_t *w, size_t wn,
                     uint8_t *r, size_t rn, uint32_t timeout);
uint8_t timed_i2c_transfer(uint32_t i2c, uint8_t addr, uint8_t *w, size_t wn,
                           uint8_t *r, size_t rn, uint32_t timeout,
                           uint32_t *timings);

#endif // I2C_H
