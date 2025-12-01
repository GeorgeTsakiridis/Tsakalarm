#include "state_menu.hpp"
#include "sound/buzzer.hpp"
#include "display/digits.hpp"
#include "display/top_led.hpp"
#include "states/states.hpp"
#include "control/rotary_encoder.hpp"
#include "utils.hpp"
#include "rtc.hpp"
#include "storage/storage.hpp"

static void (*change_state_callback)(uint8_t);

/*
MENU_YEAR ----------------> SEL_YEAR
  |
MENU_DAY_MONTH -----------> SEL_DAY_MONTH_DD <--> SEL_DAY_MONTH_MM
  |
MENU_HOUR_MINUTES --------> SEL_HOUR_MINUTES_HH <--> SEL_HOUR_MINUTES_MM
  |
MENU_BRIGHTNESS_STRENGTH -> SEL_BRIGHTNESS_STRENGTH
  |
MENU_BRIGHTNESS_OFFSET ---> SEL_BRIGHTNESS_OFFSET
  |
MENU_BUZZER_MELODY -------> SEL_BUZZER_MELODY
  |
MENU_BUZZER_VOLUME -------> SEL_BUZZER_VOLUME
  |
MENU_SNOOZE_TIMES --------> SEL_SNOOZE_TIMES
  |
MENU_SNOOZE_DURATION -----> SEL_SNOOZE_DURATION
  |
MENU_DOTS_BLINKING_SPEED --> SEL_DOTS_BLINKING_SPEED
  |
MENU_BLUETOOTH -----------> (Change to bluetooth screen)
  |
MENU_RESET ---------------> SEL_RESET ---> SEL_RESET_CONF ----> X
*/

enum settings_sub_states{
    MENU_YEAR,
    MENU_DAY_MONTH,
    MENU_HOUR_MINUTES,
    MENU_BRIGHTNESS_STRENGTH,
    MENU_BRIGHTNESS_OFFSET,
    MENU_BUZZER_MELODY,
    MENU_BUZZER_VOLUME,
    MENU_SNOOZE_TIMES,
    MENU_SNOOZE_DURATION,
    MENU_DOTS_BLINKING_SPEED,
    MENU_BLUETOOTH,
    MENU_RESET,
    SEL_YEAR,
    SEL_DAY_MONTH_MM,
    SEL_DAY_MONTH_DD,
    SEL_HOUR_MINUTES_HH,
    SEL_HOUR_MINUTES_MM,
    SEL_BRIGHTNESS_STRENGTH,
    SEL_BRIGHTNESS_OFFSET,
    SEL_BUZZER_MELODY,
    SEL_BUZZER_VOLUME,
    SEL_SNOOZE_TIMES,
    SEL_SNOOZE_DURATION,
    SEL_DOTS_BLINKING_SPEED,
    SEL_RESET,
    SEL_RESET_CONF
};

static enum settings_sub_states state;
static const uint8_t menu_entries = 12;
static uint8_t update_display;
static uint8_t var1;
static uint8_t var2;

static unsigned long last_millis;

void state_settings_init(){
  update_display = 1;
  last_millis = 0;
  state = MENU_YEAR;
  var1 = 0;
  var2 = 0;
  top_led_set_state(1);
}

