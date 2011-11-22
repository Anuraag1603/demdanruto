                                                                                                                        // ----------------------------------------
// Filename: analog.c
// Description: Routines for setting up and
//   reading from the ADC/DAC
// Author: Danny Sok
//         Edward Khou
// Date: 24-09-2011
// Date Last Modified: 12-10-2011 

#include "analog.h"


static const INT16 ADC_OFFSET = 2048; // 12 Bit Offset, 1000 0000 0000
static Element;
//static const INT16 DAC_OFFSET = 2048;

// ----------------------------------------
// Analog_Setup
// 
// Sets up the ADC and DAC
// Input:
//   busClk is the bus clock speed
// Output:
//   none
// Conditions:
//   none

void Analog_Setup(const UINT32 busClk) 
{
  
  TSPISetup aSPISetup;
    
  aSPISetup.isMaster          = bTRUE;                      // Master
  aSPISetup.activeLowClock    = bFALSE;                     // High clock
  aSPISetup.evenEdgeClock     = bTRUE;                      // Even
  aSPISetup.LSBFirst          = bFALSE;                     // MSB
  aSPISetup.baudRate          = 1000000;                    // 1 Mbit
  
  NbAnalogInputs.l            = 2;
  NbAnalogOutputs.l           = 2;
  Element = 0;
  
  SPI_Setup(&aSPISetup, busClk);
  
  // No selection
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 0;
}

// ----------------------------------------
// PWM_Setup
// 
// Sets up the Pulse Width Modulators for use with the DAC (missing LPFilter)
// Input:
//   busClk is the bus clock speed
// Output:
//   none
// Conditions:
//   none
void PWM_Setup(const UINT32 busClk)
{
  // 50 Duty Cycle => PWMDTY = 128, PWMPER = 256
  // Setup 2 PWMs, one for Voltage, one for Current
  
  // PWM Setups
  
  PWMPOL_PPOL0  = 1;    // Low to High 50
  PWMPOL_PPOL1  = 1;    // Low to High 25
  PWMPOL_PPOL2  = 1;    // Low to High 0
  PWMPOL_PPOL3  = 0;    // High to Low 50
  PWMPOL_PPOL4  = 0;    // High to Low 25
  PWMPOL_PPOL5  = 0;    // High to Low 0
  
  PWMCLK_PCLK0  = 1;    // Clock A
  PWMCLK_PCLK1  = 1;    // Clock A
  PWMCLK_PCLK2  = 1;    // Clock B
  PWMCLK_PCLK3  = 1;    // Clock B
  PWMCLK_PCLK4  = 1;    // Clock A
  PWMCLK_PCLK5  = 1;    // Clock A
  
  PWMCAE_CAE0   = 0;    // Left Align
  PWMCAE_CAE1   = 0;    // Left Align
  PWMCAE_CAE2   = 0;    // Left Align
  PWMCAE_CAE3   = 0;    // Left Align
  PWMCAE_CAE4   = 0;    // Left Align
  PWMCAE_CAE5   = 0;    // Left Align
  
  PWMPRCLK_PCKA = 7;    // busClk / 128 prescale
  PWMPRCLK_PCKB = 7;    // busClk / 128 prescale
  
  
  PWMCTL_CON01  = 0;    // Seperate Channel
  PWMCTL_CON23  = 0;    // Seperate Channel
  PWMCTL_CON45  = 0;    // Seperate Channel
  
  PWMCTL_PSWAI  = 1;    // PWM Stops in Wait Mode           :: 1 Stop Input Clock
  PWMCTL_PFRZ   = 1;    // PWM Counters Stop in Freeze Mode :: 1 Disable PWM Input Clock
  
  PWMPER0       = 255;  // PWM Period
  PWMPER1       = 255;  // PWM Period
  PWMPER2       = 255;  // PWM Period
  PWMPER3       = 255;  // PWM Period
  PWMPER4       = 255;  // PWM Period
  PWMPER5       = 255;  // PWM Period
  
  PWMDTY0       = 127;  // 50 Duty Cycle
  PWMDTY1       = 63;   // 25 Duty Cycle
  PWMDTY2       = 0;    // 0  Duty Cycle
  PWMDTY3       = 127;  // 50 Duty Cycle
  PWMDTY4       = 63;   // 25 Duty Cycle
  PWMDTY5       = 0;    // 0  Duty Cycle
  
  PWME_PWME0    = 1;    // PWM 0 Enable
  PWME_PWME1    = 1;    // PWM 1 Enable
  PWME_PWME2    = 1;    // PWM 2 Enable
  PWME_PWME3    = 1;    // PWM 3 Enable
  PWME_PWME4    = 1;    // PWM 4 Enable
  PWME_PWME5    = 1;    // PWM 5 Enable
}

// ----------------------------------------
// Analog_Get
// 
// Gets an analog input channel's value based on the mode
// Input:
//   channelNb is the number of the anlog input channel to read
// Output:
//   none
// Conditions:
//   Assumes that the ADC has been set up

