#ifndef __LED_SEQUENCE_H__
#define __LED_SEQUENCE_H__


#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

#include <stdbool.h>
#include <stdint.h>

extern void clear_led(void);
extern void boot_up_led_sequence(void);
extern void led_indicate_charging_state(bool is_charging);
extern void breathe_all(uint8_t color);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif
