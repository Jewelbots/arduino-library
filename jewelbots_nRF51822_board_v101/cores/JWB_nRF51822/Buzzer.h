
#ifndef __BUZZER_H__
#define __BUZZER_H__


extern "C"{


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

};
} // extern "C"

#endif
