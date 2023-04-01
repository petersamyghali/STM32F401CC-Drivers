/*
 * Sched_cfg.c
 *
 *  Created on: Mar 18, 2023
 *      Author: Peter Ghali
 */

#include "Bit_Math.h"
#include "STD_TYPES.h"

#include "Sched.h"
#include "Sched_cfg.h"

extern void LCD_Task_F(void) ;
extern void LCD_APP_F(void) ;
extern void UART_F(void) ;
extern void Switch_Application(void) ;
extern void Switch_Task(void) ;



const runnable_t Sched_strRunnable_cfg[Number_Of_Runnable]=
{

		[LED1]
		 {
			.cbf = UART_F,
					.Period_ms = 1000,
					.Start_Dealyms = 0,

		 },

		 [LCD_T]
		  {
			 .cbf = LCD_Task_F,
					 .Period_ms = 2,
					 .Start_Dealyms = 0,

		  },
		  [LCD_APP]
		   {

			  .cbf = LCD_APP_F,
					  .Period_ms = 1000,
					  .Start_Dealyms = 25,
		   },
		   [switch_app]
			{

			   .cbf = Switch_Application,
					   .Period_ms = 100,
					   .Start_Dealyms = 25,
			},
			[switch_de]
			 {

				.cbf = Switch_Task,
						.Period_ms = 5,
						.Start_Dealyms = 0,
			 },
};


