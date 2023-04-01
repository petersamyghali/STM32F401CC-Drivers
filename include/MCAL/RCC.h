/*
 * RCC.h
 *
 *  Created on: Jan 28, 2023
 *      Author: Peter Ghali
 */
#ifndef _RCC_H_
#define _RCC_H_

#include "STD_TYPES.h"

#define SPEED_HSI		16
#define SPEED_HSE		25
#define SPEED_SYS_MAX	84

typedef enum
{
	CLK_Ready = 0 ,
	CLk_Not_Ready = 1 ,
}RCC_CLK_Ready_NOT_t;

//defining the CLKs that the system may run with
typedef enum
{
	RCC_SYS_CLK_HSI				  =0  ,
	RCC_SYS_CLK_HSE_CRYSTAL	=	1 ,
	RCC_SYS_CLK_PLL			  	=2  ,
}RCC_SYS_CLK_t;



//PLL Source
typedef enum
{
	RCC_PLL_SOURCE_HSI	=	0,
	RCC_PLL_SOURCE_HSE	=	1
}RCC_PLL_SRC_t;


/*  the buses   */
typedef enum
{
	RCC_BUS_AHB1 = 0,
	RCC_BUS_AHB2 = 1,
	RCC_BUS_APB1	=2,
	RCC_BUS_APB2	=3,
}RCC_Buses_t;


/*
 * RCC AHB1 peripheralS
 * */
typedef enum
{
	RCC_PER_GPIOA	=0  ,
	RCC_PER_GPIOB	=1  ,
	RCC_PER_GPIOC	=2  ,
	RCC_PER_GPIOD	=3  ,
	RCC_PER_GPIOE	=4  ,
	RCC_PER_GPIOH	=7  ,
	RCC_PER_CRC		=12 ,
	RCC_PER_DMA1	=21 ,
	RCC_PER_DMA2	=22 ,
}RCC_AHP1_peripheralS_t;


/*
 * RCC AHB2 peripheralS
 * */
#define	 RCC_PER_OTGFS		7

/*
 * RCC APB1 peripheralS
 * */
typedef enum
{
	RCC_PER_TIM2	=0  ,
	RCC_PER_TIM3	=1  ,
	RCC_PER_TIM4	=2  ,
	RCC_PER_TIM5	=3  ,
	RCC_PER_WWDG	=11 ,
	RCC_PER_SPI2	=14 ,
	RCC_PER_SPI3	=15 ,
	RCC_PER_USART2=	17,
	RCC_PER_I2C1	=21 ,
	RCC_PER_I2C2	=22 ,
	RCC_PER_I2C3	=23 ,
	RCC_PER_PWR		=28 ,
}RCC_APB1_peripheralS_t;


/*
 * RCC APB2 peripheralS
 * */
typedef enum
{
	RCC_PER_TIM1		=0 ,
	RCC_PER_USART1	=	4,
	RCC_PER_USART6	=	5,
	RCC_PER_ADV1		=8 ,
	RCC_PER_SDIO		=11,
	RCC_PER_SPI1		=12,
	RCC_PER_SPI4		=13,
	RCC_PER_SYSCD		=14,
	RCC_PER_TIM9		=16,
	RCC_PER_TIM10		=17,
	RCC_PER_TIM11		=18,
}RCC_APB2_peripheralS;


/**********************************  TYPES  ********************************************************************************************/
//An Enum Datatype to have the errors that may occur in the RCC APIs
typedef enum
{
	RCC_enuOK,
	RCC_enuNOTOK,
	RCC_enuInvalid_CLK_Source,
	RCC_enuInvalid_PreID,
	RCC_enuInvalid_BUS,
	RCC_enuInvalid_Parameters ,
	RCC_enuNullPtr,
	RCC_enuTimeOUT

}RCC_tenuErrorStatus;

/*
 * NOTE F(VCO) = F(PLL_SRC) * (N/M)
 * CAUTION = F(VCO) must not exceed SPEED_SYS_MAX
 * */

//An enum data type to hole the Division factor for VCO may hvae

/*The software has to set these bits correctly to ensure that the VCO input frequency
ranges from 1 to 2 MHz
 */
