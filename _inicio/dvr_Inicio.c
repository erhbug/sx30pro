#include <stdio.h>
#include <math.h>
#include <string.h>
#include "stm8l15x.h"
#include "stm8l15x_iwdg.h"
#include "stm8l15x_adc.h"
#include "stm8l15x_it.h"
#include "main.h"
#include "dvr_lcd.h"
#include "dvr_HX712.h"
#include "dvr_def.h"
#include "dvr_keyboard.h"
#include "dvr_scale.h"
#include "dvr_plu.h"
#include "dvr_battery.h"
#include "dvr_e2prom.h"
#include "dvr_registradora.h"

/* --- Registro de Password */

const enum digi_key eSEQUENCE_CONFIGURATION[8] = {
	KEY_TARA, KEY_C, KEY_CERO, KEY_RCL};
const enum digi_key ePRE_CONFIGURATION_15KG[8] = {
	KEY_TARA, KEY_M1, KEY_TARA, KEY_1, KEY_5};
const enum digi_key ePRE_CONFIGURATION_20KG[8] = {
	KEY_TARA, KEY_M1, KEY_TARA, KEY_2, KEY_0};
const enum digi_key ePRE_CONFIGURATION_30KG[8] = {
	KEY_TARA, KEY_M1, KEY_TARA, KEY_3, KEY_0};
const enum digi_key ePRE_CONFIGURATION_31KG[8] = {
	KEY_TARA, KEY_M1, KEY_TARA, KEY_3, KEY_1};	
const enum digi_key ePRE_CONFIGURATION_32KG[8] = {
	KEY_TARA, KEY_M1, KEY_TARA, KEY_3, KEY_2};	
const enum digi_key ePRE_CONFIGURATION_40KG[8] = {
	KEY_TARA, KEY_M1, KEY_TARA, KEY_4, KEY_0};

const enum digi_key eLCD_ALL_SEGMENT[8] = {
	KEY_TARA, KEY_8};
const enum digi_key eDATA_SCALE[8] = {
	KEY_TARA, KEY_C, KEY_RCL, KEY_MEM}; 
const enum digi_key eOFFSET_ADC[8] = {
	KEY_TARA, KEY_MEM, KEY_1, KEY_5};
const enum digi_key eSOBRE_PESO_OFF[8] = {
	KEY_TARA, KEY_MEM, KEY_1, KEY_TARA};
const enum digi_key eERASE_PLUS[8] = {
	KEY_TARA, KEY_TARA, KEY_2, KEY_9};
const enum digi_key eRESET_COUNTER[8] = {
	KEY_TARA, KEY_RCL, KEY_C, KEY_CERO};
const enum digi_key eVIEW_COUNTER_CALIBRATION[8] = {
	KEY_TARA, KEY_MEM, KEY_C, KEY_1, KEY_0, KEY_0, KEY_0};
const enum digi_key eVIEW_COUNTER_CONFIGURATION[8] = {
	KEY_TARA, KEY_MEM, KEY_C, KEY_1, KEY_0, KEY_0, KEY_1};
const enum digi_key eVIEW_VOLTAGE_BATTERY[8] = {
	KEY_TARA, KEY_MEM, KEY_MEM, KEY_C, KEY_C}; 
const enum digi_key eFUNC_ESPECIAL[8] = {
	KEY_C, KEY_MEM, KEY_RCL, KEY_MEM};
	
const enum digi_key eVIEW_MEMORY[8] = {
	KEY_TARA, KEY_MEM, KEY_C, KEY_C};
	
const enum digi_key eSEQUENCE_RESET_REFERENCE[8] = {
	KEY_TARA, KEY_TARA, KEY_0};
	
const enum digi_key eTEST_TECLADO[8] = {	
	KEY_TARA, KEY_MEM, KEY_MEM, KEY_RCL, KEY_RCL};

const enum digi_key eTEST_CALIDAD[8] = {	
	KEY_TARA, KEY_MEM, KEY_MEM, KEY_RCL, KEY_MEM};

const enum digi_key eACT_PROGRAM[8] = {
	KEY_TARA, KEY_2, KEY_4, KEY_2, KEY_7};	

const enum digi_key eINF_CAL[8] = {
	KEY_TARA, KEY_MEM, KEY_0};

const enum digi_key eTEMPERATURE[8] = {
	KEY_TARA, KEY_CERO, KEY_CERO, KEY_RCL};
	
const enum digi_key eVER_OVERLOAD[8] = {
	KEY_TARA, KEY_RCL, KEY_CERO, KEY_1};

const enum digi_key ePASS_MULTIRANGO_OFF[8] = {
	KEY_TARA, KEY_5, KEY_4, KEY_3, KEY_2, KEY_1, KEY_0};
	
const enum digi_key eBACKLIGHT[8] = {
	KEY_TARA, KEY_1, KEY_0, KEY_1, KEY_0};	


const unsigned char eCODE_CALIBRACION[7] 	= "765432";
const unsigned char eCODE_MENU[7] 				= "779103";

