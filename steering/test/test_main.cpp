#include <Arduino.h>
#include <unity.h>
#include "steering_io.h"

/*
void setUp(void) {
// set stuff up here
}

void tearDown(void) {
// clean stuff up here
}
*/

void test_led_builtin_pin_number(void) {
    TEST_ASSERT_EQUAL(PB14, STEERING_STATUS_LED);
}

void test_led_state_high(void) {
    digitalWrite(STEERING_STATUS_LED, HIGH);
    TEST_ASSERT_EQUAL(HIGH, digitalRead(STEERING_STATUS_LED));
}

void test_led_state_low(void) {
    digitalWrite(STEERING_STATUS_LED, LOW);
    TEST_ASSERT_EQUAL(LOW, digitalRead(STEERING_STATUS_LED));
}

void setup() {
    delay(2000);

    UNITY_BEGIN();
    RUN_TEST(test_led_builtin_pin_number);

    pinMode(STEERING_STATUS_LED, OUTPUT);
}

uint8_t i = 0;
uint8_t max_blinks = 5;

void loop() {
    if (i < max_blinks)
    {
        RUN_TEST(test_led_state_high);
        delay(500);
        RUN_TEST(test_led_state_low);
        delay(500);
        i++;
    }
    else if (i == max_blinks) {
      UNITY_END(); // stop unit testing
    }
}