#include "unity.h"
#include "../src/adc.h"
#include "../fake/fake_stm_adc.h"

void test_adc_values(void) {
    uint8_t chans[] = {1, 2, 5};
    adc_init(3, chans, ADC_MODE_SCAN_INFINITE);
    fake_stm_adc_set(1, 13);
    fake_stm_adc_set(5, 1011);
    TEST_ASSERT_EQUAL(13, adc_read(1));
    TEST_ASSERT_EQUAL(0, adc_read(2));
    TEST_ASSERT_EQUAL(1011, adc_read(5));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_adc_values);
    return UNITY_END();
}
