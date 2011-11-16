// ----------------------------------------
// Filename:     Math.c
// Description:
//  The math library containing Q Notation and Square Root formulas.  
//
// Author: Danny Sok
// Date:   14-11-2011
// Date LM: 14-11-2011

#ifndef MATH_H
#define MATH_H

#include "DEM.h"
#define Base 3 // default base because i cbf changing shit.

typedef enum
{
  qLeft,
  qRight
} TQNotationSide;

UINT32 Math_ToQNotation(const UINT32 number, const UINT8 base);
UINT16 Math_FromQNotation(UINT32 const number, const TQNotationSide side, const BOOL base);
UINT16 Math_FindPower(const INT16 voltage, const INT16 current);
void Math_FindEnergy(const INT16 DEM_AvePower_Array[]);
void Math_FindCost(void);

UINT32 Math_SQRT(INT16 number, INT16 guess);


#endif