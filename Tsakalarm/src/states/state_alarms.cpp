#include "state_menu.hpp"
#include "sound/buzzer.hpp"
#include "display/digits.hpp"
#include "display/top_led.hpp"
#include "states/states.hpp"
#include "control/rotary_encoder.hpp"
#include "utils.hpp"

static void (*change_state_callback)(uint8_t);

/*

1: d0-d7 -> 2: a1-a3
                  +----> 3: Edit -> 4/5: hh:mm
                  L----> 6: En:Di
*/

enum alarms_sub_states{
  MENU, //day selection
  MENU_SUB1_ALARM_SELECT, //alarm index selection 
  MENU_SUB2_ALARM_EDIT, //alarm edit selection
  MENU_SUB3_ALARM_EDIT_HH, //alarm edit action (hour)
  MENU_SUB3_ALARM_EDIT_MM, //alarm edit action (minutes)
  MENU_SUB2_ALARM_STATUS, //alarm status display
};

static enum alarms_sub_states state;

static uint8_t day_selection;
static uint8_t alarm_selection;
static uint8_t alarm_hour;
static uint8_t alarm_minutes;
static uint8_t alarm_active;
static uint8_t update_display;
static unsigned long last_millis;

void state_alarms_init(){
  update_display = 1;
  last_millis = 0;
  day_selection = 0;
  state = MENU;
}

void state_alarms_tick(unsigned long current_millis){

  switch (state){

    case MENU:
    {
      if(update_display){
        update_display = 0;
        digits_set(0, DIGITS_d);
        digits_set(1, day_selection);
        digits_set(2, DIGITS_TURN_OFF);
        digits_set(3, DIGITS_TURN_OFF);
        digits_update();
      }
    }
    break;

    case MENU_SUB1_ALARM_SELECT:
    {
      if(update_display){
        update_display = 0;
        digits_set(0, DIGITS_a);
        digits_set(1, alarm_selection+1);
        digits_set(2, DIGITS_TURN_OFF);
        digits_set(3, DIGITS_TURN_OFF);
        digits_update();
      }
    }
    break;
  
    case MENU_SUB2_ALARM_EDIT:
    {
      if(update_display){
        update_display = 0;
        digits_set(0, DIGITS_E);
        digits_set(1, DIGITS_d);
        digits_set(2, DIGITS_i);
        digits_set(3, DIGITS_t);
        digits_update();
      }
    }
    break;

    case MENU_SUB2_ALARM_STATUS:
    {
      if(update_display){
        update_display = 0;
        if(alarm_active){
          digits_set(0, DIGITS_E);
          digits_set(1, DIGITS_n);
          digits_set(2, DIGITS_TURN_OFF);
          digits_set(3, DIGITS_TURN_OFF);
        }
        else{
          digits_set(0, DIGITS_TURN_OFF);
          digits_set(1, DIGITS_TURN_OFF);
          digits_set(2, DIGITS_d);
          digits_set(3, DIGITS_i);
        }
        digits_update();
      }
    }
    break;

    case MENU_SUB3_ALARM_EDIT_HH:
    {

      if(update_display){
        update_display = 0;
        digits_set(0, alarm_hour/10);
        digits_set(1, alarm_hour%10);
        digits_set(2, alarm_minutes/10);
        digits_set(3, alarm_minutes%10);
        digits_update();
      }

      if(current_millis - last_millis > 250){
        last_millis = current_millis;
        digits_invert_mask(DIGITS_MASK_D1 | DIGITS_MASK_D2);
        digits_update();
      }
    }
    break;

   case MENU_SUB3_ALARM_EDIT_MM:
    {

      if(update_display){
        update_display = 0;
        digits_set(0, alarm_hour/10);
        digits_set(1, alarm_hour%10);
        digits_set(2, alarm_minutes/10);
        digits_set(3, alarm_minutes%10);
        digits_update();
      }

      if(current_millis - last_millis > 250){
        last_millis = current_millis;
        digits_invert_mask(DIGITS_MASK_D3 | DIGITS_MASK_D4);
        digits_set(0, alarm_hour/10);
        digits_set(1, alarm_hour%10);
        digits_set(2, alarm_minutes/10);
        digits_set(3, alarm_minutes%10);
        digits_update();
      }
    }
    break;

  }

}

