/*
 * SSegment_Cfg.h
 *
 *  Created on: Feb 22, 2023
 *      Author: Peter Ghali
 */

#ifndef HAL_SSEGMENT_CFG_H_
#define HAL_SSEGMENT_CFG_H_

typedef enum
{
	SSegment_Alram ,
	SSegment_2,
	SSegmentS_Count,
}SSegment_Names_t;


extern u8 SSegment_LookUpTable [NUMBERS_DISPLAYED];

extern const SSegment_strSSegmentCfg_t SSegment_strSSegmentCfg [SSegmentS_Count];



#endif /* HAL_SSEGMENT_CFG_H_ */
