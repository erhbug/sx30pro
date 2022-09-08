/*********************************************************************************************************
*                                   	BSP SERIAL DRIVER
*
* Filename      : bsp_keyboard_membrane_driver.c
* Version       : V1.00
* Programmer(s) : PVR
*********************************************************************************************************/



		#include "stdint.h"
		#include "string.h"
		#include "stdio.h"
		#include "stdarg.h"

		#include "stm32xx_hal.h"

		#include "bsp.h"
		#include "bsp_cpu.h"
		#include "bsp_keyboard.h"
		
		#include "app_cfg.h"
		
		#include "dvr_scale.h"
		#include "dvr_def.h"

#if MEMBRANE_KEYBOARD > 0 || MEMBRANE_KEYBOARD_CRYSTAL > 0
		
	#if PCB_20300284 > 0		
		#define GPIOX GPIOA
	#else
		#define GPIOX GPIOB
	#endif
		
		/**
			******************************************************************************
			* Objective: Delay for Keyboard Reading 
			******************************************************************************
			*/
			uint16_t iDelay_KeyG(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
				
				uint32_t tickstart = HAL_GetTick();
				
				while(HAL_GetTick() - tickstart <25){
					
					IWDG->KR =  IWDG_KEY_REFRESH;
					
					if(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) != 0){
						return 1;
					}
				}
				
				return 0;
			}

		/**
			******************************************************************************
			* Objective: Rotate the common signal. 
			******************************************************************************
			*/

