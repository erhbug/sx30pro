//#pragma code symbols debug oe
//#define EXTERN
#include <REG52.H>
//#include <math.h>
#include "./_solidic/head_file_version.h"
#include "./_display/dvr_lcd_SDI1621.h"
#include "./_weight/dvr_HX712.h"
#include "./_scale/app_cfg.h"
#include "./_scale/dvr_scale.h"
#include "./_scale/dvr_def.h"
#include "./_battery/dvr_battery.h"
#include "./customer/keyboard.h"
#include "./_inicio/dvr_inicio.h"
#include "./_data_nvm/data_nvm_5219_Vc_Dec09_13.h"
	
unsigned int convertidorADC(void);
void TestEEPROM(void);
void init_pwm(void);
void wdt_init(void);
void gpio_init(void);
void adc_init(void);
void init_int_timer0(void);

enum ActionScale Key; //unsigned char Key = 0;
unsigned char lecturaADC[4]= {0};
enum ActionScale eAccionScale;
struct Parameter stScaleParam;
struct FlagScale srFlagScale;
struct strTimers strTimer;

#define     BL_EN	P1 |= 0x20
#define     BL_DIS	P1 &= 0xDF

#define     BEEPER_EN	P1 |= 0x10
#define     BEEPER_DIS	P1 &= 0xEF


enum ActionScale eAccionScale;	



void main(void){

unsigned char cRta_Function = 0;
  char txt[7]={0};
  float peso=0, voltaje=0;
  unsigned int i=0;

  wdt_init();  /// watch dog ///
  gpio_init();
  adc_init();
  init_pwm();
  // Configuracion salida pin P0.1 prueba de togle pin interrup timer0. Salida
  // para toogle prueba
  // P0M0 |= (1<<1);
  // P0M1 &= ~(1<<1);
  //init_int_timer0();
  iLCD_GLASS_Init();
	eAccionScale = ScalePreOn;

while(1){
//iLCD_GLASS_Clear();
switch(eAccionScale){

	case ScalePreOn:
			
		srFlagScale.bScaleOn = 1;//TRUE;
		//uON_ON;
		//vClearMemory();			
		//vReadParamScale();	// Inicia los parametros de la Bascula					

		//IWDG->KR = IWDG_KEY_REFRESH;
			
		srFlagScale.bReadBattery = 0;

		#if DISPLAY_20400047_EN > 0
		vGestorBateria();
		driver_symbol_init();
		#endif
			
		iLCD_GLASS_Init();
		LCD_GLASS_String("-----", LCD_PESO);
		LCD_GLASS_String("-----", LCD_PRECIO);
		LCD_GLASS_String("------", LCD_TOTAL);
		delay_ms(500);
		
		srFlagScale.bTopeSobrePeso = 0;
		
		vBeep_Key();
		
		eAccionScale = ScaleBattery; // Pasa al siguiente estado
		
		//EXTI_ClearITPendingBit(EXTI_IT_Pin0);
		//GPIO_Init(GPIOF, ONu, GPIO_Mode_In_FL_IT);
		//LCD_GLASS_Float(cRta_Function, 0, LCD_PRECIO);
	break;

	case ScaleBattery:
		
		// Verifica que el voltaje sea el apropiado para operar
		cRta_Function = uGet_Status_Volt();
		
		if(cRta_Function != SOURCE_BATTERY_VERY_LOW){
			if(cRta_Function == SOURCE_BATTERY_LOW){
				iLCD_GLASS_Clear();
				if(stScaleParam.cLenguage == ESPANOL){
					LCD_GLASS_String("BAJA  ", LCD_PRECIO);	
					LCD_GLASS_String("   BAT", LCD_TOTAL);
				}else{
					LCD_GLASS_String("LOW   ", LCD_PRECIO);	
					LCD_GLASS_String("  BATT", LCD_TOTAL);
				}
			}
			
		if(cRta_Function != SOURCE_ADAPTER && srFlagScale.bBateriaLow == 0)
			{
				strTimer.cFLag_TimerJ_Start = 1;
				if(stScaleParam.cBacklight){OnBackLight;}
			}
			strTimer.cFLag_TimerC_Start = 1;					
			eAccionScale = ScaleWait;
		}else{
			
			iLCD_GLASS_Clear();

			if(stScaleParam.cLenguage == ENGLISH){
				LCD_GLASS_String("LOW ", LCD_PESO);	
				LCD_GLASS_String("BATT", LCD_PRECIO);
				LCD_GLASS_String(" OFF", LCD_TOTAL);
			}else{
				LCD_GLASS_String("BAJA ", LCD_PESO);	
				LCD_GLASS_String("BAT", LCD_PRECIO);
				LCD_GLASS_String(" OFF", LCD_TOTAL);
			}

	#if !SX30PRO			
			strTimer.cFLag_TimerE_Start = 1;
			strTimer.cFLag_TimerE_End = 0;		
			while(strTimer.cFLag_TimerE_End == 0){
				IWDG->KR = IWDG_KEY_REFRESH;WD_TA = 0x05; WD_TA = 0x0a;WDCON = 0x1f;
				
				//if(srFlagScale.bScaleOn == FALSE){
					//eAccionScale = ScalePreOff;				/* Inicia en el primer estado Off */
				//}
			}
			IWDG->KR = IWDG_KEY_ENABLE;
			while(1);    // Wait until reset occurs from IWDG
	#endif	
			eAccionScale = ScaleBattery;			/* Inicia en el primer estado Off */
			
		}
	delay_ms(5000);		
	break;

	case ScaleWait:
				
		//GPIO_Init(GPIOA, PUSH_SWITCH, GPIO_Mode_In_PU_No_IT);
		
		if(cWait_Scale() == 0){
			
			eAccionScale = ScaleRun;
			//Auto_Off_On_15m;
			strTimer.iTimerF = Number_Count_Sec;
			//KeyPressed = FALSE;
			
			if(srFlagScale.bScaleOn == FALSE){
				eAccionScale = ScalePreOff;				//// Inicia en el primer estado Off ////
			}
		}else if(srFlagScale.bCodeConfiguration == 1){
			eAccionScale = ScaleCode;
		}
		
		//GPIO_Init(GPIOA, PUSH_SWITCH, GPIO_Mode_Out_PP_Low_Slow);
		
	break;

	case ScaleCode:
	
		cRta_Function = cWrite_Code_Scale();
		
		if(srFlagScale.bScaleOn == FALSE){
			eAccionScale = ScalePreOff;				/* Inicia en el primer estado Off */
		}else if(cRta_Function == 0){
			eAccionScale = ScaleMenu;
		}else if(cRta_Function == 1){
			eAccionScale = ScaleCalibrate;
		}else{
			eAccionScale = ScalePreOff;				/* Inicia en el primer estado Off */
		}
					
		break;

	case ScaleCalibrate:	
	
		vCalibrate_Scale();		/* Llama la funcion para el proceso de calibracion*/
		/* al terminar de calibrar se pide que apague la bascula, si no, se 
		 apagara automaticamente a los 2 min */	 
		
		if(srFlagScale.bScaleOn == FALSE){
			eAccionScale = ScalePreOff;				/* Inicia en el primer estado Off */
			break;
		}
		
		LCD_GLASS_String("  OFF", LCD_PRECIO);	/* Muestra el mensaje de
																							apagar la bascula */

		//Auto_Off_On_5m_Restart;
		//Auto_Off_On_5m;
		
		while(!srFlagScale.cWait_Time_Code){
			IWDG_KEY_REFRESH;//IWDG->KR = IWDG_KEY_REFRESH;
			key_scan();//vActionKey();
			
			if(srFlagScale.bScaleOn == FALSE){
				eAccionScale = ScalePreOff;				/* Inicia en el primer estado Off */
				break;
			}
		}

		eAccionScale = ScalePreOff;
		
		break;

	case ScaleMenu:

		//vMenu_Configuration_Scale();				
		eAccionScale = ScalePreOff;
		
		break;

	case ScaleRun:
		
		if(srFlagScale.bScaleOn == 1){//if(srFlagScale.bScaleOn == TRUE){
			cRun_Scale(1);
		}else{
			eAccionScale = ScalePreOff;				/* Inicia en el primer estado Off */
		}				
		break;
	
	case ScalePreOff:

		stScaleParam.cSpecialAction = 0;
		
		//GPIO_Init(GPIOF, ONu, GPIO_Mode_In_PU_No_IT);
		iLCD_GLASS_Clear();				
		OffBackLight;				// Apaga el Backlight		
		
		// Verifica que el voltaje sea el apropiado para operar
		cRta_Function = uGet_Status_Volt();
		
		if(cRta_Function != SOURCE_ADAPTER){
			//uON_OFF;
			while(i<2000){
				i++;
				IWDG_KEY_REFRESH;//IWDG->KR = IWDG_KEY_REFRESH;
			}
		}
		
		strTimer.cFLag_TimerD_End = 0;
		strTimer.cFLag_TimerD_Start = 1;

		while(!strTimer.cFLag_TimerD_End){//while(ONOFF_uC != 0 && !strTimer.cFLag_TimerD_End){
			IWDG_KEY_REFRESH;//IWDG->KR = IWDG_KEY_REFRESH;
		}
		
		eAccionScale = scaleStandBy; 					

		iLCD_GLASS_DeInit();//LCD_Cmd(DISABLE);		// Deshabilita el modulo LCD 				

		break;

}//swith
}//while


}

