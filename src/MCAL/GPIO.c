/*
 * GPIO.c
 *
 *  Created on: Feb 14, 2023
 *      Author: Peter Ghali
 */
#include <LIB/Bit_Math.h>
#include <LIB/STD_TYPES.h>
#include <MCAL/GPIO.h>
/************************************* MACROS ****************************************************/
//some needed macros refers to the bit number
#define MODE_CLR_MASK	0b11
#define MODE_INPUT		0x00000000
#define	MODE_OUTPUT		0x00000001
#define	MODE_AF			0x00000002
#define	MODE_ANALOG		0x00000003
#define MODE_PINS_NUM		2


#define	SPEED_CLR_MASK	0b11
#define	SPEED_LOW		0x00000000
#define SPEED_MEDIUIM	0x00000001
#define SPEED_HIGH		0x00000002
#define SPEED_V_HIGH	0x00000003


#define NO_PU_PD		0x00000000
#define PULL_UP			0x00000001
#define PULL_DOWN		0x00000002
#define	PU_PD_CLR_MASK	0b11

#define PUSH_PULL		0
#define OPEN_Drain		1

#define OUTPUT_TYPE_MASK	0b1

#define PORT_SET		0xFFFF
#define PORT_RESET		0xFFFF0000

#define NUMBER_OF_GPIOs	6
#define LOCK_PIN_POS	16
#define ENABLE			1

#define PORT_CHECK			(u64)0xf00000000
#define PORT_Value			0x100000000
#define PORT_CHECK_CLEAR	0x0ffffffff

#define RESET_POS			16
#define AF_LOW_REG_SIZE		8
#define AF_CONFIG_PIN_SIZE	4

#define AF_CLR_MASK			0b11



/************************************* TYPES ****************************************************/
typedef struct
{
	volatile u32 GPIO_MODER ;
	volatile u32 GPIO_OTYPER ;
	volatile u32 GPIO_OSPEEDR ;
	volatile u32 GPIO_PUPDR ;
	volatile u32 GPIO_IDR ;
	volatile u32 GPIOx_ODR ;
	volatile u32 GPIO_BSRR ;
	volatile u32 GPIO_LCKR ;
	volatile u32 GPIO_AFRL ;
	volatile u32 GPIO_AFRH ;
}GPIO_Registers_t;

/********************************************  VARIABLES  ****************************************/

static GPIO_ErrorStatus_t PIN_INPUT(	GPIO_enuPORT_t Port 	 ,	GPIO_enuPin_t Pin);
static GPIO_ErrorStatus_t PIN_OUTPUT(GPIO_enuPORT_t Port 	 ,	GPIO_enuPin_t Pin);
static GPIO_ErrorStatus_t PIN_AF(		GPIO_enuPORT_t Port 	 ,	GPIO_enuPin_t Pin);
static GPIO_ErrorStatus_t PIN_ANALOG(	GPIO_enuPORT_t Port	 ,	GPIO_enuPin_t Pin);
static GPIO_ErrorStatus_t PIN_SPEED(	GPIO_enuPORT_t Port	 ,	GPIO_enuPin_t Pin , GPIO_enuSpeed_t Speed );
static GPIO_ErrorStatus_t PIN_OUTPUT_Type(	GPIO_enuPORT_t Port	 ,	GPIO_enuPin_t Pin , u32 Cpy_Type);
static GPIO_ErrorStatus_t PIN_PU_PD(	GPIO_enuPORT_t Port	 ,	GPIO_enuPin_t Pin , u32 PU_PD);
static u32 Real_PIN_NUMBER(GPIO_enuPin_t Pin);

