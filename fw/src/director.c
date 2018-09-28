#include "./adc.h"
#include "./director.h"
#include "./pindefs.h"
#include "./temperature.h"

#include <libopencm3/stm32/gpio.h>

struct director_t director;

static int16_t min_temp = 5, max_temp = 35;
static uint16_t vbat_min, vbat_max;

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
    uint16_t vbat;
    tsens_e tsens;
    //  if a pair is in CHG_DISCHG, the lower-numbered cell+tsens are charging,
    // and the other cell+tsens are discharging. Swap for DISCHG_CHG.
    switch (which) {
    case CENA:
        flag = &director.cenA;
        tsens = (director.dirA == CHG_DISCHG ? TSENS0 : TSENS1);
        vbat = adc_read(director.dirA == CHG_DISCHG ? CHAN_VB0 : CHAN_VB1);
        break;
    case CENB:
        flag = &director.cenB;
        tsens = (director.dirB == CHG_DISCHG ? TSENS2 : TSENS3);
        vbat = adc_read(director.dirB == CHG_DISCHG ? CHAN_VB2 : CHAN_VB3);
        break;
    case DENA:
        flag = &director.denA;
        tsens = (director.dirA == CHG_DISCHG ? TSENS1 : TSENS0);
        vbat = adc_read(director.dirA == CHG_DISCHG ? CHAN_VB1 : CHAN_VB0);
        break;
    case DENB:
        flag = &director.denB;
        tsens = (director.dirB == CHG_DISCHG ? TSENS3 : TSENS2);
        vbat = adc_read(director.dirB == CHG_DISCHG ? CHAN_VB3 : CHAN_VB2);
        break;
    default:
        return -1; // invalid state
    }
    if (*flag) {
        // already enabled
        return 0;
    }
    int16_t temp = temperature_read(tsens);
    if (min_temp <= temp && temp <= max_temp
        && vbat_min <= vbat && vbat <= vbat_max) {
        *flag = true;
        if (flag == &director.cenA) {
            gpio_set(GPIOB, CEN_A);
        } else if (flag == &director.cenB) {
            gpio_set(GPIOA, CEN_B);
        } else if (flag == &director.denA) {
            gpio_set(GPIOB, DEN_A);
        } else {
            gpio_set(GPIOB, DEN_B);
        }
    }
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

    _disable(CENA);
    _disable(CENB);
    _disable(DENA);
    _disable(DENB);

    _set_dir(CHG_DISCHG, CHG_DISCHG);
    return;
}

int director_enable(dir_state_t target) {
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
