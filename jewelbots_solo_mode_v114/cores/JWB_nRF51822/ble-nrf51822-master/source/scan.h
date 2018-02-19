#ifndef SCAN_H_
#define SCAN_H_
#include "ble_gap.h"

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus
const ble_gap_scan_params_t *get_scan_params(void);
void scan_start(void);
void scan_stop(void);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif
