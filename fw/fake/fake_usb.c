#include "./fake_usb.h"
#include "../driver/usb.h"
#include "../src/ringbuffer.h"

//  In here, tx and rx and from the user's context (opposite the device's
// context). So to fake out a command/response pair, we send the command uisng
// fake_usb_tx(), which stores it in the tx_buff and usb_read can access that.
// Then the code under test can use usb_write to send a response, which we will
// store in rx_buff until we pull it out with fake_usb_rx.
mk_rb(tx_buff, 1024);
mk_rb(rx_buff, 1024);

void usb_init(void) {
}

void usb_poll(void) {
}

int usb_read(uint8_t *buff, size_t len) {
    return rb_pop(&tx_buff, buff, len);
}

uint8_t usb_peek(void) {
    // it's up to the caller to make sure there's something to peek at
    if (tx_buff.start == tx_buff.end) {
        // there isn't anything
        return 0;
    }
    return tx_buff.buff[tx_buff.start];
}

int usb_write(uint8_t *buff, size_t len) {
    return rb_push(&rx_buff, buff, len);
}

int usb_readlen(void) {
    return 1024 - rb_cap(&tx_buff);
}

void fake_usb_clear_all(void) {
    uint8_t devnull[1024];
    if (rb_cap(&tx_buff) < 1024) {
        rb_pop(&tx_buff, devnull, 1024 - rb_cap(&tx_buff));
    }
    if (rb_cap(&rx_buff) < 1024) {
        rb_pop(&rx_buff, devnull, 1024 - rb_cap(&rx_buff));
    }
}

void fake_usb_tx(uint8_t *buff, size_t len) {
    rb_push(&tx_buff, buff, len);
}

int fake_usb_rx(uint8_t *buff, size_t len) {
    return rb_pop(&rx_buff, buff, len);
}
