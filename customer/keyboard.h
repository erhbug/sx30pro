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

#include <REG52.H>

//------------------------------------------------------------------

sbit KEY_K0 = P0^2;	
sbit KEY_K1 = P2^4;	
sbit KEY_K2 = P2^0;	
sbit KEY_K3 = P1^6;	

sbit KEY_D0 = P0^1;
sbit KEY_D1 = P0^0;	
sbit KEY_D2 = P1^0;	
sbit KEY_D3 = P1^1;	
sbit KEY_D4 = P1^2;

#define PRESS 1
#define PRESSED 2
#define RELEASE  3

enum digi_key{ 																													//Definicion Teclas para LPCRN
			KEY_NULL,
			KEY_CERO,KEY_TARA,KEY_M1,KEY_RCL,KEY_CAMBIO,KEY_MAS,KEY_M2,
			KEY_M,KEY_7,KEY_4,KEY_1,KEY_C,KEY_8,KEY_5,KEY_2,KEY_0,KEY_9,
			KEY_6,KEY_3,KEY_PUNTO,
		};


//*************************************************************************//
//----------  函数原型 ---------------
//*************************************************************************//
void key_scan(void);
void delay_ms(unsigned int num);
void vBeep_Key(void);
void vSound_Saved_Param(void);

extern unsigned char KeyState;
extern unsigned char Key;

#endif