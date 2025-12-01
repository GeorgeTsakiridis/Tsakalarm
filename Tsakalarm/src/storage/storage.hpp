#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>

#define SETTING_ALARMS_START 0x0
#define SETTING_BRIGHTNESS_STRENGTH 0x48
#define SETTING_BRIGHTNESS_OFFSET 0x49
#define SETTING_BUZZER_MELODY 0x4A
#define SETTING_BUZZER_VOLUME 0x4B
#define SETTING_SNOOZE_TIMES 0x4C
#define SETTING_SNOOZE_DURATION 0x4D
#define SETTING_DOTS_BLINKING_SPEED 0x4E
#define SETTING_FIRST_BOOT 0x1FF

void storage_store_setting(int, uint8_t);
uint8_t storage_retrieve_setting(int);

#endif