#include "./fake_flash.h"
#include "../driver/flash.h"

#include <assert.h>

#define MAX_ADDR 0x7fff

static uint8_t storage[MAX_ADDR + 1];

uint8_t flash_read(uint32_t address, uint8_t *buff, size_t len) {
    assert(address + len <= MAX_ADDR);
    for (int i = 0; i < len; i++) {
        buff[i] = storage[address + i];
    }
    return len;
}

uint8_t flash_write(uint8_t *buff, uint32_t address, size_t len) {
    assert(address + len <= MAX_ADDR);
    for (int i = 0; i < len; i++) {
        // emulate flash storage, we can only clear bits
        storage[address + i] &= buff[i];
    }
    return len;
}

void fake_flash_init(void) {
    for (uint32_t i = 0; i < MAX_ADDR; i++) {
        storage[i] = 0xff;
    }
}
