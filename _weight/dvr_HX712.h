#ifndef _DVR_HX712_H_
#define _DVR_HX712_H_

extern unsigned char lecturaADC[4];

float fRead_Adc(unsigned char cFillFilter);
void vSend_Status_Adc(void);
void SPIGPIO_Sync(char*); //pvr	

#endif
