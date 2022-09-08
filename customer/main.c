#pragma code symbols debug oe
#define EXTERN
#include <REG52.H>
#include <math.h>
#include "./_solidic/head_file_version.h"
#include "./_display/dvr_lcd_SDI1621.h"



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

main()
{
	P1M0 = 0xff;
	P1M1 = 0x00;
	P2M0 = 0xff;
	P2M1 = 0x00;
	P2 = 0x00;
	P1 = 0x00;

	iLCD_GLASS_Init();
	LCD_GLASS_String("TORREY",LCD_PESO);
	LCD_GLASS_String("Y",LCD_PRECIO);
	LCD_GLASS_String("2022",LCD_TOTAL);
 

	init_pwm();
	BL_DIS;
	BEEPER_EN;

while(1){	
	;
/*P1 = 0x04;
	delay_ms(400);
	P1 = 0x00;
	delay_ms(400);
	P2 = 0x0E;
	delay_ms(200);
	P2 = 0x00;
	delay_ms(200);*/
}

}

/*	while(1){

LCD_DATA_ON;//P2.2
delay_ms(10);
LCD_DATA_OFF;
delay_ms(10);

	}*/
