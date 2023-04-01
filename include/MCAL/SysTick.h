/*
 * SysTick.h
 *
 *  Created on: Mar 7, 2023
 *      Author: Peter Ghali
 */

#ifndef MCAL_SYSTICK_H_
#define MCAL_SYSTICK_H_

typedef void (*cbf)(void);

typedef enum
{
	SysTick_OK = 0 ,
	SysTick_NOK ,
	SysTick_NULL_PTR ,
	SysTick_InavalidParametrs,
	SysTick_InavalidMode,
}SysTick_ErrorStatus_t;

typedef enum
{
	SysTick_CLK_Source_AHB  = 1,
	SysTick_CLK_Source_AHB_Div_8 = 0  ,
}SysTick_CLK_Source_t;

SysTick_ErrorStatus_t SysTick_Start (void);

SysTick_ErrorStatus_t SysTick_Stop (void);

SysTick_ErrorStatus_t SysTick_Clear_Value (void);

SysTick_ErrorStatus_t SysTick_CLK_Source(SysTick_CLK_Source_t Cpy_SysTick_Clk_Source);

SysTick_ErrorStatus_t SysTick_Enable_Exeption (void);

SysTick_ErrorStatus_t SysTick_Set_Period_ms (u64 Cpy_Period_ms);

SysTick_ErrorStatus_t SysTick_Set_CBF(cbf Add_cbf_Function);
#endif /* MCAL_SYSTICK_H_ */
