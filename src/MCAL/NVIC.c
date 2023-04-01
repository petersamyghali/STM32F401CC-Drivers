/*
 * NVIC.c
 *
 *  Created on: Mar 1, 2023
 *      Author: Peter Ghali
 */
#include <LIB/Bit_Math.h>
#include <LIB/STD_TYPES.h>
#include "NVIC.h"
/******************************************  MACROS  ************************************************************/
#define NVIC_BASE_ADD			0xE000E100
#define ARRAY_SIZE				8
#define RESERVES_ARRAY_SIZE		24
#define IPR_ARRAY_SIZE			240
#define STIR					*((volatile u32*)0xE000EF00)
#define REGISTER_SIZE			32
#define ENABLE					1
#define DIABEL					0
#define NUMBER_OF_CMSIS			16

#define OFFSET_CHECK			0x200
#define FLASH_BASE				0x08000000
#define VECTACTIVE_CLR_MSK		0x000001FF
#define SCB_AIRCR_PASS			0x5FA
#define SCB_AIRCR_PASS_POS		16
#define SCB_AIRCR_PRI_GROUP_Pos  8
#define SCB_AIRCR_PRI_GROUP_Msk  (7 << SCB_AIRCR_PRI_GROUP_Pos)
#define SCB_AIRCR_KEY_Pos      	16
#define SCB_AIRCR_KEY_Msk       (0xFFFF << SCB_AIRCR_VECTKEY_Pos)
#define BASE_2					2
#define NVIC_PRIO_BITS			4
#define SCB_BASE_ADD			 0xE000ED00
#define NMIPENDSET				31

/******************************************  TYPES  ************************************************************/

typedef struct
{
	u32 CPUID;
	u32 ICSR;
	u32 VTOR;
	u32 AIRCR;
	u32 SCR;
	u32 CCR;
	u32 SHP[12U];
	u32 SHCSR;
	u32 CFSR;
	u32 HFSR;
	u32 DFSR;
	u32 MMFAR;
	u32 BFAR;
	u32 AFSR;
	u32 PFR[2U];
	u32 DFR;
	u32 ADR;
	u32 MMFR[4U];
	u32 ISAR[5U];
	u32 RESERVED0[5U];
	u32 CPACR;
} SCB_Registers_t;



typedef struct
{
	u32 ISER[ARRAY_SIZE];
	u32 Reserved1[RESERVES_ARRAY_SIZE];
	u32 ICER[ARRAY_SIZE];
	u32 Reserved2[RESERVES_ARRAY_SIZE];
	u32 ISPR[ARRAY_SIZE];
	u32 Reserved3[RESERVES_ARRAY_SIZE];
	u32 ICPR[ARRAY_SIZE];
	u32 Reserved4[RESERVES_ARRAY_SIZE];
	u32 IABR[ARRAY_SIZE];
	u32 Reserved5[56];
	u8 IPR[IPR_ARRAY_SIZE];

}NVIC_Registers_t;

/********************************************  VARIABLES  ***********************************************************************/
static NVIC_Registers_t* NVIC = (NVIC_Registers_t*)NVIC_BASE_ADD ;
static SCB_Registers_t* SCB = (SCB_Registers_t*)SCB_BASE_ADD ;
static volatile u8 GROUP_Priority_Count = 0 ;
static volatile u8 SUB_Group_Priority_Count = 0 ;
static int Power(u8 Base , u8 Pow);
static u8 GROUP_SHIFT = 0 ;
static u16 SUB_GROUP_MSK = 0 ;
//static void Clear_Pending(IRQn_t IRQn);
/******************************************  FUNCTIONS IMPLEMENTATION  ************************************************************/

NVIC_ErrorStatus_t NVIC_EnableIRQ(IRQn_t IRQn)
{

	u8 Pre_NUM = IRQn % REGISTER_SIZE ;
	u8 POS =IRQn / REGISTER_SIZE ;
	NVIC_ErrorStatus_t Erro_Status = NVIC_OK ;
	if (IRQn  > IRQHandler_Count)
	{
		Erro_Status = NVIC_InavalidParametrs ;
	}
	else
	{
		SET_BIT(NVIC -> ISER[POS],Pre_NUM);
	}

	return Erro_Status ;
}

NVIC_ErrorStatus_t NVIC_DisableIRQ(IRQn_t IRQn)
{
	u8 Pre_NUM = IRQn % REGISTER_SIZE ;
	u8 POS =IRQn / REGISTER_SIZE ;
	NVIC_ErrorStatus_t Erro_Status = NVIC_OK ;
	if (IRQn  > IRQHandler_Count)
	{
		Erro_Status = NVIC_InavalidParametrs ;
	}
	else
	{
		SET_BIT(NVIC -> ICER[POS],Pre_NUM);
	}

	return Erro_Status ;
}

