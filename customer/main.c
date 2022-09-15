//#pragma code symbols debug oe
//#define EXTERN
#include <REG52.H>
//#include <math.h>
#include "./_solidic/head_file_version.h"
#include "./_display/dvr_lcd_SDI1621.h"
#include "./_weight/dvr_HX712.h"
#include "./_scale/dvr_scale.h"
#include "./_scale/dvr_def.h"
#include "./_scale/app_cfg.h"
#include "./_battery/dvr_battery.h"
#include "./customer/keyboard.h"
#include "./_inicio/dvr_inicio.h"

//#define IWDG_KEY_REFRESH WD_TA = 0x05; WD_TA = 0x0a;WDCON = 0x1f;

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

void init_pwm(void){
    PWMF_H  = 0x0F; //0FFF ->1.25k
	PWMF_L  = 0xFF;//0xF0;->2k//0xC0;->3.33K//0x80;->5K//0x60;->6.66//0x40;->10K//0x10;//0x20;
	PWM0  	= 05;
	PWM1  	= 03;
	PWMCON  = 0x04;	//PWM0-P1.4(LCD_LAMP)????(?PWM0=0xff?,?????)
}

void main(void)
{
  unsigned char cRta_Function = 0;
  char txt[5]={0};
  float peso=0, voltaje=0;
  unsigned int i=0;

   /// watch dog ///
	EA = 0;
    WD_TA = 0x05;
	WD_TA = 0x0a;
	WDCON = 0x1f; /// 4s?,0.2s ///
    EA = 1;

	//int a = 0, b = 0;
    P0M0 = 0xF0; //0b11111111;
    P0M1 = 0x00; //0b00000000;    
    P1M0 = 0xBF; //0b10111111;
    P1M1 = 0x00; //0b00000000;        
    P2M0 = 0xEE; //0b11101110;
    P2M1 = 0x00; //0b00000000;
	//Config lectura adc Only input
	P0M0 &= ~(1<<5);
	P0M1 |= (1<<5);
	//Configuracion salida BL
    P1M0 |= (1<<5);
    P1M1 &= ~(1<<5);
	//Configuracion salida pin P0.1 prueba de togle pin interrup timer0. Salida para toogle prueba
    P0M0 |= (1<<1);
    P0M1 &= ~(1<<1);
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
	//Salida en 1 para lectura de adc
	P0|= (1<<5);

	//Interrupcion timer0
	IE = 0x82;
	IP = 0x02;
	
	//timer0
	TMOD = 0x00;
	TL0 = 0x00;
	TH0 = 0x7F;
	CKCON = 0x04;

/*	P2 = 0x00;
	P1 = 0x00;*/

/*	iLCD_GLASS_Init();
			delay_ms(500);
	LCD_GLASS_String("TORR",LCD_PESO);
		delay_ms(500);
	LCD_GLASS_String("EY",LCD_PRECIO);
			delay_ms(500);    */
 
	init_pwm();
	BL_DIS;
	BEEPER_DIS;//BEEPER_EN;
	
	TCON |= (1<<4); //Inicio de timer0
	eAccionScale = ScalePreOn;

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
		iLCD_GLASS_Clear();
		LCD_GLASS_String("-----", LCD_PESO);
		delay_ms(500);
		LCD_GLASS_String("-----", LCD_PRECIO);
		delay_ms(500);
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

		vMenu_Configuration_Scale();				
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
		LCD_GLASS_Clear();				
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
/* TCON |= (1<<4);
 iLCD_GLASS_Clear();
 while(1){   
 //key_scan();
	//sprintf(txt,"%d  ",(int)(Key));
	//LCD_GLASS_String(txt,LCD_TOTAL)

	//sprintf(txt,"%d  ",(int)(KeyState));
	//LCD_GLASS_String(txt,LCD_PRECIO);

	
	delay_ms(50);

	peso=fRead_Adc(0);
	P0|= (1<<5);
	voltaje=convertidorADC()*(3.3/255);
	LCD_GLASS_Float(peso, 2,  LCD_TOTAL);
	LCD_GLASS_Float(voltaje, 2, LCD_PESO);

}*/

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
		if(strTimer.cFLag_TimerA_Start = 1){
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
