/******************************************************************************
 * WaveformData.h
 *
 * Created on: March 9, 2012
 * Board: DRV2604EVM-CT, DRV2605EVM-CT
 *
 * Desc: This file contains the data for use with the Actuator_Waveforms.h
 *
 ******************************************************************************/
#ifndef WAVEFORM_DATA_H
#define WAVEFORM_DATA_H
#define DRV2604_HEADERSIZE 62
#define DRV2604_DATASIZE 71

// Header Info
extern const unsigned char DRV2604_HEADER[]; // DRV2667 RAM Header Information

// Data Info
extern const unsigned char DRV2604_DATA[]; // DRV2667 RAM Waveform Data
#endif
