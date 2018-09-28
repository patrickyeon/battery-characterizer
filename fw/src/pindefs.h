#ifndef PINDEFS_H
#define PINDEFS_H

#include <libopencm3/stm32/gpio.h>

// current setting, on PORTB
#define SET_ID_A GPIO0
#define SET_ID_B GPIO1
#define SET_IC_A GPIO4
#define SET_IC_B GPIO5

// current measurement, on PORTA
#define MEAS_IC_A GPIO0
#define MEAS_IC_B GPIO1
#define MEAS_ID_A GPIO2
#define MEAS_ID_B GPIO3
// cell voltage measurement, on PORTA
#define VBAT_0 GPIO11
#define VBAT_1 GPIO10
#define VBAT_2 GPIO13
#define VBAT_3 GPIO12

// ADC channels
#define CHAN_IC_A 0
#define CHAN_IC_B 1
#define CHAN_ID_A 2
#define CHAN_ID_B 3
#define CHAN_VB0 11
#define CHAN_VB1 10
#define CHAN_VB2 13
#define CHAN_VB3 12

// direction setting, charge/discharge enables. mixed across PORTA/PORTB
// sigh. Would've been better to swap CEN_B and DIR_B.
// PORTA
#define DIR_A GPIO8
#define CEN_B GPIO15
// PORTB
#define DIR_B GPIO3
#define CEN_A GPIO6
#define DEN_A GPIO7
#define DEN_B GPIO8

// comms, PORTA
#define SCL GPIO9
#define SDA GPIO10
#define USBDP GPIO12
#define USBDM GPIO11

#endif // PINDEFS_H
