// ----------------------------------------
// Filename: main.c
// Description: 
//   This file contains the high-level code for the DEM project.
//   It initialises appropriate hardware subsystems,
//   creates application threads, and then starts the OS.
//
// Author: Danny Sok
// Date: 3-11-2011
// Date LM: 19-11-2011

// Constants and dependencies
#include "main.h"

static UINT16 SampleCount;
//OS_ECB SCIInUse;
//UINT8 *SCI0Stack[1024];


// ----------------------------------------
// main
//
// Initializes the various peripherals
// Handles received packets endlessly
// Input: none
// Output: none
// Conditions: none
//

void main(void)
{
  // Setup Routines.
  BOOL bSetupState;
  TTimerSetup timerSetup;
  
  
  bSetupState = Packet_Setup(baudRate, busClk) &&
                EEPROM_Setup(oscClk, busClk) &&
                CRG_SetupPLL(busClk, oscClk, refClk) &&
                CRG_SetupCOP(watchDogRate);
                
                

  timerSetup.outputCompare = bTRUE;
  timerSetup.outputAction = TIMER_OUTPUT_DISCONNECT;      
  timerSetup.inputDetection = TIMER_INPUT_OFF;
  timerSetup.toggleOnOverflow = bFALSE;
  timerSetup.interruptEnable = bTRUE;
                
  Clock_Setup(prescaleRate, modulusCount);
  Analog_Setup(busClk);
  PWM_Setup(busClk);
  
  Timer_Setup();
  Timer_PeriodicTimerEnable(bTRUE);
  Timer_SetupPeriodicTimer(Periodic_Delay, busClk);
  
  Timer_SetupPulseAccumulators();
  Timer_Init(TIMER_Ch7, &timerSetup);
  Timer_Set(TIMER_Ch7, Timer_Ch7_Delay);
  
  Timer_Init(TIMER_Ch4, &timerSetup);
  Timer_Set(TIMER_Ch4, Timer_Ch4_Delay);
  Timer_Enable(TIMER_Ch4, bTRUE);
  
  HMI_Setup();
  
  if (bSetupState)
  {
    // Turn on LED D11 to show success.
    DDRE_BIT7   = 1;
    PORTE_BIT7  = 0;
  }
  
  EnableInterrupts;
  (void)HandleStartupPacket();
  
  SampleCount = 0;
  
  //OS_Init();
  //
  //SCIInUse = OS_SemaphoreCreate(1); 
  //error = OS_ThreadCreate(SCIThread, (void *)&SCI0DR, &SCI0Stack[1024], 10);
   
  //OS_Start();
  
  // Main loop
  for (;;)
  {
    ARMCOP = copReset;
    
    if ( Packet_Get() )
      HandlePacket();
    
    // Determine if a second has passed. If it has then send a time packet  
    if ( Clock_Update() )
    {
      //Analog_Put(Ch1);
      (void)HandleTimePacket();
      HMI_Update();
    }
    
    
  
    ARMCOP = copEnd;
  }
}

