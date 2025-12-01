#include "rotary_encoder.hpp"
#include "pins.hpp"

static uint8_t last_a_state;
static uint8_t last_b_state;

static void(*callback)(uint8_t);

void rotary_encoder_init(){
    last_a_state = 0;
    last_b_state = 0;

    pinMode(ROT_ENC_A, INPUT_PULLUP);
    pinMode(ROT_ENC_B, INPUT_PULLUP);
}

void rotary_encoder_set_callback(void(*func)(uint8_t)){
    callback = func;
}

void rotary_encoder_update() {

    int a_state = !digitalRead(ROT_ENC_A);
    int b_state = !digitalRead(ROT_ENC_B);

    //Rotations must be inverted because the rotary encoder is mounted on the back of the device

    //A got pulled high and b is already high, so we have a CCW rotation
    if(a_state == 1 && last_a_state == 0 && b_state == 1){
        callback(ROTARY_ENCODER_CLOCKWISE);
    }

    //B got pulled high and a is already high, so we have a CW rotation
    else if(b_state == 1 && last_b_state == 0 && a_state == 1){
        callback(ROTARY_ENCODER_ANTICLOCKWISE);
    }

    last_a_state = a_state;
    last_b_state = b_state;
}
