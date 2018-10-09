#ifndef FAKE_USB_H
#define FAKE_USB_H

#include <stdlib.h>
#include <stdint.h>

void fake_usb_clear_all(void);
// these two are "write to device" and "read from device"
void fake_usb_tx(uint8_t *buff, size_t len);
int fake_usb_rx(uint8_t *buff, size_t len);

#endif // FAKE_USB_H