extern const unsigned char ADDR_POINT_ZERO;	/* 4 byte */
extern const unsigned char ADDR_POINT_ZERO_UNL;

extern float fStablePoint(u8 cSetCountBack, u8 cShowCount, u8 cRunStable);

unsigned char cFuncion_Especial = 0;

unsigned char vKey_Code(enum digi_key arCode[7]);
unsigned char cWrite_Code_Scale(void);
void vSpecial_Action(unsigned char cAction);
float fGet_Temp_Amb_Micro(void);
void vCalidadTest(void);

void vTestTeclado(void);

/**
  ******************************************************************************
  * Objetivo: Capturar el codigo de configuracion mientras se estabiliza el peso
			de la bascula 
  * Parametros entrada:  
	* Parametros Salida:
		0 -> No se digito ningun codigo se inicia el programa 
		1 -> Se digito una secuencia valida se pasa a solicitar el pass
  * Prerequisitos: 
  ***
	*/
unsigned char cWait_Scale(void){
	
	enum digi_key arPass_Configuration[8] = {0,0,0,0,0,0,0,0};
	enum digi_key Value_Key_Press;
	
	int i=0;
	
	strTimer.cFLag_TimerD_End = 0;
	strTimer.cFLag_TimerD_Start = 1;	/* Inicia un timer para esperar 2 seg
											a qe se digite alguna tecla para introducir un codigo */
											
	for(i=0;i<4;i++){
		fRead_Adc(1);
	}
	
	while(1){
		
		IWDG->KR = IWDG_KEY_REFRESH;
		Value_Key_Press = vActionKey();		/* Verifica si se oprimio alguna tecla y
																		almacena este valor */

		/* La tecla pulsada se almacena en el vector de pass de configuracion */
		if(Value_Key_Press != KEY_NULL){
			
			vBeep_Key();
			strTimer.cFLag_TimerD_Start = 1;		/* Reinicia el contador para permitir
																		/* escribir todo el codigo */
			/* Solo permite 7 digitos para el codigo */
			if(strlen(arPass_Configuration) < 7){
				/* Agrega la tecla pulsada a la cadena */
				strncat(arPass_Configuration, (unsigned char *)&Value_Key_Press, 1);
			}else{
				LCD_GLASS_Clear();
				LCD_GLASS_String("CODE", LCD_PESO);
				LCD_GLASS_String("BAD", LCD_PRECIO);
				
				Auto_Off_On_5m_Restart;
				Auto_Off_On_5m;
				
				/*while(!srFlagScale.cWait_Time_Code){
					vActionKey();
					IWDG->KR = IWDG_KEY_REFRESH;
					if(srFlagScale.bScaleOn == FALSE){
						return 0;
					}
				}*/
			}
			
			if(strcmp(eSEQUENCE_CONFIGURATION, arPass_Configuration) == 0){
				srFlagScale.bCodeConfiguration = 1;
				return 1;
			}else if(strcmp(ePRE_CONFIGURATION_15KG, arPass_Configuration) == 0){
				vPreConfiguration(PreConfig15KG);
			}else if(strcmp(ePRE_CONFIGURATION_20KG, arPass_Configuration) == 0){
				vPreConfiguration(PreConfig20KG);
			}else if(strcmp(ePRE_CONFIGURATION_30KG, arPass_Configuration) == 0){
				vPreConfiguration(PreConfig30KG);
			}else if(strcmp(ePRE_CONFIGURATION_31KG, arPass_Configuration) == 0){
				vPreConfiguration(PreConfig31KG);				
			}else if(strcmp(ePRE_CONFIGURATION_32KG, arPass_Configuration) == 0){
				vPreConfiguration(PreConfig32KG);				
			}else if(strcmp(ePRE_CONFIGURATION_40KG, arPass_Configuration) == 0){
				vPreConfiguration(PreConfig40KG);
			//}else if(strcmp(ePRE_CONFIGURATION_60LB, arPass_Configuration) == 0){
			//	vPreConfiguration(PreConfig60LB);				
			}else if(strcmp(eDATA_SCALE, arPass_Configuration) == 0){
				vSpecial_Action(Data_Scale);
			}else if(strcmp(eOFFSET_ADC, arPass_Configuration) == 0){
				vSpecial_Action(Offset_Adc);
			}else if(strcmp(eERASE_PLUS, arPass_Configuration) == 0){
				vSpecial_Action(Erase_Plus);
				strTimer.cFLag_TimerD_End = 1;
			}else if(strcmp(eRESET_COUNTER, arPass_Configuration) == 0){
				vSpecial_Action(Reset_Counter);
				strTimer.cFLag_TimerD_End = 1;
			}else if(strcmp(eVIEW_COUNTER_CALIBRATION, arPass_Configuration) == 0){
				vSpecial_Action(View_Counter_Calibration);
			}else if(strcmp(eVIEW_COUNTER_CONFIGURATION, arPass_Configuration) == 0){
				vSpecial_Action(View_Counter_Configuration);
			}else if(strcmp(eVIEW_VOLTAGE_BATTERY, arPass_Configuration) == 0){
				vSpecial_Action(View_Voltage_Battery);
			}else if(strcmp(eVIEW_MEMORY, arPass_Configuration) == 0){
				vSpecial_Action(View_Memory);
			}else if(strcmp(eACT_PROGRAM, arPass_Configuration) == 0){
				vSpecial_Action(Activar_Debug);
			}else if(strcmp(eINF_CAL, arPass_Configuration) == 0){
				vSpecial_Action(Info_Calibracion);
			}else if(strcmp(eTEMPERATURE, arPass_Configuration) == 0){
				vSpecial_Action(Function_Temperature);
			}else if(strcmp(eVER_OVERLOAD, arPass_Configuration) == 0){
				vSpecial_Action(Function_VerOverload);
			}else if(strcmp(eTEST_TECLADO, arPass_Configuration) == 0){				
				vSpecial_Action(Function_TestTeclado);	
			}else if(strcmp(eTEST_CALIDAD, arPass_Configuration) == 0){				
				vSpecial_Action(Function_CalidadTest);	
			}else if(strcmp(eBACKLIGHT, arPass_Configuration) == 0){				
				vSpecial_Action(Function_Backlight);					
			}else if(strcmp(eSEQUENCE_RESET_REFERENCE, arPass_Configuration) == 0){
						
				stScaleParam.fPointZeroCali = fStablePoint(1, 0, 0);
				vEepromInit(ENABLE);					/* Habilita la escritura/lectura en la EEPROM */
				vSaveFloatEeprom(ADDR_POINT_ZERO, stScaleParam.fPointZeroCali);				
				vEepromInit(DISABLE);					/* Deshabilita la escritura en la EEPROM */
				
				LCD_GLASS_String("RESET", LCD_PESO);
				LCD_GLASS_String("  ADC", LCD_PRECIO);
				LCD_GLASS_String("      ", LCD_TOTAL);
				
			}else if(strcmp(ePASS_MULTIRANGO_OFF, arPass_Configuration) == 0){
				
				stScaleParam.cMultirango = 0;				
				LCD_GLASS_String("NNULR", LCD_PESO);
				LCD_GLASS_String("  OFF", LCD_PRECIO);
				LCD_GLASS_String("      ", LCD_TOTAL);
				vSound_Saved_Param();
			}
		}else{
			
			if(stScaleParam.iDebug != 24275){
				if((GPIOA->IDR & PUSH_SWITCH) == 0){
					while(!strTimer.cFLag_TimerD_End){
						IWDG->KR = IWDG_KEY_REFRESH;
						vActionKey();
					}
					srFlagScale.bCodeConfiguration = 1;
					return 1;
				}
			}
		}
		
		if(srFlagScale.bScaleOn == FALSE){
			return 0;
		
		}else if(strTimer.cFLag_TimerD_End){

			if(strcmp(eLCD_ALL_SEGMENT, arPass_Configuration) == 0){
				
				vSpecial_Action(All_Segment);
				
			}else if(strcmp(eSOBRE_PESO_OFF,arPass_Configuration) == 0){
				srFlagScale.bTopeSobrePeso = 1;
				vSound_Saved_Param();
			}

			strTimer.cFLag_TimerD_End = 0;

			/* Se espera hasta encontrar un punto estable ademas de verificar las 
				condiciones iniciales del peso */
			stScaleParam.fWeightOverload = (stScaleParam.fCapacityCali);
			stScaleParam.fWeightOverload += (stScaleParam.fFactorCalibrate * 9);

			if(cFuncion_Especial){
				stScaleParam.fPointZero = stScaleParam.fPointZeroCali;				
			}else{
				cSetZeroPoint();
			}
			
			if(stScaleParam.cTimeAutooff == 1){
				Auto_Off_On_15m;
			}
			
			Restart_Sleep_Time;
						
			stScaleParam.fPointZeroInitial = stScaleParam.fPointZero;
			
			return 0;
		}
	}
	return 0;
}

