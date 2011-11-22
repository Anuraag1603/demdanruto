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

// 2.3VRMS
const static TUINT16 Analog_OutVoltage[] = 
{
  2047,
  2128,
  2210,
  2291,
  2371,
  2451,
  2529,
  2606,
  2683,
  2757,
  2830,
  2901,
  2970,
  3036,
  3101,
  3162,
  3221,
  3278,
  3331,
  3381,
  3428,
  3471,
  3512,
  3548,
  3581,
  3611,
  3636,
  3658,
  3676,
  3690,
  3700,
  3706,
  3708,
  3706,
  3700,
  3690,
  3676,
  3658,
  3636,
  3611,
  3581,
  3548,
  3512,
  3471,
  3428,
  3381,
  3331,
  3278,
  3221,
  3162,
  3101,
  3036,
  2970,
  2901,
  2830,
  2757,
  2683,
  2606,
  2529,
  2451,
  2371,
  2291,
  2210,
  2128,
  2047,
  1966,
  1884,
  1803,
  1723,
  1643,
  1565,
  1488,
  1411,
  1337,
  1264,
  1193,
  1124,
  1058,
  993,
  932,
  873,
  816,
  763,
  713,
  666,
  623,
  582,
  546,
  513,
  483,
  458,
  436,
  418,
  404,
  394,
  388,
  386,
  388,
  394,
  404,
  418,
  436,
  458,
  483,
  513,
  546,
  582,
  623,
  666,
  713,
  763,
  816,
  873,
  932,
  993,
  1058,
  1124,
  1193,
  1264,
  1337,
  1411,
  1488,
  1565,
  1643,
  1723,
  1803,
  1884,
  1966
};

// 1VRMS
const static TUINT16 Analog_OutCurrent[] = 
{
  2047,
  2118,
  2188,
  2259,
  2328,
  2397,
  2466,
  2533,
  2599,
  2664 ,
  2727 ,
  2789 ,
  2848 ,
  2906 ,
  2962 ,
  3016 ,
  3067 ,
  3116 ,
  3162 ,
  3206 ,
  3246,
  3284,
  3319,
  3351,
  3380,
  3405,
  3427,
  3446,
  3462,
  3474,
  3482,
  3488,
  3489,
  3488,
  3482,
  3474,
  3462,
  3446,
  3427,
  3405,
  3380,
  3351,
  3319,
  3284,
  3246,
  3206,
  3162,
  3116,
  3067,
  3016,
  2962,
  2906 ,
  2848,
  2789,
  2727,
  2664,
  2599,
  2533,
  2466,
  2397,
  2328,
  2259,
  2188,
  2118 ,
  2047 ,
  1976,
  1906,
  1835,
  1766,
  1697,
  1628,
  1561,
  1495,
  1430,
  1367,
  1305,
  1246,
  1188,
  1132,
  1078,
  1027,
  978 ,
  932 ,
  888 ,
  848 ,
  810 ,
  775 ,
  743 ,
  714 ,
  689 ,
  667 ,
  648 ,
  632 ,
  620  ,
  612  ,
  605 ,
  606 ,
  612 ,
  620 ,
  632 ,
  648 ,
  667 ,
  689 ,
  714 ,
  743 ,
  775 ,
  810 ,
  848  ,
  888  ,
  932  ,
  978  ,
  1027 ,
  1078,
  1132,
  1188,
  1246,
  1305,
  1367,
  1430,
  1495,
  1561,
  1628,
  1697,
  1766,
  1835,
  1906,
  1976
};

// No amplitude variation
const static TUINT16 DACLUT[] =
{
  2047,
  2147,
  2248,
  2347,
  2446,
  2544,
  2641,
  2737,
  2830,
  2922,
  3012,
  3099,
  3184,
  3266,
  3346,
  3422,
  3494,
  3564,
  3629,
  3691,
  3749,
  3803,
  3852,
  3897,
  3938,
  3974,
  4006,
  4033,
  4055,
  4072,
  4084,
  4092,
  4094,
  4092,
  4084,
  4072,
  4055,
  4033,
  4006,
  3974,
  3938,
  3897,
  3852,
  3803,
  3749,
  3691,
  3629,
  3564,
  3494,
  3422,
  3346,
  3266,
  3184,
  3099,
  3012,
  2922,
  2830,
  2737,
  2641,
  2544,
  2446,
  2347,
  2248,
  2147,
  2047,
  1947,
  1846,
  1747,
  1648,
  1550,
  1453,
  1357,
  1264,
  1172,
  1082,
  995 ,
  910 ,
  828 ,
  748 ,
  672 ,
  600 ,
  530 ,
  465 ,
  403 ,
  345 ,
  291 ,
  242 ,
  197 ,
  156 ,
  120 ,
  88  ,
  61  ,
  39  ,
  22  ,
  10  ,
  2   ,
  0   ,
  2   ,
  10  ,
  22  ,
  39  ,
  61  ,
  88  ,
  120 ,
  156 ,
  197 ,
  242 ,
  291 ,
  345 ,
  403 ,
  465 ,
  530 ,
  600 ,
  672 ,
  748 ,
  828 ,
  910 ,
  995 ,
  1082,
  1172,
  1264,
  1357,
  1453,
  1550,
  1648,
  1747,
  1846,
  1947
      
};    

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