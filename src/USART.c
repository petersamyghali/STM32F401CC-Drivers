/*
 * LED.c
 *
 *  Created on: Mar 22, 2023
 *      Author: Peter Ghali
 */




#include "LED.h"
#include "USART.h"

void UART_F(void)
{
	static u8 Data = 5 ;
	static u8 Buffer[5] = "PETER";
	USART_enuRecive_Buffer(USART_6, &Data, 1, NULL);
	USART_enuTransmit_Buffer_Zero_cpy(USART_6, Buffer, 5, NULL);

	if (Data == 'a')
	{
		LED_Set_State(LED_0, LED_ON);
	}
	else if (Data == 'b')
	{
		LED_Set_State(LED_0, LED_OFF);
	}


}
