/*
 * Switch.c
 *
 *  Created on: Feb 23, 2023
 *      Author: Peter Ghali
 */

#include "Bit_Math.h"
#include "STD_TYPES.h"

#include "GPIO.h"

#include "Switch.h"
#include "Switch_Cfg.h"


 Switch_State_t Switches_State[Switch_Count]  ;

Switch_Error_t Switch_Init(void)
{
	Switch_Error_t Loc_Error_Status = Switch_ok ;
	GPIO_Cfg_t GPIO_Cfg ;
	u8 Loc_Counter = 0 ;
	for (Loc_Counter = 0 ; Loc_Counter < Switch_Count ;Loc_Counter++ )
	{
		if (Switch_Cfg_Array[Loc_Counter].Type == Switch_Active_High)
		{
			GPIO_Cfg.Mode = GPIO_Input_pull_down;
		}
		else if (Switch_Cfg_Array[Loc_Counter].Type == Switch_Active_Low)
		{
			GPIO_Cfg.Mode = GPIO_Input_pull_up;
		}
		else if ((Switch_Cfg_Array[Loc_Counter].Type ==Switch_Floating_Active_High) || (Switch_Cfg_Array[Loc_Counter].Type == Switch_Floating_Active_LOW))
		{
			GPIO_Cfg.Mode = GPIO_Input_floating ;
		}
		else
		{

		}
		GPIO_Cfg.Pin = Switch_Cfg_Array[Loc_Counter].Pin ;
		GPIO_Cfg.Port = Switch_Cfg_Array[Loc_Counter].Port ;
		GPIO_Cfg.Speed  =Switch_Cfg_Array[Loc_Counter].Speed ;
		GPIO_Init_Pin(&GPIO_Cfg);
		Switches_State[Loc_Counter] = Switch_NA;
	}

	return Loc_Error_Status ;
}


Switch_Error_t Switch_Get_State(Switch_Names_t Cpy_Switch_Name,  Switch_State_t  * Cpy_State)
{
	Switch_Error_t Loc_Error_Status = Switch_ok ;
	if (Cpy_State == NULL )
	{
		Loc_Error_Status = Switch_NPTR ;

	}
	else
	{
		* Cpy_State = Switches_State[Cpy_Switch_Name];
		/*
		GPIO_Get_Pin_Value(Switch_Cfg_Array[Cpy_Switch_Name].Port,
			Switch_Cfg_Array[Cpy_Switch_Name].Pin,
			Cpy_State );
		* Cpy_State ^= Switch_Cfg_Array[Cpy_Switch_Name].Type ;*/

	}


	return Loc_Error_Status ;
}

