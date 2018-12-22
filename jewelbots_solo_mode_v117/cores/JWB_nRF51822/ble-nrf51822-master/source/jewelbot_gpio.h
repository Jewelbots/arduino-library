#ifndef __jewelbot_gpio_h__
#define __jewelbot_gpio_h__

#include "nrf_gpio.h"

#define BUTTON_PULL NRF_GPIO_PIN_PULLUP

/**********************************************************************************************
                                   REAL JEWELBOT
 *********************************************************************************************/
#ifdef REAL_JEWELBOT
// real jewelbot GPIOs go here

// these have no meaning on the jewelbot ... assigning them unused GPIOs on that
// for now

#define WAKEUP_BUTTON_PIN 26 /**< Button used to wake up the application. */
#define ADVERTISING_LED_PIN_NO 28 /**< Is on when device is advertising. */
#define CONNECTED_LED_PIN_NO 29   /**< Is on when device has connected. */
#define BUTTON_PIN (WAKEUP_BUTTON_PIN)

#define I2C_SCL 12u
#define I2C_SDA 11u
#define HAPTIC_I2C_ADDR                                                        \
  (0x5A) // 0b1011 010x  // 0x5A << 1 //no need to do this in SDK10
#define PMIC_I2C_ADDR (0x48) // 0b1001 000x //0x48 << 1   // same
#define PMIC_I2C_ADDRESS PMIC_I2C_ADDR
#define PMIC_INTERRUPT_PIN 27
#define DRV2604_I2C_ADDRESS HAPTIC_I2C_ADDR
#define DRV2604_ENABLE_PIN (8)
#define DRV2604_TRIGGER_PIN (9)

// UART over USB
#define RX_PIN_NUMBER 24
#define TX_PIN_NUMBER 21
#define RTS_PIN_NUMBER 22 // disconnected pin
#define CTS_PIN_NUMBER 23 // disconnected pin

// SPI for LEDs
#define SPIM0_MISO_PIN 0u // MISO -> SO / Nordic SDI
#define SPIM0_MOSI_PIN 1u // MOSI -> SI/A0 / Nordic SDO
#define SPIM0_SS_PIN 2u   // slave CS -> SS/SDA
#define LED_CS SPIM0_SS_PIN
#define SPIM0_SCK_PIN 3u // SPI clock GPIO -> SCL
#define LED_RST 5u       // -> NRST

/**********************************************************************************************
                                        DEVKIT
 *********************************************************************************************/
#else
// devkit GPIOS go here

#define BUTTON_0 17
#define BUTTON_1 18
#define BUTTON_2 19
#define BUTTON_3 20
#define LED_0 21
#define LED_1 22
#define LED_2 23
#define LED_3 24

// these have no meaning on the jewelbot ... assigning them unused GPIOs on that
// for now
#define WAKEUP_BUTTON_PIN                                                      \
  BUTTON_0 /**< Button used to wake up the application. */
#define ADVERTISING_LED_PIN_NO LED_0 /**< Is on when device is advertising. */
#define CONNECTED_LED_PIN_NO LED_1   /**< Is on when device has connected. */
#define BUTTON_PIN WAKEUP_BUTTON_PIN
#define I2C_SCL 28
#define I2C_SDA 29
#define HAPTIC_I2C_ADDR (0x5a << 1) // 0b1011 010x
#define PMIC_I2C_ADDR (0x48 << 1)   // 0b1001 000x
#define DRV2604_I2C_ADDRESS HAPTIC_I2C_ADDR
#define DRV2604_ENABLE_PIN (8)
#define DRV2604_TRIGGER_PIN (9)
#define PMIC_I2C_ADDRESS PMIC_I2C_ADDR

// UART over USB
#define RX_PIN_NUMBER 11
#define TX_PIN_NUMBER 9
#define CTS_PIN_NUMBER 10 // only used on
#define RTS_PIN_NUMBER 8  // dev board

// SPI for LEDs
#define SPIM0_MISO_PIN 0u // MISO -> SO / Nordic SDI
#define SPIM0_MOSI_PIN 1u // MOSI -> SI/A0 / Nordic SDO
#define SPIM0_SS_PIN 2u   // slave CS -> SS/SDA
#define LED_CS SPIM0_SS_PIN
#define SPIM0_SCK_PIN 3u // SPI clock GPIO -> SCL
#define LED_RST 5u       // -> NRST
#define PMIC_INTERRUPT_PIN 27

#endif // #ifdef REAL_JEWELBOT

#endif // inclusion guard
