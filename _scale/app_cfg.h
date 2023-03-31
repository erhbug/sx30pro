/*
*******************************MAIN APP CONFIGURATION FILE**********************************************
* Filename      : app_cfg.h
* Version       : V1.00
* Programmer(s) : PVR
*********************************************************************************************************
*/
#ifndef APP_CFG_H
#define APP_CFG_H

#define DEF_ENABLED   1
#define DEF_DISABLED  0

#define ENABLE   1
#define DISABLE  0
			 
/*--------------------------------------------------------------
								TIPO DE DISPLAY
----------------------------------------------------------------*/
#define 	DISPLAY_20400047_EN			DEF_ENABLED				//Habilita display con las siguientes caracter?sticas
																	// -Indicador peso y precio en la parte superior
																	// -Indicador de bater?a
																// En caso de deshabilitaci?n, se considera el arreglo de display 20400022
#if DISPLAY_20400047_EN > 0
		#define LCD_TOTAL   0 
		#define LCD_PESO    1 
		#define LCD_PRECIO  2
#else
		#define LCD_PESO		0
		#define LCD_PRECIO	1
		#define LCD_TOTAL		2
#endif

/*--------------------------------------------------------------
								MODELO BASCULA
----------------------------------------------------------------*/

#define 	SX30PRO		DEF_ENABLED

#define sVersion "23C1"

#endif // APP_CFG_H
