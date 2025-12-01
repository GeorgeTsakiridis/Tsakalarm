#include "bt.hpp"
#include "utils.hpp"
#include "sound/buzzer.hpp"
#include "states/state_standby.hpp"
#include "rtc.hpp"
#include "storage/storage.hpp"
#include "display/digits.hpp"

static char command_buffer[32];
static char arguments_buffer[32];

enum parsing_state{
    PARSING_STATE_WAITING,
    PARSING_STATE_PARSING_COMMAND,
    PARSING_STATE_PARSING_ARGUMENTS
};

static enum parsing_state state;
static uint8_t command_buffer_index;
static uint8_t arguments_buffer_index;

void bt_init(void){
    pinMode(BT_ENABLE_PIN, OUTPUT);
    pinMode(BT_STATUS_PIN, INPUT);
    state = PARSING_STATE_WAITING;
    command_buffer_index = 0;
    arguments_buffer_index = 0;
}

void parse_command(){
    buzzer_play_melody_unstoppapble(1);

    if(command_buffer_index > 0){

        if(strcmp(command_buffer, "get alarms") == 0){
            for (uint8_t i = 0; i < 8; i++)
            {
                for(uint8_t j = 0; j < 3; j++){
                    uint8_t active;
                    uint8_t hour;
                    uint8_t minutes;
                    get_alarm_clock_raw(i, j, &active, &hour, &minutes);
                    Serial.write("#alarm^");
                    Serial.write(i);
                    Serial.write(j);
                    Serial.write(active);
                    Serial.write(hour);
                    Serial.write(minutes);
                    Serial.write('@');
                    // delay(10);
                }
            }
        }
        else if(strcmp(command_buffer, "get settings") == 0){
            Serial.write("#settings^");
            Serial.write(rtc_get_value(RTC_DATE));
            Serial.write(rtc_get_value(RTC_MONTH));
            Serial.write(rtc_get_value(RTC_YEAR));
            Serial.write(rtc_get_value(RTC_HOUR));
            Serial.write(rtc_get_value(RTC_MINUTE));
            Serial.write(rtc_get_value(RTC_SECOND));
            Serial.write(storage_retrieve_setting(SETTING_BRIGHTNESS_STRENGTH));
            Serial.write(storage_retrieve_setting(SETTING_BRIGHTNESS_OFFSET));
            Serial.write(storage_retrieve_setting(SETTING_BUZZER_MELODY));
            Serial.write(storage_retrieve_setting(SETTING_BUZZER_VOLUME));
            Serial.write(storage_retrieve_setting(SETTING_SNOOZE_TIMES));
            Serial.write(storage_retrieve_setting(SETTING_SNOOZE_DURATION));
            Serial.write(storage_retrieve_setting(SETTING_DOTS_BLINKING_SPEED));
            Serial.write('@');
        }
        else if(strcmp(command_buffer, "alarm set active") == 0 && arguments_buffer_index == 3){
            set_alarm_clock_active((uint8_t)arguments_buffer[0], (uint8_t)arguments_buffer[1], (uint8_t)arguments_buffer[2]);
            state_standby_init();
        }
        
        else if(strcmp(command_buffer, "alarm set time") == 0 && arguments_buffer_index == 4){
            set_alarm_clock_time((uint8_t)arguments_buffer[0], (uint8_t)arguments_buffer[1], (uint8_t)arguments_buffer[2], (uint8_t)arguments_buffer[3]);
            state_standby_get_next_alarm_clock();
            state_standby_init();
        }
        else if(strcmp(command_buffer, "setting set") == 0 && arguments_buffer_index > 1){
            uint8_t command = arguments_buffer[0];

            if(command == 0){//date command (day, month, year)
                uint8_t day = (uint8_t)arguments_buffer[1];
                uint8_t month = (uint8_t)arguments_buffer[2];
                uint8_t year = (uint8_t)arguments_buffer[3];

                rtc_set_value(RTC_DATE, day);
                rtc_set_value(RTC_MONTH, month);
                rtc_set_value(RTC_YEAR, year);
                rtc_set_value(RTC_DAY_OF_WEEK, calculate_day_of_week(day, month, year));
            }
            else if(command == 1){//time command (hour,minute, second)
                rtc_set_value(RTC_HOUR, (uint8_t)arguments_buffer[1]);
                rtc_set_value(RTC_MINUTE, (uint8_t)arguments_buffer[2]);
                rtc_set_value(RTC_SECOND, (uint8_t)arguments_buffer[3]);
            }
            else if(command == 2){//brightness command (brightness)
                storage_store_setting(SETTING_BRIGHTNESS_STRENGTH, (uint8_t)arguments_buffer[1]);
            }
            else if(command == 3){//brightness offset command (brightness_offset)
                storage_store_setting(SETTING_BRIGHTNESS_OFFSET, (uint8_t)arguments_buffer[1]);
            }
            else if(command == 4){//alarm tone command (alarm_tone)
                storage_store_setting(SETTING_BUZZER_MELODY, (uint8_t)arguments_buffer[1]);
            }
            else if(command == 5){//alarm volume command (alarm_volume)
                storage_store_setting(SETTING_BUZZER_VOLUME, (uint8_t)arguments_buffer[1]);

            }
            else if(command == 6){//snooze times command (snooze_times)
                storage_store_setting(SETTING_SNOOZE_TIMES, (uint8_t)arguments_buffer[1]);
            }
            else if(command == 7){//snooze duration command (snooze_duration)
                storage_store_setting(SETTING_SNOOZE_DURATION, (uint8_t)arguments_buffer[1]);
            }
            else if(command == 8){//blinking speed command (blinking_speed)
                storage_store_setting(SETTING_DOTS_BLINKING_SPEED, (uint8_t)arguments_buffer[1]);
            }
            
            state_standby_init();

        }

    }

}

