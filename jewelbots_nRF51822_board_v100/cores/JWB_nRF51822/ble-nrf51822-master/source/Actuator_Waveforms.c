/******************************************************************************
 * Actuator_Waveforms.h
 *
 * Created on: April 23, 2012
 * Board: DRV2604EVK-CT RevA
 *
 ******************************************************************************/

#include "Actuator_Waveforms.h"
#include "DRV2605.h"

//--------------------------------------------------------//
// RAM Playback Examples
//--------------------------------------------------------//
static const unsigned char StrongClick_data[] = {DRV260x_WAVEFORMSEQ1, 0x01,
                                                 DRV260x_WAVEFORMSEQ2, 0x00};
const Waveform StrongClick = {MODE_RAM, 4, StrongClick_data};

static const unsigned char MediumClick_data[] = {DRV260x_WAVEFORMSEQ1, 0x02,
                                                 DRV260x_WAVEFORMSEQ2, 0x00};
const Waveform MediumClick = {MODE_RAM, 4, MediumClick_data};

static const unsigned char LightClick_data[] = {DRV260x_WAVEFORMSEQ1, 0x03,
                                                DRV260x_WAVEFORMSEQ2, 0x00};
const Waveform LightClick = {MODE_RAM, 4, LightClick_data};

static const unsigned char Tick_data[] = {DRV260x_WAVEFORMSEQ1, 0x04,
                                          DRV260x_WAVEFORMSEQ2, 0x00};
const Waveform Tick = {MODE_RAM, 4, Tick_data};

static const unsigned char Bump_data[] = {DRV260x_WAVEFORMSEQ1, 0x05,
                                          DRV260x_WAVEFORMSEQ2, 0x00};
const Waveform Bump = {MODE_RAM, 4, Bump_data};

static const unsigned char DoubleStrongClick_data[] = {
    DRV260x_WAVEFORMSEQ1, 0x06, DRV260x_WAVEFORMSEQ2, 0x00};
const Waveform DoubleStrongClick = {MODE_RAM, 4, DoubleStrongClick_data};

static const unsigned char DoubleStrongBump_data[] = {DRV260x_FEEDBACK_CONTROL,
                                                      FBBrakeFactor_Disabled,
                                                      0x70,
                                                      DRV260x_WAVEFORMSEQ1,
                                                      0x06,
                                                      DRV260x_WAVEFORMSEQ2,
                                                      0x00};
const Waveform DoubleStrongBump = {MODE_RAM, 9, DoubleStrongBump_data};

static const unsigned char DoubleMediumClick_data[] = {
    DRV260x_WAVEFORMSEQ1, 0x07, DRV260x_WAVEFORMSEQ2, 0x00};
const Waveform DoubleMediumClick = {MODE_RAM, 4, DoubleMediumClick_data};

static const unsigned char DoubleLightClick_data[] = {
    DRV260x_WAVEFORMSEQ1, 0x08, DRV260x_WAVEFORMSEQ2, 0x00};
const Waveform DoubleLightClick = {MODE_RAM, 4, DoubleLightClick_data};

static const unsigned char TripleStrongClick_data[] = {
    DRV260x_WAVEFORMSEQ1, 0x09, DRV260x_WAVEFORMSEQ2, 0x00};
const Waveform TripleStrongClick = {MODE_RAM, 4, TripleStrongClick_data};

static const unsigned char BuzzAlert_data[] = {DRV260x_WAVEFORMSEQ1, 0x0A,
                                               DRV260x_WAVEFORMSEQ2, 0x00};
const Waveform BuzzAlert = {MODE_RAM, 4, BuzzAlert_data};

static const unsigned char RampUp_data[] = {DRV260x_WAVEFORMSEQ1, 0x0B,
                                            DRV260x_WAVEFORMSEQ2, 0x00};
const Waveform RampUp = {MODE_RAM, 4, RampUp_data};

static const unsigned char RampDown_data[] = {DRV260x_WAVEFORMSEQ1, 0x0C,
                                              DRV260x_WAVEFORMSEQ2, 0x00};
