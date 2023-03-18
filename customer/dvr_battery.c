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
//#include "./customer/usr_dbg.h"
#include "./_scale/dvr_def.h"
#include "./_scale/dvr_scale.h"
#include "./_display/dvr_lcd_SDI1621.h"
#include "./customer/dvr_battery.h"
#include "./customer/keyboard.h"
#include "./_solidic/head_file_version.h"
#include "./customer/beeper.h"

sbit BAT_DET_PIN = P0^5;



#define HIGH_SPEED_CLOCK 	1
#define LOW_SPEED_CLOCK		0

#define COUNT_1_HORA  3600
#define COUNT_1_MIN		60

#define HIGH_ADAPTER        7.0  //6.1
#define LOW_BATTERY_MSG 		3.5
#define LOW_BATTERY_OFF 		3.2
#define THRESHOLD_VOLTAGE 	4.5	 //#define LOW_VOLTAGE_ADAPTER 4.35
#define REFERENCE			1.22 //0.48
#define HIGH_BATTERY_MSG 	6.0
#define NO_ON				3.0  //2.5

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

void vVadapHigh(void);

#if DISPLAY_20400047_EN > 0

const float delta= 0.0;//0.05; 
const float lvl_5 = 4.7;
const float lvl_3 = 4.2;
const float lvl_2 = 4.0;
const float lvl_1 = 3.5;


unsigned char estado=0;
unsigned char var=0;

void driver_symbol(void);
//void driver_symbol_init(void);

#endif

/*

*/
void vGestorBateria(void){	  	

	if(srFlagScale.bReadBattery!=0){  	
		srFlagScale.bReadBattery = 0;   
		stScaleParam.cSourceVoltage = uGet_Status_Volt();	
		vDriver_Backlight_Source(stScaleParam.cSourceVoltage);
		switch(stScaleParam.cSourceVoltage){
				
			case SOURCE_ADAPTER_HIGH:			
				vVadapHigh();			
				break;
			case SOURCE_ADAPTER_OK:
				break;
			case SOURCE_BATTERY_HIGH:	
				break;
			case SOURCE_BATTERY_OK:
				break;
			case SOURCE_BATTERY_LOW:
				if(srFlagScale.bBateriaCount > 8){
					srFlagScale.bBateriaCount = 0;
				}
				else if(srFlagScale.bBateriaCount >= 7 && srFlagScale.bBateriaCount <= 9 ){				
//					srFlagScale.bMsgBatteryLow = 1;
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
				//srFlagScale.bMsgBatteryLow = 0;
				}
				break;
			case SOURCE_BATTERY_VERY_LOW:
				vVbatVeryLow();
				break;
		}
	#if DISPLAY_20400047_EN > 0
	driver_symbol();
	#endif
	}
}

#if DISPLAY_20400047_EN > 0

/*void driver_symbol_init(){
		
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
*/
/*
*/

