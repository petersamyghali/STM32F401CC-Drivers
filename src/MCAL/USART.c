/*
 * USART.c
 *
 *  Created on: Mar 28, 2023
 *      Author: Peter Ghali
 */
#include "STD_TYPES.h"
#include "Bit_Math.h"
#include "RCC.h"

#include "Clock_Control.h"

#include "USART.h"

/*****************	types	**************************/

typedef struct
{
	u32 USART_SR ;
	u32 USART_DR ;
	u32 USART_BRR ;
	u32 USART_CR1 ;
	u32 USART_CR2 ;
	u32 USART_CR3 ;
	u32 USART_GTPR ;
}USART_Reg_t ;
/*****************	MACROS	**************************/

#define USART_CHECK				 0xf00000000
#define USART_VALUE				 0x200000000
#define USART_CHECK_CLEAR		 0x0ffffffff

#define USART_CLEAR_DATA		 0xFFFFFF00
#define USART_ENABLE			 0x00002000
#define USART_ENABLE_POS		 13
#define USART_CR1_CLEAR_MASK	 0xFFFF49F3
#define USART_CR2_CLEAR_MASK	 0xFFFFCFFF

#define USART_BRR_CLEAR_MASK	 0xFFFF0000
#define MEGA_HERTZ				 1000000
#define OVER8_POS					15
#define DIV_Mantissa_POS			4

#define DATA_CLR_MASK			0xFFFFFF00


#define USART1						 0
#define USART2						 1
#define USART6						 2

#define	Transmit_Data_Register_Empty_INT 0
#define	Transmission_Complete_INT		 1
#define	Received_Data_Ready_INT	 		 2
#define	Overrun_Error_INT			     3
#define	Idle_Line_Detected_INT 			 4
#define	Parity_Error_INT				 5
#define NUMBER_OF_CHANNELS				 3
#define	NUMBER_OF_INTS					 6



#define	Transmit_Data_Register_Empty_FLAG 	 7
#define	Transmission_Complete_FLAG		 	 6
#define	Received_Data_Ready_FLAG 	 		 5
#define	Overrun_Error_FLAG			    	 3
#define	Idle_Line_Detected_FLAG 			 4
#define	Parity_Error_FLAG					 0

/*Register USART Addresses*/
#define USART1_BASE_ADDRESS	((USART_Reg_t*)0x40011000)
#define USART2_BASE_ADDRESS	((USART_Reg_t*)0x40004400)
#define USART6_BASE_ADDRESS	((USART_Reg_t*)0x40011400)

#define ENABLE		1
#define DISABLE		0




/*****************	VARIABLES	**************************/
/*2D array of pointer function each array have an array of the USART INTs*/
static cbf CBF_ARR[NUMBER_OF_CHANNELS][NUMBER_OF_INTS] = {NULL};

/*************************	TRANSMITTER	***********************************/
/*An array to have the buffered data of each USART*/
static volatile u8 TX_DATA_Buffeered[NUMBER_OF_CHANNELS] = {0} ;
/*An array to have the Address of the buffered data of each USART*/
static volatile u8 *TX_ADD_Buffers[NUMBER_OF_CHANNELS] = {0} ;
/*An array to hold the size of the TX_buufers*/
static volatile u16 TX_Buffer_Size[NUMBER_OF_CHANNELS] = {0} ;
/*An array to have the busy flag of each USART*/
static u8 TXE_Busy_Flag[NUMBER_OF_CHANNELS] = {DISABLE} ;
/*A variable to hold the index of the given buffer */
static volatile u16 TX_Current_Index[NUMBER_OF_CHANNELS] = {0} ;

