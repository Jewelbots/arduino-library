#ifndef BUTTON_HANDLER_H_
#define BUTTON_HANDLER_H_
#include <stdint.h>


#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

void buttons_init(void);
void demo_run(void);
void pmic_int_pin_handler(uint32_t event_pins_low_to_high,
                          uint32_t event_pins_high_to_low);
void display_charging(void);


#ifdef __cplusplus
}
#endif // __cplusplus
#endif
