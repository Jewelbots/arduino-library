/****************************************************************************
 * DRV260x Header File for Register Defines
 * Updated: 12/21/2012
 *
 ****************************************************************************/

/****************************************************************************/
/* Status Register */
/****************************************************************************/
#define DRV260x_STATUS (0x00)

/****************************************************************************/
/* Mode Register */
/****************************************************************************/
#define DRV260x_MODE (0x01)
/*** Register Bits ***/
#define Dev_Reset (0x80)
#define STANDBY (0x40)
#define ACTIVE (0x00)
/* Mode Definitions */
#define Int_Trig (0x00)
#define Ext_Trig_Edge (0x01)
#define Ext_Trig_Level (0x02)
#define PWM_Analog (0x03)
#define Audio2Haptics (0x04)
#define RTP (0x05)
#define Diagnostics (0x06)
#define Auto_Calibration (0x07)

/****************************************************************************/
/* Real-Time Playback (RTP) Input  Register */
/****************************************************************************/
#define DRV260x_RTP (0x02)
/* Insert RTP Data directly into the data field at this address
Open Loop (ERM Only), BiDir_Input = N/A, SignedUnsigned_RTP = 0
0x7F, Full scale positive (forward rotation)
0x00, No signal
0x81, Full scale negative (reverse rotation or braking)

Open Loop (ERM Only), BiDir_Input = N/A, SignedUnsigned_RTP = 1
0xFF, Full scale positive (forward rotation)
0x80, No signal
0x00, Full scale negative (reverse rotation or braking)

Closed Loop, BiDir_Input = 1, SignedUnsigned_RTP = 0
0x7F, Full scale drive
Less than 0x00 interpreted as 0x81 (full-scale brake)

Closed Loop, BiDir_Input = 1, SignedUnsigned_RTP = 1
0xFF, Full scale drive
Less than 0x7F interpreted as 0x00 (full-scale brake)

Closed Loop, BiDir_Input = 0, SignedUnsigned_RTP = N/A
0xFF, Full scale drive
0x80, Half scale drive
0x00, No signal, braking happens when input is less than back-EMF */

/****************************************************************************/
/* Library Selection Register */
/****************************************************************************/
#define DRV260x_LIBRARY (0x03)
/*** Register Bits ***/
#define HIZ (0x10)
#define RAM (0x00)
#define ROM_A (0x01)
#define ROM_B (0x02)
#define ROM_C (0x03)
#define ROM_D (0x04)
#define ROM_E (0x05)
#define ROM_LRA (0x06)

/****************************************************************************/
/* Waveform Sequence Registers */
/****************************************************************************/
#define DRV260x_WAVEFORMSEQ1 (0x04)
#define DRV260x_WAVEFORMSEQ2 (0x05)
#define DRV260x_WAVEFORMSEQ3 (0x06)
#define DRV260x_WAVEFORMSEQ4 (0x07)
#define DRV260x_WAVEFORMSEQ5 (0x08)
#define DRV260x_WAVEFORMSEQ6 (0x09)
#define DRV260x_WAVEFORMSEQ7 (0x0A)
#define DRV260x_WAVEFORMSEQ8 (0x0B)

/* When bit[7] is set, bits [6:0] are interpreted as a �wait time� in which the
playback engine will idle.
This is used to insert timed delays between sequentially played waveforms.
Delay time is 10 ms * WavefrmSeq[6:0].
If bit[7] = 0, then WavfrmSeq[6:0] is interpreted as a waveform identifier for
sequence playback.
A Waveform Sequence value holds the waveform identifier of the waveform to be
played.
A waveform identifier is an integer value referring to the index position of a
waveform in a RAM or ROM library.
Playback begins at register address 0x04 when the user asserts the GO bit
(Register 0x0C).
When playback of that waveform ends, the waveform sequencer plays the next
waveform identifier held in register 0x05, if it is non-zero.
The waveform sequencer continues in this fashion until it reaches an identifier
value of zero, or
all eight identifiers are played (register addresses 0x04 through 0x0B),
whichever comes first. */

