#include "state_boot.hpp"
#include "sound/buzzer.hpp"
#include "display/digits.hpp"
#include "display/top_led.hpp"
#include "storage/storage.hpp"
#include "control/rotary_encoder.hpp"
#include "utils.hpp"
#include "rtc.hpp"
#include "states/states.hpp"

static void (*change_state_callback)(uint8_t);

static unsigned long last_millis = 0;
static uint8_t animation_index = 0;

enum boot_sub_states{
  BOOTING, 
  FIRST_SETUP_HELLO_MSG, 
  FIRST_SETUP_TIME, 
  FIRST_SETUP_DATE_DAY, 
  FIRST_SETUP_DATE_MONTH, 
  FIRST_SETUP_DATE_YEAR,
  FIRST_SETUP_HOUR,
  FIRST_SETUP_MINUTES
};

static enum boot_sub_states state;
static uint8_t temp_day = 0;
static uint8_t temp_month = 0;
static uint8_t temp_year = 0;
static uint8_t temp_hour = 0;
static uint8_t temp_minutes = 0;

void state_boot_init(){
  state = BOOTING;

  digits_set_brightness(5);
  top_led_set_state(1);

  //Check if it is the first time the device is turned on (or if it has been reset)
  if(storage_retrieve_setting(SETTING_FIRST_BOOT) != 1){//If yes we must display a Hi message and wait for user to press the top button
    state = FIRST_SETUP_HELLO_MSG;

    digits_set(0, DIGITS_TURN_OFF);
    digits_set(1, DIGITS_H);
    digits_set(2, DIGITS_TURN_OFF);
    digits_set(3, DIGITS_TURN_OFF);
    digits_set_dots(1);
    digits_update();

  }
  else{
    //Clock is already setup. Load some basic settings
      digits_set_brightness(storage_retrieve_setting(SETTING_BRIGHTNESS_STRENGTH));
      buzzer_set_volume(storage_retrieve_setting(SETTING_BUZZER_VOLUME));
      change_state_callback(STATE_SET_STANDBY);
  }
}

void state_boot_tick(unsigned long current_millis){
  switch(state){
    case BOOTING:
    {
      if (current_millis-last_millis >= 83) {//Loop every 83ms (~12Hz)
        last_millis = current_millis;
    
        digits_set(0, DIGITS_LOAD_ANIM_1 + animation_index);
        digits_set(1, DIGITS_LOAD_ANIM_1 + animation_index);
        digits_set(2, DIGITS_LOAD_ANIM_1 + animation_index);
        digits_set(3, DIGITS_LOAD_ANIM_1 + animation_index);
        digits_update();
        animation_index++;

        if(animation_index == 6)animation_index = 0;
      }
    }
    break;

    case FIRST_SETUP_HELLO_MSG:
    {
      if(current_millis - last_millis > 250){
        last_millis = current_millis;
        top_led_invert_state();
      }
    }
    break;

    case FIRST_SETUP_DATE_DAY:
    {
      digits_set(0, temp_day/10);
      digits_set(1, temp_day%10);

      if(current_millis - last_millis > 250){//Switch editing day on and off
        last_millis = current_millis;
        digits_invert_mask(DIGITS_MASK_D1 | DIGITS_MASK_D2);
      }

      digits_update();
    }
    break;

    case FIRST_SETUP_DATE_MONTH:
    {
      digits_set(2, temp_month/10);
      digits_set(3, temp_month%10);

      if(current_millis - last_millis > 250){//Switch editing month on and off
        last_millis = current_millis;
        digits_invert_mask(DIGITS_MASK_D3 | DIGITS_MASK_D4);
      }

      digits_update();
    }
    break;

    case FIRST_SETUP_DATE_YEAR:
    {
      digits_set(2, temp_year/10);
      digits_set(3, temp_year%10);
      digits_update();
    }
    break;

    case FIRST_SETUP_HOUR:
    {
      digits_set(0, temp_hour/10);
      digits_set(1, temp_hour%10);

      if(current_millis - last_millis > 250){//Switch editing hour on and off
        last_millis = current_millis;
        digits_invert_mask(DIGITS_MASK_D1 | DIGITS_MASK_D2);
      }

      digits_update();
    }
    break;

    case FIRST_SETUP_MINUTES:
    {
      digits_set(2, temp_minutes/10);
      digits_set(3, temp_minutes%10);

      if(current_millis - last_millis > 250){//Switch editing minutes on and off
        last_millis = current_millis;
        digits_invert_mask(DIGITS_MASK_D3 | DIGITS_MASK_D4);
      }

      digits_update();
    }
    break;

  }

}

