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
 
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "dvr_def.h"
#include "dvr_scale.h"
#include "dvr_lcd.h"
#include "dvr_battery.h"
#include "dvr_keyboard.h"

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

float fVoltage_Conector = 0;
float fVoltage_Battery = 0;
float fVoltage_Ref = 0;

float FactConvAjustado = 0;

enum ActionCargador{ 
	CargadorOff, CargadorOn, CargadorInicio, CargadorTrickle 
};

enum ActionCargador eAccionCargador;

struct Time_Battery{
	long iDescanso_2_Hora;
	long iTimeOut_16;
	long iTrickleCharge;
};
struct Time_Battery stTime_Battery;

//extern enum ActionScale eAccionScale;

void vGet_Volts_System(void);
void vGet_Volts_Ref(void);
unsigned char uGet_Status_Volt(void);
void vDriver_Backlight_Source(unsigned char cEliminadorOn);

void vVbatBad(void);
void vVbatHigh(void);
void vVadapHigh(void);
int vGet_ADCx_Value(uint16_t ADC_Channel, int iSampleNumber);
int vGet_ADCx_Value_Ref(uint16_t ADC_Channel, int iSampleNumber);

#if DISPLAY_20400047_EN > 0

const float delta=0.05; 
const float lvl_5 = 5.14;
const float lvl_3 = 4.0;
const float lvl_2 = 3.65;
const float lvl_1 = 3.52;


uint8_t estado=0;
uint8_t var=0;

void driver_symbol(void);
void driver_symbol_init(void);

#endif

