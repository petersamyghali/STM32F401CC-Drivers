/*
 * USART.h
 *
 *  Created on: Mar 28, 2023
 *      Author: Peter Ghali
 */

#ifndef MCAL_USART_H_
#define MCAL_USART_H_

/*callback function*/
typedef void(*cbf)(void) ;

typedef enum
{
	USART_TRANSMITTER			= 0x00000008,
	USART_RECIVER				= 0x00000004,
	USART_TRANSMITTER_RECIVER	= 0x0000000C,

}USART_MODE_t;


typedef enum
{
	USART_ONE_STOP_BIT = 0x00000000,
	USART_TWO_STOP_BIT = 0x00002000,
}USART_STOP_BITS_t;

typedef enum
{
	USART_PARITY_BIT_EVEN 	 = 0x00000400 ,
	USART_PARITY_BIT_ODD	 = 0x00000600,
	USART_PARITY_BIT_DISABLE = 0x00000000,
}USART_PARITY_BIT_t;

typedef enum
{
	USART_EIGHT_DATA_BITS = 0x00000000,
	USART_NINE_DATA_BITS  = 0x00001000,
}USART_DATA_BITS_NUMBER_t;
typedef enum
{
	OVER_SAMPLING_BY_16 = 0x00000000,
	OVER_SAMPLING_BY_8  = 0x00008000 ,

}USART_Over_Sampling_MODE;

typedef enum
{
	USART_1 = 0x240011000 ,
	USART_2 = 0x240004400 ,
	USART_6 = 0x240011400  ,
}USART_Channel_t;

typedef struct
{
	USART_Channel_t			Channel ;
	USART_MODE_t 	 		 Mode ;
	u32				 		 BaudRate ;
	USART_STOP_BITS_t 		 Stop_Bits ;
	USART_PARITY_BIT_t 		 Parity_Bit ;
	USART_DATA_BITS_NUMBER_t Data_Bits ;
	USART_Over_Sampling_MODE Over_Sampling ;
}USART_CFG_t ;

typedef enum
{
	Transmit_Data_Register_Empty = 0x00000080 ,
	Transmission_Complete		 = 0x00000040 ,
	Received_Data_Ready 	 	 = 0x00000020 ,
	Overrun_Error			     = 0x00000020 ,
	Idle_Line_Detected 			 = 0x00000010 ,
	Parity_Error				 = 0x00000100 ,
}USART_INT_t;
typedef enum
{
	USART_OK = 0 ,
	USART_NOK ,
	USART_NULL_PTR ,
	USART_Inavalid_Parametrs,
	USART_Inavalid_Mode,
	USART_Inavalid_Channel,
	USART_TX_BUSY ,
	USART_RX_BUSY ,
}USART_Error_Status_t;
/*A function to init the required USART with the given configuration */
USART_Error_Status_t USART_enuInit(USART_CFG_t * Add_str_USART_cfg) ;

/*A function to disable the given USART*/
USART_Error_Status_t USART_enuDisable(USART_Channel_t  Cpy_USART_Channel) ;

USART_Error_Status_t USART_enuEnable_INT(USART_Channel_t Cpy_USART_Channel , USART_INT_t Cpy_INTs) ;

//USART_Error_Status_t USART_enuTransmit_Byte_Async(USART_Channel_t Cpy_USART_Channel, u8 Cpy_Byte  , cbf ADD_Function);

//USART_Error_Status_t USART_enuRecive_Byte_Async (USART_Channel_t Cpy_USART_Channel ,u8 *ADD_Byte , cbf ADD_Function);

USART_Error_Status_t USART_enuTransmit_Buffer_Zero_cpy (USART_Channel_t Cpy_USART_Channel, u8 *Cpy_Buffer , u16 CpyBufffer_Size , cbf ADD_Function);

USART_Error_Status_t USART_enuRecive_Buffer (USART_Channel_t Cpy_USART_Channel ,u8 *Cpy_Buffer , u16 CpyBufffer_Size, cbf ADD_Function);


#endif /* MCAL_USART_H_ */
