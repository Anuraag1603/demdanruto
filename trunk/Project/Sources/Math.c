#include "types.h"
#include "Math.h"


// *8 ~= << 3
// /8 ~= >> 3

UINT32 Math_ToQNotation(const UINT32 number, const UINT8 base)
{
  return number << base;
}

UINT16 Math_FromQNotation(UINT32 const number, const TQNotationSide side)
{
  UINT32 tempNumber;
  UINT8 hundredThousand, tenThousand, thousand;
  hundredThousand, tenThousand, thousand = 0;
  tempNumber = (UINT32)(number >> Base);
  
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

UINT16 Math_FindPower(const INT16 voltage, const INT16 current)
{
  return voltage * current;
}

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

UINT16 Math_FindCost(void)
{
  // Energy in kWh * Tarrif
  return (UINT16) ( DEM_Total_Energy.l * Clock_TimeInHours() ) * DEM_Tarrif;
  //return 0;
}

UINT32 Math_SQRT(INT16 number, INT16 guess)
{
  return 0;
}
