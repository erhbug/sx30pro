/** 
 ******************************************************************************
 * Filename 	dvr_battery_nm.c
 * Abstract 	Functions for Smart battery management.
 * Author 		MC. Jaime Ramirez
 * Company 		Fabricantes de Basculas Torrey S.A. de C.V.
 * Version 		0
 * Date:			15/08/12
 ******************************************************************************
 * Filename 	dvr_battery.c
 * Modify 		Time charge 4 hrs, Charge Current 180 mA, Low Battery Off 3.55
 * Author 		Ing. Edgar Mota
 * Company 		Fabricantes de Basculas Torrey S.A. de C.V.
 * Version 		1
 * Date:			12/02/13
 */
 
#include <REG52.H>
#include <stdio.h>
#include <string.h>

#include "./_scale/dvr_def.h"
#include "./_scale/dvr_scale.h"
#include "./_display/dvr_lcd_SDI1621.h"
#include "./customer/dvr_battery.h"
#include "./customer/keyboard.h"
#include "./_solidic/head_file_version.h"

sbit BAT_DET_PIN = P0^5;

#define HIGH_SPEED_CLOCK 	1
#define LOW_SPEED_CLOCK		0

#define COUNT_1_HORA  3600
#define COUNT_1_MIN		60

#define HIGH_ADAPTER        7.0  //6.1
#define LOW_BATTERY_MSG 		3.5
#define LOW_BATTERY_OFF 		3.2
#define THRESHOLD_VOLTAGE 	4.5	 //#define LOW_VOLTAGE_ADAPTER 4.35
#define REFERENCE						1.22 //0.48
#define HIGH_BATTERY_MSG 		6.0
#define NO_ON						 		3.0  //2.5

#define CuentasAdcRef       1514 //*** Cuentas de ADC para 1.22V de referencia

float fVoltage_Battery = 0;

float FactConvAjustado = 0;

struct Time_Battery{
	long iDescanso_2_Hora;
	long iTimeOut_16;
	long iTrickleCharge;
};
struct Time_Battery stTime_Battery;

extern enum ActionScale eAccionScale;

void vGet_Volts_System(void);
unsigned char uGet_Status_Volt(void);
void vDriver_Backlight_Source(unsigned char cEliminadorOn);

void vVbatBad(void);
void vVbatHigh(void);
void vVadapHigh(void);
//int vGet_ADCx_Value(uint16_t ADC_Channel, int iSampleNumber);
//int vGet_ADCx_Value_Ref(uint16_t ADC_Channel, int iSampleNumber);

#if DISPLAY_20400047_EN > 0

const float delta=0.05; 
const float lvl_5 = 5.14;
const float lvl_3 = 4.0;
const float lvl_2 = 3.65;
const float lvl_1 = 3.52;


unsigned char estado=0;
unsigned char var=0;

void driver_symbol(void);
void driver_symbol_init(void);

#endif

