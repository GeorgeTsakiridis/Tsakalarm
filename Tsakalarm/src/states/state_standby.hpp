#ifndef STATE_STANDBY_HPP
#define STATE_STANDBY_HPP

#include <Arduino.h>

void state_standby_init();
void state_standby_tick(unsigned long current_time);

void state_standby_get_next_alarm_clock(void);

void state_standby_rotary_encoder_rotation(uint8_t direction);
void state_standby_top_button_pressed();
void state_standby_back_button_pressed();

void state_standby_change_state_callback(void (*)(uint8_t));

#endif // STATE_STANDBY_HPP