
#ifndef __BUZZER_H__
#define __BUZZER_H__


extern "C"{


class Buzzer {
public:
  Buzzer();
  ~Buzzer();
  void shortBuzz();
  void longBuzz();

};
} // extern "C"

#endif
