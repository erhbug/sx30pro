//*************************************************************************//
//* Name				:   display_cal.c
//* Vision				:	V-Nov07_12
//* Project				:   All project
//* Function			:   Solidic ���ӳ� �Դ�����ļ�
//* $Author				:   superc
//*
//* Copyright 2012 by solidic
//*
//* Recode:
//*						1: 2012-11-07  First Creat By Superc
//*
//*
//*
//*************************************************************************//
#include <REG52.H>
//#include <math.h>
//**************************************************************************
// -- ȫ��ͷ�ļ� ���й��� ---- ���汾�޸��ļ�����䶯 ---  �����޸� ����ļ�
//**************************************************************************
#include "./_solidic/head_file_version.h"//comentar
#include "./customer/keyboard.h"
#include "./_scale/dvr_def.h"
#include "./customer/beeper.h"
#include "./customer/dvr_plu.h"
#include "./customer/dvr_registradora.h"
#include <math.h>
#include <stdio.h>

unsigned char idata Key;
unsigned char idata LastKey;
unsigned char idata KeyState=0;

unsigned int iIndex_Address_Plus = 0;
//unsigned char cValue_Precio[5] = "     ";


volatile void delay_ms(unsigned int num)
{ 	
	unsigned int i;
    unsigned char j;
  	for(i= 0;i<num;i++){	//(SDI5219) ??1ms
	IWDG_KEY_REFRESH; 
		for(j=0;j<200;j++){
			j++;j--;
			}
    } 

}

void key_scan(void) {
  unsigned char k = 0;
  Key = 0;

  for (k = 0; k < 5; k++) {
    KEY_D0 = 1;
    KEY_D1 = 1;
    KEY_D2 = 1;
    KEY_D3 = 1;
    KEY_D4 = 1;
    if (k == 0) KEY_D0 = 0;
    if (k == 1) KEY_D1 = 0;
    if (k == 2) KEY_D2 = 0;
    if (k == 3) KEY_D3 = 0;
    if (k == 4) KEY_D4 = 0;
    delay_ms(1);
    if (KEY_K0 != 1) Key = 1 + (k * 4);
    if (KEY_K1 != 1) Key = 2 + (k * 4);
    if (KEY_K2 != 1) Key = 3 + (k * 4);
    if (KEY_K3 != 1) Key = 4 + (k * 4);
  }

if(Key==0)Key=KEY_NULL;
if(Key==1)Key=KEY_CERO;
if(Key==2)Key=KEY_TARA;
if(Key==3)Key=KEY_M1;
if(Key==4)Key=KEY_RCL;
if(Key==5)Key=KEY_CHG;
if(Key==6)Key=KEY_MAS;
if(Key==7)Key=KEY_M2;
if(Key==8)Key=KEY_MEM;
if(Key==9)Key=KEY_7;
if(Key==10)Key=KEY_4;
if(Key==11)Key=KEY_1;
if(Key==12)Key=KEY_C;
if(Key==13)Key=KEY_8;
if(Key==14)Key=KEY_5;
if(Key==15)Key=KEY_2;
if(Key==16)Key=KEY_0;
if(Key==17)Key=KEY_9;
if(Key==18)Key=KEY_6;
if(Key==19)Key=KEY_3;
if(Key==20)Key=KEY_PUNTO;



  if (LastKey == 0 && Key != 0) {
    KeyState = PRESS; //se presiono
    for (k = 0; k < 10; k++);//peque�o delay
  } else if (LastKey == Key && Key != 0) {
    KeyState = PRESSED; //se mantiene presionado
  } else if (LastKey != 0 && Key == 0) {
    KeyState = RELEASE; //se solto
	for (k = 0; k < 10; k++);//peque�o delay
  } else if (LastKey == 0 && Key == 0) {
    KeyState = 0; //no se ha presionado una tecla	
  }
  LastKey = Key;
  if(Key!=0&&KeyState == PRESS)vBeep_Key();
  IWDG_KEY_REFRESH;
}


