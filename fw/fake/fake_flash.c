#include "./fake_flash.h"
#include "../driver/flash.h"

#include <assert.h>

#define MAX_ADDR 0x7fff

static uint8_t storage[MAX_ADDR + 1];
static uint32_t _base;

static uint32_t reloc(uint32_t address) {
    assert(address >= _base);
    assert(address - _base <= MAX_ADDR);
    return address - _base;
}

uint8_t flash_read(uint32_t address, uint8_t *buff, size_t len) {
    address = reloc(address);
    for (int i = 0; i < len; i++) {
        buff[i] = storage[address + i];
    }
    return len;
}

uint8_t flash_peek(uint32_t address) {
    address = reloc(address);
    return storage[address];
}

uint8_t flash_write(uint8_t *buff, uint32_t address, size_t len) {
    address = reloc(address);
    for (int i = 0; i < len; i++) {
        // emulate flash storage, we can only clear bits
        storage[address + i] &= buff[i];
    }
    return len;
}

void fake_flash_init(uint32_t base) {
    _base = base;
    for (uint32_t i = 0; i < MAX_ADDR; i++) {
        storage[i] = 0xff;
    }
}

int flash_erase(uint8_t page) {
    assert(page < (MAX_ADDR + 1) / FLASH_PAGESIZE);
    for(int i = page * FLASH_PAGESIZE; i < (page + 1) * FLASH_PAGESIZE; i++) {
        storage[i] = 0xff;
    }
    return 0;
}
