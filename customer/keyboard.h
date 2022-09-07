//*************************************************************************//
//* Name				:   display_cal.h
//* Vision				:	V-Nov07_12
//* Project				:   All project
//* Function			:   Solidic ���ӳ� �Դ�����ļ�
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
//--- ���� ��ֵ ���� --
//---ǰ14��------
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
//---������------
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
//--- ���� �ֲ� ���� --
//--- KEY5�ϵİ���---
#define KEY_1_5 PRICE_M2
#define KEY_2_5 PRICE_M1
#define KEY_3_5 PRICE_M3
#define KEY_4_5 PRICE_M4
//--- KEY6�ϵİ���---
#define KEY_1_6 PRICE_M6
#define KEY_2_6 PRICE_M5
#define KEY_3_6 PRICE_M7
#define KEY_4_6 PRICE_M8
//--- KEY7�ϵİ���---
#define KEY_1_7 NUM_EIGHT
#define KEY_2_7 NUM_SEVEN
#define KEY_3_7 NUM_NINE
#define KEY_4_7 CLEAR
//--- KEY8�ϵİ���---
#define KEY_1_8 POINT
#define KEY_2_8 NUM_ZERO
#define KEY_3_8 STORE
#define KEY_4_8 ZERO
//--- KEY9�ϵİ���---
#define KEY_1_9 NUM_FIVE
#define KEY_2_9 NUM_FOUR
#define KEY_3_9 NUM_SIX
#define KEY_4_9 ACCUM
//--- KEY10�ϵİ���---
#define KEY_1_10 NUM_TWO
#define KEY_2_10 NUM_ONE
#define KEY_3_10 NUM_THREE
#define KEY_4_10 TAPE

//------------------------------------------------------------------
//--- ��������������� --
//EXTERN unsigned char Key_code;		//��������
EXTERN bit Key_press;					//��ʾ�а���������
EXTERN bit Key_response;				// ==1 ��ʾһ�ΰ����Ѿ���Ӧ���ˣ��ſ���������ʹ KEY_RESPONSE = 0���Ӷ��п�����һ�α���Ӧ;

//*************************************************************************//
//----------  ����ԭ�� ---------------
//*************************************************************************//
void key_scan(void);

#endif