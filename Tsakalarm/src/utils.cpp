#include "utils.hpp"
#include "rtc.hpp"
#include "storage/storage.hpp"

uint8_t days_per_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

uint8_t scale_value_log(uint8_t min_input, uint8_t max_input, uint8_t min_output, uint8_t max_output, uint8_t input){

    float log_min = log(min_output);
    float log_max = log(max_output);
    float scale = (float)(input - min_input) / (max_input - min_input);
    float log_vol = log_min + scale * (log_max - log_min);

    return (uint8_t)round(exp(log_vol));
};

uint8_t is_leap_year(uint8_t year_last_digits){
    uint16_t year = 2000 + year_last_digits;
    //A year is a leap year if divisible by 4, and NOT divisible by 100;
    //There is an exception for years divisible by 400, making them leap years
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

uint8_t days_of_month(uint8_t month, uint8_t year_last_digits){
    
    if(month > 11)return 0;

    uint8_t days = days_per_month[month];
    if(month == 1 && is_leap_year(year_last_digits)){//acount for febuary leap years
        days++;
    }

    return days;
}

void set_alarm_clock(uint8_t day_of_week, uint8_t alarm_index, uint8_t active, uint8_t hour, uint8_t minutes){
    uint16_t storage_address = SETTING_ALARMS_START + 9*day_of_week + 3*alarm_index;
    storage_store_setting(storage_address, active);
    storage_store_setting(storage_address+1, hour);
    storage_store_setting(storage_address+2, minutes);
}

void set_alarm_clock_time(uint8_t day_of_week, uint8_t alarm_index, uint8_t hour, uint8_t minutes){
    uint16_t storage_address = SETTING_ALARMS_START + 9*day_of_week + 3*alarm_index;

    storage_store_setting(storage_address+1, hour);
    storage_store_setting(storage_address+2, minutes);
}

void set_alarm_clock_active(uint8_t day_of_week, uint8_t alarm_index, uint8_t active){
    storage_store_setting(SETTING_ALARMS_START + 9*day_of_week + 3*alarm_index, active);
}

void get_alarm_clock(uint8_t alarm_index, uint8_t* hour, uint8_t* minutes){

    uint8_t day_of_week = rtc_get_value(RTC_DAY_OF_WEEK);

    if(alarm_index >= 3){
        day_of_week = 0;
        alarm_index -= 3;   
    }

    uint16_t storage_address = SETTING_ALARMS_START + 9*day_of_week + 3*alarm_index;


    //Is the clock activated?
    if(storage_retrieve_setting(storage_address)){
        *hour = storage_retrieve_setting(storage_address+1);
        *minutes = storage_retrieve_setting(storage_address+2);
    }
    else{
        *hour = 255; //No alarm clock set
    }
}

void get_alarm_clock_raw(uint8_t day_of_week, uint8_t alarm_index, uint8_t* active, uint8_t* hour, uint8_t* minutes){

    uint16_t storage_address = SETTING_ALARMS_START + 9*day_of_week + 3*alarm_index;

    //Is the clock activated?
    *active = storage_retrieve_setting(storage_address);
    *hour = storage_retrieve_setting(storage_address+1);
    *minutes = storage_retrieve_setting(storage_address+2);

}

//Zeller’s Congruence Algorithm
uint8_t calculate_day_of_week(uint8_t day, uint8_t month, uint8_t year_last_digits) {
    //ranges of arguments are normal (starting from 1)
    uint16_t year = 2000 + year_last_digits;
    if (month < 3) {
        month += 12;
        year -= 1;
    }
    uint16_t K = year % 100;
    uint16_t J = year / 100;
    uint8_t h = (day + 13*(month + 1)/5 + K + K/4 + J/4 + 5*J) % 7;

    return ((h + 5) % 7) + 1; // 1 = Monday, 2 = Tuesdays, ..., 7 = Sunday
}