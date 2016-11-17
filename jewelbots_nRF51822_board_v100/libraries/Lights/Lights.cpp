#include "led_sequence.h"

void logo_breathe(void) {
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
