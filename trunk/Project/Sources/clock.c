// ----------------------------------------
// Filename: clock.c
// Description: Routines for maintaining
//   a real-time clock
// Author: PMcL
//         Danny Sok
//         Edward Khou
// Date Created: 12-09-2011
// Date Last Modified: 16-09-2011

// new types
#include "clock.h"




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

void Clock_Setup(const UINT8 prescaleRate, const UINT8 modulusCount)
{
  // 2^16 = 63536. So RTR[6:4] (Prescale) = 111 and RTR[3:0] (Mod) = 0000.
  Clock_Minutes = 0;
  Clock_Seconds = 0;
  CRG_SetupRTI(prescaleRate, modulusCount);
}

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

BOOL Clock_Update(void)
{
  UINT32 second;
  UINT8  minute;
  UINT8  hour;
  UINT8  day;
  
  second = 1000000;              // In microSeconds
  minute = 60;
  hour   = 60;
  day    = 24;
  
  // Save current state to allow us to modify these sections
  EnterCritical();
   
  // Every half second flash the LED
  if (Clock_MicroSeconds >= second/2)
    PORTE_BIT7 = 0;                // ON
  else
    PORTE_BIT7 = 1;                // OFF
  
  // Update the clock  
  if (Clock_MicroSeconds >= second)
  {
    if (Debug)
    {
      Clock_Hours++;
      Clock_MicroSeconds -= second;  // 1 second has passed
      
      if (Clock_Hours >= day)
      {
        Clock_Days++;
        Clock_Hours -= day;
      }
      
      ExitCritical();
      return bTRUE;
    }
    
    Clock_Seconds++;
    Clock_Interval++;
    Clock_MicroSeconds -= second;  // 1 second has passed
    
    
    
    if (Clock_Seconds >= minute)
    {
    Clock_Minutes++;               // 1 minute has passed
    Clock_Seconds -= minute;
    }
    
    if (Clock_Minutes >= hour)
    {
      Clock_Hours++;
      Clock_Minutes -= hour;
    }
       
    if (Clock_Hours >= day)
    {
      Clock_Days++;
      Clock_Hours -= day;
    }
    
    // Restore state
    ExitCritical();
    return bTRUE;
  }
  
  
  // Restore state
  ExitCritical();
  return bFALSE;  
}

UINT16 Clock_TimeInHours(void)
{
  UINT16 minutes, seconds;
  minutes = (UINT16)Math_ToQN(16, 3) * (Clock_Minutes << 3);
  minutes = minutes >> 3;
  seconds = (UINT16)Math_ToQN(27, 5) * (Clock_Seconds << 5);
  seconds = seconds >> 5;
  // Nfi how I'm gonna do this :S
  //return (Clock_Days * 24) + Clock_Hours + (Clock_Minutes * 0.0166666667)  = (Clock_Seconds * 0.000277777778);
  return (Clock_Days * 24) + Clock_Hours + minutes + seconds;
}