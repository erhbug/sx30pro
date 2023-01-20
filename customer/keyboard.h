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


extern unsigned char idata KeyState;
extern unsigned char idata Key;

void key_scan(void);
void delay_ms(unsigned int num);
void vScan_Key(void);
float vCapture_Valor_Test(unsigned char fNew_Digit, unsigned char cDecimal_Number, float fValue_Capture);
void vCapture_Valor(float *pfValor, float fNew_Digit, int iNumber_Max_Digit,unsigned char cCounter_Decimal);
void vActionMemoryPlu(unsigned char cIndexMemory);
//unsigned char cConvert_Char(unsigned char Key_Num);

#endif