/**
  ******************************************************************************
  * Objetivo: Capturar el pass para entrar a calibrar o al menu de configuracion
  * Parametros entrada:  
	* Parametros Salida:
		0 -> Se digito el codigo para ingresar al menu de configuracion 
		1 -> Se digito el codigo para ingresar a calibracion
		2 -> No se digito ningun codigo, retorno por timeout
  * Prerequisitos: 
  ***
	*/
unsigned char cWrite_Code_Scale(void){

	unsigned char arCode_Acces[7] = "      ";
	unsigned char cCaracter;
	unsigned char i=0;
	enum digi_key Value_Key_Press;
	
	LCD_GLASS_Clear();
	LCD_GLASS_String("CODE ", LCD_PESO);

	Auto_Off_On_5m;
	
	while(!srFlagScale.cWait_Time_Code && srFlagScale.bScaleOn == TRUE){
		IWDG->KR = IWDG_KEY_REFRESH;
		Value_Key_Press = vActionKey();		/* Lee el teclado */
	
		if(Value_Key_Press != KEY_NULL){
			vBeep_Key();
			
			cCaracter = cConvert_Char(Value_Key_Press);
			
			if(cCaracter >= '0' && cCaracter <= '9'){
				for(i=0; i<5; i++){
					arCode_Acces[i] = arCode_Acces[i+1];
				}
				
				arCode_Acces[5] = cCaracter;
				LCD_GLASS_String(arCode_Acces, LCD_TOTAL);
			}
		}
		
		/* Si el pass es correcto se verifica a que accion corresponde */
		if(strcmp(eCODE_CALIBRACION, arCode_Acces) == 0){
			if(Value_Key_Press == KEY_C){
				return 1;
			}
		}else if(strcmp(eCODE_MENU, arCode_Acces) == 0){
			if(Value_Key_Press == KEY_MEM){
				return 0;
			}
		}
	}
	return 2;
}

