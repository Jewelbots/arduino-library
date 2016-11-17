#ifndef _HAPTICS_DRIVER_H_
#define _HAPTICS_DRIVER_H_

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus
void haptics_init(void);
void haptics_active(void);
void haptics_standby(void);
void haptics_enable(void);
void haptics_disable(void);
unsigned char haptics_test_cal_diags(void);
unsigned char haptics_test_run1(void);
unsigned char haptics_test_run2(void);
unsigned char haptics_test_run3(void);
unsigned char haptics_test_run4(void);
unsigned char haptics_msg_short(void);
unsigned char haptics_msg_long(void);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif
