/*
 * SSegment_Cfg.c
 *
 *  Created on: Feb 22, 2023
 *      Author: Peter Ghali
 */
#include "STD_TYPES.h"

#include "GPIO.h"


#include "SSegment.h"
#include "SSegment_Cfg.h"





const SSegment_strSSegmentCfg_t SSegment_strSSegmentCfg [SSegmentS_Count] =
{
		//THE right most number
		[SSegment_Alram] =
		{
				.Type = SSegment_Common_Anode ,
				.Port = GPIO_PORT_A ,
				.Pins =
				{
						GPIO_PIN_0 ,
						GPIO_PIN_1 ,
						GPIO_PIN_2 ,
						GPIO_PIN_3 ,
						GPIO_PIN_4 ,
						GPIO_PIN_5 ,
						GPIO_PIN_6 ,
						GPIO_PIN_7 ,
				},
				.Init_Number = ZERO ,
				.Speed = GPIO_Speed_Very_High ,
				.Name = "SSegment_Alaram" ,

		} ,


				[SSegment_2] =
				{
						.Type = SSegment_Common_Anode ,
						.Port = GPIO_PORT_B ,
						.Pins =
						{
								GPIO_PIN_0 ,
								GPIO_PIN_1 ,
								GPIO_PIN_2 ,
								GPIO_PIN_3 ,
								GPIO_PIN_4 ,
								GPIO_PIN_5 ,
								GPIO_PIN_6 ,
								GPIO_PIN_7 ,
						},
						.Init_Number = ZERO ,
						.Speed = GPIO_Speed_Very_High ,
						.Name = "SSegment_2" ,

				} ,

};
