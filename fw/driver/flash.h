#ifndef FLASH_H
#define FLASH_H

#include <stdint.h>
#include <stdlib.h>

#define USERFLASH_PAGESIZE 1024
#define USERFLASH_BASE 0x08004800
#define USERFLASH_LEN (0x08008000 - USERFLASH_BASE)

uint8_t flash_read(uint32_t address, uint8_t *buff, size_t len);
uint8_t flash_peek(uint32_t address);
uint8_t flash_write(uint8_t *buff, uint32_t address, size_t len);
int flash_erase(uint8_t page);
void flash_init(void);

#endif // FLASH_H
