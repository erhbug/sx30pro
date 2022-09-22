//#pragma code symbols debug oe
//#define EXTERN
#include <REG52.H>
//#include <math.h>
#include "./_solidic/head_file_version.h"
#include "./_display/dvr_lcd_SDI1621.h"
#include "./_weight/dvr_HX712.h"
#include "./customer/keyboard.h"

#include "./_scale/dvr_def.h"
#include "./_scale/dvr_scale.h"
#include <stdio.h>



unsigned int convertidorADC(void);
void TestEEPROM(void);
void init_pwm(void);
void wdt_init(void);
void gpio_init(void);
void adc_init(void);
void init_int_timer0(void);

void main(void)
{
char txt[5];
float peso;
unsigned long iTemp_RA = 0;

  wdt_init();  /// watch dog ///
  gpio_init();
  adc_init();
  init_pwm();
  // Configuracion salida pin P0.1 prueba de togle pin interrup timer0. Salida
  // para toogle prueba
  // P0M0 |= (1<<1);
  // P0M1 &= ~(1<<1);
 // init_int_timer0();

	LCD_GLASS_Init(); 
	LCD_GLASS_String("- - -",LCD_PESO);
	LCD_GLASS_String("-----",LCD_PRECIO); 
    LCD_GLASS_String("------", LCD_TOTAL);
	// vSound_Saved_Param();
	 delay_ms(1000); 		
 LCD_GLASS_Clear();
// vCalibrate_Scale();
 while(1){   
    key_scan();
	sprintf(txt,"%d  ",(int)(Key));
	LCD_GLASS_String(txt,LCD_PESO);

//	sprintf(txt,"%d  ",(int)(KeyState));
//	LCD_GLASS_String(txt,LCD_PRECIO);

	
//	delay_ms(50);
   
//	peso=fRead_Adc(0);
//	sprintf(txt,"%f   ",peso);  LCD_GLASS_String(txt,LCD_PESO); 

	P0|= (1<<5);
//	voltaje=convertidorADC()*(3.3/255);
 //	LCD_GLASS_Float(peso, 2,  LCD_TOTAL);
//	LCD_GLASS_Float(voltaje, 2, LCD_PESO);


    
  //iTemp_RA=123456789;	
	
	ReadHX712(&iTemp_RA); 
    sprintf(txt,"%ld   ",iTemp_RA);  LCD_GLASS_String(txt,LCD_TOTAL); 

 //delay_ms(3000); 
//TestEEPROM();
}

}

void init_pwm(void){
//apagar bl y beeper
	BL_DIS;
	BEEPER_DIS;

   //Configuracion salida BL
    P1M0 |= (1<<5);
    P1M1 &= ~(1<<5);

    PWMF_H  = 0x00;
	PWMF_L  = 0xA0;
	PWM0  	= 0X6C;//BEEPER
	PWM1  	= 0X50;
	PWMCON  = 0x04;	//PWM0-P1.4(LCD_LAMP)????(?PWM0=0xff?,?????)

	
}

void wdt_init(void){// watch dog ///
    EA = 0;
    WD_TA = 0x05;
	WD_TA = 0x0a;
	WDCON = 0x1f; /// 4s?,0.2s ///
    EA = 1;
	IWDG_KEY_REFRESH;
}

void gpio_init(void)
{
    P0 = 0x0C;//P0 = 0x04;
    P1 = 0x40;
    P2 = 0x11;

    P0M0 = 0xF0; //0b11111111;
    P0M1 = 0x00; //0b00000000;    
    P1M0 = 0xBF; //0b10111111;
    P1M1 = 0x00; //0b00000000;        
    P2M0 = 0xEE; //0b11101110;
    P2M1 = 0x00; //0b00000000;
}

void adc_init(void)
{
    //Config lectura adc Only input
	P0M0 &= ~(1<<5);
	P0M1 |= (1<<5);
	//Salida en 1  para lectura adc
	P0|= (1<<5);
}

void init_int_timer0(void)
{
	//Interrupcion timer0
	IE = 0x82;
	IP = 0x02;	
	//timer0
	TMOD = 0x00;
	TL0 = 0x00;
	TH0 = 0x7F;
	CKCON = 0x04;

    TCON |= (1<<4);//Start timer0
}

unsigned int convertidorADC(){

unsigned int v=0;
SARCON = 0x09;
	if(!(SARCON & 0x04))
	{
		SARCON |= 0x04;
		while(SARCON & 0x04)
		{
		}
	}
v=SARDATA;
SARCON &= 0xf7;
return v;

}
/////////   Interrups timer0    ////////////////

static void timer0(void) interrupt 1
{		
	
		/* 200mS*/
	if(strTimer.cFLag_TimerA_Start){
		strTimer.iTimerA = 200;//200;//700; //891
		//strTimer.cFLag_TimerA_Start = 0;
		strTimer.cFLag_TimerA_On = 1;
		strTimer.cFLag_TimerA_End = 0;
	}

	/* 5S*/
	if(strTimer.cFLag_TimerE_Start){
		strTimer.iTimerE = Number_Count_Sec * 4;
		strTimer.cFLag_TimerE_Start = 0;
		strTimer.cFLag_TimerE_On = 1;
		strTimer.cFLag_TimerE_End = 0;
	}


	/*Accion de desbordamiento del timer */
	if(strTimer.cFLag_TimerA_On){
		
		/*if(stScaleParam.cTypeBeeper == BEEPER_SMT){
			GPIO_ToggleBits(GPIOA, BEEPER);
		}else{
			GPIO_SetBits(GPIOA, BEEPER);
		}*/
		if(strTimer.cFLag_TimerA_Start == 1){
			strTimer.cFLag_TimerA_Start = 0;
			BEEPER_EN;
		}
		if(strTimer.iTimerA > 0){
			strTimer.iTimerA--;
		}else{
			strTimer.cFLag_TimerA_On = 0;
			strTimer.cFLag_TimerA_End = 1;
			BEEPER_DIS;
			//GPIO_ResetBits(GPIOA, BEEPER);
		}
	}

	/* timer usado en calibracion */
	if(strTimer.cFLag_TimerE_On){
		if(strTimer.iTimerE > 0){
			strTimer.iTimerE--;
		}else{
			strTimer.cFLag_TimerE_On = 0;
			strTimer.cFLag_TimerE_End = 1;
		}
	}

	//P0 ^= (1<<1);//P1 ^= (1<<5);
	TL0 = 0xCF;
	TH0 = 0xb5;
	TCON |= (1<<4);

}


void TestEEPROM(void)
{
	unsigned int addr=ADDRESS_PLU;
	float val;
	float i=0.12;

	unsigned int x=0;
 
	NRM_securty_a = 0xaa;
	NRM_securty_b = 0x55;
	


	while(1){	
	for(x=1;x<11;x++){
	flash_write_float32(addr+(x*4),i);

	val = flash_read_float32(addr+(x*4)-4);
	LCD_GLASS_Float(val, 2,  LCD_PESO);

	val = flash_read_float32(addr+(x*4));
	LCD_GLASS_Float(val, 2,  LCD_TOTAL);

	val = flash_read_float32(addr+(x*4)+4);
	LCD_GLASS_Float(val, 2,  LCD_PRECIO);

	i++;
	delay_ms(1000);
	}
}

NRM_securty_a = 0x00;
	NRM_securty_b = 0x00;
}
