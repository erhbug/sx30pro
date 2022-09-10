//#pragma code symbols debug oe
//#define EXTERN
#include <REG52.H>
//#include <math.h>
#include "./_solidic/head_file_version.h"
#include "./_display/dvr_lcd_SDI1621.h"
#include "./_weight/dvr_HX712.h"


unsigned char Key;
unsigned char lecturaADC[4]= {0};

#define     BL_EN	P1 |= 0x20
#define     BL_DIS	P1 &= 0xDF

#define     BEEPER_EN	P1 |= 0x10
#define     BEEPER_DIS	P1 &= 0xEF



void init_pwm(void){
    PWMF_H  = 0x00;
	PWMF_L  = 0x20;
	PWM0  	= 05;
	PWM1  	= 03;
	PWMCON  = 0x04;	//PWM0-P1.4(LCD_LAMP)????(?PWM0=0xff?,?????)
}

unsigned char a[512]={0};

void main(void)
{char txt[5]={0};
 float peso=0;
	//int a = 0, b = 0;
    P0M0 = 0xF0; //0b11111111;
    P0M1 = 0x00; //0b00000000;    
    P1M0 = 0xBF; //0b10111111;
    P1M1 = 0x00; //0b00000000;        
    P2M0 = 0xEE; //0b11101110;
    P2M1 = 0x00; //0b00000000;
/*bit KEY_K0 = P0^2;	
sbit KEY_K1 = P2^4;	
sbit KEY_K2 = P2^0;	
sbit KEY_K3 = P1^6;		*/

/*	P1M0 = 0xff;
	P1M1 = 0x00;
	P2M0 = 0xff;
	P2M1 = 0x00;*/

    P0 = 0x0C;//P0 = 0x04;
    P1 = 0x40;
    P2 = 0x11;

/*	P2 = 0x00;
	P1 = 0x00;*/

	iLCD_GLASS_Init();
			delay_ms(500);
	LCD_GLASS_String("TORR",LCD_PESO);
		delay_ms(500);
	LCD_GLASS_String("EY-",LCD_PRECIO);
			delay_ms(500);    
 
	init_pwm();
	BL_DIS;
	BEEPER_EN;
	

/*	while(1)
	{
	KEY_D0=1;
KEY_D1=1;
KEY_D2=1;
KEY_D3=1;
KEY_D4=1;
delay_ms(100);

KEY_D0=0;
KEY_D1=0;
KEY_D2=0;
KEY_D3=0;
KEY_D4=0;
delay_ms(100);

	}*/

 while(1){   
 /*key_scan();
	sprintf(txt,"%d  ",(int)(Key));
	LCD_GLASS_String(txt,LCD_TOTAL)*/

	//sprintf(txt,"%d  ",(int)(KeyState));
	//LCD_GLASS_String(txt,LCD_PRECIO);


	delay_ms(50);

	peso=fRead_Adc(0);
	LCD_GLASS_Float(peso, 2,  LCD_TOTAL);


}

}
