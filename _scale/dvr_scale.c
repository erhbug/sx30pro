/* Includes ------------------------------------------------------------------*/
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

extern float fWeightScale;									/* Contiene el valor del peso leido */
extern float fWeightScaleBefore;
extern float fWeightLight;
extern unsigned int iCounterZeroTracking ;

/* --- Prototype Function ----------------------------------------------------*/

float fSleep_Run(void);
void vCalculate_Weight (void);
void vWeight_Negative(void);
void vWeight_Positive(void);

void vGetZeroTracking(float fActualWeightScale);
float fStablePoint(unsigned char cSetCountBack, unsigned char cShowCount, unsigned char cRunStable);


/**
  ******************************************************************************
  * Objetivo: Asignar valores de parametros de la bascula.
  * Parametros entrada: Ninguno.
	* Parametros Salida: Ninguno.
  * Prerequisitos: Modifica las variables globales de la estruct stScaleParam
  ******************************************************************************
  */
void vReadParamScale(void){
	stScaleParam.fCapacityCali		= flash_read_float32(ADDR_CAPACITY_CALI);
	stScaleParam.fPointZeroCali		= flash_read_float32(ADDR_POINT_ZERO);
	stScaleParam.fFactorCalibrate = flash_read_float32(ADDR_FACTOR_CALIBRATE);
	stScaleParam.fValueTara				= 0;
	
	stScaleParam.cMultirango	= flash_read_u8(ADDR_MULTIRANGO);
	stScaleParam.cLenguage				= flash_read_u8(ADDR_LENGUAGE);
	stScaleParam.cUnits 					= flash_read_u8(ADDR_UNITS);
	stScaleParam.cFormatoImpresion				= flash_read_u8(ADDR_FORMATO_IMPRESION);
	stScaleParam.iCapacity				=	flash_read_u16(ADDR_CAPACITY);
	stScaleParam.iLoadPorcRefer 	= flash_read_u16(ADDR_PORC_REF);
	stScaleParam.iDivisionMinima 	= flash_read_u16(ADDR_DIVISION_MINIMA);
	stScaleParam.iDivisionMenor 	= flash_read_u16(ADDR_DIVISION_MENOR);
	stScaleParam.iDivisionMenorMenor 	= flash_read_u16(ADDR_DIVISION_MENORMENOR);
	stScaleParam.cPuntoDecimalTotal		= flash_read_u8(ADDR_DECIMAL_TOTAL);
	stScaleParam.cRedondeoCifraTotal	= flash_read_u8(ADDR_REDONDEO_TOTAL);
//	stScaleParam.cBacklight		= flash_read_u8(ADDR_BACKLIGHT);
	
	if(stScaleParam.cRedondeoCifraTotal == 0){
		stScaleParam.cValorRedondeoCifraTotal = 5;
	}else{
		stScaleParam.cValorRedondeoCifraTotal = 1;
	}
	
	stScaleParam.cRedondeoCifraVentaTotal	= flash_read_u8(ADDR_REDONDEO_VENTA);
	
	if(stScaleParam.cRedondeoCifraVentaTotal == 0){
		stScaleParam.cValorcRedondeoCifraVentaTotal = 5;
	}else{
		stScaleParam.cValorcRedondeoCifraVentaTotal = 1;
	}
	
	stScaleParam.cPuntoDecimalPrecio			= flash_read_u8(ADDR_DECIMAL_PRECIO);
	stScaleParam.cZeroTracking						= 1; //flash_read_u8(ADDR_ZERO_TRACKING);
	stScaleParam.cMsgUnload								= flash_read_u8(ADDR_MSG_UNLOAD);
	stScaleParam.cTimeAutooff							= flash_read_u8(ADDR_TIME_OFF);
	stScaleParam.cModeloBascula						= flash_read_u8(ADDR_MODELO);
			
	stScaleParam.iCounter_Calibration	= flash_read_u16(ADDR_COUNTER_CALIBRATION);
	stScaleParam.iCounter_Configuration	= flash_read_u16(ADDR_COUNTER_CONFIGURATION);
	
	stScaleParam.cShowHighBat							= flash_read_u8(ADDR_MSGBAT); 
	
//	stScaleParam.iDebug 									= flash_read_u16(ADDR_DEBUG);

	stScaleParam.cCountRange 			 				= flash_read_u8(ADDR_COUNT_RANGE);
	
	stScaleParam.cMoneda									= flash_read_u8(ADDR_MONEDA);
		
	stScaleParam.iCountOverload						=	flash_read_u16(ADDR_COUNT_OVERLOAD);
	
	stScaleParam.fValueOverload						=	flash_read_float32(ADDR_VALUE_OVERLOAD);
	
	stScaleParam.cWeightDecimal 					= flash_read_u8(ADDR_DECIMAL_WEIGHT);
	
	stScaleParam.cSaveBattery					= flash_read_u8(ADDR_SAVEBATT);
	
	srFlagScale.bShowErroBat						= flash_read_u8(ADDR_ERRORBATT);
	
	stScaleParam.fVenta_Total_Scale    = flash_read_float32(ADDR_VENTA_TOTAL);
}

/**
  ******************************************************************************
  * Objetivo: Guardar parametros en la E2PROM
  * Parametros entrada: Ninguno
	* Parametros Salida: Ninguno
  ******************************************************************************
  */
