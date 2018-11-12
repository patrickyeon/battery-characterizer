#include "./temperature.h"
#include "../driver/at30ts74.h"

int16_t temperature_read(tsens_e which) {
    int16_t val = at30ts74_read(which);
    uint8_t err = at30ts74_read_err(val);
    if (err) {
        // bad form, but hopefully a -127degC reading raises alarms
        return 0xff00 | err;
    }
    return val;
}
