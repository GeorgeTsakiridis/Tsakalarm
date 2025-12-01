#include "state_boot.hpp"
#include "sound/buzzer.hpp"
#include "display/digits.hpp"
#include "display/top_led.hpp"
#include "storage/storage.hpp"
#include "states/states.hpp"
#include "rtc.hpp"
#include "utils.hpp"

static void (*change_state_callback)(uint8_t);

static unsigned long last_millis;
static unsigned long last_time_retrieve_millis;
static unsigned long last_time_500ms_millis;

static uint16_t snooze_duration;
static uint8_t snooze_counter;
static uint16_t ringing_time;

static uint16_t dots_blinking_interval;
static uint8_t current_hour;
static uint8_t current_minute;
static uint8_t last_second; //Used for counting seconds
static uint8_t current_second;
static uint8_t current_day_of_week;
static uint8_t last_current_day_of_week; //Used for checking for day change;

static uint8_t alarm_hour;
static uint8_t alarm_minutes;
static uint8_t alarm_is_one_time;
static uint8_t alarm_index; //Only valid/used for one time clocks, to know which clock to disable after ringing

enum standby_sub_states{
  WAITING, 
  ALARM_CLOCK_RINGING,
  WAITING_SNOOZED
};

static enum standby_sub_states state;

void state_standby_get_next_alarm_clock(){
  uint8_t next_alarm_hour = 255;
  uint8_t next_alarm_minutes = 255;

  for (uint8_t i = 0; i < 6; i++){
    get_alarm_clock(i, &alarm_hour, &alarm_minutes);

    //Candidate alarm's time is in the future.
    if(alarm_hour > current_hour || (alarm_hour == current_hour && alarm_minutes > current_minute)){
      if(alarm_hour < next_alarm_hour || (alarm_hour == next_alarm_hour && alarm_minutes < next_alarm_minutes)){
        next_alarm_hour = alarm_hour;
        next_alarm_minutes = alarm_minutes;
        alarm_is_one_time = i >= 3;
        alarm_index = i-3;
      }
    }

  }

  alarm_hour = next_alarm_hour;
  alarm_minutes = next_alarm_minutes;
}

void standby_set_state(enum standby_sub_states new_state){
 
  state = new_state;
  digits_set_mask(DIGITS_MASK_ALL);

  switch(new_state){

    case ALARM_CLOCK_RINGING:{
      last_second = current_second;
      ringing_time = 0;
      alarm_hour = 255;

      if(alarm_is_one_time){
        alarm_is_one_time = 0;
        set_alarm_clock_active(0, alarm_index, 0);
        alarm_index = 0;
      }

      top_led_set_state(1);
    }
    break;

    case WAITING_SNOOZED:{
      snooze_counter++;
      snooze_duration = 0;
      top_led_set_state(1);
      buzzer_stop_melody();
    }
    break;

    case WAITING:{
      snooze_counter = 0;
      alarm_hour = 255;
      alarm_is_one_time = 0;
      alarm_index = 0;
      top_led_set_state(0);
      buzzer_stop_melody();
    }

  }
}

void state_standby_init(){
  last_millis = 0;
  last_time_retrieve_millis = 0;
  last_time_500ms_millis = 0;

  dots_blinking_interval = storage_retrieve_setting(SETTING_DOTS_BLINKING_SPEED);
  if(dots_blinking_interval > 0) dots_blinking_interval = 1000 / dots_blinking_interval; 

  digits_set_dots(1);

  alarm_hour = 255;
  alarm_minutes = 255;

  last_current_day_of_week = 0;

  current_hour = rtc_get_value(RTC_HOUR);
  current_minute = rtc_get_value(RTC_MINUTE);
  current_second = rtc_get_value(RTC_SECOND);
  current_day_of_week = rtc_get_value(RTC_DAY_OF_WEEK);

  digits_set_brightness(storage_retrieve_setting(SETTING_BRIGHTNESS_STRENGTH));
  buzzer_set_volume(storage_retrieve_setting(SETTING_BUZZER_VOLUME));
}

void state_standby_tick(unsigned long current_millis){
  
  if(current_millis - last_time_retrieve_millis > 500){//Retrieve time from RTC 2 times per second
    last_time_retrieve_millis = current_millis;

    current_hour = rtc_get_value(RTC_HOUR);
    current_minute = rtc_get_value(RTC_MINUTE);
    current_second = rtc_get_value(RTC_SECOND);
    current_day_of_week = rtc_get_value(RTC_DAY_OF_WEEK);

    digits_set(0, current_hour/10);
    digits_set(1, current_hour%10);
    digits_set(2, current_minute/10);
    digits_set(3, current_minute%10);
    digits_update();

    if(alarm_hour == 255)
      state_standby_get_next_alarm_clock();

  }

  //In both states we must show the time and blink the dots
  if(state == WAITING || state == WAITING_SNOOZED){
    if(dots_blinking_interval > 0 &&  current_millis - last_millis >= dots_blinking_interval){
      last_millis = current_millis;
      digits_invert_mask(DIGITS_MASK_DOTS);
      digits_update();
    }

    //ALARM!
    if(state == WAITING && alarm_hour == current_hour && alarm_minutes == current_minute){
      standby_set_state(ALARM_CLOCK_RINGING);
    }

    //Day changed, check for new alarms
    if(last_current_day_of_week != current_day_of_week){
      last_current_day_of_week = current_day_of_week;
    }

    //If we are waiting with a snooze in progress, check if seconds passed are more than the snooze duration
    if(state == WAITING_SNOOZED && last_second != current_second){
      last_second = current_second;
      snooze_duration++;

      if(snooze_duration >= storage_retrieve_setting(SETTING_SNOOZE_DURATION)*60){
        standby_set_state(ALARM_CLOCK_RINGING);
      }

    }

  }
  //If we are ringing, blink all the lights and keep repeating the alarm melody.
  //Check if we are ringing for more than a minute. In that case consider it a snooze.
  else if(state == ALARM_CLOCK_RINGING){
    if(dots_blinking_interval > 0 && current_millis - last_millis >= dots_blinking_interval){
      last_millis = current_millis;
      digits_invert_mask(DIGITS_MASK_ALL);
      digits_update();
    }

    //A second passed!
    if(last_second != current_second){
      last_second = current_second;

      if(!buzzer_is_playing()){
        buzzer_play_melody(storage_retrieve_setting(SETTING_BUZZER_MELODY));
      }

      ringing_time++;
      
      //If we exceeded are ringing for 60 seconds then stop and auto-snooze
      if(ringing_time >= 60){

        //Exceeded snooze times set by user. Abandon alarm clock.
        if(snooze_counter >= storage_retrieve_setting(SETTING_SNOOZE_TIMES)){
          standby_set_state(WAITING);
        }
        else{
          standby_set_state(WAITING_SNOOZED);
        }

      }

    }

  }

}

void state_standby_rotary_encoder_rotation(uint8_t direction){
}

void state_standby_top_button_pressed(){
  if(state == ALARM_CLOCK_RINGING){
    //snoozing by button does not count for auto snooze.
    standby_set_state(WAITING_SNOOZED);
    snooze_counter--;
  }
}

void state_standby_back_button_pressed(){
  if(state == ALARM_CLOCK_RINGING || state == WAITING_SNOOZED){
    standby_set_state(WAITING);
  }
  else{
    change_state_callback(STATE_SET_MENU);
  }

}

void state_standby_change_state_callback(void (*f)(uint8_t)){
  change_state_callback = f;
}
