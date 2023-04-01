/*
 * APP3.c
 *
 *  Created on: Mar 18, 2023
 *      Author: Peter Ghali
 */



#include "LCD.h"

extern u8 LCD_Init_Done ;
extern LCD_enuRequest_t    LCD_enuCurrRequest ;
extern void LCD_vidWriteProcess(void);
void LCD_Task_F(void)
{
	/* CHECKING IF THE INITIALIZE IS DONE */
	if(LCD_Init_Done != LCD_enuInitDone )
	{
		LCD_enuInit();
	}
	/* INITIALIZE IS DONE */
	else
	{
		/* CHECKING THE CURRENT REQUEST */
		switch(LCD_enuCurrRequest){

		/* WRITE REQUEST */
		case LCD_enuReqWrite:
			LCD_vidWriteProcess();
			break;
		case LCD_equReqClear:
			LCD_Clear();
			break;
		default:
			break;
		}

	}
}