/****************************************************************************/
/* GO Register */
/****************************************************************************/
#define DRV260x_GO (0x0C)
/*** Register Bits ***/
#define GO (0x01)
#define STOP (0x00)
/* Used to fire processes in the DRV260x */

/****************************************************************************/
/* Overdrive Time Offset (ODT) Register */
/****************************************************************************/
#define DRV260x_ODT (0x0D)
/* OverDrive Time Offset (ms) = ODT[7:0] * 5
*/
/****************************************************************************/
/* Sustain Time Offset Positive (SPT) Register */
/****************************************************************************/
#define DRV260x_SPT (0x0E)
/* Sustain Time Positive Offset (ms) = SPT[7:0] * 5
*/
/****************************************************************************/
/* Sustain Time Offset Negative (SNT) Register */
/****************************************************************************/
#define DRV260x_SNT (0x0F)
/* Sustain Time Negative Offset (ms) = SNT[7:0] * 5
*/
/****************************************************************************/
/* Brake Time Offset (BRT) Register */
/****************************************************************************/
#define DRV260x_BRT (0x10)
/* Braking Time Offset (ms) = BRT[7:0] * 5
*/

/****************************************************************************/
/* Rated Voltage Register */
/****************************************************************************/
#define DRV260x_RATED_VOLTAGE (0x16)
/* Rated Voltage (V) = RatedVoltage[7:0] / 255 * 5.3 V
 * User should write this value before performing autocal
 */

// ERM Overdrive(Open Loop), LRA Overdrive (Closed Loop)
#define Voltage_1p3 (0x3B)
#define Voltage_1p5 (0x44)
#define Voltage_1p8 (0x52)
#define Voltage_2p0 (0x5B)
#define Voltage_2p3 (0x69)
#define Voltage_2p5 (0x72)
#define Voltage_2p7 (0x7B)
#define Voltage_3p0 (0x89)
#define Voltage_3p3 (0x96)
#define Voltage_3p6 (0xA4)
#define Voltage_5p0 (0xE4)

// ERM Overdrive - Closed Loop
#define Voltage_ERM_OD_CL_1p3 (0x43)
#define Voltage_ERM_OD_CL_1p5 (0x4D)
#define Voltage_ERM_OD_CL_1p8 (0x5D)
#define Voltage_ERM_OD_CL_2p0 (0x67)
#define Voltage_ERM_OD_CL_2p5 (0x81)
#define Voltage_ERM_OD_CL_2p7 (0x8B)
#define Voltage_ERM_OD_CL_3p0 (0x9B)
#define Voltage_ERM_OD_CL_3p3 (0xAA)
#define Voltage_ERM_OD_CL_3p6 (0xBA)
#define Voltage_ERM_OD_CL_5p0 (0xFF)

// ERM Rated Voltage - Closed Loop
#define Voltage_ERM_RV_CL_1p3 (0x3D)
#define Voltage_ERM_RV_CL_1p5 (0x46)
#define Voltage_ERM_RV_CL_1p8 (0x54)
#define Voltage_ERM_RV_CL_2p0 (0x5E)
#define Voltage_ERM_RV_CL_2p5 (0x75)
#define Voltage_ERM_RV_CL_2p7 (0x7F)
#define Voltage_ERM_RV_CL_3p0 (0x8D)
#define Voltage_ERM_RV_CL_3p3 (0x9B)
#define Voltage_ERM_RV_CL_3p6 (0xA9)
#define Voltage_ERM_RV_CL_5p0 (0xEB)

// LRA Rated Voltage - Closed Loop
#define VoltageRMS_LRA_RV_1p5 (0x3E) // 1.5 Vrms
#define VoltageRMS_LRA_RV_2p0 (0x53) // 2.0 Vrms
// Jewelbots LRA motor is smaller and higher frequency so new value needed
#define VoltageRMS_LRA_RV_1p8                                                  \
  (0x39) // 1.8 Vrms @ 235 Hz (was 0x38 for non 'L' version of Haptics chip).

