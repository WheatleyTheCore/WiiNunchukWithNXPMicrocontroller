/* ###################################################################
**     Filename    : main.c
**     Project     : nunchuk
**     Processor   : MC9S08QD4CPC
**     Version     : Driver 01.12
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2019-06-17, 17:21, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.12
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "LED.h"
#include "EI2C1.h"
#include "DataPin1.h"
#include "ClockPin1.h"
#include "PWM1.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
byte I2C_data[6];                  
int AccelX;  
int AccelY;                   //All of these are global for debugging purposes, if desired they can be moved into main().
int AnalogX;
int AnalogY;
int Z;
int C;
int servoPos;
void main(void)
{
  /* Write your local variable definition here */
  
  word numbytes;

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  I2C_data[0] = 0xF0;      //init first register
  I2C_data[1] = 0x55;
  
  EI2C1_SendBlock(I2C_data, 2, &numbytes);
  EI2C1_SendStop();
  Cpu_Delay100US(100);
  
  I2C_data[0] = 0xFB;      // init second register
  I2C_data[1] = 0x00;
    
  EI2C1_SendBlock(I2C_data, 2, &numbytes);
  EI2C1_SendStop();
  Cpu_Delay100US(100);
  
  for(;;) { 
	  EI2C1_RecvBlock(I2C_data, 6, &numbytes); //Recieve the data from the nunchuk. It should be 6 bytes total.
	  EI2C1_SendStop();
	  
	  AnalogX = I2C_data[0];                                     
	  AnalogY = I2C_data[1];
	  AccelX = (I2C_data[2] << 2) | ((I2C_data[5] >> 2) & 3);
	  AccelY = (I2C_data[3] << 2) | ((I2C_data[5] >> 4) & 3);
	  Z = !((I2C_data[5] >> 0) & 1);
	  C = !((I2C_data[5] >> 1) & 1);
	  servoPos = (AccelX-300)/2;         //scale AccelX to between 0 and 255(about). The project was built without access to floating point numbers to conserve space, so this is the closest I came up with.
	   
	  PWM1_SetRatio8(servoPos);          //write our scaled value to the servo. The function takes 255 as fully turned to the right, and 0 as fully turned to the left. 
	  I2C_data[0] = 0x00;                //preparing data to be sent as ACK
	  I2C_data[1] = 0x00;
	  
	  EI2C1_SendBlock(I2C_data, 2, &numbytes); //ACK sent
	  EI2C1_SendStop();
	  Cpu_Delay100US(100);              //Waiting a second for the nunchuk to process and respond
  }

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