NVIC_ErrorStatus_t NVIC_SetPendingIRQ(IRQn_t IRQn)
{
	u8 Pre_NUM = IRQn % REGISTER_SIZE ;
	u8 POS =IRQn / REGISTER_SIZE ;
	NVIC_ErrorStatus_t Erro_Status = NVIC_OK ;
	if (IRQn  > IRQHandler_Count)
	{
		Erro_Status = NVIC_InavalidParametrs ;
	}
	else
	{
		SET_BIT(NVIC -> ISPR[POS],Pre_NUM);
	}

	return Erro_Status ;
}
/*
NVIC_ErrorStatus_t NVIC_ClearPendingIRQ(IRQn_t IRQn)
{
	u8 Pre_NUM = IRQn % REGISTER_SIZE ;
	u8 POS =IRQn / REGISTER_SIZE ;
	NVIC_ErrorStatus_t Erro_Status = NVIC_OK ;
	if (IRQn  > IRQHandler_Count)
	{
		Erro_Status = NVIC_InavalidParametrs ;
	}
	else
	{
		SET_BIT(NVIC -> ICPR[POS],Pre_NUM);
	}

	return Erro_Status ;
}
 */
NVIC_ErrorStatus_t NVIC_GetPendingIRQ(IRQn_t IRQn , NVIC_enu_State_t * Add_PendingStatus)
{
	u8 Pre_NUM = IRQn % REGISTER_SIZE ;
	u8 POS =IRQn / REGISTER_SIZE ;
	NVIC_ErrorStatus_t Erro_Status = NVIC_OK ;
	if (IRQn  > IRQHandler_Count)
	{
		Erro_Status = NVIC_InavalidParametrs ;
	}
	if (Add_PendingStatus == NULL)
	{
		Erro_Status   = NVIC_NULL_PTR ;
	}
	else
	{
		* Add_PendingStatus = GET_BIT(NVIC -> ICPR[POS],Pre_NUM);
	}

	return Erro_Status ;

}

NVIC_ErrorStatus_t NVIC_SetPriorityGrouping(NVIC_enuGroup_t Cpy_Number_of_Groups)
{
	NVIC_ErrorStatus_t Erro_Status = NVIC_OK ;
	u32 Temp_Reg = SCB->AIRCR ;

	switch (Cpy_Number_of_Groups)
	{
	case NVIC_GROUP_16:
		GROUP_Priority_Count = 16 ;
		SUB_Group_Priority_Count = 0 ;
		GROUP_SHIFT = 4;
		SUB_GROUP_MSK = 0b1111;
		break ;
	case NVIC_GROUP8:
		GROUP_Priority_Count = 8 ;
		SUB_Group_Priority_Count = 2 ;
		GROUP_SHIFT = 5;
		SUB_GROUP_MSK = 0b0111;
		break ;
	case NVIC_GROUP4:
		GROUP_Priority_Count = 4 ;
		SUB_Group_Priority_Count = 4 ;
		GROUP_SHIFT = 6;
		SUB_GROUP_MSK = 0b0011;
		break ;
	case NVIC_GROUP2:
		GROUP_Priority_Count = 2 ;
		SUB_Group_Priority_Count = 8 ;
		GROUP_SHIFT = 7;
		SUB_GROUP_MSK = 0b0001;
		break ;
	case NVIC_GROUP_None:
		GROUP_Priority_Count = 0 ;
		SUB_Group_Priority_Count = 16 ;
		GROUP_SHIFT = 0;
		SUB_GROUP_MSK = 0000;
		break ;
	default :
		Erro_Status = NVIC_InavalidParametrs ;
		break ;

	}
	Temp_Reg = SCB->AIRCR;
	Temp_Reg = SCB_AIRCR_PASS | Cpy_Number_of_Groups ;
	Temp_Reg &=~(SCB_AIRCR_PRI_GROUP_Msk) ;
	Temp_Reg|=Cpy_Number_of_Groups ;
	SCB->AIRCR = Temp_Reg  ;
	return Erro_Status ;
}


NVIC_ErrorStatus_t NVIC_SetPriority(IRQn_t IRQn  ,  NVIC_Groups_t Cpy_u8Group , NVIC_Sub_Groups_t Cpy_u8Sub_group )
{
	NVIC_ErrorStatus_t Erro_Status = NVIC_OK ;
	u32 Temp_Reg = 0 ;
	u8 LOC_SUB_Group_Priority_Count = Power(BASE_2, SUB_Group_Priority_Count) ;
	u8 LOC_GROUP_Priority_Count =  Power(BASE_2, GROUP_Priority_Count) ;
	if (IRQn  > IRQHandler_Count)
	{
		Erro_Status = NVIC_InavalidParametrs ;
	}
	if ((Cpy_u8Sub_group < LOC_SUB_Group_Priority_Count) ||(Cpy_u8Group < LOC_GROUP_Priority_Count) )
	{
		Erro_Status = NVIC_InavalidParametrs ;

	}
	else
	{

		Temp_Reg = (Cpy_u8Group<<GROUP_SHIFT) | ((Cpy_u8Sub_group))<<4;
		NVIC->IPR[IRQn] = Temp_Reg ;
	}

	return Erro_Status ;
}

