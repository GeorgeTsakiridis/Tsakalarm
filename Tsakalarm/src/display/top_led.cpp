#include "display/top_led.hpp"
#include "control/pins.hpp"
#include "utils.hpp"
#include "display/digits.hpp"

static uint8_t state;

void top_led_init(){
    pinMode(TOP_BTN_LED, OUTPUT);
    state = 0;
    digitalWrite(TOP_BTN_LED, LOW);
}

void top_led_set_state(uint8_t new_state){
    state = new_state;
    digitalWrite(TOP_BTN_LED, state);
}

void top_led_invert_state(){
    state = !state;
    digitalWrite(TOP_BTN_LED, state);
}