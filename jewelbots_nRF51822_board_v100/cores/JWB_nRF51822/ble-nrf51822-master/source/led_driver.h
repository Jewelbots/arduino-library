#ifndef __LED_DRIVER_H__
#define __LED_DRIVER_H__

#include "jewelbot_types.h"
#include <stdbool.h>

/**@brief Function for initializing services that will be used by the
 * application.
 */



 #ifdef __cplusplus
 extern "C"{
 #endif // __cplusplus

void services_init(void);
void led_test_run(void);
void enable_led(void);
void disable_led(void);
void set_led_state_handler(led_cmd_t *state);
// JWBS -> global jewelbot state variable
extern time_t jwbs_tod; // current time of day
extern time_t jwbs_last_sync;
extern uint8_t* color_lookup(char *color);
#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
#endif