void driver_symbol(void){

/*if(eAccionScale == ScalePreOn || srFlagScale.bOverLoad == 1)
{
		LCD_GLASS_Symbols(9,0); //bateria 0/3
		LCD_GLASS_Symbols(8,0);
		LCD_GLASS_Symbols(7,0);
		LCD_GLASS_Symbols(10,0);
		return;
} */

if(stScaleParam.cSourceVoltage >= SOURCE_ADAPTER_LOW)
{
	if(strTimer.iTimerCharge==0)
		strTimer.iTimerCharge=1;

	if(strTimer.iTimerCharge>=TimerChargeend)
	{
		strTimer.iTimerCharge=0;
		estado++;
		if(estado==4)
			estado=0;
	}
	
	if(fVoltage_Battery>=lvl_5)
		estado=5;
// Condicion de carga completa para TM con dos puntos de lectura de voltaje.
	if(fVoltage_Battery<=5.22 && fVoltage_Battery>=4.72)
		estado=5;
		
}else{
	if(fVoltage_Battery < (lvl_3) && estado == 3)	
		estado = 2;
	else if(fVoltage_Battery >= (lvl_3) && estado == 2)
		estado = 3;
	else if(fVoltage_Battery < (lvl_2) && estado == 2)
		estado = 1;
	else if(fVoltage_Battery >= (lvl_2) && estado == 1)
		estado = 2;
	else if(fVoltage_Battery < (lvl_1) && estado == 1)
		estado = 0;	
	else if(fVoltage_Battery >= (lvl_1) && estado == 0)
		estado = 1;
/*	if(fVoltage_Battery < (lvl_3 - delta) && estado == 3)	
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
		*/
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

	//LCD_GLASS_Float((float)source,2,LCD_PRECIO);
	return source;
}

void vGet_Volts_System(void){
	unsigned char iValue_Adc = 0;

	IWDG_KEY_REFRESH; 
	EA = 0;
	BAT_DET_PIN = 1;
	SARCON  = 0x09;
	delay_ms(1);
	if(!(SARCON & 0x04)) // 
	{
		SARCON |= 0x04;
		while(SARCON & 0x04) 
		{
		}
	}
	iValue_Adc = (unsigned char)SARDATA;
	SARCON  &= 0xf7;
	EA = 1;
	if(iValue_Adc>47){
		fVoltage_Battery = (((3.3/256.0)* (float)iValue_Adc))*10.0;
	}
	else{
			switch (iValue_Adc)
			{
			case 33:
				fVoltage_Battery = 3.66;
				break;
			case 34:
				fVoltage_Battery = 3.87;
				break;
			case 35:
				fVoltage_Battery = 4.0;
				break;
			case 36:
				fVoltage_Battery = 4.2;
				break;
			case 37:
				fVoltage_Battery = 4.4;
				break;
			case 38:
				fVoltage_Battery = 4.6;
				break;
			case 39:
				fVoltage_Battery = 4.8;
				break;
			case 40:
				fVoltage_Battery = 5.0;
				break;
			case 41:
				fVoltage_Battery = 5.2;
				break;
			case 42:
				fVoltage_Battery = 5.3;
				break;
			case 43:
				fVoltage_Battery = 5.5;
				break;
			case 44:
				fVoltage_Battery = 5.8;
				break;
			case 45:
				fVoltage_Battery = 6.0;
				break;
			case 46:
				fVoltage_Battery = 6.2;
				break;
			case 47:
				fVoltage_Battery = 6.5;
				break;
			}
	}
	// fVoltage_Battery = (((3.3/256.0)* (float)iValue_Adc))*6.0 +1.5;
	 //LCD_GLASS_Float(iValue_Adc,2,LCD_TOTAL);
	


}

void vDriver_Backlight_Source(unsigned char cEliminadorOn){
	if(eAccionScale==ScaleRun || eAccionScale==ScaleWait || eAccionScale==ScaleBattery){
		if(cEliminadorOn>=SOURCE_ADAPTER_LOW){   
			OnBackLight;	
		}
		else
		if(srFlagScale.bBacklight_On == 1){
			if(strTimer.iTimerBlk>=iTimerBlk1end){
				DecreaseBackLight;
			}
		}
		if(strTimer.iTimerBlk>=iTimerBlk2end){
					OffBackLight;
		}
	}
}

void vVadapHigh(void){
	LCD_GLASS_Clear();
	
	if(stScaleParam.cLenguage == ENGLISH){
		LCD_GLASS_String("HIGH  ", LCD_PRECIO);	
		LCD_GLASS_String("  ADAP", LCD_TOTAL);
	}else{
		LCD_GLASS_String("ALTA  ", LCD_PRECIO);	
		LCD_GLASS_String(" ELINN", LCD_TOTAL);
	}
		
	while(uGet_Status_Volt() == SOURCE_ADAPTER_HIGH){		
		vSound_Saved_Param();
		IWDG_KEY_REFRESH;
	}
	
	LCD_GLASS_Clear();

}


/**
  ******************************************************************************
  * Objetivo: Proteger el circuito de una polaridad inversa de la bateria
  ******************************************************************************
	*/
void vVbatVeryLow(void){
	LCD_GLASS_Clear();
	
	if(stScaleParam.cLenguage == ENGLISH){
		LCD_GLASS_String("LOW  ", LCD_PRECIO);	
		LCD_GLASS_String("  BATT", LCD_TOTAL);
	}else{
		LCD_GLASS_String("BAJA  ", LCD_PRECIO);	
		LCD_GLASS_String("   BAT", LCD_TOTAL);
	}
vSound_Saved_Param();


while(uGet_Status_Volt() == SOURCE_BATTERY_VERY_LOW){		
		IWDG_KEY_REFRESH;
	}
	

}

/*

*/
void vSet_Volts_System(void){
	uGet_Status_Volt();
	stScaleParam.fVoltage_Batt =	fVoltage_Battery;
}