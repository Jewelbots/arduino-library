
#include "Animation.h"
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



  Animation::Animation()
  {}

  Animation::~Animation()
  {}

  extern void Animation::breathe_all(uint8_t color){
    enable_led();
    clear_led();
    // enable_led();
    led_cmd_t led1 = {3, 0x00, 0x00, 0x00, 0};
    led_cmd_t led2 = {1, 0x00, 0x00, 0x00, 0};
    led_cmd_t led3 = {2, 0x00, 0x00, 0x00, 0};
    led_cmd_t led4 = {0, 0x00, 0x00, 0x00, 0};
    for (uint8_t i = 0; i < 31; i++) {
      if (color == 1) {
        led1.r = led1.r + 1;
        led2.r = led2.r + 1;
        led3.r = led3.r + 1;
        led4.r = led4.r + 1;
      }
      if (color == 2) {
        led1.g = led1.g + 1;
        led2.g = led2.g + 1;
        led3.g = led3.g + 1;
        led4.g = led4.g + 1;
      }
      if (color == 3) {
        led1.b = led1.b + 1;
        led2.b = led2.b + 1;
        led3.b = led3.b + 1;
        led4.b = led4.b + 1;
      }
      if (color == 4) {
        led1.r = led1.r + 1;
        led2.r = led2.r + 1;
        led3.r = led3.r + 1;
        led4.r = led4.r + 1;
        led1.b = led1.b + 1;
        led2.b = led2.b + 1;
        led3.b = led3.b + 1;
        led4.b = led4.b + 1;
      }
      set_led_state_handler(&led1);
      set_led_state_handler(&led2);
      set_led_state_handler(&led3);
      set_led_state_handler(&led4);
      nrf_delay_us(30000);
    }
    nrf_delay_us(100);
    for (uint8_t i = 31; i > 0; i--) {
      if (color == 1) {
        led1.r = led1.r - 1;
        led2.r = led2.r - 1;
        led3.r = led3.r - 1;
        led4.r = led4.r - 1;
      }
      if (color == 2) {
        led1.g = led1.g - 1;
        led2.g = led2.g - 1;
        led3.g = led3.g - 1;
        led4.g = led4.g - 1;
      }
      if (color == 3) {
        led1.b = led1.b - 1;
        led2.b = led2.b - 1;
        led3.b = led3.b - 1;
        led4.b = led4.b - 1;
      }
      if (color == 4) {
        led1.r = led1.r - 1;
        led2.r = led2.r - 1;
        led3.r = led3.r - 1;
        led4.r = led4.r - 1;
        led1.b = led1.b - 1;
        led2.b = led2.b - 1;
        led3.b = led3.b - 1;
        led4.b = led4.b - 1;
      }
      set_led_state_handler(&led1);
      set_led_state_handler(&led2);
      set_led_state_handler(&led3);
      set_led_state_handler(&led4);
      nrf_delay_us(30000);
    }
    nrf_delay_us(10000);
    clear_led();
    disable_led();
  }

  void Animation::jewelbots_logo(void){

  enable_led();
  clear_led();
  // Fade in the logo colors quickly
  led_cmd_t led1 = {3, 0x00, 0x00, 0x00, 0};
  led_cmd_t led2 = {1, 0x00, 0x00, 0x00, 0};
  led_cmd_t led3 = {2, 0x00, 0x00, 0x00, 0};
  led_cmd_t led4 = {0, 0x00, 0x00, 0x00, 0};
  for (uint8_t i = 0; i < 12; i++) {
    led1.r = led1.r + 5;
    led2.r = led2.r + 0;
    led3.r = led3.r + 0;
    led4.r = led4.r + 0;
    led1.g = led1.g + 0;
    led2.g = led2.g + 2;
    led3.g = led3.g + 2;
    led4.g = led4.g + 2;
    led1.b = led1.b + 0;
    led2.b = led2.b + 1;
    led3.b = led3.b + 1;
    led4.b = led4.b + 1;

    set_led_state_handler(&led1);
    set_led_state_handler(&led2);
    set_led_state_handler(&led3);
    set_led_state_handler(&led4);
    nrf_delay_us(40000);
  }

  //  Rotate the logo colors around the face
  led_cmd_t red_hold = {3, 0x3F, 0x00, 0x00, 0};
  led_cmd_t teal_hold1 = {1, 0x00, 0x18, 0x0C, 0};
  led_cmd_t teal_hold2 = {2, 0x00, 0x18, 0x0C, 0};
  led_cmd_t teal_hold3 = {0, 0x00, 0x18, 0x0C, 0};
  set_led_state_handler(&red_hold);
  set_led_state_handler(&teal_hold1);
  set_led_state_handler(&teal_hold2);
  set_led_state_handler(&teal_hold3);

  uint8_t red_led[] = {3, 0, 1, 2};
  uint8_t teal_led[] = {2, 3, 0, 1};
  for (uint8_t i = 0; i < 6; i++) {
    for (uint8_t j = 0; j < 4; j++) {
      led_cmd_t red_ch = {red_led[j], 0x3F, 0x00, 0x00, 0};
      led_cmd_t teal_ch = {teal_led[j], 0x00, 0x24, 0x18, 0};
      set_led_state_handler(&red_ch);
      set_led_state_handler(&teal_ch);
      nrf_delay_us(80000);
    }
    haptics_test_run1();
  }

  //  quickly fade out the colors
  for (uint8_t i = 12; i > 0; i--) {
    led1.r = led1.r - 5;
    led2.r = led2.r - 0;
    led3.r = led3.r - 0;
    led4.r = led4.r - 0;
    led1.g = led1.g - 0;
    led2.g = led2.g - 2;
    led3.g = led3.g - 2;
    led4.g = led4.g - 2;
    led1.b = led1.b - 0;
    led2.b = led2.b - 1;
    led3.b = led3.b - 1;
    led4.b = led4.b - 1;

    set_led_state_handler(&led1);
    set_led_state_handler(&led2);
    set_led_state_handler(&led3);
    set_led_state_handler(&led4);
    nrf_delay_us(40000);
  }
  nrf_delay_us(10000);
  clear_led();
  disable_led();
}

void Animation::rainbows(void){

    enable_led();
    clear_led();
    led_cmd_t red = {0, 0x3F, 0x00, 0x00, 1};
    led_cmd_t orange = {1, 0x3F, 0x1C, 0x00, 1};
    led_cmd_t yellow = {2, 0x3F, 0x3F, 0x00, 1};
    led_cmd_t green = {3, 0x00, 0x3F, 0x00, 1};
    led_cmd_t blue = {0, 0x00, 0x00, 0x3F, 1};
    led_cmd_t violet = {1, 0x1F, 0x00, 0x3F, 1};
    led_cmd_t pink = {2, 0x3F, 0x1A, 0x3A, 1};
    led_cmd_t magenta = {3, 0x3F, 0x0C, 0x3F, 1};

    int delay = 100000;
    for (int i = 0; i < 4; i++) {
      set_led_state_handler(&red);
      nrf_delay_us(delay);
      set_led_state_handler(&orange);
      nrf_delay_us(delay);
      set_led_state_handler(&yellow);
      nrf_delay_us(delay);
      set_led_state_handler(&green);
      nrf_delay_us(delay);
      set_led_state_handler(&blue);
      nrf_delay_us(delay);
      set_led_state_handler(&violet);
      nrf_delay_us(delay);
      set_led_state_handler(&pink);
      nrf_delay_us(delay);
      set_led_state_handler(&magenta);
      nrf_delay_us(delay);
    }
    clear_led();
    disable_led();
}
} // extern "C"
