#ifndef _DVR_HX712_H_
#define _DVR_HX712_H_

extern unsigned char lecturaADC[4];

float fRead_Adc(unsigned char cFillFilter);	
void vSend_Status_Adc(void);
void ReadHX712(unsigned long *ptr	);
float fFilter_Averaging(unsigned long iActualWeight, unsigned char cFastFill);	

#endif
