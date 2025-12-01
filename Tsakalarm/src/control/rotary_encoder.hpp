#ifndef ROTARY_ENCODER_HPP
#define ROTARY_ENCODER_HPP

#include <Arduino.h>

#define ROTARY_ENCODER_CLOCKWISE 0
#define ROTARY_ENCODER_ANTICLOCKWISE 1

void rotary_encoder_init();
void rotary_encoder_set_callback(void(*)(uint8_t));

void rotary_encoder_update();

#endif //ROTARY_ENCODER_HPP
