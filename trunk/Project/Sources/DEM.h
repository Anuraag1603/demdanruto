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

#define DEM_PWRSIZE 16
#define MAX_COST    7999992 //999999*8
//#define MAX_ENERGY  7992000 //999000*8
#define MAX_ENERGY  639336000 //999000*64
#define MAX_DAYS    99   

extern TINT16 DEM_Average_Power;
extern INT16 DEM_AvePower_Array[DEM_PWRSIZE];
extern TUINT32 DEM_Total_Energy;
extern UINT32 DEM_Total_Cost;
extern UINT16 DEM_Tarrif;
extern TUINT16 DEM_VRMS;
extern TUINT16 DEM_IRMS;
extern TUINT16 DEM_Frequency;

void DEM_Setup(void);
void DEM_SetTarrif(void);

#endif