/*
 * NVIC.h
 *
 *  Created on: Mar 1, 2023
 *      Author: Peter Ghali
 */

#ifndef MCAL_NVIC_H_
#define MCAL_NVIC_H_

typedef enum
{
	IRQHandler_Reset =-3 ,
	NMI=-2,
	/*HardFault,
	MemManage,
	BusFault,
	UsageFault,
	SVCall,
	Debug_Monitor,
	PendSV,
	Systick,*/
	IRQHandler_WWDG=0                             ,
	IRQHandler_EXTI16__PVD                      ,
	IRQHandler_EXTI21__TAMP_STAMP               ,
	IRQHandler_EXTI22__RTC_WKUP                 ,
	IRQHandler_FLASH                            ,
	IRQHandler_RCC                              ,
	IRQHandler_EXTI0                            ,
	IRQHandler_EXTI1                            ,
	IRQHandler_EXTI2                            ,
	IRQHandler_EXTI3                            ,
	IRQHandler_EXTI4                            ,
	IRQHandler_DMA1_Stream0                     ,
	IRQHandler_DMA1_Stream1                     ,
	IRQHandler_DMA1_Stream2                     ,
	IRQHandler_DMA1_Stream3                     ,
	IRQHandler_DMA1_Stream4                     ,
	IRQHandler_DMA1_Stream5                     ,
	IRQHandler_DMA1_Stream6                     ,
	IRQHandler_ADC                              ,
	IRQHandler_EXTI9_5 = 23                       ,
	IRQHandler_TIM1_BRK_TIM9                    ,
	IRQHandler_TIM1_UP_TIM10                    ,
	IRQHandler_TIM1_TRG_COM_TIM11               ,
	IRQHandler_TIM1_CC                          ,
	IRQHandler_TIM2                             ,
	IRQHandler_TIM3                             ,
	IRQHandler_TIM4                             ,
	IRQHandler_I2C1_EV                          ,
	IRQHandler_I2C1_ER                          ,
	IRQHandler_I2C2_EV                          ,
	IRQHandler_I2C2_ER                          ,
	IRQHandler_SPI1                             ,
	IRQHandler_SPI2                             ,
	IRQHandler_USART1                           ,
	IRQHandler_USART2                           ,
	IRQHandler_EXTI15_10                        ,
	IRQHandler_EXTI17__RTC_Alarm	            ,
	IRQHandler_EXTI18__OTG_FS_WKUP              ,
	IRQHandler_DMA1_Stream7 = 47                  ,
	IRQHandler_SDIO = 49                             ,
	IRQHandler_TIM5                             ,
	IRQHandler_SPI3                             ,
	IRQHandler_DMA2_Stream0 = 59                     ,
	IRQHandler_DMA2_Stream1                     ,
	IRQHandler_DMA2_Stream2                     ,
	IRQHandler_DMA2_Stream3                     ,
	IRQHandler_DMA2_Stream4                     ,
	IRQHandler_OTG_FS = 67                           ,
	IRQHandler_DMA2_Stream5                     ,
	IRQHandler_DMA2_Stream6                     ,
	IRQHandler_DMA2_Stream7                     ,
	IRQHandler_USART6                           ,
	IRQHandler_I2C3_EV                          ,
	IRQHandler_I2C3_ER                          ,
	IRQHandler_FPU = 81                              ,
	IRQHandler_SPI4 = 84                            ,
	IRQHandler_Count ,
}IRQn_t;

typedef enum
{
	NVIC_OK = 0 ,
	NVIC_NOK ,
	NVIC_NULL_PTR ,
	NVIC_InavalidParametrs,
	NVIC_InavalidMode,
}NVIC_ErrorStatus_t;

typedef enum
{
	NVIC_HIGH = 0 ,
	NVIC_Low,
}NVIC_enu_State_t;

typedef enum
{
	NVIC_GROUP_16 =  0x05FA0300,
	NVIC_GROUP8 =0x05FA0400	,
	NVIC_GROUP4 =0x05FA0500	,
	NVIC_GROUP2= 0x05FA0600	,
	NVIC_GROUP_None =0x05FA0700,
}NVIC_enuGroup_t;
typedef enum
{
	Offset_0 = 0 ,
	offset_1 = 0x200 ,
	offset_2 = 0x400 ,
	offset_3 = 0x600 ,
	offset_4 = 0x800 ,
	offset_5 = 0xA00 ,
	offset_6 = 0xC00 ,
	offset_7 = 0xE00 ,
	offset_8 = 0x1000 ,
	offset_9 = 0x1200 ,
	offset_10 = 0x1400 ,
	offset_11 = 0x1600 ,
	offset_12 = 0x1800 ,
	offset_13 = 0x1A00 ,
	offset_14 = 0x1C00 ,
	offset_15 = 0x1E00 ,
	offset_16 = 0x2000 ,

}VECT_TAB_OFFSET_t;
typedef enum
{
	NVIC_Group0,
	NVIC_Group1,
	NVIC_Group2,
	NVIC_Group3,
	NVIC_Group4,
	NVIC_Group5,
	NVIC_Group6,
	NVIC_Group7,
	NVIC_Group8,
	NVIC_Group9,
	NVIC_Group10,
	NVIC_Group11,
	NVIC_Group12,
	NVIC_Group13,
	NVIC_Group14,
	NVIC_Group15,
}NVIC_Groups_t;

