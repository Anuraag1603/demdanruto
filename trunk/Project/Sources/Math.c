// ----------------------------------------
// Filename:     Math.c
// Description:
//  The math library containing Q Notation and Square Root formulas.  
//
// Author: Danny Sok
// Date:   14-11-2011
// Date LM: 16-11-2011

#include "types.h"
#include "Math.h"


// *8 ~= << 3
// /8 ~= >> 3



// ----------------------------------------
// Math_ToQN
// 
// Converts the given number to Q Notation based on a base given
// Input:
//  number is the number to use for Q Notation conversion
//  base is the n in mQn.
// Output:
//  The QNotated form of the number
// Conditions:
//  none
INT32 Math_ToQN(const UINT32 number, const UINT8 base)
{
  return number << base;
}

// ----------------------------------------
// Math_FromQN
// 
// Determines the total energy usage
// Input:
//  number is the number in Q Notation form
//  side is the side of the Q Notation where m is on the left, and n is on the right. (mQn)
// Output:
//  The number on the side. Useful for display on the LCD.
// Conditions:
//  none
INT16 Math_FromQN(UINT32 const number, const TQNotationSide side, const UINT8 base)
{
  INT32 tempNumber;
  //INT8 hundredThousand, tenThousand, thousand = 0;
  tempNumber = (INT32)(number >> base);
  
  switch(side)
  {
    case qLeft:
      return (INT16) (tempNumber / 1000);   // Change base back to 2
    break;
    case qRight:
      /*if (tempNumber >= 100000)
      {
        hundredThousand = (UINT8)(number / 100000);
        tempNumber -= (hundredThousand * 100000);  
      }
      if (tempNumber >= 10000)
      {
        tenThousand = (UINT8)(tempNumber / 10000);
        tempNumber -= (tenThousand * 10000);
      }
      if (tempNumber >= 1000)
      {
        thousand = (UINT8)(tempNumber / 1000);
        tempNumber -= (thousand * 1000);
      }*/
      return (INT16)tempNumber % 1000;              // Only take the last 3 digits as the base is 1000
    break;
    
  }
}

INT16 Math_ConvertADCValue(const INT16 number)
{ 
  /* 
    5/4095 = 0.00122
    122mv	1    122
		5000  4095 5000

    5000*N/4095
    61*N+25/50             
    where N = ADC Number    
  */
  return ((61 * number) + 25 ) / 50;
}

INT16 Math_ToDACValue(const INT32 number) 
{
  // 4096 * Vin / Vref
  return (INT16) (( (number << 13) + 3) / 5);
}


// ----------------------------------------
// Math_FindPower
// 
// Determines the power usage
// Input:
//  voltage is the analog representation of voltage e.g. 5 Volts
//  current is the analog representation of current e.g. 1 Amp
// Output:
//  The instantaneous power
// Conditions:
//  none
UINT32 Math_FindPower(const INT16 voltage, const INT16 current)
{
  // Will come in as mV
  return (voltage * current);
}

// ----------------------------------------
// Math_FindEnergy
// 
// Determines the total energy usage
// Input:
//  DEM_AvePower_Array is the array for the 16 samples of power
// Output:
//  none
// Conditions:
//  none
void Math_FindEnergy(const UINT16 sample)
{
  // 16 Samples per Period/Cycle.
  // Sum of PkTs between k=0 and n
  // n = 15 then.
  // 1 cycle is approx 20 milliseconds
  // so 20/16 = 1.25, so sample approx every 1ms
  UINT8 i;
  UINT16 energy = 0;
  
  for (i = 0; i < DEM_PWRSIZE; i++)
  {
    // Power is in 1000 p.u
    // Sample is in ms in 1000 p.u. Energy is?
    energy += DEM_Power_Array[i] * sample;
    //DEM_Total_Energy.l += powerArr[i];
  }
  // Power is in milliWatt? (mV * mV)
  DEM_Total_Energy.l += energy;
  DEM_Total_Energy.l *= 1000 * 3600; // Wh in base 1000, need to div by 1 000 000 to get base 0
  DEM_Total_Energy.l /= 1000000;     // Energy in Wh
  //DEM_Total_Energy.l = (DEM_Total_Energy.l) * (Clock_RunningTimeInHours()) ;
  //DEM_Total_Energy.l = DEM_Total_Energy.l;
}

// ----------------------------------------
// Math_FindCost
// 
// Determines the total cost of the user
// Input:
//  none
// Output:
//  none
// Conditions:
//  none
void Math_FindCost(void)
{
  // Energy in kWh * Tarrif in cents...
  // So energy in Watt-second * Tarrif in cents?
  // Energy is mind fuck base, DEM_Tarrif is 256 p.u.
  UINT32 cost =  (DEM_Total_Energy.l) * (DEM_Tarrif);
  DEM_Total_Cost += cost;
}

// ----------------------------------------
// Math_SQRT
// 
// Determines the square roots by using an initial guess
// Input:
//  number is the number to find the root of
//  guess is the initial estimation of the root
// Output:
//  none
// Conditions:
//  none
UINT16 Math_SQRT(const INT16 number, const INT16 guess)
{
  return (UINT16) ( ((number + guess) + guess) >> 1);
}

void Math_FindFrequency(const UINT16 delay)
{
  // F = 1/T
  // delay in ms. to get in seconds div by 1000
  DEM_Frequency.l = (1000000 / (delay)) * 10;
    
}

UINT16 Math_FindRMS(const INT16 number)
{
  return number / Math_SQRT(2, 1);
}

UINT16 Math_SampleTime(const UINT16 delay)
{
  return (delay) / 16;   // 16 samples 
}