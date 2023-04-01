/*
 * LCD.c
 *
 * Created: 10-Feb-23 2:52:25 PM
 *  Author: Peter Ghali
 */
#include "STD_TYPES.h"
#include "Bit_Math.h"

#include "LCD.h"
#include "LCD_Cfg.h"

#include "GPIO.h"
/********************************** Macros *********************************************/
#define DISABLE					0
#define ENABLE					1
#define LCD_ENTRY_MODE			0x06          // shift cursor from left to right on read/write
#define LCD_FUNCTION_Set8bit	0x38          // 8-bit data, 2-line display, 5 x 7 font
#define LCD_4BIT				0x02
#define LCD_FUNCTION_Set4bit	0x28          // 4-bit data, 2-line display, 5 x 7 font
#define	ROW_0_POS				0x00
#define	ROW_1_POS				0x40
#define SET_DDRAM_ADDRESS		0x80
#define SET_CGRAM_ADDRESS		0x40
#define LCD_Argument_Check		0x00
#define LCD_Argument_Check_EQ	0x00
#define BLOCK_SIZE				8
/*Some needed commands */
#define LCD_CLEAR				0x01          /* replace all characters with ASCII 'space'        */
#define LCD_HOME				0x02          /* return cursor to first position on first line     */
#define LCD_DISPLAY_OFF			0x08          // turn display off
#define LCD_DISPLAY_ON			0x0C          // display on, cursor off, don't blink character
#define LCD_Reset				0x30          // reset the LCD
#define LCD_CURSOR_LEFT			0x10
#define	LCD_CURSOR_RIGHT		0x14
#define	LCD_DISPLAY_LEFT		0x18
#define	LCD_DISPLAY_RIGHT		0x1C
#define LCD_BUFFERED			1
#define LCD_NOTBUFFERED			0
#define CURSOR_ON	1

#define CURSOR_OFF	2

typedef enum{
	LCD_enuEnableOff = 1 ,
	LCD_enuEnableOn= 0
}LCD_enuEnableLCD_State;



static LCD_strUserData_t  		 LCD_strUserData[LCD_BUFFERSIZE]={0};
u8 								 LCD_Init_Done = 0 ;
static volatile LCD_enuWrite_t	 LCD_enuWriteState = LCD_enuWriteStart;
static LCD_enuEnableLCD_State 	 LCD_Enable_PIN = LCD_enuEnableOff;
LCD_enuRequest_t   				 LCD_enuCurrRequest    = LCD_enuReqIdle;
static volatile u8   			 LCD_u8CurrentBuffer   = 0;
static volatile u8    			 LCD_u8CurrentPosition = 0;
static volatile u8   			 LCD_u8BufferIndex     = 0;
static u8       				 LCD_u8BusyFlag = DISABLE;
/*
A function to Send a command to the LCD
INPUT	: the required command
Return	: Error status if one had occurred
 */
static void LCD_enuSendCommand (u16 Copy_u8Command);
/*
A function to Make the cursor go to the given position
INPUT	: The X , Y coordinates of the LCD (0 included)
Return	: Error status if one had occurred
 */
static void LCD_enuGotoXY      (u8 Copy_u8XPos, u8 Copy_u8YPos);

/*A function to set or clear the Enable pin in the LCD*/
static inline void LCD_Set_Enable_PIN(LCD_enuEnableLCD_State LCD_State );


