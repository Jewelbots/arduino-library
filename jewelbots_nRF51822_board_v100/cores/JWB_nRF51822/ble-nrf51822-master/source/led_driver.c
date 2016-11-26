#include "app_error.h"
#include "app_util_platform.h"
#include "jewelbot_gpio.h"
#include "nordic_common.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#ifndef __LED_DRIVER_H__// TODO: Rename to what this actually is:
                                // led_driver
#include "led_driver.h"
#endif
#include "common_defines.h"
#include "led_sequence.h"
#include "nrf_drv_spi.h"
#include "nrf_log.h"
#include "pmic_driver.h"

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus
#define TX_RX_BUF_LENGTH 100
#define LSBFIRST true
#define MSBFIRST false
static uint8_t
    m_tx_data[TX_RX_BUF_LENGTH]; /**< A buffer with data to transfer. */
static uint8_t m_rx_data[TX_RX_BUF_LENGTH]; /**< A buffer for incoming data. */
/**< SPI transaction buffer length. */
// static spi_master_ex_state_t m_spi_master_ex_state =
// (spi_master_ex_state_t)0;
static const nrf_drv_spi_t m_spi_master_0 = NRF_DRV_SPI_INSTANCE(0);
// #if defined(SPI_MASTER_0_ENABLE)
//     #define SPI_MASTER_HW SPI_MASTER_0
// #elif defined(SPI_MASTER_1_ENABLE)
//     #define SPI_MASTER_HW SPI_MASTER_1
// #else
//     #error "No SPI enabled"
// #endif

// Data buffers.

static volatile bool m_transfer_completed =
    true; /**< A flag to inform about completed transfer. */

/** @brief Function for initializing a SPI master driver.
 */
static uint32_t spi_master_init(nrf_drv_spi_t const *p_instance, bool lsb) {
  // init_buf(m_tx_data, m_rx_data, len);
  uint32_t err_code = NRF_SUCCESS;

  nrf_drv_spi_config_t config = {
      .ss_pin = SPIM0_SS_PIN,
      .irq_priority = APP_IRQ_PRIORITY_LOW,
      .orc = 0xCC,
      .frequency = NRF_DRV_SPI_FREQ_1M,
      .mode = NRF_DRV_SPI_MODE_3,
      .bit_order = (lsb ? NRF_DRV_SPI_BIT_ORDER_LSB_FIRST
                        : NRF_DRV_SPI_BIT_ORDER_MSB_FIRST),
  };

  config.sck_pin = SPIM0_SCK_PIN;
  config.mosi_pin = SPIM0_MOSI_PIN;
  config.miso_pin = SPIM0_MISO_PIN;
  err_code = nrf_drv_spi_init(p_instance, &config, NULL);

  APP_ERROR_CHECK(err_code);

  // #if defined(SPI_MASTER_0_ENABLE)
  // spi_config.SPI_Pin_SCK  = SPIM0_SCK_PIN;
  // spi_config.SPI_Pin_MISO = SPIM0_MISO_PIN;
  // spi_config.SPI_Pin_MOSI = SPIM0_MOSI_PIN;
  // spi_config.SPI_Pin_SS   = SPIM0_SS_PIN;
  // #elif defined(SPI_MASTER_1_ENABLE)
  // spi_config.SPI_Pin_SCK  = SPIM1_SCK_PIN;
  // spi_config.SPI_Pin_MISO = SPIM1_MISO_PIN;
  // spi_config.SPI_Pin_MOSI = SPIM1_MOSI_PIN;
  // spi_config.SPI_Pin_SS   = SPIM1_SS_PIN;
  // #endif /* SPI_MASTER_ENABLE */

  // spi_config.SPI_Pin_SCK  = SPIM0_SCK_PIN;
  // spi_config.SPI_Pin_MISO = SPIM0_MISO_PIN;
  // spi_config.SPI_Pin_MOSI = SPIM0_MOSI_PIN;
  // spi_config.SPI_Pin_SS   = SPIM0_SS_PIN;
  // spi_config.SPI_CONFIG_ORDER = SPI_CONFIG_ORDER_MsbFirst;
  // spi_config.SPI_CONFIG_CPOL = SPI_CONFIG_CPOL_ActiveHigh; //ACTIVEHIGH
  // /**< Serial clock polarity ACTIVEHIGH or ACTIVELOW. */
  // spi_config.SPI_CONFIG_CPHA = SPI_CONFIG_CPHA_Leading; //TRAILING
  // spi_config.SPI_Freq = SPI_FREQUENCY_FREQUENCY_M1;
  //
  // spi_master_evt_handler_reg(SPI_MASTER_HW, spi_master_event_handler);

  return err_code;
}

