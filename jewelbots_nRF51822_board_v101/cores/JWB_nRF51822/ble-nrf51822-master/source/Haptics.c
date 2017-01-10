#include "Haptics.h"
#include "DRV2605.h"
#include "app_error.h"
#include "app_util_platform.h"
#include "jewelbot_gpio.h"
#include "nrf_delay.h"
#include "nrf_drv_twi.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "jwb_twi.h"

#if DRV2604
#include "WaveformData.h"
#endif

// Private variables
static uint16_t tickdelay;
static uint8_t playEffect = 1;                 // If 1 = play, 0 = do not play
static uint8_t EffectActive = 0;               // Flag if effect is playing
static uint8_t triggerType = TRIGGER_INTERNAL; // Trigger Setting
static uint8_t actuatorType = ACTUATOR_LRA;    // Actuator Setting
static uint8_t disableAllowed = 1; // Flag if hardware disable is allowed
static uint8_t IsERMOpenLoop = ERM_ClosedLoop;    // Flag for ERM Open Loop
static uint8_t IsLRAOpenLoop = LRA_AutoResonance; // Flag for ERM Open Loop

/* DRV260x Register Variables */
static uint8_t LRA_AutoCal_Comp, LRA_AutoCal_BEMF, LRA_AutoCal_FB;
static uint8_t LRA_RatedVoltage, LRA_ODClamp;
static uint8_t AutoCal_Result;
static uint8_t control1, control2, control3;
static uint8_t ERMDefaultOpenLoopSetting, LRADefaultOpenLoopSetting;
uint16_t RDS_Comp_temp;

extern nrf_drv_twi_t app_twi_instance;

static bool I2C_WriteSingleByte(uint8_t regstr, uint8_t data) {
  uint8_t writeData[2];
  writeData[0] = regstr;
  writeData[1] = data;
  ret_code_t err_code = nrf_drv_twi_tx(&app_twi_instance, DRV2604_I2C_ADDRESS,
                                       writeData, 2, false);
  APP_ERROR_CHECK(err_code);
  return (err_code == NRF_SUCCESS);
}

static bool I2C_WriteMultiByte(uint8_t *data, uint16_t len) {
  ret_code_t err_code =
      nrf_drv_twi_tx(&app_twi_instance, DRV2604_I2C_ADDRESS, data, len, false);
  APP_ERROR_CHECK(err_code);
  return (NRF_SUCCESS == err_code);
}

static uint8_t I2C_ReadSingleByte(uint8_t regstr) {
  uint8_t data;
  data = regstr;
  ret_code_t err_code =
      nrf_drv_twi_tx(&app_twi_instance, DRV2604_I2C_ADDRESS, &data, 1, true);
  APP_ERROR_CHECK(err_code);
  err_code = nrf_drv_twi_rx(&app_twi_instance, DRV2604_I2C_ADDRESS, &data, 1);
  APP_ERROR_CHECK(err_code);
  return data;
}

/**
 * Haptics_Init - initialize haptics variables and settings
 * This should be run once at device power up
 */
void Haptics_Init(void) {
  tickdelay = (unsigned int)TICKDELAY;
  nrf_gpio_pin_clear(
      DRV2604_ENABLE_PIN); // initialize enable pin to output and low
  nrf_gpio_cfg_output(DRV2604_ENABLE_PIN);

  // Set DRV260x Hardware Controls
  Haptics_EnableAmplifier();              // Enable the DRV260x
  Haptics_LoadSwitchSelect(ACTUATOR_LRA); // Select ERM using load switch

  //nrf_delay_us(500); // Pause after enabling

  // DRV260x Initialization
  Haptics_Active();

  I2C_WriteSingleByte(DRV260x_RTP,
                      0x00); // Set RTP register to zero, prevent playback
  I2C_WriteSingleByte(DRV260x_WAVEFORMSEQ1,
                      0x01); // Insert waveform 1 into sequence register 1
  I2C_WriteSingleByte(
      DRV260x_WAVEFORMSEQ2,
      0x00); // Insert termination character in sequence register 2
  I2C_WriteSingleByte(DRV260x_ODT, 0x00); // Set Library Overdrive time to zero
  I2C_WriteSingleByte(DRV260x_SPT, 0x00); // Set Library Sustain positive time
  I2C_WriteSingleByte(DRV260x_SNT, 0x00); // Set Library Sustain negative time
  I2C_WriteSingleByte(DRV260x_BRT, 0x00); // Set Library Brake Time
  I2C_WriteSingleByte(0x13, 0x64);        // A2H Vpeak maximum

#if DRV2604
  // Load header into RAM
  I2C_WriteSingleByte(DRV2604_RAMADDR_UB, 0x00);
  I2C_WriteSingleByte(DRV2604_RAMADDR_LB, 0x00);
  I2C_WriteMultiByte((unsigned char *)DRV2604_HEADER,
                     DRV2604_HEADERSIZE); // Send header data
  // Load waveform data into RAM
  I2C_WriteSingleByte(DRV2604_RAMADDR_UB, 0x01);
  I2C_WriteSingleByte(DRV2604_RAMADDR_LB, 0x00);
  I2C_WriteMultiByte((unsigned char *)DRV2604_DATA,
                     DRV2604_DATASIZE); // Send waveform data

#endif

  Haptics_SetControlRegisters(); // Set control register values
  // NRF_LOG("q");
  Haptics_Standby();
  // NRF_LOG("r");
}

