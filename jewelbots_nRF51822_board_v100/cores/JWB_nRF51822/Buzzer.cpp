#include <stdint.h>
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

  void Buzzer::short_buzz()
  {
    haptics_msg_short();
  }


  void Buzzer::long_buzz()
  {
    haptics_msg_long();
  }

  /*
    short_buzz: 250
    long_buzz: 750
  */
  void Buzzer::buzz(uint32_t len)
  {
    haptics_msg(len);
  }
  
} // extern "C"
