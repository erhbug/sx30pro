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

#define ADDRESS_PAGE_26 0x6800
#define ADDRESS_PAGE_27 0x6C00
#define ADDRESS_PAGE_28 0x7000
#define ADDRESS_PAGE_29 0x7400

#define MINIMUN_ADDRESS_ALLOWED      ADDRESS_PAGE_27

#define ADDRESS_AUX_PAGE             ADDRESS_PAGE_26 //No usar esta pagina, se usa como auxiliar 
#define ADDRESS_SCALE_CONFIG_PAGE    ADDRESS_PAGE_27
#define ADDRESS_QLTY_AND_CNTRS_PAGE  ADDRESS_PAGE_28
#define ADDRESS_PLU                  ADDRESS_PAGE_29

extern unsigned char xdata NRM_securty_a;						// EEPROM 安全码A
extern unsigned char xdata NRM_securty_b;						// EEPROM 安全码B

//**************************
//EEPROM接口函数
//**************************
//读取EEPROM中的一个字（4字节）；
unsigned char flash_read_u8(unsigned int addr);
unsigned int  flash_read_u16(unsigned int addr);
unsigned long flash_read_u32(unsigned int addr);
float flash_read_float32(unsigned int addr);

void flash_write_u8(unsigned int addr,unsigned char in_data);
void flash_write_u16(unsigned int addr,unsigned int in_data);
void flash_write_u32(unsigned int addr,unsigned long in_data);
void flash_write_float32(unsigned int addr,float in_data);

void e2rom_erase(unsigned int addr);


#endif