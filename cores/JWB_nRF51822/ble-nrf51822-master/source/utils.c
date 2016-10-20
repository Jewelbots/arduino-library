#include "utils.h"
#include "ble_gap.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

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
