/*
 * LED_Cfg.h
 *
 *  Created on: Feb 21, 2023
 *      Author: Peter Ghali
 */

#ifndef LED_CFG_H_
#define LED_CFG_H_

#define NAME_LENGHT		20

typedef enum
{
	LED_Active_Low = 0 ,
	LED_Active_High = 1
}LED_Type_t ;

typedef enum
{
	LED_0,
	LED_1,
	LED_Count ,
}LED_Names_t;

typedef struct
{
	GPIO_enuPORT_t Port ;
	GPIO_enuPin_t Pin;
	GPIO_enuSpeed_t Speed ;
	LED_Type_t Type ;

}LED_Cfg_t;

//extern the Cfg array that has the LED type and the LED Pins
extern const LED_Cfg_t LED_Cfg_Array [LED_Count] ;

#endif /* LED_CFG_H_ */
