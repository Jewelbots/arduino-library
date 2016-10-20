#include "pmic_driver.h"
#include "app_error.h"
#include "app_util_platform.h"
#include "jewelbot_gpio.h"
#include "nrf_drv_twi.h"
#include <stdbool.h>
#include <stdint.h>

extern nrf_drv_twi_t app_twi_instance;

static volatile bool m_xfer_done = true;
/* Indicates if setting mode operation has ended. */
static volatile bool m_set_mode_done;

static bool pmic_driver_read_reg(uint8_t reg, uint8_t *data) {
  uint32_t err_code;
  err_code = nrf_drv_twi_tx(&app_twi_instance, PMIC_I2C_ADDRESS, &reg, 1,
                            true); // was TWI_DONT_ISSUE_STOP
  if (err_code != NRF_SUCCESS) {
    NRF_LOG_PRINTF(
        "PMIC nrf_drv_twi_tx failed for read register: %u, Error code: %u\r\n",
        reg, err_code);
    return false;
  }
  err_code = nrf_drv_twi_rx(&app_twi_instance, PMIC_I2C_ADDRESS, data, 1);
  if (err_code != NRF_SUCCESS) {
    NRF_LOG_PRINTF(
        "PMIC nrf_drv_twi_rx failed for read register: %u, Error code: %u\r\n",
        reg, err_code);
  }
  return (err_code == NRF_SUCCESS);
}

static bool pmic_driver_write_reg(uint8_t reg, uint8_t data) {
  uint8_t tmp_data[2] = {reg, data};
  uint32_t err_code =
      nrf_drv_twi_tx(&app_twi_instance, PMIC_I2C_ADDRESS, tmp_data, 2, false);
  if (err_code != NRF_SUCCESS) {
    NRF_LOG_PRINTF(
        "PMIC nrf_drv_twi_tx failed for write register: %u, Error code: %u\r\n",
        reg, err_code);
  }
  return err_code == NRF_SUCCESS;
}

void pmic_init() {
  // pmic_turn_off_charging();
  bool success;
  uint8_t val = 0;
  /*success = pmic_driver_read_reg(PMIC_REG_DEFDCDC1, &val);
  if(!success) {
          SEGGER_RTT_WriteString(0,"Error reading DCDC1 configuration!\n");
          return;
  } */

  // To do - make nice defines for all these
  // Configure CHGCONFIG0 register
  // set: 0x40 = Vsys to 4.4V | 0x20 = 500 mA input current, input DPPM disabled
  // | 0xF = default settings
  // Settings valid for both PMIC versions
  val = (0x40) | (0x20) | (0x0F);
  success = pmic_driver_write_reg(PMIC_REG_CHGCONFIG0, val);
  if (!success) {
    NRF_LOG("Error writing CHGCONFIG0 configuration!\r\n");
    return;
  }

// Configure CHGCONFIG1 register
// set:  0x40 = default precharge current | 0x10 = default current scaling |
// 0x4 = default current termination
#ifdef PMICV1 // old xx20 PMIC
  val = (0x40) | (0x00) | (0x04);
#else // new xx21 PMIC
  val = (0x40) | (0x10) | (0x04);
#endif
  success = pmic_driver_write_reg(PMIC_REG_CHGCONFIG1, val);
  if (!success) {
    NRF_LOG("Error writing CHGCONFIG1 configuration!\r\n");
    return;
  }

  // Configure CHGCONFIG2 register
  // set:  0x40 = default safety timer | 0x08 = default sensor resistance | 0x04
  // = default - DPPM @ 4.3V
  // Settings valid for both PMIC versions
  val = (0x40) | (0x08) | (0x04);
  success = pmic_driver_write_reg(PMIC_REG_CHGCONFIG2, val);
  if (!success) {
    NRF_LOG("Error writing CHGCONFIG2 configuration!\r\n");
    return;
  }

  // Configure CHGCONFIG3 register
  // set:  0x40 = default charging voltage | 0x01 = disable battery comp (not
  // used for rechargeable battery)
  // Settings valid for both PMIC versions
  val = (0x40) | (0x01);
  success = pmic_driver_write_reg(PMIC_REG_CHGCONFIG3, val);
  if (!success) {
    NRF_LOG("Error writing CHGCONFIG3 configuration!\r\n");
    return;
  }

/* Configure DEFDCDC1 register*/
// app_trace_log("DCDC 1 Before Write %02x\n", val);
// add voltage mask to explicitly set register to correct, default values
#ifdef PMICV1 // old xx20 PMIC
  val = PMIC_DCDC1_ENABLE_MASK | PMIC_DCDC1_VOLT_MASK_V1;
#else
  val = PMIC_DCDC1_ENABLE_MASK | PMIC_DCDC1_VOLT_MASK_V2;
#endif
  success = pmic_driver_write_reg(PMIC_REG_DEFDCDC1, val);
  if (!success) {
    NRF_LOG("Error writing DCDC1 configuration!\r\n");
    return;
  }

/*Configure LDO_CTRL register */
#ifndef PMICV1
  val =
      PMIC_LDO1_ENABLE_MASK | PMIC_LDO1_ENABLE_DISCHARGE | PMIC_LDO1_VOLT_MASK;
  success = pmic_driver_write_reg(PMIC_REG_LDOCNTRL, val);
  if (!success) {
    NRF_LOG(0, "Error writing LDO1 configuration!\r\n");
    return;
  }
#endif

  val = ~(PMIC_CH_PGOOD |
          PMIC_CH_ACTIVE); // enable interrupts for CH_PGOOD and CH_ACTIVE
  success = pmic_driver_write_reg(PMIC_REG_IRMASK0, val);
  if (!success) {
    NRF_LOG(0, "Error writing PMIC_REG_IRMASK0 configuration!\r\n");
    return;
  }

  pmic_clear_interrupts();
}