/**
  ******************************************************************************
  * Objetivo: Ejecutar funciones especiales de inicip
  * Parametros entrada:  
	* Parametros Salida:
  * Prerequisitos: 
  ***
	*/
void vSpecial_Action(unsigned char cAction){
	
	enum digi_key Value_Key_Press = KEY_NULL;

	unsigned char cCaracter_Serial;
	unsigned char uFlag_Show_Filter = 1;
	
	unsigned char sStringValue[10] = {0};

	int iVoltage = 0;
	int iAux_Value = 0;
	int i=0, j=0, k=0;
	
	float ValueVolt = 0;
	float fValue_Memory = 0;
	float fVoltage_Battery = 0;	
	double fAux_Value = 0;
	
	stScaleParam.cSpecialAction = 1;
	
	LCD_GLASS_Clear();
	
	switch(cAction){
		
		case All_Segment:
		
			if(stScaleParam.cBacklight){OnBackLight;}
			LCD_GLASS_All_On();
			
			while (1){
				IWDG->KR = IWDG_KEY_REFRESH;
				vActionKey();
				if(srFlagScale.bScaleOn == FALSE){
					return;
				}
				vGestorBateria();
			}
			
			OffBackLight;
			break;
		
		case Data_Scale:
	
			LCD_GLASS_String(sVersion, LCD_TOTAL);
		
			if(stScaleParam.cModeloBascula == PreConfig60LB){
				LCD_GLASS_String("SH   ", LCD_PRECIO);
			}else{
				LCD_GLASS_String("SNART", LCD_PRECIO);
			}
			
			if(stScaleParam.cUnits == UNIT_KG){
				LCD_GLASS_Symbols(SYMBOL_KG, 1);
			}else if(stScaleParam.cUnits == UNIT_LB){
				LCD_GLASS_Symbols(SYMBOL_LB, 1);
			}
			
			LCD_GLASS_Float(stScaleParam.iCapacity, (int)stScaleParam.cWeightDecimal, LCD_PESO);			
			LCD_GLASS_String(sVersion, LCD_TOTAL);
			
			while (1){
				IWDG->KR = IWDG_KEY_REFRESH;
				Value_Key_Press = vActionKey();
				
				if(srFlagScale.bScaleOn == FALSE){
					return;
				}else if(Value_Key_Press == KEY_C){
					vBeep_Key();
					break;
				}
				vGestorBateria();
			}
			
			break;
		
		case Offset_Adc:
		
			LCD_GLASS_String("  ADC", LCD_PESO);
			LCD_GLASS_String("70015", LCD_PRECIO);
			
			vConfigADC();
						
			while(1){
				
				if(uFlag_Show_Filter == 1){					
					fAux_Value = fRead_Adc(0);
				}else{
					fAux_Value = fRead_Adc(1);
				}
				
				if(srFlagScale.bErrorResponseAdc == 1){
					LCD_GLASS_String(" NCADC", LCD_TOTAL);
				}else if(srFlagScale.bErrorReadAdc == 1){
					LCD_GLASS_String(" ELADC", LCD_TOTAL);
				}else{
					LCD_GLASS_Float(fAux_Value, 0, LCD_TOTAL);
				}
				
				Value_Key_Press = vActionKey();
				
				/*if(srFlagScale.bScaleOn == FALSE){
					return;
				}else if(Value_Key_Press != KEY_NULL){					
					vBeep_Key();
					if(Value_Key_Press == KEY_1){
						uFlag_Show_Filter = 1;
					}else if(Value_Key_Press == KEY_0){
						uFlag_Show_Filter = 0;
					}else{					
						return;
					}
				}*/
				
				if(srFlagScale.bScaleOn == FALSE){
					return;
				}
				
				vGestorBateria();
			}
			
			break;
		
		case Reset_Counter:
					
			stScaleParam.iCounter_Calibration = 1;
			stScaleParam.iCounter_Configuration = 1;
			LCD_GLASS_String("RESET", LCD_PESO);
			LCD_GLASS_String("COUNT", LCD_PRECIO);
			vSaveParamScale(Parameter_Configuration);
			vSaveParamScale(Parameter_Calibration);
			LCD_GLASS_String("  DONE", LCD_TOTAL);
			vSound_Saved_Param();
			vSound_Saved_Param();
			
			break;
			
		case View_Counter_Calibration:
					
			LCD_GLASS_String("CA", LCD_PRECIO);
			LCD_GLASS_Float(stScaleParam.iCounter_Calibration, 0, LCD_TOTAL);
			
			while (1){
				IWDG->KR = IWDG_KEY_REFRESH;
				Value_Key_Press = vActionKey();
				
				if(srFlagScale.bScaleOn == FALSE){
					return;
				}else if(Value_Key_Press != KEY_NULL){
					vBeep_Key();

					if(Value_Key_Press == KEY_C){						
						break;
					}
				}
			}
			
			break;
			
		case View_Counter_Configuration:
					
			LCD_GLASS_String("CF", LCD_PRECIO);
			LCD_GLASS_Float(stScaleParam.iCounter_Configuration, 0, LCD_TOTAL);
			
			while (1){
				IWDG->KR = IWDG_KEY_REFRESH;
				Value_Key_Press = vActionKey();
				
				if(srFlagScale.bScaleOn == FALSE){
					return;
				}else if(Value_Key_Press != KEY_NULL){
					vBeep_Key();

					if(Value_Key_Press == KEY_C){						
						break;
					}
				}
			}

			break;
			
		case Erase_Plus:
						
			LCD_GLASS_String("RESET", LCD_PESO);
			LCD_GLASS_String(" PLUS", LCD_PRECIO);
			vErase_All_Address_Plus();
			LCD_GLASS_String("  DONE", LCD_TOTAL);
			vSound_Saved_Param();
			vSound_Saved_Param();
			
			break;
			
		case View_Voltage_Battery:
			
			fAux_Value = 0;
			
			stScaleParam.cSpecialAction = 0;
			
		#if DISPLAY_20400047_EN > 0
			driver_symbol();
		#endif
			
			while(1){
				IWDG->KR = IWDG_KEY_REFRESH;
				//vGestorBateria();
				//vGet_Volts_System();
				uGet_Status_Volt();
				
				if(fAux_Value == 0){
					vShowDataBatLcd();	
				}else{
					vShowDataCargador();
				}
				
				Value_Key_Press = vActionKey();
				
				if(srFlagScale.bScaleOn == FALSE){
					return;
				}else if(Value_Key_Press != KEY_NULL){
					vBeep_Key();

					if(Value_Key_Press == KEY_C){						
						break;
					}else if(Value_Key_Press == KEY_1){
						fAux_Value = 1;
					}else if(Value_Key_Press == KEY_0){
						fAux_Value = 0;
					}
				}				
			}
			
			break;
			
		case View_Memory:
			
			while(1){
				
				for(i=0; i<100; i++){
					
					vActionKey();
					LCD_GLASS_Float(i, 0, LCD_PRECIO);					
					fValue_Memory = fFuncionPlus(i, 1, 0, 0, 0);						
					LCD_GLASS_Float(fValue_Memory, stScaleParam.cNumberDecimalPrice , LCD_TOTAL);		
								
					for(j=0; j<400; j++){
						for(k=0; k<500; k++){
							IWDG->KR = IWDG_KEY_REFRESH;
							if(srFlagScale.bScaleOn == FALSE){
								return;
							}
						}
					}
				}
			}
			
			break;
			
		case Funcion_Especial:
					
			LCD_GLASS_All_On();
			if(stScaleParam.cBacklight){OnBackLight;}
			
			strTimer.cFLag_TimerD_Start = 1;		// Inicia un timer para esperar 2 seg 
			strTimer.cFLag_TimerD_End = 0;
			while(strTimer.cFLag_TimerD_End == 0){
				IWDG->KR = IWDG_KEY_REFRESH;
				if(srFlagScale.bScaleOn == FALSE){
					return;
				}
			}	
						
			OffBackLight;
			LCD_GLASS_Clear();
			LCD_GLASS_String("REF 0", LCD_PESO);
			
			strTimer.cFLag_TimerD_Start = 1;		// Inicia un timer para esperar 2 seg 
			strTimer.cFLag_TimerD_End = 0;
			while(strTimer.cFLag_TimerD_End == 0){
				IWDG->KR = IWDG_KEY_REFRESH;
				if(srFlagScale.bScaleOn == FALSE){
					return;
				}
			}	
				
			/* Solicita la referencia de zero */
			stScaleParam.fPointZeroCali = fStablePoint(5, 1, 0);
			
			LCD_GLASS_Clear();
			LCD_GLASS_String(" LOAD", LCD_PESO);
						
			strTimer.cFLag_TimerD_Start = 1;		// Inicia un timer para esperar 2 seg 
			strTimer.cFLag_TimerD_End = 0;
			while(strTimer.cFLag_TimerD_End == 0){
				IWDG->KR = IWDG_KEY_REFRESH;
				if(srFlagScale.bScaleOn == FALSE){
					return;
				}
			}	
			
			strTimer.cFLag_TimerD_Start = 1;		// Inicia un timer para esperar 2 seg 
			strTimer.cFLag_TimerD_End = 0;
			while(strTimer.cFLag_TimerD_End == 0){
				IWDG->KR = IWDG_KEY_REFRESH;
				if(srFlagScale.bScaleOn == FALSE){
					return;
				}
			}	
			
			/* Solicita el peso para calibrar y obtiene el Factor de calibracion */
			stScaleParam.fCapacityCali = fStablePoint(5, 1, 0);
			
			stScaleParam.fCapacityCali -= stScaleParam.fPointZeroCali;
			stScaleParam.fCapacityCali *= (100/stScaleParam.iLoadPorcRefer);
			
			stScaleParam.fFactorCalibrate = (float)(stScaleParam.fCapacityCali);
			fAux_Value = (float)(stScaleParam.iCapacity);
			fAux_Value *= pow(10,(int)stScaleParam.cWeightDecimal);
			fAux_Value = (float)(fAux_Value)/(float)(stScaleParam.iDivisionMinima);
			stScaleParam.fFactorCalibrate /= fAux_Value;
			
			if(stScaleParam.fCapacityCali < 0){
				stScaleParam.fCapacityCali *= (-1);
			}
				
			/* Aumenta el contador de calibraciones */
			stScaleParam.iCounter_Calibration++;	
			/*Almacena los parametros en la E2prom*/	
			vSaveParamScale(Parameter_Calibration);			
	
         	vSet_Volts_System();	
			vSaveParamScale(Parameter_Voltages);
	
			stScaleParam.fTemperature = fGet_Temp_Amb_Micro();
			vSaveParamScale(Parameter_Temperature);			
																				
			LCD_GLASS_Clear();	
			LCD_GLASS_String("  END", LCD_PESO);
			LCD_GLASS_Float(stScaleParam.fFactorCalibrate, 2, LCD_TOTAL);
			LCD_GLASS_String("R", LCD_TOTAL);
			LCD_GLASS_Dot(2, LCD_TOTAL, 1);
			LCD_GLASS_String("FACTO", LCD_PRECIO);
			
			vSound_Saved_Param();
			
			strTimer.cFLag_TimerD_Start = 1;		// Inicia un timer para esperar 2 seg 
			strTimer.cFLag_TimerD_End = 0;
			while(strTimer.cFLag_TimerD_End == 0){
				IWDG->KR = IWDG_KEY_REFRESH;
			}	
			
			LCD_GLASS_Clear();
			cFuncion_Especial = 1;
		
			break;
		
		case Info_Calibracion:
		
			LCD_GLASS_Float(stScaleParam.fCapacityCali, 1, LCD_TOTAL);		
			LCD_GLASS_Float(stScaleParam.fFactorCalibrate, 3, LCD_PESO);
			LCD_GLASS_Float(stScaleParam.fPointZeroCali, 0, LCD_PRECIO);			
			
			while (1){
				IWDG->KR = IWDG_KEY_REFRESH;
				vActionKey();
				
				if(srFlagScale.bScaleOn == FALSE){
					return;
				}
			}			
			
			break;
		
		case Activar_Debug:
					
			LCD_GLASS_String("DEBUG", LCD_PESO);
			
			if(stScaleParam.iDebug == 24275){
				LCD_GLASS_String("   ON", LCD_PRECIO);
			}else{
				LCD_GLASS_String("  OFF", LCD_PRECIO);
			}
			
			Value_Key_Press = KEY_NULL;
			
			while(Value_Key_Press != KEY_TARA){				

				Value_Key_Press = vActionKey();
				
				if(srFlagScale.bScaleOn == FALSE){
					return;
				}else if(Value_Key_Press == KEY_MEM){
					vBeep_Key();
					LCD_GLASS_String("   ON", LCD_PRECIO);
					stScaleParam.iDebug = 24275;
				}else if(Value_Key_Press == KEY_RCL){
					vBeep_Key();
					LCD_GLASS_String("  OFF", LCD_PRECIO);
					stScaleParam.iDebug = 0;
				}
			} 
						
			vSaveParamScale(Parameter_Debug);
			LCD_GLASS_String("SAVED", LCD_TOTAL);
		
			vSound_Saved_Param();
			vSound_Saved_Param();
	
			strTimer.cFLag_TimerD_Start = 1;		// Inicia un timer para esperar 2 seg 
			strTimer.cFLag_TimerD_End = 0;
			while(strTimer.cFLag_TimerD_End == 0){
				IWDG->KR = IWDG_KEY_REFRESH;
				if(srFlagScale.bScaleOn == FALSE){
					return;
				}
			}	
			
			while(1){
				IWDG->KR = IWDG_KEY_REFRESH;
				if(srFlagScale.bScaleOn == FALSE){
					return;
				}
			}
			
			break;
			
		case Function_Temperature:
			
			LCD_GLASS_String("TENNP", LCD_PESO);
			LCD_GLASS_String("ANNB", LCD_PRECIO);
			
			while(1){
				fAux_Value = fGet_Temp_Amb_Micro();		
				LCD_GLASS_Float(fAux_Value, 1, LCD_TOTAL);	
				
				if(srFlagScale.bScaleOn == FALSE){
					return;
				}				
			}			
			break;
		
		case Function_VerOverload:		
			
			LCD_GLASS_Float(fCuentasToPeso(stScaleParam.fValueOverload), (int)stScaleParam.cWeightDecimal, LCD_PESO);
			
			LCD_GLASS_Float(stScaleParam.iCountOverload, 0, LCD_TOTAL);
			LCD_GLASS_String("OVERL", LCD_PRECIO);
			
			if(stScaleParam.cUnits == UNIT_KG){
				LCD_GLASS_Symbols(SYMBOL_KG, 1);
			}else if(stScaleParam.cUnits == UNIT_LB){
				LCD_GLASS_Symbols(SYMBOL_LB, 1);
			}
			
			Value_Key_Press = KEY_NULL;
			
			while(Value_Key_Press != KEY_C){
				Value_Key_Press = vActionKey();
				
				if(srFlagScale.bScaleOn == FALSE){
					return;
				}else if(Value_Key_Press != KEY_NULL){
					vBeep_Key();
				}
			}
			
			break;
			
		case Function_TestTeclado:
				
			vTestTeclado();
			break;
			
		case Function_CalidadTest:
		
			vCalidadTest();			
			break;
			
		case Function_Backlight:
		
			LCD_GLASS_Clear();
			vBeep_Key();
			if(stScaleParam.cBacklight==0){				
				OffBackLight;
				LCD_GLASS_String("BL", LCD_PESO);
				LCD_GLASS_String("OFF", LCD_PRECIO);
			}else{				
				OnBackLight;
				LCD_GLASS_String("BL", LCD_PESO);
				LCD_GLASS_String("ON ", LCD_PRECIO);				
			}			
			
			Value_Key_Press = KEY_NULL;			
			while(Value_Key_Press != KEY_TARA){				

				Value_Key_Press = vActionKey();				
				if(srFlagScale.bScaleOn == FALSE){
					return;
				}else if(Value_Key_Press == KEY_MEM){
					vBeep_Key();
					stScaleParam.cBacklight = 0;
					OffBackLight;
					LCD_GLASS_String("BL", LCD_PESO);
					LCD_GLASS_String("OFF", LCD_PRECIO);										
				}else if(Value_Key_Press == KEY_RCL){
					vBeep_Key();
					stScaleParam.cBacklight = 1;
					OnBackLight;
					LCD_GLASS_String("BL", LCD_PESO);
					LCD_GLASS_String("ON ", LCD_PRECIO);									
				}
			}			
			
			vSaveParamScale(Parameter_Backlight);
			LCD_GLASS_String("SAVED", LCD_TOTAL);		
			vSound_Saved_Param();
			vSound_Saved_Param();			
			break;
	}
	
	stScaleParam.cSpecialAction = 0;
}		