typedef enum
{
	PLL_FACT_M_MIN = 2,
	PLL_FACT_M_2=2,
	PLL_FACT_M_3 ,
	PLL_FACT_M_4 ,
	PLL_FACT_M_5 ,
	PLL_FACT_M_6 ,
	PLL_FACT_M_7 ,
	PLL_FACT_M_8 ,
	PLL_FACT_M_9 ,
	PLL_FACT_M_10,
	PLL_FACT_M_11,
	PLL_FACT_M_12,
	PLL_FACT_M_13,
	PLL_FACT_M_14,
	PLL_FACT_M_15,
	PLL_FACT_M_16,
	PLL_FACT_M_17,
	PLL_FACT_M_18,
	PLL_FACT_M_19,
	PLL_FACT_M_20,
	PLL_FACT_M_21,
	PLL_FACT_M_22,
	PLL_FACT_M_23,
	PLL_FACT_M_24,
	PLL_FACT_M_25,
	PLL_FACT_M_26,
	PLL_FACT_M_27,
	PLL_FACT_M_28,
	PLL_FACT_M_29,
	PLL_FACT_M_30,
	PLL_FACT_M_31,
	PLL_FACT_M_32,
	PLL_FACT_M_33,
	PLL_FACT_M_34,
	PLL_FACT_M_35,
	PLL_FACT_M_36,
	PLL_FACT_M_37,
	PLL_FACT_M_38,
	PLL_FACT_M_39,
	PLL_FACT_M_40,
	PLL_FACT_M_41,
	PLL_FACT_M_42,
	PLL_FACT_M_43,
	PLL_FACT_M_44,
	PLL_FACT_M_45,
	PLL_FACT_M_46,
	PLL_FACT_M_47,
	PLL_FACT_M_48,
	PLL_FACT_M_49,
	PLL_FACT_M_50,
	PLL_FACT_M_51,
	PLL_FACT_M_52,
	PLL_FACT_M_53,
	PLL_FACT_M_54,
	PLL_FACT_M_55,
	PLL_FACT_M_56,
	PLL_FACT_M_57,
	PLL_FACT_M_58,
	PLL_FACT_M_59,
	PLL_FACT_M_60,
	PLL_FACT_M_61,
	PLL_FACT_M_62,
	PLL_FACT_M_MAX = 62,

}PLL_FACT_M_t;

//An enum data type to hole the multiplication factor for VCO may hvae

/*Caution: The software has to set these bits correctly to ensure that the VCO output
frequency is between 192 and 432 MHz.
 */
