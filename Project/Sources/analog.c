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

// ----------------------------------------
// Analog_Setup
// 
// Sets up the ADC and DAC
// Input:
//   none
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
  
  PWM_Setup(busClk);
  SPI_Setup(&aSPISetup, busClk);
  
  // Init 0 for ADC
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 0;
}

void PWM_Setup(const UINT32 busClk)
{
    // 50 Duty Cycle => PWMDTY = 128, PWMPER = 256
  // Setup 2 PWMs, one for Voltage, one for Current
  
  // PWM Single (ends up as Saw Wave)
  
  PWMPOL_PPOL0 = 1; // Low to High 50
  PWMPOL_PPOL1 = 1; // Low to High 25
  PWMPOL_PPOL2 = 1; // Low to High 0
  PWMPOL_PPOL3 = 0; // High to Low 50
  PWMPOL_PPOL4 = 0; // High to Low 25
  PWMPOL_PPOL5 = 0; // High to Low 0
  
  PWMCLK_PCLK0 = 0; // Clock A
  PWMCLK_PCLK1 = 0; // Clock A
  PWMCLK_PCLK2 = 0; // Clock B
  PWMCLK_PCLK3 = 0; // Clock B
  PWMCLK_PCLK4 = 0; // Clock A
  PWMCLK_PCLK5 = 0; // Clock A
  
  PWMCAE_CAE0  = 0; // Left Align
  PWMCAE_CAE1  = 0;
  PWMCAE_CAE2  = 0; // Left Align
  PWMCAE_CAE3  = 0; // Left Align
  PWMCAE_CAE4  = 1;
  PWMCAE_CAE5  = 1; // Cen Align
  
  PWMPRCLK_PCKA = 7; // busClk / 128 prescale
  PWMPRCLK_PCKB = 7; // busClk / 128 prescale
  
  
  PWMCTL_CON01 = 0; // Seperate
  PWMCTL_CON23 = 0;
  PWMCTL_CON45 = 0;
  
  PWMCTL_PSWAI = 1;
  PWMCTL_PFRZ = 1;
  
  PWMPER0 = 255;
  PWMPER1 = 255;
  PWMPER2 = 255;
  PWMPER3 = 255;
  PWMPER4 = 255;
  PWMPER5 = 255;
  
  PWMDTY0 = 127; // 50 Duty Cycle
  PWMDTY1 = 63; // 25 Duty Cycle
  PWMDTY2 = 0; // 0 Duty Cycle
  PWMDTY3 = 127;  // 50
  PWMDTY4 = 63;   // 25
  PWMDTY5 = 0; // 0
  
  PWME_PWME0  = 1;
  PWME_PWME1  = 1;
  PWME_PWME2  = 1;
  PWME_PWME3  = 1;
  PWME_PWME4  = 1;
  PWME_PWME5  = 1;
  
  
  
  
  //PWMSDN_PWMIE = 1;
  
  /*//Pwm Concat
  
  PWMPOL_PPOL0 = 1; // Low to High 50
  PWMPOL_PPOL1 = 1; // Low to High 25
  PWMPOL_PPOL2 = 1; // Low to High 0
  PWMPOL_PPOL3 = 1; // High to Low 50
  PWMPOL_PPOL4 = 1; // High to Low 25
  PWMPOL_PPOL5 = 1; // High to Low 0
  
  PWMCLK_PCLK0 = 0; // Clock A
  PWMCLK_PCLK1 = 0; // Clock A
  PWMCLK_PCLK2 = 0; // Clock B
  PWMCLK_PCLK3 = 0; // Clock B
  PWMCLK_PCLK4 = 0; // Clock A
  PWMCLK_PCLK5 = 0; // Clock A
  
  PWMCAE_CAE0  = 0; // Left Align
  PWMCAE_CAE1  = 0;
  PWMCAE_CAE2  = 0; // Left Align
  PWMCAE_CAE3  = 0; // Left Align
  PWMCAE_CAE4  = 0;
  PWMCAE_CAE5  = 0; // Cen Align
  
  PWMPRCLK_PCKA = 7; // busClk / 128 prescale
  PWMPRCLK_PCKB = 7; // busClk / 128 prescale
  
  
  PWMCTL_CON01 = 1; // Con
  PWMCTL_CON23 = 1;
  PWMCTL_CON45 = 1;
  
  PWMCTL_PSWAI = 1;
  PWMCTL_PFRZ = 1;
  
  PWMPER0 = 255;
  PWMPER1 = 255;
  PWMPER2 = 255;
  PWMPER3 = 255;
  PWMPER4 = 255;
  PWMPER5 = 255;
  
  PWMDTY0 = 127; // 50 Duty Cycle
  PWMDTY1 = 127; // 25 Duty Cycle
  PWMDTY2 = 63; // 0 Duty Cycle
  PWMDTY3 = 63;  // 50
  PWMDTY4 = 0;   // 25
  PWMDTY5 = 0; // 0
  
  PWME_PWME0  = 1;
  PWME_PWME1  = 1;
  PWME_PWME2  = 1;
  PWME_PWME3  = 1;
  PWME_PWME4  = 1;
  PWME_PWME5  = 1;*/
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
  INT16 x, y, temp;
  
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

void Analog_Put(void)
{
  // PWM into a filter(Low)
  // PWMCNTn(counter) -> Duty -> Pin Logic -> PWMn
  // 50 Duty Cycle => PWMDTY = 128, PWMPER = 256
  // To simulate a sine wave, modify the duty cycles e.g.
  // 0-> 25 -> 50 -> 25 -> 0
  // Have the values hold over the period and it should display a sine wave
  // 12 bit DAC
  
  TUINT16 rxByte;
  
  // 4 Low (Inverted) for transfer (CS4) - DAC
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 1;
  
  // DAC Data Structure:
  // A1 A0   !PD !LDAC D11 D10 D9 D8 D7 D6 D5 D4 D3 D2 D1 D0
  //|OutSel| |ContOps| |              Data                 |
  
   
  //SPI_ExchangeChar(0,  &rxByte.s.Hi);      // Output 1 + Normal Operation + Input register updated [0011 0000]
  //SPI_ExchangeChar(0, &rxByte.s.Lo);
  
  asm nop;
  asm nop;
  asm nop;
  
  SPI_ExchangeChar( ((PWMCNT2 & 0x0F) | 0x00),  &rxByte.s.Hi);      // Output 1 + Normal Operation + Input register updated [0011 0000]
  asm nop;
  asm nop;
  SPI_ExchangeChar(PWMCNT2, &rxByte.s.Lo);
  asm nop;
  SPI_ExchangeChar(PWMCNT1, &rxByte.s.Lo);
  asm nop;
  SPI_ExchangeChar(PWMCNT0, &rxByte.s.Lo);
  asm nop;
  
  SPI_ExchangeChar(PWMCNT4, &rxByte.s.Lo);
  asm nop;
  
  SPI_ExchangeChar(PWMCNT5, &rxByte.s.Lo);
  
  // 0 High (Inverted) for no transfer
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 0;
  
  asm nop;
  asm nop;
  
  // 4 Low (Inverted) for transfer (CS4) - DAC
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 1;
  
  // DAC Data Structure:
  // A1 A0   !PD !LDAC D11 D10 D9 D8 D7 D6 D5 D4 D3 D2 D1 D0
  //|OutSel| |ContOps| |              Data                 |
  
   
  //SPI_ExchangeChar(0,  &rxByte.s.Hi);      // Output 2 + Normal Operation + Input register updated [0111 0000]
  //SPI_ExchangeChar(0, &rxByte.s.Lo);
  
  asm nop;
  asm nop;
  asm nop;
  
  SPI_ExchangeChar( ((PWMCNT2 & 0x0F) | 0x70),  &rxByte.s.Hi);      // Output 2 + Normal Operation + Input register updated [0111 0000]
  asm nop;
  asm nop;
  
  SPI_ExchangeChar(PWMCNT2, &rxByte.s.Lo);
  asm nop;
  SPI_ExchangeChar(PWMCNT1, &rxByte.s.Lo);
  asm nop;
  SPI_ExchangeChar(PWMCNT0, &rxByte.s.Lo);
  asm nop;
  
  SPI_ExchangeChar(PWMCNT4, &rxByte.s.Lo);
  asm nop;
  
  SPI_ExchangeChar(PWMCNT5, &rxByte.s.Lo);
  
  
  // 0 High (Inverted) for no transfer
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 0;
  
  
    
  /*// 4 Low (Inverted) for transfer (CS4) - DAC
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 1;
  
  // DAC Data Structure:
  // A1 A0   !PD !LDAC D11 D10 D9 D8 D7 D6 D5 D4 D3 D2 D1 D0
  //|OutSel| |ContOps| |              Data                 |
  
   
  //SPI_ExchangeChar(0,  &rxByte.s.Hi);      // Output 1 + Normal Operation + Input register updated [0011 0000]
  //SPI_ExchangeChar(0, &rxByte.s.Lo);
  
  asm nop;
  asm nop;
  asm nop;
  
  SPI_ExchangeChar( ((PWMCNT4 & 0x0F) | 0x00),  &rxByte.s.Hi);      // Output 1 + Normal Operation + Input register updated [0011 0000]
  asm nop;
  asm nop;
  
  SPI_ExchangeChar(PWMCNT5, &rxByte.s.Lo);
  asm nop;
  SPI_ExchangeChar(PWMCNT2, &rxByte.s.Lo);
  asm nop;
  
  SPI_ExchangeChar(PWMCNT3, &rxByte.s.Lo);
  asm nop;
  
  SPI_ExchangeChar(PWMCNT1, &rxByte.s.Lo);
  asm nop;
  asm nop;
  SPI_ExchangeChar(PWMCNT0, &rxByte.s.Lo);
  asm nop
  SPI_ExchangeChar(PWMCNT3, &rxByte.s.Lo);
  asm nop;
  
  SPI_ExchangeChar(PWMCNT2, &rxByte.s.Lo);
  asm nop;
  SPI_ExchangeChar(PWMCNT5, &rxByte.s.Lo);
  asm nop;
  SPI_ExchangeChar(PWMCNT4, &rxByte.s.Lo);
  
  // 0 High (Inverted) for no transfer
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 0;
  
  asm nop;
  asm nop;
  
  // 4 Low (Inverted) for transfer (CS4) - DAC
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 1;
  
  // DAC Data Structure:
  // A1 A0   !PD !LDAC D11 D10 D9 D8 D7 D6 D5 D4 D3 D2 D1 D0
  //|OutSel| |ContOps| |              Data                 |
  
   
  //SPI_ExchangeChar(0,  &rxByte.s.Hi);      // Output 2 + Normal Operation + Input register updated [0111 0000]
  //SPI_ExchangeChar(0, &rxByte.s.Lo);
  
  asm nop;
  asm nop;
  asm nop;
  
  SPI_ExchangeChar( ((PWMCNT4 & 0x0F) | 0x70),  &rxByte.s.Hi);      // Output 2 + Normal Operation + Input register updated [0111 0000]
  asm nop;
  asm nop;
  
  SPI_ExchangeChar(PWMCNT5, &rxByte.s.Lo);
  asm nop;
  SPI_ExchangeChar(PWMCNT2, &rxByte.s.Lo);
  asm nop;
  
  SPI_ExchangeChar(PWMCNT3, &rxByte.s.Lo);
  asm nop;
  
  SPI_ExchangeChar(PWMCNT1, &rxByte.s.Lo);
  asm nop;
  asm nop;
  SPI_ExchangeChar(PWMCNT0, &rxByte.s.Lo);
  asm nop
  SPI_ExchangeChar(PWMCNT3, &rxByte.s.Lo);
  asm nop;
  
  SPI_ExchangeChar(PWMCNT2, &rxByte.s.Lo);
  asm nop;
  SPI_ExchangeChar(PWMCNT5, &rxByte.s.Lo);
  asm nop;
  SPI_ExchangeChar(PWMCNT4, &rxByte.s.Lo);
  
  // 0 High (Inverted) for no transfer
  PTH_PTH4 = 0;
  PTH_PTH5 = 0;
  PTH_PTH6 = 0;*/
  
}
