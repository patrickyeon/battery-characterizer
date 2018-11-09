#include "./adc.h"
#include "./director.h"
#include "./logger.h"
#include "./pindefs.h"
#include "./temperature.h"

#include <libopencm3/stm32/gpio.h>
#include <stdbool.h>

struct director_t director;

static int16_t min_temp = 5, max_temp = 35;
static uint16_t vbat_min, vbat_max;

static int16_t temp_cache[4]; // temperatures
static bool _logging = false;
static uint32_t _log_period = 1000;
static abs_time_t _last_log = (abs_time_t){0, 0};

static void _read_temps(void) {
    temp_cache[0] = temperature_read(TSENS0);
    temp_cache[1] = temperature_read(TSENS1);
    temp_cache[2] = temperature_read(TSENS2);
    temp_cache[3] = temperature_read(TSENS3);
}

static int16_t _temp_for(dir_state_e channel) {
    //  if a pair is in CHG_DISCHG, the lower-numbered cell+tsens are charging,
    // and the other cell+tsens are discharging. Swap for DISCHG_CHG.
    switch(channel) {
    case CENA:
        return temp_cache[director.dirA == CHG_DISCHG ? 0 : 1];
    case CENB:
        return temp_cache[director.dirB == CHG_DISCHG ? 2 : 3];
    case DENA:
        return temp_cache[director.dirA == CHG_DISCHG ? 1 : 0];
    case DENB:
        return temp_cache[director.dirB == CHG_DISCHG ? 3 : 2];
    }
    // shouldn't end up here, but here's a nonsense temperature
    return 0xffff;
}

static uint16_t _voltage_for(dir_state_e channel) {
    switch(channel) {
    case CENA:
        return adc_read(director.dirA == CHG_DISCHG ? CHAN_VB0 : CHAN_VB1);
    case CENB:
        return adc_read(director.dirB == CHG_DISCHG ? CHAN_VB2 : CHAN_VB3);
    case DENA:
        return adc_read(director.dirA == CHG_DISCHG ? CHAN_VB1 : CHAN_VB0);
    case DENB:
        return adc_read(director.dirB == CHG_DISCHG ? CHAN_VB3 : CHAN_VB2);
    }
    // shouldn't end up here
    return 0xffff;
}

static void _disable(dir_state_e which) {
    switch (which) {
    case CENA:
        director.cenA = false;
        gpio_clear(GPIOB, CEN_A);
        break;
    case CENB:
        director.cenB = false;
        gpio_clear(GPIOA, CEN_B);
        break;
    case DENA:
        director.denA = false;
        gpio_clear(GPIOB, DEN_A);
        break;
    case DENB:
        director.denB = false;
        gpio_clear(GPIOB, DEN_B);
        break;
    default:
        // urkh
        return;
    }
}

static int _enable(dir_state_e which) {
    bool *flag;
    uint32_t port;
    uint16_t pin;
    switch (which) {
    case CENA:
        flag = &director.cenA;
        port = GPIOB;
        pin = CEN_A;
        break;
    case CENB:
        flag = &director.cenB;
        port = GPIOA;
        pin = CEN_B;
        break;
    case DENA:
        flag = &director.denA;
        port = GPIOB;
        pin = DEN_A;
        break;
    case DENB:
        flag = &director.denB;
        port = GPIOB;
        pin = DEN_B;
        break;
    default:
        return -1; // invalid state
    }
    if (*flag) {
        // already enabled
        return 0;
    }
#ifndef DIRECTOR_UNSAFE
    int16_t temp = _temp_for(which);
    uint16_t vbat = _voltage_for(which);
    if (min_temp <= temp && temp <= max_temp
        && vbat_min <= vbat && vbat <= vbat_max) {
#endif // DIRECTOR_UNSAFE
        *flag = true;
        gpio_set(port, pin);
#ifndef DIRECTOR_UNSAFE
    }
#endif
    return 0;
}

static void _set_dir(chg_direction_e a, chg_direction_e b) {
    if (a == CHG_DISCHG) {
        gpio_set(GPIOA, DIR_A);
    } else {
        gpio_clear(GPIOA, DIR_A);
    }
    director.dirA = a;

    if (b == CHG_DISCHG) {
        gpio_set(GPIOB, DIR_B);
    } else {
        gpio_clear(GPIOB, DIR_B);
    }
    director.dirB = b;
}

