
#ifndef __DELAY_H__
#define __DELAY_H__


extern "C"{

  void arduino_timer_init(void);

  class Timer {
  public:
    Timer();
    ~Timer();
    void pause(uint32_t milliseconds);
    uint32_t runtime_ms(void);

  };
} // extern "C"

#endif
