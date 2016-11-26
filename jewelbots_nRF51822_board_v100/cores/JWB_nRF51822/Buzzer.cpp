#include "Buzzer.h"
#include "haptics_driver.h"

extern "C"{

  Buzzer::Buzzer()
  {
    haptics_init();
    haptics_enable();
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