/**
 * Haptics_SetControlRegisters - set the control register variables
 */
void Haptics_SetControlRegisters() {
  /* Set the Default control register values, see Haptics.h to define*/
  control1 = DEFAULT_CTRL1;
  control2 = DEFAULT_CTRL2;
  control3 = DEFAULT_CTRL3;
}

/**
 * Haptics_RunAutoCal_LRA - run auto-calibration for an LRA actuator
 */
void Haptics_RunAutoCal_LRA(void) {
  Haptics_Active();
  Haptics_LoadSwitchSelect(ACTUATOR_LRA);

/* Store settings from ActuatorSettings.h in variables for reuse */
#define LRA_AUTOCAL_COMP 0x08                   // Compensation Coef.
#define LRA_AUTOCAL_BEMF 0xB9                   // BEMF Coef.
#define LRA_AUTOCAL_FB 0xB5                     // Feedback Control
#define LRA_RATED_VOLTAGE VoltageRMS_LRA_RV_1p8 // Rated voltage
#define LRA_OVERDRIVE_VOLTAGE Voltage_3p0       // Overdrive voltage
  LRA_AutoCal_FB = LRA_AUTOCAL_FB;
  LRA_AutoCal_Comp = LRA_AUTOCAL_COMP;
  LRA_AutoCal_BEMF = LRA_AUTOCAL_BEMF;
  LRA_RatedVoltage = LRA_RATED_VOLTAGE;
  LRA_ODClamp = LRA_OVERDRIVE_VOLTAGE;

  /* Setup Auto-calibration for LRA */
  I2C_WriteSingleByte(DRV260x_RATED_VOLTAGE, LRA_RatedVoltage);
  I2C_WriteSingleByte(DRV260x_OD_CLAMP, LRA_ODClamp);
  I2C_WriteSingleByte(DRV260x_FEEDBACK_CONTROL, LRA_AutoCal_FB);
  I2C_WriteSingleByte(DRV260x_CONTROL1, control1);
  I2C_WriteSingleByte(DRV260x_CONTROL2, control2);
  I2C_WriteSingleByte(DRV260x_CONTROL3, control3);

  /* Run Auto-calibration */
  I2C_WriteSingleByte(DRV260x_MODE, Auto_Calibration);
  I2C_WriteSingleByte(DRV260x_AUTOCAL_MEMIF, AutoCalTime_500MS);
  I2C_WriteSingleByte(DRV260x_GO, GO);

  /* Poll GO bit until it AutoCal is finished */
  nrf_delay_us(50000);
  while (I2C_ReadSingleByte(DRV260x_GO) == GO) {
    nrf_delay_us(50000);
  }

  LRA_AutoCal_Comp = I2C_ReadSingleByte(DRV260x_AUTOCAL_COMP);
  LRA_AutoCal_BEMF = I2C_ReadSingleByte(DRV260x_AUTOCAL_BEMF);
  LRA_AutoCal_FB = I2C_ReadSingleByte(DRV260x_FEEDBACK_CONTROL);

  /* Check if AutoCal was Successful */
  AutoCal_Result = I2C_ReadSingleByte(DRV260x_STATUS);
  AutoCal_Result = (AutoCal_Result & 0x08) >> 3;
  NRF_LOG_PRINTF_DEBUG("LRA AutoCal Result (0=passed, 1=failed): %02x\r\n",
                 AutoCal_Result);

  I2C_WriteSingleByte(DRV260x_MODE,
                      ACTIVE | Int_Trig); // Set to internal trigger mode
  Haptics_Standby();
}

