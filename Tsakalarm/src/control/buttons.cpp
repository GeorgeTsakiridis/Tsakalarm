#include <Arduino.h>
#include "buttons.hpp"
#include "pins.hpp"

static void(*back_cal_func)();
static void(*top_cal_func)();

static uint8_t back_last_state;
static uint8_t top_last_state;

void buttons_init(){

    back_last_state = 0;
    top_last_state = 0;

    pinMode(ROT_ENC_BTN, INPUT_PULLUP);
    //Top button pin (A6) is special. It doesn't support digital read or write is always
    //in INPUT mode. We must not change it as it will lead to strange behaviour.

}

void buttons_set_callbacks(void(*back_callback_func)(), void(*top_callback_func)()){
    back_cal_func = back_callback_func;
    top_cal_func = top_callback_func;
}

void buttons_update(){
    //Remember, states are inverted!
    
    uint8_t back_state = !digitalRead(ROT_ENC_BTN);
    uint8_t top_state = analogRead(TOP_BTN) < 512;

    if(back_last_state == 0 && back_state == 1){
        back_cal_func();
    }


    if(top_last_state == 0 && top_state == 1){
        top_cal_func();
    }

    back_last_state = back_state;
    top_last_state = top_state;
}