//---------------------------------
//  HandlePacket
//
//  Determines what packet is recieved, then replies with the
//  coresponding answer
//  Input: none
//  Output: none
//  Conditions: none
void HandlePacket(void)
{
  BOOL bACK = bFALSE;

  if (Packet_Command >= 0x80) // 128 in hex
  {
    bACK = bTRUE;
    Packet_Command -= 0x80;  
  }
  
  switch (Packet_Command)
  {
    case CMD_STARTUP:
         Packet_CommandOK = HandleStartupPacket();
    break;
    case CMD_SPECIAL:
         Packet_CommandOK = HandleSpecialPacket();
    break;
    case CMD_MODCON_NB:
         Packet_CommandOK = HandleNumberPacket();
    break;
    case CMD_MODCON_MODE:
         Packet_CommandOK = HandleModePacket();
    break;
    case CMD_EEPROM_PROG:
         Packet_CommandOK = HandleEEPROMProgramPacket();
    break;
    case CMD_EEPROM_GET:
         Packet_CommandOK = HandleEEPROMGetPacket();
    break;
    case CMD_MODCON_PMODE:
         Packet_CommandOK = HandlePacketModePacket();
    break;
    case CMD_TEST_MODE:
         HandleTestModePack();
    break;
    case CMD_TARRIF:
         HandleTarrifPacket();
    break;
    case CMD_TIME1:
         Packet_CommandOK = HandleTime1Packet();
    break;
    case CMD_TIME2:
         Packet_CommandOK = HandleTime2Packet();
    break;
    case CMD_POWER:
         Packet_CommandOK = HandlePowerPacket();
    break;
    case CMD_ENERGY:
         Packet_CommandOK = HandleEnergyPacket();
    break;
    case CMD_COST:
         Packet_CommandOK = HandleCostPacket();
    break;
    case CMD_FREQUENCY:
         Packet_CommandOK = HandleFrquencyPacket();
    break;
    case CMD_VOLRMS:
         Packet_CommandOK = HandleVRMS();
    break;
    case CMD_CURRMS:
         Packet_CommandOK = HandleIRMS();
    break;
    case CMD_POWFAC:
         Packet_CommandOK = HandlePFactor();
    break;
    
    default:
         Packet_CommandOK = bFALSE;
    break;
  }
         
  if (bACK && Packet_CommandOK)
  {
    (void)Packet_Put(Packet_Command += 0x80, Packet_Parameter1, Packet_Parameter2, Packet_Parameter3);
  }
  else if (bACK)
    (void)Packet_Put(Packet_Command, Packet_Parameter1, Packet_Parameter2, Packet_Parameter3);
  

  
 }
 
void interrupt 12 TIE4_ISR(void)
{
  TFLG1_C4F = 1; // Clear flag to say it was done.
  //OS_ISREnter();
  
  if (Debug)
  {
    Analog_Put(Ch1);
    Analog_Put(Ch2);
  }
  
  //OS_ISRExit();
}

//---------------------------------
//  MCCNT_ISR
//
//  The ISR for Modulus Count.
//  This will send the analog values to the pc
//  Input: none
//  Output: none
//  Conditions: none    
 void interrupt 26 MCCNT_ISR(void)
 {
  INT32 voltage, current, i;
  MCFLG_MCZF = 1; // Clear/Ack
  //OS_ISREnter();  
  
  if (Debug)
    PTT_PTT4 ^= 1;
  
  Analog_Get(Ch1);
  Analog_Get(Ch2);
  
  // In ASYNC mode, we send only if the value has changed.
  if (sControlMode == PACKET_ASYNCHRONOUS)
  {
    if (Analog_Input[Ch1].OldValue.l != Analog_Input[Ch1].Value.l)
      (void)HandleAnalogValPacket(Ch1);
    if (Analog_Input[Ch2].OldValue.l != Analog_Input[Ch2].Value.l)
      (void)HandleAnalogValPacket(Ch2);
  }
  // In SYNC mode, we send all the time.
  else
  {
    (void)HandleAnalogValPacket(Ch1);
    (void)HandleAnalogValPacket(Ch2);
  }
  if (SampleCount >= DEM_PWRSIZE)
  {
    SampleCount = 0;
    Math_FindEnergy(DEM_AvePower_Array);
    DEM_Average_Power.l = 0;
    for (i = 0; i < DEM_PWRSIZE; i++)
    {
      DEM_Average_Power.l += DEM_AvePower_Array[i];
    }
    // Find the average power over the samples
    DEM_Average_Power.l = DEM_Average_Power.l >> 4;
  }
    
  voltage = Math_ConvertADCValue(Analog_Input[Ch1].Value.l);
  current = Math_ConvertADCValue(Analog_Input[Ch2].Value.l);
  DEM_AvePower_Array[SampleCount] = Math_FindPower(voltage, current);
  //DEM_Average_Power.l = DEM_AvePower_Array[SampleCount];
  SampleCount++;
  Math_FindFrequency(Periodic_Delay);
  /*
  Clock_MilliSeconds += 2;
  if (Clock_MilliSeconds >= 1000)
  {
    if (Debug)
    {
      Clock_Hours++;
      Clock_MilliSeconds -= 1000;  // 1 second has passed
      
      if (Clock_Hours >= 24)
      {
        Clock_Days++;
        Clock_Hours -= 24;
      }
    }
    
    Clock_Seconds++;
    Clock_Interval++;
    Clock_MilliSeconds -= 1000;  // 1 second has passed
    
    
    
    if (Clock_Seconds >= 60)
    {
    Clock_Minutes++;               // 1 minute has passed
    Clock_Seconds -= 60;
    }
    
    if (Clock_Minutes >= 60)
    {
      Clock_Hours++;
      Clock_Minutes -= 60;
    }
       
    if (Clock_Hours >= 24)
    {
      Clock_Days++;
      Clock_Hours -= 24;
    }
  }
  
  Use later when ditching Clock*/
  
  //OS_ISRExit();
 }
   
 

