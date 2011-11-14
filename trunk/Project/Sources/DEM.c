#include "DEM.h"

void DEM_SetTarrif(void)
{
  /*
  Peak 14-20
  Shoulder 7-14
  Off Peak every
  */
  
  // Time of Use
  if (DEM_Average_Power.l >= 1)
  {
    
    if (Clock_Hours >= 14 && Clock_Hours <= 20)
      DEM_Tarrif = sT1Peak;  
    else if (Clock_Hours >= 7 && Clock_Hours <= 14)
      DEM_Tarrif = sT1Shoulder;
    else
      DEM_Tarrif = sT1OffPeak;
  }
  else
  {
    DEM_Tarrif = sT2NonTOU;
  }
}