/**********************************  FUNCTIONS IMPLEMENTATION  ***********************************/
GPIO_ErrorStatus_t GPIO_Init_Pin ( GPIO_Cfg_t * Add_GPIO_Cfg)
{
	GPIO_ErrorStatus_t Error_Status = GPIO_OK ;
	u8 LOC_PIN = 0 ;
	LOC_PIN = Real_PIN_NUMBER(Add_GPIO_Cfg->Pin);
	//LOC_PINin = LOC_PIN ;
	switch (Add_GPIO_Cfg->Mode)
	{
	case GPIO_Input_floating :
		PIN_INPUT(Add_GPIO_Cfg->Port, LOC_PIN);
		PIN_PU_PD(Add_GPIO_Cfg->Port , LOC_PIN , NO_PU_PD);
		break ;

	case GPIO_Input_pull_up :
		PIN_INPUT(Add_GPIO_Cfg->Port , LOC_PIN);
		PIN_PU_PD(Add_GPIO_Cfg->Port , LOC_PIN , PULL_UP);
		break ;

	case GPIO_Input_pull_down :
		PIN_INPUT(Add_GPIO_Cfg->Port , LOC_PIN);
		PIN_PU_PD(Add_GPIO_Cfg->Port , LOC_PIN , PULL_DOWN);
		break ;

	case GPIO_Analog :
		PIN_ANALOG(Add_GPIO_Cfg->Port , LOC_PIN);
		break ;

	case GPIO_Output_open_drain :
		PIN_OUTPUT(Add_GPIO_Cfg->Port , LOC_PIN);
		PIN_OUTPUT_Type(Add_GPIO_Cfg->Port , LOC_PIN , OPEN_Drain);
		break ;

	case GPIO_Output_open_drain_pull_up :
		PIN_OUTPUT(Add_GPIO_Cfg->Port , LOC_PIN);
		PIN_OUTPUT_Type(Add_GPIO_Cfg->Port , LOC_PIN , OPEN_Drain);
		PIN_PU_PD(Add_GPIO_Cfg->Port , LOC_PIN , PULL_UP);

		break ;

	case GPIO_Output_open_drain_pull_down :
		PIN_OUTPUT(Add_GPIO_Cfg->Port , LOC_PIN);
		PIN_OUTPUT_Type(Add_GPIO_Cfg->Port , LOC_PIN , OPEN_Drain);
		PIN_PU_PD(Add_GPIO_Cfg->Port , LOC_PIN , PULL_DOWN);

		break ;
	case GPIO_Output_push_pull:
		PIN_OUTPUT(Add_GPIO_Cfg->Port , LOC_PIN);
		PIN_OUTPUT_Type(Add_GPIO_Cfg->Port , LOC_PIN , PUSH_PULL);
		break ;

	case GPIO_Output_push_pull_pull_up :
		PIN_OUTPUT(Add_GPIO_Cfg->Port , LOC_PIN);
		PIN_OUTPUT_Type(Add_GPIO_Cfg->Port , LOC_PIN , PUSH_PULL);
		PIN_PU_PD(Add_GPIO_Cfg->Port , LOC_PIN , PULL_UP);


		break ;

	case GPIO_Output_push_pull_pull_down :
		PIN_OUTPUT(Add_GPIO_Cfg->Port , LOC_PIN);
		PIN_OUTPUT_Type(Add_GPIO_Cfg->Port , LOC_PIN , PUSH_PULL);
		PIN_PU_PD(Add_GPIO_Cfg->Port , LOC_PIN , PULL_DOWN);
		break ;

	case GPIO_Alternate_function_push_pull :
		PIN_AF(Add_GPIO_Cfg->Port , LOC_PIN);
		PIN_OUTPUT_Type(Add_GPIO_Cfg->Port , LOC_PIN , PUSH_PULL);
		break  ;

	case GPIO_Alternate_function_push_pull_pull_up :
		PIN_AF(Add_GPIO_Cfg->Port , LOC_PIN);
		PIN_OUTPUT_Type(Add_GPIO_Cfg->Port , LOC_PIN , PUSH_PULL);
		PIN_PU_PD(Add_GPIO_Cfg->Port , LOC_PIN , PULL_UP);

		break ;


	case GPIO_Alternate_function_push_pull_pull_down :
		PIN_AF(Add_GPIO_Cfg->Port , LOC_PIN);
		PIN_OUTPUT_Type(Add_GPIO_Cfg->Port , LOC_PIN , PUSH_PULL);
		PIN_PU_PD(Add_GPIO_Cfg->Port , LOC_PIN , PULL_DOWN);

		break ;

	case GPIO_Alternate_function_open_drain :
		PIN_AF(Add_GPIO_Cfg->Port , LOC_PIN);
		PIN_OUTPUT_Type(Add_GPIO_Cfg->Port , LOC_PIN , OPEN_Drain);
		break ;

	case GPIO_Alternate_function_open_drain_pull_up :
		PIN_AF(Add_GPIO_Cfg->Port , LOC_PIN);
		PIN_OUTPUT_Type(Add_GPIO_Cfg->Port , LOC_PIN , OPEN_Drain);
		PIN_PU_PD(Add_GPIO_Cfg->Port , LOC_PIN , PULL_UP);

		break ;

	case GPIO_Alternate_function_open_drain_pull_down :
		PIN_AF(Add_GPIO_Cfg->Port , LOC_PIN);
		PIN_OUTPUT_Type(Add_GPIO_Cfg->Port , LOC_PIN , OPEN_Drain);
		PIN_PU_PD(Add_GPIO_Cfg->Port , LOC_PIN , PULL_DOWN);

		break ;

	default :
		Error_Status = GPIO_InavalidPort ;
		break ;


	}
	PIN_SPEED(Add_GPIO_Cfg->Port , LOC_PIN , Add_GPIO_Cfg->Speed);
	return Error_Status ;
}

