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
//#include "./_solidic/head_file_version.h"
#include "./customer/keyboard.h"

extern unsigned char Key;

void key_scan(void)		
{
char k=0;
char d=0;


Key=0;
for(k=0;k<4;k++){
KEY_K0=0;
KEY_K1=0;
KEY_K2=0;
KEY_K3=0;
if(k==1)KEY_K0=1;
if(k==2)KEY_K1=1;
if(k==3)KEY_K2=1;
if(k==4)KEY_K3=1;

if(KEY_D0!=0)Key=1+(k*4);
if(KEY_D1!=0)Key=2+(k*4);
if(KEY_D2!=0)Key=3+(k*4);
if(KEY_D3!=0)Key=4+(k*4);
if(KEY_D4!=0)Key=5+(k*4);
	//delay_ms(5);
}

}