void state_boot_rotary_encoder_rotation(uint8_t direction){
  buzzer_play_melody(0);

  switch(state){
    case FIRST_SETUP_DATE_DAY:
    {
      uint8_t max_days = days_of_month(temp_month-1, temp_year);
      if(direction == ROTARY_ENCODER_CLOCKWISE){
        if(temp_day == max_days)temp_day = 1;
        else temp_day++;
      }
      else{
        if(temp_day == 1) temp_day = max_days;
        else temp_day--;
      }
    }
    break;

    case FIRST_SETUP_DATE_MONTH:
    {
      if(direction == ROTARY_ENCODER_CLOCKWISE){
        if(temp_month == 12)temp_month = 1;
        else temp_month++;
      }
      else{
        if(temp_month == 1)temp_month = 12;
        else temp_month--;
      }
      
      uint8_t max_days = days_of_month(temp_month-1, temp_year);
      if(temp_day > max_days){
        temp_day = max_days;
        digits_set(0, temp_day/10);
        digits_set(1, temp_day%10);
      }
    }

    break;

    case FIRST_SETUP_DATE_YEAR:
    {
      if(direction == ROTARY_ENCODER_CLOCKWISE){
        if(temp_year == 99) temp_year = 0;
        else temp_year++;
      }
      else{
        if(temp_year == 0) temp_year = 99;
        else temp_year--;
      }
    }
    break;

    case FIRST_SETUP_HOUR:
    {
      if(direction == ROTARY_ENCODER_CLOCKWISE){
        if(temp_hour == 23)temp_hour = 0;
        else temp_hour++;
      }
      else{
        if(temp_hour == 0)temp_hour = 23;
        else temp_hour--;
      }
    }
    break;

    case FIRST_SETUP_MINUTES:
    {
      if(direction == ROTARY_ENCODER_CLOCKWISE){
        if(temp_minutes == 59)temp_minutes = 0;
        else temp_minutes++;
        
      }
      else{
        if(temp_minutes == 0)temp_minutes = 59;
        else temp_minutes--;
      }
    }
    break;

    default:
      break;

  }

}

void state_boot_top_button_pressed(){
  buzzer_play_melody(0);

  switch(state){
    case FIRST_SETUP_HELLO_MSG:
    {
      state = FIRST_SETUP_DATE_YEAR;

      digits_set_mask(DIGITS_MASK_ALL);
      digits_set(0, DIGITS_2);
      digits_set(1, DIGITS_0);
      digits_set(2, DIGITS_2);
      digits_set(3, DIGITS_5);
      digits_set_dots(0);
      digits_update();
      top_led_set_state(1);

      temp_year = 25;
    }
    break;

    case FIRST_SETUP_DATE_YEAR:
    {
      state = FIRST_SETUP_DATE_DAY;  
      temp_day = 1;
      temp_month = 1;

      digits_set_mask(DIGITS_MASK_ALL);
      digits_set(0, DIGITS_0);
      digits_set(1, DIGITS_1);
      digits_set(2, DIGITS_0);
      digits_set(3, DIGITS_1);
      digits_set_dots(1);
      digits_update();
      top_led_set_state(1);
    }
    break;

    case FIRST_SETUP_DATE_DAY:
    case FIRST_SETUP_DATE_MONTH:
      {
        state = FIRST_SETUP_HOUR;
        temp_hour = 0;
        temp_minutes = 0;
        digits_set_mask(DIGITS_MASK_ALL);
        digits_set(0, DIGITS_0);
        digits_set(1, DIGITS_0);
        digits_set(2, DIGITS_0);
        digits_set(3, DIGITS_0);
        digits_set_dots(1);
        digits_update();
        top_led_set_state(1);
      }
    break;

    case FIRST_SETUP_HOUR:
    case FIRST_SETUP_MINUTES:
    {
      //SETUP COMPLETE; STORE SETTINGS TO RTC AND CHANGE SCREEN
      rtc_set_value(RTC_YEAR, temp_year);
      rtc_set_value(RTC_MONTH, temp_month);
      rtc_set_value(RTC_DATE, temp_day);
      rtc_set_value(RTC_HOUR, temp_hour);
      rtc_set_value(RTC_MINUTE, temp_minutes);
      rtc_set_value(RTC_SECOND, 0);
      rtc_set_value(RTC_DAY_OF_WEEK, calculate_day_of_week(temp_day, temp_month, temp_year));
      
      for (uint8_t i = SETTING_ALARMS_START; i < SETTING_ALARMS_START+(8*3*3); i++){
          storage_store_setting(i, 0);
      }

      storage_store_setting(SETTING_FIRST_BOOT, 1);
      storage_store_setting(SETTING_BRIGHTNESS_STRENGTH, 5);
      storage_store_setting(SETTING_BRIGHTNESS_OFFSET, 0);
      storage_store_setting(SETTING_BUZZER_MELODY, 2);
      storage_store_setting(SETTING_BUZZER_VOLUME, 5);
      storage_store_setting(SETTING_SNOOZE_TIMES, 5);
      storage_store_setting(SETTING_SNOOZE_DURATION, 5);
      storage_store_setting(SETTING_DOTS_BLINKING_SPEED, 1);

      change_state_callback(STATE_SET_STANDBY);
    }
    break;
  };

}

void state_boot_back_button_pressed(){
  buzzer_play_melody(0);

  switch(state){

    case FIRST_SETUP_DATE_DAY:
      {
      digits_set_mask(DIGITS_MASK_ALL);
      state = FIRST_SETUP_DATE_MONTH;
      }
    break;
    
    case FIRST_SETUP_DATE_MONTH:
      {
      digits_set_mask(DIGITS_MASK_ALL);
      state = FIRST_SETUP_DATE_DAY;
      }
    break;

    case FIRST_SETUP_HOUR:
      {
      digits_set_mask(DIGITS_MASK_ALL);
      state = FIRST_SETUP_MINUTES;
      }
    break;
    
    case FIRST_SETUP_MINUTES:
      {
      digits_set_mask(DIGITS_MASK_ALL);
      state = FIRST_SETUP_HOUR;
      }
    break;

  }

}

void state_boot_change_state_callback(void (*f)(uint8_t)){
  change_state_callback = f;
}