void vSaveParamScale(unsigned char cType_Parameter){

	/* Habilita la escritura/lectura en la EEPROM */
	NRM_securty= 0xaa;


	switch(cType_Parameter){

		case Parameter_Calibration:	//ok	
			flash_write_u8(ADDR_SET_CALIBRATE, 1);
			flash_write_float32(ADDR_CAPACITY_CALI, stScaleParam.fCapacityCali);
			flash_write_float32(ADDR_POINT_ZERO, stScaleParam.fPointZeroCali);
			flash_write_float32(ADDR_FACTOR_CALIBRATE, stScaleParam.fFactorCalibrate);
            flash_write_u16(ADDR_COUNTER_CALIBRATION, stScaleParam.iCounter_Calibration);

			break;
		
		case Parameter_Count_Calibration:
			flash_write_u16(ADDR_COUNTER_CALIBRATION, stScaleParam.iCounter_Calibration);		
			break;
			
		case Parameter_Configuration:	
			flash_write_u8(ADDR_LENGUAGE, stScaleParam.cLenguage);
			flash_write_u8(ADDR_UNITS, stScaleParam.cUnits);		
			flash_write_u16(ADDR_CAPACITY, stScaleParam.iCapacity);
			flash_write_u16(ADDR_PORC_REF, stScaleParam.iLoadPorcRefer);
			flash_write_u16(ADDR_DIVISION_MINIMA, stScaleParam.iDivisionMinima);	
			flash_write_u16(ADDR_DIVISION_MENOR, stScaleParam.iDivisionMenor);
			flash_write_u16(ADDR_DIVISION_MENORMENOR, stScaleParam.iDivisionMenorMenor);			
			flash_write_u8(ADDR_DECIMAL_TOTAL, stScaleParam.cPuntoDecimalTotal);
			flash_write_u8(ADDR_REDONDEO_TOTAL, stScaleParam.cRedondeoCifraTotal);
			flash_write_u8(ADDR_DECIMAL_PRECIO, stScaleParam.cPuntoDecimalPrecio);
			flash_write_u8(ADDR_ZERO_TRACKING, stScaleParam.cZeroTracking);
			flash_write_u8(ADDR_MSG_UNLOAD, stScaleParam.cMsgUnload);
			flash_write_u8(ADDR_TIME_OFF, stScaleParam.cTimeAutooff);
			flash_write_u8(ADDR_MODELO, stScaleParam.cModeloBascula);
			flash_write_u8(ADDR_MSGBAT, stScaleParam.cShowHighBat);
			flash_write_u16(ADDR_COUNTER_CONFIGURATION, stScaleParam.iCounter_Configuration);
			flash_write_u8(ADDR_COUNT_RANGE, stScaleParam.cCountRange);
			flash_write_u8(ADDR_MONEDA, stScaleParam.cMoneda);		
			flash_write_u16(ADDR_COUNT_OVERLOAD, stScaleParam.iCountOverload);	

			flash_write_float32(ADDR_VALUE_OVERLOAD, stScaleParam.fValueOverload);	

			flash_write_u8(ADDR_DECIMAL_WEIGHT, stScaleParam.cWeightDecimal);	
			flash_write_u8(ADDR_SAVEBATT, stScaleParam.cSaveBattery);			
			flash_write_u8(ADDR_ERRORBATT, srFlagScale.bShowErroBat);
			flash_write_u8(ADDR_MULTIRANGO, stScaleParam.cMultirango);
			flash_write_u8(ADDR_FORMATO_IMPRESION, stScaleParam.cFormatoImpresion);		
			break;
	
		case Parameter_Count_Configuration:
			flash_write_u16(ADDR_COUNTER_CONFIGURATION, stScaleParam.iCounter_Configuration);
			break;
			
		case Parameter_Point_Zero_Calibration:
			flash_write_float32(ADDR_POINT_ZERO, stScaleParam.fPointZeroCali);			
			break;
				
	//	case Parameter_Debug:
	//		flash_write_u16(ADDR_DEBUG, stScaleParam.iDebug);
	//		break;
				
		case Parameter_Overload:
			flash_write_u16(ADDR_COUNT_OVERLOAD, stScaleParam.iCountOverload);		
			flash_write_float32(ADDR_VALUE_OVERLOAD, stScaleParam.fValueOverload);						
			break;
			
		case Parameter_Voltages:
			flash_write_float32(ADDR_VOLT_BATT, stScaleParam.fVoltage_Batt);
			flash_write_float32(ADDR_VOLT_ADAP, stScaleParam.fVoltage_Adap);
			break;
		
	
			
		case Parameter_Register:
			flash_write_float32(ADDR_VENTA_TOTAL, stScaleParam.fVenta_Total_Scale);
			break;

//		case Parameter_Backlight:
//			flash_write_u8(ADDR_BACKLIGHT, stScaleParam.cBacklight);
//			break;
	}
	
	/* Deshabilita la escritura en la EEPROM */
	NRM_securty = 0x00;
}

/**
  ******************************************************************************
  * Objetivo: Encontrar un valor estable durante un tiempo X.
  * Parametros entrada: 
	*		cSetCountBack -> Tiempo de muestreo (conteo descendente)
	*		cShowCount -> Hacer visible el conteo.
	*		cRunStable -> Llenar el filtro con el primer dato leido.
	* Parametros Salida: Ninguno.
	* Nota: Si durante la cuenta regrsiva el valor no esta dentro del rango
	* 	asignado, el conteo se reinicia.
  ******************************************************************************
  */
float fStablePoint(unsigned char cSetCountBack, unsigned char cShowCount, unsigned char cRunStable){	
	unsigned char cCountReading = 0;		/* Contador de lecturas */
	unsigned char cNumber_Count = 0;
	signed char cCountBack = (signed char)cSetCountBack;			/* Contador de regresion */
	float fActualWeightAdc=0;						/* Alamacena el peso actual valores adc */
	float fWeightAdc = 0;								/* Almacena el valor referencia */
	float fLimitRange =  (float)stScaleParam.cCountRange;//5
	int i=0;
	unsigned char strText_LCD[5];
	strText_LCD[5]=0;
	
	cNumber_Count = 0;
	if(cRunStable == 0){
		cNumber_Count = 2;
	}

	strTimer.iTimerE=1;
	while(strTimer.iTimerE < 1000)
	    fWeightAdc = (unsigned long)fRead_Adc(cRunStable);

	while(cCountBack > -1){
		delay_ms(150);
		fActualWeightAdc = fRead_Adc(cRunStable);
		/* Verifica si debe mostrar el contador en la LCD */
		if(cShowCount == 1){
			   strText_LCD[0]=strText_LCD[1]=strText_LCD[2]=strText_LCD[3]=strText_LCD[4]=cCountBack+'0';
				LCD_GLASS_String(strText_LCD, LCD_PESO);
				LCD_GLASS_String(strText_LCD, LCD_PRECIO);
//				LCD_GLASS_String(strText_LCD, LCD_TOTAL);
				LCD_GLASS_Float(fActualWeightAdc, 0, LCD_TOTAL);
		}
   		/* Verica que el ultimo valor leido este dentro del rango del valor 
			referencia */
		if(fActualWeightAdc >= (fWeightAdc - fLimitRange) && 
			fActualWeightAdc <= (fWeightAdc + fLimitRange)){
				cCountReading++;
				
				/* Si al completar 3 lecturas dentro del rango se decrementa el 
					contador */
				if(cCountReading > cNumber_Count){
					cCountBack--;
					cCountReading = 0;
				}					
		}else{
			// Si el dato no esta dentro del rango, se toma una nueva referencia y se reinicia la cuenta regresiva
			if(cShowCount == 1){
				LCD_GLASS_Clear();
				LCD_GLASS_String("   UN", LCD_PESO);
				LCD_GLASS_String("STABL", LCD_TOTAL);

                
			}
			cCountReading = 0;
			cCountBack = cSetCountBack;

			strTimer.iTimerE=1;
            while(strTimer.iTimerE < 1000)
	            fWeightAdc = fRead_Adc(cRunStable);
			
			if(cRunStable){
				srFlagScale.bFlagStable = 0;
				return (-100.0);
			}
		}
	}
	
	srFlagScale.bFlagStable = 1;
	return fWeightAdc;
}

