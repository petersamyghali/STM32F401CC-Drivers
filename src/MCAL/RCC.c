/*
 * RCC.c
 *
 *  Created on: Jan 28, 2023
 *      Author: Peter Ghali
 */
#include <LIB/Bit_Math.h>
#include <LIB/STD_TYPES.h>
#include <MCAL/RCC.h>


//some needed macros refers to the bit number
#define HSI_ON		0
#define HSI_RDY		1
#define HSE_ON		16
#define HSE_RDY		17
#define PLL_ON		24
#define PLL_RDY		25

#define FACTOR_Q_POS	24
#define FACTOR_P_POS	16
#define FACTOR_N_POS	6
#define FACTOR_M_POS	0
#define PLL_SOURCE 		22

#define AHB_PreScaler_POS	4
#define AHB_CLR_MSK 	 	0xFFFFFF0F
#define GET_AHB_PreScaler 	0x0000000F

#define SYS_SOURCE_CLR_MASK		0xFFFFFFFC

#define FACTOR_Q_CLR_MASK		0xF0FFFFFF
#define FACTOR_P_CLR_MASK		0xFFFCFFFF
#define FACTOR_N_CLR_MASK		0xFFFF803F
#define FACTOR_M_CLR_MASK		0xFFFFFFC0

#define GET_FACTOR_N			0x0
#define GET_FACTOR_P
#define GET_FACTOR_Q

#define ENABLE		1
#define DISABLE		0

//the System Clk prescaler
#define			SYSTEM_CLK_NOT_DIVIDED		0b0000
#define 		SYSTEM_CLK_DIVIDED_2		0b1000
#define 		SYSTEM_CLK_DIVIDED_4   	 	0b1001
#define 		SYSTEM_CLK_DIVIDED_8   	 	0b1010
#define 		SYSTEM_CLK_DIVIDED_16  	 	0b1011
#define 		SYSTEM_CLK_DIVIDED_64   	0b1100
#define 		SYSTEM_CLK_DIVIDED_128 	 	0b1101
#define 		SYSTEM_CLK_DIVIDED_256  	0b1110
#define 		SYSTEM_CLK_DIVIDED_512  	0b1111

//to control APB speed clock division factor.
#define		HCLK_NOT_DIVIDED    0b000
#define		HCLK_DIVIDED_2      0b100
#define		HCLK_DIVIDED_4      0b101
#define		HCLK_DIVIDED_8      0b110
#define		HCLK_DIVIDED_16		0b111

//division factor for main system clock(P)
#define		PLLP_DIVIDED_2		0b00
#define		PLLP_DIVIDED_4      0b01
#define		PLLP_DIVIDED_6      0b10
#define		PLLP_DIVIDED_8      0b11


#define TIME_OUT_TRIALS		10000

#define RCC_BASE_ADD	0x40023800
#define RCC_CR_OFFSET	0x00

#define SWS0_POS		2
#define	SWS1_POS		3
/******************************************  TYPES  ************************************************************/
typedef struct
{
	u32	RCC_CR ;
	u32 RCC_PLLCFGR ;
	u32 RCC_CFGR;
	u32 RCC_CIR ;
	u32 RCC_AHB1RSTR ;
	u32 RCC_AHB2RSTR ;

	u32 Reserved0;
	u32 Reserved1 ;

	u32 RCC_APB1RSTR;
	u32 RCC_APB2RSTR ;

	u32 Reserved2 ;
	u32 Reserved3 ;

	u32 RCC_AHB1ENR ;
	u32 RCC_AHB2ENR ;

	u32 Reserved4 ;
	u32 Reserved5 ;

	u32 RCC_APB1ENR ;
	u32 RCC_APB2ENR ;

	u32 Reserved6 ;
	u32 Reserved7 ;

	u32 RCC_AHB1LPENR ;
	u32 RCC_AHB2LPENR;

	u32 Reserved8 ;
	u32 Reserved9 ;

	u32 RCC_APB1LPENR ;
	u32 RCC_APB2LPENR ;

	u32 Reserved10 ;
	u32 Reserved11;

	u32 RCC_BDCR ;
	u32 RCC_CSR ;

	u32 Reserved12 ;
	u32 Reserved13 ;

	u32 RCC_SSCGR ;
	u32 RCC_PLLI2SCFGR ;

	u32 Reserved14 ;
	u32 RCC_DCKCFGR ;


}RCC_Registers_t;

