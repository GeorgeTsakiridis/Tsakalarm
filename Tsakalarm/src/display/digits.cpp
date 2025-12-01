#include "digits.hpp"
#include "control/pins.hpp"
#include "utils.hpp"

#define SEG_A 1
#define SEG_B 2
#define SEG_C 4
#define SEG_D 8
#define SEG_E 16
#define SEG_F 32
#define SEG_G 64

/*
        A
     ----------
     |        |
   F |    G   | B
     |--------|
   E |        | 
     |        | C
     ----------
        D
*/

static uint8_t dots_enabled = 0;
static uint8_t digits_values[] = {0, 0, 0, 0};
static uint8_t mask = DIGITS_MASK_ALL;

const static uint8_t digits[] = {
    SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F, //0
    SEG_B|SEG_C, //1
    SEG_A|SEG_B|SEG_D|SEG_E|SEG_G, //2
    SEG_A|SEG_B|SEG_C|SEG_D|SEG_G, //3
    SEG_B|SEG_C|SEG_F|SEG_G, //4
    SEG_A|SEG_C|SEG_D|SEG_F|SEG_G, //5
    SEG_A|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G, //6
    SEG_A|SEG_B|SEG_C, //7
    SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G, //8
    SEG_A|SEG_B|SEG_C|SEG_D|SEG_F|SEG_G, //9
    SEG_E|SEG_G, //r
    SEG_A|SEG_B|SEG_C|SEG_E|SEG_F|SEG_G, //A
    SEG_C|SEG_E|SEG_G, //n
    SEG_B|SEG_C|SEG_D|SEG_E|SEG_G, //d
    SEG_C|SEG_D|SEG_E, //u
    SEG_A|SEG_D|SEG_E|SEG_F|SEG_G, //E
    SEG_A|SEG_C|SEG_D|SEG_F|SEG_G, //S
    SEG_C|SEG_D|SEG_E|SEG_G, //o
    SEG_A|SEG_E|SEG_F|SEG_G, //F
    SEG_B|SEG_C|SEG_D|SEG_E|SEG_F, //U
    SEG_C|SEG_D|SEG_E|SEG_F|SEG_G, //b
    SEG_D|SEG_E|SEG_F|SEG_G, //t
    SEG_A, //load animation frame 1
    SEG_B, //load animation frame 2
    SEG_C, //load animation frame 3
    SEG_D, //load animation frame 4
    SEG_E, //load animation frame 5
    SEG_F, //load animation frame 6,
    SEG_B|SEG_C|SEG_E|SEG_F|SEG_G, //H
    SEG_E|SEG_F, //l
    SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_G, //a
    SEG_E, //i
    SEG_B|SEG_C|SEG_D|SEG_F|SEG_G, //y
    SEG_C|SEG_E|SEG_F|SEG_G, //h
    SEG_G, // - (minus)
};

void digits_init(){
    pinMode(SR_DATA_PIN, OUTPUT);
    digitalWrite(SR_DATA_PIN, LOW);
    pinMode(SR_ST_CLK_PIN, OUTPUT);
    digitalWrite(SR_ST_CLK_PIN, LOW);
    pinMode(SR_OUT_CLK_PIN, OUTPUT);
    digitalWrite(SR_OUT_CLK_PIN, LOW);
    pinMode(DIGITS_ENABLE_PIN, OUTPUT);
    digitalWrite(DIGITS_ENABLE_PIN, HIGH);
}

void digits_set_brightness(uint8_t brightness){
    
    //DIGITS_ENABLE_PIN is inverted!    
    if(brightness == 0){
        digitalWrite(DIGITS_ENABLE_PIN, HIGH);
    }
    else if(brightness == 255){
        digitalWrite(DIGITS_ENABLE_PIN, LOW);
    }
    else{
        uint8_t brightness_log = scale_value_log(1, 10, 1, 254, brightness);

        analogWrite(DIGITS_ENABLE_PIN, 255 - brightness_log);
    }

}

void digits_set(uint8_t digit, uint8_t value){
    if(value == 255){
        digits_values[digit] = 0;
    }
    else{
        digits_values[digit] = digits[value];
    }
}

void digits_set_dots(uint8_t en){
    dots_enabled = en;
}

void digits_invert_dots(){
    dots_enabled = !dots_enabled;
}

void digits_update(){
    
    digitalWrite(SR_ST_CLK_PIN, LOW);

    uint8_t digit_1_value =  ((mask & DIGITS_MASK_D1) ? digits_values[0] : 0);
    uint8_t digit_2_value =  ((mask & DIGITS_MASK_D2) ? digits_values[1] : 0);
    uint8_t digit_3_value =  ((mask & DIGITS_MASK_D3) ? digits_values[2] : 0);
    uint8_t digit_4_value =  ((mask & DIGITS_MASK_D4) ? digits_values[3] : 0);

    uint8_t b1 = digit_1_value | digit_2_value << 7; //First Shift Register
    uint8_t b2 = digit_2_value >> 1 | digit_3_value << 6; //Second Shift Register
    uint8_t b3 = digit_3_value >> 2 | digit_4_value << 5; //Third Shift Register
    uint8_t b4 = digit_4_value >> 3 | (dots_enabled && (mask&DIGITS_MASK_DOTS)) << 4; //Last Shift Register

    shiftOut(SR_DATA_PIN, SR_OUT_CLK_PIN, MSBFIRST, b4);
    shiftOut(SR_DATA_PIN, SR_OUT_CLK_PIN, MSBFIRST, b3);
    shiftOut(SR_DATA_PIN, SR_OUT_CLK_PIN, MSBFIRST, b2);
    shiftOut(SR_DATA_PIN, SR_OUT_CLK_PIN, MSBFIRST, b1);


    digitalWrite(SR_ST_CLK_PIN, HIGH);

}

void digits_set_mask(uint8_t new_mask){
    mask = new_mask;
}
void digits_invert_mask(uint8_t invert_mask){
    mask ^= invert_mask;
}