//---------------------------------
//  HandleStartupPacket
//
//  Creates the packet that will be in
//  response to the startup call by the PC.
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleStartupPacket(void)
{
  if(Packet_Parameter1 != 0 || Packet_Parameter1 != 0 || Packet_Parameter3 != 0)
    return bFALSE;  
    
  
  return Packet_Put(CMD_STARTUP, 0, 0, 0) && // startup takes 0 for all parameters
         HandleSpecialPacket() &&
         HandleNumberPacket() &&
         HandleModePacket() &&
         Packet_Put( CMD_MODCON_PMODE, 1, (UINT8)sControlMode, 0 );
}

//---------------------------------
//  HandleSpecialPacket
//
//  Creates the packet that will be in
//  response to the special call by the PC.
//  It responds with the ModCon Version.
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleSpecialPacket(void)
{
  switch (Packet_Parameter1)
  {
    case 'v':
      return Packet_Put(CMD_SPECIAL, 'v', MAJOR_VERSION, MINOR_VERSION);
    break;
    case 'd':
      EEPROM_Write16(&Debug, Debug ^ 1); // Set debug
      return HandleTimePacket();
    break;
  }
  
  // If called by Startup Packet
  return Packet_Put(CMD_SPECIAL, 'v', MAJOR_VERSION, MINOR_VERSION);
}

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
BOOL HandleNumberPacket(void)
{
  TUINT16 NbModCon;
  if (Packet_Parameter1 == 2)
  {
    NbModCon.s.Lo = Packet_Parameter2;
    NbModCon.s.Hi = Packet_Parameter3;
  }
  else
    NbModCon.l = sModConNb;
  
  return Packet_Put(CMD_MODCON_NB, 1, NbModCon.s.Lo, NbModCon.s.Hi);
  
}

//---------------------------------
//  HandleModePacket
//
//  Creates the packet that will be in
//  response to the ModCon mode call by the PC.
//  It responds with the modcon mode
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleModePacket(void)
{
  TUINT16 mode;
  mode.l = sModConMode;
  return Packet_Put(CMD_MODCON_MODE, 1, mode.s.Lo, mode.s.Hi);
}

//---------------------------------
//  HandleEEPROMProgramPacket
//
//  Creates the packet that will be in
//  response to the ModCon EEPROM Programming call by the PC.
//  It responds by writing the data recieved
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleEEPROMProgramPacket(void)
{
  TUINT16 address;
  address.s.Lo = Packet_Parameter1;    //LSB
  address.s.Hi = Packet_Parameter2;    //MSB
  
  if (address.s.Hi == 10 && address.s.Lo == 0)
    return EEPROM_Erase();

  return EEPROM_Write8( (volatile UINT8 *)address.l, Packet_Parameter3 ); 
}

//---------------------------------
//  HandleEEPROMGetPacket
//
//  Creates the packet that will be in
//  response to the ModCon mode call by the PC.
//  It responds with the modcon mode
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleEEPROMGetPacket(void)
{
  TUINT16 address;
  address.s.Lo = Packet_Parameter1;
  address.s.Hi = Packet_Parameter2;
  
  if ( (Packet_Parameter2 > 0x0F) || (Packet_Parameter2 < 0x04) )
    return bFALSE;
  
  return Packet_Put( CMD_EEPROM_GET, Packet_Parameter1, Packet_Parameter2, _EB(address.l) ); // cmd, lsb, msb, data
}

//---------------------------------
//  HandleTimePacket
//
//  Creates the packet that will be in
//  response to the ModCon time call by the PC.
//  It responds with the modcon time
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleTimePacket(void)
{
  // Intervals : 'i', Seconds, Minutes
  return Packet_Put(CMD_MODCON_TIME, 'i', Clock_Seconds, Clock_Minutes);
}

