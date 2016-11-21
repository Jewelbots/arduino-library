#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "advertising.h"
#include "app_button.h"
#include "app_error.h"
#include "app_scheduler.h"
#include "app_timer.h"
#include "app_timer_appsh.h"
#include "app_trace.h"
#include "app_uart.h"
#include "bootloader_types.h"
#include "ble.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "ble_conn_state.h"
#include "ble_db_discovery.h"
#include "common_defines.h"
#include "nordic_common.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_drv_clock.h"
#include "nrf_drv_spi.h"
#include "nrf_drv_timer.h"
#include "nrf51_bitfields.h"
#include "nrf_drv_wdt.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_soc.h"
#include "jwb_twi.h"
#include "softdevice_handler_appsh.h"
// application

#include "ble_central_event_handler.h"
#include "button_handler.h"
#include "db_discovery.h"
#include "Haptics.h"
#include "haptics_driver.h"
#include "hardfault.h"
#include "jewelbot_gpio.h"
#include "jewelbot_types.h"
#include "led_driver.h"
#include "led_sequence.h"
#include "pmic_driver.h"
#include "scan.h"

// Arduino
#include "Arduino.h"



#define ARDUINO_MAIN

#define UART_TX_BUF_SIZE                                                       \
  256 /**< Size of the UART TX buffer, in bytes. Must be a power of two. */
#define UART_RX_BUF_SIZE                                                       \
  1 /**< Size of the UART RX buffer, in bytes. Must be a power of two. */

/* Central related. */

#define CENTRAL_SCANNING_LED BSP_LED_0_MASK
#define CENTRAL_CONNECTED_LED BSP_LED_1_MASK

#define APP_TIMER_PRESCALER 0 /**< Value of the RTC1 PRESCALER register. */

#define APP_GPIOTE_MAX_USERS 6

#define SCHED_MAX_EVENT_DATA_SIZE                                              \
  ((CEIL_DIV(MAX(MAX(BLE_STACK_EVT_MSG_BUF_SIZE, ANT_STACK_EVT_STRUCT_SIZE),   \
                 SYS_EVT_MSG_BUF_SIZE),                                        \
             sizeof(uint32_t))) *                                              \
   sizeof(uint32_t))
#define SCHED_QUEUE_SIZE 16u

#ifdef REAL_JEWELBOT
static char *__attribute__((unused)) ident =
    "$Build: JEWELBOT " __DATE__ ", " __TIME__ " $";
#else
static char *__attribute__((unused)) ident =
    "$Build: DEVKIT " __DATE__ ", " __TIME__ " $";
#endif


/**@brief Function to handle asserts in the SoftDevice.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product.
 * You need to analyze
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in] line_num     Line number of the failing ASSERT call.
 * @param[in] p_file_name  File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t *p_file_name) {
  app_error_handler(0xDEADBEEF, line_num, p_file_name);
}

void uart_error_handle(app_uart_evt_t *p_event) {
  if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR) {
    APP_ERROR_HANDLER(p_event->data.error_communication);
  } else if (p_event->evt_type == APP_UART_FIFO_ERROR) {
    APP_ERROR_HANDLER(p_event->data.error_code);
  }
}

/*
  MEAT OF PROGRAM STARTS HERE.  BOILERPLATE ABOVE.  DOMAIN LOGIC BELOW.
*/

time_t jwbs_last_sync;

static void scheduler_init(void) {
  APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
}

static void timers_init(void) {
  APP_TIMER_APPSH_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, true);
}


int main(void) {
  //Set register flag on chip after exiting bootloader
  bool  app_dfu = (NRF_POWER->GPREGRET == BOOTLOADER_DFU_END);

	if (app_dfu)
	{
			NRF_POWER->GPREGRET = 0;
	}

  // Jewelbots Hardware Init
  ret_code_t err_code;
  nrf_gpio_cfg_output(LED_RST);
  nrf_gpio_pin_clear(LED_RST);
  nrf_gpio_pin_set(LED_RST);
  err_code = NRF_LOG_INIT();
  APP_ERROR_CHECK(err_code);
  timers_init();
  scheduler_init();
  twi_init();
  pmic_init();
  services_init();
  buttons_init();
  haptics_init();
  // Bluetooth init
  ble_stack_init();
  db_discovery_init();
  gap_params_init();
  conn_params_init();
  advertising_init();
  //check_reset_reason();
  scan_start();
  err_code = ble_advertising_start(BLE_ADV_MODE_FAST);
  APP_ERROR_CHECK(err_code);
  boot_up_led_sequence();
  // Run additional init from the setup function
  setup();
  // Main loop
  for (;;) {
    display_charging();
    app_sched_execute();
    if (!pmic_5V_present()) {
      //Run Arduino loop function
      loop();
    }
    // Put the device into low power mode until next event occurs
    ret_code_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
  }
}
