
#ifndef __BUZZER_H__
#define __BUZZER_H__




#ifdef __cplusplus
extern "C"{
#endif // __cplusplus


class Buzzer {
public:
  Buzzer();
  ~Buzzer();

  void shortBuzz();
  void longBuzz();

};


#ifdef __cplusplus
}
#endif // __cplusplus

#endif