void director_direction(chg_direction_e a, chg_direction_e b) {
    if (a != director.dirA) {
        _disable(CENA);
        _disable(DENA);
    }
    if (b != director.dirB) {
        _disable(CENB);
        _disable(DENB);
    }
    _set_dir(a, b);
}

void director_init(void) {
    vbat_min = adc_mv_to_code(2500, 6600);
    vbat_max = adc_mv_to_code(4200, 6600);

    _last_log = (abs_time_t){0, 0};
    _log_period = 1000;
    _logging = false;

    _read_temps();

    _disable(CENA);
    _disable(CENB);
    _disable(DENA);
    _disable(DENB);

    _set_dir(CHG_DISCHG, CHG_DISCHG);
    return;
}

int director_enable(dir_state_t target) {
    adc_scan();
    dir_state_e dirs[4] = {CENA, CENB, DENA, DENB};
    for (int i = 0; i < 4; i++) {
        if (target & dirs[i]) {
            _enable(dirs[i]);
        }
    }
    return 0;
}

int director_disable(dir_state_t target) {
    if (target & CENA) {
        _disable(CENA);
    }
    if (target & CENB) {
        _disable(CENB);
    }
    if (target & DENA) {
        _disable(DENA);
    }
    if (target & DENB) {
        _disable(DENB);
    }
    return 0;
}

static uint32_t _check(bool condition, dir_state_e channel, dir_err_e errcode) {
    if (condition) {
#ifndef DIRECTOR_UNSAFE
        _disable(channel);
#endif // DIRECTOR_UNSAFE
        return errcode;
    } else {
        return 0;
    }
}

static uint32_t _check_all(dir_state_e channel, dir_err_e basecode) {
    uint32_t retval = 0;

    int16_t temp = _temp_for(channel);
    retval |= _check(temp < min_temp, channel, basecode << 3);
    retval |= _check(temp > max_temp, channel, basecode << 2);

    uint16_t vbat = _voltage_for(channel);
    retval |= _check(vbat < vbat_min, channel, basecode << 1);
    retval |= _check(vbat > vbat_max, channel, basecode);

    return retval;
}

uint32_t director_checkup(void) {
    uint32_t retval = 0;
    if (director.cenA) {
        retval |= _check_all(CENA, (director.dirA == CHG_DISCHG ?
                                    ERR_OVERVOLT_CELL0 : ERR_OVERVOLT_CELL1));
    }
    if (director.cenB) {
        retval |= _check_all(CENB, (director.dirB == CHG_DISCHG ?
                                    ERR_OVERVOLT_CELL2 : ERR_OVERVOLT_CELL3));
    }
    if (director.denA) {
        retval |= _check_all(DENA, (director.dirA == CHG_DISCHG ?
                                    ERR_OVERVOLT_CELL1 : ERR_OVERVOLT_CELL0));
    }
    if (director.denB) {
        retval |= _check_all(DENB, (director.dirB == CHG_DISCHG ?
                                    ERR_OVERVOLT_CELL3 : ERR_OVERVOLT_CELL2));
    }
    return retval;
}

void director_tick(void) {
    uint32_t err = director_checkup();
    abs_time_t now = systime();
    if (_logging && err) {
        logger_log_error(&now, err);
    }
    if (_logging && ms_elapsed(&_last_log, &now) > _log_period) {
        // TODO also log state, temperature
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
        // TODO I forget what I was going to log as stat flags :/
        logger_log_temp_stat(&now, LOG_TEMPSTAT_BAT0, temp_cache[0], 0);
        logger_log_temp_stat(&now, LOG_TEMPSTAT_BAT1, temp_cache[1], 0);
        logger_log_temp_stat(&now, LOG_TEMPSTAT_BAT2, temp_cache[2], 0);
        logger_log_temp_stat(&now, LOG_TEMPSTAT_BAT3, temp_cache[3], 0);

        _last_log = now;
    }
    _read_temps();
    adc_scan();
}

void director_log_rate(uint16_t period) {
    if (period > 0) {
        _log_period = 10 * period;
    }
}

void director_log_start(void) {
    _logging = true;
}

void director_log_stop(void) {
    _logging = false;
}
