/*
 * Sched.c
 *
 *  Created on: Mar 18, 2023
 *      Author: Peter Ghali
 */
#include "Bit_Math.h"
#include "STD_TYPES.h"

#include "SysTick.h"

#include "Sched.h"
#include "Sched_cfg.h"

#define DISABLE 	0
#define ENABLE		1

u8 Sched_Flag = DISABLE ;
u8 CPU_LOAD_100 = DISABLE ;

typedef struct
{
	const runnable_t * Task_Info ;
	u32 Remain_Time ;
}Tasks_t;

Tasks_t   Tasks [Number_Of_Runnable];

static void  Tick_CBF(void);
static void 	Sched(void);


void Sched_Init (void)
{
	u8 counter = 0 ;
	SysTick_Enable_Exeption();
	SysTick_Set_CBF(Tick_CBF);
	SysTick_Set_Period_ms(TICK_TIME);
	for (counter = 0 ; counter < Number_Of_Runnable ; counter++)
	{
		Tasks[counter].Task_Info = &Sched_strRunnable_cfg[counter];
		Tasks[counter].Remain_Time = Sched_strRunnable_cfg[counter].Start_Dealyms;
	}

}

void Sched_Start(void)
{
	SysTick_Start();
	while(1)
	{
		if (Sched_Flag == ENABLE)
		{
			Sched();
			Sched_Flag= DISABLE;
		}
	}
}

void Sched(void)
{
	u8 counter=  0 ;
	for (counter = 0 ; counter < Number_Of_Runnable ; counter++)
	{
		if (Tasks[counter].Task_Info->cbf)
		{
			if (Tasks[counter].Remain_Time == 0 )
			{
				Tasks[counter].Task_Info->cbf();
				Tasks[counter].Remain_Time= Tasks[counter].Task_Info->Period_ms ;
			}
		}

		Tasks[counter].Remain_Time-=TICK_TIME ;


	}

}

void  Tick_CBF(void)
{
	if (Sched_Flag == DISABLE)
	{
		Sched_Flag = ENABLE ;
	}
	else
	{
		CPU_LOAD_100 = ENABLE ;
	}

}
