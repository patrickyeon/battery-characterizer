#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/rcc.h>

#include "./i2c.h"
#include "../src/pindefs.h"
#include "../src/timers.h"

// GPIOA, SCL/SDA

void i2c_init(void) {
    rcc_periph_clock_enable(RCC_I2C1);
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, (SCL | SDA));
    gpio_set_af(GPIOA, GPIO_AF4, (SCL | SDA));

    i2c_peripheral_disable(I2C1);
    i2c_set_speed(I2C1, i2c_speed_fmp_1m, rcc_apb1_frequency / 1000000);
    i2c_peripheral_enable(I2C1);
}

// if you're passing timings, it will write wn + rn + (rn ? 1 : 0) timestamps
uint8_t timed_i2c_transfer(uint32_t i2c, uint8_t addr, uint8_t *w, size_t wn,
                           uint8_t *r, size_t rn, uint32_t timeout,
                           uint32_t *timings) {
    // libopencm3 i2c_transfer7(), with error report and timeout
    //  waiting for busy is unnecessary. read the RM
    if (wn) {
        i2c_set_7bit_address(i2c, addr);
        i2c_set_write_transfer_dir(i2c);
        i2c_set_bytes_to_transfer(i2c, wn);
        if (rn) {
                i2c_disable_autoend(i2c);
        } else {
                i2c_enable_autoend(i2c);
        }
        i2c_send_start(i2c);

        while (wn--) {
            bool wait = true;
            tick(TIMER_TIMEOUT);
            uint32_t t_elapsed;
            while (wait) {
                t_elapsed = tock(TIMER_TIMEOUT);
                if (t_elapsed > timeout) {
                    return I2C_ERR_TX_TIMEOUT;
                }
                if (i2c_transmit_int_status(i2c)) {
                    wait = false;
                }
                if (i2c_nack(i2c)) {
                    return I2C_ERR_NACK;
                }
            }
            if (timings) {
                *timings++ = t_elapsed;
            }
            i2c_send_data(i2c, *w++);
        }
        //  Not entirely sure this is really necessary in normal case, but it's
        // useful for timeouts.
        if (rn) {
            tick(TIMER_TIMEOUT);
            uint32_t t_elapsed;
            while (!i2c_transfer_complete(i2c)) {
                t_elapsed = tock(TIMER_TIMEOUT);
                if (t_elapsed > timeout) {
                    return I2C_ERR_TX_COMPLETE_TIMEOUT;
                }
            }
            if (timings) {
                *timings++ = t_elapsed;
            }
        }
    }

    if (rn) {
        // Setting transfer properties
        i2c_set_7bit_address(i2c, addr);
        i2c_set_read_transfer_dir(i2c);
        i2c_set_bytes_to_transfer(i2c, rn);
        // start transfer
        i2c_send_start(i2c);
        // important to do it afterwards to do a proper repeated start!
        i2c_enable_autoend(i2c);

        for (size_t i = 0; i < rn; i++) {
            tick(TIMER_TIMEOUT);
            uint32_t t_elapsed;
            while (i2c_received_data(i2c) == 0) {
                t_elapsed = tock(TIMER_TIMEOUT);
                if (t_elapsed > timeout) {
                    return I2C_ERR_RX_TIMEOUT;
                }
            }
            if (timings) {
                *timings++ = t_elapsed;
            }
            r[i] = i2c_get_data(i2c);
        }
    }
    return I2C_NO_ERR;
}

uint8_t i2c_transfer(uint32_t i2c, uint8_t addr, uint8_t *w, size_t wn,
                     uint8_t *r, size_t rn, uint32_t timeout) {
    return timed_i2c_transfer(i2c, addr, w, wn, r, rn, timeout, NULL);
}
