// ----------------------------------------
// Filename: clock.h
// Description: Routines for maintaining
//   a real-time clock
// Author: PMcL
// Date: 29-Mar-06

#ifndef CLOCK_H
#define CLOCK_H

// new types
#include "types.h"
#include "CRG.h"
#include "Math.h"
#include <mc9s12a256.h>     /* derivative information */

extern UINT8 Clock_Seconds, Clock_Minutes, Clock_Hours, Clock_Days;
extern UINT32 Clock_MicroSeconds, Clock_MilliSeconds;
extern UINT32 Clock_Interval;

// ----------------------------------------
// Clock_Setup
// 
// Initializes the clock
// Input:
//   prescaleRate is the desired prescale rate and modulus count
//   of the CRG block's real-time interrupt module,
//   corresponding to Table 3.2 in the CRG Block User Guide,
//   to ensure ticks of the clock occur every 65.536 ms
// Output:
//   TRUE if the clock was setup successfully
// Conditions:
//   none

void Clock_Setup(const UINT8 prescaleRate, const UINT8 modulusCount);

// ----------------------------------------
// Clock_Update
// 
// Updates the clock by converting milliseconds
//   and microseconds into seconds and minutes
// Input:
//   none
// Output:
//   TRUE if clock seconds have changed
// Conditions:
//   Assumes that the clock has been set up

BOOL Clock_Update(void);

//UINT16 Clock_RunningTimeInHours(void);

//UINT16 Clock_TimeInSeconds(void);

#endif