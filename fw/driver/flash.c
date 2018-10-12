#include "./flash.h"

#include <libopencm3/stm32/flash.h>

uint8_t flash_read(uint32_t address, uint8_t *buff, size_t len) {
    for (int i = 0; i < len; i++) {
        buff[i] = *((uint32_t *)(address + i));
    }
    return len;
}

uint8_t flash_peek(uint32_t address) {
    return *(uint8_t *)address;
}

uint8_t flash_write(uint8_t *buff, uint32_t address, size_t len) {
    // FIXME current only works right with len == n*4
    for (int i = 0; i < len; i += 4) {
        flash_program_word(address + i, *((uint32_t *)(buff + i)));
    }
    return len;
}

int flash_erase(uint8_t page) {
    // FIXME this is hacky to hardcode here
    flash_erase_page(0x08004000 + page * FLASH_PAGESIZE);
    return 0;
}

void flash_init(void) {
    flash_unlock();
}
