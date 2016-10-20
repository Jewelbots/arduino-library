#ifndef BLE_CENTRAL_EVENT_HANDLER_H
#define BLE_CENTRAL_EVENT_HANDLER_H
#include "ble.h"
#include "ble_gap.h"
#include "ble_srv_common.h"
#include "ble_types.h"
#include <stdint.h>

void ble_stack_init(void);
void gap_params_init(void);
void on_ble_central_evt(const ble_evt_t *const p_ble_evt);
void disconnect(void);
void conn_params_init(void);
uint16_t get_current_connection_handle(void);
typedef struct {
  uint16_t manuf_id;       /** manufacturer specific identifier*/
  uint8_t beacon_dev_type; /** manufacturer data type*/
  ble_uuid128_t uuid;      /** 128 uuid*/
  uint16_t major;          /** beacon Major*/
  uint16_t minor;          /** beacon Minor*/
  uint16_t rssi;           /** beacon Rssi */
} adv_data_t;

#define NRF_CLOCK_LFCLKSRC                                                     \
  {                                                                            \
    .source = NRF_CLOCK_LF_SRC_RC, .rc_ctiv = 16, .rc_temp_ctiv = 2,           \
    .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_250_PPM                        \
  }

#endif