/*

*/
void vGestorBateria(void){
	
	unsigned char bStatus_Volt;

	if(srFlagScale.bReadBattery > 0){
		
		srFlagScale.bReadBattery = 0;
		
		if(stScaleParam.iDebug != 24275){ //Debug OFF{
			uOFF_Carga;
		}
		
		bStatus_Volt = uGet_Status_Volt();
	
//		vDriver_Backlight_Source(bStatus_Volt);
		
		switch(bStatus_Volt){
			
			
			
			case SOURCE_ADAPTER:
				if(srFlagScale.bBateriaLow == 1){
					LCD_BlinkConfig(LCD_BlinkMode_Off,LCD_BlinkFrequency_Div128);
					srFlagScale.bMsgBatteryLow = 0;
					srFlagScale.bBateriaLow = 0;
					srFlagScale.bBateriaCount = 0;
				}
				if(eAccionScale == ScaleRun || eAccionScale ==ScaleWait){					
					if(stScaleParam.cBacklight){OnBackLight;}
				}	
				srFlagScale.bSourceVoltage =  SOURCE_ADAPTER;
				Auto_Off_On_15m;
				Restart_Sleep_Time;				
				if(stScaleParam.iDebug != 24275){ //Debug OFF
					uON_Carga;
				}
				
				break;
				
			case SOURCE_BATTERY_OK:
				if(srFlagScale.bBateriaLow == 1){
					LCD_BlinkConfig(LCD_BlinkMode_Off,LCD_BlinkFrequency_Div128);
					srFlagScale.bMsgBatteryLow = 0;
					srFlagScale.bBateriaLow = 0;
					srFlagScale.bBateriaCount = 0;
				}
				srFlagScale.bSourceVoltage =  SOURCE_BATTERY;
				break;
			
			case SOURCE_BATTERY_HIGH:	
				if(srFlagScale.bBateriaLow == 1){
					LCD_BlinkConfig(LCD_BlinkMode_Off,LCD_BlinkFrequency_Div128);
					srFlagScale.bMsgBatteryLow = 0;
					srFlagScale.bBateriaLow = 0;
					srFlagScale.bBateriaCount = 0;
				}
				
				vVbatHigh();
				break;
			
			case SOURCE_BATTERY_LOW:
			
				srFlagScale.bSourceVoltage =  SOURCE_BATTERY;
				srFlagScale.bBateriaLow = 1;
				OffBackLight;
			/* //CCC 
				if(srFlagScale.bBateriaCount > 8){
					srFlagScale.bBateriaCount = 0;
					srFlagScale.bMsgBatteryLow = 0;
					LCD_BlinkConfig(LCD_BlinkMode_Off,LCD_BlinkFrequency_Div128);
				}
				else if(srFlagScale.bBateriaCount >= 7 && srFlagScale.bBateriaCount <= 9 ){				
					LCD_BlinkConfig(LCD_BlinkMode_AllSEG_AllCOM,LCD_BlinkFrequency_Div128);				
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
				}*/else{
					srFlagScale.bMsgBatteryLow = 0;
				}
				break;
				
			case SOURCE_BATTERY_VERY_LOW:
				
				OffBackLight;
				LCD_GLASS_Clear();
				LCD_BlinkConfig(LCD_BlinkMode_Off,LCD_BlinkFrequency_Div128);
				
			/*	if(stScaleParam.cLenguage == ENGLISH){
					LCD_GLASS_String("LOW  ", LCD_PRECIO);	
					LCD_GLASS_String("  BATT", LCD_TOTAL);
				}else{
					LCD_GLASS_String("BAJA  ", LCD_PRECIO);	
					LCD_GLASS_String("   BAT", LCD_TOTAL);
				}*/

				if(stScaleParam.cLenguage == ENGLISH){
					LCD_GLASS_String("LOW ", LCD_PESO);	
					LCD_GLASS_String("BATT", LCD_PRECIO);
					LCD_GLASS_String(" OFF", LCD_TOTAL);
				}else{
					LCD_GLASS_String("BAJA ", LCD_PESO);	
					LCD_GLASS_String("BAT", LCD_PRECIO);
					LCD_GLASS_String(" OFF", LCD_TOTAL);
				}

				strTimer.cFLag_TimerE_Start = 1;
				strTimer.cFLag_TimerE_End = 0;	
				
				while(strTimer.cFLag_TimerE_End == 0){
					vReadKey();
					IWDG->KR = IWDG_KEY_REFRESH;
				}
				IWDG->KR = IWDG_KEY_ENABLE;
				while(1);    // Wait until reset occurs from IWDG
				
				break;
				
			case SOURCE_ADAPTER_HIGH:
			
				if(srFlagScale.bBateriaLow == 1){
					LCD_BlinkConfig(LCD_BlinkMode_Off,LCD_BlinkFrequency_Div128);
					srFlagScale.bMsgBatteryLow = 0;
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

if(eAccionScale == scalePreOnDc || stScaleParam.cSpecialAction == 1)
	return;
else if(srFlagScale.bOverLoad == 1)
		{
			LCD_GLASS_Symbols(9,0); //bateria 0/3
			LCD_GLASS_Symbols(8,0);
			LCD_GLASS_Symbols(7,0);
			LCD_GLASS_Symbols(10,0);
			return;
		} 
	
if(eAccionScale==scalePreOnDc || srFlagScale.bOverLoad ==1)
	return;

if(srFlagScale.bSourceVoltage == SOURCE_ADAPTER)
{
	
	if(strTimer.cFLag_TimerCharge_End==1)
	{
			estado++;
		if(estado==4)
			estado=0;
	}
	
	if(strTimer.cFLag_TimerCharge_On==0)
		strTimer.cFLag_TimerCharge_Start=1;

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
	
	vGet_Volts_Ref();
	
	//return SOURCE_ADAPTER;
	
	///
	
	vGet_Volts_System();
	
/*	if(fVoltage_Ref >= 0.475){
		fVoltage_Conector = (-5.5483)*fVoltage_Ref + 5.9148;
	}*/
		
	if(fVoltage_Conector > HIGH_ADAPTER){
		return SOURCE_ADAPTER_HIGH;
	}
		
	if(fVoltage_Battery < THRESHOLD_VOLTAGE){//if(fVoltage_Conector < LOW_VOLTAGE_ADAPTER){
		
/*		if (fVoltage_Conector < NO_ON){
			return SOURCE_NO_ON;
		}else if(fVoltage_Conector < LOW_BATTERY_OFF){// || srFlagScale.bStateBatDown){
			return SOURCE_BATTERY_VERY_LOW;
		}else if(fVoltage_Conector < LOW_BATTERY_MSG){
			return SOURCE_BATTERY_LOW;
		}else{
			return SOURCE_BATTERY_OK;
		} 
*/		
	if (fVoltage_Battery < NO_ON){
			return SOURCE_NO_ON;
		}else if(fVoltage_Battery < LOW_BATTERY_OFF){// || srFlagScale.bStateBatDown){
			return SOURCE_BATTERY_VERY_LOW;
		}else if(fVoltage_Battery < LOW_BATTERY_MSG){
			return SOURCE_BATTERY_LOW;
		}else{
			return SOURCE_BATTERY_OK;
		}	
		
	}else{
		return SOURCE_ADAPTER;
	}
}

/**


**/
void vGet_Volts_System(void){
	
	int iValue_Adc = 0;
	
	// Lectura del Voltaje del eliminador
	ADC_SamplingTimeConfig(ADC1, ADC_Group_FastChannels, ADC_SamplingTime_96Cycles);
	
	iValue_Adc = vGet_ADCx_Value(ADC_VIN, 100);	//Obtiene voltaje eliminador
	fVoltage_Conector = (float)(iValue_Adc)*(0.000805664)*6.11+1;
	fVoltage_Battery = (float)(iValue_Adc)*(0.000805664)*6.11+0.37;

if(fVoltage_Ref > REFERENCE)
	{
		fVoltage_Battery = (float)(iValue_Adc)*FactConvAjustado*6.11+0.37;
	} 
		
}

/*

*/
void vGet_Volts_Ref(void){

	int iValue_Adc;	

	srFlagScale.bStateBatDown = 0;

	ADC_SamplingTimeConfig(ADC1, ADC_Group_FastChannels, ADC_SamplingTime_96Cycles);
	iValue_Adc = vGet_ADCx_Value_Ref(ADC_VREF, 10);
	fVoltage_Ref = 0.000805664*(float)(iValue_Adc);

	if(fVoltage_Ref > REFERENCE)
	{
		FactConvAjustado = (CuentasAdcRef*0.000805664)/(float)(iValue_Adc);
	}
/*	if(fVoltage_Ref > REFERENCE){
		srFlagScale.bStateBatDown = 1;
	}
	else{
		srFlagScale.bStateBatDown = 0;
	}*/
}

/**

**/

void vDriver_Backlight_Source(unsigned char cEliminadorOn){
	
	if(cEliminadorOn != SOURCE_ADAPTER && srFlagScale.bBateriaLow == 0)
	{
		if(eAccionScale == ScaleRun || eAccionScale == ScaleWait){
		 if(stScaleParam.cBacklight){OnBackLight;}
		}
	}


/*	if(cEliminadorOn != 0){
		
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
	
	int iValue_Adc = 0;
	float fVoltage_Aux = 0;
	//enum digi_key Value_Key_Press;
	
	LCD_Cmd(ENABLE);		/* Habilita el modulo LCD */	
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
	
	LCD_GLASS_Clear();
}

/*
*/
void vVbatHigh(void){
	
	float fVoltage_Aux = 0;

	LCD_Cmd(ENABLE);		/* Habilita el modulo LCD */	
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
	
	LCD_GLASS_Clear();
}
/*
*/
void vVadapHigh(void){
	
	float fVoltage_Aux = 0;

	LCD_Cmd(ENABLE);		/* Habilita el modulo LCD */	
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
	
}


/**
  ******************************************************************************
  * Objetivo: Proteger el circuito de una polaridad inversa de la bateria
  ******************************************************************************
	*/
void vVbatVeryLow(void){

	LCD_Cmd(ENABLE);		/* Habilita el modulo LCD */	
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
}

/**
  ******************************************************************************
  * Objective: To obtain the value of the channel ADC.
	******************************************************************************
	*/
int vGet_ADCx_Value(uint16_t ADC_Channel, int iSampleNumber){

	long iValue_ADC = 0;
	int i = 0;
	
	CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	ADC_ChannelCmd(ADC1, ADC_Channel, ENABLE);
		
	for(i=0; i<iSampleNumber; i++){		
		ADC_SoftwareStartConv(ADC1);
		
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)){}
	
		iValue_ADC += (long)(ADC_GetConversionValue(ADC1));
	}
	
	iValue_ADC = iValue_ADC/iSampleNumber;
					
	ADC_ChannelCmd(ADC1, ADC_Channel, DISABLE);
	
	ADC_Cmd(ADC1, DISABLE);
	CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
	
	return (int)(iValue_ADC);
}


/**
  ******************************************************************************
  * Objective: To obtain the value of the Reference channel ADC.
	******************************************************************************
	*/
int vGet_ADCx_Value_Ref(uint16_t ADC_Channel, int iSampleNumber){

	long iValue_ADC = 0;
	int i = 0;
	
	CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);
	ADC_VrefintCmd(ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	ADC_ChannelCmd(ADC1, ADC_Channel, ENABLE);
	
	for(i=0; i<iSampleNumber; i++){		
		ADC_SoftwareStartConv(ADC1);
		
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)){}
	
		iValue_ADC += (long)(ADC_GetConversionValue(ADC1));
	}
	
	iValue_ADC = iValue_ADC/iSampleNumber;
					
	ADC_ChannelCmd(ADC1, ADC_Channel, DISABLE);
	
	ADC_Cmd(ADC1, DISABLE);
	
	ADC_VrefintCmd(DISABLE);
	
	CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
	
	return (int)(iValue_ADC);
}

