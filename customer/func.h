//*************************************************************************//
//* Name				:   func.h
//* Vision				:	V-Nov07_12
//* Project				:   ���ӼƼ۳� ϵ�з���
//* Function			:   main����ԭ�� ͷ�ļ�
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
// main.c �к�������
//**************************
void _nop_(void);	//�ղ���

void delay_ms(unsigned int num);	 //��ʱ���� ms

void buzzer(unsigned char mode); 	//����������

void init_cal_point(void); 			//��ȡEEPROM�ı궨�����ݣ���ʼ�궨���ӳ�

void core_memory_write(void);		//��Flash���ݴ���				

void follow_zero_cal(void);

void key_process(void);				//���̴�����

void money_cal(void);				//----������--------------

unsigned int power_detect(void); 	//---����ص�ѹ----

void temp_code_catch(bit mode);		//---�¶ȼ��---

#endif
