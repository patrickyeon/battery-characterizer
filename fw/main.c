#include "./driver/at30ts74.h"
#include "./driver/flash.h"
#include "./driver/i2c.h"
#include "./driver/usb.h"

#include "./src/adc.h"
#include "./src/commands.h"
#include "./src/director.h"
#include "./src/gpio.h"
#include "./src/logger.h"
#include "./src/timers.h"

#include "libopencm3/stm32/flash.h"

void init(void) {
    gpio_init();
    timers_init();
    i2c_init();
    director_init();
    usb_init();
    commands_init();
    flash_init();
    logger_init();
    uint8_t adc_chans[] = {0};
    adc_init(1, adc_chans, ADC_MODE_SCAN_INFINITE);

    at30ts74_init(0x48);
    at30ts74_init(0x49);
    at30ts74_init(0x4c);
    at30ts74_init(0x4d);
}

int main(void) {
    init();
    abs_time_t lastloop = (abs_time_t){0, 0};

    while (1) {
        abs_time_t now = systime();
        if (ms_elapsed(&lastloop, &now) >= 100) {
            // 10hz loop
            lastloop = now;
            commands_process();
        }
        usb_poll();
    }
    return 0;
}
