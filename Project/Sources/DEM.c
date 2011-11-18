// ----------------------------------------
// Filename:     DEM.c
// Description:
//  The Digital Energy meter library containing the cost, power and energy usage
//
// Author: Danny Sok
// Date:   14-11-2011
// Date LM: 14-11-2011

#include "DEM.h"

void DEM_SetTarrif(void)
{
  /*
  Peak 14-20
  Shoulder 7-14
  Off Peak every
  */
  
  // Time of Use
  if (DEM_Average_Power.l > 0)
  {
    
    if (Clock_Hours >= 14 && Clock_Hours <= 20)
      DEM_Tarrif = sT1Peak;  
    else if (Clock_Hours >= 7 && Clock_Hours < 14)
      DEM_Tarrif = sT1Shoulder;
    else
      DEM_Tarrif = sT1OffPeak;
  }
  else
    DEM_Tarrif = sT2NonTOU;
}