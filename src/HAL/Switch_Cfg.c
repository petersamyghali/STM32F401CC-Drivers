/*
 * Switch_Cfg.c
 *
 *  Created on: Feb 23, 2023
 *      Author: Peter Ghali
 */




#include "Switch.h"
#include "Switch_Cfg.h"
#include "GPIO.h"

const Switch_Cfg_t Switch_Cfg_Array [Switch_Count] =
{
		[Switch_Alarm] =
		{
				.Type= Switch_Active_Low,
				.Port = GPIO_PORT_B ,
				.Pin = GPIO_PIN_8 ,
				.Speed = GPIO_Speed_Very_High ,

		},
};

