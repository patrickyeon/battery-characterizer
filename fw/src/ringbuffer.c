#include <stdlib.h>

#include "./ringbuffer.h"

int rb_cap(rbuff_t *rb) {
    if (rb->end < rb->start) {
        return rb->len - (rb->end + rb->len - rb->start) - 1;
    } else {
        return rb->len - (rb->end - rb->start) - 1;
    }
}

int rb_pop(rbuff_t *rb, uint8_t *buff, size_t len) {
    int i;
    for (i = 0; i < len && rb->start != rb->end; i++) {
        buff[i] = rb->buff[rb->start++];
        rb->start %= rb->len;
    }
    return i;
}

int rb_push(rbuff_t *rb, uint8_t *buff, size_t len) {
    int i;
    for (i = 0; i < len && (rb->end + 1) % rb->len != rb->start; i++) {
        rb->buff[rb->end++] = buff[i];
        rb->end %= rb->len;
    }
    return i;
}