void vRotate_Comun(unsigned char cIndex_Comun){
	
			/*
			#define DATAK0				P13_OUT_Pin	//PA1 (P13)
			#define DATAK1				P12_OUT_Pin	//PA0 (P12)
			#define DATAK2				P11_OUT_Pin	//PA2 (P11)
			#define DATAK3				P10_OUT_Pin	//PA3 (P10)
			#define DATAK4				P9_OUT_Pin	//PA4 (P9)
			#define DATAK5				P8_OUT_Pin	//PA5 (P8)
			#define DATAK6				P7_OUT_Pin	//PA6 (P7)
			#define DATAK7				P6_OUT_Pin	//PA7 (P6)
			*/
	
			switch(cIndex_Comun){
				case 0:
					HAL_GPIO_WritePin(GPIOA, DATAK7, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOA, DATAK0, GPIO_PIN_RESET); HAL_GPIO_WritePin(GPIOA, DATAK1, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK2, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOA, DATAK3, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK4, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK5, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOA, DATAK6, GPIO_PIN_SET); 
					break;
				case 1:
					HAL_GPIO_WritePin(GPIOA, DATAK0, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK1, GPIO_PIN_RESET); HAL_GPIO_WritePin(GPIOA, DATAK2, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOA, DATAK3, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK4, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK5, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOA, DATAK6, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK7, GPIO_PIN_SET);
					break;
				case 2:
					HAL_GPIO_WritePin(GPIOA, DATAK0, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK1, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK2, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(GPIOA, DATAK3, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK4, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK5, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOA, DATAK6, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK7, GPIO_PIN_SET);
					break;
				case 3:
					HAL_GPIO_WritePin(GPIOA, DATAK0, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK1, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK2, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOA, DATAK3, GPIO_PIN_RESET); HAL_GPIO_WritePin(GPIOA, DATAK4, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK5, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOA, DATAK6, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK7, GPIO_PIN_SET);
					break;
				case 4:
					HAL_GPIO_WritePin(GPIOA, DATAK0, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK1, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK2, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOA, DATAK3, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK4, GPIO_PIN_RESET); HAL_GPIO_WritePin(GPIOA, DATAK5, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOA, DATAK6, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK7, GPIO_PIN_SET);
					break;
				case 5:
					HAL_GPIO_WritePin(GPIOA, DATAK0, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK1, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK2, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOA, DATAK3, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK4, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK5, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(GPIOA, DATAK6, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK7, GPIO_PIN_SET);
					break;
				case 6:
					HAL_GPIO_WritePin(GPIOA, DATAK0, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK1, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK2, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOA, DATAK3, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK4, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK5, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOA, DATAK6, GPIO_PIN_RESET); HAL_GPIO_WritePin(GPIOA, DATAK7, GPIO_PIN_SET);
					break;
				case 7:
					HAL_GPIO_WritePin(GPIOA, DATAK0, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK1, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK2, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOA, DATAK3, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK4, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK5, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOA, DATAK6, GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOA, DATAK7, GPIO_PIN_RESET);
					break;
			}
		}



		/**
			******************************************************************************
			* Objective: Know the key that was pressed.
			******************************************************************************
			*/
			enum digi_key eFind_Key_Press(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, 
				enum digi_key eKey_cero, enum digi_key eKey_uno, enum digi_key eKey_dos, 
					enum digi_key eKey_tres, enum digi_key eKey_cuatro, enum digi_key eKey_cinco,
						enum digi_key eKey_seis, enum digi_key eKey_siete){

				unsigned char i;
				enum digi_key eKeyATouch = KEY_NULL;
				
				for(i=0; i<8 && eKeyATouch == KEY_NULL; i++){
				
					vRotate_Comun(i);
					
					if(iDelay_KeyG(GPIOx, GPIO_Pin) == 0){
					
						if(i == 0){
							eKeyATouch = eKey_cero;
						}else if(i == 1){
							eKeyATouch = eKey_uno;
						}else if(i == 2){
							eKeyATouch = eKey_dos;
						}else if(i == 3){
							eKeyATouch = eKey_tres;
						}else if(i == 4){
							eKeyATouch = eKey_cuatro;
						}else if(i == 5){
							eKeyATouch = eKey_cinco;
						}else if(i == 6){
							eKeyATouch = eKey_seis;
						}else{
							eKeyATouch = eKey_siete;
						}
					}
				}
				
				vRotate_Comun(0);
				
				return eKeyATouch;
			}


		/**
			******************************************************************************
			* Objective: analyze the keyboard to find the key that was pressed.
			******************************************************************************
			*/

		void BSP_MembKeyboard_ReadKey(enum digi_key* peKey)
			{
			unsigned char i;
			*peKey = KEY_NULL;
					
		#if PROTEK_ON > 0

       #if PLUS > 0 && PRO == 0
				for(i=0; i<4 && *peKey == KEY_NULL; i++){
					
					switch(i){	
						case 0:
							*peKey = eFind_Key_Press(GPIOB, K0, KEY_CERO,KEY_VT,KEY_X,KEY_M,KEY_7,KEY_8,KEY_9,KEY_VARIOS);
							break;
							
						case 1:
							*peKey = eFind_Key_Press(GPIOX, K1, KEY_TARA,KEY_FIJAR,KEY_MENOS,KEY_R,KEY_4,KEY_5,KEY_6,KEY_CAMBIO);
							break;
					
						case 2:					
							*peKey = eFind_Key_Press(GPIOX, K2, KEY_M1,KEY_M2,KEY_M3,KEY_M4,KEY_1,KEY_2,KEY_3,KEY_MAS);				
							break;						
						
						case 3:
							*peKey = eFind_Key_Press(GPIOB, K3, KEY_M5,KEY_M6,KEY_M7,KEY_M8,KEY_C,KEY_0,KEY_PUNTO,KEY_NC);
							break;	
					}
				}				
			 #elif  PLUS == 0 && PRO > 0
				for(i=0; i<4 && *peKey == KEY_NULL; i++){
					
					switch(i){	
						case 0:
							*peKey = eFind_Key_Press(GPIOB, K0, KEY_M,KEY_X,KEY_VT,KEY_KEY_CERO,KEY_7,KEY_8,KEY_9,KEY_NC);
							break;
							
						case 1:
							*peKey = eFind_Key_Press(GPIOX, K1, KEY_R,KEY_MENOS,KEY_NULL,KEY_TARA,KEY_4,KEY_5,KEY_6,KEY_NULL);
							break;
						
						case 2:
							*peKey = eFind_Key_Press(GPIOX, K2, KEY_M4,KEY_M3,KEY_M2,KEY_M1,KEY_1,KEY_2,KEY_3,KEY_NULL);
							break;						
						
						case 3:
							*peKey = eFind_Key_Press(GPIOB, K3, KEY_VARIOS,KEY_NULL,KEY_CAMBIO,KEY_NULL,KEY_C,KEY_0,KEY_PUNTO,KEY_MAS);
							break;	
					}
				}				
			 #else
				for(i=0; i<4 && *peKey == KEY_NULL; i++){
					
					switch(i){
				
						case 0:
							*peKey = eFind_Key_Press(GPIOB, K0, KEY_NULL,KEY_M,KEY_R,KEY_CERO,KEY_7,KEY_8,KEY_9,KEY_NULL);
							break;
							
						case 1:
							*peKey = eFind_Key_Press(GPIOX, K1, KEY_NULL,KEY_VT,KEY_FIJAR,KEY_TARA,KEY_4,KEY_5,KEY_6,KEY_CAMBIO);
							break;
					
						case 2:
							*peKey = eFind_Key_Press(GPIOX, K2, KEY_M4,KEY_M3,KEY_M2,KEY_M1,KEY_1,KEY_2,KEY_3,KEY_MAS);
							break;						
						
						case 3:
							*peKey = eFind_Key_Press(GPIOB, K3, KEY_NULL,KEY_NULL,KEY_NULL,KEY_NULL,KEY_C,KEY_0,KEY_PUNTO,KEY_NC);
							break;	
					}
				}
			#endif
			
		#elif PCB_PCP_ON > 0				
				
			for(i=0; i<4 && *peKey == KEY_NULL; i++){
		
				switch(i){
	
					case 0:
						*peKey = eFind_Key_Press(GPIOB, K0, KEY_M1,KEY_M2,KEY_M3,KEY_7,KEY_8,KEY_9,KEY_CERO,KEY_NULL); 
					break;
				
					case 1:
						*peKey = eFind_Key_Press(GPIOX, K1, KEY_M4,KEY_M5,KEY_M6,KEY_4,KEY_5,KEY_6,KEY_FOCO,KEY_NULL);
					break;
			
					case 2:
						*peKey = eFind_Key_Press(GPIOX, K2, KEY_P_PACK,KEY_RCL,KEY_STO,KEY_1,KEY_2,KEY_3,KEY_PRINT,KEY_NULL);
					break;						
			
					case 3:
						*peKey = eFind_Key_Press(GPIOB, K3, KEY_NULL,KEY_NULL,KEY_TARA,KEY_C,KEY_0,KEY_PUNTO,KEY_MAS,KEY_NULL);
					break;	
							}
			}				

		#elif MFQ_MEM_ON > 0

				for(i=0; i<4 && *peKey == KEY_NULL; i++){
			
					switch(i){
		
						case 0:
							*peKey = eFind_Key_Press(GPIOA, K0, KEY_NC,KEY_CAMBIO,KEY_C,KEY_0,KEY_PUNTO,KEY_MAS,KEY_NULL,KEY_NULL); 
						break;
					
						case 1:
							*peKey = eFind_Key_Press(GPIOA, K1, KEY_FOCO,KEY_VT,KEY_1,KEY_2,KEY_3,KEY_VARIOS,KEY_NULL,KEY_NULL);
						break;
				
						case 2:
							*peKey = eFind_Key_Press(GPIOB, K2, KEY_R,KEY_M,KEY_4,KEY_5,KEY_6,KEY_X,KEY_NULL,KEY_NULL);
						break;						
				
						case 3:
							*peKey = eFind_Key_Press(GPIOB, K3, KEY_TARA,KEY_CERO,KEY_7,KEY_8,KEY_9,KEY_MENOS,KEY_NULL,KEY_NULL);
						break;	
								}
				}

		#else

		if(stScaleParam.iModeloBascula == BasculaPCR_kglb){
			
			 for(i=0; i<4 && *peKey == KEY_NULL; i++){
					
					switch(i){
				
						case 0:
							*peKey = eFind_Key_Press(GPIOB, K0, KEY_MENOS,KEY_M,KEY_R,KEY_CERO,KEY_7,KEY_8,KEY_9,KEY_VARIOS);
							break;
							
						case 1:
							*peKey = eFind_Key_Press(GPIOX, K1, KEY_X,KEY_VT,KEY_FIJAR,KEY_TARA,KEY_4,KEY_5,KEY_6,KEY_CAMBIO);
							break;
					
						case 2:
							*peKey = eFind_Key_Press(GPIOX, K2, KEY_KGLB,KEY_M3,KEY_M2,KEY_M1,KEY_1,KEY_2,KEY_3,KEY_MAS);
							break;						
						
						case 3:
							*peKey = eFind_Key_Press(GPIOB, K3, KEY_M8,KEY_M6,KEY_M5,KEY_M4,KEY_C,KEY_0,KEY_PUNTO,KEY_NC);
							break;	
					}
				}
		 }else{

				for(i=0; i<4 && *peKey == KEY_NULL; i++){
					
					switch(i){
				
						case 0:
							*peKey = eFind_Key_Press(GPIOB, K0, KEY_MENOS,KEY_M,KEY_R,KEY_CERO,KEY_7,KEY_8,KEY_9,KEY_VARIOS);
							break;
							
						case 1:
							*peKey = eFind_Key_Press(GPIOX, K1, KEY_X,KEY_VT,KEY_FIJAR,KEY_TARA,KEY_4,KEY_5,KEY_6,KEY_CAMBIO);
							break;
					
						case 2:
							*peKey = eFind_Key_Press(GPIOX, K2, KEY_M4,KEY_M3,KEY_M2,KEY_M1,KEY_1,KEY_2,KEY_3,KEY_MAS);
							break;						
						
						case 3:
							*peKey = eFind_Key_Press(GPIOB, K3, KEY_M8,KEY_M7,KEY_M6,KEY_M5,KEY_C,KEY_0,KEY_PUNTO,KEY_NC);
							break;	
					}
				}
			}	
			#endif
		}
#endif
