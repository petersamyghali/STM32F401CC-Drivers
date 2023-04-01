/*
 * Clock_Control.c
 *
 *  Created on: Mar 7, 2023
 *      Author: Peter Ghali
 */

#include "STD_TYPES.h"
#include "Bit_Math.h"
#include "Clock_Control.h"
#include "RCC.h"

u8 Get_SYS_CLK_SPEED (void)
{
	f32 SYS_SPEED = 0  ;
	AHB_PRE_Scaler_t AHB ;
	PLL_FACT_N_t FACTN = 0 ;
	PLL_FACT_M_t FACTM = 0  ;
	PLL_FACT_P_t FACTP = 0;
	PLL_FACT_Q_t FACTQ = 0;
	RCC_PLL_SRC_t Source = 0 ;
	RCC_SYS_CLK_t Sys_Clk = 0 ;

	RCC_enuGetAHB_PreScaler(&AHB);

	RCC_enuGetSysClock(&Sys_Clk);
	if (Sys_Clk == RCC_SYS_CLK_PLL )
	{
		RCC_enuGet_PLL_Cfg(&Source , &FACTM , &FACTN , &FACTP , &FACTQ);
		if ( Source == RCC_PLL_SOURCE_HSI)
		{
			SYS_SPEED = ((f32)((f32)((SPEED_HSI ) * (FACTN)) /(FACTM)) /FACTP);
		}
		else if ( Source  == RCC_PLL_SOURCE_HSE)
		{
			SYS_SPEED = ((f32)((f32)((SPEED_HSE ) * (FACTN)) /(FACTM))/ FACTP);
		}

	}
	else if (Sys_Clk == RCC_SYS_CLK_HSE_CRYSTAL )
	{
		SYS_SPEED = SPEED_HSE  ;

	}
	else if (Sys_Clk == RCC_SYS_CLK_HSI )
	{
		SYS_SPEED = SPEED_HSI  ;
	}
	//if the system clk is divided using the AHB prescaler
	if (AHB != 0 )
	{
		switch (AHB)
		{
		case AHB_PRE_divided_by_2:
			SYS_SPEED/=2 ;
			break ;

		case  AHB_PRE_divided_by_4:
			SYS_SPEED/=2 ;
			break ;

		case AHB_PRE_divided_by_8:
			SYS_SPEED/=8 ;
			break ;

		case  AHB_PRE_divided_by_16:
			SYS_SPEED/=16 ;
			break ;

		case  AHB_PRE_divided_by_64:
			SYS_SPEED/=64 ;
			break ;

		case AHB_PRE_divided_by_128:
			SYS_SPEED/=128 ;
			break ;

		case  AHB_PRE_divided_by_256:
			SYS_SPEED/=256 ;
			break ;

		case  AHB_PRE_divided_by_512:
			SYS_SPEED/=512 ;
			break ;
		case AHB_PRE_not_divided:
			SYS_SPEED/=1 ;
			break ;
		}

	}
	return (u8)SYS_SPEED ;

}


