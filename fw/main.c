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

void init(void) {
    gpio_init();
    timers_init();
    i2c_init();
    director_init();
    usb_init();
    commands_init();
    flash_init();
    logger_init();
    // ohhh, channel list has to be in order
    uint8_t adc_chans[] = {CHAN_IC_A, CHAN_IC_B, CHAN_ID_A, CHAN_ID_B,
                           CHAN_VB1, CHAN_VB0, CHAN_VB3, CHAN_VB2};
    adc_init(8, adc_chans, ADC_MODE_SCAN);

    at30ts74_init(0x48);
    at30ts74_init(0x49);
    at30ts74_init(0x4c);
    at30ts74_init(0x4d);
}

static int16_t temperature[4];
static const tsens_e tsens[4] = {TSENS0, TSENS1, TSENS2, TSENS3};
static bool logging = false;
static uint32_t long_tenhz = 0;

static void tenhz(void) {
    static int idx = 0;
    abs_time_t now = systime();
    uint32_t err = director_checkup();
    if (logging) {
        if (err) {
            logger_log_error(&now, err);
        }
        // TODO figure out how to manage this logging
        if (idx % 10 == 0) {
            if (director.dirA == CHG_DISCHG) {
                logger_log_iv(&now, LOG_IV_CHG_BAT0,
                              adc_code_to_mv(adc_read(CHAN_VB0), 6600),
                              adc_code_to_mv(adc_read(CHAN_IC_A), 6600));
                logger_log_iv(&now, LOG_IV_DCH_BAT1,
                              adc_code_to_mv(adc_read(CHAN_VB1), 6600),
                              adc_code_to_mv(adc_read(CHAN_ID_A), 6600));
            } else {
                logger_log_iv(&now, LOG_IV_DCH_BAT0,
                              adc_code_to_mv(adc_read(CHAN_VB0), 6600),
                              adc_code_to_mv(adc_read(CHAN_ID_A), 6600));
                logger_log_iv(&now, LOG_IV_CHG_BAT1,
                              adc_code_to_mv(adc_read(CHAN_VB1), 6600),
                              adc_code_to_mv(adc_read(CHAN_IC_A), 6600));
            }
            if (director.dirB == CHG_DISCHG) {
                logger_log_iv(&now, LOG_IV_CHG_BAT2,
                              adc_code_to_mv(adc_read(CHAN_VB2), 6600),
                              adc_code_to_mv(adc_read(CHAN_IC_B), 6600));
                logger_log_iv(&now, LOG_IV_DCH_BAT3,
                              adc_code_to_mv(adc_read(CHAN_VB3), 6600),
                              adc_code_to_mv(adc_read(CHAN_ID_B), 6600));
            } else {
                logger_log_iv(&now, LOG_IV_DCH_BAT2,
                              adc_code_to_mv(adc_read(CHAN_VB2), 6600),
                              adc_code_to_mv(adc_read(CHAN_ID_B), 6600));
                logger_log_iv(&now, LOG_IV_CHG_BAT3,
                              adc_code_to_mv(adc_read(CHAN_VB3), 6600),
                              adc_code_to_mv(adc_read(CHAN_IC_B), 6600));
            }
        }
    }
    adc_scan();
    temperature[idx % 4] = temperature_read(tsens[idx % 4]);
    idx++;
}

static void onehz(void) {
    if (logging && long_tenhz > 80000) {
        abs_time_t now = systime();
        // FIXME be much better with errors
        logger_log_error(&now, (1 << 20));
    }
}

int main(void) {
    init();
    abs_time_t last_tenhz = (abs_time_t){0, 0};
    abs_time_t last_onehz = (abs_time_t){0, 0};

    while (1) {
        abs_time_t now = systime();
        if (ms_elapsed(&last_tenhz, &now) >= 100) {
            // 10hz loop
            last_tenhz = now;
            tick();
            tenhz();
            uint32_t t = tock();
            if (t > long_tenhz) {
                long_tenhz = t;
            }
        } else if (ms_elapsed(&last_onehz, &now) >= 1000) {
            last_onehz = now;
            onehz();
        }
        usb_poll();
        commands_process();
    }
    return 0;
}
