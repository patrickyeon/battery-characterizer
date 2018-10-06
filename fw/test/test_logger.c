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

int add_many_lines(int start_i, int end_i, int ma, int mv) {
    for (int i = start_i; i <= end_i; i++) {
        timers_set_systime(i, 0);
        abs_time_t now = (abs_time_t){i, 0};
        int err = logger_log_iv(&now, LOG_IV_CHG_BAT0, ma - i, mv + i);
        if (err < 0) {
            return err;
        }
    }
    return 0;
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

void test_add_logline_after_timeout(void) {
    setup();
    logger_init();
    timers_set_systime(12, 0);
    abs_time_t now = (abs_time_t){13, 80};
    TEST_ASSERT_EQUAL(0, logger_log_iv(&now, LOG_IV_CHG_BAT0, 1800, 3700));
    timers_set_systime(0xf0000, 0);
    now = (abs_time_t){0xf0000, 12};
    TEST_ASSERT_EQUAL(1, logger_log_iv(&now, LOG_IV_CHG_BAT0, 600, 4200));

    log_msg_t buff;
    TEST_ASSERT_EQUAL(0, logger_read(1, &buff));
    _equal_iv_logmsg(1, 0xf0000, LOG_IV_CHG_BAT0, 600, 4200, buff);
    return;
}

void test_log_into_next_page(void) {
    setup();
    logger_init();
    for (int i = 0; i < 130; i++) {
        timers_set_systime(i + 5, 0);
        abs_time_t now = (abs_time_t){i + 5, 13};
        TEST_ASSERT_EQUAL(i, logger_log_iv(&now, LOG_IV_CHG_BAT0,
                                           1800 - i, 3700 + i));
    }

    log_msg_t buff;

    for (int i = 0; i < 130; i++) {
        TEST_ASSERT_EQUAL(0, logger_read(i, &buff));
        _equal_iv_logmsg(i, i+5, LOG_IV_CHG_BAT0, 1800 - i, 3700 + i, buff);
    }
}

void test_log_full(void) {
    setup();
    logger_init();
    for (int i = 0; i < 2; i++) {
        timers_set_systime(i * 0xf0000, 0);
        abs_time_t t = (abs_time_t){i * 0xf0000, 99};
        TEST_ASSERT(logger_log_iv(&t, LOG_IV_CHG_BAT0, 188, 3777) >= 0);
    }
    // we should now be on the third page, so when this one fills we're full
    for (int i = 0; i < 127; i++) {
        timers_set_systime(2 * 0xf0000 + i + 2, 0);
        abs_time_t t = (abs_time_t){2 * 0xf0000 + i + 2, 333};
        TEST_ASSERT(logger_log_iv(&t, LOG_IV_CHG_BAT0, 180, 3700) >= 0);
    }
    // this should be one too many
    timers_set_systime(2 * 0xf0000 + 149, 0);
    abs_time_t t = (abs_time_t){2 * 0xf0000 + 149, 30};
    TEST_ASSERT(logger_log_iv(&t, LOG_IV_CHG_BAT0, 12, 333) < 0);
}

void test_dequeue_simple(void) {
    setup();
    logger_init();
    add_many_lines(0, 4, 1600, 4000);
    
    log_msg_t buff;
    for (int i = 0; i < 5; i++) {
        TEST_ASSERT_EQUAL(0, logger_dequeue(&buff));
        _equal_iv_logmsg(i, i, LOG_IV_CHG_BAT0, 1600 - i, 4000 + i, buff);
    }
    TEST_ASSERT(logger_dequeue(&buff) < 0);
}

void test_dequeue_empty_log(void) {
    setup();
    logger_init();
    log_msg_t buff;
    TEST_ASSERT(logger_dequeue(&buff) < 0);
}

void test_dequeue_through_unfilled_page(void) {
    setup();
    logger_init();
    add_many_lines(0, 4, 1600, 4000);
    // reset, so that it'll have to bump to the next page
    logger_init();
    add_many_lines(5, 9, 1600, 4000);

    log_msg_t buff;
    for (int i = 0; i < 10; i++) {
        TEST_ASSERT_EQUAL(0, logger_dequeue(&buff));
        _equal_iv_logmsg(i, i, LOG_IV_CHG_BAT0, 1600 - i, 4000 + i, buff);
    }
    TEST_ASSERT(logger_dequeue(&buff) < 0);
}

void test_log_after_dequeueing_all(void) {
    setup();
    logger_init();
    add_many_lines(0, 4, 1600, 3500);
    log_msg_t buff;
    for (int i = 0; i < 5; i++) {
        logger_dequeue(&buff);
    }

    timers_set_systime(6, 0);
    abs_time_t now = (abs_time_t){6, 7};
    TEST_ASSERT_EQUAL(5, logger_log_iv(&now, LOG_IV_CHG_BAT0, 1200, 3800));
    TEST_ASSERT_EQUAL(0, logger_dequeue(&buff));
    _equal_iv_logmsg(5, 6, LOG_IV_CHG_BAT0, 1200, 3800, buff);
}

void test_fill_log_then_dequeue_to_free_space(void) {
    setup();
    logger_init();
    add_many_lines(0, 4, 1600, 3500);
    logger_init();
    add_many_lines(5, 9, 1600, 3500);
    logger_init();
    add_many_lines(10, 15, 1600, 3500);
    logger_init();
    abs_time_t now = systime();
    TEST_ASSERT(logger_log_iv(&now, LOG_IV_CHG_BAT0, 1200, 3200) < 0);
    for (int i = 0; i < 5; i++) {
        log_msg_t buff;
        logger_dequeue(&buff);
    }
    TEST_ASSERT_EQUAL(16, logger_log_iv(&now, LOG_IV_CHG_BAT0, 1200, 3200));
}

int main(void) {
    UNITY_BEGIN();
    // this needs to run first because the logger is stateful :/
    RUN_TEST(test_fail_add_logline_no_init);
    RUN_TEST(test_add_logline);
    RUN_TEST(test_add_logline_after_timeout);
    RUN_TEST(test_log_into_next_page);
    RUN_TEST(test_log_full);
    RUN_TEST(test_dequeue_simple);
    RUN_TEST(test_dequeue_empty_log);
    RUN_TEST(test_dequeue_through_unfilled_page);
    RUN_TEST(test_log_after_dequeueing_all);
    RUN_TEST(test_fill_log_then_dequeue_to_free_space);
    return UNITY_END();
}
