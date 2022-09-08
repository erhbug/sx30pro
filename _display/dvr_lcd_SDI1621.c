/*void delay_35u(void);
int iLCD_GLASS_Init(void);
void lcd_unit_write(unsigned char d);
void write_lcd(SOLIDIC Display);
int iLCD_GLASS_DeInit(void);
int iLCD_GLASS_Clear(void);
int iLCD_GLASS_All_On(void);
int iLCD_GLASS_Blink(char comand_value);
void LCD_SET_Char(char cCaracter, char cPosition_On_LCD, 
	char cPosition_Text);
void LCD_GLASS_Dot(char iNumber_Dot, char cPosition_On_LCD, char cFLag_On);
void LCD_GLASS_Symbols(char cSymbol, char cFlag_On);
void LCD_GLASS_Float(float fNumber_To_LCD, char iNumber_Decimal, char cPosition_On_LCD);
void LCD_GLASS_String(char *pCaracter, char cPosition_On_LCD);
/* Includes ------------------------------------------------------------------*/
//#include "app_cfg.h"

#include "./_display/dvr_lcd_SDI1621.h"

#include <stdio.h>
#include <string.h>
//#include "stm32xx_hal.h"
//#include "dvr_def.h"
//#include "dvr_lcd.h"
//#include "dvr_scale.h"

//#include "app_keyboard.h"
//#include "bsp.h"

//#include "dvr_battery_la.h"

	SOLIDIC Display;

#if DISPLAY_20400047_EN > 0	
const char cNumber_LCD[10] = { 
		0x5F, //	01011111,  	//0
		0x50,	//	01010000, 	//1
		0x3D,	//	00111101,		//2
		0x79,	//	01111001, 	//3
		0x72,	//	01110010, 	//4
		0x6B,	//	01101011,		//5
		0x6F,	//	01101111,		//6
		0x51,	//	01010001,		//7
		0x7F,	//	01111111,		//8
		0X73};//	01110011		//9

const char cABC_LCD[28] = {
		0x77,	//	01110111,	//A
		0x6E,	//	01101110,	//b
		0x0F,	//	00001111,	//C
		0x7C,	//	01111100,	//d
		0x2F,	//	00101111, //E
		0x27,	//	00100111,	//F
		0x6F,	//	01101111,	//G
		0x76,	//	01110110,	//H
		0x50,	//	01010000,	//I
		0x5C,	//	01011100,	//J
		0x36,	//	00110110,	//K
		0x0E,	//	00001110,	//L
		0x00,	//	00000000,	//M
		0x64,	//	01100100,	//n
		0x5F,	//	01011111,	//O
		0x37,	//	00110111,	//P
		0x73,	//	01110011,	//q
		0x24,	//	00100100,	//r
		0x6B,	//	01101011,	//S
		0x2E,	//	00101110,	//T
		0x5E,	//	01011110,	//U
		0x4C,	//	01001100,	//V
		0x00,	//	00000000,	//W
		0x00,	//	00000000,	//X
		0x72,	//	01110010,	//Y
		0x3D,	//	00111101,	//Z
		0x20};//	00100000	//-

#define S_ZERO      0
#define P_PESO_3    1
#define P_PESO_2    2
#define P_PESO_1    3
#define S_TARE      4		
#define S_14        5
#define S_34        6
#define P_TOTAL_2   7
#define P_TOTAL_1   8		
#define S_24        9
#define S_44        10
#define S_Y         11
#define P_PRECIO_3  12
#define P_PRECIO_2  13
#define P_PRECIO_1  14
#define S_X         15
		
		

#else
const char cNumber_LCD[10] = { 
		0x7D, //	01111101,  	//0
		0x60,	//	01100000, 	//1
		0x3E,	//	00111110,		//2
		0x7A,	//	01111010, 	//3
		0x63,	//	01100011, 	//4
		0x5B,	//	01011011,		//5
		0x5F,	//	01011111,		//6
		0x70,	//	01110000,		//7
		0x7F,	//	01111111,		//8
		0x7B};	//	01111011		//9