/********************************************  VARIABLES  ***********************************************************************/
static RCC_Registers_t* RCC = (RCC_Registers_t*)RCC_BASE_ADD ;
static u32 LOC_u32TimeOut  = 0 ;
static RCC_tenuErrorStatus Error_Staus = RCC_enuOK ;
static volatile u32 LOCTemp_REG=0;

static RCC_tenuErrorStatus Select_HSE_SYS_CLK(void) ;
static u8 GetSysClock(u8 * Add_u8SysClock) ;
static RCC_tenuErrorStatus Select_PLL_SYS_CLK(void);
/******************************************  FUNCTIONS IMPLEMENTATION  ************************************************************/

RCC_tenuErrorStatus RCC_enuSetSysClock(RCC_SYS_CLK_t  Cpy_u8SysClock )
{
	Error_Staus = RCC_enuOK;
	LOC_u32TimeOut  = 0;
	u8 LocalSysClk2 =  5 ;
	LOCTemp_REG=0;
	switch (Cpy_u8SysClock)
	{
	case RCC_SYS_CLK_HSE_CRYSTAL:
		/*Enable HSE without bypass (NO RC) using a crystal*/
		SET_BIT( RCC -> RCC_CR , HSE_ON);
		/*wait until the HSE is ready or the Timeout occurs*/
		while ((MEM_ADDR(BIT_BAND((u32)&RCC -> RCC_CR,HSE_RDY)) == DISABLE) && LOC_u32TimeOut < TIME_OUT_TRIALS )
		{
			LOC_u32TimeOut++;
		}
		if (MEM_ADDR(BIT_BAND((u32)&RCC -> RCC_CR,HSE_RDY)))
		{
			LOCTemp_REG =( (RCC -> RCC_CFGR) & (SYS_SOURCE_CLR_MASK) );
			LOCTemp_REG |= RCC_SYS_CLK_HSE_CRYSTAL ;
			/*Select HSE as input clock*/
			RCC -> RCC_CFGR  = LOCTemp_REG;
			LocalSysClk2 = GetSysClock(&LocalSysClk2);
			while (LocalSysClk2 != RCC_SYS_CLK_HSE_CRYSTAL && LOC_u32TimeOut < TIME_OUT_TRIALS )
			{
				LOC_u32TimeOut++;
			}
			if (LocalSysClk2 == RCC_SYS_CLK_HSE_CRYSTAL)
			{
				Error_Staus = RCC_enuOK;
			}
			else if (LOC_u32TimeOut > TIME_OUT_TRIALS)
			{
				Error_Staus = RCC_enuTimeOUT ;
			}
		}
		else if (LOC_u32TimeOut > TIME_OUT_TRIALS)
		{
			Error_Staus = RCC_enuTimeOUT ;
		}
		break ;

	case RCC_SYS_CLK_PLL :

		/*PLL ON*/
		SET_BIT(RCC -> RCC_CR ,PLL_ON);
		/*wait until the PLL is ready or the Timeout occurs*/
		while ((MEM_ADDR(BIT_BAND((u32)&RCC -> RCC_CR,PLL_RDY)) == DISABLE ) && (LOC_u32TimeOut < TIME_OUT_TRIALS ))
		{
			LOC_u32TimeOut++;
		}
		if ((MEM_ADDR(BIT_BAND((u32)&RCC -> RCC_CR,PLL_RDY))))
		{
			LOCTemp_REG =(RCC -> RCC_CFGR) ;
			LOCTemp_REG&= (SYS_SOURCE_CLR_MASK);
			LOCTemp_REG |= RCC_SYS_CLK_PLL;
			/*Select PLL as input clock*/
			RCC -> RCC_CFGR  = LOCTemp_REG;
			LocalSysClk2 = GetSysClock(&LocalSysClk2);
			while (LocalSysClk2 != RCC_SYS_CLK_PLL && LOC_u32TimeOut < TIME_OUT_TRIALS )
			{
				LOC_u32TimeOut++;
			}
			if (LocalSysClk2 == RCC_SYS_CLK_PLL)
			{
				Error_Staus = RCC_enuOK;
			}
			else if (LOC_u32TimeOut > TIME_OUT_TRIALS)
			{
				Error_Staus = RCC_enuTimeOUT ;
			}
		}

		else if (LOC_u32TimeOut > TIME_OUT_TRIALS)
		{
			Error_Staus = RCC_enuTimeOUT ;
		}
		break ;

	case RCC_SYS_CLK_HSI :
		SET_BIT(RCC -> RCC_CR ,HSI_ON); //
		/*wait until the HSI is ready or the Timeout occurs*/
		while (MEM_ADDR(BIT_BAND((u32)&RCC -> RCC_CR,HSI_RDY)) == DISABLE && LOC_u32TimeOut < TIME_OUT_TRIALS )
		{
			LOC_u32TimeOut++;
		}
		if (MEM_ADDR(BIT_BAND((u32)&RCC -> RCC_CR,HSI_RDY)))
		{

			LOCTemp_REG =( (RCC -> RCC_CFGR) & (SYS_SOURCE_CLR_MASK) );
			/*Select HSI as input clock*/
			LOCTemp_REG |= RCC_SYS_CLK_HSI;
			RCC -> RCC_CFGR  = LOCTemp_REG;
			GetSysClock(&LocalSysClk2);
			while (LocalSysClk2 != RCC_SYS_CLK_HSI && LOC_u32TimeOut < TIME_OUT_TRIALS )
			{
				LOC_u32TimeOut++;
			}
			if (LocalSysClk2 == RCC_SYS_CLK_HSI)
			{
				Error_Staus = RCC_enuOK;
			}
			else if (LOC_u32TimeOut > TIME_OUT_TRIALS)
			{
				Error_Staus = RCC_enuTimeOUT ;
			}

		}
		else if (LOC_u32TimeOut > TIME_OUT_TRIALS)
		{
			Error_Staus = RCC_enuTimeOUT ;
		}
		break ;
	}

	return Error_Staus;
}
RCC_tenuErrorStatus RCC_enuEnablePerClock( u8 Cpy_u8BusId, u8 Cpy_u8PerId)
{
	Error_Staus = RCC_enuOK ;
	switch (Cpy_u8BusId)
	{
	case RCC_BUS_AHB1 :
		RCC ->  RCC_AHB1ENR |= (1<<Cpy_u8PerId);
		break ;
	case RCC_BUS_AHB2 :
		RCC ->  RCC_AHB2ENR |= (1<<Cpy_u8PerId);
		break ;
	case RCC_BUS_APB1 :
		RCC ->  RCC_APB1ENR |= (1<<Cpy_u8PerId);
		break ;
	case RCC_BUS_APB2 :
		RCC ->  RCC_APB2ENR |= (1<<Cpy_u8PerId);
		break ;
	default :
		Error_Staus = RCC_enuInvalid_BUS;
		break ;

	}

	return Error_Staus ;
}

