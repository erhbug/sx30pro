//*************************************************************************//
//* Name				:   func.h
//* Vision				:	V-Nov07_12
//* Project				:   电子计价秤 系列方案
//* Function			:   main函数原型 头文件
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

#ifndef	_Func_
#define _Func_

//**************************
// main.c 中函数定义
//**************************
void _nop_(void);	//空操作

void delay_ms(unsigned int num);	 //延时函数 ms

void buzzer(unsigned char mode); 	//蜂鸣器函数

void init_cal_point(void); 			//读取EEPROM的标定点数据，初始标定电子秤

void core_memory_write(void);		//将Flash数据存入				

void follow_zero_cal(void);

void key_process(void);				//键盘处理函数

void money_cal(void);				//----金额计算--------------

unsigned int power_detect(void); 	//---检测电池电压----

void temp_code_catch(bit mode);		//---温度检测---

#endif
