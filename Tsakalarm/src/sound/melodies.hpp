#ifndef MELODIES_HPP
#define MELODIES_HPP

#include <Arduino.h>

#define C3 48
#define C3s 49
#define D3 50
#define D3s 51
#define E3 52
#define F3 53
#define F3s 54
#define G3 55
#define G3s 56
#define A3 57
#define A3s 58
#define B3 59
#define C4 60
#define C4s 61
#define D4 62
#define D4s 63
#define E4 64
#define F4 65
#define F4s 66
#define G4 67
#define G4s 68
#define A4 69
#define A4s 70
#define B4 71
#define C5 72
#define C5s 73
#define D5 74
#define D5s 75
#define E5 76
#define F5 77
#define F5s 78
#define G5 79
#define G5s 80
#define A5 81
#define A5s 82
#define B5 83
#define C6 84

//A B C D E F G A B | C D E F G

//Feedback beep
uint8_t melody0[] = {
  G4
};

uint8_t durations0[] = {
  1
};

//Long beep
uint8_t melody1[] = {
  G4
};

uint8_t durations1[] = {
  1
};

//Breakeven
uint8_t melody2[] = {
        D4s, D5, D4s, D5, A3s, D5, A3s, D5, A3s, D5, F3, A4, A4, F3, A4, G3, A4s, G3, A4s, G3, A4s,
        D4s, D5, D4s, D5, A3s, D5, A3s, D5, A3s, D5, F3, A4, A4, F3, A4, G3, A4s, G3, A4s, G3, A4s,
        F3, D4, A3s, C4, C4, C4, A3s, D4, A3s, 0, A3s, D4, D4, D4, A3s,
        C4, A3s, C4, A3s, D4, A3, A3s, 0, A3s, D4, F4, G4, F4, C4, C4, C4, D4, 0,
        A3s, A3s, A3s, D4, D4, C4, A3s, C4, A3s, D4, A3s, D4, A3s, A3s, C4, 0,
        A4s, A4, G4, G4, F4, F4, F4, D4, F4, F4, D4, F4, F4, G4, D4, D4, C4, A3s, 0,
        A4s, A4, G4, G4, F4, F4, F4, D4, F4, F4, D4, F4, F4, G4, D4, D4, C4, A3s, 0,
        D4, D4, D4s, F4, C5, A4s, D4, 0, D4, D4, D4s, F4, C5, A4s, 0
    };

uint8_t durations2[] = {
    1, 1, 1, 2, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 2, 1, 1, 1, 2, 2, 2,
    1, 2, 1, 2, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 2, 1, 2, 1, 2, 2, 2,
    2, 2, 2, 1, 1, 2, 1, 3, 4, 6, 2, 1, 1, 2, 2,
    2, 2, 2, 2, 3, 3, 4, 5, 1, 2, 2, 2, 2, 2, 2, 2, 6, 4,
    1, 1, 1, 2, 2, 1, 1, 2, 2, 3, 5, 4, 4, 1, 5, 2,
    1, 1, 2, 2, 1, 3, 1, 1, 2, 1, 1, 2, 1, 3, 2, 1, 1, 2, 4,
    1, 1, 2, 2, 1, 3, 1, 1, 2, 1, 1, 2, 1, 3, 2, 1, 1, 2, 6,
    2, 2, 2, 2, 6,   6, 2, 6, 2, 2, 2, 2, 8, 8, 14
    };

//Grandfather's Clock
uint8_t melody3[] = {
  G4, C5, B4, C5, D5, C5, D5, E5, F5, E5, A4, D5, D5, C5, C5, C5, B4, A4, B4, C5, 0,
  C5, E5, G5, E5, D5, C5, B4, C5, D5, C5, B4, A4, G4, E5, F5,
  G5, E5, D5, C5, B4, C5, D5, 0, G4, C5, C5, 0, D5, 0, E5, E5, F5, E5, A4, D5, D5, C5, B4, C5, G4, C4, 0
};

uint8_t durations3[] = {
  2, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 4, 2,
  1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 
  6, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 2, 1, 1, 4, 4, 6, 2, 2, 2

};

//Korea Anthem
uint8_t melody4[] = {
  G4, C5, C5, C5, B4, A4, G4, F4, E4, G4, A4, A4, A4, B4, C5, D5, G4, G4, G4,
  E5, E5, F5, E5, D5, D5, A4, A4, A4, B4, B4, B4, B4, A4, B4, C5
};

uint8_t durations4[] = {
  4, 6, 2, 4, 2, 2, 6, 2, 4, 4, 6, 2, 4, 2, 2, 6, 2, 4, 4,
  6, 2, 4, 2, 2, 6, 2, 4, 4, 6, 2, 2, 2, 2, 2, 10
};

uint8_t* melodies[] = {melody0, melody1, melody2, melody3, melody4};
uint8_t* durations[] = {durations0, durations1, durations2, durations3, durations4};
uint16_t durations_mul[] = {10, 200, 150, 200, 150};
uint8_t sizes[] = {1, 1, 144, 63, 35};
int8_t offsets[] = {0, 0, -2, -6, 0};


#endif // MELODIES_HPP