#ifndef DIGITS_HPP
#define DIGITS_HPP

#include <Arduino.h>

#define DIGITS_MASK_ALL 0b11111
#define DIGITS_MASK_D1 1
#define DIGITS_MASK_D2 2
#define DIGITS_MASK_D3 4
#define DIGITS_MASK_D4 8
#define DIGITS_MASK_DOTS 16

#define DIGITS_0 0
#define DIGITS_1 1
#define DIGITS_2 2
#define DIGITS_3 3
#define DIGITS_4 4
#define DIGITS_5 5
#define DIGITS_6 6
#define DIGITS_7 7
#define DIGITS_8 8
#define DIGITS_9 9
#define DIGITS_r 10
#define DIGITS_A 11
#define DIGITS_n 12
#define DIGITS_d 13
#define DIGITS_u 14
#define DIGITS_E 15
#define DIGITS_S 16
#define DIGITS_o 17
#define DIGITS_F 18
#define DIGITS_U 19
#define DIGITS_b 20
#define DIGITS_t 21
#define DIGITS_LOAD_ANIM_1 22
#define DIGITS_LOAD_ANIM_2 23
#define DIGITS_LOAD_ANIM_3 24
#define DIGITS_LOAD_ANIM_4 25
#define DIGITS_LOAD_ANIM_5 26
#define DIGITS_LOAD_ANIM_6 27
#define DIGITS_H 28
#define DIGITS_l 29
#define DIGITS_a 30
#define DIGITS_i 31
#define DIGITS_y 32
#define DIGITS_h 33
#define DIGITS_MINUS 34

#define DIGITS_TURN_OFF 255

void digits_init();
void digits_set_brightness(uint8_t);
void digits_set(uint8_t, uint8_t);
void digits_set_dots(uint8_t);
void digits_invert_dots();
void digits_update();
void digits_set_mask(uint8_t mask);
void digits_invert_mask(uint8_t invert_mask);

#endif //DIGITS_HPP