/**
  ******************************************************************************
  * Objetivo: Calcular el numero de cuenta que se tomara como cero.
  * Parametros entrada: Ninguno.
	* Parametros Salida: Ninguno.
  * Prerequisitos: Se usan las variables globales
	*			stScaleParam.cSetCalibrate, stScaleParam.fCapacityCali,  
	*			arDataDisplayLcd.
	*	Modifica las variables globales:
	*			stScaleParam.fPointZero, arDataDisplayLcd.
  ******************************************************************************
  */


  unsigned char cSetZeroPoint(void){	

//////	unsigned char Value_Key_Press;
	float fWeightPlate = 0;		// Calcula el peso del plato en numero de cuentas 
	float fPointZero = 0;
		
	fPointZero = fStablePoint(0, 0, 0);
	

	//Se encuentra el peso actual del plato respecto al punto cero de calibracion 
	fWeightPlate = fPointZero - stScaleParam.fPointZeroCali;
	
	if(stScaleParam.cMsgUnload){
		// Verifica los rangos para los distintos avisos 
		if(((fWeightPlate > (stScaleParam.fCapacityCali * 0.20)) && (fWeightPlate < (stScaleParam.fCapacityCali * 0.50))) ||
				(fWeightPlate < (-1)*(stScaleParam.fCapacityCali * 0.20))){
			
			// Quita los simboloes de unidad de peso y Zero 
			LCD_GLASS_Clear();
			LCD_GLASS_String("  PLS", LCD_PESO);				
			LCD_GLASS_String("UNLOAD", LCD_TOTAL);	
		
			while(1){
				delay_ms(1);
			}
		}
	}
	
	if((fWeightPlate >= (stScaleParam.fCapacityCali * 0.50)) && fWeightPlate > 0){
		
		// Quita los simboloes de unidad de peso y Zero 
		LCD_GLASS_Clear();	
		LCD_GLASS_String("HELP ", LCD_PRECIO);	
	
		
		while(1){
			delay_ms(1);
		}
				
	}else{
		stScaleParam.fPointZero = fPointZero;
	}
	
	

return 0;
}


/**
  ******************************************************************************
  * Objetivo: Activar/Desactivar el uso de la Tara.
  * Parametros entrada: 
	*		cActionTara -> Accion a ejecutar (ON) Activa la Tara, 
  *																		(OFF) Desactiva la Tara.
	* Parametros Salida: Retorna
	*		(0) Para indicar que no se proceso la orden.
	*		(1) Para indicar que se Desactivo o Activo la Tara.
  * Prerequisitos: Se usan las variables globales
	*			stScaleParam.fCapacityCali.
	*	Modifica las variables globales:
	*		stScaleParam.fPointZeroTara, stScaleParam.fPointZero.
  ******************************************************************************
  */
void cOnOffModeTara(float fWeightTara){
	float fAdcCountTara = 0; 
	float fLimite_Inf = 0;
	float fLimite_Sup = 0;
	float fWeightZero = 0;
	float AuxTara = 0;
	
	fWeightZero = fStablePoint(1, 0, 1);
	
	if(srFlagScale.bFlagStable == 1){
	
		if(fWeightTara > 0){
			if(srFlagScale.bTara == 0){	
				
				if(stScaleParam.cUnits == UNIT_LB){
					if(fCuentasToPeso(stScaleParam.fWeightScaleTara) <= stScaleParam.iCapacity){
						stScaleParam.fValueTara = stScaleParam.fWeightScaleTara;
						srFlagScale.bTara = 1;
					}
				}else{
					if(fCuentasToPeso(stScaleParam.fWeightScaleTara) <= 9.995){
						stScaleParam.fValueTara = stScaleParam.fWeightScaleTara;
						srFlagScale.bTara = 1;
					}
				}
			}else{
				if(stScaleParam.cUnits == UNIT_LB){
					if(fCuentasToPeso(stScaleParam.fWeightScaleTara) <= stScaleParam.iCapacity){
						stScaleParam.fValueTara = stScaleParam.fWeightScaleTara;
					}
				}else{
					if(fCuentasToPeso(stScaleParam.fWeightScaleTara) < 9.999){
						stScaleParam.fValueTara = stScaleParam.fWeightScaleTara;
					}
				}
			}
		}else if(fWeightTara < 0){
			
			if(srFlagScale.bTara){
				fWeightTara *= (-1);
				
				if(stScaleParam.cMultirango == 1){
					if(stScaleParam.fValueTara <= stScaleParam.fCapacityCali*0.2){
							AuxTara = stScaleParam.fFactorCalibrate / stScaleParam.iDivisionMinima;
							AuxTara *= stScaleParam.iDivisionMenor;
							fLimite_Inf = fCuentasToPeso(stScaleParam.fValueTara - 3*AuxTara);
							fLimite_Sup = fCuentasToPeso(stScaleParam.fValueTara + 3*AuxTara);
						}else{
							fLimite_Inf = fCuentasToPeso(stScaleParam.fValueTara - 3*stScaleParam.fFactorCalibrate);
							fLimite_Sup = fCuentasToPeso(stScaleParam.fValueTara + 3*stScaleParam.fFactorCalibrate);
						}
				}else{
					fLimite_Inf = fCuentasToPeso(stScaleParam.fValueTara - 3*stScaleParam.fFactorCalibrate);
					fLimite_Sup = fCuentasToPeso(stScaleParam.fValueTara + 3*stScaleParam.fFactorCalibrate);
				}
					
				if((fWeightTara >= fLimite_Inf) && (fWeightTara <= fLimite_Sup)){
					LCD_GLASS_Symbols(SYMBOL_NET, 0);
					stScaleParam.fValueTara = 0;
					srFlagScale.bTara = 0;
					stScaleParam.fPointZero = stScaleParam.fWeightScaleCount;
					fFilter_Averaging(fWeightZero,5); 	//Ingresa el dato al filtro 
				}
			}
		}
		
	}
}

/**
  ******************************************************************************
  * Objetivo: Establecer el punto Zero
  * Parametros entrada: Ninguno
	* Parametros Salida: Retorna
  ******************************************************************************
  */
void vSetZero(void){
	float fWeightZero = 0;
	
	/* Verifica si el peso es estable para volver cero */
	fWeightZero = fStablePoint(1, 0, 1);
		
	if(srFlagScale.bFlagStable == 1){		
		stScaleParam.fPointZero = fWeightZero;
	}
}

