#include "dfu_handler.h"
#include "app_error.h"
#include "ble_conn_params.h"
#include "ble_dfu.h"
#include "dfu_app_handler.h"
#include "haptics_driver.h"
#include "led_driver.h"
#include "led_sequence.h"
#include "nrf_log.h"
#include "ble_central_event_handler.h"
#include "advertising.h"
#include "scan.h"

#define IS_SRVC_CHANGED_CHARACT_PRESENT 1

#define DFU_REV_MAJOR 0x00
#define DFU_REV_MINOR 0x01
#define DFU_REVISION ((DFU_REV_MAJOR << 8) | DFU_REV_MINOR)

static ble_dfu_t m_dfus;

static void reset_prepare(void) {
  clear_led();
  disable_led();
	disconnect();
#ifdef REAL_JEWELBOT
  haptics_standby();
  haptics_disable();
#endif
	advertising_stop();
	scan_stop();
  ble_conn_params_stop();
}

void notify_dfu_ble_evt(ble_evt_t *p_ble_evt) {
  ble_dfu_on_ble_evt(&m_dfus, p_ble_evt);
}

void dfu_handler_init(void) {
  ble_dfu_init_t dfus_init;
  uint32_t err_code;
  memset(&dfus_init, 0, sizeof(dfus_init));

  dfus_init.evt_handler = dfu_app_on_dfu_evt;
  dfus_init.error_handler = NULL;
  dfus_init.evt_handler = dfu_app_on_dfu_evt;
  dfus_init.revision = DFU_REVISION;

  err_code = ble_dfu_init(&m_dfus, &dfus_init);
  NRF_LOG_PRINTF("dfu_handler_init: %u\n", err_code);
  APP_ERROR_CHECK(err_code);

  dfu_app_reset_prepare_set(
      reset_prepare); // todo: add shutdown for all Jewelbot stuff to this.
}

void bootloader_start(void) {
  // reset Jewelbots services
  breathe_all(4);
  reset_prepare();
  // A "simple" reset is not enough, the SD and IRQs must be disabled first
  ble_dfu_evt_t evt;
  memset(&evt, 0, sizeof(evt));
  evt.ble_dfu_evt_type = BLE_DFU_START;
  dfu_app_on_dfu_evt(&m_dfus, &evt);
}