const char cABC_LCD[28] = {
		0x77,	//	01110111,	//A
		0x4F,	//	01001111,	//b
		0x1D,	//	00011101,	//C
		0x6E,	//	01101110,	//d
		0x1F,	//	00011111, //E
		0x17,	//	00010111,	//F
		0x5F,	//	01011111,	//G
		0x67,	//	01100111,	//H
		0x60,	//	01100000,	//I
		0x6C,	//	01101000,	//J
		0x27,	//	01100111,	//K
		0x0D,	//	00001101,	//L
		0x00,	//	01000110,	//M
		0x46,	//	01000110,	//n
		0x7D,	//	01111101,	//O
		0x37,	//	00110111,	//P
		0x73,	//	01110011,	//q
		0x06,	//	00000110,	//R
		0x5B,	//	01011011,	//S
		0x0F,	//	00001111,	//T
		0x6D,	//	01101101,	//U
		0x4C,	//	01001100,	//V
		0x00,	//	01001100,	//W
		0x00,	//	00001000,	//X
		0x27,	//	00100111,	//Y
		0x3E,	//	00111110,	//Z
		0x02};//	00000010	//-



#define P_PESO_3    6
#define P_PESO_2    7
#define P_PESO_1    8
#define P_PRECIO_2  1
#define P_PRECIO_1  2
#define P_TOTAL_2   9
#define P_TOTAL_1   3		
#define S_ZERO      12
#define S_X         14
#define S_Y         15	
		
#define S_KG        11
#define S_LB        10		
#define S_NET       13

#endif
		




//extern UART_HandleTypeDef huart1;
//extern void vSend_String_Usart(UART_HandleTypeDef *huart,char *pData);
//extern void vSend_String_USB(char *pData);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Timer handler -------------------------------------------------------------*/


// funci��n de retardo 10us
void delay_35u(void){ 	
	unsigned int j;
  	for(j= 0;j<5;j++);
}

int iLCD_GLASS_Init(void){//int i,j,k;

/* Configurar como salidas
  LCD_CS_N_Pin
  LCD_WR_N_Pin
  LCD_DATA_Pin
*/

	LCD_CS_N_ON;
	LCD_WR_N_ON;
	LCD_DATA_ON;
	delay_35u();
	delay_35u();	
	
	Display.mode=LCD_COMAND;
	
	Display.cmd=SYS_EN;
	write_lcd(Display);//Turn on system oscillator
	
	Display.cmd=LCD_ON;	
	write_lcd(Display);//Turn on LCD bias generator		
	
	Display.cmd=BIAS_4;	
	write_lcd(Display);//LCD 1/2 bias option
	
	Display.cmd=COM_4;	
	write_lcd(Display);//LCD 1/3 bias option 
		
	Display.mode=LCD_DATA;
	Display.addr=0;
	Display.counter=sizeof(Display.LCD_BUF);
	memset(Display.LCD_BUF, 0x00, sizeof(Display.LCD_BUF));
	write_lcd(Display);//Write data to the RAM 
	/*while(1){
		memset(Display.LCD_BUF, 0x00, sizeof(Display.LCD_BUF));
		Display.LCD_BUF[5]=1<<7;write_lcd(Display);
		HAL_Delay(1000);
		Display.LCD_BUF[9]=1<<7;write_lcd(Display);
		HAL_Delay(1000);
		Display.LCD_BUF[6]=1<<7;write_lcd(Display);
		HAL_Delay(1000);
		Display.LCD_BUF[10]=1<<7;write_lcd(Display);
		HAL_Delay(1000);
		
		
	}*/
	/*while(1){
	LCD_GLASS_Symbols(9,0); //bateria 0/3
				LCD_GLASS_Symbols(8,0);
				LCD_GLASS_Symbols(7,0);
				LCD_GLASS_Symbols(10,1);
		for(i=0;i<1000;i++){
		IWDG->KR =  IWDG_KEY_REFRESH; 
			HAL_Delay(1);
		}
		
		LCD_GLASS_Symbols(9,0); //bateria 0/3
				LCD_GLASS_Symbols(8,0);
				LCD_GLASS_Symbols(7,1);
				LCD_GLASS_Symbols(10,1);
		for(i=0;i<1000;i++){
		IWDG->KR =  IWDG_KEY_REFRESH; 
			HAL_Delay(1);
		}
		
		LCD_GLASS_Symbols(9,0); //bateria 0/3
				LCD_GLASS_Symbols(8,1);
				LCD_GLASS_Symbols(7,1);
				LCD_GLASS_Symbols(10,1);
		for(i=0;i<1000;i++){
		IWDG->KR =  IWDG_KEY_REFRESH; 
			HAL_Delay(1);
		}
		
		LCD_GLASS_Symbols(9,1); //bateria 0/3
				LCD_GLASS_Symbols(8,1);
				LCD_GLASS_Symbols(7,1);
				LCD_GLASS_Symbols(10,1);
		for(i=0;i<1000;i++){
		IWDG->KR =  IWDG_KEY_REFRESH; 
			HAL_Delay(1);
		}
	}*/
	
	return 0;
}