/**
 * Haptics_Diagnostics - run actuator diagnostics
 * @param actuator - unsigned char - actuator constant (ACTUATOR_ERM,
 * ACTUATOR_LRA)
 * @return unsigned char - DRV260x status register
 */
unsigned char Haptics_Diagnostics(uint8_t actuator) {
  unsigned char diagResults = 0;

  Haptics_Active();
  Haptics_LoadSwitchSelect(actuator); // Set load switch to specified actuator
  Haptics_SetActuator(actuator);      // Select the actuator
  Haptics_SendActuatorSettings(); // Send the actuator settings to the DRV260x

  I2C_WriteSingleByte(DRV260x_MODE, Diagnostics); // Set mode to Diagnostics
  I2C_WriteSingleByte(DRV260x_GO, GO);            // Begin Diagnostics

  /* Poll GO bit until it Diagnostics is finished */
  nrf_delay_us(50000);
  while (I2C_ReadSingleByte(DRV260x_GO) == GO) {
    nrf_delay_us(50000);
  }

  diagResults =
      I2C_ReadSingleByte(DRV260x_STATUS); // Get status register values
  diagResults = (diagResults & 0x08) >> 3;
  NRF_LOG_PRINTF_DEBUG("LRA Diagnostics Result (0=passed, 1=failed): %02x\r\n",
                 diagResults);
  Haptics_Standby();
  Haptics_DisableAmplifier();

  return diagResults;
}

/**
 * Haptics_SendWaveformDefault- send haptic waveform using previously
 * 		selected actuator and trigger.
 * @param struct Waveform - the waveform output type, length in bytes, and data
 */
void Haptics_SendWaveformDefault(const Waveform waveform) {
  if (playEffect) {
    Haptics_Start(
        waveform.inputMode); // Enable the amplifier, begin PWM (if required)
    Haptics_HardwareMode(
        waveform.inputMode);   // Configure the Hardware (load switch, GPIOs)
    Haptics_SendTriggerType(); // Send trigger type to DRV260x
    Haptics_SendActuatorSettings(); // Send actuator settings to DRV260x

    Haptics_DisableTriggerActive(); // Stop waveform playback

    Haptics_OutputWaveform(waveform); // Send waveform data to driver

    /* Stop PWM and RTP output when finished */
    if (waveform.inputMode == MODE_PWM || waveform.inputMode == MODE_RTP) {
      Haptics_Stop(waveform.inputMode);
    }
  }
}

/**
 * Haptics_SendWaveform - send haptic waveform
 * @param struct Waveform - the waveform input type, length in bytes, and data
 * @param unsigned char actuator - actuator type
 * @param unsigned char trigger - trigger type (internal, ext. edge, ext. level)
 */
void Haptics_SendWaveform(const Waveform waveform, const uint8_t actuator,
                          const uint8_t trigger) {
  if (playEffect) {
    actuatorType = actuator; // Set actuator type
    triggerType = trigger;   // Set trigger type

    Haptics_Start(
        waveform.inputMode); // Enable the amplifier, begin PWM (if required)
    Haptics_HardwareMode(
        waveform.inputMode);   // Configure the Hardware (load switch, GPIOs)
    Haptics_SendTriggerType(); // Send trigger type to DRV260x
    Haptics_SendActuatorSettings(); // Send actuator settings to DRV260x

    Haptics_DisableTriggerActive(); // Stop waveform playback

    Haptics_OutputWaveform(waveform); // Send waveform data to driver
//    uint8_t reg = DRV260x_VBAT;
//    uint8_t data = I2C_ReadSingleByte(reg);
//    NRF_LOG_PRINTF("Haptics battery voltage is: %02x\r\n", data);
//    /* Stop PWM and RTP output when finished */
    if (waveform.inputMode == MODE_PWM || waveform.inputMode == MODE_RTP) {
      Haptics_Stop(waveform.inputMode);
    }
  }
}

/**
 * Haptics_SendWaveform - send haptic waveform
 * @param struct Waveform - the waveform output type, length in bytes, and data
 */
