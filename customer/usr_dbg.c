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
//#include <math.h>
//**************************************************************************
// -- ȫ��ͷ�ļ� ���й��� ---- ���汾�޸��ļ�����䶯 ---  �����޸� ����ļ�
//**************************************************************************
#include "./_solidic/head_file_version.h"//comentar
//#include "./customer/usr_dbg.h"
#include "./_scale/dvr_def.h"
#include "./_scale/dvr_scale.h"
#include "./customer/beeper.h"
#include "./customer/dvr_plu.h"
#include "./customer/dvr_registradora.h"
#include <math.h>
#include <stdio.h>


void usr_dbg(char *txt, unsigned int Time){
	#ifdef USR_DBG
	LCD_GLASS_String(txt, LCD_TOTAL); 

	strTimer.iTimerDBG=1;
	while (strTimer.iTimerDBG<Time)
	{
		IWDG_KEY_REFRESH; 
	}strTimer.iTimerDBG=0;
	
	//delay_ms(Time);
	#endif
}