#include "scan.h"
#include "app_error.h"
#include "ble.h"
#include "nordic_common.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "sdk_errors.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
//#include "nrf_log.h"

// 0x0168 //225ms
// 0x00A0 // 100 ms
#define SCAN_INTERVAL MSEC_TO_UNITS(240, UNIT_0_625_MS)
// 0x00E0 //140ms
// 0x0050 // 50 ms
#define SCAN_WINDOW MSEC_TO_UNITS(40, UNIT_0_625_MS)


static const ble_gap_scan_params_t m_scan_param = {
    1,    // Active scanning set.
    0,    // Selective scanning not set.
    NULL, // No whitelist provided.
    SCAN_INTERVAL,
    SCAN_WINDOW,
    0x0000 // No timeout.
};
const ble_gap_scan_params_t *get_scan_params(void) { return &m_scan_param; }

void scan_start(void) {
  ret_code_t err_code;

  sd_ble_gap_scan_stop();

  err_code = sd_ble_gap_scan_start(&m_scan_param);
  APP_ERROR_CHECK(err_code);
  // It is okay to ignore this error since we are stopping the scan anyway.
  if (err_code != NRF_ERROR_INVALID_STATE) {
    APP_ERROR_CHECK(err_code);
  }
}

void scan_stop(void) {
  uint32_t err_code = sd_ble_gap_scan_stop();
  if (err_code != NRF_ERROR_INVALID_STATE) {
    APP_ERROR_CHECK(err_code);
  }
}
