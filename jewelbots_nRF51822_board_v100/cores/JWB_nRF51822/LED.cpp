
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



  void LED::on(uint8_t number, char *color, uint32_t length)  {
    enable_led();
    clear_led();
    color_lookup(color);
    led_cmd_t options[4] = {number, color_values[0], color_values[1], color_values[2], 1};
    set_led_state_handler(options);
    nrf_delay_us(length * 1000);
    clear_led();
  }


  void LED::color_lookup(char *color)
  {
    if(color == "red") {
      color_values[0] = 0x3F;
      color_values[1] = 0x00;
      color_values[2] = 0x00;
    }
    else if (color == "orange"){
      color_values[0] = 0x3F;
      color_values[1] = 0x1C;
      color_values[2] = 0x00;

    }
    else if (color == "yellow"){
      color_values[0] = 0x3F;
      color_values[1] = 0x3F;
      color_values[2] = 0x00;

    }
    else if (color == "green"){
      color_values[0] = 0x00;
      color_values[1] = 0x3F;
      color_values[2] = 0x00;
    }
    else if (color == "blue"){
      color_values[0] = 0x00;
      color_values[1] = 0x00;
      color_values[2] = 0x3F;
    }
    else if (color == "violet"){
      color_values[0] = 0x1F;
      color_values[1] = 0x00;
      color_values[2] = 0x3F;
     }
    else if (color == "pink"){
      color_values[0] = 0x3F;
      color_values[1] = 0x1A;
      color_values[2] = 0x3A;
      }
    else if (color == "magenta"){
      color_values[0] = 0x3F;
      color_values[1] = 0x0C;
      color_values[2] = 0x3F;
      }
  }


} // extern "C"