RCC_tenuErrorStatus RCC_enuDisablePerClock(u8 Cpy_u8BusId, u8 Cpy_u8PerId)
{
	Error_Staus = RCC_enuOK ;
	switch (Cpy_u8BusId)
	{
	case RCC_BUS_AHB1 :
		RCC ->  RCC_AHB1ENR &= ~(1<<Cpy_u8PerId);
		break ;
	case RCC_BUS_AHB2 :
		RCC ->  RCC_AHB2ENR  &= ~(1<<Cpy_u8PerId);
		break ;
	case RCC_BUS_APB1 :
		RCC ->  RCC_APB1ENR  &= ~(1<<Cpy_u8PerId);
		break ;
	case RCC_BUS_APB2 :
		RCC ->  RCC_APB2ENR  &= ~(1<<Cpy_u8PerId);
		break ;
	default :
		Error_Staus = RCC_enuInvalid_BUS;
		break ;

	}

	return Error_Staus ;
}

RCC_tenuErrorStatus RCC_enuGetSysClock(RCC_SYS_CLK_t * Add_u8SysClock)
{
	Error_Staus = RCC_enuOK;
	if (Add_u8SysClock == NULL)
	{
		Error_Staus = RCC_enuNullPtr ;
	}
	*Add_u8SysClock = (MEM_ADDR(BIT_BAND((u32)&RCC -> RCC_CFGR,SWS0_POS)) | (MEM_ADDR(BIT_BAND((u32)&RCC -> RCC_CFGR,SWS1_POS))) << 1);

	return Error_Staus;
}