void pmic_clear_interrupts(void) {
  bool success;
  uint8_t val;
  do {
    success = pmic_driver_read_reg(PMIC_REG_IR0, &val);
  } while (success && val != 0);
}

void pmic_disable() {
  bool success;
  uint8_t val;
  success = pmic_driver_read_reg(PMIC_REG_DEFDCDC1, &val);
  if (!success) {
    NRF_LOG("Error reading DCDC1 configuration!\r\n");
    return;
  }

  /* Enable DCDC1 */
  // app_trace_log("DCDC 1 Before Write %02x\n", val);
  val &= !PMIC_DCDC1_ENABLE_MASK;
  success = pmic_driver_write_reg(PMIC_REG_DEFDCDC1, val);
  if (!success) {
    NRF_LOG("Error writing DCDC1 configuration!\r\n");
    return;
  }
}

bool pmic_is_charging(void) {
#ifdef SIMULATE_CHARGING
  return true;
#elif SIMULATE_BATTERY
  return false;
#endif
  uint8_t success;
  uint8_t val;
  success = pmic_driver_read_reg(PMIC_REG_CHGSTATUS, &val);
  if (!success) {
    NRF_LOG("Error reading CHGSTATUS register!\r\n");
    return false;
  }
  if (val & PMIC_CHARGING_MASK) {
    return true;
  }
  return false;
}

bool pmic_5V_present(void) {
#ifdef SIMULATE_CHARGING
  return true;
#elif SIMULATE_BATTERY
  return false;
#endif

  uint8_t success;
  uint8_t val;
  success = pmic_driver_read_reg(PMIC_REG_CHGSTATUS, &val);
  if (!success) {
    NRF_LOG("Error reading CHGSTATUS register!\r\n");
    return false;
  }
  if (val & PMIC_5V_PRESENT_MASK) {
    return true;
  }
  return false;
}

