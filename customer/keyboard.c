//*************************************************************************//
//* Name				:   display_cal.c
//* Vision				:	V-Nov07_12
//* Project				:   All project
//* Function			:   Solidic 电子称 显存计算文件
//* $Author				:   superc
//*
//* Copyright 2012 by solidic
//*
//* Recode:
//*						1: 2012-11-07  First Creat By Superc
//*
//*
//*
//*************************************************************************//
#include <REG52.H>
#include <math.h>
//**************************************************************************
// -- 全部头文件 集中管理 ---- ，版本修改文件名会变动 ---  必须修改 这个文件
//**************************************************************************
//#include "./_solidic/head_file_version.h"
#include "./_scale/dvr_scale.h"
#include "./_scale/dvr_def.h"
#include "./_scale/app_cfg.h"
#include "./customer/keyboard.h"

unsigned char LastKey;
unsigned char KeyState=0;


#define KEY_D0_ON				P2 |= 0x02 
#define KEY_D0_OFF			P2 &= 0xFD 

void delay_ms(unsigned int num)
{ 	
	unsigned int i,j;
  	for(i= 0;i<num;i++)	//(SDI5219) ??1ms
		for(j=0;j<164;j++)
			;
}

void key_scan(void) {
  char k = 0;
  Key = 0;

  for (k = 0; k < 5; k++) {
    KEY_D0 = 1;
    KEY_D1 = 1;
    KEY_D2 = 1;
    KEY_D3 = 1;
    KEY_D4 = 1;
    if (k == 0) KEY_D0 = 0;
    if (k == 1) KEY_D1 = 0;
    if (k == 2) KEY_D2 = 0;
    if (k == 3) KEY_D3 = 0;
    if (k == 4) KEY_D4 = 0;
    delay_ms(1);
    if (KEY_K0 != 1) Key = 1 + (k * 4);
    if (KEY_K1 != 1) Key = 2 + (k * 4);
    if (KEY_K2 != 1) Key = 3 + (k * 4);
    if (KEY_K3 != 1) Key = 4 + (k * 4);
  }

  if (LastKey == 0 && Key != 0) {
    KeyState = PRESS; //se presiono
  } else if (LastKey == Key && Key != 0) {
    KeyState = PRESSED; //se mantiene presionado
  } else if (LastKey != 0 && Key == 0) {
    KeyState = RELEASE; //se solto
  } else if (LastKey == 0 && Key == 0) {
    KeyState = 0; //no se ha presionado una tecla
  }
  LastKey = Key;
}

void vBeep_Key(void){

	/*enum digi_key Value_Key_Press;
	
	if(srFlagScale.bCalidadTest == 2){
		if(stScaleParam.cBacklight){OnBackLight;}
	}*/
	
	if(!strTimer.cFLag_TimerA_On){
		strTimer.cFLag_TimerA_End = 0;
		Beep_On_Key;
	}


	/*while(strTimer.cFLag_TimerA_End == 0){
		//IWDG->KR = IWDG_KEY_REFRESH;
	}*/
	
	/*do{
		Value_Key_Press = vActionKey();		
	}while(Value_Key_Press != KEY_NULL);
	
	if(srFlagScale.bCalidadTest == 2){
		//OffBackLight;
	}
	
	KeyPressed = FALSE;*/
}

