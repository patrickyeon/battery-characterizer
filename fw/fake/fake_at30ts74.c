#include "../driver/at30ts74.h"
#include "./fake_at30ts74.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct sensor_t {
    int16_t temp;
    uint8_t addr;
    struct sensor_t *next;
} sensor_t;

static sensor_t sensors = {0, 0, NULL};
static bool empty = true;

void fake_at30ts74_set(uint8_t addr, int16_t temp) {
    if (empty) {
        sensors.addr = addr;
        sensors.temp = temp;
        empty = false;
        return;
    }

    for (sensor_t *sensor = &sensors; sensor != NULL; sensor = sensor->next) {
        if (sensor->addr == addr) {
            sensor->temp = temp;
            return;
        }
        if (sensor->next == NULL) {
            sensor->next = malloc(sizeof(sensor_t));
            assert(sensor->next != NULL);
            sensor->next->addr = addr;
        }
    }
}

int16_t at30ts74_read(uint8_t addr) {
    assert(!empty);
    for (sensor_t *sensor = &sensors; sensor != NULL; sensor = sensor->next) {
        if (sensor->addr == addr) {
            return sensor->temp;
        }
    }
    // reading a sensor that hasn't had a temp set for it yet
    assert(0);
}

uint8_t at30ts74_read_err(int16_t val) {
    if (val & 0xf) {
        return val >> 8;
    }
    return 0;
}
