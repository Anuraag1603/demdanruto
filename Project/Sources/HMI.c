// ----------------------------------------
// Filename: HMI.c
// Description: Lab 5
//  Implements the HMI to control the LCD and menu system
//  backlighting, display and setup
// Author: Danny Sok
//         Edward Khou
// Date Created: 20-10-2011
// Date Last Modified: 2-11-2011

#include "HMI.h"

// Position of the cursors;
static TButtonInputs PBState, PrevPBState;
static TLCDState LCDState;


// ----------------------------------------
// HMI_Setup
// 
// Sets up the HMI
// Input:
//   
// Output:
//   none
// Conditions:
//  none
void HMI_Setup(void)
{
  // Port K - Push Buttons
  DDRK_BIT2 = 0;    // 0: Input, PK2
  DDRK_BIT3 = 0;    // 0: Input, PK3
  DDRK_BIT4 = 0;    // 0: Input, PK4
  DDRK_BIT5 = 0;    // 0: Input, PK5
  DDRK_BIT7 = 0;    // 0: Input, PK7
  
  // Port A: LCD Display
  DDRA_BIT0 = 1;    // 1: Output, PA0
  DDRA_BIT1 = 1;    // 1: Output, PA1
  DDRA_BIT2 = 1;    // 1: Output, PA2
  DDRA_BIT3 = 1;    // 1: Output, PA3
  DDRA_BIT4 = 1;    // 1: Output, PA4
  DDRA_BIT5 = 1;    // 1: Output, PA5
  DDRA_BIT6 = 1;    // 1: Output, PA6
  DDRA_BIT7 = 1;    // 1: Output, PA7
  
  // Port B: LCD Control
  DDRB_BIT0 = 1;    // 1: Output, PB0
  DDRB_BIT1 = 1;    // 1: Output, PB1
  DDRB_BIT2 = 1;    // 1: Output, PB2
  DDRB_BIT3 = 1;    // 1: Output, PB3
  DDRB_BIT4 = 1;    // 1: Output, PB4
  DDRB_BIT5 = 1;    // 1: Output, PB5
  DDRB_BIT6 = 1;    // 1: Output, PB6
  DDRB_BIT7 = 1;    // 1: Output, PB7
  
  LCD_Setup();
  DEM_Setup();
    
  LCDState = MeteringTime;
  CreateMenu(MeteringTime);
  
  Clock_Interval      = 0;
  
}

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
void HMI_Update(void)
{
  UINT16 tarrifL = Math_FromQN(DEM_Tarrif, qLeft, DefaultBase);
  UINT16 tarrifR = Math_FromQN(DEM_Tarrif, qRight, DefaultBase);
  //INT16 voltage, current;
  DEM_SetTarrif();
  Math_FindCost();
  
  DEM_VRMS.l = Math_FindRMS(DEM_Average_Power.l);
  DEM_IRMS.l = Math_FindRMS(Analog_Input[Ch2].Value.l);
  
  switch(LCDState)
  {
    case MeteringTime:
      if (Clock_Days <= MAX_DAYS)
      {
        LCD_ClearLine(2);
        LCD_SetLine(2);
        (void)LCD_OutChar(' ');
        LCD_OutInteger(Clock_Days);
        (void)LCD_OutChar(':');
        LCD_OutInteger(Clock_Hours);
        (void)LCD_OutChar(':');
        LCD_OutInteger(Clock_Minutes);
        (void)LCD_OutChar(':');
        LCD_OutInteger((UINT16)Clock_Seconds);
        
        LCD_ClearLine(3);
        LCD_SetLine(3);
        LCD_OutString(" Tarrif:");
        LCD_OutInteger(tarrifL);
        (void)LCD_OutChar('.');
        LCD_OutInteger(tarrifR);
      }
      else
        LCD_OutString(" xx:xx:xx:xx  ");
    break;
    
    case AveragePower:
      LCD_ClearLine(2);
      LCD_SetLine(2);
      (void)LCD_OutChar(' ');
      //voltage = Math_ConvertADCValue(Analog_Input[Ch1].Value.l);
      //current = Math_ConvertADCValue(Analog_Input[Ch2].Value.l);
      //DEM_Average_Power.l = Math_FindPower(voltage, current);
      LCD_OutInteger(Math_FromQN(DEM_Average_Power.l, qLeft, DefaultBase << 1));
      (void)LCD_OutChar('.');
      LCD_OutInteger(Math_FromQN(DEM_Average_Power.l, qRight, DefaultBase << 1));
      LCD_OutString("kW");
      LCD_ClearLine(3);
      LCD_SetLine(3);
      LCD_OutString(" Tarrif:");
      LCD_OutInteger(tarrifL);
      (void)LCD_OutChar('.');
      LCD_OutInteger(tarrifR);
    break;
    
    case TotalEnergy:
      LCD_ClearLine(2);
      LCD_SetLine(2);
      if (DEM_Total_Energy.l <= MAX_ENERGY)
      {
        (void)LCD_OutChar(' ');
        LCD_OutInteger( Math_FromQN(DEM_Total_Energy.l, qLeft, DefaultBase << 1) );
        (void)LCD_OutChar('.');
        LCD_OutInteger( Math_FromQN(DEM_Total_Energy.l, qRight, DefaultBase << 1) );
        LCD_OutString("kWh");
        LCD_ClearLine(3);
        LCD_SetLine(3);
        LCD_OutString(" Tarrif:");
        LCD_OutInteger(tarrifL);
        (void)LCD_OutChar('.');
        LCD_OutInteger(tarrifR);
      } 
      else
        LCD_OutString(" xxx.xxx      ");
    break;
    
    case TotalCost:
      LCD_ClearLine(2);
      LCD_SetLine(2);
      if (DEM_Total_Cost <= MAX_COST)
      {
        (void)LCD_OutChar(' ');
        (void)LCD_OutChar('$');
        LCD_OutInteger( Math_FromQN(DEM_Total_Cost, qLeft, DefaultBase) );
        (void)LCD_OutChar('.');
        LCD_OutInteger( Math_FromQN(DEM_Total_Cost, qRight, DefaultBase) );
        
        LCD_ClearLine(3);
        LCD_SetLine(3);
        LCD_OutString(" Tarrif:");
        LCD_OutInteger(tarrifL);
        (void)LCD_OutChar('.');
        LCD_OutInteger(tarrifR);
      } 
      else
      {
        LCD_OutString(" xxxx.xx      ");
      }
    break;
  }
}

