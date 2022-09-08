#pragma code symbols debug oe
#define EXTERN
#include <REG52.H>
#include <math.h>
#include "./_solidic/head_file_version.h"
#include "./_display/dvr_lcd_SDI1621.h"
#include "./customer/keyboard.h"


unsigned char Key_code;

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

/*sbit KEY_K0 = P0^2;	
sbit KEY_K1 = P2^4;	
sbit KEY_K2 = P2^0;	
sbit KEY_K3 = P1^6;	

sbit KEY_D0 = P0^1;
sbit KEY_D1 = P0^0;	
sbit KEY_D2 = P1^0;	
sbit KEY_D3 = P1^1;	
sbit KEY_D4 = P1^2;*/

	P1M0 = 0xff;
	P1M1 = 0x00;
	P2M0 = 0xff;
	P2M1 = 0x00;
	P2 = 0x00;
	P1 = 0x00;

	iLCD_GLASS_Init();
			delay_ms(500);
	LCD_GLASS_String("-TORR",LCD_PESO);
		delay_ms(500);
	LCD_GLASS_String("EY-",LCD_PRECIO);
			delay_ms(500);

	LCD_GLASS_String("2022",LCD_TOTAL);
				delay_ms(500);

	

    
 

	init_pwm();
	BL_DIS;
	BEEPER_EN;

while(1){	

     key_scan();
	sprintf(txt,"%d",Key_code);
LCD_GLASS_String(txt,LCD_TOTAL);
	delay_ms(50);

}

}


