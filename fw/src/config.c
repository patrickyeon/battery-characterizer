#include "./config.h"

#include <assert.h>
#include <stdlib.h>

//FIXME make it packed
static struct config_t {
    uint16_t hwid;
    // charge/discharge currents
    uint16_t ichgA, idchgA, ichgB, idchgB;
    // cutoff charge currents (charge end when current below this)
    uint16_t ic_cutoffA, ic_cutoffB;
    // cutoff discharge voltages (discharged when VBAT below this)
    uint16_t vd_cutoffA, vd_cutoffB;
    // temperature range for charging and discharging
    int16_t temp_min, temp_max;
} base_config;

uint32_t _calculate_crc(uint8_t *buff, size_t len) {
    //FIXME not a real CRC happening here
    uint32_t retval = 0;
    for (int i = 0; i < len; i++) {
        retval += buff[i];
    }
    return retval;
}

void config_init(void) {
    // fresh config with some sane defaults
    //TODO: make these defaults sane
    base_config = (struct config_t) {
        HWID,
        1000, 1000, 1000, 1000,
        25, 25,
        2500, 2500,
        15, 25
    };
}

uint16_t config_hwid(void) {
    return base_config.hwid;
}