/********************	Receiver ******************************/
/*An array to have the busy flag of each USART*/
static u8 RXE_Busy_Flag[NUMBER_OF_CHANNELS] = {DISABLE} ;
/*An array of pointer to u8 to have the address of the data to overwrite them when a new data hits*/
static volatile u8 *RX_ADD_Buffers[NUMBER_OF_CHANNELS] = {0} ;
/*An array to hold the size of the TX_buufers*/
static volatile u16 RX_Buffer_Size[NUMBER_OF_CHANNELS] = {0} ;
/*An array of u8 to have the addresses of the buffered array to overwite them */
static volatile u8 *RX_ADD_Buffeered[NUMBER_OF_CHANNELS] = {NULL} ;
/*A variable to hold the index of the given buffer */
static volatile u16 RX_Current_Index[NUMBER_OF_CHANNELS] = {0} ;
/************************	PROTOTYPES	***************************************/
/*A sataic function to enable and disable the USART Interrupts */
static void USART_Disable_INT(u32 Cpy_USART_Channel , USART_INT_t Cpy_INTs) ;
static void USART_Enable_INT(u32 Cpy_USART_Channel , USART_INT_t Cpy_INTs) ;

/*
 * 	USART_Channel_t			Channel ;
	USART_MODE_t 	 		 Mode ;
	u32				 		 BaudRate ;
	USART_STOP_BITS_t 		 Stop_Bits ;
	USART_PARITY_BIT_t 		 Parity_Bit ;
	USART_DATA_BITS_NUMBER_t Data_Bits ;
	USART_Over_Sampling_MODE Over_Sampling ;
 * */

/*****************	FUNCTIONS IMPLEMNTATION 	**************************/
USART_Error_Status_t USART_enuInit(USART_CFG_t * Add_str_USART_cfg)
{
	USART_Error_Status_t Error_Status = USART_OK ;
	f32 BBR_Value = 0 ;
	u32 Speed = 0  ,Temp_Reg = 0  , Real_Channel = 0 ;
	u8 DIV_Fraction = 0 ;
	u16 DIV_Mantissa = 0 ;
	Speed = Get_SYS_CLK_SPEED();
	Speed*=MEGA_HERTZ ;
	if ( Add_str_USART_cfg == NULL)
	{
		Error_Status = USART_NULL_PTR ;
	}
	/* the password I have implemnet in order to check if its is a valid address*/
	if (((Add_str_USART_cfg->Channel) &(USART_CHECK) )== USART_VALUE )
	{
		/*Clearing the password I have implemnet in order to check if its is a valid address*/
		Real_Channel = (Add_str_USART_cfg->Channel)& USART_CHECK_CLEAR ;
		/*Assign the CR1 reg to a temp register */
		Temp_Reg = ((USART_Reg_t*)(Real_Channel))->USART_CR1;
		/*Clearing all the previous configrations in order to change them by the new ones */
		Temp_Reg &= USART_CR1_CLEAR_MASK ;
		/*Assign the given parameters to the CR1 Reg. */
		Temp_Reg|=(Add_str_USART_cfg->Data_Bits)|(Add_str_USART_cfg->Mode)|(Add_str_USART_cfg->Over_Sampling)|(Add_str_USART_cfg->Parity_Bit);
		/*Assign the Reg with the new Confgirations to the right Register */
		((USART_Reg_t *)Real_Channel)->USART_CR1 = Temp_Reg ;
		/*Assign the CR1 reg to a temp register */
		Temp_Reg = ((USART_Reg_t*)(Real_Channel))->USART_CR2;
		/*Clearing all the previous configrations in order to change them by the new ones */
		Temp_Reg &= USART_CR2_CLEAR_MASK ;
		/*Assign the given parameters to the CR1 Reg. */
		Temp_Reg|=(Add_str_USART_cfg->Stop_Bits);
		/*Assign the Reg with the new Confgirations to the right Register */
		((USART_Reg_t *)Real_Channel)->USART_CR2 = Temp_Reg ;
		/*A way to get the fraction and the DIV_Mantissa */
		if (Add_str_USART_cfg->Over_Sampling == OVER_SAMPLING_BY_16)
		{
			BBR_Value = (u32)(16 * Add_str_USART_cfg->BaudRate);
			BBR_Value = BBR_Value/Speed ;
			BBR_Value = 1 / BBR_Value ;
			DIV_Mantissa= BBR_Value ;
			DIV_Fraction = (BBR_Value - DIV_Mantissa)*16 ;
		}
		else if (Add_str_USART_cfg->Over_Sampling == OVER_SAMPLING_BY_8)
		{
			BBR_Value = (u32)(8 * Add_str_USART_cfg->BaudRate);
			BBR_Value = BBR_Value/Speed ;
			BBR_Value = 1 / BBR_Value ;
			DIV_Mantissa= BBR_Value ;
			DIV_Fraction = (BBR_Value - DIV_Mantissa)*8 ;
		}

		Temp_Reg = 	((USART_Reg_t *)Real_Channel)->USART_BRR  ;
		Temp_Reg&=USART_BRR_CLEAR_MASK ;
		Temp_Reg |= ((DIV_Fraction)) |(DIV_Mantissa<<DIV_Mantissa_POS);
		((USART_Reg_t *)Real_Channel)->USART_BRR = Temp_Reg ;

		SET_BIT(((USART_Reg_t *)Real_Channel)->USART_CR1,USART_ENABLE_POS);
		((USART_Reg_t *)Real_Channel)->USART_SR = 0 ;
	}
	else
	{
		Error_Status = USART_Inavalid_Channel ;
	}


	return Error_Status ;
}
USART_Error_Status_t USART_enuDisable(USART_Channel_t  Cpy_USART_Channel)
{
	USART_Error_Status_t Error_Status = USART_OK ;
	u32 Real_Channel = 0 ;
	if (((Cpy_USART_Channel) &(USART_CHECK) )== USART_VALUE )
	{

		/*Clearing the password I have implemented in order to check if its is a valid address*/
		Real_Channel = (Cpy_USART_Channel)& USART_CHECK_CLEAR ;
		CLR_BIT(((USART_Reg_t *)Real_Channel)->USART_CR1,USART_ENABLE_POS);
	}
	return Error_Status ;
}

