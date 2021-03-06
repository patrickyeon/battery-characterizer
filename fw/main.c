#include "./driver/at30ts74.h"
#include "./driver/flash.h"
#include "./driver/i2c.h"
#include "./driver/usb.h"

#include "./src/adc.h"
#include "./src/commands.h"
#include "./src/director.h"
#include "./src/gpio.h"
#include "./src/logger.h"
#include "./src/pindefs.h"
#include "./src/temperature.h"
#include "./src/timers.h"

#include <libopencm3/stm32/flash.h>

static uint32_t last_tenhz_time = 0;

void init(void) {
    gpio_init();
    timers_init();
    i2c_init();
    // ohhh, channel list has to be in order
    uint8_t adc_chans[] = {CHAN_IC_A, CHAN_IC_B, CHAN_ID_A, CHAN_ID_B,
                           CHAN_VB1, CHAN_VB0, CHAN_VB3, CHAN_VB2,
                           CHAN_TEMP, CHAN_VREF};
    adc_init(10, adc_chans, ADC_MODE_SCAN);
    temperature_init();
    director_init();
    usb_init();
    commands_init();
    flash_init();
    logger_init();
}

static void tenhz(void) {
    director_tick(last_tenhz_time);
}

static void onehz(void) {
}

int main(void) {
    init();
    watchdog_start();
    abs_time_t last_tenhz = (abs_time_t){0, 0};
    abs_time_t last_onehz = (abs_time_t){0, 0};

    while (1) {
        abs_time_t now = systime();
        if (ms_elapsed(&last_tenhz, &now) >= 100) {
            // 10hz loop
            last_tenhz = now;
            tick(TIMER_LOOP);
            tenhz();
            uint32_t t = tock(TIMER_LOOP);
            last_tenhz_time = t;
        } else if (ms_elapsed(&last_onehz, &now) >= 1000) {
            last_onehz = now;
            onehz();
        }
        usb_poll();
        commands_process();
        watchdog_pet();
    }
    return 0;
}