GPIO_ErrorStatus_t GPIO_SET_Alternate_Function(GPIO_enuPORT_t Cpy_Port ,GPIO_enuPin_t Cpy_pin , GPIO_AF_t Cpy_AF)
{
	GPIO_ErrorStatus_t Error_Status = GPIO_OK ;
	u64 TEMP_PORT = Cpy_Port ;
	u32 Temp_Reg = 0 ;
	u8 Real_PIN = 0 ;

	if (((TEMP_PORT &= PORT_CHECK) == (u64)PORT_Value) )
	{
		Real_PIN = Real_PIN_NUMBER(Cpy_pin);
		Cpy_Port = Cpy_Port & PORT_CHECK_CLEAR ;
		if (Real_PIN < AF_LOW_REG_SIZE)
		{

			Temp_Reg = ((GPIO_Registers_t*)Cpy_Port)->GPIO_AFRL  ;
			Temp_Reg &= (~(AF_CLR_MASK<<(Real_PIN*AF_CONFIG_PIN_SIZE)));
			Temp_Reg |=(Cpy_AF<<(Real_PIN*AF_CONFIG_PIN_SIZE));
			((GPIO_Registers_t*)Cpy_Port)->GPIO_AFRL = Temp_Reg ;
		}
		else
		{
			Real_PIN-=AF_LOW_REG_SIZE ;
			Temp_Reg = ((GPIO_Registers_t*)Cpy_Port)->GPIO_AFRH  ;
			Temp_Reg &= (u32)(~(AF_CLR_MASK<<(Real_PIN*AF_CONFIG_PIN_SIZE)));
			Temp_Reg |=(Cpy_AF<<(Real_PIN*AF_CONFIG_PIN_SIZE));
			((GPIO_Registers_t*)Cpy_Port)->GPIO_AFRH = Temp_Reg ;
		}

	}
	else
	{
		Error_Status = GPIO_InavalidPort ;
	}

	return Error_Status ;
}
GPIO_ErrorStatus_t GPIO_Write_Pin_Value(GPIO_enuPORT_t Cpy_Port ,GPIO_enuPin_t Cpy_pin , GPIO_Value_t Cpy_Value)
{
	GPIO_ErrorStatus_t Error_Status = GPIO_OK ;
	u64 TEMP_PORT = Cpy_Port ;

	if (((TEMP_PORT &= PORT_CHECK) == (u64)PORT_Value) )
	{
		Cpy_Port = Cpy_Port & PORT_CHECK_CLEAR ;
		((GPIO_Registers_t*)Cpy_Port)->GPIO_BSRR = (Cpy_pin<< Cpy_Value*RESET_POS) ;
	}
	else
	{
		Error_Status = GPIO_InavalidPort ;
	}

	return Error_Status ;
}

