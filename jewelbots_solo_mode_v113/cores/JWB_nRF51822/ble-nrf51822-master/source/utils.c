#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "ble_gap.h"
#include "fstorage.h"
#include "nrf_soc.h"
#include "nrf_log.h"

#include "utils.h"

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

bool arduino_coding = false;
bool arduino_button = false;
bool run_loop_charging = false;

void set_arduino_coding(void) { arduino_coding = true; }
void set_arduino_coding_false(void) { arduino_coding = false; }
bool get_arduino_coding(void) { return arduino_coding; }

void set_arduino_button(void) { arduino_button = true; }
bool get_arduino_button(void) { return arduino_button; }

void set_run_loop_charging(void) {run_loop_charging = true; }
bool get_run_loop_charging(void) { return run_loop_charging; }

void ble_address_to_string_convert(ble_gap_addr_t address,
                                   uint8_t *string_buffer) {
  const int address_length = 6;
  char temp_str[3];
  int i = 0;

  for (i = address_length - 1; i >= 0; --i) {
    sprintf(temp_str, "%02X", address.addr[i]);
    strcat((char *)string_buffer, temp_str);
  }
}

bool flash_access_in_progress() {
  uint32_t err_code;
  uint32_t count = 0;

  err_code = fs_queued_op_count_get(&count);
  if ((err_code != NRF_ERROR_INVALID_STATE) && (err_code != NRF_SUCCESS)) {
    NRF_LOG_PRINTF_DEBUG("[ADV]: pstorage_access_status_get returned %d.\r\n",
                   err_code);
    return true;
  }

  if (err_code == NRF_ERROR_INVALID_STATE) {
    err_code = fs_queued_op_count_get(&count);
    if (err_code != FS_SUCCESS) {
      return false;
    }
    NRF_LOG_PRINTF_DEBUG("[ADV]: fs_queued_op_count_get gives count %d.\r\n", count);
  }

  if (count != 0) {
    return true;
  } else {
    return false;
  }
}

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
