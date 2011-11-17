// ----------------------------------------
// Filename: SCI.c
// Description: I/O routines for MC9S12 serial
//   communication interface 
// Author: Danny Sok
//         Edward Khou
// Date Created: 4-08-2011
// Date Last Modified: 10-08-2011

#include "SCI.h"


static TFIFO RxFIFO;
static TFIFO TxFIFO;

static const UINT8 SCI_Timeout = 255;
static UINT8 i;

// ----------------------------------------
// SCI_Setup
// 
// Sets up the Serial Communication Interface
// Input:
//   baudRate is the baud rate in bits/sec
//   busClk is the bus clock rate in Hz
// Output:
//   none
// Conditions:
//   none
void SCI_Setup(const UINT32 baudRate, const UINT32 busClk)
{
  // baud: SCI Module Clock / (16xBR[12:0])
  SCI0BD = (UINT8) ((busClk / baudRate) / 16);
  
  // Friendly code below
  // SCI0 Control Register 1
  SCI0CR1_PT      = 0;  // Parity Type                                : 0 = Even Parity
  SCI0CR1_PE      = 0;  // Parity Enable                              : 0 = Disabled
  SCI0CR1_ILT     = 0;  // Idle Line Type                             : 0 = Idle after start bit
  SCI0CR1_WAKE    = 0;  // Wakeup Condition                           : 0 = Idle line wakeup
  SCI0CR1_M       = 0;  // Data Format Mode                           : 0 = 8 data + 1 stop
  SCI0CR1_RSRC    = 0;  // Receiver Source                            : when LOOPS = 1
  SCI0CR1_SCISWAI = 0;  // Stop in Wait Mode                          : 0 = Enabled
  SCI0CR1_LOOPS   = 0;  // Loop Select Bit                            : 0 = OFF
  
  // SCI0 Control Register 2
  SCI0CR2_SBK     = 0;  // Send Break Bit                             : 0 = No Break Char
  SCI0CR2_RWU     = 0;  // Receiver Wakeup                            : 0 = Normal Operation
  SCI0CR2_RE      = 1;  // Reciever Enable                            : 1 = ENABLED
  SCI0CR2_TE      = 1;  // Transmitter Enable                         : 1 = ENABLED
  SCI0CR2_ILIE    = 0;  // Idle Line Interrupt Enable                 : 0 = DISABLED
  SCI0CR2_RIE     = 1;  // Receiver Full Interrupt Enable             : 1 = Enabled | 0 = DISABLED
  SCI0CR2_TCIE    = 0;  // Transmission Complete Interrupt Enable     : 0 = DISABLED
  SCI0CR2_SCTIE   = 0;  // Transmitter Interrupt Enable               : 0 = DISABLED (Will be handled by Timer CH7)
  
  
  PTM_PTM7        = 0;  // PORT M Init
  
  // Data Direction Port M (USB)
  DDRM_DDRM7      = 1;  // Data Direction Bit 7                       : 1 = Output            
  
  // Initialise the FIFO's
  FIFO_Init(&RxFIFO);
  FIFO_Init(&TxFIFO);
  
  i = 0;
}

/*void SCI_Thread(void *pData)
{
  OS_ERROR error;
  
  for (;;)  
  {
    error = OS_SemaphoreWait(SCIInUse, 0); 
    OS_TimeDelay(100)
    //OS_ThreadDelete();
    OS_ENTER_CRITICAL();
    (void)SendData();
    OS_EXIT_CRITICAL();
    if (error == OS_NO_ERROR)
    {
      // signalled
      return;
    }
    else
    {
      // overflow 
      return;
    }
  }
}*/

// ----------------------------------------
// SCI_InChar
// 
// Get a character from the receive FIFO if it is not empty
// Input:
//   dataPtr is a pointer to memory to store the retrieved byte
// Output:
//   TRUE if the receive FIFO returned a character
// Conditions:
//   Assumes the receive FIFO has been initialized
BOOL SCI_InChar(UINT8 * const dataPtr)
{ 
  return FIFO_Get(&RxFIFO, dataPtr);
}

// ----------------------------------------
// SCI_OutChar
//
// Put a byte in the transmit FIFO if it is not full
// Input:
//   data is a byte to be placed in FIFO
// Output:
//   TRUE if the data was placed in the FIFO
// Conditions:
//   Assumes receive FIFO has been initialized
BOOL SCI_OutChar(const UINT8 data)
{
  if(FIFO_Put(&TxFIFO, data))
    return SendData();
  
  return bFALSE;
}

// ----------------------------------------
// SendData
//
// Place a byte into the SCI0DRL register
// Input:
//   None.
// Output:
//   TRUE if the data was placed in the register
//   FALSE if there is no data to place or unable to place
// Conditions:
//   Assumes receive FIFO has been initialized
BOOL SendData(void)
{
  if ( SCI0SR1_TDRE && !FIFO_Get(&TxFIFO, &SCI0DRL) )
  {
    Timer_Enable(TIMER_Ch7, bFALSE); // Stop interrupts from occuring
    return bFALSE;
  }
  Timer_Set(TIMER_Ch7, Timer_Ch7_Delay);
  return bTRUE;
}

// ----------------------------------------
// SCI0_ISR
//
// The interrupt called when the RDRF flag is set
// Input:
//   none
// Output:
//   none
// Conditions:
//   Assumes transmit FIFO have been initialized
void interrupt 20 SCI0_ISR(void)
{
  //OS_ISREnter();
  if (SCI0SR1_RDRF)
    // Saves recieved data to data, clears RDRF
    (void)FIFO_Put(&RxFIFO, SCI0DRL);
  //OS_ISRExit();
}

// ----------------------------------------
// TIE7_ISR
//
// The interrupt called when the Channel 7 timer
// has triggered. It will kick start the packet sending
// Input:
//   none
// Output:
//   none
// Conditions:
//   Assumes receive FIFO have been initialized
void interrupt 15 TIE7_ISR(void)
{
  TFLG1_C7F = 1; // Clear flag to say it was done.
  //OS_ISREnter();
  
  if (Debug)
    PTT_PTT6 ^= 1;
  
  // Kick start the process
  (void)SendData();
  
  //OS_ISRExit();
}

// ----------------------------------------
// SCI_Poll
//
// Poll the SCI status to see whether to receive
//   or transmit a single byte
// Input:
//   none
// Output:
//   none
// Conditions:
//   Assumes transmit and receive FIFOs have been initialized
void SCI_Poll(void) 
{  
  // Recieving data
  if (SCI0SR1_RDRF)
    // Saves recieved data to data, clears RDRF
    (void)FIFO_Put(&RxFIFO, SCI0DRL);
  
  // Transfer data
  if (SCI0SR1_TDRE && TxFIFO.NbBytes > 0)
    (void)FIFO_Get(&TxFIFO, &SCI0DRL);
}

// ----------------------------------------
// SCI_WaitForChar
// 
// Waits for a character to be received from the SCI until a timeout occurs
// Input:
//   dataPtr is a pointer to memory to store the retrieved byte
// Output:
//   TRUE if the receive FIFO returned a character within the timeout time
// Conditions:
//   Assumes the receive FIFO has been initialized
BOOL SCI_WaitForChar(UINT8 * const dataPtr)
{
  
  for(i = 0; i < SCI_Timeout; i++)
  {
    if(SCI_InChar(dataPtr))
      return bTRUE;
  }
  return bFALSE;
}