void vScan_Key(void){
	int idata iMultiplicador_Producto = 0;
	float idata fMultiplicador_Producto = 0;
	float idata fWeightScale = 0;
	unsigned char idata fBeep_Key_Off = 0; 
	
	key_scan();

	Key_Press_Operation:
	
	if( Key != KEY_NULL && KeyState==PRESS ){
		
					
		/* Verifica si el peso es positivo */
		if(srFlagScale.bFlagWeightNeg == 0){
				/* Verifica si la tecla paulsada es un numero */
			if(Key >= '0' && Key <= '9'){
//CCC					srFlagScale.bBL_Lpcr = 0;
					
					if(srFlagScale.bWritePlus == 1 || srFlagScale.bReadPlus == 1){
						
						if(iIndex_Address_Plus < 2){
						
							vCapture_Valor((float *)&stScaleParam.fAddress_Plus, Key - '0', 5, 0);//ARREGLAR
						
							//LCD_GLASS_String("8888", LCD_TOTAL); 
							//delay_ms(10000);

							iIndex_Address_Plus++;
							
							if(iIndex_Address_Plus == 2){
								stScaleParam.fPrice_Unit = fFuncionPlus((int)(stScaleParam.fAddress_Plus), 
									srFlagScale.bReadPlus, srFlagScale.bWritePlus, stScaleParam.fPrice_Unit, stScaleParam.cNumberDecimalPrice);
								iIndex_Address_Plus = 0;
								srFlagScale.bWritePlus = 0;
								srFlagScale.bReadPlus = 0;
							} 
						}
					}else{
						
						if(srFlagScale.bDotDecimalPrice){
							if(stScaleParam.cNumberDecimalPrice < stScaleParam.cPuntoDecimalPrecio - 1){
								stScaleParam.cNumberDecimalPrice++;
							}else{
								stScaleParam.fPrice_Unit = 0;
								srFlagScale.bDotDecimalPrice = 0;
								stScaleParam.cNumberDecimalPrice = 0;
							}
						}else{
							stScaleParam.cNumberDecimalPrice = 0;
						}
							
						if(stScaleParam.cPuntoDecimalPrecio < 3){
								
							stScaleParam.fPrice_Unit = vCapture_Valor_Test(Key, stScaleParam.cPuntoDecimalPrecio, stScaleParam.fPrice_Unit);
							stScaleParam.fPrice_Unit /= (float)(pow(10,stScaleParam.cPuntoDecimalPrecio));
							
						}else{
							vCapture_Valor((float *)&stScaleParam.fPrice_Unit, Key - '0', 5, stScaleParam.cNumberDecimalPrice);
						}
					}
					
					return;
			
			}else if(Key == KEY_PUNTO){	
				if(stScaleParam.cNumberDecimalPrice == 0 && stScaleParam.cPuntoDecimalPrecio > 2){
					srFlagScale.bDotDecimalPrice = 1;
				}
				return;
			}
		}					
			
		if(Key != KEY_M1  && Key != KEY_M2){//if(Value_Key_Press != KEY_M1  && Value_Key_Press != KEY_M2 && Value_Key_Press != KEY_M3 && Value_Key_Press != KEY_M4){
				iIndex_Address_Plus = 0;
				srFlagScale.bWritePlus = 0;
				srFlagScale.bReadPlus = 0;
		}
							
			switch(Key){
				/* Si la tecla presiona fue C borra el valor del precio */
				case KEY_C:
				
					if(srFlagScale.bAdd_Articulos == 1)
						
				
					srFlagScale.bMultiplicar_Producto = 0;
					stScaleParam.iMultiplicador_Producto = 0;
					stScaleParam.fPrice_Unit = 0;
					srFlagScale.bFlagFijarPRecio = 0;
					stScaleParam.cNumberDecimalPrice = 0;
					srFlagScale.bDotDecimalPrice = 0;
					srFlagScale.bAdd_Articulos = 0;
					srFlagScale.bArticulosTotalCero = 0;
					
					vBeep_Key();
					
					break;
				
				/* Si la tecla presiona fue Tara activa/desactiva la tara */
				case KEY_TARA:
					if(srFlagScale.bTopeSobrePeso){
						srFlagScale.bShowCountADC = 1;						
					}else{
						if(stScaleParam.fWeightScale != 0){
							cOnOffModeTara(stScaleParam.fWeightScale);
						}
					}
					
					break;
					
				/* Se asigna la nueva referencia a cero */
				case KEY_CERO:			
						srFlagScale.bShowCountADC = 0;
						if(srFlagScale.bTara == 0 && srFlagScale.bOverLoad == 0){
							vSetZero();
						}
						key_scan();
						break;
					
				case KEY_MEM:
					//CCC srFlagScale.bBL_Lpcr = 1;
					srFlagScale.bWritePlus = 1;
					iIndex_Address_Plus = 0;
					stScaleParam.fAddress_Plus = 0;
					break;
				
				case KEY_RCL:
				strTimer.iTimerE=1;
					while(strTimer.iTimerE < 60000){
						key_scan();
						if(Key != KEY_PUNTO && Key != KEY_NULL){
							strTimer.iTimerE = 60000;
							srFlagScale.bReadPlus = 1;
							iIndex_Address_Plus = 0;
							stScaleParam.fAddress_Plus = 0;
							
						}else if(Key == KEY_PUNTO){
							vMostrar_Venta_Total();
							srFlagScale.bAdd_Articulos = 0;
						}
					}
					//strTimer.cFLag_TimerI_End=0;
					break;
				
				case KEY_M1:
					vActionMemoryPlu(1);				
					break;
				
				case KEY_M2:
					vActionMemoryPlu(2);
					break;
	
	/*			case KEY_M3:
					vActionMemoryPlu(3);
					break;
		
				case KEY_M4:
					vActionMemoryPlu(4);
					break;
					
				case KEY_FOCO:
					if(srFlagScale.bBacklight_On){
						OffBackLight;
					}else{
						if(srFlagScale.bBateriaLow == 0){
						if(stScaleParam.cBacklight){OnBackLight;}
						}
					}
					break;
	*/			
				case KEY_MAS:
			
					if(srFlagScale.bFlagWeightNeg == 0 && (stScaleParam.fTotal_Venta > 0 || stScaleParam.cFormatoImpresion == 2)){
						
						fWeightScale = fStablePoint(1, 0, 1);

						if(srFlagScale.bTara == 1)
							fWeightScale -= stScaleParam.fPointZero;
						else
							fWeightScale -= stScaleParam.fValueTara;		
						
						fWeightScale = fCuentasToPeso(fWeightScale);

						if(srFlagScale.bFlagStable == 1)
							{
								if(srFlagScale.bMultiplicar_Producto !=1)
								{
									stScaleParam.fTotal_Venta = stScaleParam.fPrice_Unit * stScaleParam.fWeightScale;
									stScaleParam.fTotal_Venta = fRoundFloat(stScaleParam.fTotal_Venta, stScaleParam.cPuntoDecimalTotal, stScaleParam.cValorRedondeoCifraTotal);
								}
							}
						else
							break;
						
						if(srFlagScale.bAdd_Producto_Sin_Peso){
							vAdd_Articulos(stScaleParam.fPrice_Unit);
						}else{
							vAdd_Articulos(stScaleParam.fTotal_Venta);
						}
						
						srFlagScale.bMultiplicar_Producto = 0;
						stScaleParam.iMultiplicador_Producto = 0;
						srFlagScale.bAdd_Articulos = 1;
						
						if(!srFlagScale.bFlagFijarPRecio){
							stScaleParam.fPrice_Unit = 0;
							srFlagScale.bDotDecimalPrice = 0;
							stScaleParam.cNumberDecimalPrice = 0;						
						}					
						
						if(srFlagScale.bFlagKeyNumber != KEY_NULL){
							Key = srFlagScale.bFlagKeyNumber;
							fBeep_Key_Off = 1;
							srFlagScale.bFlagKeyNumber = KEY_NULL;
							goto Key_Press_Operation;
						} 
						
					}
					
					break;
					
				/* Si la tecla presiona fue cambio inicia el proceso de calcular el cambio 
					de la venta*/
				case KEY_CHG:
	
					if(srFlagScale.bFlagWeightNeg == 0){
						vCalcular_Cambio();
					}
					break;
				
			}
		}	
}

