#include "unity.h"
#include "../src/adc.h"
#include "../src/director.h"
#include "../src/gpio.h"
#include "../src/pindefs.h"
#include "../src/temperature.h"
#include "../fake/fake_stm_adc.h"
#include "../fake/fake_at30ts74.h"
#include "../fake/fake_gpio.h"

#include <stdbool.h>

void setup(int16_t setpoint, uint16_t vcells) {
    gpio_init();
    uint8_t chans[] = {CHAN_VB0, CHAN_VB1, CHAN_VB2, CHAN_VB3};
    adc_init(4, chans, ADC_MODE_SCAN_INFINITE);
    director_init();
    fake_at30ts74_set(TSENS0, setpoint);
    fake_at30ts74_set(TSENS1, setpoint);
    fake_at30ts74_set(TSENS2, setpoint);
    fake_at30ts74_set(TSENS3, setpoint);

    fake_stm_adc_set(CHAN_VB0, adc_mv_to_code(vcells, 6600));
    fake_stm_adc_set(CHAN_VB1, adc_mv_to_code(vcells, 6600));
    fake_stm_adc_set(CHAN_VB2, adc_mv_to_code(vcells, 6600));
    fake_stm_adc_set(CHAN_VB3, adc_mv_to_code(vcells, 6600));
    adc_scan();
}

void _assert_cenden(bool cena, bool cenb, bool dena, bool denb) {
    TEST_ASSERT_EQUAL(cena, director.cenA);
    TEST_ASSERT_EQUAL(cenb, director.cenB);
    TEST_ASSERT_EQUAL(dena, director.denA);
    TEST_ASSERT_EQUAL(denb, director.denB);

    TEST_ASSERT_EQUAL((cena? CEN_A : 0), fake_gpio_get(GPIOB, CEN_A));
    TEST_ASSERT_EQUAL((cenb? CEN_B : 0), fake_gpio_get(GPIOA, CEN_B));
    TEST_ASSERT_EQUAL((dena? DEN_A : 0), fake_gpio_get(GPIOB, DEN_A));
    TEST_ASSERT_EQUAL((denb? DEN_B : 0), fake_gpio_get(GPIOB, DEN_B));
}

void test_init(void) {
    setup(20, 3700);
    _assert_cenden(false, false, false, false);
    return;
}

void test_start_charge(void) {
    setup(20, 3700);
    int err = director_enable(CENA | CENB);
    TEST_ASSERT_EQUAL(0, err);
    _assert_cenden(true, true, false, false);
}

void test_start_and_stop_discharge(void) {
    setup(20, 3700);
    int err = director_enable(CENA | CENB | DENA | DENB);
    TEST_ASSERT_EQUAL(0, err);
    _assert_cenden(true, true, true, true);
    err = director_disable(DENA | DENB);
    TEST_ASSERT_EQUAL(0, err);
    _assert_cenden(true, true, false, false);
}

void test_direction_setting(void) {
    //  in chg_dischg direction, the cells over TSENS0/TSENS2 are charging and
    // the cells over TSENS1/TSENS3 are discharging
    setup(20, 3700);
    fake_at30ts74_set(TSENS0, 100);
    fake_at30ts74_set(TSENS3, 100);
    director_init();

    director_direction(CHG_DISCHG, DISCHG_CHG);
    director_enable(CENA | CENB | DENA | DENB);
    // CENA and CENB should not succeed, due to temp limits
    _assert_cenden(false, false, true, true);

    director_disable(DENA | DENB);
    director_direction(DISCHG_CHG, CHG_DISCHG);
    director_enable(CENA | CENB | DENA | DENB);
    _assert_cenden(true, true, false, false);
}

void test_direction_switch(void) {
    setup(20, 3700);
    director_init();
    TEST_ASSERT_EQUAL(CHG_DISCHG, director.dirA);
    TEST_ASSERT_EQUAL(CHG_DISCHG, director.dirB);

    director_enable(CENA | CENB | DENA | DENB);
    director_direction(CHG_DISCHG, DISCHG_CHG);
    TEST_ASSERT_EQUAL(CHG_DISCHG, director.dirA);
    TEST_ASSERT_EQUAL(DISCHG_CHG, director.dirB);
    // we should disable charging/discharging when we switch direction
    _assert_cenden(true, false, true, false);
}

void test_over_under_voltage(void) {
    setup(20, 3700);
    director_init();
    fake_stm_adc_set(CHAN_VB0, adc_mv_to_code(4300, 6600));
    fake_stm_adc_set(CHAN_VB3, adc_mv_to_code(2200, 6600));
    director_enable(CENA | CENB | DENA | DENB);

    _assert_cenden(false, true, true, false);
}

void test_stop_over_voltage(void) {
    setup(20, 3700);
    director_init();
    director_enable(CENA | CENB | DENA | DENB);
    uint32_t err =  director_checkup();
    TEST_ASSERT_EQUAL(0, err);
    _assert_cenden(true, true, true, true);

    fake_stm_adc_set(CHAN_VB0, adc_mv_to_code(4300, 6600));
    adc_scan();
    err = director_checkup();
    TEST_ASSERT_EQUAL(ERR_OVERVOLT_CELL0, err);
    _assert_cenden(false, true, true, true);

    fake_stm_adc_set(CHAN_VB3, adc_mv_to_code(4400, 6600));
    adc_scan();
    err = director_checkup();
    TEST_ASSERT_EQUAL(ERR_OVERVOLT_CELL3, err);
    _assert_cenden(false, true, true, false);
}

void test_stop_overvoltage_discharge(void) {
    setup(20, 3700);
    director_init();
    // reverse the direction on one pair, make sure that is caught
    director_direction(CHG_DISCHG, DISCHG_CHG);
    director_enable(CENA | CENB | DENA | DENB);
    _assert_cenden(true, true, true, true);

    fake_stm_adc_set(CHAN_VB0, adc_mv_to_code(4300, 6600));
    fake_stm_adc_set(CHAN_VB2, adc_mv_to_code(4300, 6600));
    adc_scan();

    uint32_t err = director_checkup();
    TEST_ASSERT_EQUAL(ERR_OVERVOLT_CELL0 | ERR_OVERVOLT_CELL2, err);
    _assert_cenden(false, true, true, false);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_init);
    RUN_TEST(test_start_charge);
    RUN_TEST(test_start_and_stop_discharge);
    RUN_TEST(test_direction_setting);
    RUN_TEST(test_direction_switch);
    RUN_TEST(test_over_under_voltage);
    RUN_TEST(test_stop_over_voltage);
    RUN_TEST(test_stop_overvoltage_discharge);
    return UNITY_END();
}
