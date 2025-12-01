#ifndef STATE_BOOT_HPP
#define STATE_BOOT_HPP

#include <Arduino.h>

void state_boot_init();
void state_boot_tick(unsigned long current_time);

void state_boot_rotary_encoder_rotation(uint8_t direction);
void state_boot_top_button_pressed();
void state_boot_back_button_pressed();

void state_boot_change_state_callback(void (*)(uint8_t));

#endif