typedef enum
{
	PLL_FACT_N_MIN =192 ,
	PLL_FACT_N_192 = 192,
	PLL_FACT_N_193,
	PLL_FACT_N_194,
	PLL_FACT_N_195,
	PLL_FACT_N_196,
	PLL_FACT_N_197,
	PLL_FACT_N_198,
	PLL_FACT_N_199,
	PLL_FACT_N_200,
	PLL_FACT_N_201,
	PLL_FACT_N_202,
	PLL_FACT_N_203,
	PLL_FACT_N_204,
	PLL_FACT_N_205,
	PLL_FACT_N_206,
	PLL_FACT_N_207,
	PLL_FACT_N_208,
	PLL_FACT_N_209,
	PLL_FACT_N_210,
	PLL_FACT_N_211,
	PLL_FACT_N_212,
	PLL_FACT_N_213,
	PLL_FACT_N_214,
	PLL_FACT_N_215,
	PLL_FACT_N_216,
	PLL_FACT_N_217,
	PLL_FACT_N_218,
	PLL_FACT_N_219,
	PLL_FACT_N_220,
	PLL_FACT_N_221,
	PLL_FACT_N_222,
	PLL_FACT_N_223,
	PLL_FACT_N_224,
	PLL_FACT_N_225,
	PLL_FACT_N_226,
	PLL_FACT_N_227,
	PLL_FACT_N_228,
	PLL_FACT_N_229,
	PLL_FACT_N_230,
	PLL_FACT_N_231,
	PLL_FACT_N_232,
	PLL_FACT_N_233,
	PLL_FACT_N_234,
	PLL_FACT_N_235,
	PLL_FACT_N_236,
	PLL_FACT_N_237,
	PLL_FACT_N_238,
	PLL_FACT_N_239,
	PLL_FACT_N_240,
	PLL_FACT_N_241,
	PLL_FACT_N_242,
	PLL_FACT_N_243,
	PLL_FACT_N_244,
	PLL_FACT_N_245,
	PLL_FACT_N_246,
	PLL_FACT_N_247,
	PLL_FACT_N_248,
	PLL_FACT_N_249,
	PLL_FACT_N_250,
	PLL_FACT_N_251,
	PLL_FACT_N_252,
	PLL_FACT_N_253,
	PLL_FACT_N_254,
	PLL_FACT_N_255,
	PLL_FACT_N_256,
	PLL_FACT_N_257,
	PLL_FACT_N_258,
	PLL_FACT_N_259,
	PLL_FACT_N_260,
	PLL_FACT_N_261,
	PLL_FACT_N_262,
	PLL_FACT_N_263,
	PLL_FACT_N_264,
	PLL_FACT_N_265,
	PLL_FACT_N_266,
	PLL_FACT_N_267,
	PLL_FACT_N_268,
	PLL_FACT_N_269,
	PLL_FACT_N_270,
	PLL_FACT_N_271,
	PLL_FACT_N_272,
	PLL_FACT_N_273,
	PLL_FACT_N_274,
	PLL_FACT_N_275,
	PLL_FACT_N_276,
	PLL_FACT_N_277,
	PLL_FACT_N_278,
	PLL_FACT_N_279,
	PLL_FACT_N_280,
	PLL_FACT_N_281,
	PLL_FACT_N_282,
	PLL_FACT_N_283,
	PLL_FACT_N_284,
	PLL_FACT_N_285,
	PLL_FACT_N_286,
	PLL_FACT_N_287,
	PLL_FACT_N_288,
	PLL_FACT_N_289,
	PLL_FACT_N_290,
	PLL_FACT_N_291,
	PLL_FACT_N_292,
	PLL_FACT_N_293,
	PLL_FACT_N_294,
	PLL_FACT_N_295,
	PLL_FACT_N_296,
	PLL_FACT_N_297,
	PLL_FACT_N_298,
	PLL_FACT_N_299,
	PLL_FACT_N_300,
	PLL_FACT_N_301,
	PLL_FACT_N_302,
	PLL_FACT_N_303,
	PLL_FACT_N_304,
	PLL_FACT_N_305,
	PLL_FACT_N_306,
	PLL_FACT_N_307,
	PLL_FACT_N_308,
	PLL_FACT_N_309,
	PLL_FACT_N_310,
	PLL_FACT_N_311,
	PLL_FACT_N_312,
	PLL_FACT_N_313,
	PLL_FACT_N_314,
	PLL_FACT_N_315,
	PLL_FACT_N_316,
	PLL_FACT_N_317,
	PLL_FACT_N_318,
	PLL_FACT_N_319,
	PLL_FACT_N_320,
	PLL_FACT_N_321,
	PLL_FACT_N_322,
	PLL_FACT_N_323,
	PLL_FACT_N_324,
	PLL_FACT_N_325,
	PLL_FACT_N_326,
	PLL_FACT_N_327,
	PLL_FACT_N_328,
	PLL_FACT_N_329,
	PLL_FACT_N_330,
	PLL_FACT_N_331,
	PLL_FACT_N_332,
	PLL_FACT_N_333,
	PLL_FACT_N_334,
	PLL_FACT_N_335,
	PLL_FACT_N_336,
	PLL_FACT_N_337,
	PLL_FACT_N_338,
	PLL_FACT_N_339,
	PLL_FACT_N_340,
	PLL_FACT_N_341,
	PLL_FACT_N_342,
	PLL_FACT_N_343,
	PLL_FACT_N_344,
	PLL_FACT_N_345,
	PLL_FACT_N_346,
	PLL_FACT_N_347,
	PLL_FACT_N_348,
	PLL_FACT_N_349,
	PLL_FACT_N_350,
	PLL_FACT_N_351,
	PLL_FACT_N_352,
	PLL_FACT_N_353,
	PLL_FACT_N_354,
	PLL_FACT_N_355,
	PLL_FACT_N_356,
	PLL_FACT_N_357,
	PLL_FACT_N_358,
	PLL_FACT_N_359,
	PLL_FACT_N_360,
	PLL_FACT_N_361,
	PLL_FACT_N_362,
	PLL_FACT_N_363,
	PLL_FACT_N_364,
	PLL_FACT_N_365,
	PLL_FACT_N_366,
	PLL_FACT_N_367,
	PLL_FACT_N_368,
	PLL_FACT_N_369,
	PLL_FACT_N_370,
	PLL_FACT_N_371,
	PLL_FACT_N_372,
	PLL_FACT_N_373,
	PLL_FACT_N_374,
	PLL_FACT_N_375,
	PLL_FACT_N_376,
	PLL_FACT_N_377,
	PLL_FACT_N_378,
	PLL_FACT_N_379,
	PLL_FACT_N_380,
	PLL_FACT_N_381,
	PLL_FACT_N_382,
	PLL_FACT_N_383,
	PLL_FACT_N_384,
	PLL_FACT_N_385,
	PLL_FACT_N_386,
	PLL_FACT_N_387,
	PLL_FACT_N_388,
	PLL_FACT_N_389,
	PLL_FACT_N_390,
	PLL_FACT_N_391,
	PLL_FACT_N_392,
	PLL_FACT_N_393,
	PLL_FACT_N_394,
	PLL_FACT_N_395,
	PLL_FACT_N_396,
	PLL_FACT_N_397,
	PLL_FACT_N_398,
	PLL_FACT_N_399,
	PLL_FACT_N_400,
	PLL_FACT_N_401,
	PLL_FACT_N_402,
	PLL_FACT_N_403,
	PLL_FACT_N_404,
	PLL_FACT_N_405,
	PLL_FACT_N_406,
	PLL_FACT_N_407,
	PLL_FACT_N_408,
	PLL_FACT_N_409,
	PLL_FACT_N_410,
	PLL_FACT_N_411,
	PLL_FACT_N_412,
	PLL_FACT_N_413,
	PLL_FACT_N_414,
	PLL_FACT_N_415,
	PLL_FACT_N_416,
	PLL_FACT_N_417,
	PLL_FACT_N_418,
	PLL_FACT_N_419,
	PLL_FACT_N_420,
	PLL_FACT_N_421,
	PLL_FACT_N_422,
	PLL_FACT_N_423,
	PLL_FACT_N_424,
	PLL_FACT_N_425,
	PLL_FACT_N_426,
	PLL_FACT_N_427,
	PLL_FACT_N_428,
	PLL_FACT_N_429,
	PLL_FACT_N_430,
	PLL_FACT_N_431,
	PLL_FACT_N_432,
	PLL_FACT_N_MAX = 432 ,

}PLL_FACT_N_t;