/*
USART_Error_Status_t USART_enuTransmit_Byte_Async(USART_Channel_t Cpy_USART_Channel , u8 Cpy_Byte , cbf ADD_Function)
{
	USART_Error_Status_t Error_Status = USART_OK ;
	u32  Real_Channel = 0 ;
	if (((Cpy_USART_Channel) &(USART_CHECK) )== USART_VALUE )
	{

Real_Channel = (Cpy_USART_Channel)& USART_CHECK_CLEAR ;


	switch (Cpy_USART_Channel)
	{
	case USART_1:
		if (TXE_Busy_Flag[USART1] == ENABLE )
		{
			Error_Status = USART_TX_BUSY;
		}
		else
		{
			TX_DATA_Buffeered[USART1] = Cpy_Byte;
			CBF_ARR[USART1][Transmit_Data_Register_Empty_INT]=ADD_Function ;
			TXE_Busy_Flag[USART1] = ENABLE ;
		}

		break ;
	case USART_2:
		if (TXE_Busy_Flag[USART2] == ENABLE )
		{
			Error_Status = USART_TX_BUSY;
		}
		else
		{
			TX_DATA_Buffeered[USART2] = Cpy_Byte;
			CBF_ARR[USART2][Transmit_Data_Register_Empty_INT]=ADD_Function ;
			TXE_Busy_Flag[USART2] = ENABLE ;
		}

		break ;
	case USART_6:
		if (TXE_Busy_Flag[USART6] == ENABLE )
		{
			Error_Status = USART_TX_BUSY;
		}
		else
		{
			TX_DATA_Buffeered[USART6] = Cpy_Byte;
			CBF_ARR[USART6][Transmit_Data_Register_Empty_INT]=ADD_Function ;
			TXE_Busy_Flag[USART6] = ENABLE ;
		}

		break ;
	}
	USART_Enable_INT(Real_Channel, Transmit_Data_Register_Empty);


}
else
{
	Error_Status = USART_Inavalid_Channel;
}
return Error_Status ;
}*/
/*
USART_Error_Status_t USART_enuRecive_Byte_Async (USART_Channel_t Cpy_USART_Channel ,u8 *ADD_Byte ,cbf ADD_Function )
{
	USART_Error_Status_t Error_Status = USART_OK ;
	u32  Real_Channel = 0 ;
	if (((Cpy_USART_Channel) &(USART_CHECK) )== USART_VALUE )
	{

Real_Channel = (Cpy_USART_Channel)& USART_CHECK_CLEAR ;

switch (Cpy_USART_Channel)
{
case USART_1:
	if (RXE_Busy_Flag[USART1] == ENABLE)
	{
		Error_Status = USART_RX_BUSY ;
	}
	else
	{
		RX_ADD_Buffeered[USART1] = ADD_Byte;
		CBF_ARR[USART1][Received_Data_Ready_INT]=ADD_Function ;
		RXE_Busy_Flag[USART1] = ENABLE ;
	}
	break ;
case USART_2:
	if (RXE_Busy_Flag[USART2] == ENABLE)
	{
		Error_Status = USART_RX_BUSY ;
	}
	else
	{
		RX_ADD_Buffeered[USART2] = ADD_Byte;
		CBF_ARR[USART2][Received_Data_Ready_INT]=ADD_Function ;
		RXE_Busy_Flag[USART2] = ENABLE ;
	}
	break ;
case USART_6:
	if (RXE_Busy_Flag[USART6] == ENABLE)
	{
		Error_Status = USART_RX_BUSY ;
	}
	else
	{
		RX_ADD_Buffeered[USART6] = ADD_Byte;
		CBF_ARR[USART6][Received_Data_Ready_INT]=ADD_Function ;
		RXE_Busy_Flag[USART6] = ENABLE ;
	}

	break ;
}
USART_Enable_INT(Real_Channel, Received_Data_Ready);
}
else
{
	Error_Status = USART_Inavalid_Channel;
}
return Error_Status ;
}*/
USART_Error_Status_t USART_enuEnable_INT(USART_Channel_t Cpy_USART_Channel , USART_INT_t Cpy_INTs)
{
	USART_Error_Status_t Error_Status = USART_OK ;
	u32 Temp_Reg = 0 ,  Real_Channel = 0 ;
	if (((Cpy_USART_Channel) &(USART_CHECK) )== USART_VALUE )
	{
		/*Clearing the password I have implemented in order to check if its is a valid address*/
		Real_Channel = (Cpy_USART_Channel)& USART_CHECK_CLEAR ;
		Temp_Reg = ((USART_Reg_t *)Real_Channel)->USART_CR1;
		Temp_Reg|=Cpy_INTs ;
		((USART_Reg_t *)Real_Channel)->USART_CR1 = Temp_Reg ;
	}
	else
	{
		Error_Status = USART_Inavalid_Channel;
	}
	return Error_Status ;
}