void state_alarms_rotary_encoder_rotation(uint8_t direction){
  buzzer_play_melody(0);
  update_display = 1;

  switch (state){
    case MENU:
      {
        if(direction == ROTARY_ENCODER_CLOCKWISE){
          if(day_selection == 7) day_selection = 0;
          else day_selection++;
        }
        else{
          if(day_selection == 0) day_selection = 7;
          else day_selection--;
        }
      }
    break;

    case MENU_SUB1_ALARM_SELECT:
      {
        if(direction == ROTARY_ENCODER_CLOCKWISE){
          if(alarm_selection == 2) alarm_selection = 0; 
            else alarm_selection++;
        }
        else{
          if(alarm_selection == 0) alarm_selection = 2;
          else alarm_selection--;
        }
      }
    break;

    case MENU_SUB2_ALARM_EDIT:
    {
      state = MENU_SUB2_ALARM_STATUS;
    }
    break;

    case MENU_SUB2_ALARM_STATUS:
    {
      state = MENU_SUB2_ALARM_EDIT;
    }
    break;

    case MENU_SUB3_ALARM_EDIT_HH:
    {
      if(direction == ROTARY_ENCODER_CLOCKWISE){
        if(alarm_hour == 23) alarm_hour = 0;
        else alarm_hour++;
      }
      else{
        if(alarm_hour == 0) alarm_hour = 23;
        else alarm_hour--;
      }
    }
    break;

    case MENU_SUB3_ALARM_EDIT_MM:
    {
      if(direction == ROTARY_ENCODER_CLOCKWISE){
        if(alarm_minutes == 59) alarm_minutes = 0;
        else alarm_minutes++;
      }
      else{
        if(alarm_minutes == 0) alarm_minutes = 59;
        else alarm_minutes--;
      }
    }
    break;

  }


}

void state_alarms_top_button_pressed(){
  buzzer_play_melody(0);
  update_display = 1;

  switch (state){
    case MENU:
    {
      change_state_callback(STATE_SET_STANDBY);
    }
    break;

    case MENU_SUB1_ALARM_SELECT:
    {
      state = MENU;
    }
    break;

    case MENU_SUB2_ALARM_EDIT:
    {
      state = MENU_SUB1_ALARM_SELECT;
    }
    break;

    case MENU_SUB3_ALARM_EDIT_MM:
    case MENU_SUB3_ALARM_EDIT_HH:
    {
      set_alarm_clock(day_selection, alarm_selection, alarm_active, alarm_hour, alarm_minutes);
      digits_set_mask(DIGITS_MASK_ALL);
      digits_set_dots(0);
      state = MENU_SUB2_ALARM_EDIT;
    }
    break;

    case MENU_SUB2_ALARM_STATUS:
    {
      state = MENU_SUB1_ALARM_SELECT;
    }
    break;

  }

}

void state_alarms_back_button_pressed(){
  buzzer_play_melody(0);
  update_display = 1;

  switch (state){
    case MENU:
    {
      alarm_selection = 0;
      state = MENU_SUB1_ALARM_SELECT;
    }
    break;

    case MENU_SUB1_ALARM_SELECT:
    {
      get_alarm_clock_raw(day_selection, alarm_selection, &alarm_active, &alarm_hour, &alarm_minutes);
      state = MENU_SUB2_ALARM_EDIT;
    }
    break;

    case MENU_SUB2_ALARM_EDIT:
    {
      digits_set_dots(1);
      state = MENU_SUB3_ALARM_EDIT_HH; 
    }
    break;

    case MENU_SUB3_ALARM_EDIT_HH:
    {
      digits_set_mask(DIGITS_MASK_ALL);
      state = MENU_SUB3_ALARM_EDIT_MM;
    }
    break;

    case MENU_SUB3_ALARM_EDIT_MM:
    {
      digits_set_mask(DIGITS_MASK_ALL);
      state = MENU_SUB3_ALARM_EDIT_HH;
    }
    break;

    case MENU_SUB2_ALARM_STATUS:
    {
      alarm_active = !alarm_active;
      set_alarm_clock_active(day_selection, alarm_selection, alarm_active);
    }
    break;

  }
}

void state_alarms_change_state_callback(void (*f)(uint8_t)){
  change_state_callback = f;
}
