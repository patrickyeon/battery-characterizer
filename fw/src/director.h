#ifndef DIRECTOR_H
#define DIRECTOR_H

#include <stdbool.h>
#include <stdint.h>

enum chg_direction_e {
    CHG_DISCHG,
    DISCHG_CHG
};

extern struct director_t {
    bool cenA, cenB, denA, denB;
    enum chg_direction_e dirA, dirB;
} director;

typedef uint8_t dir_state_t;
// TODO better name to differentiate from CEN_A etc pin defines
enum dir_state_e {
    CENA = 1 << 0,
    CENB = 1 << 1,
    DENA = 1 << 2,
    DENB = 1 << 3
};

void director_init(void);
int director_enable(dir_state_t);
int director_disable(dir_state_t);

#endif // DIRECTOR_H
