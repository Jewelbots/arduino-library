#ifndef JEWELBOT_UTILS_H_
#define JEWELBOT_UTILS_H_
#include "ble_gap.h"
#include <stdint.h>
void ble_address_to_string_convert(ble_gap_addr_t address,
                                   uint8_t *string_buffer);
#endif