void USART_Enable_INT(u32 Cpy_USART_Channel , USART_INT_t Cpy_INTs)
{

	u32 Temp_Reg = 0 ;

	/*Clearing the password I have implemented in order to check if its is a valid address*/
	Temp_Reg = ((USART_Reg_t *)Cpy_USART_Channel)->USART_CR1;
	Temp_Reg|=Cpy_INTs ;
	((USART_Reg_t *)Cpy_USART_Channel)->USART_CR1 = Temp_Reg ;
}

void USART_Disable_INT(u32 Cpy_USART_Channel , USART_INT_t Cpy_INTs)
{
	u32 Temp_Reg = 0 ;
	/*copying the data of the register in a temp reg in order not ot corrupt the old data */
	Temp_Reg = ((USART_Reg_t *)Cpy_USART_Channel)->USART_CR1;
	Cpy_INTs = ~Cpy_INTs ;
	Temp_Reg &= Cpy_INTs ;
	((USART_Reg_t *)Cpy_USART_Channel)->USART_CR1 = Temp_Reg ;
}

USART_Error_Status_t USART_enuTransmit_Buffer_Zero_cpy (USART_Channel_t Cpy_USART_Channel, u8 *Cpy_Buffer , u16 CpyBufffer_Size,cbf ADD_Function)
{
	USART_Error_Status_t Error_Status = USART_OK ;
	u32  Real_Channel = 0 ;
	if (((Cpy_USART_Channel) &(USART_CHECK) )== USART_VALUE )
	{
		/*Clearing the password I have implemented in order to check if its is a valid address*/
		Real_Channel = (Cpy_USART_Channel)& USART_CHECK_CLEAR ;
		switch (Cpy_USART_Channel)
		{
		case USART_1:
			if (TXE_Busy_Flag[USART1] == ENABLE )
			{
				Error_Status = USART_TX_BUSY;
			}
			else
			{
				TX_ADD_Buffers[USART1] = Cpy_Buffer;
				TX_Buffer_Size[USART1] = CpyBufffer_Size ;
				TX_Current_Index[USART1] = 0 ;
				CBF_ARR[USART1][Transmit_Data_Register_Empty_INT]=ADD_Function ;
				TXE_Busy_Flag[USART1] = ENABLE ;
			}

			break ;
		case USART_2:
			if (TXE_Busy_Flag[USART2] == ENABLE )
			{
				Error_Status = USART_TX_BUSY;
			}
			else
			{
				TX_ADD_Buffers[USART2] = Cpy_Buffer;
				TX_Buffer_Size[USART2] = CpyBufffer_Size ;
				TX_Current_Index[USART2] = 0 ;
				CBF_ARR[USART2][Transmit_Data_Register_Empty_INT]=ADD_Function ;
				TXE_Busy_Flag[USART2] = ENABLE ;
			}

			break ;
		case USART_6:
			if (TXE_Busy_Flag[USART6] == ENABLE )
			{
				Error_Status = USART_TX_BUSY;
			}
			else
			{
				TX_ADD_Buffers[USART6] = Cpy_Buffer;
				TX_Buffer_Size[USART6] = CpyBufffer_Size ;
				TX_Current_Index[USART6] = 0 ;
				CBF_ARR[USART6][Transmit_Data_Register_Empty_INT]=ADD_Function ;
				USART6_BASE_ADDRESS->USART_DR = TX_ADD_Buffers[USART6][TX_Current_Index[USART1]];
				TX_Current_Index[USART6]++;
				TXE_Busy_Flag[USART6] = ENABLE ;
			}

			break ;
		}
		USART_Enable_INT(Real_Channel, Transmission_Complete);


	}
	else
	{
		Error_Status = USART_Inavalid_Channel;
	}
	return Error_Status ;
}

