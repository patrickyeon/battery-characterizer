#include "./temperature.h"
#include "../driver/at30ts74.h"

int16_t temperature_read(tsens_e which) {
    return at30ts74_read(which);
}
