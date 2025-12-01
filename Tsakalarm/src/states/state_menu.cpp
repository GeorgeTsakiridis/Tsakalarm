#include "state_menu.hpp"
#include "sound/buzzer.hpp"
#include "display/digits.hpp"
#include "display/top_led.hpp"
#include "states/states.hpp"

static void (*change_state_callback)(uint8_t);

enum menu_sub_states{
  ALARMS, 
  SETTINGS, 
};

static enum menu_sub_states state;

void menu_set_state(enum menu_sub_states new_state){
  state = new_state;

  switch (new_state) {
    case ALARMS:
    {
      digits_set(0, DIGITS_TURN_OFF);
      digits_set(1, DIGITS_A);
      digits_set_dots(1);
      digits_set(2, DIGITS_a);
      digits_set(3, DIGITS_r);
      digits_update();
    }
    break;
    
    case SETTINGS:
    {
      digits_set(0, DIGITS_S);
      digits_set(1, DIGITS_E);
      digits_set(2, DIGITS_t);
      digits_set(3, DIGITS_t);
      digits_set_dots(0);
      digits_update();
    }
    break;
  
  }
}

void state_menu_init(){
  menu_set_state(ALARMS);
  top_led_set_state(1);
}

void state_menu_tick(unsigned long current_millis){
}

void state_menu_rotary_encoder_rotation(uint8_t direction){
  buzzer_play_melody(0);

  switch(state){
    case ALARMS:
    {
      menu_set_state(SETTINGS);
    }
    break;

    case SETTINGS:
    {
      menu_set_state(ALARMS);
    }

    break;
  }

}

void state_menu_top_button_pressed(){
  buzzer_play_melody(0);
  change_state_callback(STATE_SET_STANDBY);
}

void state_menu_back_button_pressed(){
  buzzer_play_melody(0);
  
  switch (state)
  {
  case ALARMS:
    change_state_callback(STATE_SET_ALARMS);
    break;
  
  case SETTINGS:
    change_state_callback(STATE_SET_SETTINGS);
    break;
  }

}

void state_menu_change_state_callback(void (*f)(uint8_t)){
  change_state_callback = f;
}
