/*
 * APP2.c
 *
 *  Created on: Mar 18, 2023
 *      Author: Peter Ghali
 */
#include "LCD.h"


void LCD_APP_F(void)
{
	static u8 counter = 0 ;

	if ((counter %2) ==0)
	{
		LCD_enuSendString_Async("PETER",5,0,0);

	}
	else
	{
		LCD_Clear();
	}
	counter++ ;


}


