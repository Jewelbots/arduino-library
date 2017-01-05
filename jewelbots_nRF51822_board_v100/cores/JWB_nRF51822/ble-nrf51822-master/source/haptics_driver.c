#include "app_timer.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "sdk_config.h"

#include "Actuator_Waveforms.h"
#include "Haptics.h"
#include "haptics_driver.h"
#include "pmic_driver.h"


#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

#define TICK_TIMEOUT APP_TIMER_TICKS(50, APP_TIMER_PRESCALER)
#define BUZZALERT_TIMEOUT APP_TIMER_TICKS(790, APP_TIMER_PRESCALER)
#define DOUBLESTRONGCLICK_TIMEOUT APP_TIMER_TICKS(170, APP_TIMER_PRESCALER)
#define PULSINGALERT_TIMEOUT APP_TIMER_TICKS(380, APP_TIMER_PRESCALER)
#define MSGEXTRASHORT_TIMEOUT APP_TIMER_TICKS(125, APP_TIMER_PRESCALER)
#define MSGSHORT_TIMEOUT APP_TIMER_TICKS(250, APP_TIMER_PRESCALER)
#define MSGMEDIUM_TIMEOUT APP_TIMER_TICKS(500, APP_TIMER_PRESCALER)
#define MSGLONG_TIMEOUT APP_TIMER_TICKS(750, APP_TIMER_PRESCALER)
#define MSGEXTRALONG_TIMEOUT APP_TIMER_TICKS(1000, APP_TIMER_PRESCALER)
#define MSGREALLYLONG_TIMEOUT APP_TIMER_TICKS(1500, APP_TIMER_PRESCALER)

APP_TIMER_DEF(haptics_timer_id);

static void haptics_timeout_handler(void *p_context) {

  UNUSED_PARAMETER(p_context);
  uint32_t err_code;
  err_code = app_timer_stop(haptics_timer_id);
  APP_ERROR_CHECK(err_code);
  haptics_standby();
  haptics_disable();
}

void haptics_init(void) {
  Haptics_Init();
  Haptics_SetActuator(ACTUATOR_LRA);
  Haptics_RunAutoCal_LRA();
  uint8_t ret;
  ret = Haptics_Diagnostics(ACTUATOR_LRA);
  UNUSED_VARIABLE(ret);
	uint32_t err_code;
  err_code = app_timer_create(&haptics_timer_id, APP_TIMER_MODE_SINGLE_SHOT,
                              haptics_timeout_handler);
  APP_ERROR_CHECK(err_code);
}

void haptics_active(void) { Haptics_Active(); }
void haptics_standby(void) { Haptics_Standby(); }
void haptics_enable(void) { Haptics_EnableAmplifier(); }
void haptics_disable(void) { Haptics_DisableAmplifier(); }

unsigned char haptics_test_cal_diags(void) {
  unsigned char ret;
  Haptics_RunAutoCal_LRA();

  ret = Haptics_Diagnostics(ACTUATOR_LRA);
  // ret = (ret & 0x8) >> 3;  //And to only get at one specific bit, comment out
  // to see all bits for diagnosis
  return ret;
}

unsigned char haptics_test_run1(void) {
#ifdef PMIC
  bool battery_is_charging = pmic_is_charging();
  if (battery_is_charging) {
    return 0; //  Don't activate the motor if the battery  charging
  }
#endif
  Haptics_SendWaveform(Tick, ACTUATOR_LRA, TRIGGER_INTERNAL);
	uint32_t err_code = app_timer_start(haptics_timer_id, TICK_TIMEOUT, NULL);
  APP_ERROR_CHECK(err_code);
  return 0; // ret;
}

unsigned char haptics_test_run2(void) {
#ifdef PMIC
  bool battery_is_charging = pmic_is_charging();
  if (battery_is_charging) {

    return 0; //  Don't activate the motor if the battery charging
  }
#endif
  Haptics_SendWaveform(BuzzAlert, ACTUATOR_LRA, TRIGGER_INTERNAL);
  uint32_t err_code = app_timer_start(haptics_timer_id, BUZZALERT_TIMEOUT, NULL);
  APP_ERROR_CHECK(err_code);
	return 0; // ret;
}

