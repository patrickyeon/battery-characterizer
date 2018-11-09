#include "unity.h"
#include "../src/timers.h"
#include "../fake/fake_time.h"

#include <libopencm3/cm3/nvic.h>

void test_simple_systick(void) {
    timers_init();
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 1000; j++) {
            abs_time_t t = systime();
            TEST_ASSERT_EQUAL(i, t.sec);
            TEST_ASSERT_EQUAL(j, t.ms);
            sys_tick_handler();
        }
    }
}

void test_tick_tock(void) {
    fake_time_init(48*1000*1000);
    timers_init();
    tick(TIMER_LOOP);
    fake_time_run(ms_to_ticks(10));
    uint16_t tpassed = tock(TIMER_LOOP);
    TEST_ASSERT_INT_WITHIN(3, 10*1000, tpassed);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_simple_systick);
    RUN_TEST(test_tick_tock);
    return UNITY_END();
}