void Haptics_OutputWaveform(const Waveform waveform) {
  uint8_t i, j, k;
  uint8_t ReadVal;

  if (playEffect) {
    // XXX		P3OUT &= 0xFB;		// Set Trigger Pin Low

    switch (waveform.inputMode) {
    case MODE_RAM: // RAM Mode

      I2C_WriteSingleByte(DRV260x_LIBRARY, RAM); // Select RAM Library

      for (i = 0; i < waveform.length; i += 2) {
        I2C_WriteSingleByte(waveform.data[i],
                            waveform.data[(i + 1)]); // Send (register, value)
      }

      Haptics_EnableTrigger(); // Set RAM trigger (Internal, Ext. Edge, Ext.
                               // Level)

      break;
    case MODE_ROM: // ROM Mode
      for (i = 0; i < waveform.length; i += 3) {
        if (waveform.data[i + 2] !=
            0xFF) { // Read register, Operate, Write register
          ReadVal = I2C_ReadSingleByte(waveform.data[i]);
          ReadVal = waveform.data[i + 1] |
                    (ReadVal & ~waveform.data[i + 2]); // Apply Mask to ReadData
          I2C_WriteSingleByte(waveform.data[i],
                              ReadVal); // Send (register, value)
        } else {                        // Write over existing register setting
          I2C_WriteSingleByte(waveform.data[i],
                              waveform.data[(i + 1)]); // Send (register, value)
        }
      }

      Haptics_EnableTrigger(); // Set ROM trigger (Internal, Ext. Edge, Ext.
                               // Level)

      break;

    case MODE_PWM: // PWM Mode

      I2C_WriteSingleByte(DRV260x_MODE, PWM_Analog); // Set PWM input mode

      for (k = 0; k < waveform.length;
           k = k + 2) { // Go through waveform data array of time-value pairs
                        // XXX				TA1CCR1 = waveform.data[k]+
                        // waveform.data[k]/4
        // + waveform.data[k]/16;  // Set PWM duty cycle - 1.316x
        for (j = 0; j < waveform.data[k + 1];
             ++j) {                // Count number of 5ms intervals
          nrf_delay_us(tickdelay); // Time delay before next change in amplitude
        }
      }

      break;

    case MODE_RTP: // RTP Mode

      I2C_WriteSingleByte(DRV260x_MODE, RTP); // Set RTP Mode

      for (k = 0; k < waveform.length;
           k = k + 2) { // Go through waveform data array of time-value pairs
        I2C_WriteSingleByte(DRV260x_RTP,
                            waveform.data[k]); // Send (register, value)
        for (j = 0; j < waveform.data[k + 1];
             ++j) {                // Count number of 5ms intervals
          nrf_delay_us(tickdelay); // Time delay before change in amplitude
        }
      }

      break;
    default:

      for (i = 0; i < waveform.length; i += 3) {
        if (waveform.data[i + 2] !=
            0xFF) { // Read register, Operate, Write register
          ReadVal = I2C_ReadSingleByte(waveform.data[i]);
          ReadVal = waveform.data[i + 1] |
                    (ReadVal & ~waveform.data[i + 2]); // Apply Mask to ReadData
          I2C_WriteSingleByte(waveform.data[i],
                              ReadVal); // Send (register, value)
        } else {                        // Write over existing register setting
          I2C_WriteSingleByte(waveform.data[i],
                              waveform.data[(i + 1)]); // Send (register, value)
        }
      }

      Haptics_EnableTrigger(); // Set ROM trigger (Internal, Ext. Edge, Ext.
                               // Level)

      break;
    }
  }
}

/**
 * Haptics_HardwareMode - Set the hardware pins to the appropriate setting
 * @param unsigned char inputMode - the waveform input type
 */