void bt_tick(){

    if(bt_is_connected()){
        while(Serial.available()){
            char c = Serial.read();

            switch (state){

                case PARSING_STATE_WAITING:{
                    //command start
                    if(c == '#'){
                        command_buffer_index = 0;
                        memset(command_buffer, 0, 32);
                        state = PARSING_STATE_PARSING_COMMAND;
                    }
                }
                break;
                
                case PARSING_STATE_PARSING_COMMAND:{
                    if(c == '#'){
                        command_buffer_index = 0;
                        memset(command_buffer, 0, 32);
                    }
                    else if(c == '^'){
                        arguments_buffer_index = 0;
                        memset(arguments_buffer, 0, 32);
                        state = PARSING_STATE_PARSING_ARGUMENTS;
                    }
                    else{
                        command_buffer[command_buffer_index] = c;
                        command_buffer_index++;
                        if(command_buffer_index == 32){//if we exceed buffer size abandon parsing
                            state = PARSING_STATE_WAITING;
                        }
                    } 
                }
                break;

                case PARSING_STATE_PARSING_ARGUMENTS:{
                    if(c == '#'){
                        command_buffer_index = 0;
                        memset(command_buffer, 0, 32);
                        state = PARSING_STATE_PARSING_COMMAND;
                    }
                    else if(c == '^'){//Did not expect that, abandon
                        state = PARSING_STATE_WAITING;
                    }
                    else if(c == '@'){//execute command
                        parse_command();
                        state = PARSING_STATE_WAITING;
                    }
                    else{
                        arguments_buffer[arguments_buffer_index] = c;
                        arguments_buffer_index++;
                        if(arguments_buffer_index == 32){
                            state = PARSING_STATE_WAITING;
                        }
                    }
                }
                break;

            }
        }
    }

}

void bt_set_active(uint8_t active){
    digitalWrite(BT_ENABLE_PIN, active);

    if(active){
        Serial.begin(9600);
    }
    else{
        Serial.end();
    }

}

void bt_send(const char*);
void bt_send_ok(const char*);
void bt_send_error(const char*);

uint8_t bt_is_connected(){
    return digitalRead(BT_STATUS_PIN);
}