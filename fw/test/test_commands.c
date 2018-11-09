#include "unity.h"
#include "../src/commands.h"
#include "../driver/usb.h"
#include "../fake/fake_usb.h"

#define cmd_sz 11
#define rsp_sz 15

void setup(void) {
    usb_init();
    fake_usb_clear_all();
    commands_init();
}

void test_ping(void) {
    setup();
    uint8_t packet[cmd_sz] = {CMD_STARTBYTE, CMD_PING};
    fake_usb_tx(packet, cmd_sz);
    commands_process();
    uint8_t resp[rsp_sz];
    for (int i = 0; i < rsp_sz; i++) {
        resp[i] = 0xff;
    }
    TEST_ASSERT_EQUAL(rsp_sz, fake_usb_rx(resp, rsp_sz));
    TEST_ASSERT_EQUAL(CMD_STARTBYTE, resp[0]);
    TEST_ASSERT_EQUAL(CMD_PONG, resp[1]);
    // make sure nothing else sneaks onto the queue
    TEST_ASSERT_EQUAL(0, fake_usb_rx(resp, rsp_sz));
    commands_process();
    commands_process();
    TEST_ASSERT_EQUAL(0, fake_usb_rx(resp, rsp_sz));
}

void test_split_packet(void) {
    setup();
    uint8_t packet[cmd_sz] = {CMD_STARTBYTE, CMD_PING};
    uint8_t resp[rsp_sz];
    for (int i = 0; i < rsp_sz; i++) {
        resp[i] = 0xff;
    }
    fake_usb_tx(packet, 2);
    commands_process();
    TEST_ASSERT_EQUAL(0, fake_usb_rx(resp, rsp_sz));
    fake_usb_tx(packet + 2, cmd_sz - 2);
    commands_process();
    TEST_ASSERT_EQUAL(rsp_sz, fake_usb_rx(resp, rsp_sz));
    TEST_ASSERT_EQUAL(CMD_STARTBYTE, resp[0]);
    TEST_ASSERT_EQUAL(CMD_PONG, resp[1]);
}

void test_ignore_noise(void) {
    setup();
    uint8_t packet[cmd_sz + 4] = {0, 0xff, (uint8_t)~CMD_STARTBYTE, 0,
                                  CMD_STARTBYTE, CMD_PING};
    uint8_t resp[rsp_sz];
    for (int i = 0; i < rsp_sz; i++) {
        resp[i] = 0xff;
    }
    fake_usb_tx(packet, cmd_sz + 4);
    commands_process();
    TEST_ASSERT_EQUAL(rsp_sz, fake_usb_rx(resp, rsp_sz));
    TEST_ASSERT_EQUAL(CMD_STARTBYTE, resp[0]);
    TEST_ASSERT_EQUAL(CMD_PONG, resp[1]);
    commands_process();
    TEST_ASSERT_EQUAL(0, fake_usb_rx(resp, rsp_sz));
}

void test_ignore_false_packet(void) {
    setup();
    uint8_t packet[cmd_sz + 4] = {CMD_STARTBYTE, CMD_PING, 0, 0,
                                  CMD_STARTBYTE, CMD_TIME_GET};
    packet[cmd_sz - 1] = 0x7; // bad CRC
    uint8_t resp[rsp_sz];
    for (int i = 0; i < rsp_sz; i++) {
        resp[i] = 0xff;
    }
    fake_usb_tx(packet, cmd_sz + 4);
    commands_process();
    // it could take two rounds to handle this, that's fine
    commands_process();
    TEST_ASSERT_EQUAL(rsp_sz, fake_usb_rx(resp, rsp_sz));
    TEST_ASSERT_EQUAL(CMD_STARTBYTE, resp[0]);
    TEST_ASSERT_EQUAL(CMD_TIME_SET, resp[1]);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_ping);
    RUN_TEST(test_split_packet);
    RUN_TEST(test_ignore_noise);
    RUN_TEST(test_ignore_false_packet);
    return UNITY_END();
}