u8 GetSysClock(u8 * Add_u8SysClock)
{
	Error_Staus = RCC_enuOK;
	if (Add_u8SysClock == NULL)
	{
		Error_Staus = RCC_enuNullPtr ;
	}
	else
	{
		*Add_u8SysClock = (MEM_ADDR(BIT_BAND((u32)&RCC -> RCC_CFGR,SWS0_POS)) | (MEM_ADDR(BIT_BAND((u32)&RCC -> RCC_CFGR,SWS1_POS))) << 1);
	}
	return *Add_u8SysClock;
}



RCC_tenuErrorStatus RR_enuSourcReadyorNOT (u8 Cpy_u8SysClock ,u8 * Add_u8SysClock)
{
	Error_Staus = RCC_enuOK ;
	switch (Cpy_u8SysClock)
	{
	case RCC_SYS_CLK_HSE_CRYSTAL :
		* Add_u8SysClock = MEM_ADDR(BIT_BAND((u32)&RCC -> RCC_CR,HSE_RDY));
		break ; 

	case RCC_SYS_CLK_HSI :
		* Add_u8SysClock = MEM_ADDR(BIT_BAND((u32)&RCC -> RCC_CR,HSI_RDY)) ;
		break;

	case RCC_SYS_CLK_PLL :
		* Add_u8SysClock = MEM_ADDR(BIT_BAND((u32)&RCC -> RCC_CR,PLL_RDY));
		break ; 
	default :
		Error_Staus = RCC_enuInvalid_CLK_Source ; 
		break ; 
	}
	return Error_Staus ;
}

RCC_tenuErrorStatus RCC_enuPLL_Cfg( u8 Cpy_u8PLL_Source , PLL_FACT_M_t Cpy_u8FactorM , PLL_FACT_N_t Cpy_u8FactorN , PLL_FACT_P_t Cpy_u8FactorP , PLL_FACT_Q_t Cpy_u8FactorQ)

{
	Error_Staus = RCC_enuOK ;
	u8 flag = 0 ;
	u8 LOC_SYS_CLK ;
	u32 LOCTemp_REG = RCC -> RCC_PLLCFGR ;
	GetSysClock(&LOC_SYS_CLK);
	//u16 Temp_CLK_Value = 0 ;
	if ((Cpy_u8FactorM > PLL_FACT_M_MAX) || (Cpy_u8FactorN > PLL_FACT_N_MAX)||(Cpy_u8FactorN < PLL_FACT_N_MIN)||(Cpy_u8FactorM < PLL_FACT_M_MIN))
	{
		Error_Staus = RCC_enuInvalid_Parameters;
	}
	else if ( ((SPEED_HSE*Cpy_u8FactorN)/Cpy_u8FactorM) > SPEED_SYS_MAX  || ((SPEED_HSI*Cpy_u8FactorN)/Cpy_u8FactorM) > SPEED_SYS_MAX )
	{
		Error_Staus = RCC_enuInvalid_Parameters;

	}
	else
	{

		if (Cpy_u8PLL_Source == RCC_PLL_SOURCE_HSE )
		{
			SET_BIT(RCC -> RCC_CR , HSE_ON);
		}
		else if (Cpy_u8PLL_Source == RCC_PLL_SOURCE_HSE )
		{
			SET_BIT(RCC -> RCC_CR , HSI_ON);
		}

		if (LOC_SYS_CLK == RCC_SYS_CLK_PLL)
		{
			Select_HSE_SYS_CLK();
			flag = 1 ;
		}

		LOCTemp_REG &= (FACTOR_M_CLR_MASK );
		LOCTemp_REG &= (FACTOR_N_CLR_MASK );
		LOCTemp_REG &= FACTOR_P_CLR_MASK;
		LOCTemp_REG &= FACTOR_Q_CLR_MASK;
		LOCTemp_REG &= ~(DISABLE << PLL_SOURCE);

		LOCTemp_REG |= (Cpy_u8FactorM << FACTOR_M_POS ) | (Cpy_u8FactorN << FACTOR_N_POS )
								| (Cpy_u8FactorP << FACTOR_P_POS) | (Cpy_u8FactorQ << FACTOR_Q_POS) | ( Cpy_u8PLL_Source << PLL_SOURCE )  ;

		RCC -> RCC_PLLCFGR = LOCTemp_REG;
		if (flag == 1)
		{
			Select_PLL_SYS_CLK();
		}

	}


	return Error_Staus ;
}

