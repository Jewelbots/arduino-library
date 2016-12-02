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

  void Buzzer::shortBuzz()
  {
    haptics_msg_short();
  }


  void Buzzer::longBuzz()
  {
    haptics_msg_long();
  }

} // extern "C"