// ----------------------------------------
// TIE5_ISR
// 
// The interrupt service routine for Timer Channel 5.
// It will determine if a pushbutton has been pressed and will
// allow for the button event to be handled.
// Input:
//   
// Output:
//   none
// Conditions:
//  none
void interrupt 13 TIE5_ISR(void)
{
  TFLG1_C5F = 1; // Clear flag to say it was done.  
  //OS_ISREnter();
  
  Buttons_Get(&PBState); 
  
  if (PBState != PrevPBState)
  {
    HandleButtonPress(PBState, LCDState);
    Clock_Interval = 0;
  }
  else if (PBState == ButtonIdle && Clock_Interval >= 15) // 15 seconds
  {
    LCD_DisplayOff();
    LCDState = Dormant;
  }
  PrevPBState = PBState;
  
  Timer_Enable(TIMER_Ch6, bFALSE);
  //OS_ISRExit();
}

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
void CreateMenu(TLCDState menu)
{
  UINT16 tarrifL = Math_FromQN(DEM_Tarrif, qLeft, DefaultBase);
  UINT16 tarrifR = Math_FromQN(DEM_Tarrif, qRight, DefaultBase);
  //INT16 voltage, current;
  
  LCD_Clear();
  
  LCDState = menu;
  
  switch(menu)
  {
    case Dormant:
      LCD_SetLine(0);
      LCD_OutString("                ");
      LCD_SetLine(1);
      LCD_OutString("                ");
      LCD_SetLine(2);
      LCD_OutString("                ");
      LCD_SetLine(3);
      LCD_OutString("                ");
      LCD_SetLine(4);
      LCD_OutString("                ");
      LCD_SetLine(5);
      LCD_OutString("                ");
      LCD_SetLine(6);
      LCD_OutString("                ");
      LCD_SetLine(7);
      LCD_OutString("                ");
    break;
    
    case MeteringTime:
      
      LCD_SetLine(0);
      LCD_OutString("$--------------$");
      LCD_SetLine(1);
      LCD_OutString("|Metering Time |");
      
      if (Clock_Days <= MAX_DAYS)
      {
        LCD_SetLine(2);
        (void)LCD_OutChar(' ');
        LCD_OutInteger(Clock_Days);
        (void)LCD_OutChar(':');
        LCD_OutInteger(Clock_Hours);
        (void)LCD_OutChar(':');
        LCD_OutInteger(Clock_Minutes);
        (void)LCD_OutChar(':');
        LCD_OutInteger((UINT16)Clock_Seconds);
      }
      else
        LCD_OutString(" xx:xx:xx:xx  ");
      
      LCD_SetLine(3);
      LCD_OutString(" Tarrif:");
      LCD_OutInteger(tarrifL);
      (void)LCD_OutChar('.');
      LCD_OutInteger(tarrifR);
      LCD_SetLine(4);
      LCD_OutString("|              |");
      LCD_SetLine(5);
      LCD_OutString("|              |");
      LCD_SetLine(6);
      LCD_OutString("|              |");
      LCD_SetLine(7);
      LCD_OutString("$------CYC-----$");
    break;
    
    case AveragePower:
      LCD_SetLine(0);
      LCD_OutString("$--------------$");
      LCD_SetLine(1);
      LCD_OutString("|   Ave Power  |");
      LCD_SetLine(2);
      (void)LCD_OutChar(' ');
      //voltage = Math_ConvertADCValue(Analog_Input[Ch1].Value.l);
      //current = Math_ConvertADCValue(Analog_Input[Ch2].Value.l);
      //DEM_Average_Power.l = Math_FindPower(voltage, current);
      LCD_OutInteger(Math_FromQN(DEM_Average_Power.l, qLeft, DefaultBase << 1));
      (void)LCD_OutChar('.');
      LCD_OutInteger(Math_FromQN(DEM_Average_Power.l, qRight, DefaultBase << 1));
      LCD_OutString("kW");
      LCD_SetLine(3);
      LCD_OutString(" Tarrif:");
      LCD_OutInteger(tarrifL);
      (void)LCD_OutChar('.');
      LCD_OutInteger(tarrifR);
      LCD_SetLine(4);
      LCD_OutString("|              |");
      LCD_SetLine(5);
      LCD_OutString("|              |");
      LCD_SetLine(6);
      LCD_OutString("|              |");
      LCD_SetLine(7);
      LCD_OutString("$------CYC-----$");
    break;
    
    case TotalEnergy:
      LCD_SetLine(0);
      LCD_OutString("$--------------$");
      LCD_SetLine(1);
      LCD_OutString("| Total Energy |");
      
      LCD_SetLine(2);
      if (DEM_Total_Energy.l <= MAX_ENERGY)
      {
        (void)LCD_OutChar(' ');
        LCD_OutInteger( Math_FromQN(DEM_Total_Energy.l, qLeft, DefaultBase << 1) );
        (void)LCD_OutChar('.');
        LCD_OutInteger( Math_FromQN(DEM_Total_Energy.l, qRight, DefaultBase << 1) );
        LCD_OutString("kWh");
      } 
      else
        LCD_OutString(" xxx.xxx      ");
      
      LCD_SetLine(3);
      LCD_OutString(" Tarrif:");
      LCD_OutInteger(tarrifL);
      (void)LCD_OutChar('.');
      LCD_OutInteger(tarrifR);
      LCD_SetLine(4);
      LCD_OutString("|              |");
      LCD_SetLine(5);
      LCD_OutString("|              |");
      LCD_SetLine(6);
      LCD_OutString("|              |");
      LCD_SetLine(7);
      LCD_OutString("$------CYC-----$");
    break;
    
    case TotalCost:
      LCD_SetLine(0);
      LCD_OutString("$--------------$");
      LCD_SetLine(1);
      LCD_OutString("|  Total Cost  |");
      LCD_SetLine(2);
      
      if (DEM_Total_Cost <= MAX_COST)
      {
        (void)LCD_OutChar(' ');
        (void)LCD_OutChar('$');
        LCD_OutInteger( Math_FromQN(DEM_Total_Cost, qLeft, DefaultBase) );
        (void)LCD_OutChar('.');
        LCD_OutInteger( Math_FromQN(DEM_Total_Cost, qRight, DefaultBase) );
      } 
      else
      {
        LCD_OutString(" xxxx.xx      ");
      }
      LCD_SetLine(3);
      LCD_OutString(" Tarrif:");
      LCD_OutInteger(tarrifL);
      (void)LCD_OutChar('.');
      LCD_OutInteger(tarrifR);
      LCD_SetLine(4);
      LCD_OutString("|              |");
      LCD_SetLine(5);
      LCD_OutString("|              |");
      LCD_SetLine(6);
      LCD_OutString("|              |");
      LCD_SetLine(7);
      LCD_OutString("$------CYC-----$");
    break;
  }
}

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
void HandleButtonPress(TButtonInputs PBState, TLCDState LCDState)
{
  switch(LCDState)
  {
    case Dormant:
      switch(PBState)
      {
        case Button0:
        case Button1:
        case Button2:
        case Button3:
        case Button4:
          LCD_DisplayOn();
          CreateMenu(MeteringTime);
        break;
      }
    break;
    
    case MeteringTime:
      switch(PBState)
      {
        case Button2:
          CreateMenu(AveragePower);
        break;
      }
    break;
    
    case AveragePower:
      switch(PBState)
      {
        case Button2:
          CreateMenu(TotalEnergy);  
        break;
      }
    break;
    
    case TotalEnergy:
      switch(PBState)
      {
        case Button2:
          CreateMenu(TotalCost);
        break;
      }
    break;
    
    case TotalCost:
      switch(PBState)
      {
        case Button2:
          CreateMenu(MeteringTime);
        break;
      }
    break;
  }
  
}
