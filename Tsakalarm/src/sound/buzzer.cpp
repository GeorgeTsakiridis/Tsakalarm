#include "buzzer.hpp"
#include "melodies.hpp"
#include <math.h>
#include "control/pins.hpp"
#include "utils.hpp"

static void set_frequency(uint16_t);
static uint16_t note_to_hz(uint8_t);
static void set_note(uint8_t, uint8_t);

static uint8_t melody_playing = 255; //255 means no melody is playing
static uint8_t melody_index = 0;
static uint8_t volume = 1;
static uint8_t unstoppable = 0;
static unsigned long note_start_time = 0;

void buzzer_init(){
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void buzzer_play_melody(uint8_t melody){
  
  if(melody_playing != 255)return;

  melody_playing = melody;
  melody_index = 0;
  
  set_note(1, melodies[melody_playing][0] + offsets[melody_playing]);
  note_start_time = millis();

}

void buzzer_play_melody_unstoppapble(uint8_t melody){
  buzzer_stop_melody();
  unstoppable = true;
  buzzer_play_melody(melody);
}

void buzzer_stop_melody(){
  if(unstoppable) return;
  melody_playing = 255;
  set_note(0, 0);
}

void buzzer_set_volume(uint8_t new_volume){
  volume = scale_value_log(1, 10, 10, 200, new_volume);
}

void buzzer_tick(unsigned long current_time){

  if(melody_playing == 255) return;

    uint16_t note_duration = durations[melody_playing][melody_index]*durations_mul[melody_playing];

    if(current_time - note_start_time >= note_duration){
        //Move to next note
        melody_index++;
        if(melody_index >= sizes[melody_playing]){
            //End of melody
            melody_playing = 255;
            unstoppable = 0;
            set_note(0, 0);
        }else{
            uint8_t note = melodies[melody_playing][melody_index];
            if(note != 0){
                set_note(1, note + offsets[melody_playing]);
            }else{
                set_note(0, 0);
            }
            note_start_time = current_time;
        }
    }
    
}

uint8_t buzzer_is_playing(){
  return melody_playing != 255;
}


void set_frequency(uint16_t frequency){


  if(frequency > 0){
    noInterrupts();
    TCNT1 = 0;
    TCCR1A = 0x00; // Set Timer 1 to Normal Mode
    TCCR1B = 0x00; // Disable Timer 1 until we setup everything
    
    float divisor = (1.f/(float)frequency*16000000.f)/65536;

    if(divisor <= 1){
      OCR1A = ((uint16_t)(16000000.0/frequency));
      TCCR1B |= (1 << CS10); //No Prescaling
    }
    else if(divisor <= 8){
      OCR1A = ((uint16_t)(2000000.0/frequency));
      TCCR1B |= (1 << CS11);//Set prescaler to x/8
    }
    else if(divisor <= 64){
      OCR1A = ((uint16_t)(250000.0/frequency));
      TCCR1B |= (1 << CS11);
      TCCR1B |= (1 << CS10); //Set prescaler to x/64
    }
    else if(divisor <= 256){
      OCR1A = ((uint16_t)(62500.0/frequency));
      TCCR1B |= (1 << CS12); //Set prescaler to x/256
    }
    else{
      OCR1A = ((uint16_t)(15625.0/frequency));
      TCCR1B |= (1 << CS12); //Set prescaler to x/1024
      TCCR1B |= (1 << CS10);
    }

    TCCR1B |= (1 << WGM12); // Set to CTC Mode
    TIMSK1 |= (1 << OCIE1A); //Enable Timer Compare Interrupt
    interrupts();
  }
  
  else{
    TCCR1B = 0x00;
  }
}

uint16_t note_to_hz(uint8_t note){
    return (uint16_t)(8.176*pow(2, (double)note/12.0));
}

void set_note(uint8_t on, uint8_t note){
    uint16_t frequency = note_to_hz(note);
    
    set_frequency(on ? frequency : 0);
}

ISR(TIMER1_COMPA_vect){
    noInterrupts();

    PORTD |= 0x80; //ON
    delayMicroseconds(volume);
    PORTD &= 0x7F; //OFF

    interrupts();
}
