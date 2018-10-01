#include "unity.h"
#include "../src/logger.h"
#include "../src/timers.h"
#include "../fake/fake_flash.h"

void setup(void) {
    fake_flash_init();
    logger_init();
}

void test_empty_log(void) {
    logger_init();
    TEST_ASSERT_EQUAL(0, logger_len());
}

void test_add_logline(void) {
    setup();
    TEST_ASSERT_EQUAL(0, logger_initpage(0));
    abs_time_t now = (abs_time_t){1, 20};
    TEST_ASSERT_EQUAL(0, logger_log_iv(&now, LOG_IV_CHG_BAT0, 1800, 3700));
    now = (abs_time_t){1, 40};
    TEST_ASSERT_EQUAL(1, logger_log_iv(&now, LOG_IV_CHG_BAT0, 1901, 3705));

    log_msg_t buff;
    uint16_t mv, ma;

    TEST_ASSERT_EQUAL(0, logger_read(0, &buff));
    TEST_ASSERT_EQUAL(0, buff.seqnum);
    TEST_ASSERT_EQUAL(1, buff.timestamp); // timestamps are seconds
    TEST_ASSERT_EQUAL(LOG_IV_CHG_BAT0, buff.type);
    logger_payload_to_ma_mv(buff.payload, &ma, &mv);
    TEST_ASSERT_EQUAL(3700, mv);
    TEST_ASSERT_EQUAL(1800, ma);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_empty_log);
    RUN_TEST(test_add_logline);
    return UNITY_END();
}
