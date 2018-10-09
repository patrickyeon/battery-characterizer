#include "unity.h"
#include "../src/ringbuffer.h"

void test_create_empty(void) {
    mk_rb(rb, 64);
    TEST_ASSERT_EQUAL(64, rb_cap(&rb));
}

void test_no_rb_collision(void) {
    mk_rb(inbuff, 20);
    mk_rb(outbuff, 20);
    uint8_t instr[21] = "mimimimimimimimimimi";
    uint8_t outstr[21] = "txtxtxtxtxtxtxtxtxtx";
    rb_push(&inbuff, instr, 20);
    rb_push(&outbuff, outstr, 20);
    TEST_ASSERT_EQUAL_STRING_LEN(instr, inbuff.buff, 20);
    TEST_ASSERT_EQUAL_STRING_LEN(outstr, outbuff.buff, 20);

    uint8_t buff[21];
    rb_pop(&outbuff, buff, 20);
    TEST_ASSERT_EQUAL_STRING_LEN(outstr, buff, 20);
    rb_pop(&inbuff, buff, 20);
    TEST_ASSERT_EQUAL_STRING_LEN(instr, buff, 20);
}

void test_add_string(void) {
    mk_rb(rb, 64);
    uint8_t str[10] = "hello";
    TEST_ASSERT_EQUAL(5, rb_push(&rb, str, 5));
    TEST_ASSERT_EQUAL(64 - 5, rb_cap(&rb));
}

void test_read_string(void) {
    mk_rb(rb, 64);
    uint8_t str[10] = "hello\0";
    rb_push(&rb, str, 6);

    uint8_t outbuff[10];
    TEST_ASSERT_EQUAL(6, rb_pop(&rb, outbuff, 6));
    TEST_ASSERT_EQUAL_STRING(str, outbuff);
    TEST_ASSERT_EQUAL(0, rb_pop(&rb, outbuff, 6));
    TEST_ASSERT_EQUAL(64, rb_cap(&rb));
}

void test_do_not_overfill(void) {
    mk_rb(rb, 12);
    uint8_t str[40] = "test string too long\0";
    TEST_ASSERT_EQUAL(12, rb_push(&rb, str, 16));
    TEST_ASSERT_EQUAL(0, rb_cap(&rb));
    TEST_ASSERT_EQUAL(0, rb_push(&rb, str + 4, 12));
    
    uint8_t outstr[12];
    TEST_ASSERT_EQUAL(12, rb_pop(&rb, outstr, 12));
    TEST_ASSERT_EQUAL_STRING_LEN(str, outstr, 12);
}

void test_do_not_overdrain(void) {
    mk_rb(rb, 12);
    uint8_t str[12] = "hello\0";
    rb_push(&rb, str, 6);
    uint8_t outstr[12] = "\0";
    TEST_ASSERT_EQUAL(6, rb_pop(&rb, outstr, 10));
    TEST_ASSERT_EQUAL_STRING(str, outstr);
}

void test_wraps_around(void) {
    mk_rb(rb, 10);
    uint8_t str[12] = "hi mom!\0";
    rb_push(&rb, str, 8);
    uint8_t outstr[12];
    rb_pop(&rb, outstr, 8);
    TEST_ASSERT_EQUAL(8, rb_push(&rb, str, 8));
    TEST_ASSERT_EQUAL(10 - 8, rb_cap(&rb));
    TEST_ASSERT_EQUAL(8, rb_pop(&rb, outstr, 10));
    TEST_ASSERT_EQUAL_STRING(str, outstr);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create_empty);
    RUN_TEST(test_add_string);
    RUN_TEST(test_read_string);
    RUN_TEST(test_do_not_overfill);
    RUN_TEST(test_do_not_overdrain);
    RUN_TEST(test_wraps_around);
    return UNITY_END();
}
