#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "Arduino.h"
#include "app_error.h"
#include "app_timer.h"
#include "app_util.h"
#include "Timer.h"

extern "C"{

  Timer::Timer()
  {
    //Init or enable code here causes the Jewelbot to lock up on reset
  }

  Timer::~Timer()
  {}

  APP_TIMER_DEF(pause_timer_id);
  static bool timer_flag = false;

  static void pause_timeout_handler(void *p_context) {
  	uint32_t err_code;
    UNUSED_PARAMETER(p_context);
    err_code = app_timer_stop(pause_timer_id);
  	APP_ERROR_CHECK(err_code);
    timer_flag = false;
  }

  static uint32_t convert_ms_ticks(uint32_t milliseconds){
    uint32_t num_ticks;
    return num_ticks = ((uint32_t)ROUNDED_DIV((milliseconds) * (uint64_t)APP_TIMER_CLOCK_FREQ, ((APP_TIMER_PRESCALER) + 1) * 1000));
  }

  void arduino_timer_init(void) {
    uint32_t err_code;
    err_code = app_timer_create(&pause_timer_id, APP_TIMER_MODE_SINGLE_SHOT, pause_timeout_handler);
    APP_ERROR_CHECK(err_code);
  }

  void Timer::pause(uint32_t milliseconds)
  {
    if (milliseconds < 1){
      milliseconds = 50;
    } else if (milliseconds > 20000) {
      milliseconds = 20000;
    }
    uint32_t pause_in_ticks = convert_ms_ticks(milliseconds);
    timer_flag = true;
    app_timer_start(pause_timer_id, pause_in_ticks, NULL);
    while (timer_flag){
      app_sched_execute();
      uint32_t err_code = sd_app_evt_wait();
      APP_ERROR_CHECK(err_code);
    }
  }


}