/****************************************************************************/
/* OverDrive Clamp Voltage Register */
/****************************************************************************/
#define DRV260x_OD_CLAMP (0x17)
/* Overdrive Clamp Voltage (V) = ODClamp[7:0] / 255 * 5.6 V
*/

/****************************************************************************/
/* Auto Calibration Compensation Result Register */
/****************************************************************************/
#define DRV260x_AUTOCAL_COMP (0x18)
/* Calibration Compensation Coefficient = 1 + ACalComp[7:0] / 255
 * The AutoCal routine will populate this register
 */
/****************************************************************************/
/* Auto Calibration Back-EMF Result Register */
/****************************************************************************/
#define DRV260x_AUTOCAL_BEMF (0x19)
/* Auto Calibration Back-EMF Coefficient = (Value / 255) * 2.44 / BEMFGain
 * The AutoCal routine will populate this register
 */
/****************************************************************************/
/* Feedback Control Register */
/****************************************************************************/
#define DRV260x_FEEDBACK_CONTROL (0x1A)
/*** Register Bits ***/
#define ERM_MODE (0x00)
#define LRA_MODE (0x80)

#define FBBrakeFactor_1x (0x00)
#define FBBrakeFactor_2x (0x10)
#define FBBrakeFactor_3x (0x20)
#define FBBrakeFactor_4x (0x30)
#define FBBrakeFactor_6x (0x40)
#define FBBrakeFactor_8x (0x50)
#define FBBrakeFactor_16x (0x60)
#define FBBrakeFactor_Disabled (0x70)

#define LoopResponse_Slow (0x00)
#define LoopResponse_Medium (0x04)
#define LoopResponse_Fast (0x08)
#define LoopResponse_VeryFast (0x0C)

/* The AutoCal routine will populate BEMFGain
 * Interpretation of BEMFGain changes based on ERM/LRA MODE
 */
#define BEMFGainERM_0p3x (0x00)
#define BEMFGainERM_1x (0x01)
#define BEMFGainERM_1p8x (0x02)
#define BEMFGainERM_4x (0x03)
#define BEMFGainLRA_5x (0x00)
#define BEMFGainLRA_10x (0x01)
#define BEMFGainLRA_20x (0x02)
#define BEMFGainLRA_30x (0x03)

/****************************************************************************/
/* Control1 Register */
/****************************************************************************/
#define DRV260x_CONTROL1 (0x1B)
/*** Register Bits ***/
#define StartupBoost (0x80) // OFF 0x00 //ON (0x80)
#define BypassComp (0x40)
#define AC_Couple (0x20)
#define DC_Couple (0x00)

#define DriveTime_0p5m (0x00)
#define DriveTime_0p6m (0x01)
#define DriveTime_0p7m (0x02)
#define DriveTime_0p8m (0x03)
#define DriveTime_0p9m (0x04)
#define DriveTime_1p0m (0x05)
#define DriveTime_1p1m (0x06)
#define DriveTime_1p2m (0x07)
#define DriveTime_1p3m (0x08)
#define DriveTime_1p4m (0x09)
#define DriveTime_1p5m (0x0A)
#define DriveTime_1p6m (0x0B)
#define DriveTime_1p7m (0x0C)
#define DriveTime_1p8m (0x0D)
#define DriveTime_1p9m (0x0E)
#define DriveTime_2p0m (0x0F)
#define DriveTime_2p1m (0x10)
#define DriveTime_2p2m (0x11)
#define DriveTime_2p3m (0x12)
#define DriveTime_2p4m (0x13)
#define DriveTime_2p5m (0x14)
#define DriveTime_2p6m (0x15)
#define DriveTime_2p7m (0x16)
#define DriveTime_2p8m (0x17)
#define DriveTime_2p9m (0x18)
#define DriveTime_3p0m (0x19)
#define DriveTime_3p1m (0x1A)
#define DriveTime_3p2m (0x1B)
#define DriveTime_3p3m (0x1C)
#define DriveTime_3p4m (0x1D)
#define DriveTime_3p5m (0x1E)
#define DriveTime_3p6m (0x1F)

