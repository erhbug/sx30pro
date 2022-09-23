
#ifndef __SDI1621_H__
#define __SDI1621_H__

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


void LCD_GLASS_Init(void);
void LCD_GLASS_DeInit(void);
void LCD_GLASS_Clear(void);
void LCD_GLASS_All_On(void);
void LCD_GLASS_String(unsigned char *pCaracter, unsigned char cPosition_On_LCD);
void LCD_GLASS_Float(float fNumber_To_LCD, unsigned char iNumber_Decimal, unsigned char cPosition_On_LCD);
void LCD_GLASS_Symbols(unsigned char cSymbol, unsigned char cFlag_On);
void LCD_GLASS_Dot(unsigned char iNumber_Dot, unsigned char cPosition_On_LCD, unsigned char cFLag_On);

#endif

