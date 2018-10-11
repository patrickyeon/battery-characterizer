#include "unity.h"
#include "../src/config.h"

void setup(void) {
}

void test_hwid(void) {
    setup();
    config_init();
    // for testing, we compile with HWID=0x1337
    TEST_ASSERT_EQUAL(0x1337, config_hwid());
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_hwid);
    return UNITY_END();
}