void Haptics_HardwareMode(uint8_t inputMode) {
  switch (inputMode) {
  case MODE_PWM: // PWM Mode
                 /// XXX		P3DIR |= BIT2; 			// P3.2 = Output
                 /// XXX		P3SEL |= BIT2;			// P3.2 = PWM Output
    nrf_gpio_pin_clear(DRV2604_ENABLE_PIN);
    nrf_gpio_cfg_output(DRV2604_ENABLE_PIN);
    break;
  case MODE_RTP: // RTP Mode
                 /// XXX		P3OUT &= 0xFB;			// P3.2 = Low
                 /// XXX		P3SEL &= 0xFB;			// P3.2 = GPIO
                 /// XXX		P3DIR |= 0x04;			// P3.2 = Output
    //	XXX don't reset enable here??    nrf_gpio_pin_clear(DRV2604_ENABLE_PIN);
    //		nrf_gpio_cfg_output(DRV2604_ENABLE_PIN);
    break;
  case MODE_ANALOG: // Analog Mode
                    /// XXX		P3DIR &= 0xFB;			// P3.2 = Hi-Z
                    /// (MSP430 Pin Hi-Z)
    nrf_gpio_cfg_input(DRV2604_ENABLE_PIN, NRF_GPIO_PIN_NOPULL);
    break;
  case MODE_A2H:
    /// XXX		P3DIR &= 0xFB;			// P3.2 = Hi-Z
    /// (MSP430 Pin Hi-Z)
    nrf_gpio_cfg_input(DRV2604_ENABLE_PIN, NRF_GPIO_PIN_NOPULL);
    break;
  default: // RAM or ROM modes
           /// XXX		P3DIR |= 0x04;			// P3.2 = Output
           /// XXX		P3OUT &= 0xFB;			// P3.2 = Low
           /// XXX		P3SEL &= 0xFB;			// P3.2 = GPIO
    //	XXX don't reset enable here??    nrf_gpio_pin_clear(DRV2604_ENABLE_PIN);
    //		nrf_gpio_cfg_output(DRV2604_ENABLE_PIN);
    break;
  }
}

/**
 * Haptics_SendTriggerType - set the trigger settings in the DRV260x
 */
void Haptics_SendTriggerType() {
  /* Set the trigger mode in the DRV260x */
  if (triggerType == TRIGGER_EXTERNAL_EDGE) {
    I2C_WriteSingleByte(DRV260x_MODE, Ext_Trig_Edge);
  } else if (triggerType == TRIGGER_EXTERNAL_LEVEL) {
    I2C_WriteSingleByte(DRV260x_MODE, Ext_Trig_Level);
  } else { // Internal trigger
    I2C_WriteSingleByte(DRV260x_MODE, Int_Trig);
  }
}

/**
 * Haptics_SendActuatorSettings - set the actuator settings in the DRV260x
 */
void Haptics_SendActuatorSettings() {
  /* DRV2605 Actuator Settings */

  I2C_WriteSingleByte(DRV260x_RATED_VOLTAGE, LRA_RatedVoltage);
  I2C_WriteSingleByte(DRV260x_OD_CLAMP, LRA_ODClamp);
  I2C_WriteSingleByte(DRV260x_AUTOCAL_COMP, LRA_AutoCal_Comp);
  I2C_WriteSingleByte(DRV260x_AUTOCAL_BEMF, LRA_AutoCal_BEMF);
  I2C_WriteSingleByte(DRV260x_FEEDBACK_CONTROL, LRA_AutoCal_FB);
  Haptics_LoadSwitchSelect(ACTUATOR_LRA);
}

/**
 * Haptics_SetERMOpenLoopMode - set the device to open loop mode
 * @param openLoopMode - ERM_ClosedLoop = closed-loop mode, ERM_OpenLoop =
 * open-loop mode
 */
void Haptics_SetERMOpenLoopMode(uint8_t openLoop) { IsERMOpenLoop = openLoop; }

/**
 * Haptics_GetERMOpenLoopMode - get the status of ERM open loop flag
 * @return unsigned char 0 = Closed Loop, 1 = Open Loop
 */
unsigned char Haptics_GetERMOpenLoopMode() { return IsERMOpenLoop; }

/**
 * Haptics_SetLRAOpenLoopMode - set the device to LRA open loop mode
 * @param openLoopMode - LRA_AutoResonance = closed-loop mode, LRA_OpenLoop =
 * Divide by 128x
 */
void Haptics_SetLRAOpenLoopMode(uint8_t openLoop) { IsLRAOpenLoop = openLoop; }

/**
 * Haptics_GetLRAOpenLoopMode - get the status of LRA open loop flag
 * @return unsigned char 0 = AutoResonance, 1 = Divide by 128x
 */
unsigned char Haptics_GetLRAOpenLoopMode() { return IsLRAOpenLoop; }

/**
 * Haptics_EnableTrigger - turn on the trigger, uses last executed trigger type
 */
