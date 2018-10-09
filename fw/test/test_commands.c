#include "unity.h"
#include "../src/commands.h"
#include "../driver/usb.h"
#include "../fake/fake_usb.h"

#define pkt_sz 8

void setup(void) {
    usb_init();
    fake_usb_clear_all();
}

void test_ping(void) {
    setup();
    uint8_t packet[pkt_sz] = {CMD_STARTBYTE, CMD_PING, 0, 0, 0, 0, 0, 0};
    fake_usb_tx(packet, pkt_sz);
    commands_process();
    uint8_t resp[pkt_sz] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    TEST_ASSERT_EQUAL(pkt_sz, fake_usb_rx(resp, pkt_sz));
    TEST_ASSERT_EQUAL(CMD_STARTBYTE, resp[0]);
    TEST_ASSERT_EQUAL(RESP_PONG, resp[1]);
    // make sure nothing else sneaks onto the queue
    TEST_ASSERT_EQUAL(0, fake_usb_rx(resp, pkt_sz));
    commands_process();
    commands_process();
    TEST_ASSERT_EQUAL(0, fake_usb_rx(resp, pkt_sz));
}

void test_split_packet(void) {
    setup();
    uint8_t packet[pkt_sz] = {CMD_STARTBYTE, CMD_PING, 0, 0, 0, 0, 0, 0};
    uint8_t resp[pkt_sz] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    fake_usb_tx(packet, 2);
    commands_process();
    TEST_ASSERT_EQUAL(0, fake_usb_rx(resp, pkt_sz));
    fake_usb_tx(packet + 2, pkt_sz - 2);
    commands_process();
    TEST_ASSERT_EQUAL(pkt_sz, fake_usb_rx(resp, pkt_sz));
    TEST_ASSERT_EQUAL(CMD_STARTBYTE, resp[0]);
    TEST_ASSERT_EQUAL(RESP_PONG, resp[1]);
}

void test_ignore_noise(void) {
    setup();
    uint8_t packet[pkt_sz + 4] = {0, 0xff, (uint8_t)~CMD_STARTBYTE, 0,
                                  CMD_STARTBYTE, CMD_PING, 0, 0, 0, 0, 0, 0};
    uint8_t resp[pkt_sz] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    fake_usb_tx(packet, pkt_sz + 4);
    commands_process();
    TEST_ASSERT_EQUAL(pkt_sz, fake_usb_rx(resp, pkt_sz));
    TEST_ASSERT_EQUAL(CMD_STARTBYTE, resp[0]);
    TEST_ASSERT_EQUAL(RESP_PONG, resp[1]);
    commands_process();
    TEST_ASSERT_EQUAL(0, fake_usb_rx(resp, pkt_sz));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_ping);
    RUN_TEST(test_split_packet);
    RUN_TEST(test_ignore_noise);
    return UNITY_END();
}