static void spi_send_recv(uint8_t *p_tx_data, uint8_t *p_rx_data,
                          uint16_t len) {
  uint32_t err_code =
      nrf_drv_spi_transfer(&m_spi_master_0, p_tx_data, len, p_rx_data, len);
  APP_ERROR_CHECK(err_code);
}

typedef struct {
  uint8_t r1;
  uint8_t g1;
  uint8_t b1;
  uint8_t r2;
  uint8_t g2;
  uint8_t b2;
  uint8_t r3;
  uint8_t g3;
  uint8_t b3;
  uint8_t r4;
  uint8_t g4;
  uint8_t b4;
  uint8_t aled;
  uint8_t ctrl1;
  uint8_t ctrl2;
  uint8_t boost;
  uint8_t freqs;
  uint8_t enables;
  uint8_t led_test;
  uint8_t adc_output;
  uint8_t SPI_ADDR_RW;
} lp55281_reg_t;

// set below in the init routine to overlap m_tx_data_spi
static volatile lp55281_reg_t *led_driver;

time_t jwbs_tod;

void spi_led_init(void) {

  // uint32_t err_code = NRF_SUCCESS;
  led_driver = (lp55281_reg_t *)m_tx_data;
  memset(m_rx_data, 0, sizeof(lp55281_reg_t));
  memset(m_tx_data, 0, sizeof(lp55281_reg_t));
  led_driver->aled = 0x01;
  led_driver->enables = 0xCF;
  led_driver->boost = 0x3f;
  led_driver->freqs = 0x07;

  // turn on SPI0, set to SPI_MODE3, and set lsb_first to false
  // err_code = spi_master_init(&m_spi_master_0, MSBFIRST); //SPI0, SPI_MODE3,
  // (bool)0

  // APP_ERROR_CHECK(err_code);
  // issue reset to lp55281
  m_tx_data[0] = 0xC1;
  m_tx_data[1] = 0x00;
  spi_send_recv(m_tx_data, m_rx_data, 2);
  // set Boost ( 0x3F is default = 5V)
  m_tx_data[0] = 0x1F;
  m_tx_data[1] = led_driver->boost; // 0x3F;
  spi_send_recv(m_tx_data, m_rx_data, 2);
  // set Boost Frequency (0x07 is default = 2 MHz)
  m_tx_data[0] = 0x21;
  m_tx_data[1] = led_driver->freqs; // 0x07;
  spi_send_recv(m_tx_data, m_rx_data, 2);
}

