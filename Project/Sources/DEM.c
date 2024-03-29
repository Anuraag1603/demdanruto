// ----------------------------------------
// Filename:     DEM.c
// Description:
//  The Digital Energy meter library containing the cost, power and energy usage
//
// Author: Danny Sok
// Date:   14-11-2011
// Date LM: 19-11-2011

#include "DEM.h"

// ----------------------------------------
// DEM_Setup
// 
// Sets up the variables for the DEM
// Input:
//   none
// Output:
//   none
// Conditions:
//   none
void DEM_Setup(void)
{
  UINT8 i;
  
  DEM_Average_Power.l = 0;
  DEM_Total_Energy.l  = 0;
  DEM_Total_Cost      = 0;
  DEM_VRMS.l          = 0;
  DEM_IRMS.l          = 0;
  
  
  for (i = 0; i < DEM_PWRSIZE; i++)
  {
    DEM_Power_Array[i] = 0;
  }
}

// ----------------------------------------
// DEM_SetTarrif
// 
// Updates the tarrif variable to the correct one
// based on the time
// Input:
//   none
// Output:
//   none
// Conditions:
//   none
void DEM_SetTarrif(void)
{
  /*
  Peak 14-20
  Shoulder 7-14
  Off Peak every
  */
  
  // Time of Use
  if (DEM_Average_Power.l > 0 && sTarrifMode == 1)
  {
    
    if (Clock_Hours >= 14 && Clock_Hours <= 20)
      DEM_Tarrif = sT1Peak;  
    else if (Clock_Hours >= 7 && Clock_Hours < 14)
      DEM_Tarrif = sT1Shoulder;
    else
      DEM_Tarrif = sT1OffPeak;
  }
  else if (DEM_Average_Power.l == 0 && sTarrifMode == 2)
    DEM_Tarrif = sT2NonTOU;
  else if (DEM_Average_Power.l == 0 && sTarrifMode == 3)
    DEM_Tarrif = sT3NonTOU;
  else 
    DEM_Tarrif = 0;
}

void DEM_OutTarrif(void)
{
  UINT16 tarrifL = DEM_Tarrif >> TarrifBase;
  UINT16 tarrifR = DEM_Tarrif * 1000 >> TarrifBase;
  tarrifR %= 1000;
  
  LCD_OutInteger(tarrifL);
  (void)LCD_OutChar('.');
  LCD_OutInteger(tarrifR);
  
  
}