//---------------------------------
//  HandlePacketModePacket
//
//  Creates the packet that will be in
//  response to the ModCon Packet Mode call by the PC.
//  It responds with the Packet Mode
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandlePacketModePacket(void)
{
  // p1: 2 = set, 1 = get
  // p2: 0 = async, 1 = sync
  // p3: 0
  if (Packet_Parameter1 == 2)
    return EEPROM_Write16(&sControlMode, Packet_Parameter2);
  if (Packet_Parameter1 == 1)
    return Packet_Put( CMD_MODCON_PMODE, 1, (UINT8)sControlMode, 0 ); // cmd, 1, mode, 0;  
}

//---------------------------------
//  HandleAnalogValPacket
//
//  Creates the packet that will be in
//  response to the ModCon Analog Value call by the PC.
//  It responds with the current Analog Value
//  Input: none
//  Output: none
//  Conditions: none
BOOL HandleAnalogValPacket(TChannelNb channelNb)
{
  // cmd, channel, lsb, msb
  return Packet_Put(CMD_ANALOG_VALUE, (UINT8)channelNb, Analog_Input[channelNb].Value.s.Lo, Analog_Input[channelNb].Value.s.Hi);  
}

void HandleTestModePack(void)
{
  if(Packet_Parameter1)
    EEPROM_Write16(&Debug, 1);
  else
    EEPROM_Write16(&Debug, 0);
}

void HandleTarrifPacket(void)
{
  switch(Packet_Parameter1)
  {
    case 1:
      //DEM_Tarrif = sT1OffPeak;
      (void)EEPROM_Write16(&sTarrifMode, 1);
    break;
    case 2:
      //DEM_Tarrif = sT2NonTOU;
      (void)EEPROM_Write16(&sTarrifMode, 2);
    break;
    case 3:
      //DEM_Tarrif = sT3NonTOU;
      (void)EEPROM_Write16(&sTarrifMode, 3);
    break;
  }
  
  /*if (Packet_Parameter1 == 1 && Packet_Parameter3 == 0)
    DEM_Tarrif = sT1OffPeak;
  else if (Packet_Parameter1 == 2 && Packet_Parameter3 == 0)
    DEM_Tarrif = sT2NonTOU;
  else if (Packet_Parameter1 == 3 && Packet_Parameter3 == 0) 
    DEM_Tarrif = sT3NonTOU;*/
}

BOOL HandleTime1Packet(void)
{
  if (Packet_Parameter3 == 0)
    return Packet_Put(CMD_TIME1, (UINT8)Clock_Seconds, (UINT8)Clock_Minutes, 0);
  else
  {
    
    Clock_Seconds = Packet_Parameter1;
    Clock_Minutes = Packet_Parameter2;
  }
  
}

BOOL HandleTime2Packet(void)
{
  if (Packet_Parameter3 == 0)
    return Packet_Put(CMD_TIME2, (UINT8)Clock_Hours, (UINT8)Clock_Days, 0);
  else
  {
    Clock_Hours = Packet_Parameter1;
    Clock_Days = Packet_Parameter2;
  }
}

BOOL HandlePowerPacket(void)
{
  return Packet_Put(CMD_POWER, DEM_Average_Power.s.Lo, DEM_Average_Power.s.Hi, 0);
}

BOOL HandleEnergyPacket(void)
{
  return Packet_Put(CMD_POWER, (UINT8)DEM_Total_Energy.s.Lo, (UINT8)DEM_Total_Energy.s.Hi, 0);
}

BOOL HandleCostPacket(void)
{
  return Packet_Put(CMD_COST, (UINT8)Math_FromQN(DEM_Total_Cost, qRight, 9), (UINT8)Math_FromQN(DEM_Total_Cost, qLeft, 9), 0);
}

BOOL HandleFrquencyPacket(void)
{
  Math_FindFrequency(Periodic_Delay);
  return Packet_Put(CMD_FREQUENCY, DEM_Frequency.s.Lo, DEM_Frequency.s.Hi, 0);
}

BOOL HandleVRMS(void)
{
  return Packet_Put(CMD_VOLRMS, DEM_VRMS.s.Lo, DEM_VRMS.s.Hi, 0);
}

BOOL HandleIRMS(void)
{
  return Packet_Put(CMD_CURRMS, DEM_IRMS.s.Lo, DEM_IRMS.s.Hi, 0);
}

BOOL HandlePFactor(void)
{
  return 0;
}