GPIO_ErrorStatus_t GPIO_Write_Port_Value(GPIO_enuPORT_t Cpy_Port  , GPIO_PORT_Value_t Cpy_Value)
{
	GPIO_ErrorStatus_t Error_Status = GPIO_OK ;
	u64 TEMP_PORT = Cpy_Port ;

	if (((TEMP_PORT &= PORT_CHECK) == (u64)PORT_Value) )
	{
		Cpy_Port = Cpy_Port & PORT_CHECK_CLEAR ;
		((GPIO_Registers_t*)Cpy_Port)->GPIO_BSRR = Cpy_Value ;
	}
	else
	{
		Error_Status = GPIO_InavalidPort ;
	}



	return Error_Status ;
}

GPIO_ErrorStatus_t PIN_INPUT(GPIO_enuPORT_t Port ,	GPIO_enuPin_t Pin)
{
	GPIO_ErrorStatus_t Error_Status = GPIO_OK ;
	u32 Loc_u32Temp_Reg = 0 ;
	u64 TEMP_PORT = Port ;

	if (((TEMP_PORT &= PORT_CHECK) == (u64)PORT_Value) )
	{
		Port = Port & PORT_CHECK_CLEAR ;
		Loc_u32Temp_Reg = (u32)(((GPIO_Registers_t*)Port)->GPIO_MODER);
		Loc_u32Temp_Reg &= ~((MODE_CLR_MASK)<< Pin * MODE_PINS_NUM ) ;
		Loc_u32Temp_Reg |=  (MODE_INPUT<<Pin * MODE_PINS_NUM ) ;
		((GPIO_Registers_t*)Port)->GPIO_MODER  = (u32)Loc_u32Temp_Reg ;
	}
	else
	{
		Error_Status = GPIO_InavalidPort ;
	}



	return Error_Status ;
}


GPIO_ErrorStatus_t PIN_OUTPUT(GPIO_enuPORT_t Port ,	GPIO_enuPin_t Pin)
{
	GPIO_ErrorStatus_t Error_Status = GPIO_OK ;
	volatile u32 Loc_u32Temp_Reg = 0 ;
	u64 TEMP_PORT = Port ;

	if (((TEMP_PORT &= PORT_CHECK) == (u64)PORT_Value) )
	{
		Port = Port & PORT_CHECK_CLEAR ;
		Loc_u32Temp_Reg = ((GPIO_Registers_t*)Port)->GPIO_MODER ;
		Loc_u32Temp_Reg &= ~((MODE_CLR_MASK)<< Pin * MODE_PINS_NUM ) ;
		Loc_u32Temp_Reg |=  (MODE_OUTPUT<<Pin * MODE_PINS_NUM ) ;
		((GPIO_Registers_t*)Port)->GPIO_MODER  = (u32)Loc_u32Temp_Reg ;
	}
	else
	{
		Error_Status = GPIO_InavalidPort ;
	}

	return Error_Status ;
}

GPIO_ErrorStatus_t PIN_AF(GPIO_enuPORT_t Port ,	GPIO_enuPin_t Pin)
{
	GPIO_ErrorStatus_t Error_Status = GPIO_OK ;
	u32 Loc_u32Temp_Reg = 0 ;
	u64 TEMP_PORT = Port ;

	if (((TEMP_PORT &= PORT_CHECK) == (u64)PORT_Value) )
	{
		Port = Port & PORT_CHECK_CLEAR ;
		Loc_u32Temp_Reg = ((GPIO_Registers_t*)Port)->GPIO_MODER ;
		Loc_u32Temp_Reg &= ~((MODE_CLR_MASK)<< Pin  ) ;
		Loc_u32Temp_Reg |=  (MODE_AF<<Pin * MODE_PINS_NUM ) ;
		((GPIO_Registers_t*)Port)->GPIO_MODER  = (u32)Loc_u32Temp_Reg ;
	}
	else
	{
		Error_Status = GPIO_InavalidPort ;
	}


	return Error_Status ;
}

