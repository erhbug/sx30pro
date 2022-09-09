#ifndef _DVR_HX712_H_
#define _DVR_HX712_H_

float fRead_Adc(unsigned char cFillFilter);
void vSend_Status_Adc(void);
void SPIGPIO_Sync(char*); //pvr	

#endif
