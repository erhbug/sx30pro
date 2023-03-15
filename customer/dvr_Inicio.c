#include <stdio.h>
#include <math.h>
#include <string.h>
#include "./_scale/dvr_scale.h"
#include "./_weight/dvr_HX712.h"
#include "./_scale/dvr_def.h"
#include "./_display/dvr_lcd_SDI1621.h"
#include "./customer/beeper.h"
#include "./customer/dvr_inicio.h"
#include "./_scale/dvr_scale.h"
#include "./_scale/app_cfg.h"
#include "./_data_nvm/data_nvm_5219_Vc_Dec09_13.h"	

/* --- Registro de Password */
code enum digi_key eSEQUENCE_CONFIGURATION[8] = {KEY_TARA, KEY_C, KEY_CERO, KEY_RCL};
code enum digi_key ePRE_CONFIGURATION_30KG[8] = {KEY_TARA, KEY_M1, KEY_TARA, KEY_3, KEY_0};
code enum digi_key ePRE_CONFIGURATION_31KG[8] = {KEY_TARA, KEY_M1, KEY_TARA, KEY_3, KEY_1};	
code enum digi_key ePRE_CONFIGURATION_32KG[8] = {KEY_TARA, KEY_M1, KEY_TARA, KEY_3, KEY_2};	
code enum digi_key eLCD_ALL_SEGMENT[8] = {KEY_TARA, KEY_8};
code enum digi_key eSOBRE_PESO_OFF[8] = {KEY_TARA, KEY_MEM, KEY_1, KEY_TARA};
code enum digi_key eERASE_PLUS[8] = {KEY_TARA, KEY_TARA, KEY_2, KEY_9};
code enum digi_key eRESET_COUNTER[8] = {KEY_TARA, KEY_RCL, KEY_C, KEY_CERO};
code enum digi_key eVIEW_COUNTER_CALIBRATION[8] = {KEY_TARA, KEY_MEM, KEY_C, KEY_1, KEY_0, KEY_0, KEY_0};
code enum digi_key eVIEW_COUNTER_CONFIGURATION[8] = {KEY_TARA, KEY_MEM, KEY_C, KEY_1, KEY_0, KEY_0, KEY_1};
code enum digi_key eSEQUENCE_RESET_REFERENCE[8] = {	KEY_TARA, KEY_TARA, KEY_0};
code enum digi_key eTEST_CALIDAD[8] = {	KEY_TARA, KEY_C, KEY_0, KEY_1, KEY_5,KEY_9};
code enum digi_key eACT_PROGRAM[8] = {KEY_TARA, KEY_2, KEY_4, KEY_2, KEY_7};
code enum digi_key ePASS_MULTIRANGO_OFF[8] = {KEY_TARA, KEY_5, KEY_4, KEY_3, KEY_2, KEY_1, KEY_0};
code unsigned char eCODE_CALIBRACION[7]= "765432";
code unsigned char eCODE_MENU[7]= "779103";

void vTestTeclado(void);
void vCalidadTest(void);

