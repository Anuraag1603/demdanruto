// OS
#include "OS.h"

#define THREAD_STACK_SIZE 100

static const UINT16 LED_TIME_ON  = 10;
static const UINT16 LED_TIME_OFF = 10;

static UINT8 *LEDOnStack[THREAD_STACK_SIZE];
static UINT8 *LEDOffStack[THREAD_STACK_SIZE];

// ----------------------------------------
// Global Sempahores
// ----------------------------------------

static OS_EVENT *TurnLEDOn;
static OS_EVENT *TurnLEDOff;

// ----------------------------------------
// LEDOn
// ----------------------------------------

static void LEDOn(void *pData)
{
  for (;;)                       
  {
    // Call one of the RTOS's services:
    (void)OS_SemaphoreWait(TurnLEDOn, 0);

    // User Code
    
    // Turn LED on
    PORTE_BIT7 = 0;

    // Call more RTOS services:
    OS_TimeDelay(LED_TIME_ON);
    (void)OS_SemaphoreSignal(TurnLEDOff);
  }
} 

// ----------------------------------------
// LEDOff
// ----------------------------------------

static void LEDOff(void *pData)
{

  for (;;)                       
  {
    // Call one of the RTOS's services:
    (void)OS_SemaphoreWait(TurnLEDOff, 0);

    // User Code

    // Turn LED off
    PORTE_BIT7 = 1;

    // Call more RTOS services:
    OS_TimeDelay(LED_TIME_OFF);
    (void)OS_SemaphoreSignal(TurnLEDOn);

  }
} 

// ----------------------------------------
// main
// ----------------------------------------

/*void main(void)
{
  OS_ERROR error;

  // Initialise hardware  
  PORTE_BIT7 = 1; // Turn LED off
  DDRE_BIT7  = 1; // Set /XCLKS as output 

  // Initialize the RTOS
  OS_Init();         
  
  // Create semaphores for signalling between threads
  TurnLEDOn = OS_SemaphoreCreate(1);
  TurnLEDOff = OS_SemaphoreCreate(0);

  // Create threads using OS_ThreadCreate();
  error = OS_ThreadCreate(LEDOn, 
                  NULL,
                  &LEDOnStack[THREAD_STACK_SIZE - 1],
                  1);
  error = OS_ThreadCreate(LEDOff, 
                  NULL,
                  &LEDOffStack[THREAD_STACK_SIZE - 1],
                  2);

  // Start multithreading!  
  OS_Start();      
}*/