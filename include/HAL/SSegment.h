/*
 * SSegment.h
 *
 *  Created on: Feb 22, 2023
 *      Author: Peter Ghali
 */

#ifndef HAL_SSEGMENT_H_
#define HAL_SSEGMENT_H_

#include "STD_TYPES.h"

#define MAX_NUM_DISPLAYED		9
#define NUMBERS_DISPLAYED		10
#define SSEGMENT_NAME_LENGHT				100


typedef enum{
	SSegment_enuOk = 0,
	SSegment_enuNotOk,
	SSegment_enuNullPtr,
	SSegment_enuInvalidPINNumber,
	SSegment_enuInvalidPINType,
	SSegment_enuInvalidNumber
}SSegment_enuErrorStatus;

typedef enum {

	SSegment_enu_SSegment0 = 0 ,
	SSegment_enu_SSegment1 ,
	SSegment_enu_SSegment2 ,
	SSegment_enu_SSegment3 ,

}SSegment_tenuSSegmentCount;

typedef enum
{
	SSegment_Common_Anode = 0 ,
	SSegment_Common_Cathode = 1 ,
}SSegment_Type_t ;

typedef enum {
	ZERO	=0,
	ONE	 ,
	TWO	 ,
	THREE ,
	FOUR ,
	FIVE ,
	SIX	 ,
	SEVEN ,
	EIGHT ,
	NINE ,
}SSegment_tenuSSegmentNumber;

typedef struct
{
	GPIO_enuPORT_t Port ;
	GPIO_enuPin_t Pins[MAX_NUM_DISPLAYED];
	GPIO_enuSpeed_t Speed ;
	SSegment_Type_t Type ;
	SSegment_tenuSSegmentNumber Init_Number ;
	u8 Name[SSEGMENT_NAME_LENGHT];
}SSegment_strSSegmentCfg_t;


SSegment_enuErrorStatus SSegment_enuInit(void);

SSegment_enuErrorStatus SSegment_enuSetNumber (u16 Cpy_u8Number);


#endif /* HAL_SSEGMENT_H_ */
