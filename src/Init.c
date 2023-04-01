/*
 * Init.c
 *
 *  Created on: Mar 28, 2023
 *      Author: Peter Ghali
 */
#include "STD_TYPES.h"
#include "MCAL/GPIO.h"
#include "MCAL/RCC.h"
#include "LED.h"
#include "SSegment.h"
#include "Switch.h"
#include "NVIC.h"
#include "Clock_Control.h"
#include "SysTick.h"
#include "Sched.h"
#include "USART.h"

void Init_F(void)
{
	//RCC_enuPLL_Cfg(RCC_PLL_SOURCE_HSI, PLL_FACT_M_60, PLL_FACT_N_192, PLL_FACT_P_2, PLL_FACT_Q_2);
	RCC_enuSetSysClock(RCC_SYS_CLK_HSE_CRYSTAL);
	RCC_enuEnablePerClock(RCC_BUS_AHB1, RCC_PER_GPIOA);
	RCC_enuEnablePerClock(RCC_BUS_AHB1, RCC_PER_GPIOB);
	RCC_enuEnablePerClock(RCC_BUS_APB2 , RCC_PER_USART1);
	RCC_enuEnablePerClock(RCC_BUS_APB2 , RCC_PER_USART6);
	GPIO_SET_Alternate_Function(GPIO_PORT_A, GPIO_PIN_10, AF_USART_1_2);
	GPIO_SET_Alternate_Function(GPIO_PORT_A, GPIO_PIN_9, AF_USART_1_2);
	GPIO_SET_Alternate_Function(GPIO_PORT_A, GPIO_PIN_11, AF_USART_6);
	GPIO_SET_Alternate_Function(GPIO_PORT_A, GPIO_PIN_12, AF_USART_6);
	GPIO_Cfg_t TX_PIN ;
	TX_PIN.Mode = GPIO_Alternate_function_push_pull ;
	TX_PIN.Pin = GPIO_PIN_9 ;
	TX_PIN.Port = GPIO_PORT_A ;
	TX_PIN.Speed = GPIO_Speed_Very_High;
	GPIO_Init_Pin (&TX_PIN);

	TX_PIN.Pin = GPIO_PIN_11 ;
	GPIO_Init_Pin (&TX_PIN);
	TX_PIN.Pin = GPIO_PIN_12 ;
	GPIO_Init_Pin (&TX_PIN);
	TX_PIN.Pin = GPIO_PIN_10 ;
	GPIO_Init_Pin (&TX_PIN);
	NVIC_EnableIRQ(IRQHandler_USART1);
	NVIC_EnableIRQ(IRQHandler_USART6);
	USART_CFG_t USART1  ;
	USART1.BaudRate = 9600 ;
	USART1.Channel = USART_1 ;
	USART1.Data_Bits = USART_EIGHT_DATA_BITS ;
	USART1.Mode = USART_TRANSMITTER_RECIVER ;
	USART1.Over_Sampling = OVER_SAMPLING_BY_8 ;
	USART1.Parity_Bit = USART_PARITY_BIT_DISABLE ;
	USART1.Stop_Bits = USART_ONE_STOP_BIT ;

	USART_enuInit(&USART1);
	USART_CFG_t USART6  ;
	USART6.BaudRate = 9600 ;
	USART6.Channel = USART_6 ;
	USART6.Data_Bits = USART_EIGHT_DATA_BITS ;
	USART6.Mode = USART_TRANSMITTER_RECIVER ;
	USART6.Over_Sampling = OVER_SAMPLING_BY_8 ;
	USART6.Parity_Bit = USART_PARITY_BIT_DISABLE ;
	USART6.Stop_Bits = USART_ONE_STOP_BIT ;
	USART_enuInit(&USART6);


	LED_Init();
	//Switch_Init();
}


