/*
 * LED.c
 *
 *  Created on: Feb 21, 2023
 *      Author: Peter Ghali
 */

#include "Bit_Math.h"
#include "STD_TYPES.h"

#include "RCC.h"
#include "GPIO.h"


#include "LED.h"
#include "LED_Cfg.h"

static LED_State_t LED_State = LED_OFF ;


LED_Error_t LED_Init(void)
{
	LED_Error_t Loc_Error_Status = LED_ok ;
	GPIO_Cfg_t GPIO_Cfg ;
	u8 Loc_Counter = 0 ;
	for (Loc_Counter = 0 ; Loc_Counter < LED_Count ;Loc_Counter++ )
	{

		GPIO_Cfg.Mode = GPIO_Output_push_pull;
		GPIO_Cfg.Pin = LED_Cfg_Array[Loc_Counter].Pin ;
		GPIO_Cfg.Port = LED_Cfg_Array[Loc_Counter].Port ;
		GPIO_Cfg.Speed  =LED_Cfg_Array[Loc_Counter].Speed ;
		GPIO_Init_Pin(&GPIO_Cfg);

	}

	return Loc_Error_Status ;
}


LED_Error_t LED_Set_State(LED_Names_t Cpy_LED_Name,  LED_State_t  Cpy_State)
{
	LED_Error_t Loc_Error_Status = LED_ok ;

	if (Cpy_State == LED_Toggle)
	{
		GPIO_Get_Pin_Value(LED_Cfg_Array[Cpy_LED_Name].Port, LED_Cfg_Array[Cpy_LED_Name].Pin, &LED_State);
		GPIO_Write_Pin_Value(LED_Cfg_Array[Cpy_LED_Name].Port,
				LED_Cfg_Array[Cpy_LED_Name].Pin,
				LED_Cfg_Array[Cpy_LED_Name].Type ^ LED_State ^ 1);

	}
	else
	{
		GPIO_Write_Pin_Value(LED_Cfg_Array[Cpy_LED_Name].Port,
				LED_Cfg_Array[Cpy_LED_Name].Pin,
				Cpy_State ^ LED_Cfg_Array[Cpy_LED_Name].Type);

		LED_State = Cpy_State ;
	}

	return Loc_Error_Status ;
}