//****************************************************
// Controlador LCD 1621 bloque compilaci��n condicional ------ comenzar -------
//****************************************************
void lcd_unit_write(unsigned char d){
	unsigned char i;
	for(i=0;i<8;i++){
		if( ( d & 0x80 ) == 0 ) LCD_DATA_OFF;
		else LCD_DATA_ON;
		LCD_WR_N_OFF;	
		delay_35u();	
    LCD_WR_N_ON;			
		delay_35u();		
		d <<= 1;		
	}
}

// funci��n de escritura LCD
// modo:
// 0: Entrada de comando
// 1: actualizar todas las ��reas
// cmd: comando de codificaci��n
// ---- Nota: Mostrar la funci��n de la tecla de interrupci��n ET0 en la funci��n - (Cuando el programa no necesita abrir la interrupci��n T0, preste especial atenci��n para evitar que la funci��n de pantalla abra accidentalmente la interrupci��n T0)

void write_lcd(SOLIDIC Display){//char mode,unsigned char cmd,unsigned char addr,unsigned char counter)
	unsigned char i=0;	
	
	//HAL_NVIC_DisableIRQ(TIM14_IRQn);
	
/*	if(strTimer.cFLag_TimerF_On ==  1 && eAccionScale != scalePreOnDc){//contador autoapagado 15m
		if(strTimer.iTimerF > 36){
			strTimer.iTimerF-=35;
		}
	}
	
	if(strTimer.cFLag_TimerI_On && srFlagScale.bSourceVoltage == SOURCE_BATTERY){ //contador apagado backlite 15seg
		if(strTimer.iTimerI > 36){
			strTimer.iTimerI-=35;			
		}
	}*/
	
	LCD_CS_N_ON;
	LCD_WR_N_ON;
	LCD_DATA_ON;	
	delay_35u();		
	LCD_CS_N_OFF;//	LCD_DATA_ON;
	delay_35u();
	LCD_WR_N_OFF;
	delay_35u();
	LCD_WR_N_ON;
	delay_35u();
	LCD_DATA_OFF;
	LCD_WR_N_OFF;
	delay_35u();
	LCD_WR_N_ON;
	delay_35u();
	if(Display.mode==0)
	  	LCD_DATA_OFF;
	else 
	  	LCD_DATA_ON;
	LCD_WR_N_OFF;
	delay_35u();
	LCD_WR_N_ON;
	delay_35u();

	if(Display.mode == 0){
		lcd_unit_write(Display.cmd);		
		LCD_WR_N_OFF;
		delay_35u();
		LCD_WR_N_ON;
		delay_35u();
	}
	else 
	{		
		Display.addr = 0;
		for(i=0;i<6;i++){	// ����д������,�����׵�ַ  Comando de escritura de datos, enviar la primera direcci��n
			LCD_WR_N_OFF;			
			if( (Display.addr & 0x80) == 0 ) 
				LCD_DATA_OFF;
		  else 
				LCD_DATA_ON;
			delay_35u();
			LCD_WR_N_ON;
			delay_35u();
			Display.addr <<= 1;
		}		
		for(i=0;i<16;i++)	// ����д������,��������  Comando de escritura de datos, enviar datos
			lcd_unit_write(Display.LCD_BUF[15 - i]);		
	}
	LCD_CS_N_ON;
	LCD_DATA_ON;
	LCD_WR_N_ON;	
	delay_35u();
	
	//HAL_GPIO_WritePin(GPIOC, TARA_IN_Pin, GPIO_PIN_RESET);
	
	//HAL_NVIC_EnableIRQ(TIM14_IRQn);	
}

