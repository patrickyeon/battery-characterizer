#include "unity.h"
#include "../src/scheduler.h"

#include <libopencm3/cm3/nvic.h>

int test_counter;

void inc_counter(void) {
    test_counter++;
}

void test_10hz_loop(void) {
    test_counter = 0;
    fp_t loop10hz[] = {&inc_counter};
    scheduler_init(1, loop10hz);
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 99; j++) {
            sys_tick_handler();
            scheduler_tick();
        }
        TEST_ASSERT_EQUAL(i, test_counter);
        scheduler_tick();
        TEST_ASSERT_EQUAL(i, test_counter);
        sys_tick_handler();
        scheduler_tick();
        TEST_ASSERT_EQUAL(i + 1, test_counter);
        scheduler_tick();
        TEST_ASSERT_EQUAL(i + 1, test_counter);
    }
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_10hz_loop);
    return UNITY_END();
}