GPIO_ErrorStatus_t PIN_ANALOG(GPIO_enuPORT_t Port ,	GPIO_enuPin_t Pin)
{
	GPIO_ErrorStatus_t Error_Status = GPIO_OK ;
	u32 Loc_u32Temp_Reg = 0 ;
	u64 TEMP_PORT = Port ;

	if (((TEMP_PORT &= PORT_CHECK) == (u64)PORT_Value) )
	{
		Port = Port & PORT_CHECK_CLEAR ;
		Loc_u32Temp_Reg = ((GPIO_Registers_t*)Port)->GPIO_MODER ;
		Loc_u32Temp_Reg &= ~((MODE_CLR_MASK)<< Pin * MODE_PINS_NUM ) ;
		Loc_u32Temp_Reg |=  (MODE_ANALOG<<Pin * MODE_PINS_NUM ) ;
		((GPIO_Registers_t*)Port)->GPIO_MODER  = (u32)Loc_u32Temp_Reg ;
	}
	else
	{
		Error_Status = GPIO_InavalidPort ;
	}


	return Error_Status ;
}

GPIO_ErrorStatus_t PIN_SPEED(	GPIO_enuPORT_t Port	 ,	GPIO_enuPin_t Pin , GPIO_enuSpeed_t Speed )
{

	GPIO_ErrorStatus_t Error_Status = GPIO_OK ;
	u32 Loc_u32Temp_Reg = 0 ;
	u64 TEMP_PORT = Port ;

	if (((TEMP_PORT &= PORT_CHECK) == (u64)PORT_Value) )
	{
		Port = Port & PORT_CHECK_CLEAR ;
		Loc_u32Temp_Reg = ((GPIO_Registers_t*)Port)->GPIO_OSPEEDR ;
		Loc_u32Temp_Reg &= ~((SPEED_CLR_MASK)<< Pin * MODE_PINS_NUM ) ;
		Loc_u32Temp_Reg |=  (Speed<<Pin * MODE_PINS_NUM ) ;
		((GPIO_Registers_t*)Port)->GPIO_OSPEEDR  = (u32)Loc_u32Temp_Reg ;
	}
	else
	{
		Error_Status = GPIO_InavalidPort ;
	}



	return Error_Status ;
}

GPIO_ErrorStatus_t PIN_PU_PD(	GPIO_enuPORT_t Port	 ,	GPIO_enuPin_t Pin , u32 PU_PD)
{
	GPIO_ErrorStatus_t Error_Status = GPIO_OK ;
	u32 Loc_u32Temp_Reg = 0 ;
	u64 TEMP_PORT = Port ;

	if (((TEMP_PORT &= PORT_CHECK) == (u64)PORT_Value) )
	{
		Port = Port & PORT_CHECK_CLEAR ;
		Loc_u32Temp_Reg = ((GPIO_Registers_t*)Port)->GPIO_PUPDR ;
		Loc_u32Temp_Reg &= ~((PU_PD_CLR_MASK)<< Pin * MODE_PINS_NUM ) ;
		Loc_u32Temp_Reg |=  (PU_PD<<Pin * MODE_PINS_NUM ) ;
		((GPIO_Registers_t*)Port)->GPIO_PUPDR  = (u32)Loc_u32Temp_Reg ;

	}

	else
	{
		Error_Status = GPIO_InavalidPort ;
	}
	return Error_Status ;

}
GPIO_ErrorStatus_t PIN_OUTPUT_Type(	GPIO_enuPORT_t Port	 ,	GPIO_enuPin_t Pin , u32 Cpy_Type)
{
	GPIO_ErrorStatus_t Error_Status = GPIO_OK ;
	u32 Loc_u32Temp_Reg = 0 ;
	u64 TEMP_PORT = Port ;

	if (((TEMP_PORT &= PORT_CHECK) == (u64)PORT_Value) )
	{
		Port = Port & PORT_CHECK_CLEAR ;
		Loc_u32Temp_Reg = ((GPIO_Registers_t*)Port)->GPIO_OTYPER ;
		Loc_u32Temp_Reg &= ~((OUTPUT_TYPE_MASK)<< Pin ) ;
		Loc_u32Temp_Reg |=  (Cpy_Type<< Pin ) ;
		((GPIO_Registers_t*)Port)->GPIO_OTYPER  = (u32)Loc_u32Temp_Reg ;

	}
	else
	{
		Error_Status = GPIO_InavalidPort ;
	}

	return Error_Status ;
}

