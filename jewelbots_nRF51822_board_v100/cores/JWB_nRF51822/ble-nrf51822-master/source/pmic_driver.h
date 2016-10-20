#ifndef _PMIC_DRIVER_H_
#define _PMIC_DRIVER_H_
#include "jewelbot_gpio.h"
#include "nrf_drv_twi.h"
#include <stdbool.h>
#include <stdint.h>

#define PMIC_REG_CHGSTATUS (0x01)
#define PMIC_REG_CHGCONFIG0 (0x02)
#define PMIC_REG_CHGCONFIG1 (0x03)
#define PMIC_REG_CHGCONFIG2 (0x04)
#define PMIC_REG_CHGCONFIG3 (0x05)
#define PMIC_REG_DEFDCDC1 (0x07)
#define PMIC_REG_LDOCNTRL (0x08)

#define PMIC_CONTROL1_REG (0x0A)
#define PMIC_REG_IR0 (0x10)
#define PMIC_REG_IR1 (0x11)
#define PMIC_REG_IR2 (0x12)
#define PMIC_REG_IRMASK0 (0x0D)
#define PMIC_REG_IRMASK2 (0x0F)

#define PMIC_PB_STAT_MASK (0x02)
#define PMIC_5V_PRESENT_MASK (0x04)
#define PMIC_CHARGING_MASK (0x08)
#define PMIC_DCDC1_ENABLE_MASK (0x80)
#define PMIC_DCDC1_VOLT_MASK_V1 (0x24)
#define PMIC_DCDC1_VOLT_MASK_V2 (0x3F)
#define PMIC_LDO1_ENABLE_MASK (0x80)
#define PMIC_LDO1_ENABLE_DISCHARGE (0x40)
#define PMIC_LDO1_VOLT_MASK (0x25)
#define PMIC_CH_EN_MASK (0x00)

#define PMIC_CH_PGOOD (0x04)
#define PMIC_CH_ACTIVE (0x08)
#define PMIC_PB_STAT (0x10)
#define PMIC_DPPM_ENABLED (0x10)
#define PMIC_DPPM_DISABLED (0x20)

void pmic_init(void);
void pmic_disable(void);
bool pmic_is_charging(void);
bool pmic_5V_present(void);
void pmic_clear_interrupts(void);
bool pmic_toggle_charging(void);
bool pmic_turn_on_charging(void);
bool pmic_turn_off_charging(void);
bool pmic_enable_dynamic_ppm(void);
bool pmic_disable_dynamic_ppm(void);

#endif
