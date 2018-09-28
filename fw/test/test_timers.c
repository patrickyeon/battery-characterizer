#include "unity.h"
#include "../src/timers.h"

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

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_simple_systick);
    return UNITY_END();
}
