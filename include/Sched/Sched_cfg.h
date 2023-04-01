/*
 * Sched_cfg.h
 *
 *  Created on: Mar 18, 2023
 *      Author: Peter Ghali
 */

#ifndef SCHED_SCHED_CFG_H_
#define SCHED_SCHED_CFG_H_

#define 	TICK_TIME	1


typedef void(*runnable_cbf)(void);

typedef struct
{
	//the function to be called
	runnable_cbf cbf;
	//how often the rubbanel should be excuted
	u32 Period_ms ;
	//the first dealy of this runnable
	u32 Start_Dealyms ;
}runnable_t;

typedef enum {

	LCD_T,
	LCD_APP,
	LED1,
	switch_app,
	switch_de,
	Number_Of_Runnable ,
}Runnable_Name_t ;


extern const runnable_t Sched_strRunnable_cfg[Number_Of_Runnable];

#endif /* SCHED_SCHED_CFG_H_ */
