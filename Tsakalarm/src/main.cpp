#include <Arduino.h>
#include "display/digits.hpp"
#include "display/top_led.hpp"
#include "sound/buzzer.hpp"
#include "control/pins.hpp"
#include "control/rotary_encoder.hpp"
#include "control/buttons.hpp"
#include "rtc.hpp"
#include "states/states.hpp"
#include "states/state_boot.hpp"
#include "states/state_standby.hpp"
#include "states/state_menu.hpp"
#include "states/state_alarms.hpp"
#include "states/state_settings.hpp"
#include "utils.hpp"
#include "storage/storage.hpp"
#include "control/bt.hpp"

static unsigned long bt_last_millis;
static unsigned long buzzer_last_millis;
static unsigned long input_rot_check_last_micros;
static unsigned long input_btn_last_millis;

static void null_fun_no_args(){}
static void null_fun_uint8_arg(uint8_t arg){}
static void null_fun_ulong_arg(unsigned long arg){}

//{init, tick ,top button, back button, rotary encoder}

static state_functions_t state_control_functions[] = {
  {null_fun_no_args, null_fun_ulong_arg, null_fun_no_args, null_fun_no_args, null_fun_uint8_arg},
  {state_boot_init, state_boot_tick, state_boot_top_button_pressed, state_boot_back_button_pressed, state_boot_rotary_encoder_rotation},
  {state_standby_init, state_standby_tick, state_standby_top_button_pressed, state_standby_back_button_pressed, state_standby_rotary_encoder_rotation},
  {state_menu_init, state_menu_tick, state_menu_top_button_pressed, state_menu_back_button_pressed, state_menu_rotary_encoder_rotation},
  {state_alarms_init, state_alarms_tick, state_alarms_top_button_pressed, state_alarms_back_button_pressed, state_alarms_rotary_encoder_rotation},
  {state_settings_init, state_settings_tick, state_settings_top_button_pressed, state_settings_back_button_pressed, state_settings_rotary_encoder_rotation},

};

static void (*tick_function)(unsigned long);

static uint8_t state;

void set_state(uint8_t new_state){
  state = new_state;
  digits_set_mask(DIGITS_MASK_ALL);
  digits_set_dots(0);

  state_functions_t state_f = state_control_functions[state];
  buttons_set_callbacks(state_f.back_button_callback, state_f.top_button_callback);
  rotary_encoder_set_callback(state_f.rotary_encoder_callback);
  tick_function = state_f.tick;

  if(state == STATE_SET_UNKNOWN){
    digits_set(0, DIGITS_E);
    digits_set(1, DIGITS_r);
    digits_set(2, DIGITS_U);
    digits_set(3, DIGITS_n);
    digits_set_dots(0);
    digits_update();
    top_led_set_state(0);
    digits_update();
  }

  state_f.init();

}

void setup() {

  digits_init();
  uint8_t reason = 9;
  if(MCUSR & (1<<PORF )) reason = 0;//myprintf0P(PSTR("Power-on reset.\n"));
  if(MCUSR & (1<<EXTRF)) reason = 1;//myprintf0P(PSTR("External reset!\n"));
  if(MCUSR & (1<<BORF )) reason = 2;//myprintf0P(PSTR("Brownout reset!\n"));
  if(MCUSR & (1<<WDRF )) reason = 3;//myprintf0P(PSTR("Watchdog reset!\n"));
  MCUSR = 0;

  digits_set(0, reason);
  digits_set(1, DIGITS_TURN_OFF);
  digits_set(2, rtc_get_value(RTC_DAY_OF_WEEK));
  digits_set(3, DIGITS_TURN_OFF);
  digits_set_brightness(5);
  digits_update();

  delay(1000);

  buzzer_init();
  digits_init();
  top_led_init();
  rotary_encoder_init();
  buttons_init();

  top_led_set_state(0);
  
  rtc_set_value(0x8F, 0x0);//Disable Write Protect

  state_boot_change_state_callback(set_state);
  state_standby_change_state_callback(set_state);
  state_menu_change_state_callback(set_state);
  state_alarms_change_state_callback(set_state);
  state_settings_change_state_callback(set_state);

  bt_init();
  bt_set_active(1);

  set_state(STATE_SET_BOOT);
  buzzer_play_melody(1);
}


void loop() {
  
  unsigned long current_millis = millis();
  unsigned long current_micros = micros();
  
  if(current_millis - bt_last_millis > 1){
    bt_last_millis = current_millis;
    bt_tick();
    top_led_set_state(bt_is_connected());
  }

  if (current_millis - buzzer_last_millis >= 1) {//Loop every 1ms (1kHz)
    buzzer_last_millis = current_millis;
    buzzer_tick(current_millis);
  }


  if (current_micros - input_rot_check_last_micros >= 500) {//Loop every 0.5ms (2kHz)
    input_rot_check_last_micros = current_micros;
    rotary_encoder_update();
  }

  if(current_millis - input_btn_last_millis >= 10){//Loop every 20ms (50Hz)
    input_btn_last_millis = current_millis;
    buttons_update();
  }

  tick_function(current_millis);
}