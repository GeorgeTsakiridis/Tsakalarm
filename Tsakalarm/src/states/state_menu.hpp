#ifndef STATE_MENU_HPP
#define STATE_MENU_HPP

#include <Arduino.h>

void state_menu_init();
void state_menu_tick(unsigned long current_time);

void state_menu_rotary_encoder_rotation(uint8_t direction);
void state_menu_top_button_pressed();
void state_menu_back_button_pressed();

void state_menu_change_state_callback(void (*)(uint8_t));

#endif