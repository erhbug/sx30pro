//*************************************************************************//
//* Name				:   display_cal.c
//* Vision				:	V-Nov07_12
//* Project				:   All project
//* Function			:   Solidic ????? ???????l?
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
//**************************************************************************
// -- ?????l? ??????? ---- ????????l?????? ---  ??????? ????l?
//**************************************************************************
#include "./customer/beeper.h"
#include "./_scale/dvr_def.h"


idata strTimers strTimer;


void vBeep_Key(void){
		Beep_On_Key;
}



void vSound_Saved_Param(void){
	unsigned char i = 0;
	
	for(i=0; i<3; i++){vBeep_Key();delay_ms(250);}
}

