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
#include "./dvr_battery.h"
#include "./customer/dvr_registradora.h"
#include "./customer/dvr_inicio.h"
//usr_dbg.h"
//extern bool KeyPressed;

code unsigned char cPASS_BORRAR_VT[3] = {KEY_C, KEY_C};
//float fValor_Articulo_Sin_Peso = 0;

#define delaytimeMS 6000
//void vFinalizar_Venta(void);
//void vCalcular_Cambio(void);


/**
  ******************************************************************************
  * Objetivo: Agregar articulos con peso al total de la venta. Visualiza ultimo articulo y acumulado
  * Parametros entrada: Ninguno.
	* Parametros Salida: Ninguno.
  * Prerequisitos: 
  ******************************************************************************
  */
void vAdd_Articulos(float fPrecio_Articulo){
	unsigned char cNumber_Articulos_Venta[6] = { 0 };
	unsigned char bFlagShowLowBat = 0;
	unsigned char bFlagShowInfo = 0;
	unsigned char i = 0;
	LCD_GLASS_Clear();
	if ((stScaleParam.fWeightScale > 0 || srFlagScale.bAdd_Producto_Sin_Peso == 1) && fPrecio_Articulo > 0){	
		
		stScaleParam.iNumber_Articulos_Venta++;
		stScaleParam.fTotal_Venta_Articulos = stScaleParam.fTotal_Venta_Articulos + fPrecio_Articulo;		

		sprintf(cNumber_Articulos_Venta, "%d", stScaleParam.iNumber_Articulos_Venta);
		//LCD_GLASS_Clear();
		
		if(stScaleParam.fTotal_Venta_Articulos > (999999/(float)(pow(10,stScaleParam.cPuntoDecimalTotal)))){ 
				LCD_GLASS_String("------", LCD_PESO);
				LCD_GLASS_String("------", LCD_PRECIO);
				LCD_GLASS_String("ERROR", LCD_TOTAL);
				key_scan();

				DelayWithKey(delaytimeMS);
			}

		else{
				if(stScaleParam.cLenguage == ESPANOL){
					LCD_GLASS_String("ART.  ", LCD_PESO);
				}else{
					LCD_GLASS_String("ITENN", LCD_PESO);
				}
				

				LCD_GLASS_String(cNumber_Articulos_Venta, LCD_PRECIO);
				LCD_GLASS_Float(stScaleParam.fTotal_Venta_Articulos, stScaleParam.cPuntoDecimalTotal, LCD_TOTAL);
				srFlagScale.bPlsUnload_Enable = 0;
				key_scan();
	/*
				if(bFlagShowLowBat == 1){
					bFlagShowLowBat = 0;
					bFlagShowInfo = 0;
					}
	*/
				DelayWithKey(delaytimeMS);

			}

		}

	else{
		if(stScaleParam.cLenguage == ESPANOL){
			LCD_GLASS_String("ART.  ", LCD_PESO);
		}else{
			LCD_GLASS_String("ITENN", LCD_PESO);
		}
		sprintf(cNumber_Articulos_Venta, "%d", stScaleParam.iNumber_Articulos_Venta);
		LCD_GLASS_String(cNumber_Articulos_Venta, LCD_PRECIO);
		LCD_GLASS_Float(stScaleParam.fTotal_Venta_Articulos, stScaleParam.cPuntoDecimalTotal, LCD_TOTAL);
		srFlagScale.bPlsUnload_Enable = 0;
		//key_scan();
	/* 
		if(bFlagShowLowBat == 1){
				bFlagShowLowBat = 0;
				bFlagShowInfo = 0;
				} */
		DelayWithKey(delaytimeMS);
	}
	srFlagScale.bAdd_Producto_Sin_Peso=0;
}

//#################################################################################
/**
  ******************************************************************************
  * Objetivo: Calcular el cambio de la venta
  * Parametros entrada:
		fVenta_Total -> Valor total
	* Parametros Salida:
  * Prerequisitos: 
  ***
	*/ 