bool pmic_toggle_charging() {
  uint8_t success;
  uint8_t val = 0;

  NRF_LOG("Toggle Charging\n");
  success = pmic_driver_read_reg(PMIC_REG_CHGCONFIG0, &val);
  NRF_LOG_PRINTF("CHGCONFIG0 val after read is: %02x\r\n", val);
  if (!success) {
    NRF_LOG("failed to read CHGCONFIG0 register!\r\n");
    return false;
  }
  val = val ^ (1 << 0);
  NRF_LOG_PRINTF("CHGCONFIG0 val after change is: %02x\r\n", val);
  success = pmic_driver_write_reg(PMIC_REG_CHGCONFIG0, val);
  if (!success) {
    NRF_LOG("failed to write CHGCONFIG0 register!\r\n");
    return false;
  }
  return true;

  // success = pmic_driver_write_reg(PMIC_REG_CHGCONFIG0,
}
bool pmic_turn_off_charging() {
  uint8_t success;
  uint8_t val = 0;

  NRF_LOG("Toggle Charging\n");
  success = pmic_driver_read_reg(PMIC_REG_CHGCONFIG0, &val);
  NRF_LOG_PRINTF("CHGCONFIG0 val after read is: %02x\r\n", val);
  if (!success) {
    NRF_LOG("failed to read CHGCONFIG0 register!\r\n");
    return false;
  }
  val &= ~(1 << 0);

  NRF_LOG_PRINTF("CHGCONFIG0 val after change is: %02x\r\n", val);
  success = pmic_driver_write_reg(PMIC_REG_CHGCONFIG0, val);
  if (!success) {
    NRF_LOG("failed to write CHGCONFIG0 register!\r\n");
    return false;
  }
  return true;
}

bool pmic_turn_on_charging() {
  uint8_t success;
  uint8_t val = 0;

  NRF_LOG("Toggle Charging\n");
  success = pmic_driver_read_reg(PMIC_REG_CHGCONFIG0, &val);
  NRF_LOG_PRINTF("CHGCONFIG0 val after read is: %02x\r\n", val);
  if (!success) {
    NRF_LOG("failed to read CHGCONFIG0 register!\r\n");
    return false;
  }
  val |= 1 << 0;
  NRF_LOG_PRINTF("CHGCONFIG0 val after change is: %02x\r\n", val);
  success = pmic_driver_write_reg(PMIC_REG_CHGCONFIG0, val);
  if (!success) {
    NRF_LOG("failed to write CHGCONFIG0 register!\r\n");
    return false;
  }
  return true;
}

bool pmic_enable_dynamic_ppm(void) {
  // 00100000 //default       0x20
  // 00010000 //enabled       0x10
  uint8_t success;
  uint8_t val = 0;
  NRF_LOG("Turn on Dynamic PPM\n");
  success = pmic_driver_read_reg(PMIC_REG_CHGCONFIG0, &val);
  NRF_LOG_PRINTF("CHGCONFIG0 val after read is: %02x\r\n", val);
  if (!success) {
    NRF_LOG("failed to read CHGCONFIG0 register!\r\n");
    return false;
  }
  val &= ~((1 << 4) | (1 << 5));
  val |= PMIC_DPPM_ENABLED;
  NRF_LOG_PRINTF("CHGCONFIG0 val after change is: %02x\r\n", val);
  success = pmic_driver_write_reg(PMIC_REG_CHGCONFIG0, val);
  if (!success) {
    NRF_LOG("failed to write CHGCONFIG0 register!\r\n");
    return false;
  }
  return true;
}

bool pmic_disable_dynamic_ppm(void) {
  // 00100000 //default       0x20
  // 00010000 //enabled       0x10
  uint8_t success;
  uint8_t val = 0;
  NRF_LOG("Turn off Dynamic PPM\n");
  success = pmic_driver_read_reg(PMIC_REG_CHGCONFIG0, &val);
  NRF_LOG_PRINTF("CHGCONFIG0 val after read is: %02x\r\n", val);
  if (!success) {
    NRF_LOG("failed to read CHGCONFIG0 register!\r\n");
    return false;
  }
  val &= ~((1 << 4) | (1 << 5)); // reset bits; then disable
  val |= PMIC_DPPM_DISABLED;
  NRF_LOG_PRINTF("CHGCONFIG0 val after change is: %02x\r\n", val);
  success = pmic_driver_write_reg(PMIC_REG_CHGCONFIG0, val);
  if (!success) {
    NRF_LOG("failed to write CHGCONFIG0 register!\r\n");
    return false;
  }
  return true;
}
