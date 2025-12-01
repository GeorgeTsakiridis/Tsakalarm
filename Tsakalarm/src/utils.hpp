#ifndef UTILS_HPP
#define UTILS_HPP

#include <Arduino.h>

typedef struct state_functions {
    void (*init)();
    void (*tick)(unsigned long);
    void (*top_button_callback)();
    void (*back_button_callback)();
    void (*rotary_encoder_callback)(uint8_t);
} state_functions_t;

uint8_t scale_value_log(uint8_t min_input, uint8_t max_input, uint8_t min_output, uint8_t max_output, uint8_t input);

uint8_t is_leap_year(uint8_t year_last_digits);
uint8_t days_of_month(uint8_t month, uint8_t year_last_digits);

void set_alarm_clock(uint8_t day_of_week, uint8_t alarm_index, uint8_t active, uint8_t hour, uint8_t minutes);
void set_alarm_clock_time(uint8_t day_of_week, uint8_t alarm_index, uint8_t hour, uint8_t minutes);
void set_alarm_clock_active(uint8_t day_of_week, uint8_t alarm_index, uint8_t active);
void get_alarm_clock(uint8_t alarm_index, uint8_t* hour, uint8_t* minutes);
void get_alarm_clock_raw(uint8_t day_of_week, uint8_t alarm_index, uint8_t* active, uint8_t* hour, uint8_t* minutes);

uint8_t calculate_day_of_week(uint8_t day, uint8_t month, uint8_t year_last_digits);

#endif //UTILS_HPP