void vCalcular_Cambio(void){	
	unsigned char cCaracter;
	float fValor_Cliente = 0;
	float fVenta_Total = 0;
	unsigned char bFlagShowLowBat = 0;
	unsigned char bFlagShowInfo = 0;
	unsigned char i = 0;

	fVenta_Total = stScaleParam.fTotal_Venta_Articulos;

	stScaleParam.fPrice_Unit = 0;
	stScaleParam.cNumberDecimalPrice = 0;
	srFlagScale.bDotDecimalPrice = 0;

	LCD_GLASS_Clear();
	
		/* Captura el valor entregado por el cliente */

	do{		
		//vGestorBateria();
		

		if(bFlagShowInfo == 0){
			bFlagShowInfo = 1;
			
			switch(stScaleParam.cMoneda){
							
				case MONEDA_DLLS:
					LCD_GLASS_String("DLLS ", LCD_PESO);
					break;
					
				case MONEDA_PESOS:
					LCD_GLASS_String("PESOS", LCD_PESO);
					break;
					
				case MONEDA_EUROS:
					LCD_GLASS_String("EUROS", LCD_PESO);
					break;
					
				case MONEDA_BSF:
					LCD_GLASS_String("  BSF", LCD_PESO);
					break;
				
				default:
					LCD_GLASS_String("PESOS", LCD_PESO);
					break;
			}
			
			LCD_GLASS_String("     ", LCD_PRECIO);
			
			if(stScaleParam.cPuntoDecimalPrecio < 3){
				LCD_GLASS_Float(fValor_Cliente, stScaleParam.cPuntoDecimalPrecio, LCD_TOTAL);
			}else{
				LCD_GLASS_Float(fValor_Cliente, stScaleParam.cNumberDecimalPrice, LCD_TOTAL);
			}
		}

		key_scan();
		//if(Key != KEY_NULL)					
		cCaracter = Key;	

		//if(srFlagScale.bMsgBatteryLow == 0){		
			if(cCaracter >= '0' && cCaracter <= '9'){
				
				if(srFlagScale.bDotDecimalPrice){
					if(stScaleParam.cNumberDecimalPrice < stScaleParam.cPuntoDecimalPrecio - 1){
						stScaleParam.cNumberDecimalPrice++;
					}else{
						fValor_Cliente = 0;
						srFlagScale.bDotDecimalPrice = 0;
						stScaleParam.cNumberDecimalPrice = 0;
					}
				}else{
					stScaleParam.cNumberDecimalPrice = 0;
				}
			
				// if(stScaleParam.cPuntoDecimalPrecio < 3){
					
				// 	fValor_Cliente = vCapture_Valor_Test(cCaracter, stScaleParam.cPuntoDecimalPrecio, fValor_Cliente);
				// 	fValor_Cliente /= (float)(pow(10,stScaleParam.cPuntoDecimalPrecio));
					
				// }else{
				 	vCapture_Valor((float *)&fValor_Cliente, cCaracter - '0', 6, stScaleParam.cNumberDecimalPrice);
				// }

				if(stScaleParam.cPuntoDecimalPrecio < 3){
					LCD_GLASS_Float(fValor_Cliente, stScaleParam.cPuntoDecimalPrecio, LCD_TOTAL);
				}else{
					LCD_GLASS_Float(fValor_Cliente, stScaleParam.cNumberDecimalPrice, LCD_TOTAL);
				}
				Key = KEY_NULL;

			}else if(Key == KEY_PUNTO){				
				
				if(stScaleParam.cNumberDecimalPrice == 0 && stScaleParam.cPuntoDecimalPrecio > 2){
					srFlagScale.bDotDecimalPrice = 1;
				}
				Key = KEY_NULL;
				
			}else if(Key == KEY_C ){
				if(fValor_Cliente == 0 && srFlagScale.bDotDecimalPrice == 0 && stScaleParam.cNumberDecimalPrice == 0)
					return;
				
				fValor_Cliente = 0;
				srFlagScale.bDotDecimalPrice = 0;
				stScaleParam.cNumberDecimalPrice = 0;
				
				if(stScaleParam.cPuntoDecimalPrecio < 3){
					LCD_GLASS_Float(fValor_Cliente, stScaleParam.cPuntoDecimalPrecio, LCD_TOTAL);
				}else{
					LCD_GLASS_Float(fValor_Cliente, stScaleParam.cNumberDecimalPrice, LCD_TOTAL);
				}
		
				Key  = KEY_NULL;
			
			}else if(bFlagShowLowBat == 1){
				bFlagShowLowBat = 0;
				bFlagShowInfo = 0;
			}			
		//}else{
		//	bFlagShowLowBat = 1;
		//}			
		
	}while(Key == KEY_NULL);
	// Calcula la diferencia entre el dinero que pago el cliente con el precio		total del producto pesado 
	
	bFlagShowInfo = 0;
	bFlagShowLowBat = 0;
	
	if((fValor_Cliente - fVenta_Total) >= 0 && Key  == KEY_CHG ){
		fValor_Cliente -= fRoundFloat(fVenta_Total, 
			stScaleParam.cPuntoDecimalTotal, stScaleParam.cValorcRedondeoCifraVentaTotal);
		
		if(fValor_Cliente < 10000){
			fValor_Cliente = fRoundFloat(fValor_Cliente, 
				stScaleParam.cPuntoDecimalTotal, stScaleParam.cValorcRedondeoCifraVentaTotal);
		}
		
		Key  = KEY_NULL;		
		while(Key  == KEY_NULL){ 		
			//vGestorBateria();
			if(bFlagShowInfo == 0){
				bFlagShowInfo = 1;
				
				if(stScaleParam.cLenguage){
					LCD_GLASS_String("  CA. ", LCD_PRECIO);
				}else{
					LCD_GLASS_String("CHG", LCD_PRECIO);
				}
				
				switch(stScaleParam.cPuntoDecimalTotal){
					case 0:
					
						if(fValor_Cliente > 0){
							LCD_GLASS_Float(fValor_Cliente, stScaleParam.cPuntoDecimalTotal, LCD_TOTAL);
						}else{
							LCD_GLASS_Float(0, 0, LCD_TOTAL);
						}						
						break;
					
					case 1:		
								
						if(fValor_Cliente > 99999.9){
							LCD_GLASS_Float(fValor_Cliente, 0, LCD_TOTAL);
						}else{
							if(fValor_Cliente > 0){
								LCD_GLASS_Float(fValor_Cliente, stScaleParam.cPuntoDecimalTotal, LCD_TOTAL);
							}else{
								LCD_GLASS_Float(0, 0, LCD_TOTAL);
							}
						}						
						break;
					
					case 2:
						if(fValor_Cliente > 9999.99){
							if(fValor_Cliente > 99999.9){
								
								LCD_GLASS_Float(fValor_Cliente, 0, LCD_TOTAL);								
							}else{
								
								LCD_GLASS_Float(fValor_Cliente, 1, LCD_TOTAL);
							}
						}else{
							if(fValor_Cliente > 0){
								
								LCD_GLASS_Float(fValor_Cliente, stScaleParam.cPuntoDecimalTotal, LCD_TOTAL);
							}else{
								LCD_GLASS_Float(0, 0, LCD_TOTAL);
							}
						}						
						break;
				}
	
				if(srFlagScale.bFlagFijarPRecio && srFlagScale.bAdd_Articulos){
					while(0);	
				}
				else{
					stScaleParam.fPrice_Unit = 0;
				}
			}	

			//if(srFlagScale.bMsgBatteryLow == 0){
				Key=KEY_NULL;
				key_scan();
				
				if(bFlagShowLowBat == 1){				
					bFlagShowLowBat = 0;
					bFlagShowInfo = 0;
				}	
			//}else{
			//	bFlagShowLowBat = 1;
			//}
	
		
		}
		
		if(Key == KEY_CHG ){
			vFinalizar_Venta();
		}
		
	}
	
	else if((fValor_Cliente - fVenta_Total) < 0 && Key == KEY_CHG ){

		Key = KEY_NULL;
		
		while(Key == KEY_NULL){ 
		
			//vGestorBateria();
			
			if(stScaleParam.cLenguage){
				LCD_GLASS_String("  CA. ", LCD_PRECIO);
			}else{
				LCD_GLASS_String("CHG", LCD_PRECIO);
			}
		
			LCD_GLASS_Float(0, 0, LCD_TOTAL);
			
			key_scan();
			
			if(Key != KEY_NULL )
				break;							
		}
		
		vFinalizar_Venta();
	}

	srFlagScale.bDotDecimalPrice = 0;
	stScaleParam.cNumberDecimalPrice = 0;
}

