#ifndef _DVR_HX712_H_
#define _DVR_HX712_H_

#include <REG52.H>

sbit MISO = P1^2;//P0^3;	
sbit SCLK = P0^4;	

#define FILTER_SIZE_BUFF 5

float fRead_Adc(unsigned char cFillFilter);
float fFilter_Averaging(unsigned long iActualWeight, unsigned char cFastFill);

#endif
