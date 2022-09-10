/* Includes ------------------------------------------------------------------*/

/*#include "dvr_def.h"
#include "dvr_HX712.h"
#include "dvr_scale.h"
#include "dvr_keyboard.h"
#include "dvr_lcd.h"

#include "app_cfg.h"
#include "app_keyboard.h"
#include "bsp.h"
*/
#include "./_weight/dvr_HX712.h"
#include <stdio.h>
#include <string.h>
#include <REG52.H>
#include <math.h>
#include "./customer/keyboard.h"

sbit MISO = P0^3;	
sbit SCLK = P0^4;	


//--------------------------------------------------------------------------------
float ValueCount = 0;
int iCountFailRead = 0;
int iCountFailResponse = 0;



unsigned char iSelectFrecuency = 1;
float arfDataFilter_x[10] = {0};
float fBeforeValue_x = 0;
float fAverage_x = 0;
unsigned char iValueOut = 0;

/*
*/
float fFilter_Averaging(unsigned int iActualWeight, unsigned char cFastFill){	
	
	float arfLowestToHighest[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	//////////////////////////////////////////float fThreshold = stScaleParam.fFactorCalibrate[stScaleParam.iUnits]/2;
	float fThreshold = 7.15/2;
	float fData_Vector = 0;
	float *pfData_Filter;
	float fActualWeight = (float)(iActualWeight);
	unsigned char iLenthData_x = 6;		// Longitud de los datos a ordenar, original 6, en prueba 10 
	unsigned char i = 0; 	// Variable para ciclos iterativo 
    unsigned char j = 0;	// Variable para ciclos iterativo 
	
	pfData_Filter = arfDataFilter_x;
	
	if((fActualWeight > fAverage_x + fThreshold || fActualWeight < fAverage_x - fThreshold) && cFastFill == 0){
		if(iValueOut++ > 2){
			iValueOut = 0;	
			for(i=0; i<iLenthData_x-1; i++){
				pfData_Filter[i] = fActualWeight;
			}			
		}else{
			return fAverage_x;
		}
	}else if(cFastFill == 1){	//Llena todo el vector de promedios con el mismo valor
		iValueOut = 0;		
		for(i=0; i<iLenthData_x-1; i++){
			pfData_Filter[i] = iActualWeight;
		}
	}else if(cFastFill == 2){	//Indica que es el inicio para tomar referencia a Zero.
		for(i=0; i<iLenthData_x-1; i++){
			pfData_Filter[i] = pfData_Filter[i+1];
		}
	}else{		// Agrega nuevo valor al vector para promedio		
		iValueOut = 0;		
		for(i=0; i<iLenthData_x-1; i++){
			pfData_Filter[i] = pfData_Filter[i+1];
		}
	}
	
			
	pfData_Filter[iLenthData_x-1] = fActualWeight;
	
	for(i=0; i<iLenthData_x; i++){
		arfLowestToHighest[i] = pfData_Filter[i];
	}
	
	// Se ordenan los datos de menor a mayor por el metodo burbuja 
	for (i = 0; i < iLenthData_x -1 ; i++) {	
		for (j = (i + 1); j < iLenthData_x ; j++) {			
			if (arfLowestToHighest[j] < arfLowestToHighest[i]) {
				fData_Vector = arfLowestToHighest[j];
				arfLowestToHighest[j] = arfLowestToHighest[i];
		 	 	arfLowestToHighest[i] = fData_Vector;
			}
		}	 
	}
	
	fAverage_x = 0;
	
	for(i=1; i<iLenthData_x-1; i++){
		fAverage_x += arfLowestToHighest[i];
	}
	
	fAverage_x /= (float)(iLenthData_x - 2);
	
	fBeforeValue_x = fAverage_x;
	
	return fAverage_x;
}


/*
*/

/*
*/
float fRead_Adc(char cFillFilter){

	
	unsigned char *pWeightByte;
	unsigned int iActualWeight = 0;
	unsigned int iTemp_RA = 0;
	unsigned int tickstart = 0;
	unsigned int tickend = 0;
	unsigned int Timeout = 0;
	unsigned char BufferSPI[10]= {0};



	pWeightByte = (unsigned char *)&iActualWeight;
		
	
	tickend = 0;
	Timeout = 120;
	while(MISO != 0){		
		;
		delay_ms(1);		
		tickend ++;
		
		if(tickend >=  Timeout){
			if(iCountFailResponse++ > 5){
				iCountFailResponse = 5;
//////////////////////////////////////////////////////////////				srFlagScale.bErrorResponseAdc = 1;
			}
			
			return ValueCount;
		}
	}
	tickend = 0;
	while(tickend < 5){
	delay_ms(1);
		tickend ++;

		
		if(MISO != 0){		//Detecta ruido en la lectura
			
			tickend = 0;
			Timeout = 500;

			while(tickend <  Timeout){
        delay_ms(1);		
		tickend ++;
			}
			
			if(iCountFailResponse++ > 2){
				iCountFailResponse = 2;
/////////////////////////////////////////////////////////////////////////////////////////////////////////				srFlagScale.bErrorNoiseAdc = 1;
			}
			
			return ValueCount;
		}
	}
	
	if(iCountFailResponse > 0){
		iCountFailResponse--;
		return ValueCount;
	}
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////srFlagScale.bErrorResponseAdc = 0;
/////////////////////////////////////////////////////////////////////////////////////////////////////////	srFlagScale.bErrorNoiseAdc = 0;
	SPIGPIO_Sync(BufferSPI); //pvr
	//-----------------------------------------------------------	
	
	*(pWeightByte++)= BufferSPI[2];
	*(pWeightByte++)= BufferSPI[1];
	*(pWeightByte++) 	= BufferSPI[0];	
	*(pWeightByte) = 0;
		
	iTemp_RA = iActualWeight >> 7;	// Elimina los 0s generados 
	
	if(iTemp_RA == 65535){
	}else if(iTemp_RA > 65535){
		iTemp_RA = 131072 - iTemp_RA;
		iTemp_RA = 32500 - iTemp_RA;
	}else{
		iTemp_RA += 32500;
	}
	
	ValueCount = fFilter_Averaging(iTemp_RA,cFillFilter);
	
	return ValueCount;
}

void SPIGPIO_Sync(char* buf)
{
	long int lByte=0;
	int i,j;
	
	//24 bits de comunicación
	for(i=0;i<24;i++)
	{
    lByte<<=1;
	SCLK=1;  //for(j=0;j<5;j++);	
//	if(MISO!=0)lByte|=0x00000001;	
	lByte|=(MISO)&0x00000001;												//Lectura en flanco negativo
	SCLK=0; //for(j=0;j<10;j++);
	}
	
	//1 bit de configuración a 10Hz
	for(j=0;j<7;j++);
	SCLK=1;  for(j=0;j<6;j++);
	SCLK=0;  //for(j=0;j<10;j++);

	buf[2]= lByte&0x000000FF;
	buf[1]= (lByte>>8)&0x000000FF;
	buf[0]= (lByte>>16)&0x000000FF;

}