/*

*/
void vGestorBateria(void){
	unsigned char bStatus_Volt;

	if(srFlagScale.bReadBattery > 0){		
		srFlagScale.bReadBattery = 0;		
		bStatus_Volt = uGet_Status_Volt();
		vDriver_Backlight_Source(bStatus_Volt);
		
		switch(bStatus_Volt){
				
			
			case SOURCE_ADAPTER_OK:
				if(srFlagScale.bBateriaLow == 1){
					srFlagScale.bMsgBatteryLow = 0;
					srFlagScale.bBateriaLow = 0;
					srFlagScale.bBateriaCount = 0;
				}
				if(eAccionScale == ScaleRun || eAccionScale ==ScaleWait){					
					if(stScaleParam.cBacklight){OnBackLight;}
				}	
				srFlagScale.bSourceVoltage =  SOURCE_ADAPTER_OK;
				//Auto_Off_On_15m;
				//Restart_Sleep_Time;				
				
				break;
				
			case SOURCE_BATTERY_OK:
				if(srFlagScale.bBateriaLow == 1){
					//LCD_BlinkConfig(LCD_BlinkMode_Off,LCD_BlinkFrequency_Div128);
					srFlagScale.bMsgBatteryLow = 0;
					srFlagScale.bBateriaLow = 0;
					srFlagScale.bBateriaCount = 0;
				}
				srFlagScale.bSourceVoltage =  SOURCE_BATTERY_OK;
				break;
			
			case SOURCE_BATTERY_HIGH:	
				if(srFlagScale.bBateriaLow == 1){
//					LCD_BlinkConfig(LCD_BlinkMode_Off,LCD_BlinkFrequency_Div128);
//					srFlagScale.bMsgBatteryLow = 0;
					srFlagScale.bBateriaLow = 0;
					srFlagScale.bBateriaCount = 0;
				}
				
				vVbatHigh();
				break;
			
			case SOURCE_BATTERY_LOW:
			
				srFlagScale.bSourceVoltage =  SOURCE_BATTERY_LOW;
				srFlagScale.bBateriaLow = 1;
				OffBackLight;
			 //CCC 
				if(srFlagScale.bBateriaCount > 8){
					srFlagScale.bBateriaCount = 0;
					srFlagScale.bMsgBatteryLow = 0;
					//LCD_BlinkConfig(LCD_BlinkMode_Off,LCD_BlinkFrequency_Div128);
				}
				else if(srFlagScale.bBateriaCount >= 7 && srFlagScale.bBateriaCount <= 9 ){				
					//LCD_BlinkConfig(LCD_BlinkMode_AllSEG_AllCOM,LCD_BlinkFrequency_Div128);				
					srFlagScale.bMsgBatteryLow = 1;
					LCD_GLASS_Symbols(SYMBOL_ALL, 0);
					LCD_GLASS_String("     ", LCD_PESO);	
					
					if(stScaleParam.cLenguage == ENGLISH){
						LCD_GLASS_String("LOW  ", LCD_PRECIO);	
						LCD_GLASS_String("  BATT", LCD_TOTAL);
					}else{
						LCD_GLASS_String("BAJA  ", LCD_PRECIO);	
						LCD_GLASS_String("   BAT", LCD_TOTAL);
					}
					LCD_GLASS_Symbols(SYMBOL_ZERO, 0);				
					LCD_GLASS_Symbols(SYMBOL_X, 0);
					return;
				}else{
//					srFlagScale.bMsgBatteryLow = 0;
				}
				break;
				
			case SOURCE_BATTERY_VERY_LOW:
				
				OffBackLight;
				LCD_GLASS_Clear();
//				LCD_BlinkConfig(LCD_BlinkMode_Off,LCD_BlinkFrequency_Div128);
				
				if(stScaleParam.cLenguage == ENGLISH){
					LCD_GLASS_String("LOW  ", LCD_PRECIO);	
					LCD_GLASS_String("  BATT", LCD_TOTAL);
				}else{
					LCD_GLASS_String("BAJA  ", LCD_PRECIO);	
					LCD_GLASS_String("   BAT", LCD_TOTAL);
				}

				if(stScaleParam.cLenguage == ENGLISH){
					LCD_GLASS_String("LOW ", LCD_PESO);	
					LCD_GLASS_String("BATT", LCD_PRECIO);
					LCD_GLASS_String(" OFF", LCD_TOTAL);
				}else{
					LCD_GLASS_String("BAJA ", LCD_PESO);	
					LCD_GLASS_String("BAT", LCD_PRECIO);
					LCD_GLASS_String(" OFF", LCD_TOTAL);
				}

//				strTimer.cFLag_TimerE_Start = 1;
//				strTimer.cFLag_TimerE_End = 0;	
				
//				while(strTimer.cFLag_TimerE_End == 0){
//					vReadKey();
//					IWDG->KR = IWDG_KEY_REFRESH;
//				}
			//	IWDG->KR = IWDG_KEY_ENABLE;
				while(1);    // Wait until reset occurs from IWDG
				
				break;
				
			case SOURCE_ADAPTER_HIGH:
			
				if(srFlagScale.bBateriaLow == 1){
//					LCD_BlinkConfig(LCD_BlinkMode_Off,LCD_BlinkFrequency_Div128);
//					srFlagScale.bMsgBatteryLow = 0;
					srFlagScale.bBateriaLow = 0;
					srFlagScale.bBateriaCount = 0;
				}
				vVadapHigh();
			
				break;
		}

	#if DISPLAY_20400047_EN > 0
	driver_symbol();
	#endif
	
	}

}

