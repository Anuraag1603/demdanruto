// ----------------------------------------
// Filename:     DEM.h
// Description:
//  The Digital Energy meter library containing the cost, power and energy usage
//
// Author: Danny Sok
// Date:   14-11-2011
// Date LM: 14-11-2011
#ifndef DEM_H
#define DEM_H

#include "EEPROM.h"
#include "clock.h"
#include "types.h"
#include "LCD.h"

#define DEM_PWRSIZE 16
#define MAX_COST    7999992 //999999*8
//#define MAX_ENERGY  7992000 //999000*8
//#define MAX_ENERGY  639336000 //999000*64
#define MAX_ENERGY  999000
#define MAX_DAYS    99   
#define TarrifBase  9
#define CostBase    10
#define PowerBase   6
#define EnergyBase  9

extern TUINT32 DEM_Average_Power;
extern UINT32 DEM_Power_Array[DEM_PWRSIZE];
extern TUINT32 DEM_Total_Energy;
extern UINT32 DEM_Total_Cost;
extern UINT16 DEM_Tarrif;
extern TUINT16 DEM_VRMS;
extern TUINT16 DEM_IRMS;
extern TUINT16 DEM_Frequency;

typedef struct
{
  TUINT32 Value, Old;  
}DEM_Energy;

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
void DEM_Setup(void);

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
void DEM_SetTarrif(void);

void DEM_OutTarrif(void);

#endif