USART_Error_Status_t USART_enuRecive_Buffer (USART_Channel_t Cpy_USART_Channel ,u8 *Cpy_Buffer , u16 CpyBufffer_Size , cbf ADD_Function)
{
	USART_Error_Status_t Error_Status = USART_OK ;
	u32  Real_Channel = 0 ;
	if (Cpy_Buffer == NULL)
	{
		Error_Status = USART_NULL_PTR;
	}
	else
	{
		if (((Cpy_USART_Channel) &(USART_CHECK) )== USART_VALUE )
		{
			/*Clearing the password I have implement in order to check if its is a valid address*/
			Real_Channel = (Cpy_USART_Channel)& USART_CHECK_CLEAR ;
			switch (Cpy_USART_Channel)
			{
			case USART_1:
				if (RXE_Busy_Flag[USART1] == ENABLE)
				{
					Error_Status = USART_RX_BUSY ;
				}
				else
				{
					RX_ADD_Buffers[USART1] = Cpy_Buffer;
					RX_Buffer_Size[USART1] = CpyBufffer_Size ;
					RX_Current_Index[USART1] = 0 ;
					CBF_ARR[USART1][Received_Data_Ready_INT]=ADD_Function ;
					RXE_Busy_Flag[USART1] = ENABLE ;
				}
				break ;
			case USART_2:
				if (RXE_Busy_Flag[USART2] == ENABLE)
				{
					Error_Status = USART_RX_BUSY ;
				}
				else
				{
					RX_ADD_Buffers[USART2] = Cpy_Buffer;
					RX_Buffer_Size[USART2] = CpyBufffer_Size ;
					RX_Current_Index[USART2] = 0 ;
					CBF_ARR[USART2][Received_Data_Ready_INT]=ADD_Function ;
					RXE_Busy_Flag[USART2] = ENABLE ;
				}
				break ;
			case USART_6:
				if (RXE_Busy_Flag[USART6] == ENABLE)
				{
					Error_Status = USART_RX_BUSY ;
				}
				else
				{
					RX_ADD_Buffers[USART6] = Cpy_Buffer;
					RX_Buffer_Size[USART6] = CpyBufffer_Size ;
					RX_Current_Index[USART6] = 0 ;
					CBF_ARR[USART6][Received_Data_Ready_INT]=ADD_Function ;
					RXE_Busy_Flag[USART6] = ENABLE ;
				}

				break ;
			}
			USART_Enable_INT(Real_Channel, Received_Data_Ready);
		}
		else
		{
			Error_Status = USART_Inavalid_Channel;
		}
	}

	return Error_Status ;
}

