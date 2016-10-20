#include "button_handler.h"
#include "app_button.h"
#include "app_timer.h"
#include "bootloader_types.h"
#include "dfu_handler.h"
#include "haptics_driver.h"
#include "jewelbot_gpio.h"
#include "led_driver.h"
#include "led_sequence.h"
#include "nrf_log.h"
#include "nrf_soc.h"
#include "pmic_driver.h"
#include "sdk_config.h"

#ifdef REAL_JEWELBOT
#define BUTTON_DETECTION_DELAY APP_TIMER_TICKS(50, APP_TIMER_PRESCALER)
#else
#define BUTTON_DETECTION_DELAY APP_TIMER_TICKS(50, APP_TIMER_PRESCALER)
#endif
#define BUTTON_PRESS_LONG_TICKS 65000
#define BUTTON_PRESS_TICKS 1000

// for determining how long button was held down
static uint32_t button_tick = 0;
static uint32_t button_tick_release = 0;
static uint32_t total_ticks = 0;

// for demo
static volatile uint8_t button_sequence_number = 0;
static volatile uint8_t last_button_sequence_number = 255;

APP_TIMER_DEF(m_button_tick_timer);

static void button_handler(uint8_t button, uint8_t action) {
  uint32_t err_code;
  switch (action) {
  case APP_BUTTON_PUSH:
    app_timer_start(m_button_tick_timer, 65535, NULL);
    err_code = app_timer_cnt_get(&button_tick);
    APP_ERROR_CHECK(err_code);
    break;
  case APP_BUTTON_RELEASE:
    err_code = app_timer_cnt_get(&button_tick_release);
    APP_ERROR_CHECK(err_code);
    err_code = app_timer_stop(m_button_tick_timer);
    APP_ERROR_CHECK(err_code);
    err_code = app_timer_cnt_diff_compute(button_tick_release, button_tick,
                                          &total_ticks);
    APP_ERROR_CHECK(err_code);
    if (total_ticks > BUTTON_PRESS_TICKS) {
      button_sequence_number++;
    }
    if (pmic_5V_present() && total_ticks > BUTTON_PRESS_LONG_TICKS) {
      err_code = sd_power_gpregret_set(DFU_START_USB);
			APP_ERROR_CHECK(err_code);
			bootloader_start();
    }
    break;
  }
}

static void timer_handler(void *p_context) { UNUSED_PARAMETER(p_context); }

void buttons_init(void) {
  // Note: Array must be static because a pointer to it will be saved in the
  // Button handler
  //       module.
  uint32_t err_code = 0;
#ifdef REAL_JEWELBOT
  static app_button_cfg_t buttons[] = {
      {BUTTON_PIN, APP_BUTTON_ACTIVE_HIGH, NRF_GPIO_PIN_NOPULL, button_handler},
  };
#else
  static app_button_cfg_t buttons[] = {
      {BUTTON_PIN, APP_BUTTON_ACTIVE_LOW, NRF_GPIO_PIN_PULLUP, button_handler}};
#endif
  err_code = app_button_init((app_button_cfg_t *)buttons,
                             sizeof(buttons) / sizeof(buttons[0]),
                             BUTTON_DETECTION_DELAY);
  APP_ERROR_CHECK(err_code);
  err_code = app_button_enable();
  APP_ERROR_CHECK(err_code);
#ifdef REAL_JEWELBOT
  nrf_gpio_cfg_sense_input(WAKEUP_BUTTON_PIN, NRF_GPIO_PIN_NOPULL,
                           NRF_GPIO_PIN_SENSE_LOW);
#endif
  err_code = app_timer_create(&m_button_tick_timer, APP_TIMER_MODE_REPEATED,
                              timer_handler);
}

void demo_run() {
  if (pmic_5V_present()) {
    led_indicate_charging_state(pmic_is_charging());
  } else {
		clear_led();
	}
  if (button_sequence_number != last_button_sequence_number) {
    last_button_sequence_number = button_sequence_number;
    clear_led();
    switch (button_sequence_number) {
    case 0:
      break;
    case 1:
      haptics_test_run2();
      boot_up_led_sequence();
      if (pmic_5V_present()) {
        led_indicate_charging_state(pmic_is_charging());
      }
      break;
    case 2:
      logo_breathe();
      if (pmic_5V_present()) {
        led_indicate_charging_state(pmic_is_charging());
      }
      break;
    case 3:
#ifdef REAL_JEWELBOT
      haptics_test_run3();
#endif
      breathe_all(1);
#ifdef REAL_JEWELBOT
      haptics_test_run3();
#endif
      breathe_all(2);
#ifdef REAL_JEWELBOT
      haptics_test_run3();
#endif
      breathe_all(3);
      button_sequence_number = 0;
      if (pmic_5V_present()) {
        led_indicate_charging_state(pmic_is_charging());
      }
      break;
    default:
      button_sequence_number = 0;
      break;
    }
  }
}
