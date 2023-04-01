/*
 * Switch.h
 *
 *  Created on: Feb 23, 2023
 *      Author: Peter Ghali
 */

#ifndef HAL_SWITCH_H_
#define HAL_SWITCH_H_

#include "GPIO.h"


#define SWITCH_NAME_LENGHT		20

typedef enum
{
	Switch_Alarm ,
	Switch_Count ,
}Switch_Names_t;

typedef enum
{
	Switch_Pressed =  1 ,
	Switch_Not_Pressed = 0 ,
	Switch_NA = 2  ,
}Switch_State_t;

typedef enum
{
	Switch_Active_Low = 1 ,
	Switch_Floating_Active_LOW  =  1 ,
	Switch_Active_High = 0 ,
	Switch_Floating_Active_High = 0  ,
}Switch_Type_t ;

typedef enum
{
	Switch_ok,
	Switch_NOK,
	Switch_NPTR,
	Switch_Invalid_Parameters,
}Switch_Error_t;



typedef struct
{
	GPIO_enuPORT_t Port ;
	GPIO_enuPin_t Pin;
	GPIO_enuSpeed_t Speed ;
	Switch_Type_t Type ;
}Switch_Cfg_t;


Switch_Error_t Switch_Init(void);


Switch_Error_t Switch_Get_State(Switch_Names_t Cpy_Switch_Name  , Switch_State_t * Cpy_State);



#endif /* HAL_SWITCH_H_ */
