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

typedef enum dir_err_e {
    ERR_OVERVOLT_CELL0 = (1 << 0),
    ERR_UNDERVOLT_CELL0 = (1 << 1),
    ERR_OVERTEMP_CELL0 = (1 << 2),
    ERR_UNDERTEMP_CELL0 = (1 << 3),
    ERR_OVERCURRENT_CELL0 = (1 << 4),

    ERR_OVERVOLT_CELL1 = (1 << 5),
    ERR_UNDERVOLT_CELL1 = (1 << 6),
    ERR_OVERTEMP_CELL1 = (1 << 7),
    ERR_UNDERTEMP_CELL1 = (1 << 8),
    ERR_OVERCURRENT_CELL1 = (1 << 9),

    ERR_OVERVOLT_CELL2 = (1 << 10),
    ERR_UNDERVOLT_CELL2 = (1 << 11),
    ERR_OVERTEMP_CELL2 = (1 << 12),
    ERR_UNDERTEMP_CELL2 = (1 << 13),
    ERR_OVERCURRENT_CELL2 = (1 << 14),

    ERR_OVERVOLT_CELL3 = (1 << 15),
    ERR_UNDERVOLT_CELL3 = (1 << 16),
    ERR_OVERTEMP_CELL3 = (1 << 17),
    ERR_UNDERTEMP_CELL3 = (1 << 18),
    ERR_OVERCURRENT_CELL3 = (1 << 19)
} dir_err_e;


void director_init(void);
int director_enable(dir_state_t);
int director_disable(dir_state_t);
void director_direction(chg_direction_e a, chg_direction_e b);

uint32_t director_checkup(void);
void director_tick(void);
void director_log_rate(uint16_t period);
void director_log_start(void);
void director_log_stop(void);

#endif // DIRECTOR_H
