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
#include "./customer/usr_dbg.h"

#define ADDRES_START_PLUS	ADDRESS_PLU //200

#define Delay_tmr 1000

/**
  ******************************************************************************
  * Objetivo: Leer o guardar dato en memoria plu
  * Parametros Entrada: 
	* Parametros Salida: 
  ******************************************************************************
  */
	
float fFuncionPlus(int fAddress_Plus, unsigned char cReadPlus, 
	unsigned char cWritePlus, float fPrice_Save, unsigned char cDecimalDot){
	
	float fDataRrta;

	if(fAddress_Plus >=0 && fAddress_Plus < 100){
	//usr_dbg("AAAAA",Delay_tmr);
		//while(!strTimer.iTimerA < TimerAend);

		if(cReadPlus){

		//usr_dbg("BBBBB",Delay_tmr);
			
			//fData_rta = fReadFloatEeprom((int)(ADDRES_START_PLUS + (fAddress_Plus*5)));//falla
			fDataRrta = flash_read_float32((int)(ADDRES_START_PLUS + (fAddress_Plus*5)));
			//LCD_GLASS_Float(fDataRrta,3,LCD_PRECIO); 

			//if(fDataRrta==0.0)fDataRrta=(float)((int)0);
			
			if(fDataRrta!=0.0){ //R >= 0.0 && fData_rta <= 99999.0){
				
				//usr_dbg("DDDDD",Delay_tmr);
				LCD_GLASS_Float(fDataRrta,3,LCD_PRECIO);
				stScaleParam.cNumberDecimalPrice = flash_read_u8((int)(ADDRES_START_PLUS + (fAddress_Plus*5) + 4));
							
			}else{
				usr_dbg("CCC",Delay_tmr);
				//Existe un error en la memoria y se resetea al valor a 0
				vEepromInit(ENABLE);					
				flash_write_float32((int)(ADDRES_START_PLUS + (fAddress_Plus*5)), 0);
				flash_write_u8((int)(ADDRES_START_PLUS + (fAddress_Plus*5) + 4), 0);
				stScaleParam.cNumberDecimalPrice = 0;
				fDataRrta = 0;
				vEepromInit(DISABLE);	
				}
			
			if(stScaleParam.cNumberDecimalPrice > 0){
			usr_dbg("EEEEE",Delay_tmr);
				srFlagScale.bDotDecimalPrice = 1;
			}else{
			usr_dbg("FFFFF",Delay_tmr);
				srFlagScale.bDotDecimalPrice = 0;
			}
			usr_dbg("GGGGG",Delay_tmr);
			return fDataRrta;
			
		}else if(cWritePlus){
			usr_dbg("HHHHH",Delay_tmr);
			vEepromInit(ENABLE);					// Habilita la escritura/lectura en la EEPROM 
			
			if(fPrice_Save <= 99999 && cDecimalDot <= 3){

			usr_dbg("11111",Delay_tmr);
				flash_write_float32((int)(ADDRES_START_PLUS + (fAddress_Plus*5)), fPrice_Save);

				
				if(stScaleParam.cPuntoDecimalPrecio > 2){
				usr_dbg("22222",Delay_tmr);
					flash_write_u8((int)(ADDRES_START_PLUS + (fAddress_Plus*5) + 4), cDecimalDot);
				}else{
				usr_dbg("33333",Delay_tmr);
					flash_write_u8((int)(ADDRES_START_PLUS + (fAddress_Plus*5) + 4), stScaleParam.cPuntoDecimalPrecio);
				}
	          usr_dbg("44444",Delay_tmr);		
				vSound_Saved_Param();
			}else{

			usr_dbg("55555",Delay_tmr);
				fPrice_Save = 0;
			}
			
			usr_dbg("66666",Delay_tmr);
			vEepromInit(DISABLE);
			
			return fPrice_Save;
			
		}
		usr_dbg("77777",Delay_tmr);
	}
	usr_dbg("--OK--",Delay_tmr);
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