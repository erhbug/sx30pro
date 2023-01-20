/** 
 ******************************************************************************
 * Archivo 	dvr_def.c
 * Resumen 	Contiene las funciones para el manejo de las memorias plu
 * Autor 		MC. Jaime Ramirez
 * Empresa 	Fabricantes de Basculas Torrey S.A. de C.V.
 * version 	1
 * Fecha:		05/10/11 
 ******************************************************************************
 */
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "./_data_nvm/data_nvm_5219_Vc_Dec09_13.h"	
#include "./_scale/dvr_def.h"
#include "./_display/dvr_lcd_SDI1621.h"
#include "./customer/keyboard.h"
#include "./_weight/dvr_HX712.h"
#include "./_scale/dvr_scale.h"
#include "./customer/beeper.h"
#include "./_battery/dvr_battery.h"
#include "./customer/dvr_registradora.h"

#define ADDRES_START_PLUS	ADDRESS_PLU //200


/**
  ******************************************************************************
  * Objetivo: Leer o guardar dato en memoria plu
  * Parametros Entrada: 
	* Parametros Salida: 
  ******************************************************************************
  */
	
float fFuncionPlus(int fAddress_Plus, unsigned char cReadPlus, 
	unsigned char cWritePlus, float fPrice_Save, unsigned char cDecimalDot){
	
/*	float fData_rta;
	
	if(fAddress_Plus > -1 && fAddress_Plus < 100){
		
		while(!strTimer.iTimerA < TimerAend);
		
		if(cReadPlus){
			
			//fData_rta = fReadFloatEeprom((int)(ADDRES_START_PLUS + (fAddress_Plus*5)));
			fData_rta = flash_read_float32((int)(ADDRES_START_PLUS + (fAddress_Plus*5)));
			
			
			if(fData_rta < 0 || fData_rta > 99999){
				
				//Existe un error en la memoria y se resetea al valor a 0
				vEepromInit(ENABLE);					
				flash_write_float32((int)(ADDRES_START_PLUS + (fAddress_Plus*5)), 0);
				flash_write_u8((int)(ADDRES_START_PLUS + (fAddress_Plus*5) + 4), 0);
				stScaleParam.cNumberDecimalPrice = 0;
				fData_rta = 0;
				vEepromInit(DISABLE);					
			}else{
				stScaleParam.cNumberDecimalPrice = flash_read_u8((int)(ADDRES_START_PLUS + (fAddress_Plus*5) + 4));
			}
			
			if(stScaleParam.cNumberDecimalPrice > 0){
				srFlagScale.bDotDecimalPrice = 1;
			}else{
				srFlagScale.bDotDecimalPrice = 0;
			}
			
			return fData_rta;
			
		}else if(cWritePlus){
			vEepromInit(ENABLE);					// Habilita la escritura/lectura en la EEPROM 
			
			if(fPrice_Save <= 99999 && cDecimalDot <= 3){
				flash_write_float32((int)(ADDRES_START_PLUS + (fAddress_Plus*5)), fPrice_Save);
				
				if(stScaleParam.cPuntoDecimalPrecio > 2){
					flash_write_u8((int)(ADDRES_START_PLUS + (fAddress_Plus*5) + 4), cDecimalDot);
				}else{
					flash_write_u8((int)(ADDRES_START_PLUS + (fAddress_Plus*5) + 4), stScaleParam.cPuntoDecimalPrecio);
				}
			
				vSound_Saved_Param();
			}else{
				fPrice_Save = 0;
			}
			
			vEepromInit(DISABLE);
			
			return fPrice_Save;
			
		}
	}*/
	return fPrice_Save;
}

/**
  ******************************************************************************
  * Objetivo: Borrar todas las posiciones de memoria plu.
  * Parametros Entrada: Ninguno.
	* Parametros Salida: Ninguno
  ******************************************************************************
  */
void vErase_All_Address_Plus(void){
	
/*	int i = 0;
	
	while(!strTimer.iTimerA < TimerAend);
	
	vEepromInit(ENABLE);					// Habilita la escritura/lectura en la EEPROM 
	
	for(i=0; i<100; i++){
		flash_write_float32((int)(ADDRES_START_PLUS + (i*5)), 0);
		flash_write_u8((int)(ADDRES_START_PLUS + (i*5) + 4), 0);
		LCD_GLASS_Float(i, 0, LCD_TOTAL);
	}
	
	vEepromInit(DISABLE);	*/
}