void Haptics_EnableTrigger() {
  Haptics_EffectActive(1);

  if (triggerType == TRIGGER_INTERNAL) {
    I2C_WriteSingleByte(DRV260x_GO, GO); // Set Go bit with I2C
  } else if (triggerType == TRIGGER_EXTERNAL_EDGE) {
			nrf_gpio_pin_set(DRV2604_TRIGGER_PIN);
			nrf_delay_us(100);
			nrf_gpio_pin_clear(DRV2604_TRIGGER_PIN);
  } else if (triggerType == TRIGGER_EXTERNAL_LEVEL) {
			nrf_gpio_pin_set(DRV2604_TRIGGER_PIN);
  }
}

/**
 * Haptics_DisableTriggerActive - turn off the active trigger
 */
void Haptics_DisableTriggerActive() {
  if (Haptics_IsEffectActive()) {
    Haptics_EffectActive(0);

    if (triggerType == TRIGGER_INTERNAL) {
      I2C_WriteSingleByte(DRV260x_GO, STOP); // Set Go bit with I2C
    } else if (triggerType == TRIGGER_EXTERNAL_EDGE) {
				nrf_gpio_pin_set(DRV2604_TRIGGER_PIN);
				nrf_delay_us(100);
				nrf_gpio_pin_clear(DRV2604_TRIGGER_PIN);
    } else if (triggerType == TRIGGER_EXTERNAL_LEVEL) {
				nrf_gpio_pin_clear(DRV2604_TRIGGER_PIN);
    }
  }
}

/**
 * Haptics_DisableTriggerActive - turn off the trigger specified
 * @param trigger - unsigned int - trigger type
 */
void Haptics_DisableTrigger(uint8_t trigger) {
  Haptics_EffectActive(0);

  if (trigger == TRIGGER_INTERNAL) {
    I2C_WriteSingleByte(DRV260x_GO, STOP); // Set Go bit with I2C
  } else if (trigger == TRIGGER_EXTERNAL_EDGE) {
			nrf_gpio_pin_set(DRV2604_TRIGGER_PIN);
			nrf_delay_us(100);
			nrf_gpio_pin_clear(DRV2604_TRIGGER_PIN);
  } else if (trigger == TRIGGER_EXTERNAL_LEVEL) {
			nrf_gpio_pin_clear(DRV2604_TRIGGER_PIN);
  }
}

/**
 * Haptics_StartPWM - Initialize PWM clocks and enable the haptics driver
 * @param unsigned char inputMode - the input mode of the current waveform
 */
void Haptics_Start(uint8_t inputMode) {
  if (playEffect) {
    Haptics_EnableAmplifier();
		Haptics_Active();

#if 0 // XXX
		if(inputMode == MODE_PWM) {	// If PWM mode
			/* Setup PWM registers */
			TA1R=0;                        	// Reset PWM Count
			TA1CCTL1 |= OUTMOD_7 + CCIE;   	// PWM Set/Reset Mode
			TA1CTL = TASSEL_2 + MC_1;       // 2: TACLK = SMCLK
		}
#endif
  }
}

/**
 * Haptics_StartPWM - Stop PWM clocks and disable the haptics driver
 * @param unsigned char inputMode - the input mode of the current waveform
 */
void Haptics_Stop(uint8_t inputMode) {
  if (playEffect) {
    Haptics_EffectActive(0); // Set effect playback flag to inactive

    /* Reset DRV260x to default register settings */
    I2C_WriteSingleByte(DRV260x_CONTROL1, control1);
    I2C_WriteSingleByte(DRV260x_CONTROL2, control2);
    I2C_WriteSingleByte(DRV260x_CONTROL3, control3);

    IsERMOpenLoop = ERMDefaultOpenLoopSetting; // Reset ERM_OpenLoop bit
    IsLRAOpenLoop = LRADefaultOpenLoopSetting; // Reset LRA_OpenLoop bit

    Haptics_DisableAmplifier();

#if 0 // XXX
		if(inputMode == MODE_PWM) {	// If current mode is PWM
			TA1CCR1 = 0x00;					// Reset duty cycle
			TA1CTL = 0x0004;                // Stop PWM
			TA1CCTL1 &= ~(OUTMOD_7 | OUT);  // PWM output = LOW
			P3OUT &= 0xFB;					// Disable PWM GPIO
		}
#endif
  }
}

/**
 * Haptics_DefaultReset - reset control register 1-3 with default values
 */
void Haptics_DefaultReset() {
  Haptics_EnableAmplifier();

  /* Reset DRV260x to default register settings */
  I2C_WriteSingleByte(DRV260x_CONTROL1, control1);
  I2C_WriteSingleByte(DRV260x_CONTROL2, control2);
  I2C_WriteSingleByte(DRV260x_CONTROL3, control3);

  Haptics_DisableAmplifier();
}

