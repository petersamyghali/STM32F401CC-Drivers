/*
 * LCD.h
 *
 * Created: 10-Feb-23 2:51:42 PM
 *  Author: Peter Ghali
 */


#ifndef LCD_H_
#define LCD_H_

#include "STD_TYPES.h"
#include "GPIO.h"

//An enum data type to hold the error that may occur in any of the follwoing functions 
typedef enum
{
	LCDenuOK = 0,
	LCD_enuNOTK,
	LCD_enuNULLPtr,
	LCD_enuWrong_Parameters , 
	LCD_enuErrorNOBuffer,

}LCD_enuErrorStatus_t;

typedef enum
{
	LCD_enuInitStart ,
	LCD_Function_Set ,
	LCD_Display_ON_OFF,
	LCD_Entry_Mode,
	LCD_Display_Clear,
	LCD_Home,
	LCD_enuInitDone ,
}LCD_Init_LCD_State_t ;

typedef enum{
    LCD_enuWriteStart,
    LCD_enuWriteSetPosition,
    LCD_enuWriteCharacter,
    LCD_enuWriteDone
}LCD_enuWrite_t;


typedef enum
{
	LCD_State_Not_Init ,
	LCD_State_Idle,
	LCD_State_Busy
}LCD_enuState_t;


typedef struct
{
	GPIO_enuPORT_t LCD_PORT;
	GPIO_enuPin_t  LCD_PIN ;
}LCD_Ports_PINs_t;

typedef struct{
	u8 LCD_stringData[32];
	u8  LCD_enuPosX;
	u8  LCD_enuPosY;
	u8  LCD_enuLenght;
	u8  LCD_enuCurrPos;
	u8  LCD_enuBuffered;
}LCD_strUserData_t;

typedef enum{
    LCD_enuReqIdle,
    LCD_enuReqWrite,
	LCD_equReqClear,

}LCD_enuRequest_t;

/*
A function to init the LCD based on the Given configurations in the cfg file 
INPUT	: void
Return	: Error status
 */
void LCD_enuInit (void);

/*
A function to write a specific pattern at a specific place in the LCD
INPUT	: void
Return	: void*/

void LCD_Clear(void);

/*
A function to write a specific pattern at a specific place in the LCD
INPUT	: void
Return	: Error status if one had occurred
 */
//LCD_enuErrorStatus_t LCD_enuWriteSpecialPattern  (const u8* Add_pu8Pattern, u8 Copy_u8BlockNum, u8 Copy_u8XPos, u8 Copy_u8YPos);

/*
A function to Write a string to the LCD
INPUT	: A string
Return	: Error status if one had occurred
 */
LCD_enuErrorStatus_t LCD_enuSendString_Async (const u8* Add_pchString ,u8 Length , u8 POS_X , u8 POS_Y);

/*
A function to Write a number to the LCD
INPUT	: A number  
Return	: Error status if one had occurred
 */
//LCD_enuErrorStatus_t LCD_enuWriteNumber (u32 Copy_u16Number);



#endif /* LCD_H_ */