/**

	* Salida: 
			SOURCE_ADAPTER 					-> Fuente de voltaje eliminador. 
			SOURCE_BATTERY_OK				-> Fuente voltaje bateria.
			SOURCE_BATTERY_LOW			-> Bateria baja (Msg aviso).
			SOURCE_BATTERY_VERY_LOW	-> Bateria insuficiente AutOff.
			SOURCE_BATTERY_HIGH			-> Voltaje bateria por arriba de especificaciones
**/

#if DISPLAY_20400047_EN > 0
void driver_symbol_init(){
		
		if(fVoltage_Battery>=4.0)
		{
			estado=3;
		}else if(fVoltage_Battery<4.0 && fVoltage_Battery>=3.6)
		{
			estado=2;
		}else if(fVoltage_Battery<3.6 && fVoltage_Battery>=3.5)
		{
			estado=1;
		}else if(fVoltage_Battery<3.5)
		{
			estado=0;
		}
}

/*
*/

void driver_symbol(){

//if(eAccionScale == scalePreOnDc || stScaleParam.cSpecialAction == 1)
//	return;
//else 
if(srFlagScale.bOverLoad == 1)
		{
			LCD_GLASS_Symbols(9,0); //bateria 0/3
			LCD_GLASS_Symbols(8,0);
			LCD_GLASS_Symbols(7,0);
			LCD_GLASS_Symbols(10,0);
			return;
		} 
	
//if(eAccionScale==scalePreOnDc || srFlagScale.bOverLoad ==1)
//	return;

if(srFlagScale.bSourceVoltage >= SOURCE_ADAPTER_LOW)
{
	
	/*if(strTimer.cFLag_TimerCharge_End==1)
	{
			estado++;
		if(estado==4)
			estado=0;
	}*/
	
/*	if(strTimer.cFLag_TimerCharge_On==0)
		strTimer.cFLag_TimerCharge_Start=1;
*/
	if(fVoltage_Battery>=lvl_5)
		estado=5;
/* Condicion de carga completa para TM con dos puntos de lectura de voltaje.
	if(fVoltage_Battery<=5.22 && fVoltage_Battery>=4.72)
		estado=5;
*/		

}else{
			
	if(fVoltage_Battery < (lvl_3 - delta) && estado == 3)	
		estado = 2;
	else if(fVoltage_Battery >= (lvl_3 + delta) && estado == 2)
		estado = 3;
	else if(fVoltage_Battery < (lvl_2 - delta) && estado == 2)
		estado = 1;
	else if(fVoltage_Battery >= (lvl_2 + delta) && estado == 1)
		estado = 2;
	else if(fVoltage_Battery < (lvl_1 - delta) && estado == 1)
		estado = 0;	
	else if(fVoltage_Battery >= (lvl_1 + delta) && estado == 0)
		estado = 1;
	 }

	 if(estado==0)
	{
			LCD_GLASS_Symbols(9,0); //bateria 0/3
			LCD_GLASS_Symbols(8,0);
			LCD_GLASS_Symbols(7,0);
			LCD_GLASS_Symbols(10,1);
	}else if(estado==1)
	{
			LCD_GLASS_Symbols(9,0); //bateria 1/3
			LCD_GLASS_Symbols(8,0);
			LCD_GLASS_Symbols(7,1);
			LCD_GLASS_Symbols(10,1);
	}else if(estado==2)
	{
			LCD_GLASS_Symbols(9,0); //bateria 2/3
			LCD_GLASS_Symbols(8,1);
			LCD_GLASS_Symbols(7,1);
			LCD_GLASS_Symbols(10,1);
	}else if(estado==3)
	{
			LCD_GLASS_Symbols(9,1); //bateria 3/3
			LCD_GLASS_Symbols(8,1);
			LCD_GLASS_Symbols(7,1);
			LCD_GLASS_Symbols(10,1);
	}else if(estado==5)
	{		
			LCD_GLASS_Symbols(9,1); //bateria 3/3
			LCD_GLASS_Symbols(8,1);
			LCD_GLASS_Symbols(7,1);
			LCD_GLASS_Symbols(10,1);
		
	    estado=3;
	}	
}
#endif

