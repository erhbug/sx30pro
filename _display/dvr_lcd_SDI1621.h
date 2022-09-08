#include <REG52.H>
#include <stdio.h>
#include <string.h>
#include "./_display/app_cfg.h"
/* Define to prevent recursive inclusion -------------------------------------*/
 typedef struct
  {
    unsigned char mode;
		unsigned char cmd;
		unsigned char LCD_BUF[16];
    unsigned char addr;
		unsigned char counter;
  } SOLIDIC;

///////////////////////////////////////////////////////////////////////////////
#define SYS_DIS 0X00//Turn off both system oscillator and LCD bias generator
#define SYS_EN  0X01//Turn on system oscillator
#define LCD_OFF 0X02//Turn off LCD bias generator
#define LCD_ON  0X03//Turn on LCD bias generator
#define BIAS_2  0X20//00100000 //LCD 1/2 bias option 2 commons option
#define BIAS_3  0X24//00100100 //LCD 1/2 bias option 3 commons option
#define BIAS_4  0X28//00101000 //LCD 1/2 bias option 4 commons option
#define COM_2   0X21//00100001 //LCD 1/3 bias option 2 commons option
#define COM_3   0X25//00100101 //LCD 1/3 bias option 2 commons option
#define COM_4   0X29//00101001 //LCD 1/3 bias option 2 commons option		
#define LCD_COMAND  0X00 
#define LCD_DATA    0X01

#define LCD_CS_N_ON				P2 |= 0x02 
#define LCD_CS_N_OFF			P2 &= 0xFD 

#define LCD_WR_N_ON				P2 |= 0x04 
#define LCD_WR_N_OFF			P2 &= 0xFB 

#define LCD_DATA_ON				P2 |= 0x08 
#define LCD_DATA_OFF			P2 &= 0xF7 

///////////////////////////////////////////////////////////////////////////////
#define LCD_BufferSize          16
#define LCD_Address							0x70

#define SYMBOL_ALARM_HI	7
#define SYMBOL_MENOS		8
#define SYMBOL_ALARM		9
#define SYMBOL_ALARM_LO	10
#define SYMBOL_AC				11
#define SYMBOL_BATT			12
#define SYMBOL_TARE			13
//#define SYMBOL_ALL		14
#define SYMBOL_GC				15
#define SYMBOL_OZ				16


#define SYMBOL_KG		0
#define SYMBOL_LB		1
#define SYMBOL_ZERO	2
#define SYMBOL_NET	3
#define SYMBOL_X		4
#define SYMBOL_Y		5
#define SYMBOL_ALL	6


#define Blink_OFF       0
#define Blink_500mHz    2
#define Blink_1Hz       4
#define Blink_2Hz				6

void delay_35u(void);
int iLCD_GLASS_Init(void);
void lcd_unit_write(unsigned char d);
void write_lcd(SOLIDIC Display);
int iLCD_GLASS_DeInit(void);
int iLCD_GLASS_Clear(void);
int iLCD_GLASS_All_On(void);
int iLCD_GLASS_Blink(char comand_value);
void LCD_SET_Char(char cCaracter, char cPosition_On_LCD, 
	char cPosition_Text);
void LCD_GLASS_Dot(char iNumber_Dot, char cPosition_On_LCD, char cFLag_On);
void LCD_GLASS_Symbols(char cSymbol, char cFlag_On);
void LCD_GLASS_Float(float fNumber_To_LCD, char iNumber_Decimal, char cPosition_On_LCD);
void LCD_GLASS_String(char *pCaracter, char cPosition_On_LCD);
void LCD_GLASS_Update(void);