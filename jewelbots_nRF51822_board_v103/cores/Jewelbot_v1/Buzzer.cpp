#include "Buzzer.h"
#include "haptics_driver.h"
#include "Arduino.h"

extern "C"{

  Buzzer::Buzzer()
  {
    //Init or enable code here causes the Jewelbot to lock up on reset
  }

  Buzzer::~Buzzer()
  {}

  void Buzzer::extra_short_buzz()
  {
    haptics_msg_extra_short();
  }

  void Buzzer::short_buzz()
  {
    haptics_msg_short();
  }

  void Buzzer::medium_buzz()
  {
    haptics_msg_medium();
  }

  void Buzzer::long_buzz()
  {
    haptics_msg_long();
  }

  void Buzzer::extra_long_buzz()
  {
    haptics_msg_extra_long();
  }

  void Buzzer::really_long_buzz()
  {
    haptics_msg_really_long();
  }

  void Buzzer::custom_buzz(uint8_t amplitude, uint32_t milliseconds)
  {
    // Amplitude is a measure of how strong the motor vibrates
    // Amplitude can be from 0 (no buzz) to 127 (max buzz)
    // Jewelbots typically buzz right in the middle at an amplitude of 63
    if (amplitude < 0){
      amplitude = 0;
    }else if (amplitude > 127) {
      amplitude = 127;
    }

    // Milliseconds is how long the motor will vibrate
    // Millisecond is measured in, you guessed it, milliseconds
    // 40 ms is the minimum allowed and 2 s (2000 ms) is the max for longevity
    // The other buzzes are from 125 ms to 1500 ms for reference
    if (milliseconds < 40){
      milliseconds = 40;
    }else if (milliseconds > 2000){
      milliseconds = 2000;
    }

    haptics_custom(amplitude, milliseconds);
  }

} // extern "C"