void Analog_Get(const TChannelNb channelNb)
{
  TUINT16 conversionResult;
  INT16 medianArray[3];
  INT16 x, y, temp = 0;
  
  UINT8 txByte, txByte2, txByte3;
  txByte = 0x06; // Masking out D2 and don't cares [0000 0110]
  
  // 0000 00(d1)(d0)
  // (d1)(d0)00 0000
  txByte2 = channelNb << 6;   // Channel Select
  
  txByte3 = 0; // Don't cares
  
  // Value: 3 for Analog IO (Transmission, data to Rx)
  // Value: 0 for NULL since we don't send anything to the SPI.
  
  // 3 Low (Inverted) for transfer (CS3) - ADC
  PTH_PTH4 = 1; 
  PTH_PTH5 = 1;
  PTH_PTH6 = 0;
  
  SPI_ExchangeChar(txByte,  &conversionResult.s.Hi);  // Rx Values are '?' so discard.
  SPI_ExchangeChar(txByte2, &conversionResult.s.Hi);  // Rx Values are NULL, B11->B8
  SPI_ExchangeChar(txByte3, &conversionResult.s.Lo);  // Rx Values are B7->B0
  conversionResult.s.Hi &= 0x0F;                      // Mask out the NULL bit [? ? ? 0 B11 B10 B09 B08]
  
  // 0 High (Inverted) for no transfer
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 0;
  
  // Use median filter with sliding window of the 3 most recent samples
  Analog_Input[channelNb].Value3 = Analog_Input[channelNb].Value2;
  Analog_Input[channelNb].Value2 = Analog_Input[channelNb].Value1;
  //The term 'bipolar' indicates that the signal swings above and below some reference level. 
  //In single-ended systems, the input is typically referenced to analog ground, so a bipolar signal is one that swings above and below ground.
  Analog_Input[channelNb].Value1 = ADC_OFFSET - (INT16)conversionResult.l;
  
  
  Analog_Input[channelNb].OldValue.l = Analog_Input[channelNb].Value.l;
  
  medianArray[0] = Analog_Input[channelNb].Value1;
  medianArray[1] = Analog_Input[channelNb].Value2;
  medianArray[2] = Analog_Input[channelNb].Value3;
  
  // B.Sort. Low to High. Find Mid, yo
  for (x = 0; x < 3; x++)
  {
    for(y = 0; y < 2; y++)
    {
      if(medianArray[y] > medianArray[y+1])
      {// swap vars
        temp = medianArray[y];
        medianArray[y] = medianArray[y+1];
        medianArray[y+1] = temp;  
      } 
    }
  }
	
	Analog_Input[channelNb].Value.l = medianArray[1]; // Middle Value of B.Sort
  
}