/*

*/
float fGet_Temp_Amb_Micro(void){

	float fAux_Value = 0;
	long iValue_ADC = 0;
	int i = 0;	

	CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);
	
	ADC_SamplingTimeConfig(ADC1, ADC_Group_FastChannels, ADC_SamplingTime_48Cycles);
	
	ADC_TempSensorCmd(ENABLE);
	
	ADC_Cmd(ADC1, ENABLE);
	ADC_ChannelCmd(ADC1, ADC_Channel_TempSensor, ENABLE);
	
	for(i=0; i<100; i++){
		ADC_SoftwareStartConv(ADC1);		

		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)){}
	
		iValue_ADC += ADC_GetConversionValue(ADC1);
	}

	ADC_ChannelCmd(ADC1, ADC_Channel_TempSensor, DISABLE);
	
	ADC_Cmd(ADC1, DISABLE);
	ADC_TempSensorCmd(DISABLE);
	
	CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
	
	iValue_ADC /= i;
	
	fAux_Value =  0.00080078125*(float)(iValue_ADC);
	fAux_Value *= 1000;
	fAux_Value /= 1.645;
	fAux_Value -= 273.15;
	
	return fAux_Value;
}

/*

*/
void vTestTeclado(void){
	
	u8 *Name_Tecla[21] = {"", "     0", "     1", "     2", 
	"     3", "     4", "     5", "     6", "     7", "     8",
	"     9", "  TARA", "  NNAS", "CHANGE", "   NN1",
	"   NN2", " NNENN", "   RCL", "     C", "  CERO", " PUNTO"
	};
	
	u8 sPush_Tecla[24] = {0};
	int iSumaTeclas = 0, i = 0;
	int iNumberTeclas = 0;
	
	enum digi_key Value_Key_Press;
		
	LCD_GLASS_Clear();			
	LCD_GLASS_String("TEST ", LCD_PESO);
	LCD_GLASS_String("TECLA", LCD_PRECIO);
	
	iNumberTeclas = 21;
	
	srFlagScale.bCalidadTest = 2;
	
	while(1){
		
		Value_Key_Press = vActionKey();
		
		if(srFlagScale.bScaleOn == FALSE){
			return;
		}else if(Value_Key_Press != KEY_NULL){
			vBeep_Key();			
			LCD_GLASS_String(Name_Tecla[(int)(Value_Key_Press)], LCD_TOTAL);
			sPush_Tecla[(int)(Value_Key_Press)] = 1;
			
			iSumaTeclas = 0;
			for(i=0; i<iNumberTeclas; i++){
				iSumaTeclas += sPush_Tecla[i];
			}
						
			if(iSumaTeclas == iNumberTeclas-1){
				LCD_GLASS_Clear();
				LCD_GLASS_String("  PASS", LCD_TOTAL);	
				vSound_Saved_Param();
				vSound_Saved_Param();					
				srFlagScale.bCalidadTest = 0;
				break;
			}			
		}		
	}	
}