void LCD_vidWriteProcess(void);
/****************************************** IMPLEMENTATION ******************************************************************/
void LCD_enuInit (void)
{
	/*Set the LCD state with the starting position*/
	static LCD_Init_LCD_State_t LCD_State = LCD_enuInitStart ;
	GPIO_Cfg_t GPIO_Cfg ;
	/*A static counter in order to init the pins only once */
	static u8 Loc_Counter = 0 ;
	if (Loc_Counter == 0 )
	{
		LCD_u8BusyFlag = ENABLE;
		for (Loc_Counter = 0 ; Loc_Counter < (LCD_BIT_MODE+ Number_OF_CONTROL_PINS) ;Loc_Counter++ )
		{
			GPIO_Cfg.Mode = GPIO_Output_push_pull;
			GPIO_Cfg.Pin = LCD_Cfg_Array[Loc_Counter].LCD_PIN ;
			GPIO_Cfg.Port = LCD_Cfg_Array[Loc_Counter].LCD_PORT ;
			GPIO_Cfg.Speed  =  GPIO_Speed_Very_High ;
			GPIO_Init_Pin(&GPIO_Cfg);

		}
		/* SET THE BUSY FLAG */
		LCD_u8BusyFlag = ENABLE;
	}
	//if the function has been called once then start with the init sequence
	else
	{
		/*start the init sequence as it was some sort of state machine*/
		switch (LCD_State)
		{
		case LCD_enuInitStart:
			/*Sets the state by the next state */
			LCD_State = LCD_Function_Set;
			LCD_Enable_PIN= LCD_enuEnableOff;
			break ;
		case LCD_Function_Set :
			/*if the E pin is low then write the required command*/
			if (LCD_Enable_PIN == LCD_enuEnableOff )
			{
				LCD_enuSendCommand(LCD_FUNCTION_Set8bit);
				LCD_Set_Enable_PIN(LCD_enuEnableOn);
				LCD_Enable_PIN = LCD_enuEnableOn ;
			}
			/*if it's high then resets the pin after 2ms*/
			else
			{
				LCD_Set_Enable_PIN(LCD_enuEnableOff);
				LCD_Enable_PIN = LCD_enuEnableOff ;
				/*Sets the state by the next state */
				LCD_State = LCD_Display_ON_OFF;
			}
			break ;

		case LCD_Display_ON_OFF :
			if (LCD_Enable_PIN == LCD_enuEnableOff )
			{
				LCD_enuSendCommand(LCD_DISPLAY_ON);
				LCD_Set_Enable_PIN(LCD_enuEnableOn);
				LCD_Enable_PIN = LCD_enuEnableOn ;
			}
			else
			{
				LCD_Set_Enable_PIN(LCD_enuEnableOff);
				LCD_Enable_PIN = LCD_enuEnableOff ;
				/*Sets the state by the next state */
				LCD_State = LCD_Display_Clear;
			}

			break ;
		case LCD_Entry_Mode :
			if (LCD_Enable_PIN == LCD_enuEnableOff )
			{
				LCD_enuSendCommand(LCD_ENTRY_MODE);
				LCD_Set_Enable_PIN(LCD_enuEnableOn);

				LCD_Enable_PIN = LCD_enuEnableOn ;
			}
			else
			{
				LCD_Set_Enable_PIN(LCD_enuEnableOff);
				LCD_Enable_PIN = LCD_enuEnableOff ;
				/*Sets the state by the next state */
				LCD_State = LCD_Home;
			}
			break ;


		case LCD_Display_Clear :
			if (LCD_Enable_PIN == LCD_enuEnableOff )
			{
				LCD_enuSendCommand(LCD_CLEAR);
				LCD_Set_Enable_PIN(LCD_enuEnableOn);
				LCD_Enable_PIN = LCD_enuEnableOn ;
			}
			else
			{
				LCD_Set_Enable_PIN(LCD_enuEnableOff);
				LCD_Enable_PIN = LCD_enuEnableOff ;
				/*Sets the state by the next state */
				LCD_State = LCD_Entry_Mode;
			}
			break ;

		case LCD_Home :
			if (LCD_Enable_PIN == LCD_enuEnableOff )
			{
				LCD_enuSendCommand(LCD_HOME);
				LCD_Set_Enable_PIN(LCD_enuEnableOn);
				LCD_Enable_PIN = LCD_enuEnableOn ;
			}
			else
			{
				LCD_Set_Enable_PIN(LCD_enuEnableOff);
				LCD_Enable_PIN = LCD_enuEnableOff ;
				/*Sets the state by the next state */
				LCD_State = LCD_enuInitDone;
			}
			break ;
		case LCD_enuInitDone :
			/*the init process has been completed */
			LCD_Init_Done = LCD_enuInitDone ;
			LCD_u8BusyFlag = DISABLE;
			break;

		default :
			break ;
		}

	}


}
void LCD_enuSendData (u16 Copy_u8Command)
{
	u8 LOC_Counter = 0 ,BIT = 0 ;
	GPIO_Write_Pin_Value(LCD_Cfg_Array[LCD_RS_PIN].LCD_PORT ,LCD_Cfg_Array[LCD_RS_PIN].LCD_PIN , GPIO_SET);/*As I am sending a command*/
	GPIO_Write_Pin_Value(LCD_Cfg_Array[LCD_RW_PIN].LCD_PORT ,LCD_Cfg_Array[LCD_RW_PIN].LCD_PIN , GPIO_RESET);
	/*As I am writing to the LCD*/

	for ( LOC_Counter = 0 ; LOC_Counter< LCD_BIT_MODE ; LOC_Counter++)
	{
		BIT = GET_BIT(Copy_u8Command,LOC_Counter);
		if (BIT == 1 )
		{
			GPIO_Write_Pin_Value(LCD_Cfg_Array[LOC_Counter+Number_OF_CONTROL_PINS].LCD_PORT ,LCD_Cfg_Array[LOC_Counter+Number_OF_CONTROL_PINS].LCD_PIN , GPIO_SET);
		}
		else
		{
			GPIO_Write_Pin_Value(LCD_Cfg_Array[LOC_Counter+Number_OF_CONTROL_PINS].LCD_PORT ,LCD_Cfg_Array[LOC_Counter+Number_OF_CONTROL_PINS].LCD_PIN , GPIO_RESET);
		}
	}

}
void LCD_enuSendCommand (u16 Copy_u8Command)
{
	u8 LOC_Counter = 0 ,BIT = 0 ;
	GPIO_Write_Pin_Value(LCD_Cfg_Array[LCD_RS_PIN].LCD_PORT ,LCD_Cfg_Array[LCD_RS_PIN].LCD_PIN , GPIO_RESET);/*As I am sending a command*/
	GPIO_Write_Pin_Value(LCD_Cfg_Array[LCD_RW_PIN].LCD_PORT ,LCD_Cfg_Array[LCD_RW_PIN].LCD_PIN , GPIO_RESET);
	/*As I am writing to the LCD*/

	for ( LOC_Counter = 0 ; LOC_Counter< LCD_BIT_MODE ; LOC_Counter++)
	{
		BIT = GET_BIT(Copy_u8Command,LOC_Counter);
		if (BIT == 1 )
		{
			GPIO_Write_Pin_Value(LCD_Cfg_Array[LOC_Counter+Number_OF_CONTROL_PINS].LCD_PORT ,LCD_Cfg_Array[LOC_Counter+Number_OF_CONTROL_PINS].LCD_PIN , GPIO_SET);
		}
		else
		{
			GPIO_Write_Pin_Value(LCD_Cfg_Array[LOC_Counter+Number_OF_CONTROL_PINS].LCD_PORT ,LCD_Cfg_Array[LOC_Counter+Number_OF_CONTROL_PINS].LCD_PIN , GPIO_RESET);
		}
	}

}
static inline void LCD_Set_Enable_PIN(LCD_enuEnableLCD_State LCD_State )
{
	GPIO_Write_Pin_Value(LCD_Cfg_Array[LCD_E_PIN].LCD_PORT ,LCD_Cfg_Array[LCD_E_PIN].LCD_PIN , LCD_State);

}


