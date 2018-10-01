#include "unity.h"
#include "../src/logger.h"
#include "../src/timers.h"
#include "../fake/fake_flash.h"

void setup(void) {
    fake_flash_init();
    logger_init();
}

void _equal_iv_logmsg(log_msg_t exp_meta, uint16_t exp_ma, uint16_t exp_mv,
                      log_msg_t got) {
    TEST_ASSERT_EQUAL(exp_meta.seqnum, got.seqnum);
    TEST_ASSERT_EQUAL(exp_meta.timestamp, got.timestamp);
    TEST_ASSERT_EQUAL(exp_meta.type, got.type);
    uint16_t got_ma, got_mv;
    logger_payload_to_ma_mv(got.payload, &got_ma, &got_mv);
    TEST_ASSERT_EQUAL(exp_ma, got_ma);
    TEST_ASSERT_EQUAL(exp_mv, got_mv);
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

void test_fail_add_logline_no_init(void) {
    setup();
    abs_time_t now = (abs_time_t){1, 22};
    TEST_ASSERT(logger_log_iv(&now, LOG_IV_CHG_BAT0, 1800, 3700) < 0);
}
void test_add_logline_after_timeout(void) {
    setup();
    timers_set_systime(12, 0);
    logger_initpage(0);
    abs_time_t now = (abs_time_t){13, 80};
    TEST_ASSERT_EQUAL(0, logger_log_iv(&now, LOG_IV_CHG_BAT0, 1800, 3700));
    timers_set_systime(0xf0000, 0);
    now = (abs_time_t){0xf0000, 12};
    TEST_ASSERT_EQUAL(1, logger_log_iv(&now, LOG_IV_CHG_BAT0, 600, 4200));

    log_msg_t buff;
    uint16_t mv, ma;
    TEST_ASSERT_EQUAL(0, logger_read(1, &buff));
    _equal_iv_logmsg((log_msg_t){1, 0xf0000, LOG_IV_CHG_BAT0}, 600, 4200,
                     buff);
    return;
}

void test_log_into_next_page(void) {
    setup();
    logger_initpage(0);
    for (int i = 0; i < 130; i++) {
        timers_set_systime(i + 5, 0);
        abs_time_t now = (abs_time_t){i + 5, 13};
        TEST_ASSERT_EQUAL(i, logger_log_iv(&now, LOG_IV_CHG_BAT0,
                                           1800 - i, 3700 + i));
    }

    log_msg_t buff;
    uint16_t mv, ma;

    for (int i = 0; i < 5; i++) {
        TEST_ASSERT_EQUAL(0, logger_read(i, &buff));
        _equal_iv_logmsg((log_msg_t){i, i+5, LOG_IV_CHG_BAT0},
                         1800 - i, 3700 + i, buff);
    }
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_empty_log);
    RUN_TEST(test_add_logline);
    RUN_TEST(test_fail_add_logline_no_init);
    RUN_TEST(test_add_logline_after_timeout);
    RUN_TEST(test_log_into_next_page);
    return UNITY_END();
}
