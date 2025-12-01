#include "rtc.hpp"
#include "control/pins.hpp"
#include "utils.hpp"

#include <HardwareSerial.h>

//For DS1302ZN+ RTC

void reset_pins(){
    // digitalWrite(RTC_CLK_PIN, LOW);
    // digitalWrite(RTC_DATA_PIN, LOW);
    // digitalWrite(RTC_ENABLE_PIN, LOW);

    pinMode(RTC_CLK_PIN, INPUT);
    pinMode(RTC_DATA_PIN, INPUT);
    pinMode(RTC_ENABLE_PIN, INPUT);
}

void prepare_pins_for_transmission(){
    digitalWrite(RTC_ENABLE_PIN, LOW);
    digitalWrite(RTC_CLK_PIN, LOW);
    digitalWrite(RTC_DATA_PIN, LOW);

    pinMode(RTC_ENABLE_PIN, OUTPUT);
    pinMode(RTC_CLK_PIN, OUTPUT);
    pinMode(RTC_DATA_PIN, OUTPUT);
}

uint8_t toBCD(uint8_t number){
    div_t res = div(number, 10);

    return (res.quot << 4) | res.rem;
}

uint8_t fromBCD(uint8_t bcd){
    return (bcd & 0xF) + (bcd >> 4)*10;
}

//asumes pins are prepared
void write_with_bit_banging(uint8_t value){
    for (uint8_t i = 0; i < 8; i++){
        digitalWrite(RTC_CLK_PIN, LOW);
        digitalWrite(RTC_DATA_PIN, (value >> i) & 0x1);
        delayMicroseconds(1);
        digitalWrite(RTC_CLK_PIN, HIGH);
        delayMicroseconds(1);    
    }
}

void rtc_set_value(uint8_t setting, uint8_t value){
    prepare_pins_for_transmission();
        
    digitalWrite(RTC_ENABLE_PIN, HIGH);
    delayMicroseconds(4);

    write_with_bit_banging(setting-1);
    delayMicroseconds(4);

    write_with_bit_banging(toBCD(value));
    reset_pins();
}

uint8_t rtc_get_value(uint8_t setting){
    uint8_t result = 0;

    prepare_pins_for_transmission();

    digitalWrite(RTC_ENABLE_PIN, HIGH);
    delayMicroseconds(4);

    write_with_bit_banging(setting);
    
    digitalWrite(RTC_DATA_PIN, LOW);
    pinMode(RTC_DATA_PIN, INPUT);

    for (uint8_t i = 0; i < 8; i++){
        digitalWrite(RTC_CLK_PIN, LOW);
        delayMicroseconds(1);
        result |= (digitalRead(RTC_DATA_PIN) << i);
        digitalWrite(RTC_CLK_PIN, HIGH);
        delayMicroseconds(1);
    }

    reset_pins();

    return fromBCD(result);
}