
#ifndef __BUZZER_H__
#define __BUZZER_H__


extern "C"{


class Buzzer {
public:
  Buzzer();
  ~Buzzer();
  void short_buzz();
  void long_buzz();
  void buzz(uint32_t ms);

};
} // extern "C"

#endif