void init_pwm(void){
   //Configuracion salida BL
    P1M0 |= (1<<5);
    P1M1 &= ~(1<<5);

    PWMF_H  = 0x00;
	PWMF_L  = 0x20;
	PWM0  	= 05;
	PWM1  	= 03;
	PWMCON  = 0x04;	//PWM0-P1.4(LCD_LAMP)????(?PWM0=0xff?,?????)

	//apagar bl y beeper
	BL_DIS;
	BEEPER_EN;
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

	//P0 ^= (1<<1);//P1 ^= (1<<5);
	TL0 = 0xCF;
	TH0 = 0xAF;
	TCON |= (1<<4);

}


void TestEEPROM(void)
{
	unsigned int addr=ADDRESS_PLU;
	float val;
	unsigned char txt[10]={0};
	float i=0.12;

	unsigned int x=0;
 
	NRM_securty_a = 0xaa;
	NRM_securty_b = 0x55;
	
	flash_write_float32(addr,0.00);
	flash_write_float32(addr+11,0.00);

	while(1){	
	for(x=1;x<11;x++){
	flash_write_float32(addr+(x*4),i);

	val = flash_read_float32(addr+(x*4)-4);
	sprintf(txt,"%f",val);  LCD_GLASS_String(txt,LCD_PESO); 

	val = flash_read_float32(addr+(x*4));
	sprintf(txt,"%f",val);  LCD_GLASS_String(txt,LCD_TOTAL); 	

	val = flash_read_float32(addr+(x*4)+4);
	sprintf(txt,"%f",val);  LCD_GLASS_String(txt,LCD_PRECIO); 

	i++;
	delay_ms(1000);
	}
}

NRM_securty_a = 0x00;
	NRM_securty_b = 0x00;
}
