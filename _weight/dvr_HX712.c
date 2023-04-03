/* Includes ------------------------------------------------------------------*/
#include "./_weight/dvr_HX712.h"
#include "./_scale/dvr_scale.h"

unsigned long idata ADcode_pre=0;
float  xdata fAverage_x = 0;
unsigned char iValueOut = 0;
float xdata arfDataFilter_x[FILTER_SIZE_BUFF] = {0.00};
//extern unsigned long idata ADcode_pre;
//extern float arfDataFilter_x[];
//extern float fAverage_x;
//extern unsigned char iValueOut;
void _nop_(void);


void sclk_unit(void)
{
	_nop_();
	SCLK = 1;
	_nop_();
	SCLK = 0;
}

void ReadHX712(void){ //by ERH
    unsigned char i;
    MISO = 1;
    SCLK = 0;
	while(MISO && i++<105)delay_ms(1);
    if(MISO){
       // srFlagScale.bErrorReadAdc=1;
	    return ;
    }
    ADcode_pre=0;
	for(i=0;i<24;i++)//24 bits de comunicaci�n
	{        
		ADcode_pre<<=1;//Lectura en flanco negativo
		sclk_unit();
		if(MISO)ADcode_pre++;
	}
	//1 bit de configuraci�n a 10Hz
	ADcode_pre >>= 5;
	sclk_unit();
	
	//LCD_GLASS_Float(ADcode_pre, 0, LCD_TOTAL); 
   
	if(ADcode_pre == (unsigned long)262143){
//	  srFlagScale.bErrorReadAdc=1;
	}else if(ADcode_pre > (unsigned long)262143){
		ADcode_pre = (unsigned long)524288 - ADcode_pre;
		ADcode_pre = (unsigned long)32500 - ADcode_pre;
	//	srFlagScale.bErrorReadAdc = 0;
	}else{
		ADcode_pre += (unsigned long)32500;
//		srFlagScale.bErrorReadAdc = 0;
	}
	
   return ;
}
/*
*/
float fFilter_Averaging(unsigned long iActualWeight, unsigned char cFastFill){	
	float xdata arfLowestToHighest[FILTER_SIZE_BUFF] = {0.00};
    float xdata fThreshold = stScaleParam.fFactorCalibrate*0.5;
	float xdata fData_Vector = 0;
	float *pfData_Filter;
	float xdata fActualWeight = (float)(iActualWeight);
	unsigned char iLenthData_x = FILTER_SIZE_BUFF;		// Longitud de los datos a ordenar, original 6, en prueba 10 
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
	
	return fAverage_x;
}

/*
*/
//Funcion para leer adc:
float fRead_Adc(unsigned char cFillFilter){	
	ReadHX712(); 		
return fFilter_Averaging(ADcode_pre,cFillFilter);
}