/**
  ******************************************************************************
  * Objetivo: Asignar valor decimal a tecla pulsada si esta entre 0 y 9.
  * Parametros entrada: Nombre de la tecla.
	* Parametros Salida: Valor decimal de la tecla pulsada.
  * Prerequisitos: 
  ***
	*/
/*unsigned char cConvert_Char(unsigned char Key_Num){
	
	if(Key_Num == KEY_9){
		return '9';
	}else if(Key_Num == KEY_8){
		return '8';
	}else if(Key_Num == KEY_7){
		return '7';
	}else if(Key_Num == KEY_6){
		return '6';
	}else if(Key_Num == KEY_5){
		return '5';
	}else if(Key_Num == KEY_4){
		return '4';
	}else if(Key_Num == KEY_3){
		return '3';
	}else if(Key_Num == KEY_2){
		return '2';
	}else if(Key_Num == KEY_1){
		return '1';
	}else if(Key_Num == KEY_0){
		return '0';
	}else{
		return Key_Num;
	}
}*/

/**
  ******************************************************************************
  * Objetivo: Ejecutar la accion en la memoria de productos.
  * Parametros entrada:
	* Parametros Salida:
  * Prerequisitos: 
  ***
	*/
void vActionMemoryPlu(unsigned char cIndexMemory){
	
	if(!srFlagScale.bWritePlus){
		srFlagScale.bReadPlus = 1;
	}
			
	stScaleParam.fPrice_Unit = fFuncionPlus((int)(cIndexMemory), 
		srFlagScale.bReadPlus, srFlagScale.bWritePlus, stScaleParam.fPrice_Unit, stScaleParam.cNumberDecimalPrice);
	
	iIndex_Address_Plus = 0;
	srFlagScale.bWritePlus = 0;
	srFlagScale.bReadPlus = 0;
}


