// ----------------------------------------
// Filename: EEPROM.h
// Description: Routines for erasing and
//   writing to the EEPROM
// Author: PMcL
// Date: 16-Mar-06

#ifndef EEPROM_H
#define EEPROM_H

// new types
#include "types.h"

// EEPROM data access
#define _EB(EEPROM_ADDRESS)	  *(UINT8 volatile *)(EEPROM_ADDRESS)
#define _EI(EEPROM_ADDRESS)	  *(INT16 volatile *)(EEPROM_ADDRESS)
#define _EW(EEPROM_ADDRESS)	  *(UINT16 volatile *)(EEPROM_ADDRESS)
#define _EL(EEPROM_ADDRESS)	  *(INT32 volatile *)(EEPROM_ADDRESS)
#define _ES(EEPROM_ADDRESS)	  *(UINT32 volatile *)(EEPROM_ADDRESS)

// ----------------------------------------
// EEPROM addresses
// ----------------------------------------

// ModCon parameters
#define    sModConNb        _EW(0x400)
#define    sModConMode      _EW(0x402)
#define    sControlMode     _EW(0x404)          
#define    sNbAnalogInputs  _EW(0x406)
#define    sNbAnalogOutputs _EW(0x408)

// Time periods
#define    sControlPeriod   _EW(0x410)
#define    sPacketPeriod    _EW(0x412)
#define    sHealthPeriod    _EW(0x414)
#define    sAnalogPeriod    _EW(0x416)

// Debug flag
#define    Debug            _EW(0x420)

// EEPROM erased flag
#define    EEPROMErasedFlag _EW(0x430)

// LCD settings
#define    sLCDContrast     _EW(0x440)
                            
// Tarrifs
// :1
#define sT1Peak             _EW(0x450)
#define sT1Shoulder         _EW(0x452)
#define sT1OffPeak          _EW(0x454)
// :2
#define sT2NonTOU           _EW(0x456)
// :3
#define sT3NonTOU           _EW(0x458)

// Control reference
#define    sRefLo           _EW(0x500)
#define    sRefHi           _EW(0x502)

// Basic control constants
#define    sb0              _EI(0x510)
#define    sb1              _EI(0x512)
#define    sa1              _EI(0x514)
#define    sd0              _EI(0x516)
#define    sd1              _EI(0x518)
#define    sc1              _EI(0x51a)

// Advanced control constants
#define    sCP0Lo           _EW(0x520)
#define    sCP0Hi           _EW(0x522)
#define    sCP1Lo           _EW(0x524)
#define    sCP1Hi           _EW(0x526)
#define    sCP2Lo           _EW(0x528)
#define    sCP2Hi           _EW(0x52a)
#define    sCP3Lo           _EW(0x52c)
#define    sCP3Hi           _EW(0x52e)
#define    sCP4Lo           _EW(0x530)
#define    sCP4Hi           _EW(0x532)
#define    sCP5Lo           _EW(0x534)
#define    sCP5Hi           _EW(0x536)
#define    sCP6Lo           _EW(0x538)
#define    sCP6Hi           _EW(0x53a)
#define    sCP7Lo           _EW(0x53c)
#define    sCP7Hi           _EW(0x53e)



// ATD options - 1 byte for each channel
#define    ATDMode(i)       _EB(0x600 + i)
// ATD minimum value - 1 integer for each channel
#define    ATDMin(i)		    _EI(0x610 + (2 * i))
// ATD maximum value - 1 integer for each channel
#define    ATDMax(i)		    _EI(0x620 + (2 * i))

// Flash programmed flag - used by bootloader
#define    FlashProgFlag    _EW(0x7fe)

// ----------------------------------------
// EEPROM_Setup
// 
// Sets up the EEPROM with the correct internal clock
// Input:
//  oscClk is the oscillator clock frequency in Hz
// Output:
//   none
// Conditions:
//   none

BOOL EEPROM_Setup(const UINT32 oscClk, const UINT32 busClk);
 
// ----------------------------------------
// EEPROM_Write32
// 
// Writes a 32-bit number to EEPROM
// Input:
//   address is the address of the data,
//   data is the data to write
// Output:
//   TRUE if EEPROM was written successfully
//   FALSE if address is not aligned to a 4-byte boundary
//   or if there is a programming error
// Conditions:
//   Assumes EEPROM has been initialized

BOOL EEPROM_Write32(UINT32 volatile * const address, const UINT32 data);
 
// ----------------------------------------
// EEPROM_Write16
// 
// Writes a 16-bit number to EEPROM
// Input:
//   address is the address of the data,
//   data is the data to write
// Output:
//   TRUE if EEPROM was written successfully
//   FALSE if address is not aligned to a 2-byte boundary
//   or if there is a programming error
// Conditions:
//   Assumes EEPROM has been initialized

BOOL EEPROM_Write16(UINT16 volatile * const address, const UINT16 data);

// ----------------------------------------
// EEPROM_Write8
// 
// Writes an 8-bit number to EEPROM
// Input:
//   address is the address of the data,
//   data is the data to write
// Output:
//   TRUE if EEPROM was written successfully
//   FALSE if there is a programming error
// Conditions:
//   Assumes EEPROM has been initialized

BOOL EEPROM_Write8(UINT8 volatile * const address, const UINT8 data);

// ----------------------------------------
// EEPROM_Erase
// 
// Erases the entire EEPROM
// Input:
//   none
// Output:
//   TRUE if EEPROM was erased successfully
// Conditions:
//   Assumes EEPROM has been initialized

BOOL EEPROM_Erase(void);

BOOL Write(UINT8 const command, UINT16 volatile * const address, const UINT16 data);

#endif