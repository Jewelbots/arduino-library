/******************************************************************************
 * Actuator_Waveforms.h
 *
 * Created on: April 23, 2012
 * Board: DRV2604EVK-CT
 *
 * Desc: This file contains the waveform data arrays.  See instructions in
 * 			Actuator_Waveforms.c to create new waveforms.
 *
 ******************************************************************************/

#include "Haptics.h"

//--------------------------------------------------------//
// RAM Playback Examples
//--------------------------------------------------------//
extern const Waveform StrongClick;
extern const Waveform MediumClick;
extern const Waveform LightClick;
extern const Waveform Tick;
extern const Waveform Bump;
extern const Waveform DoubleStrongClick;
extern const Waveform DoubleStrongBump;
extern const Waveform DoubleMediumClick;
extern const Waveform DoubleLightClick;
extern const Waveform TripleStrongClick;
extern const Waveform BuzzAlert;
extern const Waveform RampUp;
extern const Waveform RampDown;
extern const Waveform StrongClick_OL;
extern const Waveform ClickBounce;
extern const Waveform TransitionClick;
extern const Waveform GallopAlert;
extern const Waveform PulsingAlert;
extern const Waveform TestClick;
extern const Waveform TestBuzz;
extern const Waveform LifeTestBuzz;
extern const Waveform LifeTestContinuousBuzz;
extern const Waveform MsgExtraShort;
extern const Waveform MsgShort;
extern const Waveform MsgMedium;
extern const Waveform MsgLong;
extern const Waveform MsgExtraLong;
extern const Waveform MsgReallyLong;
//--------------------------------------------------------//
// PWM Mode Examples
//--------------------------------------------------------//
extern const Waveform PWM_Click;
extern const Waveform PWM_Alert;
extern const Waveform PWM_Alert_NoBrake;

//--------------------------------------------------------//
// RTP Mode Examples
//--------------------------------------------------------//
extern const Waveform RTP_Click;
extern const Waveform RTP_Tick;
extern const Waveform RTP_Alert;

//--------------------------------------------------------//
// Analog Mode
//--------------------------------------------------------//
extern const Waveform Analog_DC;
extern const Waveform Analog_AC;

//--------------------------------------------------------//
// Audio-to-Haptics Mode
//--------------------------------------------------------//
extern const Waveform A2H_DC;
extern const Waveform A2H_AC;
