#ifndef ADVERTISING_H_
#define ADVERTISING_H_
#include <stdbool.h>
#include <stdint.h>
typedef struct {
  uint8_t *p_data;   /**< Pointer to data. */
  uint16_t data_len; /**< Length of data. */
} data_t;
void advertising_start(void);
void advertising_stop(void);
void advertising_init(void);
uint32_t adv_report_parse(uint8_t type, data_t *p_advdata, data_t *p_typedata);
#endif
