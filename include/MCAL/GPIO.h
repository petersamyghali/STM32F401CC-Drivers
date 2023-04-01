/*
 * GPIO.h
 *
 *  Created on: Feb 14, 2023
 *      Author: Peter Ghali
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "STD_TYPES.h"
//An enum data type to hold the state of the  PIN
typedef enum
{
	GPIO_PIN_HIGH = 0 ,
	GPIO_PIN_Low,
}GPIO_enuPIN_State_t;

//An enum data type to hold the ports the user may use
typedef enum
{
	GPIO_PORT_A 	=  0x140020000,
 	GPIO_PORT_B     =  0x140020400,
 	GPIO_PORT_C 	=  0x140020800,
 	GPIO_PORT_D 	=  0x140020C00,
	GPIO_PORT_E 	=  0x140021000,
    GPIO_PORT_H 	=  0x140021C00,

}GPIO_enuPORT_t;


typedef enum
{
	GPIO_PIN_0 = 1,
	GPIO_PIN_1 = 2,
	GPIO_PIN_2 = 4,
	GPIO_PIN_3 = 8,
	GPIO_PIN_4 = 16,
	GPIO_PIN_5 = 32,
	GPIO_PIN_6 = 64,
	GPIO_PIN_7 = 128,
	GPIO_PIN_8 = 256,
	GPIO_PIN_9 = 512,
	GPIO_PIN_10= 1024,
	GPIO_PIN_11= 2048,
	GPIO_PIN_12= 4096,
	GPIO_PIN_13= 8192,
	GPIO_PIN_14= 16384,
	GPIO_PIN_15= 32768,
	GPIO_PIN_ALL = 65535 ,

}GPIO_enuPin_t;

typedef enum
{
	GPIO_Speed_Low = 0 ,
	GPIO_Speed_Medium ,
	GPIO_Speed_High ,
	GPIO_Speed_Very_High
}GPIO_enuSpeed_t;


typedef enum
{
	GPIO_Input_floating =0 ,
	GPIO_Input_pull_up ,
	GPIO_Input_pull_down,

	GPIO_Analog,

	GPIO_Output_open_drain,
	GPIO_Output_open_drain_pull_up ,
	GPIO_Output_open_drain_pull_down ,

	GPIO_Output_push_pull,
	GPIO_Output_push_pull_pull_up,
	GPIO_Output_push_pull_pull_down,

	GPIO_Alternate_function_push_pull,
	GPIO_Alternate_function_push_pull_pull_up,
	GPIO_Alternate_function_push_pull_pull_down ,

	GPIO_Alternate_function_open_drain,
	GPIO_Alternate_function_open_drain_pull_up,
	GPIO_Alternate_function_open_drain_pull_down,
}GPIO_enuMode_t;


typedef enum
{
	GPIO_OK = 0 ,
	GPIO_NOK ,
	GPIO_NULL_PTR ,
	GPIO_InavalidPort,
	GPIO_InavalidPin,
	GPIO_InavalidParametrs,
	GPIO_InavalidMode,
}GPIO_ErrorStatus_t;

typedef enum
{
	GPIO_SET = 0 ,
	GPIO_RESET = 1 ,
}GPIO_Value_t;

typedef enum
{
	GPIO_PORT_SET = 0xFFFF ,
	GPIO_PORT_RESET = 0xFFFF0000,
}GPIO_PORT_Value_t;

typedef enum
{
	AF_Sytem ,
	AF_TIM_1_2 ,
	AF_TIM_3_5 ,
	AF_TIM_9_10_11 ,
	AF_I2C_1_2_3 ,
	AF_SPI_1_2_3_4,
	AF_SPI_3,
	AF_USART_1_2,
	AF_USART_6,
	AF_I2C_2_3,
	AF_OTG_FS,
	AF_SDIO = 12 ,
	AF_EVENTOUT = 15,
}GPIO_AF_t;

typedef struct
{
	GPIO_enuPORT_t Port ;
	GPIO_enuPin_t Pin;
	GPIO_enuSpeed_t Speed ;
	GPIO_enuMode_t Mode ;
}GPIO_Cfg_t;

GPIO_ErrorStatus_t GPIO_Init_Pin ( GPIO_Cfg_t * Add_GPIO_Cfg);

GPIO_ErrorStatus_t GPIO_SET_Alternate_Function(GPIO_enuPORT_t Cpy_Port ,GPIO_enuPin_t Cpy_pin , GPIO_AF_t Cpy_AF);

GPIO_ErrorStatus_t GPIO_Write_Pin_Value(GPIO_enuPORT_t Cpy_Port ,GPIO_enuPin_t Cpy_pin , GPIO_Value_t Cpy_Value);

GPIO_ErrorStatus_t GPIO_Write_Port_Value(GPIO_enuPORT_t Cpy_Port  , GPIO_PORT_Value_t Cpy_Value);

GPIO_ErrorStatus_t GPIO_Get_Pin_Value(GPIO_enuPORT_t Cpy_Port ,GPIO_enuPin_t Cpy_pin , u8 * Add_Value);

GPIO_ErrorStatus_t GPIO_LOCK (GPIO_enuPORT_t Cpy_Port ,GPIO_enuPin_t Cpy_pin );

#endif /* GPIO_H_ */