/**
  ******************************************************************************
  * Objetivo: Establecer el punto Zero dinamico
  * Parametros entrada: Ninguno
	* Parametros Salida: Ninguno
  ******************************************************************************
  */
void vGetZeroTracking(float fActualWeightScale){
	float fValor_Limite = 0;
	
	
	if(stScaleParam.cMultirango == 1){
		fValor_Limite = stScaleParam.fFactorCalibrate/stScaleParam.iDivisionMinima;
		fValor_Limite *= stScaleParam.iDivisionMenorMenor;
	}else	
		fValor_Limite = stScaleParam.fFactorCalibrate;
	
	fValor_Limite /= 2; 
	
	if(fValor_Limite < 0){
		fValor_Limite *= (-1);
	}
	
	if(fActualWeightScale > (stScaleParam.fPointZero - fValor_Limite) &&
		fActualWeightScale < (stScaleParam.fPointZero + fValor_Limite)){
		
		iCounterZeroTracking++;
		if(iCounterZeroTracking >=15){
			stScaleParam.fPointZero = fActualWeightScale;
		}
	}else{
		iCounterZeroTracking = 0;
	}
}

/**
  ******************************************************************************
  * Objetivo: Ejecutar el proceso de calibracion de la bascula 
  * Parametros entrada:  
	* Parametros Salida:
  * Prerequisitos: 
  ******************************************************************************
  */
void vCalibrate_Scale(void){
              float fAux_Value = 0;
              float fAuxCountAdcInicial = 0;
              float fAuxCountAdcFinal = 0;
              float fAuxCountDif = 0;
			  float weightCalibrationfactor = 100/(float)stScaleParam.iLoadPorcRefer;
			  LCD_GLASS_Clear();
			  LCD_GLASS_String("PRESS", LCD_PESO);
              LCD_GLASS_String("REF 0", LCD_TOTAL);
              LCD_GLASS_String("C", LCD_PRECIO);
        
              /* Espera a que se oprima 'C' para continuar */
              Key=0;while(Key!= KEY_C)key_scan();

              /* Solicita la referencia de zero */
              stScaleParam.fPointZeroCali = fStablePoint(5, 1, 0);
              
              fAuxCountAdcInicial = stScaleParam.fPointZeroCali;

              LCD_GLASS_Clear();
              LCD_GLASS_String("PRESS", LCD_PESO);
              LCD_GLASS_String(" LOAD", LCD_TOTAL);
              LCD_GLASS_String("C   ", LCD_PRECIO);

 /* Espera a que se oprima 'C' para continuar */
              Key=0;while(Key!= KEY_C)key_scan();
			  delay_ms(1000);
              //stScaleParam.fCapacityCali =104962;// fStablePoint(5, 1, 0);              
			  
			  
			  stScaleParam.fCapacityCali = fStablePoint(5, 1, 0);     
			  fAuxCountAdcFinal = stScaleParam.fCapacityCali;
			  fAuxCountDif = (fAuxCountAdcFinal - fAuxCountAdcInicial);
			  fAuxCountDif = fAuxCountDif  * weightCalibrationfactor;


			  stScaleParam.fCapacityCali -= stScaleParam.fPointZeroCali;
              stScaleParam.fCapacityCali = stScaleParam.fCapacityCali *weightCalibrationfactor;
              stScaleParam.fFactorCalibrate = (float)(stScaleParam.fCapacityCali);

			  fAux_Value = (float)(stScaleParam.iCapacity);
              fAux_Value *= pow(10,(int)stScaleParam.cWeightDecimal);
              fAux_Value = (float)(fAux_Value)/(float)(stScaleParam.iDivisionMinima);
			  stScaleParam.fFactorCalibrate /= fAux_Value;
			   //############Borrar, solo utilizado en pruebas, no en codigo final
              //stScaleParam.fFactorCalibrate = stScaleParam.fFactorCalibrate*(100/stScaleParam.iLoadPorcRefer);
               if(stScaleParam.fCapacityCali < 0){
                            stScaleParam.fCapacityCali = abs(stScaleParam.fCapacityCali);
			  } 

              LCD_GLASS_Clear();     
              LCD_GLASS_String("FCTOR", LCD_PESO);
			  LCD_GLASS_Float(stScaleParam.fFactorCalibrate, 2, LCD_TOTAL);

			  /* Aumenta el contador de calibraciones */
			stScaleParam.iCounter_Calibration++;	

              vSaveParamScale(Parameter_Calibration);   
              vSaveParamScale(Parameter_Voltages);
			  vSound_Saved_Param();
              vSound_Saved_Param();

			  strTimer.iTimerE=1;
			  while((Key != KEY_MEM) && strTimer.iTimerE < 3000)
			  		key_scan();	
}
/**
  ******************************************************************************
  * Objetivo: Configurar valores por default.
  * Parametros entrada:  
	* Parametros Salida:
  * Prerequisitos: 
  ******************************************************************************
  */

  
