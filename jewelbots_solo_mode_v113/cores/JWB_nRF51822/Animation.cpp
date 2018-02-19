
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


#ifdef __cplusplus
extern "C"{
#endif // __cplusplus




  Animation::Animation()
  {
    //Init or enable code here causes the Jewelbot to lock up on reset
  }

  Animation::~Animation()
  {}

  Timer animation_timer;

  void Animation::breathe_single_color(ColorLabel color){
    enable_led();
    clear_led();
    // enable_led();
    led_cmd_t led1 = {3, 0x00, 0x00, 0x00, 0};
    led_cmd_t led2 = {1, 0x00, 0x00, 0x00, 0};
    led_cmd_t led3 = {2, 0x00, 0x00, 0x00, 0};
    led_cmd_t led4 = {0, 0x00, 0x00, 0x00, 0};
    for (uint8_t i = 0; i < 31; i++) {
      if (color == RED) {
        led1.r = led1.r + 1;
        led2.r = led2.r + 1;
        led3.r = led3.r + 1;
        led4.r = led4.r + 1;
      }
      if (color == GREEN) {
        led1.g = led1.g + 1;
        led2.g = led2.g + 1;
        led3.g = led3.g + 1;
        led4.g = led4.g + 1;
      }
      if (color == BLUE) {
        led1.b = led1.b + 1;
        led2.b = led2.b + 1;
        led3.b = led3.b + 1;
        led4.b = led4.b + 1;
      }
      if (color == YELLOW) {
        led1.r = led1.r + 1;
        led2.r = led2.r + 1;
        led3.r = led3.r + 1;
        led4.r = led4.r + 1;
        led1.g = led1.g + 1;
        led2.g = led2.g + 1;
        led3.g = led3.g + 1;
        led4.g = led4.g + 1;
      }
      if (color == MAGENTA) {
        led1.r = led1.r + 1;
        led2.r = led2.r + 1;
        led3.r = led3.r + 1;
        led4.r = led4.r + 1;
        led1.b = led1.b + 1;
        led2.b = led2.b + 1;
        led3.b = led3.b + 1;
        led4.b = led4.b + 1;
      }
      if (color == CYAN) {
        led1.g = led1.g + 1;
        led2.g = led2.g + 1;
        led3.g = led3.g + 1;
        led4.g = led4.g + 1;
        led1.b = led1.b + 1;
        led2.b = led2.b + 1;
        led3.b = led3.b + 1;
        led4.b = led4.b + 1;
      }
      if (color == WHITE) {
        led1.r = led1.r + 1;
        led2.r = led2.r + 1;
        led3.r = led3.r + 1;
        led4.r = led4.r + 1;
        led1.g = led1.g + 1;
        led2.g = led2.g + 1;
        led3.g = led3.g + 1;
        led4.g = led4.g + 1;
        led1.b = led1.b + 1;
        led2.b = led2.b + 1;
        led3.b = led3.b + 1;
        led4.b = led4.b + 1;
      }
      set_led_state_handler(&led1);
      set_led_state_handler(&led2);
      set_led_state_handler(&led3);
      set_led_state_handler(&led4);
      animation_timer.pause(30);
    }
    for (uint8_t i = 31; i > 0; i--) {
      if (color == RED) {
        led1.r = led1.r - 1;
        led2.r = led2.r - 1;
        led3.r = led3.r - 1;
        led4.r = led4.r - 1;
      }
      if (color == GREEN) {
        led1.g = led1.g - 1;
        led2.g = led2.g - 1;
        led3.g = led3.g - 1;
        led4.g = led4.g - 1;
      }
      if (color == BLUE) {
        led1.b = led1.b - 1;
        led2.b = led2.b - 1;
        led3.b = led3.b - 1;
        led4.b = led4.b - 1;
      }
      if (color == YELLOW) {
        led1.r = led1.r - 1;
        led2.r = led2.r - 1;
        led3.r = led3.r - 1;
        led4.r = led4.r - 1;
        led1.g = led1.g - 1;
        led2.g = led2.g - 1;
        led3.g = led3.g - 1;
        led4.g = led4.g - 1;
      }
      if (color == MAGENTA) {
        led1.r = led1.r - 1;
        led2.r = led2.r - 1;
        led3.r = led3.r - 1;
        led4.r = led4.r - 1;
        led1.b = led1.b - 1;
        led2.b = led2.b - 1;
        led3.b = led3.b - 1;
        led4.b = led4.b - 1;
      }
      if (color == CYAN) {
        led1.g = led1.g - 1;
        led2.g = led2.g - 1;
        led3.g = led3.g - 1;
        led4.g = led4.g - 1;
        led1.b = led1.b - 1;
        led2.b = led2.b - 1;
        led3.b = led3.b - 1;
        led4.b = led4.b - 1;
      }
      if (color == WHITE) {
        led1.r = led1.r - 1;
        led2.r = led2.r - 1;
        led3.r = led3.r - 1;
        led4.r = led4.r - 1;
        led1.g = led1.g - 1;
        led2.g = led2.g - 1;
        led3.g = led3.g - 1;
        led4.g = led4.g - 1;
        led1.b = led1.b - 1;
        led2.b = led2.b - 1;
        led3.b = led3.b - 1;
        led4.b = led4.b - 1;
      }
      set_led_state_handler(&led1);
      set_led_state_handler(&led2);
      set_led_state_handler(&led3);
      set_led_state_handler(&led4);
      animation_timer.pause(30);
    }
    animation_timer.pause(10);
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
    animation_timer.pause(40);
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
      animation_timer.pause(80);
    }
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
    animation_timer.pause(40);
  }
  animation_timer.pause(10);
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

    int delay = 100;
    for (int i = 0; i < 4; i++) {
      set_led_state_handler(&red);
      animation_timer.pause(delay);
      set_led_state_handler(&orange);
      animation_timer.pause(delay);
      set_led_state_handler(&yellow);
      animation_timer.pause(delay);
      set_led_state_handler(&green);
      animation_timer.pause(delay);
      set_led_state_handler(&blue);
      animation_timer.pause(delay);
      set_led_state_handler(&violet);
      animation_timer.pause(delay);
      set_led_state_handler(&pink);
      animation_timer.pause(delay);
      set_led_state_handler(&magenta);
      animation_timer.pause(delay);
    }
    clear_led();
    disable_led();
}
#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
