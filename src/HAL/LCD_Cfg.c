/*
 * LCD_Cfg.c
 *
 * Created: 10-Feb-23 2:52:14 PM
 *  Author: Peter Ghali
 */
#include "GPIO.h"

#include "LCD.h"
#include "LCD_Cfg.h"

const LCD_Ports_PINs_t LCD_Cfg_Array [LCD_BIT_MODE+ Number_OF_CONTROL_PINS]=
{
		[LCD_RS_PIN] =
		{
				.LCD_PORT = GPIO_PORT_B ,
				.LCD_PIN = GPIO_PIN_3,
		},
		[LCD_RW_PIN] =
		{
				.LCD_PORT = GPIO_PORT_B ,
				.LCD_PIN = GPIO_PIN_4,
		},
		[LCD_E_PIN] =
		{
				.LCD_PORT = GPIO_PORT_B ,
				.LCD_PIN = GPIO_PIN_5,
		},
		[LCD_D0_PIN] =
		{
				.LCD_PORT = GPIO_PORT_A ,
				.LCD_PIN = GPIO_PIN_0,
		},
		[LCD_D1_PIN] =
		{
				.LCD_PORT = GPIO_PORT_A ,
				.LCD_PIN = GPIO_PIN_1,
		},
		[LCD_D2_PIN] =
		{
				.LCD_PORT = GPIO_PORT_A ,
				.LCD_PIN = GPIO_PIN_2,
		},
		[LCD_D3_PIN] =
		{
				.LCD_PORT = GPIO_PORT_A ,
				.LCD_PIN = GPIO_PIN_3,
		},
		[LCD_D4_PIN] =
		{
				.LCD_PORT = GPIO_PORT_A ,
				.LCD_PIN = GPIO_PIN_4,
		},
		[LCD_D5_PIN] =
		{
				.LCD_PORT = GPIO_PORT_A ,
				.LCD_PIN = GPIO_PIN_5,
		},
		[LCD_D6_PIN] =
		{
				.LCD_PORT = GPIO_PORT_A ,
				.LCD_PIN = GPIO_PIN_6,
		},
		[LCD_D7_PIN] =
		{
				.LCD_PORT = GPIO_PORT_A ,
				.LCD_PIN = GPIO_PIN_7,
		},


};

