/*
 * SysTick.c
 *
 *  Created on: Mar 7, 2023
 *      Author: Peter Ghali
 */
#include "Bit_Math.h"
#include "STD_TYPES.h"
#include "SysTick.h"
#include "Clock_Control.h"
#include "RCC.h"

#define SYS_TICK_BAEE_ADD		0xE000E010
#define SYS_TICK_ENABLE_BIT		0
#define SYS_TICK_TICKINT		1
#define SYS_TICK_CLK_SOURCE_POS	2
#define SYS_TICK_CLEAR			0xFFFFFF
#define ENABLE 					1
#define DISABLE					0

#define MAX_TICKS				268435455

static SysTick_CLK_Source_t SYSTIC_SRC (void);
static cbf cbf_SYSTICK_Global  = NULL  ;
typedef struct
{
	u32 STK_CTRL;
	u32 STK_LOAD;
	u32 STK_VAL ;
	u32 STK_CALIB;
}SysTick_Resgister_t;


static SysTick_Resgister_t* SysTick = (SysTick_Resgister_t*)SYS_TICK_BAEE_ADD ;


SysTick_ErrorStatus_t SysTick_Start (void)
{
	SysTick_ErrorStatus_t Error_Status = SysTick_OK ;
	SET_BIT(SysTick->STK_CTRL,SYS_TICK_ENABLE_BIT) ;
	return Error_Status ;

}

SysTick_ErrorStatus_t SysTick_Stop (void)
{
	SysTick_ErrorStatus_t Error_Status = SysTick_OK ;
	SysTick->STK_CTRL = DISABLE << SYS_TICK_ENABLE_BIT ;
	return Error_Status ;

}

SysTick_ErrorStatus_t SysTick_Enable_Exeption (void)
{
	SysTick_ErrorStatus_t Error_Status = SysTick_OK ;
	SET_BIT(SysTick->STK_CTRL , SYS_TICK_TICKINT) ;
	return Error_Status ;

}


SysTick_ErrorStatus_t SysTick_CLK_Source(SysTick_CLK_Source_t Cpy_SysTick_Clk_Source)
{
	SysTick_ErrorStatus_t Error_Status = SysTick_OK ;
	SysTick->STK_CTRL = Cpy_SysTick_Clk_Source << SYS_TICK_CLK_SOURCE_POS ;
	return Error_Status ;

}


SysTick_ErrorStatus_t SysTick_Clear_Value (void)
{
	SysTick_ErrorStatus_t Error_Status = SysTick_OK ;
	SysTick->STK_VAL= SYS_TICK_CLEAR ;
	return Error_Status ;
}

SysTick_ErrorStatus_t SysTick_Set_CBF(cbf Add_cbf_Function)
{
	SysTick_ErrorStatus_t Error_Status = SysTick_OK ;
	if (Add_cbf_Function != NULL )
	{
		cbf_SYSTICK_Global = Add_cbf_Function ;
	}
	else

	{
		Error_Status = SysTick_NULL_PTR ;
	}
	return Error_Status ;
}


void SysTick_Handler(void)
{
	if (cbf_SYSTICK_Global != NULL)
	{
		cbf_SYSTICK_Global();
	}
	else
	{

	}
}

SysTick_ErrorStatus_t SysTick_Set_Period_ms (u64 Cpy_Period_ms)
{
	SysTick_ErrorStatus_t Error_Status = SysTick_OK ;
	f64	Ticks = 0 ;
	SysTick_CLK_Source_t AHB = SYSTIC_SRC();
	f64 Speed = Get_SYS_CLK_SPEED();
	Speed*=1000;


	if (AHB == SysTick_CLK_Source_AHB)
	{
		Ticks = Cpy_Period_ms * Speed ;
		if (Ticks > MAX_TICKS)
		{
			Error_Status = SysTick_InavalidParametrs ;

		}
		else {
			SysTick->STK_LOAD = (u32)Ticks ;
		}

	}
	else if (AHB ==SysTick_CLK_Source_AHB_Div_8)

	{
		Ticks = (Cpy_Period_ms * Speed )/8 ;
		if (Ticks > MAX_TICKS)
		{
			Error_Status = SysTick_InavalidParametrs ;

		}
		else {
			SysTick->STK_LOAD = (u32)Ticks ;
		}
	}

	return Error_Status ;

}

SysTick_CLK_Source_t SYSTIC_SRC (void)
{
	return (GET_BIT(SysTick->STK_CTRL,SYS_TICK_CLK_SOURCE_POS));
}