void USART1_IRQHandler(void)
{
	if ((MEM_ADDR(BIT_BAND((u32)&(USART1_BASE_ADDRESS)->USART_SR,Received_Data_Ready_FLAG))) == ENABLE)
	{
		if (RXE_Busy_Flag[USART1] == ENABLE )
		{
			if (*RX_ADD_Buffeered[USART1])
			{
				USART1_BASE_ADDRESS->USART_DR = TX_DATA_Buffeered[USART1];
				*RX_ADD_Buffeered[USART1] = 0 ;
			}
			if (RX_Current_Index[USART1] < RX_Buffer_Size[USART1])
			{
				RX_ADD_Buffers[USART1][RX_Current_Index[USART1]] = (u8)USART1_BASE_ADDRESS->USART_DR ;
				RX_Current_Index[USART1]++ ;
			}
			else
			{
				RXE_Busy_Flag[USART1] = DISABLE ;
				USART_Disable_INT((u32)USART1_BASE_ADDRESS, Received_Data_Ready);
			}
			if (CBF_ARR[USART1][Received_Data_Ready_INT])
			{
				CBF_ARR[USART1][Received_Data_Ready_INT]();
			}
		}
	}

	if ((MEM_ADDR(BIT_BAND((u32)&(USART1_BASE_ADDRESS)->USART_SR,Transmit_Data_Register_Empty_FLAG))) == ENABLE)
	{
		if (TXE_Busy_Flag[USART1] == ENABLE )
		{
			if (TX_DATA_Buffeered[USART1])
			{
				USART1_BASE_ADDRESS->USART_DR = TX_DATA_Buffeered[USART1];
				TX_DATA_Buffeered[USART1] = 0 ;
			}
			if (TX_Current_Index[USART1] < TX_Buffer_Size[USART1])
			{
				USART1_BASE_ADDRESS->USART_DR = TX_ADD_Buffers[USART1][TX_Current_Index[USART1]];
				TX_Current_Index[USART1]++ ;
			}
			else
			{
				TXE_Busy_Flag[USART1] = DISABLE ;
				USART_Disable_INT((u32)USART1_BASE_ADDRESS, Transmit_Data_Register_Empty);
			}
		}

		if (CBF_ARR[USART1][Transmit_Data_Register_Empty_INT])
		{
			CBF_ARR[USART1][Transmit_Data_Register_Empty_INT]();
		}
	}

	if ((MEM_ADDR(BIT_BAND((u32)&(USART1_BASE_ADDRESS)->USART_SR,Transmission_Complete_FLAG))) == ENABLE)
	{

		if (CBF_ARR[USART1][Transmission_Complete_INT])
		{
			CBF_ARR[USART1][Transmission_Complete_INT]();
		}
	}



	if ((MEM_ADDR(BIT_BAND((u32)&(USART1_BASE_ADDRESS)->USART_SR,Overrun_Error_FLAG))) == ENABLE)
	{
		if (CBF_ARR[USART1][Overrun_Error_FLAG])
		{
			CBF_ARR[USART1][Overrun_Error_INT]();
		}
	}

	if ((MEM_ADDR(BIT_BAND((u32)&(USART1_BASE_ADDRESS)->USART_SR,Idle_Line_Detected_FLAG))) == ENABLE)
	{
		if (CBF_ARR[USART1][Idle_Line_Detected_INT])
		{
			CBF_ARR[USART1][Idle_Line_Detected_INT]();
		}
	}

	if ((MEM_ADDR(BIT_BAND((u32)&(USART1_BASE_ADDRESS)->USART_SR,Parity_Error_FLAG))) == ENABLE)
	{
		if (CBF_ARR[USART1][Parity_Error_INT])
		{
			CBF_ARR[USART1][Parity_Error_INT]();
		}
	}

}