RCC_tenuErrorStatus RCC_enuEnableSysClock(u8 Cpy_u8SystemCLk)
{
	Error_Staus = RCC_enuOK ;
	switch (Cpy_u8SystemCLk)
	{
	case RCC_SYS_CLK_HSE_CRYSTAL:
		SET_BIT( RCC -> RCC_CR , HSE_ON);
		break ;

	case RCC_SYS_CLK_HSI:
		SET_BIT( RCC -> RCC_CR , HSI_ON);
		break ;

	case RCC_SYS_CLK_PLL:
		SET_BIT( RCC -> RCC_CR , PLL_ON);
		break ;

	}
	return Error_Staus ;
}
RCC_tenuErrorStatus Select_HSE_SYS_CLK(void)
{
	u8 LocalSysClk;
	/*Enable HSE without bypass (NO RC) using a crystal*/
	SET_BIT( RCC -> RCC_CR , HSE_ON);
	/*wait until the HSE is ready or the Timeout occurs*/
	while (GET_BIT(RCC -> RCC_CR,HSE_RDY) == DISABLE && LOC_u32TimeOut < TIME_OUT_TRIALS )
	{
		LOC_u32TimeOut++;
	}
	if (GET_BIT(RCC -> RCC_CR,HSE_RDY))
	{
		LOCTemp_REG =( (RCC -> RCC_CFGR) & (SYS_SOURCE_CLR_MASK) );
		LOCTemp_REG |= RCC_SYS_CLK_HSE_CRYSTAL ;
		/*Select HSE as input clock*/
		RCC -> RCC_CFGR  = LOCTemp_REG;
		RCC_enuGetSysClock(&LocalSysClk);
		while (LocalSysClk != RCC_SYS_CLK_HSE_CRYSTAL && LOC_u32TimeOut < TIME_OUT_TRIALS )
		{
			LOC_u32TimeOut++;
		}
		if (LocalSysClk == RCC_SYS_CLK_HSE_CRYSTAL)
		{
			Error_Staus = RCC_enuOK;
		}
		else
		{
			Error_Staus = RCC_enuTimeOUT ;
		}
	}
	else
	{
		Error_Staus = RCC_enuTimeOUT ;
	}
	return Error_Staus ;
}
RCC_tenuErrorStatus Select_PLL_SYS_CLK(void)
{
	Error_Staus = RCC_enuOK;
	u8 LocalSysClk2;
	/*PLL ON*/
	SET_BIT(RCC -> RCC_CR ,PLL_ON);
	/*wait until the PLL is ready or the Timeout occurs*/
	while (GET_BIT(RCC -> RCC_CR,PLL_RDY) == DISABLE && LOC_u32TimeOut < TIME_OUT_TRIALS )
	{
		LOC_u32TimeOut++;
	}
	if (GET_BIT(RCC -> RCC_CR,PLL_RDY))
	{
		LOCTemp_REG =( (RCC -> RCC_CFGR) & (SYS_SOURCE_CLR_MASK) );
		LOCTemp_REG |= RCC_SYS_CLK_PLL;
		/*Select PLL as input clock*/
		RCC -> RCC_CFGR  = LOCTemp_REG;
		GetSysClock(&LocalSysClk2);
		while (LocalSysClk2 != RCC_SYS_CLK_PLL && LOC_u32TimeOut < TIME_OUT_TRIALS )
		{
			LOC_u32TimeOut++;
		}
		if (LocalSysClk2 == RCC_SYS_CLK_PLL)
		{
			Error_Staus = RCC_enuOK;
		}
		else
		{
			Error_Staus = RCC_enuTimeOUT ;
		}
	}

	else
	{
		Error_Staus = RCC_enuTimeOUT ;
	}
	return Error_Staus ;
}

