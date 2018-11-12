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
    i2c_set_speed(I2C1, i2c_speed_fm_400k, rcc_apb1_frequency / 1000000);
    i2c_peripheral_enable(I2C1);
}

uint8_t i2c_transfer(uint32_t i2c, uint8_t addr, uint8_t *w, size_t wn,
                     uint8_t *r, size_t rn, uint32_t timeout) {
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
           //  we know the main loop tick/tock is running, also we know it's not
           // going to be reset while we do this.
           uint32_t t = tock(TIMER_LOOP);
           while (wait) {
               if (tock(TIMER_LOOP) - t > timeout) {
                   return I2C_ERR_TX_TIMEOUT;
               }
               if (i2c_transmit_int_status(i2c)) {
                   wait = false;
               }
               if (i2c_nack(i2c)) {
                   return I2C_ERR_NACK;
               }
           }
           while (!i2c_transmit_int_status(i2c)) {
               if (tock(TIMER_LOOP) - t > timeout) {
                   return I2C_ERR_TX_TIMEOUT2;
               }
           }
           i2c_send_data(i2c, *w++);
       }
       // not entirely sure this is really necessary.
       // RM implies it will stall until it can write out the later bits
       if (rn) {
           //  we know the main loop tick/tock is running, also we know it's not
           // going to be reset while we do this.
           uint32_t t = tock(TIMER_LOOP);
           while (!i2c_transfer_complete(i2c)) {
               if (tock(TIMER_LOOP) - t > timeout) {
                   return I2C_ERR_TX_TIMEOUT;
               }
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
            uint32_t t = tock(TIMER_LOOP);
            while (i2c_received_data(i2c) == 0) {
                if (tock(TIMER_LOOP) - t > timeout) {
                    return I2C_ERR_RX_TIMEOUT;
                }
            }
            r[i] = i2c_get_data(i2c);
        }
    }
    return I2C_NO_ERR;
}