/**
  ******************************************************************************
  * Objetivo: Ensamblar numero con las teclas pulsadas
  * Parametros entrada:
	* Parametros Salida:
  * Prerequisitos: 
  ***
	*/
float vCapture_Valor_Test(unsigned char fNew_Digit, unsigned char cDecimal_Number, 
		float fValue_Capture){
/*
	
	//long int iValue_Funct = 0;
	long iValue_Funct = 0;
	unsigned char *pText_Valor = cValue_Precio;
	unsigned char i = 0;


	if(fValue_Capture == 0){
		for(i=0; i<5; i++){
			pText_Valor[i] = ' ';
		}
	}

	for(i=0; i<4; i++){
		pText_Valor[i] = pText_Valor[i+1];
	}
	
	pText_Valor[4] = fNew_Digit;
	
	//sscanf(cValue_Precio, "%ld", &iValue_Funct);//QUÉ SE SUPONE QUE HACE ESTO AQUI???!!	
	//Hacer que los valores leídos en el teclado sean el precio del producto

	return (float)(iValue_Funct);
	
*/
	return 0.0;
}


/**
  ******************************************************************************
  * Objetivo: Ensamblar numero con las teclas pulsadas
  * Parametros entrada:
	* Parametros Salida:
  * Prerequisitos: 
  ***
	*/
void vCapture_Valor(float *pfValor, float fNew_Digit, int iNumber_Max_Digit, unsigned char cCounter_Decimal){
	
	float xdata fValor_Comparacion = 0;
	unsigned long iPotencia;
	unsigned long i;

	unsigned int iNumeros = 0;
	unsigned char cText_Valor[10] = {0,0,0,0,0,0,0,0,0,0};
	unsigned char *pText_Valor = cText_Valor;


	if(cCounter_Decimal > 0){
	
		
		if(cCounter_Decimal == 1) nFloatToStr(*(pfValor),5,1,cText_Valor);//sprintf(cText_Valor, "%5.1f", *(pfValor));
		else if(cCounter_Decimal == 2)nFloatToStr(*(pfValor),5,2,cText_Valor);// sprintf(cText_Valor, "%5.2f", *(pfValor));
		
		while(*(pText_Valor)){		
			if(*(pText_Valor) != '.' && *(pText_Valor) != ' '){
				iNumeros++;
			}
			
			pText_Valor++;
		}
	}
		
	if(iNumeros <= iNumber_Max_Digit){

		if(cCounter_Decimal == 0){
			*(pfValor) *= 10;
		}else{
			fNew_Digit /= (pow(10, cCounter_Decimal));		
		}
			
		*(pfValor) += fNew_Digit;
	
		iPotencia = (long int)(pow(10, iNumber_Max_Digit));
		
		for(i=9; i>0; i--){
			fValor_Comparacion = (float)(i*iPotencia);
			if(*(pfValor) >= fValor_Comparacion){
				*(pfValor) -= (float)(i*iPotencia);
				return;
			}
		}
	}else{
		*(pfValor) = 0;
		srFlagScale.bDotDecimalPrice = 0;
		stScaleParam.cNumberDecimalPrice = 0;
	}
}
