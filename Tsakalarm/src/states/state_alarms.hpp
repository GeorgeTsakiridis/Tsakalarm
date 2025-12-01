#ifndef STATE_ALARMS_HPP
#define STATE_ALARMS_HPP

#include <Arduino.h>

void state_alarms_init();
void state_alarms_tick(unsigned long current_time);

void state_alarms_rotary_encoder_rotation(uint8_t direction);
void state_alarms_top_button_pressed();
void state_alarms_back_button_pressed();

void state_alarms_change_state_callback(void (*)(uint8_t));

#endif