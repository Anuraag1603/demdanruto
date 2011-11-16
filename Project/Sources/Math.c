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
// Math_ToQNotation
// 
// Converts the given number to Q Notation based on a base given
// Input:
//  number is the number to use for Q Notation conversion
//  base is the n in mQn.
// Output:
//  The QNotated form of the number
// Conditions:
//  none
UINT32 Math_ToQNotation(const UINT32 number, const UINT8 base)
{
  return number << base;
}

// ----------------------------------------
// Math_FromQNotation
// 
// Determines the total energy usage
// Input:
//  number is the number in Q Notation form
//  side is the side of the Q Notation where m is on the left, and n is on the right. (mQn)
// Output:
//  The number on the side. Useful for display on the LCD.
// Conditions:
//  none
UINT16 Math_FromQNotation(UINT32 const number, const TQNotationSide side, const BOOL base)
{
  UINT32 tempNumber;
  UINT8 hundredThousand, tenThousand, thousand;
  hundredThousand, tenThousand, thousand = 0;
  
  if (base)
    tempNumber = (UINT32)(number >> Base);
  else
    tempNumber = number;
  
  switch(side)
  {
    case qLeft:
      return (UINT16) (tempNumber / 1000);   // Change base back to 2
    break;
    case qRight:
      if (tempNumber >= 100000)
      {
        hundredThousand = (UINT8)(number / 1000);
        tempNumber -= (hundredThousand * 1000);  
      }
      if (tempNumber >= 10000)
      {
        tenThousand = (UINT8)(tempNumber / 1000);
        tempNumber -= (tenThousand * 1000);
      }
      if (tempNumber >= 1000)
      {
        thousand = (UINT8)(tempNumber / 1000);
        tempNumber -= (thousand * 1000);
      }
      return (UINT16)tempNumber;              // Only take the last 3 digits as the base is 1000
    break;
    
  }
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
UINT16 Math_FindPower(const INT16 voltage, const INT16 current)
{
  return voltage * current;
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
void Math_FindEnergy(const INT16 DEM_AvePower_Array[])
{
  // 16 Samples per Period/Cycle.
  // Sum of PkTs between k=0 and n
  // n = 15 then.
  // 1 cycle is approx 20 milliseconds
  // so 20/16 = 1.25, so sample approx every 1ms
  UINT8 i;
  
  for (i = 0; i < 15; i++)
  {
    DEM_Total_Energy.l += DEM_AvePower_Array[i];
  }
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
  // Energy in kWh * Tarrif
  DEM_Total_Cost += (UINT16) ( DEM_Total_Energy.l * Clock_TimeInHours() ) * DEM_Tarrif;
  //return 0;
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
UINT32 Math_SQRT(INT16 number, INT16 guess)
{
  return 0;
}