void vPreConfiguration(unsigned char cPreConfiguration){
	LCD_GLASS_Clear();	
	stScaleParam.cMultirango = 1;
	stScaleParam.cLenguage = ESPANOL;

	if(cPreConfiguration == PreConfig15KG){
		stScaleParam.iCapacity = 15;
		stScaleParam.iDivisionMinima = 5;
		stScaleParam.iDivisionMenor = 2;
		stScaleParam.iDivisionMenorMenor = 1;
		stScaleParam.cWeightDecimal = 3;
		stScaleParam.cUnits = UNIT_KG;
		stScaleParam.iLoadPorcRefer = 100;		
		//LCD_GLASS_Symbols(SYMBOL_KG, 1);
		stScaleParam.cMoneda = MONEDA_PESOS;
	}else if(cPreConfiguration == PreConfig20KG){
		stScaleParam.iCapacity = 20;
		stScaleParam.iDivisionMinima = 5;
		stScaleParam.iDivisionMenor = 2;
		stScaleParam.iDivisionMenorMenor = 1;
		stScaleParam.cWeightDecimal = 3;
		stScaleParam.cUnits = UNIT_KG;
		stScaleParam.iLoadPorcRefer = 100;
		//LCD_GLASS_Symbols(SYMBOL_KG, 1);
		stScaleParam.cMoneda = MONEDA_PESOS;
	}else if(cPreConfiguration == PreConfig30KG){
		stScaleParam.iCapacity = 30;
		stScaleParam.iDivisionMinima = 10;
		stScaleParam.iDivisionMenor = 5;
		stScaleParam.iDivisionMenorMenor = 2;
		stScaleParam.cWeightDecimal = 3;
		stScaleParam.cUnits = UNIT_KG;
		stScaleParam.iLoadPorcRefer = 100;
		//LCD_GLASS_Symbols(SYMBOL_KG, 1);
		stScaleParam.cMoneda = MONEDA_PESOS;
	}else if(cPreConfiguration == PreConfig31KG){
		stScaleParam.iCapacity = 30;
		stScaleParam.iDivisionMinima = 5;
		stScaleParam.iDivisionMenor = 2;
		stScaleParam.iDivisionMenorMenor = 1;
		stScaleParam.cWeightDecimal = 3;
		stScaleParam.cUnits = UNIT_KG;
		stScaleParam.iLoadPorcRefer = 100;
		//LCD_GLASS_Symbols(SYMBOL_KG, 1);
		stScaleParam.cMoneda = MONEDA_PESOS;		
	}else if(cPreConfiguration == PreConfig32KG){
		stScaleParam.iCapacity = 30;
		stScaleParam.iDivisionMinima = 2;
		stScaleParam.iDivisionMenor = 2;
		stScaleParam.iDivisionMenorMenor = 2;
		stScaleParam.cMultirango = 0;
		stScaleParam.cWeightDecimal = 3;
		stScaleParam.cUnits = UNIT_KG;
		stScaleParam.iLoadPorcRefer = 100;
		//LCD_GLASS_Symbols(SYMBOL_KG, 1);
		stScaleParam.cMoneda = MONEDA_PESOS;		
	}else if(cPreConfiguration == PreConfig40KG){
		stScaleParam.iCapacity = 40;
		stScaleParam.iDivisionMinima = 10;
		stScaleParam.iDivisionMenor = 5;
		stScaleParam.iDivisionMenorMenor = 2;
		stScaleParam.cWeightDecimal = 3;
		stScaleParam.cUnits = UNIT_KG;
		stScaleParam.iLoadPorcRefer = 100;
		//LCD_GLASS_Symbols(SYMBOL_KG, 1);
		stScaleParam.cMoneda = MONEDA_PESOS;
	}else if(cPreConfiguration == PreConfig60LB){
		stScaleParam.iCapacity = 60;
		stScaleParam.iDivisionMinima = 2;
		stScaleParam.cWeightDecimal = 2;
		stScaleParam.cUnits = UNIT_LB;
		stScaleParam.cLenguage = ENGLISH;
		//LCD_GLASS_Symbols(SYMBOL_LB, 1);
		stScaleParam.cMoneda = MONEDA_DLLS;
		stScaleParam.cMultirango = 0;
		stScaleParam.iLoadPorcRefer = 100;
	}
	LCD_GLASS_Float(stScaleParam.iCapacity, stScaleParam.cWeightDecimal, LCD_PESO);
	//stScaleParam.iLoadPorcRefer = 100;
	stScaleParam.cPuntoDecimalTotal = 2;
	stScaleParam.cRedondeoCifraTotal = 1;
	stScaleParam.cPuntoDecimalPrecio = 3;
	stScaleParam.cValorcRedondeoCifraVentaTotal = 0;
	stScaleParam.cZeroTracking = 1;
	stScaleParam.cMsgUnload = 1;
	stScaleParam.cTimeAutooff = 1;
	stScaleParam.cShowHighBat = 1;
	stScaleParam.cModeIncDiv = 0;
	stScaleParam.cCountRange = 5;
	stScaleParam.iCountOverload = 0;		
	stScaleParam.fValueOverload = 0;
	stScaleParam.cSaveBattery	= 0;
    stScaleParam.cFormatoImpresion = 2; 
	stScaleParam.cTypeBeeper = 0;
	stScaleParam.fVoltage_Batt = 0;
	stScaleParam.fVoltage_Adap = 0;
	stScaleParam.fCapacityCali = 0;
	stScaleParam.fPointZeroCali = 0;
	stScaleParam.fFactorCalibrate = 5;//stScaleParam.fFactorCalibrate = 2;
    srFlagScale.bShowErroBat = 1;
	stScaleParam.fVenta_Total_Scale = 0;
	stScaleParam.iCounter_Calibration = 0;
	stScaleParam.iCounter_Configuration = 0;
	strTimer.iTimerE=1;
	key_scan();
	
	/* Espera a que se oprima la tecla 'MEM' o que pase el tiempo de 5 seg  */
	while((Key != KEY_MEM) && strTimer.iTimerE < TimerEend)
		key_scan();	
	if(strTimer.iTimerE >= TimerEend){
		LCD_GLASS_Clear();
		return;
	}	

	NRM_securty = 0xaa;
	e2rom_erase(ADDRESS_AUX_PAGE);
	e2rom_erase(ADDRESS_SCALE_CONFIG_PAGE);
	e2rom_erase(ADDRESS_PLU);
	NRM_securty = 0x00;

	//vErase_All_Address_Plus();
		vSaveParamScale(Parameter_Voltages); 		
        vSaveParamScale(Parameter_Calibration);
    	vSaveParamScale(Parameter_Register);
		vSaveParamScale(Parameter_Configuration);

		LCD_GLASS_String("SAVED", LCD_PRECIO);
		vSound_Saved_Param();
		vSound_Saved_Param();
		
		key_scan();		
		strTimer.iTimerE=1;
		while((Key != KEY_MEM) && strTimer.iTimerE < TimerEend)/* Espera a que se oprima la tecla 'MEM' o que pase el tiempo de 5 seg  */
			key_scan();
			
		if(Key == KEY_MEM)		
		  vCalibrate_Scale();

	return;
}


/**
  ******************************************************************************
  * Objetivo: Calcular peso y registrar precio del producto 
  * Parametros entrada:
	* Parametros Salida:
  * Prerequisitos: 
  ***
	*/
