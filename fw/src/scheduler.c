#include "./scheduler.h"
#include "./timers.h"

static fp_t tenhz_callbacks[8];
static uint8_t ncallbacks;

void scheduler_init(uint8_t n_loop10hz, fp_t *loop10hz_cb) {
    ncallbacks = (n_loop10hz >= 8 ? 8 : n_loop10hz);
    for (int i = 0; i < ncallbacks; i++) {
        tenhz_callbacks[i] = loop10hz_cb[i];
    }
}

void scheduler_tick(void) {
    static abs_time_t last_run = {0, 0};
    abs_time_t now = systime();
    if (ms_elapsed(&last_run, &now) >= 100) {
        for (int i = 0; i < ncallbacks; i++) {
            tenhz_callbacks[i]();
        }
        last_run = systime();
    }
}
