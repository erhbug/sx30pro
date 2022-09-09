#pragma code symbols debug oe
#define EXTERN
#include <REG52.H>
#include <math.h>
#include "./_solidic/head_file_version.h"
#include "./_display/dvr_lcd_SDI1621.h"
#include "./customer/keyboard.h"



unsigned char Key;

#define     BL_EN	P1 |= 0x20
#define     BL_DIS	P1 &= 0xDF

#define     BEEPER_EN	P1 |= 0x10
#define     BEEPER_DIS	P1 &= 0xEF

void delay_ms(unsigned int num)
{ 	
	unsigned int i,j;
  	for(i= 0;i<num;i++)	//(SDI5219) ??1ms
		for(j=0;j<164;j++)
			;
}

void init_pwm(void){
    PWMF_H  = 0x00;
	PWMF_L  = 0x20;
	PWM0  	= 05;
	PWM1  	= 03;
	PWMCON  = 0x04;	//PWM0-P1.4(LCD_LAMP)????(?PWM0=0xff?,?????)
}


void main(void)
{char txt[5]={0};
    P0M0 = 0xFB; //0b11111011;
    P0M1 = 0x04; //0b00000100;    
    P1M0 = 0xBF; //0b10111111;
    P1M1 = 0x40; //0b01000000;        
    P2M0 = 0xEE; //0b11101110;
    P2M1 = 0x11; //0b00010001

/*	P1M0 = 0xff;
	P1M1 = 0x00;
	P2M0 = 0xff;
	P2M1 = 0x00;*/

	P0 = 0x04;
    P1 = 0x40;
    P2 = 0x11;

/*	P2 = 0x00;
	P1 = 0x00;*/

	iLCD_GLASS_Init();
			delay_ms(500);
	LCD_GLASS_String("-TORR",LCD_PESO);
		delay_ms(500);
	LCD_GLASS_String("EY-",LCD_PRECIO);
			delay_ms(500);


	

    
 

	init_pwm();
	BL_DIS;
	BEEPER_EN;

while(1){	

    // key_scan();
//Key=0;
	sprintf(txt,"0000");
if(KEY_K0!=0)txt[0]='1';
if(KEY_K1!=0)txt[1]='1';
if(KEY_K2!=0)txt[2]='1';
if(KEY_K3!=0)txt[3]='1';


//	sprintf(txt,"%d  ",(int)(Key));
LCD_GLASS_String(txt,LCD_TOTAL);
	delay_ms(500);

}

}