unsigned char cWait_Scale(void){	
enum digi_key arPass_Configuration[8] = {0,0,0,0,0,0,0,0};
unsigned int i=0;
unsigned int TimeEnd=2000;
								
	strTimer.iTimerE=1;//llenar vector
	while(strTimer.iTimerE < 1000)
		fRead_Adc(1);

	strTimer.iTimerE=1;// Inicia un timer para esperar 2 seg a qe se digite alguna tecla para introducir un codigo 
	while(1){		
		IWDG_KEY_REFRESH;
		key_scan();

		/* La tecla pulsada se almacena en el vector de pass de configuracion */
		if( Key != KEY_NULL  ){
			strTimer.iTimerE=1;		/* Reinicia el contador para permitir
																		/* escribir todo el codigo */
			/* Solo permite 7 digitos para el codigo */
			if(strlen(arPass_Configuration) < 7){
				/* Agrega la tecla pulsada a la cadena */
				strncat(arPass_Configuration, (unsigned char *)&Key, 1);
			}else{
				LCD_GLASS_Clear();
				LCD_GLASS_String("CODE", LCD_PESO);
				LCD_GLASS_String("BAD", LCD_PRECIO);
			}
			
			if(strcmp(eSEQUENCE_CONFIGURATION, arPass_Configuration) == 0){
				srFlagScale.bCodeConfiguration = 1;
				return 1;
			}else if(strcmp(ePRE_CONFIGURATION_30KG, arPass_Configuration) == 0){
			    vSound_Saved_Param();
				vPreConfiguration(PreConfig30KG);
				return 1;
			}else if(strcmp(ePRE_CONFIGURATION_31KG, arPass_Configuration) == 0){
			    vSound_Saved_Param();
				vPreConfiguration(PreConfig31KG);	
				return 1;	
			}else if(strcmp(ePRE_CONFIGURATION_32KG, arPass_Configuration) == 0){
				vSound_Saved_Param();
			    vPreConfiguration(PreConfig32KG);	
				return 1;
			}else if(strcmp(eTEST_CALIDAD, arPass_Configuration) == 0){				
				vCalidadTest();
				return 1;
			}else if(strcmp(eSEQUENCE_RESET_REFERENCE, arPass_Configuration) == 0){
						
				stScaleParam.fPointZeroCali = fStablePoint(1, 0, 0);
				vEepromInit(ENABLE);					/* Habilita la escritura/lectura en la EEPROM */
				flash_write_float32(ADDR_POINT_ZERO, stScaleParam.fPointZeroCali);				
				vEepromInit(DISABLE);					/* Deshabilita la escritura en la EEPROM */
				
				LCD_GLASS_String("RESET", LCD_PESO);
				LCD_GLASS_String("  ADC", LCD_PRECIO);
				LCD_GLASS_String("      ", LCD_TOTAL);
				return 1;
				
			}else if(strcmp(ePASS_MULTIRANGO_OFF, arPass_Configuration) == 0){
				
				stScaleParam.cMultirango = 0;				
				LCD_GLASS_String("NNULR", LCD_PESO);
				LCD_GLASS_String("  OFF", LCD_PRECIO);
				LCD_GLASS_String("      ", LCD_TOTAL);
				vSound_Saved_Param();
				return 1;

			}
		}
	 if(strTimer.iTimerE >= TimeEnd){
	 strTimer.iTimerE=0;
			if(strcmp(eLCD_ALL_SEGMENT, arPass_Configuration) == 0){
				
;
				
			}else if(strcmp(eSOBRE_PESO_OFF,arPass_Configuration) == 0){
				srFlagScale.bTopeSobrePeso = 1;
				vSound_Saved_Param();
			}

			/* Se espera hasta encontrar un punto estable ademas de verificar las 
				condiciones iniciales del peso */
			stScaleParam.fWeightOverload = (stScaleParam.fCapacityCali);
			stScaleParam.fWeightOverload += (stScaleParam.fFactorCalibrate * 9);

		  // if(cFuncion_Especial){
		//		stScaleParam.fPointZero = stScaleParam.fPointZeroCali;				
		//	}else{
				cSetZeroPoint();
		//	}
					
			stScaleParam.fPointZeroInitial = stScaleParam.fPointZero;
			
			return 0;
		}
	}
	return 0;
}




