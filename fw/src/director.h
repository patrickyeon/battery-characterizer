#ifndef DIRECTOR_H
#define DIRECTOR_H

#include <stdbool.h>
#include <stdint.h>

typedef enum chg_direction_e {
    CHG_DISCHG,
    DISCHG_CHG
} chg_direction_e;

extern struct director_t {
    bool cenA, cenB, denA, denB;
    chg_direction_e dirA, dirB;
} director;

typedef uint8_t dir_state_t;
// TODO better name to differentiate from CEN_A etc pin defines
typedef enum dir_state_e {
    CENA = 1 << 0,
    CENB = 1 << 1,
    DENA = 1 << 2,
    DENB = 1 << 3
} dir_state_e;

// TODO should this be generated as well, so that it can be shared?
#define NERRS (7)
#if (NERRS * 4) > 32
#error("too many error codes")
#endif

#define _ERR(CELL, CODE) (1 << (CELL * NERRS + CODE))
#define ERR_OVERVOLT(CELL)    (_ERR(CELL, 0))
#define ERR_UNDERVOLT(CELL)   (_ERR(CELL, 1))
#define ERR_OVERTEMP(CELL)    (_ERR(CELL, 2))
#define ERR_UNDERTEMP(CELL)   (_ERR(CELL, 3))
#define ERR_OVERCURRENT(CELL) (_ERR(CELL, 4))
#define ERR_CHG_CUTOFF(CELL)  (_ERR(CELL, 5))
#define ERR_DCHG_CUTOFF(CELL) (_ERR(CELL, 6))

void director_init(void);
int director_enable(dir_state_t);
int director_disable(dir_state_t);
void director_direction(chg_direction_e a, chg_direction_e b);

uint32_t director_checkup(void);
void director_tick(void);
void director_log_rate(uint16_t period);
void director_log_start(void);
void director_log_stop(void);
uint16_t get_hwid(void);

#endif // DIRECTOR_H
