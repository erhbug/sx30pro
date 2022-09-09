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

int iCountFailResponse = 0;

sbit MISO = P0^3;	
sbit SCLK = P0^4;	

float fRead_Adc(char cFillFilter){
float ValueCount = 0;
	
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
	
//	ValueCount = fFilter_Averaging(iTemp_RA,cFillFilter);
	
	return ValueCount;
}

void SPIGPIO_Sync(char* buf)
{
	long int lByte=0;
	int i,j;
	
	//24 bits de comunicación
	for(i=0;i<24;i++)
	{
    SCLK=1;  for(j=0;j<10;j++);
	lByte<<=1;
	lByte|=(P0^3)&0x00000001;												//Lectura en flanco negativo
	SCLK=0; for(j=0;j<10;j++);
	}
	
	//1 bit de configuración a 10Hz
	SCLK=1;  for(j=0;j<10;j++);
	SCLK=0; for(j=0;j<10;j++);

	buf[2]= lByte&0x000000FF;
	buf[1]= (lByte>>8)&0x000000FF;
	buf[0]= (lByte>>16)&0x000000FF;

}




