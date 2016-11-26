
#ifndef __BUZZER_H__
#define __BUZZER_H__



extern "C"{

#include "haptics_driver.h"

class Buzzer {
public:
  Buzzer();
  ~Buzzer();
  
  void shortBuzz();
  void longBuzz();

};

} // extern "C"

#endif
