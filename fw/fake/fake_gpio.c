#include "./fake_gpio.h"

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct port_t {
    uint32_t name; // as in GPIOA etc
    uint16_t valid_pins; // bitmask, depends on the part
    uint16_t in_pins; // bitmask, set as inputs
    uint16_t out_pins; //  bitmask, set as outputs (not necessarily ~in_pins,
                       // because some could be uninitialized)
    uint16_t pin_states;
} port_t;

// ignoring port F for now
#define NPORTS 2
static port_t ports[NPORTS] = {{GPIOA, 0xffff, 0x0, 0x0, 0x0},
                               {GPIOB, 0x01fb, 0x0, 0x0, 0x0}};

static port_t *get_port(uint32_t gpioport) {
    port_t *retval = NULL;
    for (int i = 0; i < NPORTS; i++) {
        if (ports[i].name == gpioport) {
            retval = &ports[i];
            break;
        }
    }
    assert(retval != NULL);
    return retval;
}

void gpio_mode_setup(uint32_t gpioport, uint8_t mode, uint8_t pull_up_down,
                     uint16_t gpios) {
    port_t *port = get_port(gpioport);
    assert((port->valid_pins & gpios) == gpios);

    switch (mode) {
    case GPIO_MODE_OUTPUT:
        port->in_pins &= ~gpios;
        port->out_pins |= gpios;
        // TODO figure out what happens with pin state when changing from
        //      input to output.
        break;
    case GPIO_MODE_INPUT:
        port->out_pins &= ~gpios;
        port->in_pins |= gpios;
        break;
    default:
        // not yet implemented, or invalid
        assert(0);
    }
    return;
}

void gpio_set(uint32_t gpioport, uint16_t gpios) {
    port_t *port = get_port(gpioport);
    assert((port->valid_pins & gpios) == gpios);
    assert((port->out_pins & gpios) == gpios);
    port->pin_states |= gpios;
}

void gpio_clear(uint32_t gpioport, uint16_t gpios) {
    port_t *port = get_port(gpioport);
    assert((port->valid_pins & gpios) == gpios);
    assert((port->out_pins & gpios) == gpios);
    port->pin_states &= ~gpios;
}

uint16_t fake_gpio_get(uint32_t gpioport, uint16_t mask) {
    port_t *port = get_port(gpioport);
    return port->pin_states & mask;
}
