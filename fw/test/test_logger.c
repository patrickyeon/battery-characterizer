#include "unity.h"
#include "../src/logger.h"
#include "../src/timers.h"
#include "../fake/fake_flash.h"

void setup(void) {
    timers_set_systime(0, 0);
    fake_flash_init();
}

void _equal_iv_logmsg(uint16_t exp_seqnum, uint32_t exp_time, uint8_t exp_type,
                      uint16_t exp_ma, uint16_t exp_mv,
                      log_msg_t got) {
    TEST_ASSERT_EQUAL(exp_seqnum, got.seqnum);
    TEST_ASSERT_EQUAL(exp_time, got.timestamp);
    TEST_ASSERT_EQUAL(exp_type, got.type);
    uint16_t got_ma, got_mv;
    logger_payload_to_ma_mv(got.payload, &got_ma, &got_mv);
    TEST_ASSERT_EQUAL(exp_ma, got_ma);
    TEST_ASSERT_EQUAL(exp_mv, got_mv);
}

void test_add_logline(void) {
    setup();
    logger_init();
    abs_time_t now = (abs_time_t){1, 20};
    TEST_ASSERT_EQUAL(0, logger_log_iv(&now, LOG_IV_CHG_BAT0, 1800, 3700));
    now = (abs_time_t){1, 40};
    TEST_ASSERT_EQUAL(1, logger_log_iv(&now, LOG_IV_CHG_BAT0, 1901, 3705));

    log_msg_t buff;
    TEST_ASSERT_EQUAL(0, logger_read(0, &buff));
    _equal_iv_logmsg(0, 1, LOG_IV_CHG_BAT0, 1800, 3700, buff);
    TEST_ASSERT_EQUAL(0, logger_read(1, &buff));
    _equal_iv_logmsg(1, 1, LOG_IV_CHG_BAT0, 1901, 3705, buff);
}

void test_fail_add_logline_no_init(void) {
    setup();
    abs_time_t now = (abs_time_t){1, 22};
    TEST_ASSERT(logger_log_iv(&now, LOG_IV_CHG_BAT0, 1800, 3700) < 0);
}

int main(void) {
    UNITY_BEGIN();
    // this needs to run first because the logger is stateful :/
    RUN_TEST(test_fail_add_logline_no_init);
    RUN_TEST(test_add_logline);
    return UNITY_END();
}