void state_settings_tick(unsigned long current_millis){
  
  switch (state)
  {
    case MENU_YEAR:
    {
      if(update_display){
        update_display = 0;
        digits_set(0, DIGITS_y);
        digits_set(1, DIGITS_E);
        digits_set(2, DIGITS_a);
        digits_set(3, DIGITS_r);
        digits_set_dots(0);
        digits_update();
      }
    }
    break;
  
    case MENU_DAY_MONTH:
    {
      if(update_display){
        update_display = 0;
        digits_set(0, DIGITS_d);
        digits_set(1, DIGITS_TURN_OFF);
        digits_set(2, DIGITS_n);
        digits_set(3, DIGITS_n);
        digits_set_dots(1);
        digits_update();
      }
    }
    break;
  
    case MENU_HOUR_MINUTES:
    {
      if(update_display){
        update_display = 0;
        digits_set(0, DIGITS_h);
        digits_set(1, DIGITS_TURN_OFF);
        digits_set(2, DIGITS_n);
        digits_set(3, DIGITS_n);
        digits_set_dots(1);
        digits_update();
      }
    }
    break;
  
    case MENU_BRIGHTNESS_STRENGTH:
    {
      if(update_display){
        update_display = 0;
        digits_set(0, DIGITS_b);
        digits_set(1, DIGITS_r);
        digits_set(2, DIGITS_TURN_OFF);
        digits_set(3, DIGITS_TURN_OFF);
        digits_set_dots(0);
        digits_update();
      }
    }
    break;
  
    case MENU_BRIGHTNESS_OFFSET:
    {
      if(update_display){
        update_display = 0;
        digits_set(0, DIGITS_b);
        digits_set(1, DIGITS_r);
        digits_set(2, DIGITS_o);
        digits_set(3, DIGITS_F);
        digits_set_dots(0);
        digits_update();
      }
    }
    break;
  
    case MENU_BUZZER_MELODY:
    {
      if(update_display){
        update_display = 0;
        digits_set(0, DIGITS_A);
        digits_set(1, DIGITS_l);
        digits_set(2, DIGITS_S);
        digits_set(3, DIGITS_d);
        digits_set_dots(0);
        digits_update();
      }
    }
    break;
  
    case MENU_BUZZER_VOLUME:
    {
      if(update_display){
        update_display = 0;
        digits_set(0, DIGITS_A);
        digits_set(1, DIGITS_l);
        digits_set(2, DIGITS_U);
        digits_set(3, DIGITS_l);
        digits_set_dots(0);
        digits_update();
      }
    }
    break;
  
    case MENU_SNOOZE_TIMES:
    {
      if(update_display){
        update_display = 0;
        digits_set(0, DIGITS_S);
        digits_set(1, DIGITS_n);
        digits_set(2, DIGITS_TURN_OFF);
        digits_set(3, DIGITS_t);
        digits_set_dots(0);
        digits_update();
      }
    }
    break;
  
    case MENU_SNOOZE_DURATION:
    {
      if(update_display){
        update_display = 0;
        digits_set(0, DIGITS_S);
        digits_set(1, DIGITS_n);
        digits_set(2, DIGITS_TURN_OFF);
        digits_set(3, DIGITS_d);
        digits_set_dots(0);
        digits_update();
      }
    }
    break;
  
    case MENU_DOTS_BLINKING_SPEED:
    {
      if(update_display){
        update_display = 0;
        digits_set(0, DIGITS_d);
        digits_set(1, DIGITS_b);
        digits_set(2, DIGITS_S);
        digits_set(3, DIGITS_d);
        digits_set_dots(0);
        digits_update();
      }
    }
    break;
  
    case MENU_BLUETOOTH:
    {
      if(update_display){
        update_display = 0;
        digits_set(0, DIGITS_b);
        digits_set(1, DIGITS_t);
        digits_set(2, DIGITS_TURN_OFF);
        digits_set(3, DIGITS_TURN_OFF);
        digits_set_dots(0);
        digits_update();
      }
    }
    break;
    
    case MENU_RESET:
    {
      if(update_display){
        update_display = 0;
        digits_set(0, DIGITS_r);
        digits_set(1, DIGITS_E);
        digits_set(2, DIGITS_S);
        digits_set(3, DIGITS_t);
        digits_set_dots(0);
        digits_update();
      }
    }
    break;
  
    case SEL_YEAR:
    {
      if(update_display){
        update_display = 0;
        digits_set(0, DIGITS_2);
        digits_set(1, DIGITS_0);
        digits_set(2, var1/10);
        digits_set(3, var1%10);
        digits_update();
      }
    }
    break;

    case SEL_HOUR_MINUTES_HH:
    case SEL_DAY_MONTH_DD:
    {
      if(update_display){
        update_display = 0;
        digits_set(0, var1/10);
        digits_set(1, var1%10);
        digits_set(2, var2/10);
        digits_set(3, var2%10);
        digits_update();
      }

      if(current_millis - last_millis > 250){
        last_millis = current_millis;
        digits_invert_mask(DIGITS_MASK_D1 | DIGITS_MASK_D2);
        digits_update();
      }
    }
    break;
    
    case SEL_HOUR_MINUTES_MM:
    case SEL_DAY_MONTH_MM:
    {
      if(update_display){
        update_display = 0;
        digits_set(0, var1/10);
        digits_set(1, var1%10);
        digits_set(2, var2/10);
        digits_set(3, var2%10);
        digits_update();
      }

      if(current_millis - last_millis > 250){
        last_millis = current_millis;
        digits_invert_mask(DIGITS_MASK_D3 | DIGITS_MASK_D4);
        digits_update();
      }
    }
    break;

    case SEL_BRIGHTNESS_STRENGTH:
    {
      if(update_display){
        update_display = 0;
        if(var1 == 0){ //auto
          digits_set(0, DIGITS_A);
          digits_set(1, DIGITS_u);
          digits_set(2, DIGITS_t);
          digits_set(3, DIGITS_o);
        }
        else{
          digits_set(0, var1/10);
          digits_set(1, var1%10);
          digits_set(2, DIGITS_TURN_OFF);
          digits_set(3, DIGITS_TURN_OFF);
        }
        digits_update();
      }
    }
    break;

    case SEL_BRIGHTNESS_OFFSET:
    {
      if(update_display){
        update_display = 0;
        if(var1 > 1){ //auto
          digits_set(0, DIGITS_TURN_OFF);
          digits_set(1, var1-2);
          digits_set(2, DIGITS_TURN_OFF);
          digits_set(3, DIGITS_TURN_OFF);
        }
        else{
          digits_set(0, DIGITS_MINUS);
          digits_set(1, 2-var1);
          digits_set(2, DIGITS_TURN_OFF);
          digits_set(3, DIGITS_TURN_OFF);
        }
        digits_update();
      }
    }
    break;

    case SEL_BUZZER_MELODY:
    case SEL_BUZZER_VOLUME:
    case SEL_SNOOZE_TIMES:
    case SEL_SNOOZE_DURATION:
    {
      if(update_display){
        update_display = 0;
        digits_set(0, var1/10);
        digits_set(1, var1%10);
        digits_set(2, DIGITS_TURN_OFF);
        digits_set(3, DIGITS_TURN_OFF);
        digits_update();
      }
    }
    break;

    case SEL_DOTS_BLINKING_SPEED:
    {
      if(update_display){
        update_display = 0;
        if(var1 == 0){
          digits_set(0, DIGITS_0);
          digits_set(1, DIGITS_F);
          digits_set(2, DIGITS_F);
          digits_set(3, DIGITS_TURN_OFF);
        }
        else if(var1 == 1){
          digits_set(0, DIGITS_S);
          digits_set(1, DIGITS_TURN_OFF);
          digits_set(2, DIGITS_TURN_OFF);
          digits_set(3, DIGITS_TURN_OFF);
        }
        else if(var1 == 2){
          digits_set(0, DIGITS_F);
          digits_set(1, DIGITS_TURN_OFF);
          digits_set(2, DIGITS_TURN_OFF);
          digits_set(3, DIGITS_TURN_OFF);
        }
        digits_update();
      }
    }
    break;

    case SEL_RESET:
    {
      if(current_millis - last_millis > 100){
        last_millis = current_millis;
        digits_invert_mask(DIGITS_MASK_ALL);
        digits_update();
      }
    }
    break;

    case SEL_RESET_CONF:
    {
      if(update_display){
        update_display = 0;
        digits_set(0, DIGITS_d);
        digits_set(1, DIGITS_o);
        digits_set(2, DIGITS_n);
        digits_set(3, DIGITS_E);
        digits_update();
      }
    }
    break;

  }

}

