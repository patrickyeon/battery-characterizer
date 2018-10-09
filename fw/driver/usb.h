#ifndef USB_H
#define USB_H

#include <stdlib.h>
#include <stdint.h>

void usb_init(void);
void usb_poll(void);

int usb_read(uint8_t *buff, size_t len);
uint8_t usb_peek(void);
int usb_write(uint8_t *buff, size_t len);
int usb_readlen(void);

#endif // USB_H