unsigned char uGet_Status_Volt(void){
	unsigned char source;
		
	vGet_Volts_System();
	if(fVoltage_Battery>LEVEL_BATTERY_HIGH)fVoltage_Battery+=DIODE_VOLTAGE;
	if(fVoltage_Battery<LEVEL_ADAPTER_HIGH) 	source=SOURCE_ADAPTER_HIGH;
	if(fVoltage_Battery<LEVEL_ADAPTER_OK) 		source=SOURCE_ADAPTER_OK;
	if(fVoltage_Battery<LEVEL_ADAPTER_LOW) 		source=SOURCE_ADAPTER_LOW;
	if(fVoltage_Battery<LEVEL_BATTERY_HIGH) 	source=SOURCE_BATTERY_HIGH;
	if(fVoltage_Battery<LEVEL_BATTERY_OK) 		source=SOURCE_BATTERY_OK;
	if(fVoltage_Battery<LEVEL_BATTERY_LOW) 		source=SOURCE_BATTERY_LOW;
	if(fVoltage_Battery<LEVEL_BATTERY_VERY_LOW) source=SOURCE_BATTERY_VERY_LOW;

	return source;
}

/**


**/
void vGet_Volts_System(void){
	unsigned char iValue_Adc = 0;
	EA = 0;
	BAT_DET_PIN = 1;
	SARCON  = 0x09;
	delay_ms(1);	
	if(!(SARCON & 0x04)) // 
	{
		SARCON |= 0x04;
		while(SARCON & 0x04) // 
		{
		}
	}
	iValue_Adc = (unsigned char)SARDATA;
	SARCON  &= 0xf7;
	EA = 1;
	fVoltage_Battery = ((3.3/256.0 * (float)iValue_Adc)/10.0)*61.1;	
}


/**

**/

void vDriver_Backlight_Source(unsigned char cEliminadorOn){
/*	
	if(cEliminadorOn != SOURCE_ADAPTER && srFlagScale.bBateriaLow == 0)
	{
		if(eAccionScale == ScaleRun || eAccionScale == ScaleWait){
		 if(stScaleParam.cBacklight){OnBackLight;}
		}
	}


	if(cEliminadorOn != 0){
		
		if(srFlagScale.bEliminadorOn == 1){
			OffBackLight;
		}
		
		srFlagScale.bEliminadorOn = 0;		
	}else{

		if(eAccionScale == ScaleRun || eAccionScale == ScaleWait){
			
			if(srFlagScale.bEliminadorOn == 0){
				srFlagScale.bEliminadorOn = 1;
				if(stScaleParam.cBacklight){OnBackLight;}
			}
		}
	} */
}


/**
  ******************************************************************************
  * Objetivo: Proteger el circuito de una polaridad inversa de la bateria
  * Parametros entrada: Ninguno
	*/
