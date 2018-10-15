#include "./driver/usb.h"
#include "./driver/flash.h"
#include "./src/gpio.h"
#include "./src/timers.h"
#include "./src/commands.h"
#include "./src/logger.h"

#include "libopencm3/stm32/flash.h"

void init(void) {
    gpio_init();
    timers_init();
    usb_init();
    commands_init();
    flash_init();
    logger_init();
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
