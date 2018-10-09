#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdint.h>

typedef struct rbuff_t {
    uint8_t *buff;
    size_t start;
    size_t end;
    size_t len;
} rbuff_t;

int rb_cap(rbuff_t *rb);
int rb_push(rbuff_t *rb, uint8_t *buff, size_t len);
int rb_pop(rbuff_t *rb, uint8_t *buff, size_t len);

#define mk_rb(name, len) \
    uint8_t _buff_##name[len + 1];\
    rbuff_t  name = {_buff_##name, 0, 0, len + 1};

#endif // RINGBUFFER_H
