#ifndef FLASH_H
#define FLASH_H

#include <stdint.h>
#include <stdlib.h>

#define FLASH_PAGESIZE 1024

uint8_t flash_read(uint32_t address, uint8_t *buff, size_t len);
uint8_t flash_peek(uint32_t address);
uint8_t flash_write(uint8_t *buff, uint32_t address, size_t len);

#endif // FLASH_H
