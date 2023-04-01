/*
 * LED_Cfg.c
 *
 *  Created on: Feb 21, 2023
 *      Author: Peter Ghali
 */

#include "LED.h"
#include "LED_Cfg.h"
#include "GPIO.h"

const LED_Cfg_t LED_Cfg_Array [LED_Count] =
{
		[LED_0] =
		{
				.Type= LED_Active_High ,
				.Port = GPIO_PORT_B ,
				.Pin = GPIO_PIN_12 ,
				.Speed = GPIO_Speed_Very_High ,


		},

		[LED_1] =
		{
				.Type= LED_Active_High ,
				.Port = GPIO_PORT_B ,
				.Pin = GPIO_PIN_8 ,
				.Speed = GPIO_Speed_Very_High ,


		},/*
		[LED_2] =
		{
				.Type= LED_Active_High ,
				.Port = GPIO_PORT_A ,
				.Pin = GPIO_PIN_2 ,
				.Speed = GPIO_Speed_Very_High ,


		},*/
};