void state_settings_rotary_encoder_rotation(uint8_t direction){
  buzzer_play_melody(0);
  
  update_display = 1;

  switch (state)
  {
  
    case MENU_YEAR:
    case MENU_DAY_MONTH:
    case MENU_HOUR_MINUTES:
    case MENU_BRIGHTNESS_STRENGTH:
    case MENU_BRIGHTNESS_OFFSET:
    case MENU_BUZZER_MELODY:
    case MENU_BUZZER_VOLUME:
    case MENU_SNOOZE_TIMES:
    case MENU_SNOOZE_DURATION:
    case MENU_DOTS_BLINKING_SPEED:
    case MENU_BLUETOOTH:
    case MENU_RESET:
    {
      if(direction == ROTARY_ENCODER_CLOCKWISE){
        if ((int)state == menu_entries-1) (settings_sub_states) 0;
        else state = (settings_sub_states)((int)state + 1);
      }
      else{
        if ((int)state == 0) (settings_sub_states) (menu_entries-1);
        else state = (settings_sub_states)((int)state - 1);
      }
    }
    break;

    case SEL_YEAR:{

      if(direction == ROTARY_ENCODER_CLOCKWISE){
        if(var1 == 99)var1 = 0;
        else var1++;
      }
      else{
        if(var1 == 0)var1 = 99;
        else var1--;
      }

    }
    break;

    case SEL_DAY_MONTH_DD:{
      uint8_t year = rtc_get_value(RTC_YEAR);
      uint8_t dof = days_of_month(var2-1, year);
      if(direction == ROTARY_ENCODER_CLOCKWISE){
        if(var1 == dof)var1 = 1;
        else var1++;
      }
      else{
        if(var1 == 1)var1 = dof;
        else var1--;
      }

    }
    break;

    case SEL_DAY_MONTH_MM:{

      if(direction == ROTARY_ENCODER_CLOCKWISE){
        if(var2 == 12)var2 = 1;
        else var2++;
      }
      else{
        if(var2 == 1)var2 = 12;
        else var2--;
      }

      uint8_t year = rtc_get_value(RTC_YEAR);
      uint8_t dof = days_of_month(var2-1, year);

      if(var1 > dof)var1 = dof;

    }
    break;

    case SEL_HOUR_MINUTES_HH:{

      if(direction == ROTARY_ENCODER_CLOCKWISE){
        if(var1 == 23)var1 = 0;
        else var1++;
      }
      else{
        if(var1 == 0)var1 = 23;
        else var1--;
      }

    }
    break;

    case SEL_HOUR_MINUTES_MM:{

      if(direction == ROTARY_ENCODER_CLOCKWISE){
        if(var2 == 59)var2 = 0;
        else var2++;
      }
      else{
        if(var2 == 0)var2 = 59;
        else var2--;
      }

    }
    break;

    case SEL_BRIGHTNESS_STRENGTH:{

      if(direction == ROTARY_ENCODER_CLOCKWISE){
        if(var1 == 10)var1 = 1;
        else var1++;
      }
      else{
        if(var1 == 1)var1 = 10;
        else var1--;
      }
      digits_set_brightness(var1);
    }
    break;

    case SEL_BRIGHTNESS_OFFSET:{

      if(direction == ROTARY_ENCODER_CLOCKWISE){
        if(var1 == 4)var1 = 0;
        else var1++;
      }
      else{
        if(var1 == 0)var1 = 4;
        else var1--;
      }

    }
    break;

    case SEL_BUZZER_MELODY:{

      if(direction == ROTARY_ENCODER_CLOCKWISE){
        if(var1 == 3)var1 = 1;
        else var1++;
      }
      else{
        if(var1 == 1)var1 = 3;
        else var1--;
      }
      buzzer_stop_melody();
      buzzer_play_melody(var1+1);
    }
    break;

    case SEL_BUZZER_VOLUME:{

      if(direction == ROTARY_ENCODER_CLOCKWISE){
        if(var1 == 10)var1 = 1;
        else var1++;
      }
      else{
        if(var1 == 1)var1 = 10;
        else var1--;
      }
      buzzer_stop_melody();
      buzzer_set_volume(var1);
      buzzer_play_melody(1);

    }
    break;

    case SEL_SNOOZE_TIMES:{

      if(direction == ROTARY_ENCODER_CLOCKWISE){
        if(var1 == 5)var1 = 0;
        else var1++;
      }
      else{
        if(var1 == 0)var1 = 5;
        else var1--;
      }

    }
    break;

    case SEL_SNOOZE_DURATION:{

      if(direction == ROTARY_ENCODER_CLOCKWISE){
        if(var1 == 15)var1 = 1;
        else var1++;
      }
      else{
        if(var1 == 1)var1 = 15;
        else var1--;
      }

    }
    break;

    case SEL_DOTS_BLINKING_SPEED:{

      if(direction == ROTARY_ENCODER_CLOCKWISE){
        if(var1 == 2)var1 = 0;
        else var1++;
      }
      else{
        if(var1 == 0)var1 = 2;
        else var1--;
      }

    }
    break;


  }

}

