#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

typedef void (*fp_t)(void);

void scheduler_init(uint8_t n_loop10hz, fp_t *loop10hz_cb);
void scheduler_tick(void);

#endif // SCHEDULER_H