const Waveform RampDown = {MODE_RAM, 4, RampDown_data};

static const unsigned char StrongClick_OL_data[] = {
    DRV260x_OD_CLAMP,     Voltage_1p3,  0xFF,
    DRV260x_CONTROL3,     ERM_OpenLoop, 0x20,
    DRV260x_WAVEFORMSEQ1, 0x01,         0xFF,
    DRV260x_WAVEFORMSEQ2, 0x00,         0xFF};
const Waveform StrongClick_OL = {MODE_DEFAULT, 12, StrongClick_OL_data};

static const unsigned char ClickBounce_data[] = {
    DRV260x_WAVEFORMSEQ1, 0x01, DRV260x_WAVEFORMSEQ2, 0x83,
    DRV260x_WAVEFORMSEQ3, 0x0C, DRV260x_WAVEFORMSEQ4, 0x00};
const Waveform ClickBounce = {MODE_RAM, 8, ClickBounce_data};

static const unsigned char TransitionClick_data[] = {
    DRV260x_WAVEFORMSEQ1, 0x0B, DRV260x_WAVEFORMSEQ2, 0x83,
    DRV260x_WAVEFORMSEQ3, 0x01, DRV260x_WAVEFORMSEQ4, 0x00};
const Waveform TransitionClick = {MODE_RAM, 8, TransitionClick_data};

static const unsigned char GallopAlert_data[] = {DRV260x_WAVEFORMSEQ1, 0x0D,
                                                 DRV260x_WAVEFORMSEQ2, 0x00};
const Waveform GallopAlert = {MODE_RAM, 4, GallopAlert_data};

static const unsigned char PulsingAlert_data[] = {DRV260x_WAVEFORMSEQ1, 0x0E,
                                                  DRV260x_WAVEFORMSEQ2, 0x00};
const Waveform PulsingAlert = {MODE_RAM, 4, PulsingAlert_data};

static const unsigned char TestClick_data[] = {DRV260x_WAVEFORMSEQ1, 0x0F,
                                               DRV260x_WAVEFORMSEQ2, 0x00};
const Waveform TestClick = {MODE_RAM, 4, TestClick_data};

static const unsigned char TestBuzz_data[] = {DRV260x_WAVEFORMSEQ1, 0x10,
                                              DRV260x_WAVEFORMSEQ2, 0x00};
const Waveform TestBuzz = {MODE_RAM, 4, TestBuzz_data};

static const unsigned char LifeTestBuzz_data[] = {DRV260x_WAVEFORMSEQ1, 0x11,
                                                  DRV260x_WAVEFORMSEQ2, 0x00};
const Waveform LifeTestBuzz = {MODE_RAM, 4, LifeTestBuzz_data};

static const unsigned char LifeTestContinuousBuzz_data[] = {
    DRV260x_WAVEFORMSEQ1, 0x12, DRV260x_WAVEFORMSEQ2, 0x00};
const Waveform LifeTestContinuousBuzz = {MODE_RAM, 4,
                                         LifeTestContinuousBuzz_data};

static const unsigned char MsgExtraShort_data[] = {DRV260x_WAVEFORMSEQ1, 0x13,
                                              DRV260x_WAVEFORMSEQ2, 0x00};
const Waveform MsgExtraShort = {MODE_RAM, 4, MsgExtraShort_data};

static const unsigned char MsgShort_data[] = {DRV260x_WAVEFORMSEQ1, 0x14,
                                              DRV260x_WAVEFORMSEQ2, 0x00};
const Waveform MsgShort = {MODE_RAM, 4, MsgShort_data};

static const unsigned char MsgMedium_data[] = {DRV260x_WAVEFORMSEQ1, 0x15,
                                              DRV260x_WAVEFORMSEQ2, 0x00};
const Waveform MsgMedium = {MODE_RAM, 4, MsgMedium_data};