int iLCD_GLASS_DeInit(void){	
	Display.mode=LCD_DATA;
	Display.addr=0;
	Display.counter=sizeof(Display.LCD_BUF);	
	
	memset(Display.LCD_BUF, 0x00, sizeof(Display.LCD_BUF));//InitLCDBuf(0);
	write_lcd(Display);//Write data to the RAM
	
	Display.mode=LCD_COMAND;
	
	Display.cmd=LCD_OFF;	
	write_lcd(Display);//Turn off LCD bias generator	
	
	Display.cmd=SYS_DIS;
	write_lcd(Display);//Turn off system oscillator
	
	return 1;
}

/*
*/
int iLCD_GLASS_Clear(void){	
	Display.mode=LCD_DATA;
	Display.addr=0;
	Display.counter=sizeof(Display.LCD_BUF);	
	memset(Display.LCD_BUF, 0x00, sizeof(Display.LCD_BUF));//InitLCDBuf(0);
	write_lcd(Display);//Write data to the RAM
	return 1;
}

/*
*/
int iLCD_GLASS_All_On(void){
	Display.mode=LCD_DATA;
	Display.addr=0;
	Display.counter=sizeof(Display.LCD_BUF);	
	memset(Display.LCD_BUF, 0xFF, sizeof(Display.LCD_BUF));
	write_lcd(Display);	
	return 1;
}

int iLCD_GLASS_Blink(char comand_value){

	return 1;
}


void LCD_SET_Char(char cCaracter, char cPosition_On_LCD, 
	char cPosition_Text){
		char i=0;
		#if DISPLAY_20400047_EN > 0			
		if(cPosition_On_LCD==LCD_PESO)i=0;
		if(cPosition_On_LCD==LCD_PRECIO)i=11;
		if(cPosition_On_LCD==LCD_TOTAL) i=5;		
		#else
		  if(cPosition_On_LCD==LCD_PESO)i=5;
		  if(cPosition_On_LCD==LCD_PRECIO)i=0;
		  if(cPosition_On_LCD==LCD_TOTAL) i=10;		
		#endif
		i+=(5-cPosition_Text);
		Display.LCD_BUF[i]&=0x80;	
	  Display.LCD_BUF[i]|=cCaracter;
	
}


