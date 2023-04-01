/*
 * Switch_App.c
 *
 *  Created on: Mar 21, 2023
 *      Author: Peter Ghali
 */
#include "LED.h"

#include "Switch.h"


void Switch_Application(void)
{

	Switch_State_t Switch_State = Switch_NA;
	Switch_Get_State(Switch_Alarm , &Switch_State);
	if (Switch_State == Switch_Pressed)
	{
		LED_Set_State(LED_1, LED_ON);
	}
	else if (Switch_State == Switch_Not_Pressed)
	{
		LED_Set_State(LED_1, LED_OFF);
	}


}
