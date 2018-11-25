#include "./temperature.h"
#include "../driver/at30ts74.h"

static uint8_t init_addrs[] = {0, 0, 0, 0};

uint32_t temperature_init(void) {
    uint8_t addrs[] = {TSENS0, TSENS1, TSENS2, TSENS3};
    int shift = 24;
    uint32_t retval = 0;
    for (int i = 0; i < 4; i++) {
        uint32_t err = at30ts74_init(addrs[i]);
        if (err == 0) {
            init_addrs[i] = addrs[i];
        }
        retval |= err << shift;
        shift -= 8;
    }
    return retval;
}

int16_t temperature_read(tsens_e which) {
    for (int i = 0; i < 4; i++) {
        if (i == 4) {
            return 0xffff;
        }
        if (init_addrs[i] == which) {
            break;
        }
    }
    int16_t val = at30ts74_read(which);
    uint8_t err = at30ts74_read_err(val);
    if (err) {
        // bad form, but hopefully a -127degC reading raises alarms
        return 0xff00 | err;
    }
    return val;
}
