// ----------------------------------------
// Filename: analog.h
// Description: Routines for setting up and
//   reading from the ADC/DAC
// Author: PMcL
// Date: 14-Feb-07

#ifndef ANALOG_H
#define ANALOG_H

// new types
#include "types.h"
#include "SPI.h"

// Maximum number of channels
#define NB_INPUT_CHANNELS 8
#define NB_OUTPUT_CHANNELS 4

// Number of channels used
extern TUINT16 NbAnalogInputs;
extern TUINT16 NbAnalogOutputs;

typedef enum
{
  Ch1, Ch2, Ch3, Ch4, Ch5, Ch6, Ch7, Ch8
} TChannelNb;

typedef struct
{
  TINT16 Value, OldValue;
  INT16 Value1, Value2, Value3;		// variables for median filtering
} TAnalogInput;

extern TAnalogInput Analog_Input[NB_INPUT_CHANNELS];

// ----------------------------------------
// Analog_Setup
// 
// Sets up the ADC and DAC
// Input:
//   none
// Output:
//   none
// Conditions:
//   none

void Analog_Setup(const UINT32 busClk);

// ----------------------------------------
// PWM_Setup
// 
// Sets up the Pulse Width Modulators for use with the DAC (missing LPFilter)
// Input:
//   busClk is the bus clock speed
// Output:
//   none
// Conditions:
//   none
void PWM_Setup(const UINT32 busClk);

// ----------------------------------------
// Analog_Get
// 
// Gets an analog input channel's value based on the mode
// Input:
//   channelNb is the number of the anlog input channel to read
// Output:
//   none
// Conditions:
//   Assumes that the ADC has been set up

void Analog_Get(const TChannelNb channelNb);

// ----------------------------------------
// Analog_Put
// 
// Places a PWM cycle into the ADC for output
// Input:
//   channelNb is the number of the anlog input channel to output to
// Output:
//   none
// Conditions:
//   Assumes that the DAC has been set up
void Analog_Put(const TChannelNb channelNb);

#endif