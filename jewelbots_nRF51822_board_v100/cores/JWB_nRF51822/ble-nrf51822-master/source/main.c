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
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#ifndef DEMO_MODE
#include "nrf_drv_timer.h"
#endif
#include "nrf51_bitfields.h"
#include "nrf_drv_twi.h"
#include "nrf_drv_wdt.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_soc.h"
#include "softdevice_handler_appsh.h"
// application

#include "Haptics.h"
#include "ble_central_event_handler.h"
#include "button_handler.h"
#include "db_discovery.h"
#include "haptics_driver.h"
#include "hardfault.h"
#include "jewelbot_gpio.h"
#include "led_driver.h"
#include "jewelbot_types.h"
#include "led_sequence.h"
#include "pmic_driver.h"
#include "scan.h"
#include "Arduino.h"


#define UART_TX_BUF_SIZE                                                       \
  256 /**< Size of the UART TX buffer, in bytes. Must be a power of two. */
#define UART_RX_BUF_SIZE                                                       \
  1 /**< Size of the UART RX buffer, in bytes. Must be a power of two. */

/* Central related. */

#define CENTRAL_SCANNING_LED BSP_LED_0_MASK
#define CENTRAL_CONNECTED_LED BSP_LED_1_MASK

#define APP_TIMER_PRESCALER 0 /**< Value of the RTC1 PRESCALER register. */

/**< Determines scan window in units of 0.625 millisecond. */

/**< Size of a UUID, in bytes. */

#define SCHED_MAX_EVENT_DATA_SIZE                                              \
  ((CEIL_DIV(MAX(MAX(BLE_STACK_EVT_MSG_BUF_SIZE, ANT_STACK_EVT_STRUCT_SIZE),   \
                 SYS_EVT_MSG_BUF_SIZE),                                        \
             sizeof(uint32_t))) *                                              \
   sizeof(uint32_t))
#define SCHED_QUEUE_SIZE 16u

/**@brief Variable length data encapsulation in terms of length and pointer to
 * data. */

/** @brief Scan parameters requested for scanning and connection. */

#ifdef REAL_JEWELBOT
static char *__attribute__((unused)) ident =
    "$Build: JEWELBOT " __DATE__ ", " __TIME__ " $";
#else
static char *__attribute__((unused)) ident =
    "$Build: DEVKIT " __DATE__ ", " __TIME__ " $";
#endif

/**@brief Connection parameters requested for connection. */

/* Peripheral related. */

//#define PERIPHERAL_ADVERTISING_LED       BSP_LED_2_MASK
//#define PERIPHERAL_CONNECTED_LED         BSP_LED_3_MASK

/**@brief UUIDs which the central applications will scan for, and which will be
 * advertised by the peripherals. */
//

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

nrf_drv_twi_t app_twi_instance = NRF_DRV_TWI_INSTANCE(1);

static void leds_init(void) {
#ifndef REAL_JEWELBOT
  nrf_gpio_cfg_output(LED_0);
  nrf_gpio_cfg_output(LED_1);
  nrf_gpio_cfg_output(LED_2);
  nrf_gpio_cfg_output(LED_3);
  nrf_gpio_pin_set(LED_0);
  nrf_gpio_pin_set(LED_1);
  nrf_gpio_pin_clear(LED_2);
  nrf_gpio_pin_clear(LED_3);
#else
// nothing right now
#endif
}

static void scheduler_init(void) {
  APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
}

#define APP_GPIOTE_MAX_USERS 6
static void timers_init(void) {
  APP_TIMER_APPSH_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, true);
}

static void twi_init() {
  uint32_t err_code;
  const nrf_drv_twi_config_t twi_config = {
      .scl = I2C_SCL,
      .sda = I2C_SDA,
      .frequency = NRF_TWI_FREQ_100K, // was 100K
      .interrupt_priority = APP_IRQ_PRIORITY_HIGH};

  err_code = nrf_drv_twi_init(&app_twi_instance, &twi_config, NULL,
                              NULL); // todo add event handler
  APP_ERROR_CHECK(err_code);

  nrf_drv_twi_enable(&app_twi_instance);
}

// bool snarf[128];
// bool plif[128];
nrf_drv_wdt_channel_id m_channel_id;
#define DELAY_MS 100 /*!< Timer Delay in milli-seconds */
void wdt_event_handler(void) {
  // LEDS_OFF(LEDS_MASK);
}
int main(void) {
	bool  app_dfu = (NRF_POWER->GPREGRET == BOOTLOADER_DFU_END);

	if (app_dfu)
	{
			NRF_POWER->GPREGRET = 0;
	}
  ret_code_t err_code;
  // bool erase_bonds = false;
  nrf_gpio_cfg_output(LED_RST);
  nrf_gpio_pin_clear(LED_RST);
  nrf_delay_us(10000);
  nrf_gpio_pin_set(LED_RST);
  nrf_delay_us(20000);
  err_code = NRF_LOG_INIT();
  APP_ERROR_CHECK(err_code);
  timers_init();
  scheduler_init();
  twi_init(); // 1
#ifdef REAL_JEWELBOT
  pmic_init(); // 2
#endif
  services_init(); // 3
  // boot_up_led_sequence();
  leds_init(); // nrf devboard leds
  buttons_init();
#ifdef REAL_JEWELBOT
  haptics_init();
#endif
  breathe_all(1);
  breathe_all(2);
  breathe_all(3);
  ble_stack_init();
  db_discovery_init();
  gap_params_init();
  conn_params_init();
  advertising_init();
  scan_start();
  err_code = ble_advertising_start(BLE_ADV_MODE_FAST);
  APP_ERROR_CHECK(err_code);
  NRF_LOG("Exited bootup. Starting main loop.\r\n");
  //setup();
  for (;;) {
    demo_run();
    app_sched_execute();
    //loop();
#ifndef RTT
    ret_code_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
#endif
  }
}

/**
 * @}
 */
