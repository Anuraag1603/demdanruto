//DEM.h
#ifndef DEM_H
#define DEM_H

#include "EEPROM.h"
#include "clock.h"

#define DEM_PWRSIZE 16
#define MAX_COST    7999992 //999999*8
#define MAX_ENERGY  7992000 //999000*8
#define MAX_DAYS    99   

extern TINT16 DEM_Average_Power;
extern INT16 DEM_AvePower_Array[DEM_PWRSIZE];
extern TUINT32 DEM_Total_Energy;
extern UINT32 DEM_Total_Cost;
extern UINT16 DEM_Tarrif;

void DEM_SetTarrif(void);

#endif