/*
 * APP1.c
 *
 *  Created on: Mar 18, 2023
 *      Author: Peter Ghali
 */

#include "Switch.h"
#include "Switch_Cfg.h"

extern Switch_State_t Switches_State[Switch_Count]  ;

void Switch_Task(void)
{
	static int Counter = 0 ,Switch_Counter = 0 ;
	static Switch_State_t Prev_State = Switch_NA ;
	Switch_State_t Current_State = Switch_NA ;
	for (Switch_Counter = 0 ;  Switch_Counter < Switch_Count ; Switch_Counter++)
	{
		GPIO_Get_Pin_Value(Switch_Cfg_Array[Switch_Counter].Port, Switch_Cfg_Array[Switch_Counter].Pin, &Current_State);
		Current_State ^=  Switch_Cfg_Array[Switch_Counter].Type ;
		if (Current_State == Prev_State )
		{
			Counter++;
		}
		else
		{
			Counter = 0 ;
		}
		if (Counter == 5 )
		{
			Switches_State[Switch_Counter] = Current_State ;
			Counter = 0 ;
		}
		Prev_State = Current_State ;
	}
}
