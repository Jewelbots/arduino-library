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

} // extern "C"
