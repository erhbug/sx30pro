//*************************************************************************//
//* Name				:   display_cal.h
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
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_




//------------------------------------------------------------------
//--- 键盘 键值 定义 --
//---前14键------
#define  NUM_ZERO 	0
#define  NUM_ONE 	1
#define  NUM_TWO	2
#define  NUM_THREE 	3
#define  NUM_FOUR 	4
#define  NUM_FIVE 	5
#define  NUM_SIX 	6
#define  NUM_SEVEN 	7
#define  NUM_EIGHT 	8
#define  NUM_NINE 	9
#define  ACCUM		10
#define  CLEAR		11
#define  TAPE		12
#define  ZERO		13
//---其他键------
#define  POINT		14
#define  STORE		15
#define  BRIGHT		16
#define  VOL_VERIFY	17
#define  PRICE_M1	18
#define  PRICE_M2	19
#define  PRICE_M3	20
#define  PRICE_M4	21
#define  PRICE_M5	22
#define  PRICE_M6	23
#define  PRICE_M7	24
#define  PRICE_M8	25
#define  RESTORE	26
#define  UV_KEY		27
#define  PAY_KEY	28
#define  BACK_LIGHT 29
#define  MODE_KEY	30
#define  SAMPLE_KEY 31
#define  POWER_KEY 	32

#define  NONE		255

//------------------------------------------------------------------
//--- 键盘 分布 定义 --
//--- KEY5上的按键---
#define KEY_1_5 PRICE_M2
#define KEY_2_5 PRICE_M1
#define KEY_3_5 PRICE_M3
#define KEY_4_5 PRICE_M4
//--- KEY6上的按键---
#define KEY_1_6 PRICE_M6
#define KEY_2_6 PRICE_M5
#define KEY_3_6 PRICE_M7
#define KEY_4_6 PRICE_M8
//--- KEY7上的按键---
#define KEY_1_7 NUM_EIGHT
#define KEY_2_7 NUM_SEVEN
#define KEY_3_7 NUM_NINE
#define KEY_4_7 CLEAR
//--- KEY8上的按键---
#define KEY_1_8 POINT
#define KEY_2_8 NUM_ZERO
#define KEY_3_8 STORE
#define KEY_4_8 ZERO
//--- KEY9上的按键---
#define KEY_1_9 NUM_FIVE
#define KEY_2_9 NUM_FOUR
#define KEY_3_9 NUM_SIX
#define KEY_4_9 ACCUM
//--- KEY10上的按键---
#define KEY_1_10 NUM_TWO
#define KEY_2_10 NUM_ONE
#define KEY_3_10 NUM_THREE
#define KEY_4_10 TAPE

sbit KEY_K0 = P0^2;	
sbit KEY_K1 = P2^4;	
sbit KEY_K2 = P2^0;	
sbit KEY_K3 = P1^6;	

sbit KEY_D0 = P0^1;
sbit KEY_D1 = P0^0;	
sbit KEY_D2 = P1^0;	
sbit KEY_D3 = P1^1;	
sbit KEY_D4 = P1^2;


//*************************************************************************//
//----------  函数原型 ---------------
//*************************************************************************//
void key_scan(void);

#endif