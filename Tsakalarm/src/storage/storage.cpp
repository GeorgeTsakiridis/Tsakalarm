#include "storage.hpp"
#include <EEPROM.h>

// SETTING_ALARMS_START 0x0
// SETTING_BRIGHTNESS_STRENGTH 0x3F
// SETTING_BRIGHTNESS_OFFSET 0x40
// SETTING_BUZZER_MELODY 0x41
// SETTING_BUZZER_VOLUME 0x42
// SETTING_SNOOZE_TIMES 0x43
// SETTING_SNOOZE_DURATION 0x44
// SETTING_DOTS_BLINKING_SPEED 0x45
// SETTING_FIRST_BOOT 0x1FF

//Do not use the actual storage and return some preset values. Useful for debugging.
#define STORAGE_NO_USE

#ifdef STORAGE_NO_USE
#define FIRST_BOOT 1

static uint8_t clocks[] = {
    1, 23, 59, //One Time
    1, 0, 1,
    0, 10, 0,
    0, 0, 0, //Day 1 Monday
    0, 0, 0,
    0, 0, 0,
    1, 2, 0, //Day 2 Tuesday
    1, 2, 15,
    1, 22, 57,
    1, 5, 0, //Day 3 Wednsday
    1, 16, 20,
    1, 16, 30,
    1, 1, 30, //Day 4 Thursday
    1, 18, 44,
    1, 18, 42,
    0, 0, 0, //Day 5 Friday
    1, 8, 0,
    0, 0, 0,
    0, 0, 0, //Day 6 Saturday
    0, 0, 0,
    0, 0, 0,
    1, 8, 0, //Day 7 Sunday
    0, 3, 25,
    0, 0, 0,
};
static uint8_t brightness_strength = 5;
static uint8_t brightness_offset = 2;
static uint8_t buzzer_melody = 3;
static uint8_t buzzer_volume = 4;
static uint8_t snooze_times = 2;
static uint8_t snooze_duration = 3;
static uint8_t dots_blinking_speed = 2;
static uint8_t first_boot = FIRST_BOOT;

#endif

void storage_store_setting(int address, uint8_t value){
    #ifndef STORAGE_NO_USE
        EEPROM.put(address, value);
    #else
        switch (address) {
            case SETTING_BRIGHTNESS_STRENGTH:
                brightness_strength = value;
                break;
            case SETTING_BRIGHTNESS_OFFSET:
                brightness_offset = value;
                break;
            case SETTING_BUZZER_MELODY:
                buzzer_melody = value;
                break;
            case SETTING_BUZZER_VOLUME:
                buzzer_volume = value;
                break;
            case SETTING_SNOOZE_TIMES:
                snooze_times = value;
                break;
            case SETTING_SNOOZE_DURATION:
                snooze_duration = value;
                break;
            case SETTING_DOTS_BLINKING_SPEED:
                dots_blinking_speed = value;
                break;
            case SETTING_FIRST_BOOT:
                first_boot = value;
                break;

            default:
                //We want to store an alarm clock setting
                if(address >= SETTING_ALARMS_START && address < (SETTING_ALARMS_START+72)){
                    clocks[address+SETTING_ALARMS_START] = value;
                }
                break;
        }
    #endif
}

uint8_t storage_retrieve_setting(int address){
    #ifndef STORAGE_NO_USE
        uint8_t value;
        EEPROM.get(address, value);
        return value;
    #else
        switch (address) {
            case SETTING_BRIGHTNESS_STRENGTH:
                return brightness_strength;
                break;
            case SETTING_BRIGHTNESS_OFFSET:
                return brightness_offset;
                break;
            case SETTING_BUZZER_MELODY:
                return buzzer_melody;
                break;
            case SETTING_BUZZER_VOLUME:
                return buzzer_volume;
                break;
            case SETTING_SNOOZE_TIMES:
                return snooze_times;
                break;
            case SETTING_SNOOZE_DURATION:
                return snooze_duration;
                break;
            case SETTING_DOTS_BLINKING_SPEED:
                return dots_blinking_speed;
            case SETTING_FIRST_BOOT:
                return first_boot;
                 break;

            default:
                //We want to retrieve an alarm clock setting
                if(address >= SETTING_ALARMS_START && address < (SETTING_ALARMS_START+72)){
                    return clocks[address+SETTING_ALARMS_START];
                }

                return 0;
              break;
        }
    #endif
}