void state_settings_top_button_pressed(){
  buzzer_play_melody(0);
  digits_set_mask(DIGITS_MASK_ALL);
  update_display = 1;
  
  switch (state) {
  
    case MENU_YEAR:
    case MENU_DAY_MONTH:
    case MENU_HOUR_MINUTES:
    case MENU_BRIGHTNESS_STRENGTH:
    case MENU_BRIGHTNESS_OFFSET:
    case MENU_BUZZER_MELODY:
    case MENU_BUZZER_VOLUME:
    case MENU_SNOOZE_TIMES:
    case MENU_SNOOZE_DURATION:
    case MENU_DOTS_BLINKING_SPEED:
    case MENU_BLUETOOTH:
    case MENU_RESET:
    {
      change_state_callback(STATE_SET_STANDBY);
    }
    break;

    case SEL_YEAR:{
      uint8_t dof = days_of_month(rtc_get_value(RTC_MONTH)-1, var1); //days of month
      if(rtc_get_value(RTC_DATE) > dof){
        rtc_set_value(RTC_DATE, dof);
      }

      rtc_set_value(RTC_YEAR, var1);
      rtc_set_value(RTC_DAY_OF_WEEK, calculate_day_of_week(rtc_get_value(RTC_DATE), rtc_get_value(RTC_MONTH), var1));
      state = MENU_YEAR;
    }
    break;

    case SEL_DAY_MONTH_MM:
    case SEL_DAY_MONTH_DD:{
      rtc_set_value(RTC_DATE, var1);
      rtc_set_value(RTC_MONTH, var2);
      rtc_set_value(RTC_DAY_OF_WEEK, calculate_day_of_week(var1, var2, rtc_get_value(RTC_YEAR)));
      state = MENU_DAY_MONTH;
    }
    break;
   
    case SEL_HOUR_MINUTES_HH:
    case SEL_HOUR_MINUTES_MM:{
      rtc_set_value(RTC_HOUR, var1);
      rtc_set_value(RTC_MINUTE, var2);
      state = MENU_HOUR_MINUTES;
    }
    break;
   
    case SEL_BRIGHTNESS_STRENGTH:{
      storage_store_setting(SETTING_BRIGHTNESS_STRENGTH, var1);
      state = MENU_BRIGHTNESS_STRENGTH;
    }
    break;
   
    case SEL_BRIGHTNESS_OFFSET:{
      storage_store_setting(SETTING_BRIGHTNESS_OFFSET, var1);
      state = MENU_BRIGHTNESS_OFFSET;
    }
    break;
   
    case SEL_BUZZER_MELODY:{
      //User sees options 1-X but 0 and 1 are system sounds, so we add one to go to the melodies section
      storage_store_setting(SETTING_BUZZER_MELODY, var1+1);
      buzzer_stop_melody();
      state = MENU_BUZZER_MELODY;
    }
    break;
   
    case SEL_BUZZER_VOLUME:{
      storage_store_setting(SETTING_BUZZER_VOLUME, var1);
      state = MENU_BUZZER_VOLUME;
    }
    break;
   
    case SEL_SNOOZE_TIMES:{
      storage_store_setting(SETTING_SNOOZE_TIMES, var1);
      state = MENU_SNOOZE_TIMES;
    }
    break;
   
    case SEL_SNOOZE_DURATION:{
      storage_store_setting(SETTING_SNOOZE_DURATION, var1);
      state = MENU_SNOOZE_DURATION;
    }
    break;
   
    case SEL_DOTS_BLINKING_SPEED:{
      storage_store_setting(SETTING_DOTS_BLINKING_SPEED, var1);
      state = MENU_DOTS_BLINKING_SPEED;
    }
    break;

    case SEL_RESET:{
      state = MENU_RESET;
    }
    break;
    
  }

}

