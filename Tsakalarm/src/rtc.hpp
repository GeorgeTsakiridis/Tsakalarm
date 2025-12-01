#ifndef rtc_h
#define rtc_h

#include <Arduino.h>

#define RTC_SECOND 0x81
#define RTC_MINUTE 0x83
#define RTC_HOUR 0x85
#define RTC_DATE 0x87
#define RTC_MONTH 0x89
#define RTC_DAY_OF_WEEK 0x8B
#define RTC_YEAR 0x8D

void rtc_set_value(uint8_t, uint8_t);
uint8_t rtc_get_value(uint8_t);

#endif