void vCalidadTest(void){
	float fAux_Value = 0;	
	
//	srFlagScale.bCalidadTest = 1;
	srFlagScale.bTopeSobrePeso = 1;
	vSound_Saved_Param();
	vSound_Saved_Param();
	
		/* Inicia un timer para esperar 2 seg 
											a qe se digite alguna tecla para introducir un codigo */
	strTimer.iTimerE=1;
	while(strTimer.iTimerE<500)
	{		
		IWDG_KEY_REFRESH;
		fRead_Adc(5);				
	}
	
	/* Se espera hasta encontrar un punto estable ademas de verificar las 
		condiciones iniciales del peso */
	cSetZeroPoint();
	
	do{
		key_scan();
		
		cRun_Scale();
		
	}while(Key != KEY_C);
	
	LCD_GLASS_Clear();
	LCD_GLASS_String("OFFSET", LCD_TOTAL);
	
	// if(stScaleParam.cUnits == UNIT_KG){
	// 	LCD_GLASS_Symbols(SYMBOL_KG, 1);
	// }else if(stScaleParam.cUnits == UNIT_LB){
	// 	LCD_GLASS_Symbols(SYMBOL_LB, 1);
	// }
	
	do{
		
		fAux_Value = fRead_Adc(5);
		fAux_Value = fCuentasToPeso(fAux_Value);
		key_scan();
		
		LCD_GLASS_Float(fAux_Value, stScaleParam.cWeightDecimal, LCD_PESO);			
		
	}while(Key != KEY_C);
	
	LCD_GLASS_Clear();
	LCD_GLASS_Float(stScaleParam.iCounter_Calibration, 0, LCD_PRECIO);
	LCD_GLASS_String("CA", LCD_PRECIO);
	
	LCD_GLASS_Float(stScaleParam.iCounter_Configuration, 0, LCD_TOTAL);
	LCD_GLASS_String(" CF", LCD_TOTAL);
	
	Key = KEY_NULL;
	
	while (Key == KEY_NULL){
		IWDG_KEY_REFRESH;
		key_scan();
		
		if(Key == KEY_C){
			vBeep_Key();	
		}
	}

	LCD_GLASS_Clear();			
	LCD_GLASS_Symbols(SYMBOL_ZERO, 1);
	LCD_GLASS_Float(stScaleParam.fPointZeroCali, 0, LCD_PRECIO);
	
	Key = KEY_NULL;
	
	while (Key == KEY_NULL){
		IWDG_KEY_REFRESH;
		key_scan();
		
		if(Key == KEY_C){
			vBeep_Key();	
		}
	}
	
//	srFlagScale.bCalidadTest = 2;
	OffBackLight;
		strTimer.iTimerE=1;
	while(strTimer.iTimerE<800)
	{		
		IWDG_KEY_REFRESH;			
	}
	OnBackLight;
	vTestTeclado();
	
//	srFlagScale.bCalidadTest = 0;
	srFlagScale.bTopeSobrePeso = 0;
	
	stScaleParam.fVenta_Total_Scale = 0;
}


void vTestTeclado(void){	
	unsigned char *Name_Tecla[(char)20] = {"CERO", "TARA", "NN1", 
	"RCL", "CHG", "NNAS", "NN2", "NNENN", "7",
	"4", "1", "C", "8", "5",
	"2", "0", "9", "6", "3", "PUNTO"
	};
	
	unsigned char sPush_Tecla[21] ;
	int iSumaTeclas = (int)0, i = (int)0;
	int iNumberTeclas = (int)0;
	LCD_GLASS_Clear();			
	LCD_GLASS_String("TEST ", LCD_PESO);
	LCD_GLASS_String("TECLA", LCD_PRECIO);
	
	iNumberTeclas = 20;
//	srFlagScale.bCalidadTest = 2;
memset(sPush_Tecla, 0x00, sizeof(sPush_Tecla));
	while(1){		
		key_scan();
		if(Key != KEY_NULL){		
			switch(Key){
				case KEY_CERO:	Key = 0; break;
				case KEY_TARA: Key = 1;	break;
				case KEY_M1:	Key = 2;	break;
				case KEY_RCL:   Key = 3;   break;   
				case KEY_CHG:   Key = 4;   break;
				case KEY_MAS:   Key = 5;   break;
				case KEY_M2:   Key = 6;   break;
				case KEY_MEM:   Key = 7;   break;
				case KEY_7:   Key = 8;   break;
				case KEY_4:   Key = 9;   break;
				case KEY_1:   Key = 10;   break;
				case KEY_C:   Key = 11;   break;
				case KEY_8:   Key = 12;   break;
				case KEY_5:   Key = 13;   break;
				case KEY_2:   Key = 14;   break;
				case KEY_0:   Key = 15;   break;
				case KEY_9:   Key = 16;   break;
				case KEY_6:   Key = 17;   break;
				case KEY_3:   Key = 18;   break;
				case KEY_PUNTO:   Key = 19;   break;
			}

LCD_GLASS_Clear();
			LCD_GLASS_String(Name_Tecla[(char)(Key)], LCD_TOTAL);
			sPush_Tecla[(char)(Key)] = 1;
			//LCD_GLASS_Float((float)Key,0, LCD_TOTAL);
			iSumaTeclas = 0;
			for(i=0; i<iNumberTeclas; i++){
				iSumaTeclas += sPush_Tecla[i];
			}
						
			if(iSumaTeclas == iNumberTeclas){
				LCD_GLASS_Clear();
				LCD_GLASS_String("  PASS", LCD_TOTAL);	
				vSound_Saved_Param();
				vSound_Saved_Param();					
//				srFlagScale.bCalidadTest = 0;
				break;
			}			
		}		
	}	
}


