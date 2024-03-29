// ----------------------------------------
// Filename: HMI.h
// Description: Lab 5
//  Implements the HMI to control the LCD and menu system
//  backlighting, display and setup
// Author: Danny Sok
//         Edward Khou
// Date Created: 20-10-2011
// Date Last Modified: 2-11-2011

#ifndef HMI_H
#define HMI_H

#include "types.h"
#include "LCD.h"
#include "buttons.h"
#include "EEPROM.h"
#include "DEM.h"
#include "Math.h"
#include "clock.h"
#include "analog.h"
#include "timer.h"
#include <mc9s12a256.h>     /* derivative information */

                                                              


// ----------------------------------------
// HMI_Setup
// 
// Sets up the HMI
// Input:
//   none
// Output:
//   none
// Conditions:
//   none
void HMI_Setup(void);

// ----------------------------------------
// HMI_Update
// 
// Updates the LCD display with the new values of Power, Energy, Cost and Time
// Input:
//   none
// Output:
//   none
// Conditions:
//   none
void HMI_Update(void);

// ----------------------------------------
// CreateMenu
// 
// Creates the menu to display on the LCD
// Input:
//   menu is the state to display on the LCD e.g. DefaultMenu
// Output:
//   none
// Conditions:
//   LCD and HMI have been setup
void CreateMenu(TLCDState menu);

// ----------------------------------------
// HandleButtonPress
// 
// Determines which functions to call/what to do when a button is pressed
// Input:
//   PBState is the push button state that was most recent
//   LCDState is the menu currently displayed on the LCD
// Output:
//   none
// Conditions:
//   LCD and Buttons have been setup
void HandleButtonPress(TButtonInputs PBState, TLCDState LCDState);

#endif