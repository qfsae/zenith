#include <Arduino.h>
#include <SPI.h>

#include "steering_io.h"

#include <EVE.h>
#include "tft_data.h"
#include "tft.h"

#include "screens.h"

// Only do print statements if we are in debug mode
#define STEERING_DEBUG 1

void setup() {
    Serial2.begin(115200);
    pinMode(EVE_CS, OUTPUT);
    digitalWrite(EVE_CS, HIGH);
    pinMode(EVE_PDN, OUTPUT);
    digitalWrite(EVE_PDN, LOW);

    SPI.begin(); // Set the SPI MODE and clock frequency
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));

    TFT_init();

    // Display splash logo
    TFT_splash();
    delay(3000);
}

void loop() {
    static uint32_t previous_millis = 0;
    static uint8_t display_delay = 0;
    uint32_t current_millis;
    current_millis = millis();

    if ((current_millis - previous_millis) > 3) {
        // Execute touch display code every ~5 ms
        previous_millis = current_millis;

        TFT_touch();

        display_delay++;
        if (display_delay > 3) { // Execute display refresh every ~20ms
            TFT_display((tft_callback)display_code);
        }
    }
}