RCC_tenuErrorStatus RCC_enRest_Per_Clk ( u8 Cpy_u8BusId, u8 Cpy_u8PerId)
{
	Error_Staus = RCC_enuOK ;
	switch (Cpy_u8BusId)
	{
	case RCC_BUS_AHB1 :
		RCC ->  RCC_AHB1RSTR |= (1<<Cpy_u8PerId);
		break ;
	case RCC_BUS_AHB2 :
		RCC ->  RCC_AHB2RSTR |= (1<<Cpy_u8PerId);
		break ;
	case RCC_BUS_APB1 :
		RCC ->  RCC_APB1RSTR |= (1<<Cpy_u8PerId);
		break ;
	case RCC_BUS_APB2 :
		RCC ->  RCC_APB2RSTR |= (1<<Cpy_u8PerId);
		break ;
	default :
		Error_Staus = RCC_enuInvalid_BUS;
		break ;

	}

	return Error_Staus ;
}


RCC_tenuErrorStatus RCC_enuGet_PLL_Cfg( RCC_PLL_SRC_t *Cpy_u8PLL_Source , PLL_FACT_M_t* Cpy_u8FactorM , PLL_FACT_N_t *Cpy_u8FactorN , PLL_FACT_P_t *Cpy_u8FactorP , PLL_FACT_Q_t *Cpy_u8FactorQ)
{
	RCC_tenuErrorStatus Error_Status = RCC_enuOK ;
	*Cpy_u8PLL_Source = GET_BIT(RCC -> RCC_PLLCFGR , PLL_SOURCE) ;
	*Cpy_u8FactorM = RCC -> RCC_PLLCFGR  & ~FACTOR_M_CLR_MASK ;
	//( << FACTOR_N_POS )
	*Cpy_u8FactorN=( RCC -> RCC_PLLCFGR  &~FACTOR_N_CLR_MASK )>>FACTOR_N_POS;
	*Cpy_u8FactorP=( RCC -> RCC_PLLCFGR  & ~FACTOR_P_CLR_MASK )>>FACTOR_P_POS;
	*Cpy_u8FactorQ= (RCC -> RCC_PLLCFGR &~FACTOR_Q_CLR_MASK )>>FACTOR_Q_POS;

	return Error_Status ;
}

RCC_tenuErrorStatus RCC_enuAHB_PreScaler (AHB_PRE_Scaler_t Cpy_AHB_PreScaler)
{
	RCC_tenuErrorStatus Error_Status = RCC_enuOK ;
	u32 Temp_Reg = 0 ;
	Temp_Reg = RCC -> RCC_CFGR  & AHB_CLR_MSK;
	//Temp_Reg  ;
	Temp_Reg |= Cpy_AHB_PreScaler << AHB_PreScaler_POS ;
	RCC -> RCC_CFGR  = Temp_Reg ;
	return Error_Status ;
}

RCC_tenuErrorStatus RCC_enuGetAHB_PreScaler (AHB_PRE_Scaler_t *ADD_AHB_PreScaler)
{
	RCC_tenuErrorStatus Error_Status = RCC_enuOK ;
	*ADD_AHB_PreScaler = ((RCC -> RCC_CFGR) >> AHB_PreScaler_POS ) & GET_AHB_PreScaler;
	return Error_Status ;
}
