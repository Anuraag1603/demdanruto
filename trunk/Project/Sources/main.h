// ----------------------------------------
// Filename:     main.h
// Description:
//  Calls the main routines for the DEM.    
//
// Author: Danny Sok
// Date:   3-11-2011
// Date LM: 19-11-2011

#ifndef MAIN_H
#define MAIN_H

#include <hidef.h>          /* common defines and macros */
#include <mc9s12a256.h>     /* derivative information */
		 
#pragma LINK_INFO DERIVATIVE "mc9s12a256b"

// include files here

// Types
#include "types.h"

// LCD interfacing
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

// DEM constants
#include "DEM.h"

// Clock timings
#include "clock.h"

// Version number
const UINT8 MAJOR_VERSION     = 1;
const UINT8 MINOR_VERSION     = 0;

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
const UINT16 Periodic_Delay   = 20000; // 20ms
const UINT16 Timer_Ch7_Delay  = 2000;  // We need enough delay for 10 bits.
const UINT16 Timer_Ch4_Delay  = 50000; //~20ms

// ----------------------------------------
// Command setups
const UINT8 copReset          = 0x55;
const UINT8 copEnd            = 0xAA;

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

// Clock Setup
UINT8  Clock_Seconds;
UINT8  Clock_Minutes;
UINT8  Clock_Hours;
UINT8  Clock_Days;
UINT32 Clock_MicroSeconds;
UINT32 Clock_Interval;

// DEM Setup
TUINT16 DEM_Frequency;
TUINT32 DEM_Average_Power;
UINT32  DEM_Power_Array[DEM_PWRSIZE];
TUINT32 DEM_Total_Energy;
UINT32  DEM_Total_Cost;
UINT16  DEM_Tarrif;
TUINT16 DEM_VRMS;
TUINT16 DEM_IRMS;



// ----------------------------------------
//  Function Prototypes

// ----------------------------------------
// main
//
// Initializes the various peripherals
// Handles received packets endlessly
// Input: none
// Output: none
// Conditions: none
void main(void);
//---------------------------------
//  HandlePacket
//
//  Determines what packet is recieved, then replies with the
//  coresponding answer
//  Input: none
//  Output: none
//  Conditions: none
void HandlePacket(void);
//---------------------------------
//  HandleStartupPacket
//
//  Creates the packet that will be in
//  response to the startup call by the PC.
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleStartupPacket(void);
//---------------------------------
//  HandleSpecialPacket
//
//  Creates the packet that will be in
//  response to the special call by the PC.
//  It responds with the ModCon Version.
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleSpecialPacket(void);
//---------------------------------
//  HandleNumberPacket
//
//  Creates the packet that will be in
//  response to the ModCon number call by the PC.
//  It responds with the last five digits of our
//  student number.
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleNumberPacket(void);
//---------------------------------
//  HandleModePacket
//
//  Creates the packet that will be in
//  response to the ModCon mode call by the PC.
//  It responds with the modcon mode
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleModePacket(void);
//---------------------------------
//  HandleEEPROMProgramPacket
//
//  Creates the packet that will be in
//  response to the ModCon EEPROM Programming call by the PC.
//  It responds by writing the data recieved
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleEEPROMProgramPacket(void);
//---------------------------------
//  HandleEEPROMGetPacket
//
//  Creates the packet that will be in
//  response to the ModCon mode call by the PC.
//  It responds with the modcon mode
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleEEPROMGetPacket(void);
//---------------------------------
//  HandleTimePacket
//
//  Creates the packet that will be in
//  response to the ModCon time call by the PC.
//  It responds with the modcon time
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleTimePacket(void);
//---------------------------------
//  HandlePacketModePacket
//
//  Creates the packet that will be in
//  response to the ModCon Packet Mode call by the PC.
//  It responds with the Packet Mode
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandlePacketModePacket(void);
//---------------------------------
//  HandleAnalogValPacket
//
//  Creates the packet that will be in
//  response to the ModCon Analog Value call by the PC.
//  It responds with the current Analog Value
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleAnalogValPacket(const TChannelNb channelNb);
//---------------------------------
//  HandleTestModePack
//
//  Toggles test mode
//  Input: none
//  Output: none
//  Conditions: none
void HandleTestModePack(void);
//---------------------------------
//  HandleTarrifPacket
//
//  Sets the tarrif type depending
//  Input: none
//  Output: none
//  Conditions: none
void HandleTarrifPacket(void);
//---------------------------------
//  HandleTime1Packet
//
//  Creates the packet that will respond with the
//  seconds and minutes 
//  OR
//  sets the seconds and minutes based on the packet
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleTime1Packet(void);
//---------------------------------
//  HandleTime2Packet
//
//  Creates the packet that will respond with the
//  hours and dayss 
//  OR
//  sets the hours and days based on the packet
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleTime2Packet(void);
//---------------------------------
//  HandlePowerPacket
//
//  Creates the packet that will return
//  the current average power
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandlePowerPacket(void);
//---------------------------------
//  HandleEnergyPacket
//
//  Creates the packet that will return
//  the current total energy
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleEnergyPacket(void);
//---------------------------------
//  HandleCostPacket
//
//  Creates the packet that will return
//  the current total cost
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleCostPacket(void);
//---------------------------------
//  HandleFrequencyPacket
//
//  Creates the packet that will return
//  the current frequency
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleFrequencyPacket(void);
//---------------------------------
//  HandleVRMS
//
//  Creates the packet that will return
//  the VRMS value
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleVRMS(void);
//---------------------------------
//  HandleIRMS
//
//  Creates the packet that will return
//  the IRMS value
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleIRMS(void);
//---------------------------------
//  HandlePFactor
//
//  Creates the packet that will return
//  the power factor
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandlePFactor(void);

#endif