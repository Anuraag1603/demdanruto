// ----------------------------------------
// Filename:     main.h
// Description:
//  Calls the main routines for the DEM.    
//
// Author: Danny Sok
// Date:   3-11-2011
// Date LM: 3-11-2011

#ifndef MAIN_H
#define MAIN_H

#include <hidef.h>          /* common defines and macros */
#include <mc9s12a256.h>     /* derivative information */
		 
#pragma LINK_INFO DERIVATIVE "mc9s12a256b"

// include files here

#include "types.h"

#include "HMI.h"

// Serial Communication Interface
#include "SCI.h"

// Packet handling
#include "packet.h"

// EEPROM
#include "EEPROM.h"

// Clocks and Reset Generator
#include "CRG.h"

// Timer - ETC
#include "timer.h"

// ADC
#include "analog.h"

// SPI Connection to ADC
#include "SPI.h"

#include "DEM.h"

#include "clock.h"

// Version number
const UINT8 MAJOR_VERSION   = 1;
const UINT8 MINOR_VERSION   = 0;

// ----------------------------------------
// Packet_Commands

const UINT8 CMD_STARTUP       = 0x04;
const UINT8 CMD_EEPROM_PROG	  = 0x07;
const UINT8 CMD_EEPROM_GET	  = 0x08;
const UINT8 CMD_SPECIAL       = 0x09;
const UINT8 CMD_MODCON_NB     = 0x0b; 
const UINT8 CMD_MODCON_MODE   = 0x0d; 
const UINT8 CMD_MODCON_TIME   = 0x0c; 
const UINT8 CMD_MODCON_PMODE  = 0x0a; 
const UINT8 CMD_ANALOG_VALUE  = 0x50;
const UINT8 CMD_TEST_MODE     = 0x10;
const UINT8 CMD_TARRIF        = 0x11;
const UINT8 CMD_TIME1         = 0x12;
const UINT8 CMD_TIME2         = 0x13;
const UINT8 CMD_POWER         = 0x14;
const UINT8 CMD_ENERGY        = 0x15;
const UINT8 CMD_COST          = 0x16;
const UINT8 CMD_FREQUENCY     = 0x17;
const UINT8 CMD_VOLRMS        = 0x18;
const UINT8 CMD_CURRMS        = 0x19;
const UINT8 CMD_POWFAC        = 0x1A;

// ----------------------------------------
// Clock/rate setups
const UINT32 baudRate         = 115200;
const UINT32 busClk           = 24000000;  // 24Mhz
const UINT32 oscClk           = 16000000;  // 16Mhz
const UINT32 refClk           = 8000000;   // refClk is E_CLK normal? [busClk normal]
const UINT8  watchDogRate     = 1;         // 1ms stub for now

// ----------------------------------------
// RTI const setups
const UINT8 prescaleRate      = 7;  //0x07;
const UINT8 modulusCount      = 15; //0x0F;

// ----------------------------------------
// Timer Delays
const UINT16 Periodic_Delay   = 1000; // 1ms
const UINT16 Timer_Ch7_Delay  = 2000;  // We need enough delay for 10 bits.

// ----------------------------------------
// Command setups
const UINT8 copReset = 0x55;
const UINT8 copEnd   = 0xAA;

// enumerated types here
// ----------------------------------------
// Packet setup
TPacket Packet;
BOOL Packet_CommandOK;
TPacketMode Packet_Mode;

// Analog Setup
TAnalogInput Analog_Input[NB_INPUT_CHANNELS];
TUINT16 NbAnalogInputs;
TUINT16 NbAnalogOutputs;

UINT8 Clock_Seconds;
UINT8 Clock_Minutes;
UINT8 Clock_Hours;
UINT8 Clock_Days;
UINT32 Clock_MicroSeconds;



// ----------------------------------------
//  Function Prototypes
void main(void);
void HandlePacket(void);
BOOL HandleStartupPacket(void);
BOOL HandleSpecialPacket(void);
BOOL HandleNumberPacket(void);
BOOL HandleModePacket(void);
BOOL HandleEEPROMProgramPacket(void);
BOOL HandleEEPROMGetPacket(void);
BOOL HandleTimePacket(void);
BOOL HandlePacketModePacket(void);
BOOL HandleAnalogValPacket(TChannelNb channelNb);
void HandleTestModePack(void);
void HandleTarrifPacket(void);
BOOL HandleTime1Packet(void);
BOOL HandleTime2Packet(void);
BOOL HandlePowerPacket(void);
BOOL HandleEnergyPacket(void);
BOOL HandleCostPacket(void);
BOOL HandleFrquencyPacket(void);
BOOL HandleVRMS(void);
BOOL HandleIRMS(void);
BOOL HandlePFactor(void);

#endif