/**
  ******************************************************************************
  * Objetivo: Finalizar una venta
  * Parametros entrada: Ninguno.
	* Parametros Salida: Ninguno.
  * Prerequisitos: 
  ******************************************************************************
  */
void vFinalizar_Venta(void){
	stScaleParam.fTotal_Venta_Articulos = fRoundFloat(stScaleParam.fTotal_Venta_Articulos, 
							stScaleParam.cPuntoDecimalTotal, stScaleParam.cValorRedondeoCifraTotal);
							
	stScaleParam.fVenta_Total_Scale += stScaleParam.fTotal_Venta_Articulos;
	
	if(stScaleParam.fVenta_Total_Scale > 999999.99){
		stScaleParam.fVenta_Total_Scale = 0;
		stScaleParam.fVenta_Total_Scale += stScaleParam.fTotal_Venta_Articulos;
	}
	
	stScaleParam.fTotal_Venta_Articulos = 0;
	stScaleParam.iNumber_Articulos_Venta = 0;
	strTimer.iTimerA = 1;
	while(strTimer.iTimerA < TimerAend){
		IWDG_KEY_REFRESH;

	}	
	vSaveParamScale(Parameter_Register);
	
	LCD_GLASS_Clear();
	if(stScaleParam.cLenguage == ESPANOL){
		LCD_GLASS_String("ART  ", LCD_PESO);
	}else{
		LCD_GLASS_String("ITENN", LCD_PESO);
	}


	LCD_GLASS_String("0", LCD_PRECIO);
	LCD_GLASS_Float(0, stScaleParam.cPuntoDecimalTotal, LCD_TOTAL);
	strTimer.iTimerE= 1;	
	while(strTimer.iTimerE < 3000){
		IWDG_KEY_REFRESH;
	} 
}

