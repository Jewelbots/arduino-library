
#ifndef __BUZZER_H__
#define __BUZZER_H__


extern "C"{

#include "Arduino.h"

class Buzzer {
public:
  Buzzer();
  ~Buzzer();
  void extra_short_buzz();
  void short_buzz();
  void medium_buzz();
  void long_buzz();
  void extra_long_buzz();
  void really_long_buzz();

  /* the custom buzz is available for creating your own effect
  Amplitude is a measure of how strong the motor vibrates
  Amplitude can be from 0 (no buzz) to 127 (max buzz)
  Jewelbots typically buzz right in the middle at an amplitude of 63

  Milliseconds is how long the motor will vibrate
  Millisecond is measured in, you guessed it, milliseconds
  40 ms is the minimum allowed and 2 s (2000 ms) is the max for longevity
  The other buzzes are from 125 ms to 1500 ms for reference */
  void custom_buzz(uint8_t amplitude, uint32_t duration);

};
} // extern "C"

#endif