/*

*/
void vCalidadTest(void){
	
	float fAux_Value = 0;
	enum digi_key Value_Key_Press;
	
	LCD_GLASS_Clear();			
	
	srFlagScale.bCalidadTest = 1;
	srFlagScale.bTopeSobrePeso = 1;
		
	LCD_GLASS_String("SOBRE", LCD_PESO);
	LCD_GLASS_String(" PESO", LCD_PRECIO);
	LCD_GLASS_String("   OFF", LCD_TOTAL);
	vSound_Saved_Param();
	vSound_Saved_Param();
	
	strTimer.cFLag_TimerD_End = 0;
	strTimer.cFLag_TimerD_Start = 1;		/* Inicia un timer para esperar 2 seg 
											a qe se digite alguna tecla para introducir un codigo */
	
	while(strTimer.cFLag_TimerD_End == 0)
	{		
		IWDG->KR = IWDG_KEY_REFRESH;
		fRead_Adc(0);			
		if(srFlagScale.bScaleOn == FALSE){
			return;
		}		
	}
	
	/* Se espera hasta encontrar un punto estable ademas de verificar las 
		condiciones iniciales del peso */
	cSetZeroPoint();
	
	do{
		Value_Key_Press = vActionKey();
	
		if(Value_Key_Press != KEY_NULL){
			vBeep_Key();
		}
		
		cRun_Scale(0);
		
		if(srFlagScale.bScaleOn == FALSE){
			return;
		}
	}while(Value_Key_Press != KEY_C);
	
	LCD_GLASS_Clear();
	LCD_GLASS_String("OFFSET", LCD_TOTAL);
	
	if(stScaleParam.cUnits == UNIT_KG){
		LCD_GLASS_Symbols(SYMBOL_KG, 1);
	}else if(stScaleParam.cUnits == UNIT_LB){
		LCD_GLASS_Symbols(SYMBOL_LB, 1);
	}
	
	do{
		
		fAux_Value = fRead_Adc(0);
		fAux_Value = fCuentasToPeso(fAux_Value);
		Value_Key_Press = vActionKey();
	
		if(srFlagScale.bScaleOn == FALSE){
			return;
		}else if(Value_Key_Press != KEY_NULL){
			vBeep_Key();
		}
		
		LCD_GLASS_Float(fAux_Value, stScaleParam.cWeightDecimal, LCD_PESO);			
		
	}while(Value_Key_Press != KEY_C);
	
	vSpecial_Action(Data_Scale);	
	vSpecial_Action(View_Voltage_Battery);
	vSpecial_Action(Erase_Plus);
	vSpecial_Action(Reset_Counter);
	
	LCD_GLASS_Clear();
	LCD_GLASS_Float(stScaleParam.iCounter_Calibration, 0, LCD_PRECIO);
	LCD_GLASS_String("CA", LCD_PRECIO);
	
	LCD_GLASS_Float(stScaleParam.iCounter_Configuration, 0, LCD_TOTAL);
	LCD_GLASS_String(" CF", LCD_TOTAL);
	
	Value_Key_Press = KEY_NULL;
	
	while (Value_Key_Press == KEY_NULL){
		IWDG->KR = IWDG_KEY_REFRESH;
		Value_Key_Press = vActionKey();
		
		if(srFlagScale.bScaleOn == FALSE){
					return;
		}else if(Value_Key_Press == KEY_C){
			vBeep_Key();	
		}
	}

	LCD_GLASS_Clear();			
	LCD_GLASS_Symbols(SYMBOL_ZERO, 1);
	LCD_GLASS_Float(stScaleParam.fPointZeroCali, 0, LCD_PRECIO);
	
	Value_Key_Press = KEY_NULL;
	
	while (Value_Key_Press == KEY_NULL){
		IWDG->KR = IWDG_KEY_REFRESH;
		Value_Key_Press = vActionKey();
		
		if(srFlagScale.bScaleOn == FALSE){
			return;
		}else if(Value_Key_Press == KEY_C){
			vBeep_Key();	
		}
	}
	
	srFlagScale.bCalidadTest = 2;
	OffBackLight;
	
	vSpecial_Action(Function_TestTeclado);
	
	srFlagScale.bCalidadTest = 0;
	srFlagScale.bTopeSobrePeso = 0;
	
	stScaleParam.fVenta_Total_Scale = 0;
}

