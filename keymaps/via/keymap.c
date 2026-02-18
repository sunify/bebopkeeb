#include QMK_KEYBOARD_H
#include "timer.h"
#include <avr/io.h>

#define RESET_PIN D4
#define BTN1_PIN  D0
#define BTN2_PIN  C6

#define RESET_DELAY 1000

bool button_states[2] = {false, false};
static uint16_t reset_timer = 0;
static bool reset_started = false;
static bool reset_engaged = false;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_B, KC_P
    )
};

void matrix_scan_user(void) {
    bool b1 = readPin(BTN1_PIN) == 0;
    bool b2 = readPin(BTN2_PIN) == 0;

    if (b1 && b2) {
        if (!reset_started) {
            reset_timer = timer_read();
            reset_started = true;
            reset_engaged = false;
        }
        if (!reset_engaged && timer_elapsed(reset_timer) >= RESET_DELAY) {
            // удержали достаточно → тянем RST к земле
            setPinOutput(RESET_PIN);
            writePinLow(RESET_PIN);
            reset_engaged = true;
        }
    } else {
        // отпустили хотя бы одну кнопку → отпускаем RST и сбрасываем таймер
        setPinInput(RESET_PIN); // Hi-Z
        reset_started = false;
        reset_engaged = false;
    }
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    bool b1 = readPin(BTN1_PIN) == 0; // LOW = нажато
    bool b2 = readPin(BTN2_PIN) == 0;
    bool both_pressed = b1 && b2;

    return !both_pressed;
}