typedef enum
{
	PLL_FACT_P_2 = 2,
	PLL_FACT_P_4 = 4 ,
	PLL_FACT_P_6 = 6 ,
	PLL_FACT_P_8 = 8 ,

}PLL_FACT_P_t;

typedef enum
{
	PLL_FACT_Q_MiN = 2 ,
	PLL_FACT_Q_2 = 2  ,
	PLL_FACT_Q_3 ,
	PLL_FACT_Q_4 ,
	PLL_FACT_Q_5 ,
	PLL_FACT_Q_6 ,
	PLL_FACT_Q_7 ,
	PLL_FACT_Q_8 ,
	PLL_FACT_Q_9 ,
	PLL_FACT_Q_10,
	PLL_FACT_Q_11,
	PLL_FACT_Q_12,
	PLL_FACT_Q_13,
	PLL_FACT_Q_14,
	PLL_FACT_Q_15 = 15 ,
	PLL_FACT_Q_MAX = 15 ,

}PLL_FACT_Q_t;
typedef enum
{
	AHB_PRE_not_divided = 0 ,
	AHB_PRE_divided_by_2 = 8 ,
	AHB_PRE_divided_by_4  ,
	AHB_PRE_divided_by_8 ,
	AHB_PRE_divided_by_16  ,
	AHB_PRE_divided_by_64  ,
	AHB_PRE_divided_by_128 ,
	AHB_PRE_divided_by_256 ,
	AHB_PRE_divided_by_512,
}AHB_PRE_Scaler_t;
/*********************************  Functions prototype  *******************************************************************************/