void LCD_GLASS_Dot(char iNumber_Dot, char cPosition_On_LCD, char cFLag_On){
	
	switch(cPosition_On_LCD){
		case LCD_PESO:
			if(iNumber_Dot == 3){                         
				if(cFLag_On == 1)
				  Display.LCD_BUF[P_PESO_3]|=0x80;
		    else
				  Display.LCD_BUF[P_PESO_3]&=0x7F;
			}
			if(iNumber_Dot == 2){
        if(cFLag_On == 1)
				  Display.LCD_BUF[P_PESO_2]|=0x80;
		    else
				  Display.LCD_BUF[P_PESO_2]&=0x7F;
		
			}
			if(iNumber_Dot == 1){
        if(cFLag_On == 1)
				  Display.LCD_BUF[P_PESO_1]|=0x80;
		    else
				  Display.LCD_BUF[P_PESO_1]&=0x7F;
			}		
			break;
		case LCD_PRECIO:
			#if DISPLAY_20400047_EN > 0	
		  	if(iNumber_Dot == 3){
          if(cFLag_On == 1)
				    Display.LCD_BUF[P_PRECIO_3]|=0x80;
		      else
				    Display.LCD_BUF[P_PRECIO_3]&=0x7F;		
			   }
			#endif
			if(iNumber_Dot == 2){
        if(cFLag_On == 1)
				  Display.LCD_BUF[P_PRECIO_2]|=0x80;
		    else
				  Display.LCD_BUF[P_PRECIO_2]&=0x7F;		
			}
			if(iNumber_Dot == 1){
        if(cFLag_On == 1)
				  Display.LCD_BUF[P_PRECIO_1]|=0x80;
		    else
				  Display.LCD_BUF[P_PRECIO_1]&=0x7F;
			}			
			break;
			
		case LCD_TOTAL:
			if(iNumber_Dot == 2){
        if(cFLag_On == 1)
				  Display.LCD_BUF[P_TOTAL_2]|=0x80;
		    else
				  Display.LCD_BUF[P_TOTAL_2]&=0x7F;		
			}
			if(iNumber_Dot == 1){
        if(cFLag_On == 1)
				  Display.LCD_BUF[P_TOTAL_1]|=0x80;
		    else
				  Display.LCD_BUF[P_TOTAL_1]&=0x7F;
			}
		break;
	}
	
	LCD_GLASS_Update();
}