typedef enum
{
	NVIC_Sub_Group0,
	NVIC_Sub_Group1,
	NVIC_Sub_Group2,
	NVIC_Sub_Group3,
	NVIC_Sub_Group4,
	NVIC_Sub_Group5,
	NVIC_Sub_Group6,
	NVIC_Sub_Group7,
	NVIC_Sub_Group8,
	NVIC_Sub_Group9,
	NVIC_Sub_Group10,
	NVIC_Sub_Group11,
	NVIC_Sub_Group12,
	NVIC_Sub_Group13,
	NVIC_Sub_Group14,
	NVIC_Sub_Group15,

}NVIC_Sub_Groups_t;
/*********************************  Functions prototype  *******************************************************************************/

/*
 *Enable IRQ numberX
 *INPUT : IRQn
 *return Type : NVIC error status
 */
NVIC_ErrorStatus_t NVIC_EnableIRQ(IRQn_t IRQn);


/*
 *Disable IRQ numberX
 *INPUT : IRQn
 *return Type : NVIC error status
 */
NVIC_ErrorStatus_t NVIC_DisableIRQ(IRQn_t IRQn);


/*
 *Set the pending of IRQ numberX
 *INPUT : IRQn
 *return Type : NVIC error status
 */
NVIC_ErrorStatus_t NVIC_SetPendingIRQ(IRQn_t IRQn);


/*
 *clear the pending of IRQ numberX
 *INPUT : IRQn
 *return Type : NVIC error status
 */
NVIC_ErrorStatus_t NVIC_ClearPendingIRQ(IRQn_t IRQn);

/*
 *Get the pending of IRQ numberX
 *INPUT : IRQn  , add of a variable to know if it is pending or not
 *return Type : NVIC error status
 */
NVIC_ErrorStatus_t NVIC_GetPendingIRQ(IRQn_t IRQn , NVIC_enu_State_t * Add_PendingStatus);

/*
 *Set the number of groups in the system (preemption level)
 *INPUT : NVIC_enuGroup_t
 *return Type : NVIC error status
 */
NVIC_ErrorStatus_t NVIC_SetPriorityGrouping(NVIC_enuGroup_t Cpy_Number_of_Groups) ;

/*
 *Set the Priority of the given interrupt (group & sub group)
 *INPUT : NVIC_enuGroup_t
 *return Type : NVIC error status
 */

NVIC_ErrorStatus_t NVIC_SetPriority(IRQn_t IRQn  ,  NVIC_Groups_t Cpy_u8Group , NVIC_Sub_Groups_t Cpy_u8Sub_group );

/*
 *Get the Priority of the given interrupt (group & sub group)
 *INPUT : NVIC_enuGroup_t
 *return Type : NVIC error status
 */
NVIC_ErrorStatus_t NVIC_GetPriority(IRQn_t IRQn , NVIC_Groups_t *Cpy_u8Group , NVIC_Sub_Groups_t *Cpy_u8Sub_group );

/*
 *IS the number of given interrupt is active or not
 *INPUT : NVIC_enuGroup_t
 *return Type : NVIC error status
 */

NVIC_ErrorStatus_t NVIC_IsActive (IRQn_t IRQn , NVIC_enu_State_t * Add_PendingStatus);

/*
 *Get the number of the active interrupt
 *INPUT : NVIC_enuGroup_t
 *return Type : NVIC error status
 */

NVIC_ErrorStatus_t NVIC_GetActive (IRQn_t *IRQn );

/*
 *Trigger the interrupt
 *INPUT :IRQn
 *return Type : NVIC error status
 */

NVIC_ErrorStatus_t NVIC_SW_Trigger_INT(IRQn_t IRQn);

NVIC_ErrorStatus_t NVIC_Change_VT_Offset(VECT_TAB_OFFSET_t Cpy_Offset);

void NVIC_Enable_IRQs(void); // Clear PRIMASK
void NVIC_Disable_IRQs(void); // Set PRIMASK

void NVIC_Enable_FAULTMASK(void); // Clear FAULTMASK
void NVIC_Disable_FAULTMASK(void); // Set FAULTMASK

void NVIC_Disable_Interrupts_Lower_Than (IRQn_t IRQn);
/*
 *Set the pending of NMI IRQ
 *INPUT : void
 *return Type : void
 */


void NVIC_NMI_Set_Pending(void);

#endif /* MCAL_NVIC_H_ */
