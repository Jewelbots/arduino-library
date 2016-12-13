
#include "LED.h"
#include "led_sequence.h"
#include "app_error.h"
#include "app_timer.h"
#include "common_defines.h"
#include "haptics_driver.h"
#include "jewelbot_gpio.h"
#include "led_driver.h"
#include "nordic_common.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "Arduino.h"


extern "C"{


  uint8_t color_values[2];

  Timer led_timer;

  LED::LED()
  {
    //Init or enable code here causes the Jewelbot to lock up on reset
  }

  LED::~LED()
  {}

  void LED::turn_on_single(LED_Pos led, ColorLabel color)
  {
    enable_led();
    setLight(uint8_t(led), COLORS[ color ].r, COLORS[ color ].g, COLORS[ color ].b);

  }

  void LED::turn_on_all(ColorLabel color)
  {
    enable_led();
    clear_led();
    setLight(0, COLORS[ color ].r, COLORS[ color ].g, COLORS[ color ].b);
    setLight(1, COLORS[ color ].r, COLORS[ color ].g, COLORS[ color ].b);
    setLight(2, COLORS[ color ].r, COLORS[ color ].g, COLORS[ color ].b);
    setLight(3, COLORS[ color ].r, COLORS[ color ].g, COLORS[ color ].b);

  }

  void LED::turn_off_single(LED_Pos led)
  {
      setLight(uint8_t(led), COLORS[ OFF ].r, COLORS[ OFF ].g, COLORS[ OFF ].b);
  }

  void LED::turn_off_all()
  {
      clear_led();
      disable_led();
  }


  void LED::flash_single(LED_Pos led, ColorLabel color, uint32_t milliseconds)
  {
      turn_on_single(led, color);
      led_timer.pause(milliseconds);
      turn_off_single(led);
  }

  void LED::flash_all(ColorLabel color, uint32_t milliseconds)
  {
      turn_on_all(color);
      led_timer.pause(milliseconds);
      turn_off_all();
  }

  void LED::setLight(uint8_t number, uint8_t r, uint8_t g, uint8_t b)
  {
    led_cmd_t options[4] = {number, r, g, b, 1};
    set_led_state_handler(options);
  }



} // extern "C"