unsigned char haptics_test_run3(void) {
// unsigned char ret;
// uint32_t err;
#ifdef PMIC
  bool battery_is_charging = pmic_is_charging();
  if (battery_is_charging) {
    return 0; //  Don't activate the motor if the battery charging
  }
#endif
  Haptics_SendWaveform(DoubleStrongClick, ACTUATOR_LRA, TRIGGER_INTERNAL);
  uint32_t err_code = app_timer_start(haptics_timer_id, DOUBLESTRONGCLICK_TIMEOUT, NULL);
  APP_ERROR_CHECK(err_code);
	return 0; // ret;
}

unsigned char haptics_test_run4(void) {
#ifdef PMIC
  // unsigned char ret;
  // uint32_t err;
  bool battery_is_charging = pmic_is_charging();
  if (battery_is_charging) {
    return 0; //  Don't activate the motor if the battery charging
  }
#endif
  Haptics_SendWaveform(PulsingAlert, ACTUATOR_LRA, TRIGGER_INTERNAL);
  uint32_t err_code = app_timer_start(haptics_timer_id, PULSINGALERT_TIMEOUT, NULL);
  APP_ERROR_CHECK(err_code);
	return 0;
}

unsigned char haptics_msg_extra_short(void) {
#ifdef PMIC
  bool battery_is_charging = pmic_is_charging();
  if (battery_is_charging) {
    return 0; //  Don't activate the motor if the battery charging
  }
#endif
  Haptics_SendWaveform(MsgExtraShort, ACTUATOR_LRA, TRIGGER_INTERNAL);
  uint32_t err_code = app_timer_start(haptics_timer_id, MSGEXTRASHORT_TIMEOUT, NULL);
  APP_ERROR_CHECK(err_code);
  return 0;
}

unsigned char haptics_msg_short(void) {
#ifdef PMIC
  bool battery_is_charging = pmic_is_charging();
  if (battery_is_charging) {
    return 0; //  Don't activate the motor if the battery charging
  }
#endif
  Haptics_SendWaveform(MsgShort, ACTUATOR_LRA, TRIGGER_INTERNAL);
  uint32_t err_code = app_timer_start(haptics_timer_id, MSGSHORT_TIMEOUT, NULL);
  APP_ERROR_CHECK(err_code);
  return 0;
}

unsigned char haptics_msg_medium(void) {
#ifdef PMIC
  bool battery_is_charging = pmic_is_charging();
  if (battery_is_charging) {
    return 0; //  Don't activate the motor if the battery charging
  }
#endif
  Haptics_SendWaveform(MsgMedium, ACTUATOR_LRA, TRIGGER_INTERNAL);
  uint32_t err_code = app_timer_start(haptics_timer_id, MSGMEDIUM_TIMEOUT, NULL);
  APP_ERROR_CHECK(err_code);
  return 0;
}

unsigned char haptics_msg_long(void) {
#ifdef PMIC
  bool battery_is_charging = pmic_is_charging();
  if (battery_is_charging) {
    return 0; //  Don't activate the motor if the battery charging
  }
#endif
  Haptics_SendWaveform(MsgLong, ACTUATOR_LRA, TRIGGER_INTERNAL);
  uint32_t err_code = app_timer_start(haptics_timer_id, MSGLONG_TIMEOUT, NULL);
  APP_ERROR_CHECK(err_code);
  return 0;
}

unsigned char haptics_msg_extra_long(void) {
#ifdef PMIC
  bool battery_is_charging = pmic_is_charging();
  if (battery_is_charging) {
    return 0; //  Don't activate the motor if the battery charging
  }
#endif
  Haptics_SendWaveform(MsgExtraLong, ACTUATOR_LRA, TRIGGER_INTERNAL);
  uint32_t err_code = app_timer_start(haptics_timer_id, MSGEXTRALONG_TIMEOUT, NULL);
  APP_ERROR_CHECK(err_code);
  return 0;
}

unsigned char haptics_msg_really_long(void) {
#ifdef PMIC
  bool battery_is_charging = pmic_is_charging();
  if (battery_is_charging) {
    return 0; //  Don't activate the motor if the battery charging
  }
#endif
  Haptics_SendWaveform(MsgReallyLong, ACTUATOR_LRA, TRIGGER_INTERNAL);
  uint32_t err_code = app_timer_start(haptics_timer_id, MSGREALLYLONG_TIMEOUT, NULL);
  APP_ERROR_CHECK(err_code);
  return 0;
}



#ifdef __cplusplus
}
#endif // __cplusplus