void Analog_Put(const TChannelNb channelNb)
{
  TUINT16 voltage, current, rxByte;
  UINT8 channelNbHex;
  voltage = Analog_OutVoltage[Element];
  current = Analog_OutCurrent[Element];
  Element++;
  
  if (Element == 127)
    Element = 0;
  //Element % 128;
  
  switch (channelNb)
  {
    case Ch1:
      channelNbHex = 0x20;
    break;
    case Ch2:
      channelNbHex = 0x60;
    break;
  }
  
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 1;
  
  
  // Output Chan + Normal Operation + Input register updated [0110 0000]
  SPI_ExchangeChar(channelNbHex | (voltage.s.Hi & 0x0F),  &rxByte.s.Hi);
  asm nop;
  //SPI_ExchangeChar( ((PWMCNT2 & 0x0F) | channelNbHex),  &rxByte.s.Hi);      
  SPI_ExchangeChar(voltage.s.Lo, &rxByte.s.Lo);
  
    // 0 High (Inverted) for no transfer
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 0;
  
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 1;
  
  
  // Output Chan + Normal Operation + Input register updated [0110 0000]
  SPI_ExchangeChar(channelNbHex | (current.s.Hi & 0x0F),  &rxByte.s.Hi);
  asm nop;
  //SPI_ExchangeChar( ((PWMCNT2 & 0x0F) | channelNbHex),  &rxByte.s.Hi);      
  SPI_ExchangeChar(current.s.Lo, &rxByte.s.Lo);
  
    // 0 High (Inverted) for no transfer
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 0;
}
// ----------------------------------------
// Analog_Put
// 
// Places a PWM cycle into the ADC for output
// Input:
//   channelNb is the number of the anlog input channel to output to
// Output:
//   none
// Conditions:
//   Assumes that the DAC has been set up
/*void Analog_Put(const TChannelNb channelNb)
{
  // PWM into a filter(Low)
  // PWMCNTn(counter) -> Duty -> Pin Logic -> PWMn
  // 50 Duty Cycle => PWMDTY = 128, PWMPER = 256
  // To simulate a sine wave, modify the duty cycles e.g.
  // 0-> 25 -> 50 -> 25 -> 0
  // Have the values hold over the period and it should display a sine wave
  // 12 bit DAC
  UINT8 channelNbHex, i;
  TUINT16 rxByte;
  UINT8 test = 0x00;
  
  switch (channelNb)
  {
    case Ch1:
      channelNbHex = 0x30;
    break;
    case Ch2:
      channelNbHex = 0x70;
    break;
  }
  // DAC Data Structure:
  // A1 A0   !PD !LDAC D11 D10 D9 D8 D7 D6 D5 D4 D3 D2 D1 D0
  //|OutSel| |ContOps| |              Data                 |
  // 2 1  0  3  4  5
  // 0 25 50 50 25 0
  //------------------------------------------------------------------------
  // PWM2
  // 4 Low (Inverted) for transfer (CS4) - DAC
  for (i = 0; i < 50; i++);
  
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 1;
  
  
  // Output Chan + Normal Operation + Input register updated [0110 0000]
  SPI_ExchangeChar(channelNbHex | test,  &rxByte.s.Hi);
  asm nop;
  //SPI_ExchangeChar( ((PWMCNT2 & 0x0F) | channelNbHex),  &rxByte.s.Hi);      
  SPI_ExchangeChar(PWMCNT2, &rxByte.s.Lo);
  
  
    // 0 High (Inverted) for no transfer
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 0;
  
  
  //------------------------------------------------------------------------
  // PWM1
  for (i = 0; i < 50; i++);
  
    // 4 Low (Inverted) for transfer (CS4) - DAC
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 1;
  
  SPI_ExchangeChar(channelNbHex | test,  &rxByte.s.Hi);
  asm nop;
  //SPI_ExchangeChar( ((PWMCNT1 & 0x0F) | channelNbHex),  &rxByte.s.Hi);
  SPI_ExchangeChar(PWMCNT1, &rxByte.s.Lo);
  
  
  // 0 High (Inverted) for no transfer
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 0;
  
  
  //------------------------------------------------------------------------
  // PWM0
  for (i = 0; i < 60; i++);
  
    // 4 Low (Inverted) for transfer (CS4) - DAC
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 1;
  
  SPI_ExchangeChar(channelNbHex | test,  &rxByte.s.Hi);
  asm nop;
  //SPI_ExchangeChar( ((PWMCNT0 & 0x0F) | channelNbHex),  &rxByte.s.Hi);
  SPI_ExchangeChar(PWMCNT0, &rxByte.s.Lo);
  
  
  // 0 High (Inverted) for no transfer
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 0;
  
  for (i = 0; i < 100; i++);
  
    // 4 Low (Inverted) for transfer (CS4) - DAC
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 1;
  
  SPI_ExchangeChar(channelNbHex | test,  &rxByte.s.Hi);
  asm nop;
  //SPI_ExchangeChar( ((PWMCNT0 & 0x0F) | channelNbHex),  &rxByte.s.Hi);
  SPI_ExchangeChar(PWMCNT0, &rxByte.s.Lo);
  
  
  // 0 High (Inverted) for no transfer
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 0;
  
  //------------------------------------------------------------------------
  // PWM3
  for (i = 0; i < 100; i++);
    // 4 Low (Inverted) for transfer (CS4) - DAC
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 1;
  
  SPI_ExchangeChar(channelNbHex | test,  &rxByte.s.Hi);
  asm nop;
  //SPI_ExchangeChar( ((PWMCNT3 & 0x0F) | channelNbHex),  &rxByte.s.Hi);
  SPI_ExchangeChar(PWMCNT3, &rxByte.s.Lo);
  
  
  // 0 High (Inverted) for no transfer
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 0;
  
  
  for (i = 0; i < 60; i++);
    // 4 Low (Inverted) for transfer (CS4) - DAC
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 1;
  
  
  SPI_ExchangeChar(channelNbHex | test,  &rxByte.s.Hi);
  asm nop;
  //SPI_ExchangeChar( ((PWMCNT3 & 0x0F) | channelNbHex),  &rxByte.s.Hi);
  SPI_ExchangeChar(PWMCNT3, &rxByte.s.Lo);
  
  
  // 0 High (Inverted) for no transfer
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 0;
  
  
  //------------------------------------------------------------------------
  // PWM4
  for (i = 0; i < 50; i++);
  
    // 4 Low (Inverted) for transfer (CS4) - DAC
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 1;
  
  
  SPI_ExchangeChar(channelNbHex | test,  &rxByte.s.Hi);
  asm nop;
  //SPI_ExchangeChar( ((PWMCNT4 & 0x0F) | channelNbHex),  &rxByte.s.Hi);
  SPI_ExchangeChar(PWMCNT4, &rxByte.s.Lo);
  
  
  // 0 High (Inverted) for no transfer
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 0;
  PTH_PTH6 = 0;
  
  //------------------------------------------------------------------------
  // PWM5
  for (i = 0; i < 50; i++);
  
  // 4 Low (Inverted) for transfer (CS4) - DAC
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 1;
  
  SPI_ExchangeChar(channelNbHex | test,  &rxByte.s.Hi);
  asm nop;
  //SPI_ExchangeChar( ((PWMCNT5 & 0x0F) | channelNbHex),  &rxByte.s.Hi);
  SPI_ExchangeChar(PWMCNT5, &rxByte.s.Lo);
  
  
  // 0 High (Inverted) for no transfer
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 0;

  
}*/