void USART2_IRQHandler(void)
{
	if ((MEM_ADDR(BIT_BAND((u32)&(USART2_BASE_ADDRESS)->USART_SR,Received_Data_Ready_FLAG))) == ENABLE)
	{
		if (RXE_Busy_Flag[USART2] == ENABLE )
		{
			if (*RX_ADD_Buffeered[USART2])
			{
				USART2_BASE_ADDRESS->USART_DR = TX_DATA_Buffeered[USART2];
				*RX_ADD_Buffeered[USART2] = 0 ;
			}
			if (RX_Current_Index[USART2] < RX_Buffer_Size[USART2])
			{
				RX_ADD_Buffers[USART2][RX_Current_Index[USART2]] = (u8)USART2_BASE_ADDRESS->USART_DR ;
				RX_Current_Index[USART2]++ ;
			}
			else
			{
				RXE_Busy_Flag[USART2] = DISABLE ;
				USART_Disable_INT((u32)USART2_BASE_ADDRESS, Received_Data_Ready);
			}
			if (CBF_ARR[USART2][Received_Data_Ready_INT])
			{
				CBF_ARR[USART2][Received_Data_Ready_INT]();
			}
		}
	}

	if ((MEM_ADDR(BIT_BAND((u32)&(USART2_BASE_ADDRESS)->USART_SR,Transmit_Data_Register_Empty_FLAG))) == ENABLE)
	{
		if (TXE_Busy_Flag[USART2] == ENABLE )
		{
			if (TX_DATA_Buffeered[USART2])
			{
				USART2_BASE_ADDRESS->USART_DR = TX_DATA_Buffeered[USART2];
				TX_DATA_Buffeered[USART2] = 0 ;
			}
			if (TX_Current_Index[USART2] < TX_Buffer_Size[USART2])
			{
				USART2_BASE_ADDRESS->USART_DR = TX_ADD_Buffers[USART2][TX_Current_Index[USART2]];
				TX_Current_Index[USART2]++ ;
			}
			else
			{
				TXE_Busy_Flag[USART2] = DISABLE ;
				USART_Disable_INT((u32)USART2_BASE_ADDRESS, Transmit_Data_Register_Empty);
			}
		}

		if (CBF_ARR[USART2][Transmit_Data_Register_Empty_INT])
		{
			CBF_ARR[USART2][Transmit_Data_Register_Empty_INT]();
		}
	}

	if ((MEM_ADDR(BIT_BAND((u32)&(USART2_BASE_ADDRESS)->USART_SR,Transmission_Complete_FLAG))) == ENABLE)
	{

		if (CBF_ARR[USART2][Transmission_Complete_INT])
		{
			CBF_ARR[USART2][Transmission_Complete_INT]();
		}
	}



	if ((MEM_ADDR(BIT_BAND((u32)&(USART2_BASE_ADDRESS)->USART_SR,Overrun_Error_FLAG))) == ENABLE)
	{
		if (CBF_ARR[USART2][Overrun_Error_FLAG])
		{
			CBF_ARR[USART2][Overrun_Error_INT]();
		}
	}

	if ((MEM_ADDR(BIT_BAND((u32)&(USART2_BASE_ADDRESS)->USART_SR,Idle_Line_Detected_FLAG))) == ENABLE)
	{
		if (CBF_ARR[USART2][Idle_Line_Detected_INT])
		{
			CBF_ARR[USART2][Idle_Line_Detected_INT]();
		}
	}

	if ((MEM_ADDR(BIT_BAND((u32)&(USART2_BASE_ADDRESS)->USART_SR,Parity_Error_FLAG))) == ENABLE)
	{
		if (CBF_ARR[USART2][Parity_Error_INT])
		{
			CBF_ARR[USART2][Parity_Error_INT]();
		}
	}

}



