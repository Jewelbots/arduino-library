#include "advertising.h"
#include "app_error.h"
#include "ble.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_srv_common.h"
#include "nordic_common.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "sdk_errors.h"
#include <stdint.h>



#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

#define MANUFACTURER_NAME                                                      \
  "Jewelbots" // Manufacturer. Will be passed to Device Information Service.

#define APP_COMPANY_IDENTIFIER 0x213
//#define APP_COMPANY_IDENTIFIER					 0x0CCC
#define APP_ADV_INTERVAL   MSEC_TO_UNITS(80, UNIT_0_625_MS)
#define APP_ADV_TIMEOUT_IN_SECONDS 5
	/**< The advertising timeout in units of seconds. */

static ble_uuid_t m_adv_uuids[] = {
    {BLE_UUID_HEART_RATE_SERVICE, BLE_UUID_TYPE_BLE},
    {BLE_UUID_RUNNING_SPEED_AND_CADENCE, BLE_UUID_TYPE_BLE}};

static void on_adv_evt(ble_adv_evt_t ble_adv_evt) {
  switch (ble_adv_evt) {
  case BLE_ADV_EVT_FAST:
    // LEDS_ON(PERIPHERAL_ADVERTISING_LED);
    break;

  case BLE_ADV_EVT_IDLE: {
    ret_code_t err_code;
    advertising_init();
    err_code = ble_advertising_start(BLE_ADV_MODE_FAST);
    APP_ERROR_CHECK(err_code);
  } break;

  default:
    // No implementation needed.
    break;
  }
}

void advertising_start() { advertising_init(); }

void advertising_init() {
  // uint8_array_t custom_data;
  uint32_t err_code;
  ble_advdata_t advdata;
  ble_advdata_manuf_data_t manufacturer_specific_data;

  memset(&advdata, 0, sizeof(advdata));
  memset(&manufacturer_specific_data, 0, sizeof(manufacturer_specific_data));

  manufacturer_specific_data.company_identifier = 0x2103;
  // manufacturer_specific_data.data
  advdata.name_type = BLE_ADVDATA_FULL_NAME;
  advdata.include_appearance = true;
  advdata.flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
  advdata.uuids_complete.uuid_cnt =
      sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
  advdata.uuids_complete.p_uuids = m_adv_uuids;
  advdata.p_manuf_specific_data = &manufacturer_specific_data;

  ble_adv_modes_config_t options = {0};
  options.ble_adv_fast_enabled = BLE_ADV_FAST_ENABLED;
  options.ble_adv_fast_interval = APP_ADV_INTERVAL;
  options.ble_adv_fast_timeout = BLE_GAP_ADV_TIMEOUT_GENERAL_UNLIMITED;

  err_code = ble_advertising_init(&advdata, NULL, &options, on_adv_evt, NULL);
  APP_ERROR_CHECK(err_code);
  // err_code = ble_advertising_start(BLE_ADV_MODE_FAST);
  // APP_ERROR_CHECK(err_code);
}

void advertising_stop() {
	uint32_t err_code;
	err_code = ble_advertising_start(BLE_ADV_MODE_IDLE);
	APP_ERROR_CHECK(err_code);
}
/**
 * @brief Parses advertisement data, providing length and location of the field
 * in case
 *        matching data is found.
 *
 * @param[in]  Type of data to be looked for in advertisement data.
 * @param[in]  Advertisement report length and pointer to report.
 * @param[out] If data type requested is found in the data report, type data
 * length and
 *             pointer to data will be populated here.
 *
 * @retval NRF_SUCCESS if the data type is found in the report.
 * @retval NRF_ERROR_NOT_FOUND if the data type could not be found.
 */

uint32_t adv_report_parse(uint8_t type, data_t *p_advdata, data_t *p_typedata) {
  uint32_t index = 0;
  uint8_t *p_data;

  p_data = p_advdata->p_data;

  while (index < p_advdata->data_len) {
    uint8_t field_length = p_data[index];
    uint8_t field_type = p_data[index + 1];
    if (field_type == type) {
      p_typedata->p_data = &p_data[index + 2];
      p_typedata->data_len = field_length - 1;
      return NRF_SUCCESS;
    }
    index += field_length + 1;
  }
  return NRF_ERROR_NOT_FOUND;
}



#ifdef __cplusplus
}
#endif // __cplusplus