unsigned char cRun_Scale(void){
	if(srFlagScale.bOverLoad ==0){//indicador de sobrecarga en la bascula == 0;
		if(srFlagScale.bSourceVoltage ==  SOURCE_ADAPTER){
			LCD_GLASS_Symbols(SYMBOL_Y, 1);
		}else{
			LCD_GLASS_Symbols(SYMBOL_Y, 0);
		}
	}			
	srFlagScale.bFlagWeightNeg = 0;			// Clarea indicador de peso negativo 
    vCalculate_Weight();// calculando el peso en la bascula
//	if( srFlagScale.bErrorReadAdc == 1)	
//		return 0;	
//LCD_GLASS_Float(stScaleParam.fWeightScale, stScaleParam.cWeightDecimal, LCD_PESO);
//return 0;

	if(srFlagScale.bOverLoad ==0){		
	/*	if(srFlagScale.bActiveSaveBattery == 1 && stScaleParam.cZeroTracking == 1 && 
			fWeightScale == 0 && stScaleParam.cSaveBattery == 1 &&
				srFlagScale.bSourceVoltage !=  SOURCE_ADAPTER && srFlagScale.bTara == 0){	
			fSleep_Run();
			 vCalculate_Weight();	
		}*/

		
		// Verifica si esta la opcion fijar precio para mostrar indicador
		if(srFlagScale.bFlagFijarPRecio){	
			LCD_GLASS_Symbols(SYMBOL_X, 1);
		}else{
			LCD_GLASS_Symbols(SYMBOL_X, 0);
		}
		
		// Verfica el tipo de unidades a mostrar
		if(stScaleParam.cUnits == UNIT_KG){	
			LCD_GLASS_Symbols(SYMBOL_KG, 1);
		}else if(stScaleParam.cUnits == UNIT_LB){	
			LCD_GLASS_Symbols(SYMBOL_LB, 1);
		}
		
		if(fWeightScale > -0.001 && fWeightScale < 0.0001){	 
			LCD_GLASS_Symbols(SYMBOL_ZERO, 1);
		}else{	
			LCD_GLASS_Symbols(SYMBOL_ZERO, 0);
			//Restart_Sleep_Time;
		}

	}
		
	srFlagScale.bFlagNeg = 0;	
	
	if(srFlagScale.bShowCountADC){		
		vWeight_Positive();
		
		
	}else{	
		// Verifica si el peso es negativo
		if(fWeightScale < -1){	
			vWeight_Negative();			
		}else{		
			vWeight_Positive();			
		}
	}
	
		if(srFlagScale.bTara){
			if((fWeightLight >= stScaleParam.fValueTara+20*stScaleParam.fFactorCalibrate) && srFlagScale.bBateriaLow == 0){
				if(stScaleParam.cBacklight){OnBackLight;}
				strTimer.iTimerJ = 1;
				srFlagScale.bBacklight_On = 1;	
			}else{
				if(srFlagScale.bSourceVoltage != SOURCE_ADAPTER && strTimer.iTimerJ >= TimerJend){
					OffBackLight;
					srFlagScale.bBacklight_On = 0;	
				}
			}
			
		}else{
			if((fWeightLight >= stScaleParam.fPointZero+20*stScaleParam.fFactorCalibrate) && srFlagScale.bBateriaLow == 0){
					if(stScaleParam.cBacklight){OnBackLight;}
					strTimer.iTimerJ = 1;
					srFlagScale.bBacklight_On = 1;	
				}else{
					if(srFlagScale.bSourceVoltage != SOURCE_ADAPTER && strTimer.iTimerJ >= TimerJend){
						OffBackLight;
						srFlagScale.bBacklight_On = 0;	
				}
			}
		} 
	
	return 0;
}

/**
  ******************************************************************************
  * Objetivo: Funcionamiento normal con peso mayor igual a cero 
  * Parametros entrada:
	* Parametros Salida:
  * Prerequisitos: 
  ***
	*/
void vWeight_Positive(void){
	float fWeightOverload = 0;
	
	float fOverloadUnit =  (float)stScaleParam.iCapacity + 
					((float)(stScaleParam.iDivisionMinima)/(float)(pow(10, (int)stScaleParam.cWeightDecimal)))*9;
	
	fOverloadUnit = fRoundFloat(fOverloadUnit, (int)stScaleParam.cWeightDecimal, stScaleParam.iDivisionMinima-1);
	
	fWeightOverload = stScaleParam.fWeightScaleCount-stScaleParam.fPointZeroInitial;
	
	 
	// Verifca que no exista la condicion de sobrepeso 
	if((fWeightOverload > stScaleParam.fWeightOverload || stScaleParam.fWeightScale > fOverloadUnit)  &&
			srFlagScale.bTopeSobrePeso == 0){
			// if((fWeightOverload > 100.00 || stScaleParam.fWeightScale > 100.00)  &&
			// srFlagScale.bTopeSobrePeso == 100){
			 
		if(srFlagScale.bOverLoad == 0){
			stScaleParam.iCountOverload++;		
			vSaveParamScale(Parameter_Overload);
		}
		
		srFlagScale.bOverLoad = 1;	
		//CCC vGestorBateria();
		
		// Cuando existe sobrepeso lo indica en la LCD
		LCD_GLASS_Symbols(SYMBOL_ALL, 0);
		LCD_GLASS_String("      ", LCD_TOTAL);
		
		if(stScaleParam.cLenguage == ESPANOL){
			LCD_GLASS_String("SOBRE", LCD_PESO);
			LCD_GLASS_String(" PESO", LCD_PRECIO);
		}else{
			LCD_GLASS_String("OVER ", LCD_PESO);
			LCD_GLASS_String(" LOAD", LCD_PRECIO);
		}
		
		strTimer.iTimerA= 1;
		while(strTimer.iTimerA<TimerAend){
			IWDG_KEY_REFRESH;
		}	
		
		if(stScaleParam.fWeightScaleCount-stScaleParam.fPointZeroInitial > stScaleParam.fValueOverload){
			stScaleParam.fValueOverload = stScaleParam.fWeightScaleCount-stScaleParam.fPointZeroInitial;
			vSaveParamScale(Parameter_Overload);
		}
		
		//Restart_Sleep_Time;
		
	}else  {
		srFlagScale.bOverLoad = 0;
		
		// Despliega el peso registrado
		if(stScaleParam.fWeightScale>0.001){		
		LCD_GLASS_Float(stScaleParam.fWeightScale, stScaleParam.cWeightDecimal, LCD_PESO);	
		}else if(stScaleParam.fWeightScale<0.001){
			LCD_GLASS_Float(0, stScaleParam.cWeightDecimal, LCD_PESO);
		}//LCD_GLASS_Float(stScaleParam.fWeightScale, stScaleParam.cWeightDecimal, LCD_PESO);
		//Despliega el valor de lectura del adc en Precio:	
		// Verifica si la opcion fijar precio no esta activada
		//if(!srFlagScale.bFlagFijarPRecio){
			if(stScaleParam.fWeightScale == 0 && fWeightScaleBefore > 0){
				stScaleParam.fPrice_Unit = 0;
				stScaleParam.cNumberDecimalPrice = 0;
				srFlagScale.bDotDecimalPrice = 0;
				stScaleParam.cNumberDecimalPrice = 0;
			}
		//}
		
		/*if(stScaleParam.fWeightScale != fWeightScaleBefore){
			//Auto_Off_On_15m;
			//Restart_Sleep_Time;
		}*/
			
		fWeightScaleBefore = stScaleParam.fWeightScale;
		
		// Muestra el precio x unidad del producto
		if(stScaleParam.fWeightScale == 0 && srFlagScale.bFlagFijarPRecio == 0 && srFlagScale.bPrecioCero == 0){
			srFlagScale.bPrecioCero = 1;
		}
		
		if(stScaleParam.cPuntoDecimalPrecio < 3){
			LCD_GLASS_Float(stScaleParam.fPrice_Unit, stScaleParam.cPuntoDecimalPrecio, LCD_PRECIO);
		}else{
			LCD_GLASS_Float(stScaleParam.fPrice_Unit, stScaleParam.cNumberDecimalPrice, LCD_PRECIO);
		}
		if(stScaleParam.fWeightScale >0.001){
		stScaleParam.fTotal_Venta = stScaleParam.fPrice_Unit * stScaleParam.fWeightScale;
		}
		else{
			stScaleParam.fTotal_Venta = 0;
		}			
         stScaleParam.fTotal_Venta = fRoundFloat(stScaleParam.fTotal_Venta, 
		   stScaleParam.cPuntoDecimalTotal, stScaleParam.cValorRedondeoCifraTotal);
		
		//Verifica si el valor del total excede el limite a mostrar en la LCD
		if(stScaleParam.fTotal_Venta > (999999 / (float)(pow(10,stScaleParam.cPuntoDecimalTotal)))){
			LCD_GLASS_String("------", LCD_TOTAL);
		}else{
			if(stScaleParam.fTotal_Venta > 0){
				LCD_GLASS_Float(stScaleParam.fTotal_Venta, stScaleParam.cPuntoDecimalTotal, LCD_TOTAL);
			}else{
				LCD_GLASS_Float(0, 0, LCD_TOTAL);
			}
		}
	}
}

