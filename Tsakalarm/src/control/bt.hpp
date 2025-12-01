#ifndef BT_HPP
#define BT_HPP

#include <Arduino.h>
#include "pins.hpp"

void bt_init(void);
void bt_tick(void);

void bt_set_active(uint8_t active);

void bt_send(const char*);
void bt_send_ok(const char*);
void bt_send_error(const char*);

uint8_t bt_is_connected();
#endif