LCD_enuErrorStatus_t LCD_enuSendString_Async (const u8* Add_pchString ,u8 Length , u8 POS_X , u8 POS_Y)
{
	LCD_enuErrorStatus_t  LOC_Error_Status = LCDenuOK ;
	u8 Loc_u8BufferIndex = 0 ,Loc_u8CharBuffer=0;
	if (POS_X >= NUMBER_OF_ROWS || POS_Y >= NUMBER_OF_COLMUNS)
	{
		LOC_Error_Status = LCD_enuWrong_Parameters ;
	}
	if (Add_pchString == NULL)
	{
		LOC_Error_Status = LCD_enuNULLPtr ;
	}
	else
	{
		/* ITERATE THROUGH BUFFER SIZE */
		for(Loc_u8BufferIndex=0;Loc_u8BufferIndex<LCD_BUFFERSIZE;Loc_u8BufferIndex++)
		{

			/* FINDING AN EMPTY BUFFER */
			if(LCD_strUserData[Loc_u8BufferIndex].LCD_enuBuffered == LCD_NOTBUFFERED)
			{

				/* BUFFERING THE DATA */

				/* BUFFER THE STRING */
				for(Loc_u8CharBuffer=0;Loc_u8CharBuffer<Length;Loc_u8CharBuffer++)
				{
					LCD_strUserData[Loc_u8BufferIndex].LCD_stringData[Loc_u8CharBuffer]  = Add_pchString[Loc_u8CharBuffer];
				}

				LCD_strUserData[Loc_u8BufferIndex].LCD_enuPosX     = POS_X;
				LCD_strUserData[Loc_u8BufferIndex].LCD_enuPosY     = POS_Y;
				LCD_strUserData[Loc_u8BufferIndex].LCD_enuLenght   = Length;
				LCD_strUserData[Loc_u8BufferIndex].LCD_enuCurrPos  = 0;
				LCD_strUserData[Loc_u8BufferIndex].LCD_enuBuffered = LCD_BUFFERED;
				/* CHANGE CURRENT REQUEST */
				if(LCD_enuCurrRequest == LCD_enuReqIdle){
					LCD_enuCurrRequest = LCD_enuReqWrite;
					LCD_enuWriteState  = LCD_enuWriteStart;
				}
				break;
			}
		}
		/* NO Empty BUFFER */
		if(Loc_u8BufferIndex == LCD_BUFFERSIZE){
			LOC_Error_Status = LCD_enuErrorNOBuffer;
		}
	}

	return LOC_Error_Status ;
}