void LCD_GLASS_Symbols(char cSymbol, char cFlag_On){
	
	//if(cFlag_On!=0&&cFlag_On!=1)vSend_String_Usart(&huart1,"-------------ERROR------------\r\n");

	switch(cSymbol){	
		#if DISPLAY_20400047_EN > 0	
		
		case 10:  //vSend_String_Usart(&huart1,"1/4 ");
			if(cFlag_On == 1)
				Display.LCD_BUF[S_14]|=0x80;
			else
				Display.LCD_BUF[S_14]&=0x7F;	
			break;	
			case 7: // vSend_String_Usart(&huart1,"2/4 ");
				if(cFlag_On == 1)
				Display.LCD_BUF[S_24]|=0x80;
			else
				Display.LCD_BUF[S_24]&=0x7F;	
			break;	
			case 8:  //vSend_String_Usart(&huart1,"3/4");
			if(cFlag_On == 1)
				Display.LCD_BUF[S_34]|=0x80;
			else
				Display.LCD_BUF[S_34]&=0x7F;	
			break;	
			case 9:  //vSend_String_Usart(&huart1,"4/4");
			if(cFlag_On == 1)
				Display.LCD_BUF[S_44]|=0x80;
			else
				Display.LCD_BUF[S_44]&=0x7F;	
			break;	
			case SYMBOL_NET:  //vSend_String_Usart(&huart1,"SYMBOL_NET");
			if(cFlag_On == 1)
				Display.LCD_BUF[S_TARE]|=0x80;
			else
				Display.LCD_BUF[S_TARE]&=0x7F;	
			break;	
		
    #else		
		case SYMBOL_KG:			//vSend_String_Usart(&huart1,"SYMBOL_KG");
			if(cFlag_On == 1)
				Display.LCD_BUF[S_KG]|=0x80;
			else
				Display.LCD_BUF[S_KG]&=0x7F;			
			break;
		case SYMBOL_LB:			//vSend_String_Usart(&huart1,"SYMBOL_LB");
			if(cFlag_On == 1)
				Display.LCD_BUF[S_LB]|=0x80;
			else
				Display.LCD_BUF[S_LB]&=0x7F;
			break;	
		case SYMBOL_NET:  //vSend_String_Usart(&huart1,"SYMBOL_NET");
			if(cFlag_On == 1)
				Display.LCD_BUF[S_NET]|=0x80;
			else
				Display.LCD_BUF[S_NET]&=0x7F;	
			break;		
		#endif
		
			/*case SYMBOL_TARE:  vSend_String_Usart(&huart1,"SYMBOL_TARE\r\n");
			if(cFlag_On == 1)
				Display.LCD_BUF[S_TARE]|=0x80;
			else
				Display.LCD_BUF[S_TARE]&=0x7F;	
			break;	*/
			
			case SYMBOL_ZERO:  //vSend_String_Usart(&huart1,"SYMBOL_ZERO");
			if(cFlag_On == 1)
				Display.LCD_BUF[S_ZERO]|=0x80;
			else
				Display.LCD_BUF[S_ZERO]&=0x7F;	
			break;		
			
			
			
			case SYMBOL_X:  //vSend_String_Usart(&huart1,"SYMBOL_X");
			if(cFlag_On == 1)
				Display.LCD_BUF[S_X]|=0x80;
			else
				Display.LCD_BUF[S_X]&=0x7F;	
			break;	
			
			case SYMBOL_Y:  //vSend_String_Usart(&huart1,"SYMBOL_Y");
			if(cFlag_On == 1)
				Display.LCD_BUF[S_Y]|=0x80;
			else
				Display.LCD_BUF[S_Y]&=0x7F;	
			break;		
		
							
			
		case SYMBOL_ALL:  //vSend_String_Usart(&huart1,"SYMBOL_ALL");	
			if(cFlag_On==1){
			  #if DISPLAY_20400047_EN > 0	
		Display.LCD_BUF[S_14]|=0x80;
		Display.LCD_BUF[S_24]|=0x80;
		Display.LCD_BUF[S_34]|=0x80;
		Display.LCD_BUF[S_44]|=0x80;	
			#else
		Display.LCD_BUF[S_LB]|=0x80;
		Display.LCD_BUF[S_KG]|=0x80;		
		Display.LCD_BUF[S_NET]|=0x80;	
      #endif					
		Display.LCD_BUF[S_ZERO]|=0x80;
		Display.LCD_BUF[S_Y]|=0x80;
		Display.LCD_BUF[S_X]|=0x80;	
				
			}
			else{
				#if DISPLAY_20400047_EN > 0	
		Display.LCD_BUF[S_14]&=0x7F;
		Display.LCD_BUF[S_24]&=0x7F;
		Display.LCD_BUF[S_34]&=0x7F;
		Display.LCD_BUF[S_44]&=0x7F;	
			#else		
	  Display.LCD_BUF[S_LB]&=0x7F;
		Display.LCD_BUF[S_KG]&=0x7F;		
		Display.LCD_BUF[S_NET]&=0x7F;	
      #endif					
		Display.LCD_BUF[S_ZERO]&=0x7F;
		Display.LCD_BUF[S_Y]&=0x7F;
		Display.LCD_BUF[S_X]&=0x7F;	
			}
			
				
		break;

	}
	
/*	if(cSymbol==SYMBOL_TARE){
	if(cFlag_On == 1)
			vSend_String_Usart(&huart1,"  on\r\n ");
			else
			vSend_String_Usart(&huart1,"  off\r\n ");
		}*/
	
	Display.mode=LCD_DATA;
	Display.addr=0;
	Display.counter=sizeof(Display.LCD_BUF);	
	LCD_GLASS_Update();
}

/**
	******************************************************************************
  * Objetivo: Mostrar en LCD numero con formato float.
  * Parametros entrada: Numero, numero de decimales, posicion en la LCD, e
	*		indicacion de punto flotante.
	* Parametros Salida: Ninguno.
  * Prerequisitos: 
  ***
*/















