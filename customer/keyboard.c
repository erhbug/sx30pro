//*************************************************************************//
//* Name				:   display_cal.c
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
#include <REG52.H>
#include <math.h>
//**************************************************************************
// -- ȫ��ͷ�ļ� ���й��� ---- ���汾�޸��ļ�����䶯 ---  �����޸� ����ļ�
//**************************************************************************
#include "./_solidic/head_file_version.h"

//****************************************************
//----ȫ�ֱ��� Dsp_buf[]:  16�ֽ��Դ�
//----
//****************************************************




//----------------------------------------------------
//----- ����ɨ�躯�� ------------
/*sbit KEY_K0 = P0^2;	
sbit KEY_K1 = P2^4;	
sbit KEY_K2 = P2^0;	
sbit KEY_K3 = P1^6;	

sbit KEY_D0 = P0^1;
sbit KEY_D1 = P0^0;	
sbit KEY_D2 = P1^0;	
sbit KEY_D3 = P1^1;	
sbit KEY_D4 = P1^2;*/
void key_scan0(void)		
{
char k=0;
char d=0;

Key_code=0;
for(k=1;k<=4;k++){
KEY_K0=0;
KEY_K1=0;
KEY_K2=0;
KEY_K3=0;
if(k=1)KEY_K0=1;
if(k=2)KEY_K1=1;
if(k=3)KEY_K2=1;
if(k=4)KEY_K3=1;

if(KEY_D0!=0)Key_code=1*k;
if(KEY_D1!=0)Key_code=2*k;
if(KEY_D2!=0)Key_code=3*k;
if(KEY_D3!=0)Key_code=4*k;
if(KEY_D4!=0)Key_code=5*k;
	delay_ms(5);
}

}