void vVbatBad(void){
/*	
	int iValue_Adc = 0;
	float fVoltage_Aux = 0;
	//enum digi_key Value_Key_Press;
	
	LCD_Cmd(ENABLE);		// Habilita el modulo LCD 	
	LCD_GLASS_Clear();
	
	if(stScaleParam.cLenguage == ENGLISH){
		LCD_GLASS_String("BAD  ", LCD_PRECIO);	
		LCD_GLASS_String("  BATT", LCD_TOTAL);
	}else{
		LCD_GLASS_String("ERROR ", LCD_PRECIO);	
		LCD_GLASS_String("   BAT", LCD_TOTAL);
	}
	
	key_scan();
	
	while(fVoltage_Battery < 1 && key  != KEY_RCL){		
		uGet_Status_Volt();		
		vSound_Saved_Param();			
		key_scan();	
	}
	
	if(key  == KEY_RCL){
		srFlagScale.bShowErroBat = 0;
	}
	
	LCD_GLASS_Clear();*/
}

/*
*/
void vVbatHigh(void){
/*	
	float fVoltage_Aux = 0;

	LCD_Cmd(ENABLE);		 Habilita el modulo LCD 	
	LCD_GLASS_Clear();
	
	if(stScaleParam.cLenguage == ENGLISH){
		LCD_GLASS_String("HIGH  ", LCD_PRECIO);	
		LCD_GLASS_String("  BATT", LCD_TOTAL);
	}else{
		LCD_GLASS_String("ALTA  ", LCD_PRECIO);	
		LCD_GLASS_String("   BAT", LCD_TOTAL);
	}
		
	while(fVoltage_Battery > HIGH_BATTERY_MSG){		
		uGet_Status_Volt();		
		vSound_Saved_Param();
	}
	
	LCD_GLASS_Clear();*/
}
/*
*/
void vVadapHigh(void){
/*	
	float fVoltage_Aux = 0;

	LCD_Cmd(ENABLE);		 Habilita el modulo LCD 	
	LCD_GLASS_Clear();
	
	if(stScaleParam.cLenguage == ENGLISH){
		LCD_GLASS_String("HIGH  ", LCD_PRECIO);	
		LCD_GLASS_String("  ADAP", LCD_TOTAL);
	}else{
		LCD_GLASS_String("ALTA  ", LCD_PRECIO);	
		LCD_GLASS_String(" ELINN", LCD_TOTAL);
	}
		
	while(fVoltage_Conector > HIGH_ADAPTER){		
		uGet_Status_Volt();		
		vSound_Saved_Param();
	}
	
	LCD_GLASS_Clear();
	*/
}


/**
  ******************************************************************************
  * Objetivo: Proteger el circuito de una polaridad inversa de la bateria
  ******************************************************************************
	*/
void vVbatVeryLow(void){
/*
	LCD_Cmd(ENABLE);		Habilita el modulo LCD 	
	OffBackLight;
	LCD_GLASS_Clear();
	
	if(stScaleParam.cLenguage == ENGLISH){
		LCD_GLASS_String("LOW  ", LCD_PRECIO);	
		LCD_GLASS_String("  BATT", LCD_TOTAL);
	}else{
		LCD_GLASS_String("BAJA  ", LCD_PRECIO);	
		LCD_GLASS_String("   BAT", LCD_TOTAL);
	}

	strTimer.cFLag_TimerE_Start = 1;
	strTimer.cFLag_TimerE_End = 0;			
	while(strTimer.cFLag_TimerE_End == 0){
		vReadKey();
		IWDG->KR = IWDG_KEY_REFRESH;
	}
	IWDG->KR = IWDG_KEY_ENABLE;
	while(1);    // Wait until reset occurs from IWDG
*/
}

/*

*/
void vSet_Volts_System(void){
	uGet_Status_Volt();
	stScaleParam.fVoltage_Batt =	fVoltage_Battery;
}