/*void LCD_GLASS_Float(float fNumber_To_LCD, char iNumber_Decimal, char cPosition_On_LCD){
	
	char strText_LCD[8] = {0,0,0,0,0,0,0,0};
	

	switch(iNumber_Decimal){
		case 0:
			if(cPosition_On_LCD == LCD_TOTAL){
				sprintf(strText_LCD, "%6.0lf", fNumber_To_LCD);
			}else{
				sprintf(strText_LCD, "%5.0lf", fNumber_To_LCD);
			}
			break;
		
		case 1:
			if(cPosition_On_LCD == LCD_TOTAL){
				sprintf(strText_LCD, "%7.1lf", fNumber_To_LCD);
			}else{
				sprintf(strText_LCD, "%6.1lf", fNumber_To_LCD);
			}
			break;
		
		case 2:
			if(cPosition_On_LCD == LCD_TOTAL){
				sprintf(strText_LCD, "%7.2lf", fNumber_To_LCD);	
			}else{
				sprintf(strText_LCD, "%6.2lf", fNumber_To_LCD);
			}
			break;
		
		case 3:
			if(cPosition_On_LCD == LCD_TOTAL){
				sprintf(strText_LCD, "%7.3lf", fNumber_To_LCD);
			}else{
				sprintf(strText_LCD, "%6.3lf", fNumber_To_LCD);
			}
			break;
	}
	
	LCD_GLASS_String(strText_LCD, cPosition_On_LCD);
}*/













	
/**
	******************************************************************************
  * Objetivo: Mostrar en LCD texto.
  * Parametros entrada: Texo y posicion en la LCD.
	* Parametros Salida: Ninguno.
  * Prerequisitos: 
  ***
*/

