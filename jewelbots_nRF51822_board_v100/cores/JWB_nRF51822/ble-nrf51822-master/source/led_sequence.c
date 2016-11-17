
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



#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

#define RED 0x3F0000
#define GREEN 0x003F00
#define BLUE 0x00003F
#define YELLOW 0x3F3F00
#define CYAN 0x003F3F
#define MAGENTA 0x3F003F
#define PINK 0x3F0524
#define CHARTREUSE 0x1F3F00




extern void signal_power_down() {
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

  int delay = 65000;
  for (int i = 2; i > 0; i--) {
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

	led_cmd_t led1 = {3, 0x3F, 0x0C, 0x3F, 1};
  led_cmd_t led2 = {1, 0x22, 0x00, 0x3F, 1};
  led_cmd_t led3 = {2, 0x3F, 0x1C, 0x3A, 1};
  led_cmd_t led4 = {0, 0x00, 0x00, 0x3F, 1};

	//  quickly fade out the colors
  for (uint8_t i = 11; i > 0; i--) {
    led1.r = led1.r - 5;
    led2.r = led2.r - 3;
    led3.r = led3.r - 5;
    led4.r = led4.r - 0;
    led1.g = led1.g - 1;
    led2.g = led2.g - 0;
    led3.g = led3.g - 1;
    led4.g = led4.g - 0;
    led1.b = led1.b - 5;
    led2.b = led2.b - 5;
    led3.b = led3.b - 5;
    led4.b = led4.b - 5;

    set_led_state_handler(&led1);
    set_led_state_handler(&led2);
    set_led_state_handler(&led3);
    set_led_state_handler(&led4);
    nrf_delay_us(95000);
  }

  clear_led();
  disable_led();
}



extern void clear_led() {
  led_cmd_t clear1 = {0, 0x00, 0x00, 0x00, 0};
  led_cmd_t clear2 = {1, 0x00, 0x00, 0x00, 0};
  led_cmd_t clear3 = {2, 0x00, 0x00, 0x00, 0};
  led_cmd_t clear4 = {3, 0x00, 0x00, 0x00, 0};
  // enable_led();
  set_led_state_handler(&clear1);
  set_led_state_handler(&clear2);
  set_led_state_handler(&clear3);
  set_led_state_handler(&clear4);
  // disable_led();
}

extern void boot_up_led_sequence(void) {

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

extern void breathe_all(uint8_t color) {
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

extern void led_indicate_charging_state(bool is_charging) {
  enable_led(); // turn on, don't turn off. will be turned off after charging is
                // done.
  led_cmd_t red_on = {0, 0x3F, 0x00, 0x00, 1};
  led_cmd_t red_off = {0, 0x00, 0x00, 0x00, 1};
  led_cmd_t green_on = {3, 0x00, 0x3F, 0x00, 1};
  led_cmd_t green_off = {3, 0x00, 0x00, 0x00, 1};
  if (is_charging) {
    set_led_state_handler(&red_on);
    set_led_state_handler(&green_off);
  } else {
    set_led_state_handler(&green_on);
    set_led_state_handler(&red_off);
  }
}



#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