void LCD_enuGotoXY (u8 Copy_u8XPos, u8 Copy_u8YPos)
{
	u8 LOC_u8Address = 0 ;

	if (Copy_u8XPos == 0 )
	{
		LOC_u8Address = Copy_u8YPos + ROW_0_POS;
	}
	else if  (Copy_u8XPos == 1 )
	{
		LOC_u8Address = Copy_u8YPos + ROW_1_POS;
	}
	//Oring the address of the CGRAM with the need position
	LCD_enuSendCommand(LOC_u8Address | SET_DDRAM_ADDRESS);

}

void LCD_Clear(void)
{
	LCD_enuCurrRequest = LCD_equReqClear ;
	if (LCD_Enable_PIN == LCD_enuEnableOff)
	{
		LCD_enuSendCommand(LCD_CLEAR);
		LCD_Set_Enable_PIN(LCD_enuEnableOn);
		LCD_Enable_PIN = LCD_enuEnableOn ;
	}
	else
	{
		LCD_Set_Enable_PIN(LCD_enuEnableOff);
		LCD_Enable_PIN = LCD_enuEnableOff ;
		LCD_enuCurrRequest = LCD_enuReqIdle;
	}

}


void LCD_vidWriteProcess(void)
{
	/*start the writing sequence*/
	switch(LCD_enuWriteState){

	case LCD_enuWriteStart:

		LCD_enuWriteState = LCD_enuWriteSetPosition;
		LCD_Enable_PIN = LCD_enuEnableOff;
		break;
	case LCD_enuWriteSetPosition:
		if (LCD_Enable_PIN == LCD_enuEnableOff)
		{
			LCD_enuGotoXY(LCD_strUserData[LCD_u8CurrentBuffer].LCD_enuPosX, LCD_strUserData[LCD_u8CurrentBuffer].LCD_enuPosY) ;
			LCD_Set_Enable_PIN(LCD_enuEnableOn);
			LCD_Enable_PIN = LCD_enuEnableOn ;
		}
		else
		{
			LCD_Set_Enable_PIN(LCD_enuEnableOff);
			LCD_Enable_PIN = LCD_enuEnableOff ;
			LCD_enuWriteState = LCD_enuWriteCharacter;
		}
		break ;

	case LCD_enuWriteCharacter:

		if(LCD_strUserData[LCD_u8CurrentBuffer].LCD_enuCurrPos < LCD_strUserData[LCD_u8CurrentBuffer].LCD_enuLenght)
		{
			if(LCD_Enable_PIN == LCD_enuEnableOff){
				LCD_u8CurrentPosition = LCD_strUserData[LCD_u8CurrentBuffer].LCD_enuCurrPos;
				LCD_enuSendData(LCD_strUserData[LCD_u8CurrentBuffer].LCD_stringData[LCD_u8CurrentPosition]);
				LCD_Set_Enable_PIN(LCD_enuEnableOn);
				LCD_Enable_PIN = LCD_enuEnableOn;
			}
			else{
				LCD_Enable_PIN = LCD_enuEnableOff;
				LCD_Set_Enable_PIN(LCD_enuEnableOff);
				LCD_strUserData[LCD_u8CurrentBuffer].LCD_enuCurrPos++;
			}
		}
		else{
			LCD_enuWriteState = LCD_enuWriteDone;
		}
		break;

	case LCD_enuWriteDone:

		LCD_strUserData[LCD_u8CurrentBuffer].LCD_enuBuffered = LCD_NOTBUFFERED;

		/* CHECKING IF THERE IS A BUFFERED ONE */
		for(LCD_u8BufferIndex=0;LCD_u8BufferIndex<LCD_BUFFERSIZE;LCD_u8BufferIndex++){

			if(LCD_strUserData[LCD_u8BufferIndex].LCD_enuBuffered == LCD_BUFFERED){
				break;
			}
		}

		/* ALL BUFFERS ARE EMPTY */
		if(LCD_u8BufferIndex == LCD_BUFFERSIZE){
			LCD_enuCurrRequest = LCD_enuReqIdle;
			LCD_u8CurrentBuffer = 0;
			LCD_u8BusyFlag = DISABLE;
		}
		else{
			LCD_enuWriteState     = LCD_enuWriteStart;
			//LCD_u8CurrentBuffer += 1;
			LCD_u8CurrentBuffer = LCD_u8BufferIndex;

			if(LCD_u8CurrentBuffer == LCD_BUFFERSIZE){
				LCD_u8CurrentBuffer = 0;
			}
		}
		break;

	}

}