void USART6_IRQHandler(void)
{
	if ((MEM_ADDR(BIT_BAND((u32)&(USART6_BASE_ADDRESS)->USART_SR,Received_Data_Ready_FLAG))) == ENABLE)
	{
		if (RXE_Busy_Flag[USART6] == ENABLE )
		{
			if (RX_Current_Index[USART6] < RX_Buffer_Size[USART6])
			{
				RX_ADD_Buffers[USART6][RX_Current_Index[USART6]] = (u8)USART6_BASE_ADDRESS->USART_DR ;
				RX_Current_Index[USART6]++ ;
			}
			else
			{
				RXE_Busy_Flag[USART6] = DISABLE ;
				USART_Disable_INT((u32)USART6_BASE_ADDRESS, Received_Data_Ready);
			}
			if (CBF_ARR[USART6][Received_Data_Ready_INT])
			{
				CBF_ARR[USART6][Received_Data_Ready_INT]();
			}
		}
	}

	if ((MEM_ADDR(BIT_BAND((u32)&(USART6_BASE_ADDRESS)->USART_SR,Transmit_Data_Register_Empty_FLAG))) == ENABLE)
	{


		if (CBF_ARR[USART6][Transmit_Data_Register_Empty_INT])
		{
			CBF_ARR[USART6][Transmit_Data_Register_Empty_INT]();
		}
	}

	if ((MEM_ADDR(BIT_BAND((u32)&(USART6_BASE_ADDRESS)->USART_SR,Transmission_Complete_FLAG))) == ENABLE)
	{
		if (TXE_Busy_Flag[USART6] == ENABLE )
		{
			if (TX_Current_Index[USART6] < TX_Buffer_Size[USART6])
			{
				USART6_BASE_ADDRESS->USART_DR = TX_ADD_Buffers[USART6][TX_Current_Index[USART6]];
				TX_Current_Index[USART6]++ ;
			}
			else
			{
				TXE_Busy_Flag[USART6] = DISABLE ;
				USART_Disable_INT((u32)USART6_BASE_ADDRESS, Transmission_Complete);
			}
		}

		if (CBF_ARR[USART6][Transmission_Complete_INT])
		{
			CBF_ARR[USART6][Transmission_Complete_INT]();
		}
	}



	if ((MEM_ADDR(BIT_BAND((u32)&(USART6_BASE_ADDRESS)->USART_SR,Overrun_Error_FLAG))) == ENABLE)
	{
		if (CBF_ARR[USART6][Overrun_Error_FLAG])
		{
			CBF_ARR[USART6][Overrun_Error_INT]();
		}
	}

	if ((MEM_ADDR(BIT_BAND((u32)&(USART6_BASE_ADDRESS)->USART_SR,Idle_Line_Detected_FLAG))) == ENABLE)
	{
		if (CBF_ARR[USART6][Idle_Line_Detected_INT])
		{
			CBF_ARR[USART6][Idle_Line_Detected_INT]();
		}
	}

	if ((MEM_ADDR(BIT_BAND((u32)&(USART6_BASE_ADDRESS)->USART_SR,Parity_Error_FLAG))) == ENABLE)
	{
		if (CBF_ARR[USART6][Parity_Error_INT])
		{
			CBF_ARR[USART6][Parity_Error_INT]();
		}
	}

}
