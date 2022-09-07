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
#include "./_solidic/head_file_version.h"

//****************************************************
//----全局变量 Dsp_buf[]:  16字节显存
//----
//****************************************************




//----------------------------------------------------
//----- 键码扫描函数 ------------
void key_scan(void)		
{
	EA = 0;
	delay_ms(5);
	// 6ms 延时后再次检测是否有健按下
	// 检测是否有健按下
	//--IO口配置--
	IO_MODE_KEY_A_EN;
	IO_MODE_KEY_B_EN;
	//------------
	KEY_PIN5 = 0;
	KEY_PIN6 = 0;
	KEY_PIN7 = 0;
	KEY_PIN8 = 0;
	//KEY_PIN9 = 0;
	KEY_PIN10 = 0;
	delay_ms(0);
	// 如果 KEY_1,KEY_2,KEY_3,KEY_4为1,则说明没有键被按下
	if(KEY_PIN1 && KEY_PIN2 && KEY_PIN3 && KEY_PIN4)
	{
		Key_code = NONE;
	}
	else
	{
//----- 扫描列线 gnd(KEY9) -------------------------------------------
		KEY_PIN5 = 1;
		KEY_PIN6 = 1;
		KEY_PIN7 = 1;
		KEY_PIN8 = 1;
		//KEY_PIN9 = 1;
		KEY_PIN10 = 1;
		delay_ms(0);
		//--- 判断按键 ---
		if(!KEY_PIN1)
		    Key_code = KEY_1_9;
		else
		if(!KEY_PIN2)
			Key_code = KEY_2_9;
		else
		if(!KEY_PIN3)
			Key_code = KEY_3_9;
		else
		if(!KEY_PIN4)
			Key_code = KEY_4_9;
		else
			Key_code = NONE;
//----- 扫描列线 KEY10(优先扫描和蜂鸣器公用的线) -------------------------------------------
		if(Key_code == NONE)
		{
		KEY_PIN10 = 0;
		delay_ms(0);
		//--- 判断按键 ---
		if(!KEY_PIN1)
		    Key_code = KEY_1_10;
		else
		if(!KEY_PIN2)
			Key_code = KEY_2_10;
		else
		if(!KEY_PIN3)
			Key_code = KEY_3_10;
		else
		if(!KEY_PIN4)
			Key_code = KEY_4_10;
		else
			Key_code = NONE;	
		}
//----- 扫描列线 KEY8 -------------------------------------------
		if(Key_code == NONE)
		{
		KEY_PIN8 = 0;
		delay_ms(0);
		//--- 判断按键 ---
		if(!KEY_PIN1)
		    Key_code = KEY_1_8;
		else
		if(!KEY_PIN2)
			Key_code = KEY_2_8;
		else
		if(!KEY_PIN3)
			Key_code = KEY_3_8;
		else
		if(!KEY_PIN4)
			Key_code = KEY_4_8;
		else
			Key_code = NONE;	
		}
//----- 扫描列线 KEY7 -------------------------------------------
		if(Key_code == NONE)
		{
		KEY_PIN7 = 0;
		delay_ms(0);
		//--- 判断按键 ---
		if(!KEY_PIN1)
		    Key_code = KEY_1_7;
		else
		if(!KEY_PIN2)
			Key_code = KEY_2_7;
		else
		if(!KEY_PIN3)
			Key_code = KEY_3_7;
		else
		if(!KEY_PIN4)
			Key_code = KEY_4_7;
		else
			Key_code = NONE;	
		}
//----- 扫描列线 KEY6 -------------------------------------------
		if(Key_code == NONE)
		{
		KEY_PIN6 = 0;
		delay_ms(0);
		//--- 判断按键 ---
		if(!KEY_PIN1)
		{
			Key_code = KEY_1_6;
		}
		else
		if(!KEY_PIN2)
		{
			Key_code = KEY_2_6;
		}
		else
		if(!KEY_PIN3)
		{
			Key_code = KEY_3_6;
		}
		else
		if(!KEY_PIN4)
		{
			Key_code = KEY_4_6;
		}
		else
			Key_code = NONE;	
		}
//----- 扫描列线 KEY5 -------------------------------------------
		if(Key_code == NONE)
		{
		KEY_PIN5 = 0;
		delay_ms(0);
		//--- 判断按键 ---
		if(!KEY_PIN1)
		    Key_code = KEY_1_5;
		else
		if(!KEY_PIN2)
			Key_code = KEY_2_5;
		else
		if(!KEY_PIN3)
			Key_code = KEY_3_5;
		else
		if(!KEY_PIN4)
			Key_code = KEY_4_5;
		else
			Key_code = NONE;	
		}
	}
	Key_press = 0;
	//--IO口配置--
	IO_MODE_KEY_DIS;
	EA = 1;
}
