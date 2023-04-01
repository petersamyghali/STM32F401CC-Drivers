/*
 * SSegment.c
 *
 *  Created on: Feb 22, 2023
 *      Author: Peter Ghali
 */
#include "STD_TYPES.h"
#include "Bit_Math.h"

#include "GPIO.h"

#include "SSegment.h"
#include "SSegment_Cfg.h"



u8 SSegment_LookUpTable [NUMBERS_DISPLAYED] = {

		[ZERO]	= 0b0111111,
		[ONE]	= 0b0000110,
		[TWO]	= 0b1011011,
		[THREE] = 0b1001111,
		[FOUR]	= 0b1100110,
		[FIVE]	= 0b1101101,
		[SIX]	= 0b1111101,
		[SEVEN]	= 0b0000111,
		[EIGHT] = 0b1111111,
		[NINE]	= 0b1101111

};

SSegment_enuErrorStatus SSegment_enuInit(void)
{
	u8 PINS_Counter = ZERO ;
	u8 BIT = 0 ;
	u8 LocalCounter2  = ZERO;
	SSegment_enuErrorStatus Error_Status = SSegment_enuOk ;
	GPIO_Cfg_t GPIO_Cfg ;
	u8 Loc_Counter = 0 , Loc_counter2 = 0 ;
	for (Loc_Counter = 0 ; Loc_Counter < SSegmentS_Count ;Loc_Counter++ )
	{
		GPIO_Cfg.Mode = GPIO_Output_push_pull;
		GPIO_Cfg.Port = SSegment_strSSegmentCfg[Loc_Counter].Port ;
		GPIO_Cfg.Speed  =SSegment_strSSegmentCfg[Loc_Counter].Speed ;
		for (Loc_counter2 = 0 ; Loc_counter2 < MAX_NUM_DISPLAYED - 1 ;Loc_counter2++ )
		{
			GPIO_Cfg.Pin = SSegment_strSSegmentCfg[Loc_Counter].Pins[Loc_counter2] ;
			GPIO_Init_Pin(&GPIO_Cfg);
		}

	}

	//for loop to iterate by the number of the SSegments
	for (LocalCounter2  = ZERO ; LocalCounter2  < SSegmentS_Count ; LocalCounter2++)
	{
		//a for loop to light each led in the SSegment if it is a common Anode
		// 0 to light it up as the common pin is connected to the VCC
		for ( PINS_Counter = ZERO ; PINS_Counter  <  MAX_NUM_DISPLAYED - 1  ; PINS_Counter++ )
		{
			//go to the look up table and check each pin
			BIT = GET_BIT(SSegment_LookUpTable[ZERO],PINS_Counter);
			//if it is a 1 (LED ON)
			//clear the DIO pin to light it up
			GPIO_Write_Pin_Value(SSegment_strSSegmentCfg[LocalCounter2].Port,
					SSegment_strSSegmentCfg[LocalCounter2].Pins[PINS_Counter],
					BIT ^ 0 ^ SSegment_strSSegmentCfg[LocalCounter2].Type);
		}
	}
	//if the SSegmet is common cathode
	return Error_Status ;
}


SSegment_enuErrorStatus SSegment_enuSetNumber (u16 Cpy_u8Number)
{
	u8 LocalCounter  = ZERO;
	u8 PINS_Counter = ZERO ;
	u8 BIT = ZERO ;
	u8 Counter =  ZERO;
	u8 Counter2 = ZERO ;
	//An array of u8s to have split the given number to 4 digits separated
	u8 Loc_Temp_Number[4] = {0} ;
	u8 LocalCounter2  = ZERO;
	SSegment_enuErrorStatus Error_Status = SSegment_enuOk ;
	//a fore loop to split the given number
	for(LocalCounter  = ZERO ; Cpy_u8Number != 0 ; LocalCounter++)
	{
		//add the most right number to the first position in the array
		Loc_Temp_Number[LocalCounter] = Cpy_u8Number % 10 ;
		//divide the number by 10 to eliminate  the most right number
		Cpy_u8Number = Cpy_u8Number/10;
		//increment the counter
		Counter++ ;
	}
	Counter--;

	if (Cpy_u8Number > SSegmentS_Count * 9)
	{
		Error_Status = SSegment_enuInvalidNumber ;
	}

	else
	{

		for (LocalCounter2  = ZERO ; LocalCounter2  < SSegmentS_Count ; LocalCounter2++)
		{
			for ( PINS_Counter = ZERO ; PINS_Counter  <  MAX_NUM_DISPLAYED - 1  ; PINS_Counter++ )
			{
				//go to the look up table and check each pin
				BIT = GET_BIT(SSegment_LookUpTable[Loc_Temp_Number[LocalCounter2]],PINS_Counter);
				//if it is a 1 (LED ON)
				//clear the DIO pin to light it up
				GPIO_Write_Pin_Value(SSegment_strSSegmentCfg[LocalCounter2].Port,
						SSegment_strSSegmentCfg[LocalCounter2].Pins[PINS_Counter],
						BIT ^ 0 ^ SSegment_strSSegmentCfg[LocalCounter2].Type);
			}
			Counter2++;
		}
	}
	return Error_Status ;
}



