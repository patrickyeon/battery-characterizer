#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>

#include "../generated/msgs.h"

// commands are all the same size, have the following structure
// {0xA5, cmd_msg_e, payload (8 bytes), checksum (1 byte)}

void commands_init(void);
void commands_process(void);

#endif // COMMANDS_H