void LCD_GLASS_String(char *pCaracter, char cPosition_On_LCD){
	char iIndex_Text = 5;
	char iPosicion_Decimal = 0;
	//char txt[30];
	/*vSend_String_USB("\r\n");	
	if(cPosition_On_LCD==LCD_TOTAL)	  vSend_String_USB("LCD_TOTAL=  ");
	if(cPosition_On_LCD==LCD_PRECIO)	vSend_String_USB("LCD_PRECIO= ");
	if(cPosition_On_LCD==LCD_PESO)	  vSend_String_USB("LCD_PESO=   ");	
	vSend_String_USB(pCaracter);vSend_String_USB("\r\n");	*/
	
	/*if(cPosition_On_LCD==LCD_TOTAL)	  vSend_String_Usart(&huart1,"LCD_TOTAL=  ");
	if(cPosition_On_LCD==LCD_PRECIO)	vSend_String_Usart(&huart1,"LCD_PRECIO= ");
	if(cPosition_On_LCD==LCD_PESO)	  vSend_String_Usart(&huart1,"LCD_PESO=   ");	
	vSend_String_Usart(&huart1,pCaracter);vSend_String_Usart(&huart1,"\r\n");	
	*/

	if(cPosition_On_LCD==LCD_TOTAL)	{Display.LCD_BUF[P_TOTAL_2]&=0x7F;Display.LCD_BUF[P_TOTAL_1]&=0x7F;}
  if(cPosition_On_LCD==LCD_PRECIO){
	#if DISPLAY_20400047_EN > 0	
			Display.LCD_BUF[P_PRECIO_3]&=0x7F;
	#endif
	Display.LCD_BUF[P_PRECIO_2]&=0x7F;Display.LCD_BUF[P_PRECIO_1]&=0x7F;}
	if(cPosition_On_LCD==LCD_PESO)	{Display.LCD_BUF[P_PESO_3]&=0x7F;Display.LCD_BUF[P_PESO_2]&=0x7F;Display.LCD_BUF[P_PESO_1]&=0x7F;}		

	while(*(pCaracter) != 0){
		
		if(*(pCaracter) >= '0' && *(pCaracter) <= '9'){			
			LCD_SET_Char(cNumber_LCD[*(pCaracter)-'0'], cPosition_On_LCD, iIndex_Text--);			
		}
		
		if(*(pCaracter) == '-'){
			LCD_SET_Char(cABC_LCD[26], cPosition_On_LCD, iIndex_Text--);			
		}
		
		if(*(pCaracter) == ' '){
			LCD_SET_Char(0x0, cPosition_On_LCD, iIndex_Text);
			iIndex_Text--;			
		}
		
		if(*(pCaracter) >= 'A' && *(pCaracter) <= 'Z'){
			LCD_SET_Char(cABC_LCD[*(pCaracter)-'A'], cPosition_On_LCD, iIndex_Text);
			iIndex_Text--;			
		}
		
		if(*(pCaracter) == '.'){
			
			//sprintf(txt,"PosDec= %d \r\n\r\n",iPosicion_Decimal);				
				//vSend_String_Usart(&huart1,txt);		
				
			if(cPosition_On_LCD == LCD_PRECIO){	
         #if DISPLAY_20400047_EN > 0	
				if(iPosicion_Decimal == 2)
					Display.LCD_BUF[P_PRECIO_3]|=0x80;	
	       #endif				
				if(iPosicion_Decimal == 3)
					Display.LCD_BUF[P_PRECIO_2]|=0x80;				
				if(iPosicion_Decimal == 4)
					Display.LCD_BUF[P_PRECIO_1]|=0x80;							
			}
			if(cPosition_On_LCD == LCD_PESO){				
				if(iPosicion_Decimal == 2)
					Display.LCD_BUF[P_PESO_3]|=0x80;
				if(iPosicion_Decimal == 3)
					Display.LCD_BUF[P_PESO_2]|=0x80;
				if(iPosicion_Decimal == 4)
					Display.LCD_BUF[P_PESO_1]|=0x80;							
			}
			if(cPosition_On_LCD == LCD_TOTAL){
			 #if DISPLAY_20400047_EN > 0	
				if(iPosicion_Decimal == 4)					
					Display.LCD_BUF[P_TOTAL_1]|=0x80;//sprintf(txt,"LCD_BUF[9]= d%d",Display.LCD_BUF[9]);				
				if(iPosicion_Decimal == 3)
					Display.LCD_BUF[P_TOTAL_2]|=0x80;//vSend_String_Usart(&huart1,"CC ");
			 #else
				if(iPosicion_Decimal == 4)					
					Display.LCD_BUF[P_TOTAL_2]|=0x80;//sprintf(txt,"LCD_BUF[9]= d%d",Display.LCD_BUF[9]);				
				if(iPosicion_Decimal == 5)
					Display.LCD_BUF[P_TOTAL_1]|=0x80;//vSend_String_Usart(&huart1,"CC ");				
			 #endif
			}						
		}
		iPosicion_Decimal++;
		pCaracter++;
	}
	
	Display.mode=LCD_DATA;
	Display.addr=0;
	Display.counter=sizeof(Display.LCD_BUF);	
	LCD_GLASS_Update();
}

void LCD_GLASS_Update(void){
	//char txt[10];int i;
	Display.mode=LCD_DATA;	
	/*for(i=0;i<16;i++){
	  sprintf(txt, "%02x, ", Display.LCD_BUF[i]);
		vSend_String_Usart(&huart1,txt);				
	}
	vSend_String_Usart(&huart1," \r\n");	
	vSend_String_Usart(&huart1," \r\n");	*/
	
	write_lcd(Display);//Write data to the RAM			
}
/*

      a7 a6 a5 a4 a3 a2 a1 a0
20400021
         / /////A4/////
         /              /
        A0             A5
        /               /
        /               /
          /////A1//////
        /               /
        /               /
        A2             A6
        /               /
        /               /
         /////A3///////
				 
				 DISPLAY PC-HS
         / /////A0/////
         /              /
        A1             A4
        /               /
        /               /
          /////A5//////
        /               /
        /               /
        A2             A6
        /               /
        /               /
         /////A3///////
				 
*/

