#ifndef TOP_LED_HPP

#include <Arduino.h>

void top_led_init();
void top_led_set_state(uint8_t);
void top_led_invert_state();

#define TOP_LED_HPP
#endif // TOP_LED_HPP