/**
 * Haptics_SetActuator - set the actuator type, call before
 * Haptics_HardwareMode()
 * @param unsigned char actuator - acutator type
 */
void Haptics_SetActuator(uint8_t actuator) { actuatorType = actuator; }

/**
 * Haptics_IsActuatorLRA - determine actuator selected
 * @return unsigned char - actuator type (0 = ERM, 1 = LRA)
 */
uint8_t Haptics_IsActuatorLRA() { return actuatorType; }

/**
 * Haptics_SetTrigger - set the trigger DRV260x trigger type, call before
 * Haptics_HardwareMode()
 * @param unsigned char trigger - trigger constant
 */
void Haptics_SetTrigger(uint8_t trigger) { triggerType = trigger; }

/**
 * Haptics_GetTrigger - get the trigger type
 * @return unsigned char - trigger type (internal = 0, ext. edge = 1, ext. level
 * = 2)
 */
uint8_t Haptics_GetTrigger() { return triggerType; }

/**
 * Haptics_OutputEnableSet - enable/disable play back of all waveforms
 */
void Haptics_OutputEnableSet(uint8_t enable) { playEffect = enable; }

/**
 * Haptics_IsOutputEnabled - check if waveform play back is enabled
 */
uint8_t Haptics_IsOutputEnabled(void) { return playEffect; }

/**
 * Haptics_EffectActive - set if effect is active
 * @param unsigned char active - set activity status
 */
void Haptics_EffectActive(uint8_t active) { EffectActive = active; }

/**
 * Haptics_IsEffectActive - check if effect is active
 * @return unsigned char - 0  = inactive, 1 = active
 */
uint8_t Haptics_IsEffectActive(void) { return EffectActive; }

/**
 * Haptics_EnableAmplifier - set enable pin high
 */
void Haptics_EnableAmplifier(void) { nrf_gpio_pin_set(DRV2604_ENABLE_PIN); }

/**
 * Haptics_DisableAmplifier - set enable pin low
 */
void Haptics_DisableAmplifier(void) { nrf_gpio_pin_clear(DRV2604_ENABLE_PIN); }

void Haptics_Active(void) { I2C_WriteSingleByte(DRV260x_MODE, ACTIVE); }
void Haptics_Standby(void) { I2C_WriteSingleByte(DRV260x_MODE, STANDBY); }

/**
 * Haptics_SetDisabledAllowed
 * @return disabledAllowed - unsigned char - 0 = disabled not allowed, 1 =
 * disabled allowed
 */
unsigned char Haptics_IsDisableAllowed() { return disableAllowed; }

/**
 * Haptics_SetDisabledAllowed
 * @param allowed - unsigned char - 0 = disabled not allowed, 1 = disabled
 * allowed
 */
void Haptics_SetDisabledAllowed(uint8_t allowed) { disableAllowed = allowed; }

/**
 * Haptics_LoadSwitchSelect - select the on-board actuator using the load switch
 * @param unsigned char actuator 0 = ERM, 1 = LRA
 */
void Haptics_LoadSwitchSelect(uint8_t actuator) {
#if 0 // XXX
	if(actuator) {
		P2OUT |= 0x40;    // Select LRA
	} else {
		P2OUT &= 0xBF;    // Select ERM
	}
#endif
}

/**
 * Haptics_SetLibrary - select ROM Library (1-6)
 * @param unsigned char Library - library number 1-6
 */
void Haptics_SetLibrary(uint8_t library) {
  I2C_WriteSingleByte(DRV260x_LIBRARY, library); // Select Library
}

/**
 * Haptics_SetSequenceRegister - insert a waveform into one of 8 sequence
 * registers
 * @param unsigned char sequenceReg - register address for sequence register
 * @param unsigned char waveformIndex - index number for waveform
 */
void Haptics_SetSequenceRegister(uint8_t sequenceReg, uint8_t waveformIndex) {
  I2C_WriteSingleByte(sequenceReg, waveformIndex); // Set register
}

/**
 * Haptics_WaitWaveformComplete - wait for playback to complete for internal
 * trigger waveforms
 */
void Haptics_WaitWaveformComplete() {
  if (playEffect && EffectActive) {
    while (I2C_ReadSingleByte(DRV260x_GO) == GO) {
      // XXX			sleep(500);
    }
  }
}
