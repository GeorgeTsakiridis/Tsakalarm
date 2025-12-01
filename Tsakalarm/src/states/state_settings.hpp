#ifndef STATE_SETTINGS_HPP
#define STATE_SETTINGS_HPP

#include <Arduino.h>

void state_settings_init();
void state_settings_tick(unsigned long current_time);

void state_settings_rotary_encoder_rotation(uint8_t direction);
void state_settings_top_button_pressed();
void state_settings_back_button_pressed();

void state_settings_change_state_callback(void (*)(uint8_t));

#endif