/*
 *Set system CLK function : Sets the system CLK by the given parameter
 *return Type : RCC error status
 *Input options : 	 RCC_SYS_CLK_HSI
	 	 	 	 	 RCC_SYS_CLK_HSE_CRYSTAL
					 RCC_SYS_CLK_PLL
	 	 	 	 	 RCC_SYS_CLK_HSE_BYP
 */
RCC_tenuErrorStatus RCC_enuSetSysClock(RCC_SYS_CLK_t Cpy_u8SystemCLk);

/*
 *RCC Get the system Clock
 *return Type : RCC error status
 *Input : An address to a variable to hold the current system clock
 */

RCC_tenuErrorStatus RCC_enuGetSysClock(RCC_SYS_CLK_t * Add_u8SysClock);

/*
 *Set system CLK function : Sets the system CLK by the given parameter
 *return Type : RCC error status
 *Input options : 	 RCC_SYS_CLK_HSI
	 	 	 	 	 RCC_SYS_CLK_HSE_CRYSTAL
					 RCC_SYS_CLK_PLL
	 	 	 	 	 RCC_SYS_CLK_HSE_BYP
 */
RCC_tenuErrorStatus RCC_enuEnableSysClock(RCC_SYS_CLK_t Cpy_u8SystemCLk);

/*
 *RCC set the PLL configurations
 *return Type : RCC error status
 *Input : PLL source & Factors : M , N , P and Q
 */

RCC_tenuErrorStatus RCC_enuPLL_Cfg( RCC_PLL_SRC_t Cpy_u8PLL_Source , PLL_FACT_M_t Cpy_u8FactorM , PLL_FACT_N_t Cpy_u8FactorN , PLL_FACT_P_t Cpy_u8FactorP , PLL_FACT_Q_t Cpy_u8FactorQ);

/*
 *RCC set the PLL configurations
 *return Type : RCC error status
 *Input : PLL source & Factors : M , N , P and Q
 */

RCC_tenuErrorStatus RCC_enuGet_PLL_Cfg( RCC_PLL_SRC_t *Cpy_u8PLL_Source , PLL_FACT_M_t* Cpy_u8FactorM , PLL_FACT_N_t *Cpy_u8FactorN , PLL_FACT_P_t *Cpy_u8FactorP , PLL_FACT_Q_t *Cpy_u8FactorQ);

/*
 *RCC check if the CLK source is ready or not
 *return Type : RCC error status
 *Input :System CLk to be checked ,  An address to a variable to hold the value (1 for ready , 0 for not s)
 */
RCC_tenuErrorStatus RCC_enuCLKReadyorNOT (RCC_SYS_CLK_t Cpy_u8SystemCLk ,RCC_CLK_Ready_NOT_t * Add_u8SysClock);

/*
 *RCC enable peripheral function : enable the peripheral CLK
 *return Type : RCC error status
 *Input : The peripheral Bus , The peripheral ID
 */
RCC_tenuErrorStatus RCC_enuEnablePerClock(  RCC_Buses_t Cpy_u8BusId ,   u8 Cpy_u8PerId);

/*
 *RCC disable peripheral function : disable  the peripheral CLK
 *return Type : RCC error status
 *Input : The peripheral Bus , The peripheral ID
 */

RCC_tenuErrorStatus RCC_enuDisablePerClock(RCC_Buses_t Cpy_u8BusId, u8 Cpy_u8PerId);

/*
 *RCC Reset peripheral function : Resets the peripheral
 *return Type : RCC error status
 *Input : The peripheral Bus , The peripheral ID
 */
RCC_tenuErrorStatus RCC_enuRest_Per_Clk ( RCC_Buses_t Cpy_u8BusId, u8 Cpy_u8PerId);

/*
 *RCC AHB prescaler function : sets a prescaler for the AHB Bus
 *return Type : RCC error status
 *Input : The peripheral Bus , The peripheral ID
 */
RCC_tenuErrorStatus RCC_enuAHB_PreScaler (AHB_PRE_Scaler_t Cpy_AHB_PreScaler);

RCC_tenuErrorStatus RCC_enuGetAHB_PreScaler (AHB_PRE_Scaler_t *ADD_AHB_PreScaler) ;


#endif /*_RCC_H_*/