// LP55281 LED driver interface
//  Based on data passed in assumed this handler exists to set one LED to the
//  given color
void set_led_state_handler(led_cmd_t *state) {
  // turn on SPI0, set to SPI_MODE3, and set lsb_first to false
  //  enable_led();
  // clear_led();

  /*
   real LEDs are driven by an SPI driver chip
   on devkit, drives GPIOs for debug
   */
  state->timestamp = HTONL(state->timestamp);

  // Set the desired LED to the rgb values
  // LED 2 and 3 reversed to make the LEDs go in a circle
  switch (state->led) {
  case 0:
    m_tx_data[0] = (0x00 << 1) | 0x01;
    m_tx_data[1] = state->r;
    m_tx_data[2] = (0x01 << 1) | 0x01;
    m_tx_data[3] = state->g;
    m_tx_data[4] = (0x02 << 1) | 0x01;
    m_tx_data[5] = state->b;

    for (int i = 0; i < 3; i++) {
      spi_send_recv((m_tx_data + 2 * i), m_rx_data, 2);
    }
    break;

  case 1:
    m_tx_data[0] = (0x03 << 1) | 0x01;
    m_tx_data[1] = state->r;
    m_tx_data[2] = (0x04 << 1) | 0x01;
    m_tx_data[3] = state->g;
    m_tx_data[4] = (0x05 << 1) | 0x01;
    m_tx_data[5] = state->b;

    for (int i = 0; i < 3; i++) {
      spi_send_recv((m_tx_data + 2 * i), m_rx_data, 2);
    }

    break;

  case 3:
    m_tx_data[0] = (0x06 << 1) | 0x01;
    m_tx_data[1] = state->r;
    m_tx_data[2] = (0x07 << 1) | 0x01;
    m_tx_data[3] = state->g;
    m_tx_data[4] = (0x08 << 1) | 0x01;
    m_tx_data[5] = state->b;

    for (int i = 0; i < 3; i++) {
      spi_send_recv((m_tx_data + 2 * i), m_rx_data, 2);
    }

    break;

  case 2:
    m_tx_data[0] = (0x09 << 1) | 0x01;
    m_tx_data[1] = state->r;
    m_tx_data[2] = (0x0A << 1) | 0x01;
    m_tx_data[3] = state->g;
    m_tx_data[4] = (0x0B << 1) | 0x01;
    m_tx_data[5] = state->b;
    for (int i = 0; i < 3; i++) {
      spi_send_recv((m_tx_data + 2 * i), m_rx_data, 2);
    }

    break;

  default:
    break;
  };
}

void led_test_run() { boot_up_led_sequence(); }
void enable_led() {

  // Enable LEDs
  m_tx_data[0] = 0x23;
  m_tx_data[1] = 0xCF;
  spi_send_recv(m_tx_data, m_rx_data, 2);
}
void disable_led() {
  if (!(pmic_5V_present() || pmic_is_charging())) {
    // uint32_t *spi_base_address = spi_master_init(SPI0, SPI_MODE3, (bool)0);
    // End of LED sequence to disable the LED chip to conserve power
    // Turn off LEDs
    m_tx_data[0] = 0x23;
    m_tx_data[1] = 0x00;
    spi_send_recv(m_tx_data, m_rx_data, 2);
  }
}
/**@brief Function for initializing services that will be used by the
 * application.
 */
void services_init(void) {
  ret_code_t err_code = spi_master_init(&m_spi_master_0, MSBFIRST);
  APP_ERROR_CHECK(err_code);
  spi_led_init();
}

extern uint8_t* color_lookup(char *color)
{
  uint8_t *color_values;
  if(color == "red") {
    color_values[0] = 0x3F;
    color_values[1] = 0x00;
    color_values[2] = 0x00;
  }
  else if (color == "orange"){
    color_values[0] = 0x3F;
    color_values[1] = 0x1C;
    color_values[2] = 0x00;

  }
  else if (color == "yellow"){
    color_values[0] = 0x3F;
    color_values[1] = 0x3F;
    color_values[2] = 0x00;

  }
  else if (color == "green"){
    color_values[0] = 0x00;
    color_values[1] = 0x3F;
    color_values[2] = 0x00;
  }
  else if (color == "blue"){
    color_values[0] = 0x00;
    color_values[1] = 0x00;
    color_values[2] = 0x3F;
  }
  else if (color == "violet"){
    color_values[0] = 0x1F;
    color_values[1] = 0x00;
    color_values[2] = 0x3F;
   }
  else if (color == "pink"){
    color_values[0] = 0x3F;
    color_values[1] = 0x1A;
    color_values[2] = 0x3A;
    }
  else if (color == "magenta"){
    color_values[0] = 0x3F;
    color_values[1] = 0x0C;
    color_values[2] = 0x3F;
    }
    return color_values;
}
#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