/**
  ******************************************************************************
  * Objetivo: Bloqueo del sistema por peso negativo 
  * Parametros entrada:
	* Parametros Salida:
  * Prerequisitos: 
  ***
	*/
void vWeight_Negative(void){
	
	srFlagScale.bFlagNeg = 1;
	srFlagScale.bOverLoad = 0;
	
	//En caso de un peso negativo verifica que no sea menor al peso tarado
	if((srFlagScale.bTara == 1) && ((-1)*(stScaleParam.fWeightScale) <= fCuentasToPeso(stScaleParam.fValueTara))){		
		LCD_GLASS_Float(stScaleParam.fWeightScale, (int)stScaleParam.cWeightDecimal, LCD_PESO);
	}else{
		LCD_GLASS_String("-----", LCD_PESO);
	}
	
	if(srFlagScale.bFlagFijarPRecio){
		LCD_GLASS_String("------", LCD_TOTAL);
	}else{
		stScaleParam.fPrice_Unit = 0;
		stScaleParam.cNumberDecimalPrice = 0;
		stScaleParam.fTotal_Venta = 0;
		LCD_GLASS_String("     0", LCD_TOTAL);
	}
		
	if(stScaleParam.cPuntoDecimalPrecio < 3){
		LCD_GLASS_Float(stScaleParam.fPrice_Unit, stScaleParam.cPuntoDecimalPrecio, LCD_PRECIO);
	}else{
		LCD_GLASS_Float(stScaleParam.fPrice_Unit, stScaleParam.cNumberDecimalPrice, LCD_PRECIO);
	}
	
	// Elimina el indicador de cero al presentar peso negativo 
	LCD_GLASS_Symbols(SYMBOL_ZERO, 0);			
}

/**
  ******************************************************************************
  * Objetivo: Calcular el peso.
  * Parametros entrada:
	* Parametros Salida:
  * Prerequisitos: 
  ***
	*/

void vCalculate_Weight (void){
/***************
Se toman 3 lecturas para garantizar el peso al realizar las sumas c/precio fijo, 
si no hay precio fijo realiza solo una lectura de forma natural.
******************/	
	if(srFlagScale.bFlagFijarPRecio){	
		fRead_Adc(5);
		fRead_Adc(5);			
	}
   	fWeightScale = fRead_Adc(5);
	//LCD_GLASS_Float(fWeightScale,0, LCD_TOTAL);
	
/*	if( srFlagScale.bErrorReadAdc == 1)	{
		LCD_GLASS_String(" -E- ", LCD_PESO);
		return;
	}*/
	
	// Si esta la funcion ZeroTracking la ejecuta
	if(stScaleParam.cZeroTracking == 1 && srFlagScale.bTara == 0){
		vGetZeroTracking(fWeightScale);
	}
	
	stScaleParam.fWeightScaleCount = fWeightScale;
	
	fWeightLight = fWeightScale;
	
	// Calcula peso bruto (numero de cuentas ADC del peso)
	fWeightScale -= stScaleParam.fPointZero;	
	stScaleParam.fWeightScaleTara = fWeightScale;
		
	if(srFlagScale.bShowCountADC == 1){
		fWeightScale = fCuentasToPeso(stScaleParam.fWeightScaleCount);
	}else{
			// Verifica si esta la opcion de tara activada para mostrar indicador NET 
		if(srFlagScale.bTara == 1){
			fWeightLight = fWeightScale; 
			fWeightScale -= stScaleParam.fValueTara;			
			if(srFlagScale.bOverLoad ==0){
				LCD_GLASS_Symbols(SYMBOL_NET, 1);
			}
		}		
		fWeightScale = fCuentasToPeso(fWeightScale);		
	}
	
	if(fWeightScale < 0 && srFlagScale.bShowCountADC == 0){
		srFlagScale.bFlagWeightNeg = 1;
	}else if(fWeightScale == 0){
		srFlagScale.bFlagWeightNeg = 0;
	}
	
	stScaleParam.fWeightScale = fWeightScale;
}

/**
  ******************************************************************************
  * Objetivo: Redondea numero flotante.
  * Parametros entrada: Ninguno
	* Parametros Salida: Ninguno
  ******************************************************************************
  */