/*

*/
void vSet_Volts_System(void){
	uGet_Status_Volt();
	stScaleParam.fVoltage_Batt =	fVoltage_Battery;
	stScaleParam.fVoltage_Adap =	fVoltage_Conector;	
}

/*

*/
void vShowDataBatLcd(void){
//	float fV_Diode = 0.3;
	float fV_Conn = 0;
	
	char cValue_Adc_Ref[5] = {0,0,0,0,0};
	int i;
	
//fV_Conn = fVoltage_Conector + fV_Diode;
	LCD_GLASS_Float(fVoltage_Conector, 3, LCD_PESO);
	LCD_GLASS_Float(fVoltage_Battery, 2, LCD_PRECIO);
	LCD_GLASS_Float(fVoltage_Ref, 2, LCD_TOTAL);
	
/*	sprintf(cValue_Adc_Ref, "%d", iValue_Adc_Ref);
	
					for(i=0; i<5; i++){
					if(cValue_Adc_Ref[i] == 0){
						cValue_Adc_Ref[i] = ' ';
						
					}
				}
				
	LCD_GLASS_String(cValue_Adc_Ref, LCD_TOTAL);*/
	
	#if DISPLAY_20400047_EN > 0
		driver_symbol();
	#endif
}
	

/*

*/
void vShowDataCargador(void){
	switch(eAccionCargador){
		
		case CargadorOff:		
			LCD_GLASS_String("  OFF", LCD_PESO);		
			LCD_GLASS_String("     ", LCD_PRECIO);
			LCD_GLASS_String("      ", LCD_TOTAL);			
			break;
		
		case CargadorInicio:			
			LCD_GLASS_String("  INI", LCD_PESO);
			LCD_GLASS_String("     ", LCD_PRECIO);
			LCD_GLASS_String("      ", LCD_TOTAL);
			break;
		
		case CargadorOn:
			LCD_GLASS_String("   ON", LCD_PESO);
			LCD_GLASS_String("     ", LCD_PRECIO);
			LCD_GLASS_Float(stTime_Battery.iTimeOut_16, 0, LCD_TOTAL);
			break;
		
		case CargadorTrickle:
			LCD_GLASS_String("   TC", LCD_PESO);
			LCD_GLASS_Float(stTime_Battery.iTrickleCharge, 0, LCD_PRECIO);			
			LCD_GLASS_Float(stTime_Battery.iDescanso_2_Hora, 0, LCD_TOTAL);
			break;		
	}
}



