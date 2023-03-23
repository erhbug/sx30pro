/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DVR_DEF
#define __DVR_DEF
#include "./_scale/app_cfg.h"
#include "./_solidic/SDI5219_Vc_Sep02_15.h"	

#define TIMTICK         (TIM4)		/* Se define el timer a utilizar */

#define BL_EN	P1 |= 0x20
#define BL_DIS	P1 &= 0xDF

#define BEEPER_EN	P1 |= 0x10
#define BEEPER_DIS	P1 &= 0xEF

void init_pwm(unsigned char BlkPWM);

#define OnBackLight 		if(srFlagScale.bBacklight_On == 0){srFlagScale.bBacklight_On = 1; BL_EN; init_pwm(0x50);}strTimer.iTimerBlk=1;
#define DecreaseBackLight	if(srFlagScale.bBacklight_On == 1){srFlagScale.bBacklight_On = 0; BL_EN; init_pwm(0x10);}
#define OffBackLight		srFlagScale.bBacklight_On = 0; BL_DIS; strTimer.iTimerBlk=0;//P1&= ~(1<<5);
/* ACCIONES */

/*

*/
#define Number_Count_Sec	1000

#define Beep_On_Key					strTimer.iTimerA = 1
#define Time_Sleep					strTimer.iTimerB = 1//; strTimer.cFLag_TimerB_End = 0

/*#define Auto_Off_On_15m 				strTimer.iTimerMinuteF = 60*15;	strTimer.cFLag_TimerF_Start = 1
#define Auto_Off_On_5m 					strTimer.iTimerMinuteF = 60*5;	strTimer.cFLag_TimerF_Start = 1
#define Auto_Off_On_5m_Restart	srFlagScale.cAuto_Off_Time = 0

#define Restart_Sleep_Time	strTimer.iTimerMinuteG = 30*1; srFlagScale.bActiveSaveBattery = 0; strTimer.cFLag_TimerG_On = 1;


#define ONOFF_uC		GPIO_ReadInputDataBit(GPIOF, ONu)

#define uON_ON			GPIO_SetBits(GPIOG, OFFu)
#define uON_OFF			GPIO_ResetBits(GPIOG, OFFu)

#define uON_Carga		GPIO_SetBits(GPIOA, PUSH_SWITCH)
#define uOFF_Carga	GPIO_ResetBits(GPIOA, PUSH_SWITCH)*/

#define IWDG_KEY_REFRESH {WD_TA = 0x05; WD_TA = 0x0a;WDCON = 0x1f;}

#define Parameter_Configuration		0
#define Parameter_Calibration			1
#define Parameter_Register				2
#define Parameter_All							3
//#define Parameter_Debug						4
#define Parameter_IncDivisiones		5
#define Parameter_Overload				6
#define Parameter_LinePrinter			7
#define Parameter_ModeloTeclado		8
#define Parameter_NumSerieCalidad	9
#define Parameter_Point_Zero_Calibration 	10
#define Parameter_Count_Calibration				11
#define Parameter_Count_Configuration			12
#define Parameter_Voltages								13
//#define Parameter_Temperature							14
//#define Parameter_Backlight								15

#define ESPANOL 1
#define ENGLISH	0

#define MONEDA_PESOS		1

#define PreConfig15KG 0
#define PreConfig20KG 1
#define PreConfig30KG 2
#define PreConfig40KG 3
#define PreConfig60LB 4
#define PreConfig31KG 5
#define PreConfig32KG 6

#define Multirango_Off	0
#define All_Segment			1
#define Data_Scale			2
#define Offset_Adc			3
#define Erase_Plus			4
#define Reset_Counter		5
#define View_Counter_Calibration		6
#define View_Counter_Configuration 	7
#define View_Voltage_Battery				8
#define Backlight_Intensity_Lcd			9
#define View_Memory									10
#define Funcion_Especial						11
#define Activar_Debug								12
#define Info_Calibracion						13
#define Funcion_IncDivisiones				14
#define Function_Temperature				16
#define Function_VerOverload				19
#define Function_VerNumeroSerie			22
#define Function_TestTeclado				23
#define Function_CalidadTest				24
#define Function_Backlight					25
/*
#define ADC_VIN		ADC_Channel_25
#define ADC_VREF	ADC_Channel_Vrefint

#if DISPLAY_20400047_EN > 0
		#define LCD_TOTAL   0 
		#define LCD_PESO    1 
		#define LCD_PRECIO  2
#else
	#define LCD_PESO		0
	#define LCD_PRECIO	1
	#define LCD_TOTAL		2
#endif

#define SYMBOL_KG			0
#define SYMBOL_LB			1
#define SYMBOL_ZERO		2
#define SYMBOL_NET		3
#define SYMBOL_X			4
#define SYMBOL_Y			5
#define SYMBOL_ALL		6

#define PreConfig15KG 0
#define PreConfig20KG 1
#define PreConfig30KG 2
#define PreConfig40KG 3
#define PreConfig60LB 4
#define PreConfig31KG 5
#define PreConfig32KG 6



#define Parameter_Configuration		0
#define Parameter_Calibration			1
#define Parameter_Register				2
#define Parameter_All							3
#define Parameter_Debug						4
#define Parameter_IncDivisiones		5
#define Parameter_Overload				6
#define Parameter_LinePrinter			7
#define Parameter_ModeloTeclado		8
#define Parameter_NumSerieCalidad	9
#define Parameter_Point_Zero_Calibration 	10
#define Parameter_Count_Calibration				11
#define Parameter_Count_Configuration			12
#define Parameter_Voltages								13
#define Parameter_Temperature							14
#define Parameter_Backlight								15

#define ESPANOL 1
#define ENGLISH	0

#define MONEDA_DLLS			0
#define MONEDA_PESOS		1
#define MONEDA_EUROS		2
#define MONEDA_BSF			3

#define BEEPER_SMT			0
#define BEEPER_TH				1*/

/* VARIABLES PERSONALIZADAS */

#endif