float fCuentasToPeso(float fCountADC){
	
	float fAuxFactorC = 0;
	float fPesoFinal = 0; 
	float fPesoAux = 0;
	long iAuxPeso = 0;
	float arfFactorMult = 0;
	char cFlagNeg = 0;
	
	if(fCountADC < 0){
		cFlagNeg = 1;
		fCountADC *= (-1);
	}
	
	if(stScaleParam.cMultirango == 0){
		arfFactorMult = (float)(stScaleParam.iDivisionMinima)/pow(10,(int)stScaleParam.cWeightDecimal);
		fPesoAux = fCountADC/stScaleParam.fFactorCalibrate;	
	}else{
		if (fCountADC <= stScaleParam.fCapacityCali*0.10){
			fAuxFactorC = 0;
			fAuxFactorC = (float)(stScaleParam.iDivisionMenorMenor)/(float)(stScaleParam.iDivisionMinima);
			arfFactorMult = (float)(stScaleParam.iDivisionMenorMenor)/pow(10,(int)stScaleParam.cWeightDecimal);
			fPesoAux = fCountADC/(stScaleParam.fFactorCalibrate*fAuxFactorC);			
		}		
		else if (fCountADC <= stScaleParam.fCapacityCali*0.20){
			fAuxFactorC = (float)(stScaleParam.iDivisionMenor)/(float)(stScaleParam.iDivisionMinima);
			arfFactorMult = (float)(stScaleParam.iDivisionMenor)/pow(10,(int)stScaleParam.cWeightDecimal);
			fPesoAux = fCountADC/(stScaleParam.fFactorCalibrate*fAuxFactorC);
		}	
		else{
			arfFactorMult = (float)(stScaleParam.iDivisionMinima)/pow(10,(int)stScaleParam.cWeightDecimal);
			fPesoAux = fCountADC/stScaleParam.fFactorCalibrate;
		}
	}
	
	iAuxPeso = (long)fPesoAux;	
	
	fPesoAux -= (float)iAuxPeso;
	iAuxPeso += (long)(fPesoAux*2);
	fPesoFinal = (float)iAuxPeso*arfFactorMult;
	
	if(cFlagNeg){
		fPesoFinal *= (-1);
	}
	
	return fPesoFinal;
}

/**
  ******************************************************************************
  * Objective: To Reduce the power consumption of the load cell.
  ******************************************************************************
	*/
float fSleep_Run(void){
float fValueReturn = 0;	
/*	float fWeightScale = 0;
	
	float fLimite = 0;
	int i = 0 ;	
	unsigned char cFlag_Continue = 1;	//Flag para 
	
	//Variable para rotar el mensaje
	unsigned char strMsgSaveBatt[16];
	unsigned char strSave[6] = {0,0,0,0,0,0};
	unsigned char strBatt[7] = {0,0,0,0,0,0,0};
	unsigned char cDataTemp = 0;
	unsigned char cDataTemp1 = 0;
	unsigned char bStatus_Battery = 0;

	
	strcpy(strMsgSaveBatt, "---------- ");
	
	fLimite = fCuentasToPeso(stScaleParam.fFactorCalibrate);
	
	Time_Sleep;	//Reinicia el contador para realizar lectura del ADC
	
	for(i=0; i<11; i++){
		if(i<5){
			strSave[i] = strMsgSaveBatt[i];
		}else if(i<11){
			strBatt[i-5] = strMsgSaveBatt[i];
		}
	}
	
	OffBackLight;//GPIO_ResetBits(GPIOA, BACK_LIGHT);	//Off BackLight
				
	LCD_GLASS_Symbols(SYMBOL_ALL, 0);
	LCD_GLASS_String("-----", LCD_PRECIO);
	LCD_GLASS_String("------", LCD_TOTAL);
	LCD_GLASS_Float(0, 0, LCD_PESO);
	
	while(cFlag_Continue){
		//Verifica por que salio del Wait
		if(srFlagScale.bScaleOn == 0){ //Si se oprimio la tecla On
			cFlag_Continue = 0;
		}else if(strTimer.iTimerB<TimerBend){ //Si ya pasaron 3 segundos
			
			fWeightScale = fRead_Adc(0);
			
			// Si esta la funcion ZeroTracking la ejecuta
			if(stScaleParam.cZeroTracking == 1 && srFlagScale.bTara == 0){
				vGetZeroTracking(fWeightScale);
			}
			
			stScaleParam.fWeightScaleCount = fWeightScale;
			
			// Calcula peso bruto (numero de cuentas ADC del peso)
			fWeightScale -= stScaleParam.fPointZero;
			fValueReturn = fWeightScale;
			fWeightScale = fCuentasToPeso(fWeightScale);
			
			if(fWeightScale >= fLimite || fWeightScale < (-1)*(fLimite)){
				cFlag_Continue = 0;
			}else if(vActionKey() != KEY_NULL){
				cFlag_Continue = 0;
				vBeep_Key();
			}				
			
			if(cFlag_Continue){				
				
				//Verifica si ya esta conectado el elimnador para salir de bajo consumo
				bStatus_Battery = uGet_Status_Volt();
				
				if(bStatus_Battery != SOURCE_ADAPTER){ 
					vGestorBateria();
				}else{
					cFlag_Continue = 0;
				}
				
				if(srFlagScale.bMsgBatteryLow == 0){
					//Muestra animacion de lineas 
					
					cDataTemp = strMsgSaveBatt[0];
					
					for(i=0; i<14; i++){
						strMsgSaveBatt[i] = strMsgSaveBatt[i+1];
					}
					
					strMsgSaveBatt[10] = cDataTemp;
					
					for(i=0; i<11; i++){
						if(i<5){
							strSave[i] = strMsgSaveBatt[i];
						}else if(i<11){
							strBatt[i-5] = strMsgSaveBatt[i];
						}
					}
					
					LCD_GLASS_String("    0", LCD_PESO);
					LCD_GLASS_String(strSave, LCD_PRECIO);
					LCD_GLASS_String(strBatt, LCD_TOTAL);
				}
				
				Time_Sleep;
			}
		}else if(vActionKey() != KEY_NULL){
			cFlag_Continue = 0;
			vBeep_Key();
		}
		
		if(srFlagScale.bScaleOn == 0){
			cFlag_Continue = 0;
		}
		
		if(srFlagScale.cAuto_Off_Time == 1 && stScaleParam.cTimeAutooff == 1){
			srFlagScale.bScaleOn = 0;
			cFlag_Continue = 0;
		}
	}	
	
	if(srFlagScale.bBacklight_On){
		if(stScaleParam.cBacklight){OnBackLight;}
	}
	
///////CCC///////CCC	CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);
	//Restart_Sleep_Time;
	//Auto_Off_On_15m;
	*/
	return fValueReturn;
}

/**
  ******************************************************************************
  * Objetivo: Redondea numero flotante.
  * Parametros entrada: Ninguno
	* Parametros Salida: Ninguno
  ******************************************************************************
  */
float fRoundFloat(float fNumber, unsigned char cDecimal, int cRedondeo){
	
	float fAuxDecimal = 0;
	float fAuxValue = 0;
	long iAuxValue = 0;

	fAuxDecimal = (float)(cRedondeo)/(float)(pow(10, (int)cDecimal));
	fAuxValue =  fNumber/fAuxDecimal;	
	iAuxValue = (long)fAuxValue;
	
	fAuxValue -= (float)iAuxValue;
	iAuxValue += (long)(fAuxValue*2);
	fNumber = (float)(iAuxValue)*fAuxDecimal;

	return fNumber;
}