/****************************************************************************/
/* Control2 Register */
/****************************************************************************/
#define DRV260x_CONTROL2 (0x1C)
/*** Register Bits ***/
#define BiDir_Input (0x80)
#define UniDir_Input (0x00)

#define BrakeStabilizer (0x40)

#define SampleTime_150us (0x00)
#define SampleTime_200us (0x10)
#define SampleTime_250us (0x20)
#define SampleTime_300us (0x30)

#define BlankingTime_VeryShort (0x00)
#define BlankingTime_Short (0x04)
#define BlankingTime_Medium (0x08)
#define BlankingTime_Long (0x06)

#define IDissTime_VeryShort (0x00)
#define IDissTime_Short (0x01)
#define IDissTime_Medium (0x02)
#define IDissTime_Long (0x03)

/****************************************************************************/
/* Control3 Register */
/****************************************************************************/
#define DRV260x_CONTROL3 (0x1D)
/*** Register Bits ***/
#define NGThresh_OFF (0x00)
#define NGThresh_2PERCENT (0x40)
#define NGThresh_4PERCENT (0x80)
#define NGThresh_8PERCENT (0xC0)

#define ERM_ClosedLoop (0x00)
#define ERM_OpenLoop (0x20)

#define SupplyCompDis (0x10)

#define DataFormat_RTP_Signed (0x00)
#define DataFormat_RTP_Unsigned (0x08)

#define LRADriveMode (0x04)
#define LRADriveMode_Once (0x00)
#define LRADriveMode_Twice (0x04)

#define InputMode_PWM (0x00)
#define InputMode_ANALOG (0x02)

#define LRA_AutoResonance (0x00)
#define LRA_OpenLoop (0x01)
#define LRA_ClosedLoop (0x00)
/****************************************************************************/
/* Auto Calibration Memory Interface Register - Control4 register on 'L'    */
/****************************************************************************/
#define DRV260x_AUTOCAL_MEMIF (0x1E)
/*** Register Bits ***/
#define AutoCalTime_150MS (0x00)
#define AutoCalTime_250MS (0x10)
#define AutoCalTime_500MS (0x20)
#define AutoCalTime_1000MS (0x30)

/* OTP_Status[2]	--Read Only
 * OTP_Fail[1]		--Read Only
 */

#define OTP_Program   (0x01)                                                    
/*Warning:  Only light this bit if you are happy with the settings 
* VDD must be 4.2V +/- 5%
* This process can only be executed once 
* Non-Volatile Memory will be burned for Registers 0x16 through 0x1A 
*/
/****************************************************************************/
/* Control5 Register */
/****************************************************************************/
#define DRV260x_CONTROL5 (0x1F)
/*** Register Bits ***/
#define IDISS_TIME (0x00)
#define BLANKING_TIME (0x00)
#define PLAYBACK_INTERVAL (0x00)
#define LRA_AUTO_OPEN_LOOP (0x00)
#define AUTO_OL_CNT (0x80)

/****************************************************************************/
/* VBAT Voltage Monitor Register */
/****************************************************************************/
#define DRV260x_VBAT (0x21)
/* VDD (V) = VBAT[7:0] * 5.6V / 255	--Read Only	*/

/****************************************************************************/
/* LRA Resonance Period Register */
/****************************************************************************/
#define DRV260x_LRA_PERIOD (0x22)
/* LRA Period (us) = LRA_Period[7:0] * 196.2 us  Read Only	*/

/****************************************************************************/
/* RAM Address/Data Registers */
/****************************************************************************/
#define DRV2604_RAMADDR_UB (0xFD)
#define DRV2604_RAMADDR_LB (0xFE)
#define DRV2604_RAMDATA (0xFF)

/* Data entry to waveform RAM interface.
 * The user may do single-byte writes or multi-byte writes to the  RAMDATA
 * register.
 * The controller starts the write at address [RAMAddrUB:RAMAddrLB].
 * For multi-byte writes, the controller automatically increments the RAM
 * Address for each byte written.
 * For single-byte writes, the RAM Address must be manually changed for each
 * transaction.
 */

/* Example I2C Write Usage
 * TBD
 */