void state_settings_back_button_pressed(){
  buzzer_play_melody(0);
  update_display = 1;

  switch(state){

    case MENU_YEAR:
      {
        var1 = rtc_get_value(RTC_YEAR);
        state = SEL_YEAR;
      }
    break;

    case MENU_DAY_MONTH:
      {
        digits_set_mask(DIGITS_MASK_ALL);
        digits_set_dots(1);
        var1 = rtc_get_value(RTC_DATE);
        var2 = rtc_get_value(RTC_MONTH);
        state = SEL_DAY_MONTH_DD;
      }
    break;

    case MENU_HOUR_MINUTES:
      {
        digits_set_mask(DIGITS_MASK_ALL);
        digits_set_dots(1);
        var1 = rtc_get_value(RTC_HOUR);
        var2 = rtc_get_value(RTC_MINUTE);
        state = SEL_HOUR_MINUTES_HH;
      }
    break;

    case MENU_BRIGHTNESS_STRENGTH:
      {
        var1 = storage_retrieve_setting(SETTING_BRIGHTNESS_STRENGTH);
        state = SEL_BRIGHTNESS_STRENGTH;
      }
    break;

    case MENU_BRIGHTNESS_OFFSET:
      {
        var1 = storage_retrieve_setting(SETTING_BRIGHTNESS_OFFSET);
        state = SEL_BRIGHTNESS_OFFSET;
      }
    break;

    case MENU_BUZZER_MELODY:
      {
        var1 = storage_retrieve_setting(SETTING_BUZZER_MELODY)-1;
        state = SEL_BUZZER_MELODY;
      }
    break;

    case MENU_BUZZER_VOLUME:
      {
        var1 = storage_retrieve_setting(SETTING_BUZZER_VOLUME);
        state = SEL_BUZZER_VOLUME;
      }
    break;

    case MENU_SNOOZE_TIMES:
      {
        var1 = storage_retrieve_setting(SETTING_SNOOZE_TIMES);
        state = SEL_SNOOZE_TIMES;
      }
    break;

    case MENU_SNOOZE_DURATION:
      {
        var1 = storage_retrieve_setting(SETTING_SNOOZE_DURATION);
        state = SEL_SNOOZE_DURATION;
      }
    break;

    case MENU_DOTS_BLINKING_SPEED:
      {
        var1 = storage_retrieve_setting(SETTING_DOTS_BLINKING_SPEED);
        state = SEL_DOTS_BLINKING_SPEED;
      }
    break;

    case MENU_BLUETOOTH:
      {
        //TODO change state to BT
        buzzer_play_melody(1);
      }
    break;

    case MENU_RESET:
      {
        digits_set_mask(DIGITS_MASK_ALL);
        state = SEL_RESET; 
      }
    break;

    case SEL_HOUR_MINUTES_HH:
      {
        digits_set_mask(DIGITS_MASK_ALL);
        state = SEL_HOUR_MINUTES_MM;
      }
    break;

    case SEL_HOUR_MINUTES_MM:
      {
        digits_set_mask(DIGITS_MASK_ALL);
        state = SEL_HOUR_MINUTES_HH;
      }
    break;

    case SEL_DAY_MONTH_DD:
      {
        digits_set_mask(DIGITS_MASK_ALL);
        state = SEL_DAY_MONTH_MM;
      }
    break;
    
    case SEL_DAY_MONTH_MM:
      {
        digits_set_mask(DIGITS_MASK_ALL);
        state = SEL_DAY_MONTH_DD;
      }
    break;

    case SEL_RESET:
      {
        digits_set_mask(DIGITS_MASK_ALL);

        //reset all alarms, 8 days (including one time alarms) * 3 alarms per day * 3 bytes per alarm
        for (uint8_t i = SETTING_ALARMS_START; i < SETTING_ALARMS_START+(8*3*3); i++){
          storage_store_setting(i, 0);
        }
        
        storage_store_setting(SETTING_BRIGHTNESS_STRENGTH, 0);
        storage_store_setting(SETTING_BRIGHTNESS_OFFSET, 0);
        storage_store_setting(SETTING_BUZZER_MELODY, 0);
        storage_store_setting(SETTING_BUZZER_VOLUME, 0);
        storage_store_setting(SETTING_SNOOZE_TIMES, 0);
        storage_store_setting(SETTING_SNOOZE_DURATION, 0);
        storage_store_setting(SETTING_DOTS_BLINKING_SPEED, 0);
        storage_store_setting(SETTING_FIRST_BOOT, 0);
      }

      state = SEL_RESET_CONF;
  }

}

void state_settings_change_state_callback(void (*f)(uint8_t)){
  change_state_callback = f;
}