u32 Real_PIN_NUMBER(GPIO_enuPin_t Pin)
{
	u32 LOC_u32PIN_Number = 0 ;
	while (GET_BIT(Pin,LOC_u32PIN_Number) == 0)
	{
		LOC_u32PIN_Number++ ;
	}
	return LOC_u32PIN_Number ;
}

GPIO_ErrorStatus_t GPIO_Get_Pin_Value(GPIO_enuPORT_t Cpy_Port ,GPIO_enuPin_t Cpy_pin , u8 * Add_Value)
{
	GPIO_ErrorStatus_t Error_Status = GPIO_OK ;
	u32 Loc_real_PIN_Number ;
	if (Add_Value == NULL)
	{
		Error_Status = GPIO_NULL_PTR ;
	}
	else
	{
		u64 TEMP_PORT = Cpy_Port ;

		if (((TEMP_PORT &= PORT_CHECK) == (u64)PORT_Value) )
		{
			Cpy_Port = Cpy_Port & PORT_CHECK_CLEAR ;
			Loc_real_PIN_Number = Real_PIN_NUMBER(Cpy_pin);

			//(MEM_ADDR(BIT_BAND((u32)&RCC -> RCC_CR,HSE_RDY));
			*Add_Value = MEM_ADDR(BIT_BAND((u32)&((GPIO_Registers_t*)Cpy_Port)->GPIO_IDR,Loc_real_PIN_Number));
		}
		else
		{
			Error_Status = GPIO_InavalidPort ;
		}


	}

	return Error_Status ;
}


GPIO_ErrorStatus_t GPIO_LOCK (GPIO_enuPORT_t Cpy_Port ,GPIO_enuPin_t Cpy_pin )
{
	GPIO_ErrorStatus_t Error_Status = GPIO_OK ;
	u32 Loc_u32Temp_Reg =  Cpy_pin;


	u64 TEMP_PORT = Cpy_Port ;

	if (((TEMP_PORT &= PORT_CHECK) == (u64)PORT_Value) )
	{
		Cpy_Port = Cpy_Port & PORT_CHECK_CLEAR ;
		Loc_u32Temp_Reg |= (ENABLE<<LOCK_PIN_POS)  ;
		((GPIO_Registers_t*)Cpy_Port)->GPIO_LCKR = Loc_u32Temp_Reg ;

		Loc_u32Temp_Reg &=	~(ENABLE<<LOCK_PIN_POS)  ;
		((GPIO_Registers_t*)Cpy_Port)->GPIO_LCKR = Loc_u32Temp_Reg ;

		Loc_u32Temp_Reg |=	(ENABLE<<LOCK_PIN_POS)  ;
		((GPIO_Registers_t*)Cpy_Port)->GPIO_LCKR = Loc_u32Temp_Reg ;

		Loc_u32Temp_Reg = ((GPIO_Registers_t*)Cpy_Port)->GPIO_LCKR ;
	}
	else
	{
		Error_Status = GPIO_InavalidPort ;
	}

	return Error_Status ;
}
