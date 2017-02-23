#ifndef DFU_HANDLER_H
#define DFU_HANDLER_H
#include "ble.h"
void dfu_handler_init(void);
void notify_dfu_ble_evt(ble_evt_t *p_ble_evt);
void bootloader_start(void);
#endif
