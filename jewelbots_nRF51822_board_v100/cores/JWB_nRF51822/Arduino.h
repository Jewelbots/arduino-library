#ifndef ARDUINO_H_
#define ARDUINO_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>


// Exposing sdk and Jewelbots code for a start
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
#include "nrf_drv_twi.h"
#include "nrf_drv_wdt.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_soc.h"
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
// Arduino specific code
#include "LED.h"
#include "Buzzer.h"
#include "Animation.h"
#include "Timer.h"


#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

/* sketch */
extern void setup( void ) ;
extern void loop( void ) ;

#ifdef __cplusplus
}
#endif

#endif