/**
  ******************************************************************************
  * Objetivo: Mostrar el total de las ventas.
  * Parametros entrada: Ninguno.
	* Parametros Salida: Ninguno.
  * Prerequisitos: 
  ******************************************************************************
  */
void vMostrar_Venta_Total(void){
	
	unsigned char xdata cText_Venta_Total[13] = "           ";
	unsigned char xdata cText_Precio[6] = "     ";
	unsigned char xdata cText_Total[8] = "      ";
	unsigned char xdata cCode_Borrar_VT[3] = "  ";
	int i;
	
	//LCD_GLASS_Clear();
	
	if(stScaleParam.cLenguage == ESPANOL){
		LCD_GLASS_String("TOTAL", LCD_PESO);
	}else{
		LCD_GLASS_String("   TS", LCD_PESO);
	}
	
	stScaleParam.fVenta_Total_Scale = fRoundFloat(stScaleParam.fVenta_Total_Scale, 
	stScaleParam.cPuntoDecimalTotal, stScaleParam.cValorcRedondeoCifraVentaTotal);
	
	if(stScaleParam.cPuntoDecimalTotal == 0){
		sprintf(cText_Venta_Total, "%10.0f", stScaleParam.fVenta_Total_Scale);
	}else if(stScaleParam.cPuntoDecimalTotal == 1){
		sprintf(cText_Venta_Total, "%11.1f", stScaleParam.fVenta_Total_Scale);
	}else if(stScaleParam.cPuntoDecimalTotal == 2){
		sprintf(cText_Venta_Total, "%11.2f", stScaleParam.fVenta_Total_Scale);
	}
		for(i=0; i<5; i++){
		cText_Precio[i] = cText_Venta_Total[i];
	}
	
	for(i=5; i<12; i++){
		cText_Total[i-5] = cText_Venta_Total[i];
	}
	
	LCD_GLASS_String(cText_Total, LCD_TOTAL);
	LCD_GLASS_Dot(stScaleParam.cPuntoDecimalTotal, LCD_TOTAL, 1);
	LCD_GLASS_String(cText_Precio, LCD_TOTAL);
	
	i = 0; 
	strTimer.iTimerE = 1;
	while(strTimer.iTimerE < TimerEend){
		
		IWDG_KEY_REFRESH;
			key_scan();
		
		if(Key  != KEY_NULL){
		}
		
		if((Key  == KEY_C ||Key  == KEY_CHG) && i < 2){
			cCode_Borrar_VT[i] =Key ;
			i++;
		}else if(i == 2){
			
			if(strcmp(cCode_Borrar_VT, cPASS_BORRAR_VT) == 0){
				stScaleParam.fVenta_Total_Scale = 0;
				vSaveParamScale(Parameter_Register);
				strTimer.iTimerE= 1;
				while(strTimer.iTimerE < TimerEend){
					LCD_GLASS_String("RESET", LCD_PESO);
					LCD_GLASS_String("TOTAL", LCD_PRECIO);
					LCD_GLASS_String("      ", LCD_TOTAL);
				}
				i++;
			}
		} 
	}
}