int Power(u8 Base , u8 Pow)
{
	int value = Base ;
	while (Pow)
	{
		value*= value;
		Pow-- ;
	}
	return value;
}
void Clear_Pending(IRQn_t IRQn)
{
	u8 Pre_NUM = IRQn % REGISTER_SIZE ;
	u8 POS =IRQn / REGISTER_SIZE ;

	SET_BIT(NVIC -> ICPR[POS],Pre_NUM);

}

NVIC_ErrorStatus_t NVIC_SW_Trigger_INT(IRQn_t IRQn)
{
	NVIC_ErrorStatus_t Erro_Status = NVIC_OK ;
	if (IRQn  > IRQHandler_Count)
	{
		Erro_Status = NVIC_InavalidParametrs ;
	}
	else
	{
		STIR = IRQn ;
	}

	return Erro_Status ;
}

NVIC_ErrorStatus_t NVIC_GetPriority(IRQn_t IRQn , NVIC_Groups_t *Cpy_u8Group , NVIC_Sub_Groups_t *Cpy_u8Sub_group )

{
	NVIC_ErrorStatus_t Erro_Status = NVIC_OK ;
	if (IRQn  > IRQHandler_Count)
	{
		Erro_Status = NVIC_InavalidParametrs ;
	}
	if (Cpy_u8Group  == NULL || Cpy_u8Sub_group == NULL)
	{
		Erro_Status   = NVIC_NULL_PTR ;
	}
	else
	{

		*Cpy_u8Group = NVIC->IPR[IRQn] >>GROUP_SHIFT;
		*Cpy_u8Sub_group =  (NVIC->IPR[IRQn]>>NVIC_PRIO_BITS)&SUB_GROUP_MSK ;
	}
	return Erro_Status ;
}




NVIC_ErrorStatus_t NVIC_IsActive (IRQn_t IRQn , NVIC_enu_State_t * Add_PendingStatus)
{
	NVIC_ErrorStatus_t Erro_Status = NVIC_OK ;
	u8 Pre_NUM = IRQn % REGISTER_SIZE ;
	u8 POS =IRQn / REGISTER_SIZE ;

	if (IRQn  > IRQHandler_Count)
	{
		Erro_Status = NVIC_InavalidParametrs ;
	}
	else
	{

		*Add_PendingStatus = GET_BIT(NVIC -> IABR[POS], Pre_NUM);
	}
	return Erro_Status ;
}

NVIC_ErrorStatus_t NVIC_GetActive (IRQn_t *IRQn )
{
	NVIC_ErrorStatus_t Erro_Status = NVIC_OK ;
	*IRQn = (SCB->ICSR&VECTACTIVE_CLR_MSK) -NUMBER_OF_CMSIS  ;
	//SCB->ICSR &=VECTACTIVE_CLR_MSK ;
	return Erro_Status ;
}

void NVIC_NMI_Set_Pending(void)
{
	SET_BIT(SCB->ICSR , NMIPENDSET);
}
NVIC_ErrorStatus_t NVIC_Change_VT_Offset(VECT_TAB_OFFSET_t Cpy_Offset)
{
	NVIC_ErrorStatus_t Erro_Status = NVIC_OK ;
	if (Cpy_Offset % OFFSET_CHECK == 0 )
	{
		SCB->VTOR = FLASH_BASE | Cpy_Offset;
	}
	else
	{
		Erro_Status = NVIC_InavalidParametrs ;
	}

	return Erro_Status ;
}

void NVIC_Enable_IRQs(void)
{
	__asm("CPSIE I");

}
void NVIC_Disable_IRQs(void)
{
	asm volatile
	(
			"MOVS R0, #1 \n"
			"MSR PRIMASK, R0");

}
void NVIC_Enable_FAULTMASK(void)
{
	asm volatile
	(
			"MOVS R0, #0 \n"
			"MSR PRIMASK, R0");
}
void NVIC_Disable_FAULTMASK(void)
{
	__asm("CPSIE F");
}
/*
void NVIC_Disable_Interrupts_Lower_Than (  IRQn_t IRQn)
{
	const volatile u32 Temp = IRQn << 3 ;
	__asm volatile(
		"MOVS R1 ,[sp,#4]\n"
		"MSR BASEPRI, R1");


}
*/
