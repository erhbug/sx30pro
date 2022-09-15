#include <stdio.h>
#include <math.h>
#include <string.h>
#include "./_scale/app_cfg.h"
#include "./_scale/dvr_scale.h"
#include "./_scale/dvr_def.h"
#include "./_battery/dvr_battery.h"
#include "./_display/dvr_lcd_SDI1621.h"
#include "./_inicio/dvr_inicio.h"

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

extern float fStablePoint(unsigned char cSetCountBack, unsigned char cShowCount, unsigned char cRunStable);

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
		//fRead_Adc(1);
	}
	
	while(1){
		
		IWDG_KEY_REFRESH;//IWDG->KR = IWDG_KEY_REFRESH;
		//Value_Key_Press = vActionKey();		/* Verifica si se oprimio alguna tecla y almacena este valor */

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
						
				/*stScaleParam.fPointZeroCali = fStablePoint(1, 0, 0);
				vEepromInit(ENABLE);					/// Habilita la escritura/lectura en la EEPROM ///
				vSaveFloatEeprom(ADDR_POINT_ZERO, stScaleParam.fPointZeroCali);				
				vEepromInit(DISABLE);					/// Deshabilita la escritura en la EEPROM /// */
				
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
			
			/*if(stScaleParam.iDebug != 24275){
				if((GPIOA->IDR & PUSH_SWITCH) == 0){
					while(!strTimer.cFLag_TimerD_End){
						IWDG->KR = IWDG_KEY_REFRESH;
						vActionKey();
					}
					srFlagScale.bCodeConfiguration = 1;
					return 1;
				}
			}*/
		}
		
		if(srFlagScale.bScaleOn == 0){//if(srFlagScale.bScaleOn == FALSE){
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