static const unsigned char MsgLong_data[] = {DRV260x_WAVEFORMSEQ1, 0x16,
                                             DRV260x_WAVEFORMSEQ2, 0x00};
const Waveform MsgLong = {MODE_RAM, 4, MsgLong_data};

static const unsigned char MsgExtraLong_data[] = {DRV260x_WAVEFORMSEQ1, 0x17,
                                             DRV260x_WAVEFORMSEQ2, 0x00};
const Waveform MsgExtraLong = {MODE_RAM, 4, MsgExtraLong_data};

static const unsigned char MsgReallyLong_data[] = {DRV260x_WAVEFORMSEQ1, 0x18,
                                             DRV260x_WAVEFORMSEQ2, 0x00};
const Waveform MsgReallyLong = {MODE_RAM, 4, MsgReallyLong_data};

//--------------------------------------------------------//
// PWM Mode Examples
//--------------------------------------------------------//
static const unsigned char PWM_Click_data[] = {0xFE, 0x06, 0x01, 0x08};
const Waveform PWM_Click = {MODE_PWM, 4, PWM_Click_data};

static const unsigned char PWM_Alert_NoBrake_data[] = {0xFE, 0x96};
const Waveform PWM_Alert_NoBrake = {MODE_PWM, 2, PWM_Alert_NoBrake_data};

static const unsigned char PWM_Alert_data[] = {0xFE, 0x96, 0x00, 0x05};
const Waveform PWM_Alert = {MODE_PWM, 4, PWM_Alert_data};

//--------------------------------------------------------//
// RTP Mode Examples
//--------------------------------------------------------//
static const unsigned char RTP_Click_data[] = {0x7F, 0x08, 0x00, 0x08};
const Waveform RTP_Click = {MODE_RTP, 4, RTP_Click_data};

static const unsigned char RTP_Tick_data[] = {0x7F, 0x02, 0x00, 0x08};
const Waveform RTP_Tick = {MODE_RTP, 4, RTP_Tick_data};

static const unsigned char RTP_Alert_data[] = {0x7F, 0x96, 0x00, 0x05};
const Waveform RTP_Alert = {MODE_RTP, 4, RTP_Alert_data};

static const unsigned char RTP_Alert_NoBrake_data[] = {0x7F, 0x96};
const Waveform RTP_Alert_NoBrake = {MODE_RTP, 2, RTP_Alert_NoBrake_data};

//--------------------------------------------------------//
// Analog Mode
//--------------------------------------------------------//
static const unsigned char Analog_AC_data[] = {
    DRV260x_CONTROL1, AC_Couple,        0x20,
    DRV260x_CONTROL3, InputMode_ANALOG, 0x02,
    DRV260x_MODE,     PWM_Analog,       0xFF};
const Waveform Analog_AC = {MODE_ANALOG, 9, Analog_AC_data};

static const unsigned char Analog_DC_data[] = {
    DRV260x_CONTROL1, DC_Couple,        0x20,
    DRV260x_CONTROL3, InputMode_ANALOG, 0x02,
    DRV260x_MODE,     PWM_Analog,       0xFF};
const Waveform Analog_DC = {MODE_ANALOG, 9, Analog_DC_data};

//--------------------------------------------------------//
// Audio2Haptics Mode
//--------------------------------------------------------//
static const unsigned char A2H_AC_data[] = {
    DRV260x_CONTROL1, AC_Couple,          0x20,
    DRV260x_CONTROL3, (InputMode_ANALOG), 0x22,
    DRV260x_MODE,     Audio2Haptics,      0xFF};
const Waveform A2H_AC = {MODE_A2H, 9, A2H_AC_data};

static const unsigned char A2H_DC_data[] = {
    DRV260x_CONTROL1, DC_Couple,          0x20,
    DRV260x_CONTROL3, (InputMode_ANALOG), 0x22,
    DRV260x_MODE,     Audio2Haptics,      0xFF};
const Waveform A2H_DC = {MODE_A2H, 9, A2H_DC_data};
