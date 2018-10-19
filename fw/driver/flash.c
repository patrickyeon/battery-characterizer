#include "./flash.h"

#include <libopencm3/stm32/flash.h>

uint8_t flash_read(uint32_t address, uint8_t *buff, size_t len) {
    for (int i = 0; i < len; i++) {
        buff[i] = *((uint8_t *)(address + i));
    }
    return len;
}

uint8_t flash_peek(uint32_t address) {
    return *(uint8_t *)address;
}

uint8_t flash_write(uint8_t *buff, uint32_t address, size_t len) {
    //  Address needs to be 16-bit aligned, and len needs to be even, due to how
    // the stm32 writes memory.
    // TODO: error-checking
    for (int i = 0; i < len; i += 2) {
        flash_program_half_word(address + i, *((uint16_t *)(buff + i)));
    }
    return len;
}

int flash_erase(uint8_t page) {
    flash_erase_page(USERFLASH_BASE + page * USERFLASH_PAGESIZE);
    return 0;
}

void flash_init(void) {
    flash_unlock();
}
