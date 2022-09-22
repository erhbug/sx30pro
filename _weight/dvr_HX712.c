/* Includes ------------------------------------------------------------------*/
#include "./_weight/dvr_HX712.h"
//#include <stdio.h>
//#include <string.h>
#include <REG52.H>
//#include <math.h>
//#include "./customer/keyboard.h"
//#include "./_display/dvr_lcd_SDI1621.h"

sbit MISO = P0^3;	
sbit SCLK = P0^4;	

float ValueCount = 0.0;

//--------------------------------------------------------------------------------

//int iCountFailRead = 0;
//int iCountFailResponse = 0;
//unsigned char iSelectFrecuency = 1;


/*
*/
float fFilter_Averaging(unsigned long iActualWeight, unsigned char cFastFill){	

float fAverage_x = 0;
float xdata arfDataFilter_x[5] = {0.00};
float xdata arfLowestToHighest[5] = {0.00};
unsigned char xdata iValueOut = 0;	
//    float xdata fThreshold = stScaleParam.fFactorCalibrate[stScaleParam.iUnits]/2;
	float fThreshold = 8.526937/2;
	float xdata fData_Vector = 0;
	float xdata *pfData_Filter;
	float xdata fActualWeight = (float)(iActualWeight);
	unsigned char xdata iLenthData_x = 5;		// Longitud de los datos a ordenar, original 6, en prueba 10 
	unsigned char xdata i = 0; 	// Variable para ciclos iterativo 
    unsigned char xdata j = 0;	// Variable para ciclos iterativo 
	
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
	
	//fBeforeValue_x = fAverage_x;


	
	return fAverage_x;
}


/*
*/
/*
*/
float fRead_Adc(unsigned char cFillFilter){
	unsigned long iTemp_RA = 0;

	if(MISO!=0)return ValueCount;
    
  //iTemp_RA=123456789;	sprintf(txt,"%ld   ",iTemp_RA);  LCD_GLASS_String(txt,LCD_PESO); delay_ms(5000);
	
	ReadHX712(&iTemp_RA); 

  //sprintf(txt,"%ld   ",iTemp_RA);  LCD_GLASS_String(txt,LCD_PESO); delay_ms(5000);
 iTemp_RA >>= 7;	// Elimina los 6 bits menos significativos
	
    

    // sprintf(txt,"%ld   ",iTemp_RA);  LCD_GLASS_String(txt,LCD_PESO); delay_ms(5000);
	
	if(iTemp_RA == 65535){
	}else if(iTemp_RA > 65535){
		iTemp_RA = 131072 - iTemp_RA;
		iTemp_RA = 32500 - iTemp_RA;
	}else{
		iTemp_RA += 32500;
	}

   // sprintf(txt,"%ld   ",iTemp_RA);  LCD_GLASS_String(txt,LCD_PESO); delay_ms(5000);

	ValueCount = fFilter_Averaging(iTemp_RA,cFillFilter);

   // sprintf(txt,"%f      ",ValueCount);  LCD_GLASS_String(txt,LCD_PESO); 	delay_ms(500);	

	return ValueCount;
}

void ReadHX712(unsigned long *ptr	){ //by ERH
    unsigned int i;
    unsigned char  *dato=ptr;//[4]={0};
    //ptr=(unsigned long *)&dato;

	if(MISO!=0)return;
	for(i=0;i<24;i++)//24 bits de comunicación
	{    
    	SCLK=1;  
    	dato[i/8+1]<<=1;//Lectura en flanco negativo
	    SCLK=0; 
        dato[i/8+1]|=MISO&0x01;
	}
	//1 bit de configuración a 10Hz
	SCLK=1;   i<<=5;
	SCLK=0;  
	*(ptr) >>= 7;
  // sprintf(txt,"%lX",*ptr);  LCD_GLASS_String(txt,LCD_TOTAL); 

}
