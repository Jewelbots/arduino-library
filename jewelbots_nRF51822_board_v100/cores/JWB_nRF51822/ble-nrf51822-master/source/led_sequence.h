#ifndef __LED_SEQUENCE_H__
#define __LED_SEQUENCE_H__

#include <stdbool.h>
#include <stdint.h>

void breathe_all(uint8_t color);
void clear_led(void);
void boot_up_led_sequence(void);
void logo_breathe(void);
void led_indicate_charging_state(bool is_charging);
#endif
