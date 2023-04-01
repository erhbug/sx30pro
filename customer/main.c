#include <REG52.H>
#include "./_solidic/head_file_version.h"
#include "./_scale/dvr_def.h"
#include "./_scale/dvr_scale.h"
#include "./customer/beeper.h"
#include "./customer/dvr_inicio.h"

#include "./customer/dvr_battery.h"
#include <stdio.h>
#include <string.h>

void wdt_init(void);
void gpio_init(void);
void adc_init(void);
void init_int_timer0(void);
Parameter stScaleParam;	/* Contiene los parametros de uso de la Bascula*/

/*Display*/
volatile SOLIDIC Display;

/*flash*/
unsigned char NRM_securty;

/*dvr_scale*/
float fWeightScale = 0;									/* Contiene el valor del peso leido */
float fWeightScaleBefore = 0;
float fWeightLight = 0;
idata FlagScale srFlagScale;		/* Contiene las banderas del sistema */
unsigned int iCounterZeroTracking = 0;
extern float fVoltage_Battery;
enum ActionScale eAccionScale;

void main(void) {
  wdt_init();
  gpio_init();
  adc_init();
  init_pwm(0x01);
  init_int_timer0();
  LCD_GLASS_Init();
  eAccionScale = ScalePreOn; /* Inicia en el primer estado Off */  
  

  while (1) {
   switch (eAccionScale) {
    case ScalePreOn:
      IWDG_KEY_REFRESH;    
      memset( & srFlagScale, 0x00, sizeof(srFlagScale));
      memset( & stScaleParam, 0x00, sizeof(stScaleParam));  
      vReadParamScale(); // Inicia los parametros de la Bascula		      			
	    LCD_GLASS_Clear();
      LCD_GLASS_String("-----", LCD_PESO);
      LCD_GLASS_String("-----", LCD_PRECIO);
      LCD_GLASS_String("------", LCD_TOTAL); 
	  OnBackLight;	
	  
      vBeep_Key();
      eAccionScale = ScaleBattery; // Pasa al siguiente estado
      break;

    case ScaleBattery:	 
      eAccionScale = ScaleWait;
      break;

    case ScaleWait:
      if (cWait_Scale() == 0) {
        eAccionScale = ScaleRun;
        LCD_GLASS_Clear();
      } else
        eAccionScale = ScalePreOn;
      break;

    case ScaleRun:
      	// Lee teclado y ejecuta las acciones correspondientes 
	    //OnBackLight;
      vScan_Key();
	    cRun_Scale();
      break;

    } //switch
  } //while
} //main

//##################################################
void init_pwm(unsigned char BlkPWM){
//apagar bl y beeper
	BL_EN;
	BEEPER_DIS;

   //Configuracion salida BL
    P1M0 |= (1<<5);
    P1M1 &= ~(1<<5);   

    PWMF_H  = 0x00;
	  PWMF_L  = 0xA0;
	  PWM0  	= 0X6C;//BEEPER correcto 0x6c
	  PWM1  	= BlkPWM;//0x10;//0X50;
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
  /*########################################
  GPIO Control:
  PxM1n:  PxM0n   I/O Modo de puerto
  0       0       Salida Estandar 51
  0       1       Salida Push Pull CMOS
  1       0       Entrada
  1       1       Colector abierto

  P0M0 Registro de configuración del modo del puerto
  P0M1 Registro de configuración del modo del puerto
  */
    P0 = 0x0C;//P0 = 0x04;
    P1 = 0x40;
    P2 = 0x11;

    P0M0 = 0xF0; //0b11111111;
    P0M1 = 0x00; //0b00000000;    
    P1M0 = 0xBF; //0b10111111;
    P1M1 = 0x00; //0b00000000;        
    P2M0 = 0xEE; //0b11101110;
    P2M1 = 0x00; //0b00000000;

	/*d4 keyboard P0.3 d4salida
    P0M0 |= 0x08; //0b11111111;
    P0M1 = 0x00; //0b00000000;  

//config miso //p1.2 miso in
    P1M0 &= 0xFB; //0b11111011;
    P1M1 |= 0x00; //0b00000000;   
*/
    MISO = 1;
    SCLK = 0;
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

/////////   Interrups timer0    ////////////////
static void timer0(void) interrupt 1
{	
    
  if(strTimer.iTimerA>0){ 
      if(strTimer.iTimerA==1)BEEPER_EN;	

	  strTimer.iTimerA++;
		if(strTimer.iTimerA>=TimerAend)BEEPER_DIS;
	}

if(strTimer.iTimerBlk>0 )
	strTimer.iTimerBlk++;

//if(strTimer.iTimerC>0 ){
	strTimer.iTimerC++;
  if (strTimer.iTimerC>=TimerCend)
  {
    strTimer.iTimerC=0;
	if(srFlagScale.bReadBattery==0)
		srFlagScale.bReadBattery++;
  }
//}

	if(strTimer.iTimerE>0 )
	strTimer.iTimerE++;

  if(strTimer.iTimerCharge>0 )
	strTimer.iTimerCharge++;

	//P0 ^= (1<<1);//P1 ^= (1<<5);
	TL0 = 0xCF;
	TH0 = 0xb5;
	TCON |= (1<<4);
}
