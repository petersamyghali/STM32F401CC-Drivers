/*
 * LED.h
 *
 *  Created on: Feb 21, 2023
 *      Author: Peter Ghali
 */

#ifndef LED_H_
#define LED_H_

//#include "LED_Cfg.h"
#include "GPIO.h"
#include "LED_Cfg.h"

typedef enum
{

	LED_ON =  1 ,
	LED_OFF = 0 ,
	LED_Toggle = 2 ,
}LED_State_t;



typedef enum
{
	LED_ok,
	LED_NOK,
	LED_NPTR,
	LED_Invalid_Parameters,
}LED_Error_t;

/*
typedef enum
{
	LED_Init_ON ,
	LED_Init_Off ,
}LEDInit_State_t;
*/


LED_Error_t LED_Init(void);


LED_Error_t LED_Set_State(LED_Names_t Cpy_LED_Name  , LED_State_t  Cpy_State);




#endif /* LED_H_ */
