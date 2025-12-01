#ifndef buzzer_h
#define buzzer

#include <Arduino.h>

void buzzer_init();
void buzzer_play_melody(uint8_t);
void buzzer_play_melody_unstoppapble(uint8_t melody);
void buzzer_stop_melody();
void buzzer_set_volume(uint8_t);
void buzzer_tick(unsigned long);
uint8_t buzzer_is_playing();

#endif