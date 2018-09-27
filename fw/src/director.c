#include "./director.h"
#include "./pindefs.h"
#include "./temperature.h"

#include <libopencm3/stm32/gpio.h>

struct director_t director;

static const int16_t min_temp = 5, max_temp = 35;

static void _disable(enum dir_state_e which) {
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

static int _enable(enum dir_state_e which) {
    bool *flag;
    // FIXME need to actually figure out which TSENS matters
    tsens_e tsens;
    switch (which) {
    case CENA:
        flag = &director.cenA;
        tsens = TSENS0;
        break;
    case CENB:
        flag = &director.cenB;
        tsens = TSENS1;
        break;
    case DENA:
        flag = &director.denA;
        tsens = TSENS0;
        break;
    case DENB:
        flag = &director.denB;
        tsens = TSENS1;
        break;
    default:
        return -1; // invalid state
    }
    if (*flag) {
        // already enabled
        return 0;
    }
    int16_t temp = temperature_read(tsens);
    if (min_temp <= temp && temp <= max_temp) {
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

static void _set_dir(enum chg_direction_e a, enum chg_direction_e b) {
    if (a == CHG_DISCHG) {
        gpio_set(GPIOA, DIR_A);
    } else {
        gpio_clear(GPIOA, DIR_A);
    }
    if (b == CHG_DISCHG) {
        gpio_set(GPIOB, DIR_B);
    } else {
        gpio_clear(GPIOB, DIR_B);
    }
}

void director_init(void) {
    _disable(CENA);
    _disable(CENB);
    _disable(DENA);
    _disable(DENB);

    _set_dir(CHG_DISCHG, CHG_DISCHG);
    return;
}

int director_enable(dir_state_t target) {
    enum dir_state_e dirs[4] = {CENA, CENB, DENA, DENB};
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
