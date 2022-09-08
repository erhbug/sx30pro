/*********************************************************************************************************
*                                   	APP KEYBOARD HEADER FILE
*
* Filename      : app_keyboard.h
* Version       : V1.00
* Programmer(s) : PVR
*********************************************************************************************************/

#ifndef  APP_KEYBOARD_H
#define  APP_KEYBOARD_H

#include "stm32xx_hal.h"
#include "bsp.h"

extern bool KeyPressed;
extern enum digi_key eKeyTouch;
	
/*
*********************************************************************************************************
*                              						KEYBOARD DEFINITIONS
*********************************************************************************************************
*/

	
void vCapture_Valor(float *pfValor, float fNew_Digit, 
int iNumber_Max_Digit, unsigned char cCounter_Decimal);
float vCapture_Valor_Test(unsigned char fNew_Digit, uint16_t cDecimal_Number,float fValue_Capture);
unsigned char cConvert_Char(enum digi_key Key_Num);
	
enum digi_key vActionKey(void);	
void vScan_Key(void);
void vBeep_Key(void);
uint16_t iDelay_Key_ON(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void vReadKey(void);
uint8_t iCompareValue(uint8_t iValue, uint8_t iTest);
uint8_t cConvert_Char(enum digi_key Key_Num);
void vCapture_Valor(float *pfValor, float fNew_Digit, int iNumber_Max_Digit, unsigned char cCounter_Decimal);
	
void beep_VT(void);
void beep_sin_FitroMultipresionTecla(void);
enum digi_key SegundaFuncion(enum digi_key Primera, enum digi_key Segunda);
#endif /* APP_KEYBOARD_H */
