
#include "nrf_drv_twi.h"
#include "nrf_gpio.h"
#include "nrf_drv_uart.h"
#include "nrf_drv_config.h"
#include "nrf_drv_timer.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_drv_clock.h"
#include "nrf_drv_spi.h"
#include "nrf51_bitfields.h"
#include "nrf_drv_twi.h"
#include "nrf_drv_wdt.h"
#include "nrf_gpio.h"
#include "app_util_platform.h"
#include "jewelbot_gpio.h"

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

nrf_drv_twi_t app_twi_instance = NRF_DRV_TWI_INSTANCE(1);

void twi_init() {
  uint32_t err_code;
  const nrf_drv_twi_config_t twi_config = {
      .scl = I2C_SCL,
      .sda = I2C_SDA,
      .frequency = NRF_TWI_FREQ_100K, // was 100K
      .interrupt_priority = APP_IRQ_PRIORITY_HIGH};

  err_code = nrf_drv_twi_init(&app_twi_instance, &twi_config, NULL,
                              NULL); // todo add event handler
  APP_ERROR_CHECK(err_code);

  nrf_drv_twi_enable(&app_twi_instance);
}

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
