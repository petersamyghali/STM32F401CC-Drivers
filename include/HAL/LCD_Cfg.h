/*
 * LCD_Cfg.h
 *
 * Created: 10-Feb-23 2:51:30 PM
 *  Author: Peter Ghali
 */ 


#ifndef LCD_CFG_H_
#define LCD_CFG_H_

typedef enum
{
	LCD_RS_PIN,
	LCD_RW_PIN,
	LCD_E_PIN,
	LCD_D0_PIN,
	LCD_D1_PIN,
	LCD_D2_PIN,
	LCD_D3_PIN,
	LCD_D4_PIN,
	LCD_D5_PIN,
	LCD_D6_PIN,
	LCD_D7_PIN,

}LCD_PINS_t;

#define LCD_8BIT_MODE	8
#define  LCD_4BIT_MODE	4
#define Number_OF_CONTROL_PINS	3
/*
In order to know how many bits you are using
Options : 
			LCD_8BIT_MODE
			LCD_4_BIT_MODE
 */
#define LCD_BIT_MODE	LCD_8BIT_MODE

/*
In order to know if you want the cursor to be on or off
Options :
		CURSOR_ON
		CURSOR_OFF
 */
#define	LCD_DISPLAY_CURSOR CURSOR_ON

/*A couple of macros to define the number of rows and the number of cols 
 */
#define NUMBER_OF_ROWS		2
#define	NUMBER_OF_COLMUNS	16

#define LCD_BUFFERSIZE        4


extern const  LCD_Ports_PINs_t LCD_Cfg_Array [LCD_BIT_MODE+ Number_OF_CONTROL_PINS] ;


#endif /* LCD_CFG_H_ */
