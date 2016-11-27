
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


    LED::LED()
    {}

    LED::~LED()
    {}



    void LED::on(uint8_t number, char *color, uint8_t length)  {
      enable_led();
      clear_led();
      color_lookup(color);
      led_cmd_t options[4] = {number, color_values[0], color_values[1], color_values[2], 1};
      set_led_state_handler(options);
      nrf_delay_us(length);
      clear_led();
    }


  void LED::turnOn(LED_Pos led, ColorLabel color)
  {
      setLight(uint8_t(led), COLORS[ color ].r, COLORS[ color ].g, COLORS[ color ].b);
  }

  void LED::turnOff(LED_Pos led)
  {
      setLight(uint8_t(led), COLORS[ OFF ].r, COLORS[ OFF ].g, COLORS[ OFF ].b);
  }

  void LED::flash(LED_Pos led, ColorLabel color, uint8_t microseconds)
  {
      turnOn(led, color);
      nrf_delay_us(microseconds);
      turnOff(led);
  }

  void LED::setLight(uint8_t number, uint8_t r, uint8_t g, uint8_t b)
  {
    led_cmd_t options[4] = {number, r, g, b, 1};
    set_led_state_handler(options);
  }



} // extern "C"
