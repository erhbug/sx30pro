//*************************************************************************//
//* Name				:   data_nvm_825.h
//* Vision				:	V-Nov07_12
//* Project				:   华邦 825 系列单片机
//* Function			:   Solidic 电子称 非易失性数据 操作
//* $Author				:   superc
//*
//* Copyright 2012 by solidic
//*
//* Recode:
//*						1: 2013-01-10  First Creat By Superc
//*
//*
//*
//*************************************************************************//

#ifndef _DATA_NRM_SDI5219_H_
#define _DATA_NRM_SDI5219_H_


EXTERN unsigned char xdata NRM_securty_a;						// EEPROM 安全码A
EXTERN unsigned char xdata NRM_securty_b;						// EEPROM 安全码B
//-- NRM_securty_temp1 这个变量如可能的话开启开好 --
//EXTERN unsigned char idata NRM_securty_temp1  _at_ 0xc4;		// 征用0xc4的RAM，防止和FLASH_ENA冲突
//-- NRM_securty_temp2/3 这个变量不开没关系
//EXTERN unsigned char idata NRM_securty_temp2  _at_ 0xc5;		// 征用0xc5的RAM，防止和FLASH_ENB冲突
//EXTERN unsigned char idata NRM_securty_temp3  _at_ 0xc6;		// 征用0xc6的RAM，防止和FLASH_ENC冲突

//**************************
//EEPROM接口函数
//**************************
//读取EEPROM中的一个字（4字节）；
unsigned long nvm_data_read(unsigned int addr);
//往EEPROM中写入一个字（4字节）；
void nvm_data_write(unsigned int addr, unsigned long in_data);
//读取EEPROM中的一个字节
unsigned char nvm_data_read_byte(unsigned int addr);
//往EEPROM中写入一个字节
void nvm_data_write_byte(unsigned int addr, unsigned char in_data);
//EEPROM擦除函数；
void e2rom_erase(unsigned int addr);


#endif