//*************************************************************************//
//* Name				:   data_nvm_825.c
//* Vision				:	V-Jun20_13
//* Project				:   
//* Function			:   Solidic 电子称 非易失性数据 操作
//* $Author				:   superc
//*
//* Copyright 2012 by solidic
//*
//* Recode:
//*						1: 2013-06-20  First Creat By Superc
//*
//*
//*
//*************************************************************************//

#include <REG52.H>
#include <absacc.h>
#include <math.h>
//***************************************************************************
// -- 全部头文件 集中管理 ---- ，版本修改文件名会变动 ---  必须修改 这个文件
//***************************************************************************
#include "./_solidic/head_file_version.h"

//****************************************************
//-------------其他模块全局变量----------------------
//----全局变量  : 		//(头文件)
//****************************************************

//****************************************************
//EEPROM接口函数
//****************************************************
/*
//读取EEPROM中的一个字（4字节）；
unsigned long nvm_data_read(unsigned int addr)
{
	unsigned char	i;
	union ADpattern d;	 //定义联合体,数据可以采用字节和字两种方式访问；
	
	for (i=0;i<4;i++)
  	{
		d.b[i] = nvm_data_read_byte(addr);	
		addr ++;	
	}
	return(d.w);
}

//往EEPROM中写入一个字（4字节）；
void nvm_data_write(unsigned int addr, unsigned long in_data)
{
	unsigned char i;
	union ADpattern d;
	d.w = in_data;

	for (i=0;i<4;i++)
  	{
		FLASH_ENA = 0x05;	//nvm_data_write_byte 函数中无这句话
		nvm_data_write_byte(addr,d.b[i]);
		addr ++;		
	}
}
*/

//读取EEPROM中的一个字节；
unsigned char nvm_data_read_byte(unsigned int addr)
{
	unsigned char	i;
	i = CBYTE[addr];	
	return(i);
}

//往EEPROM中写入一个字节
//调用前需要:
//NRM_securty_a,NRM_securty_b
//flash操作关闭总中断，操作完后会开启总中断(注意)
void nvm_data_write_byte(unsigned int addr, unsigned char in_data)
{
	union INTpattern flash_addr;
	bit ea_save;
	flash_addr.i = addr;
   	ea_save  =  EA;            // Save EA
	EA = 0;
	//-- 准备地址和数据 --
	FLASH_ADDRH = flash_addr.b[0]; // point to the address you want to erase 
	FLASH_ADDRL = flash_addr.b[1]; 
	FLASH_DATA = in_data;
	//-- 状态清除 --
	FLASH_ENA = 0x00;
	FLASH_ENB = 0x00;
	FLASH_ENC = 0x00;
	//-- 通过安全检验后才能启动 Flash 操作 --
	if((NRM_securty_a == 0xaa)&&(NRM_securty_b == 0x55))
		FLASH_CON = 0x03;	//状态清除，执行一次FLASH_CON，将安全状态复位
	_nop_();	//延时
	//-- 写安全码开启Flash操作许可 --		
	FLASH_ENA = 0x05;
	FLASH_ENB = 0x0a;
	FLASH_ENC = 0x09;
	//-- 通过安全检验后才能启动 Flash 操作 --
	if((NRM_securty_a == 0xaa)&&(NRM_securty_b == 0x55))
		FLASH_CON = 0x03;
	//-- 清除安全码 --
	FLASH_ENA = 0x00;
	FLASH_ENB = 0x00;
	FLASH_ENC = 0x00;
	EA = ea_save;
}

//EEPROM BLOCK(1k) 擦除
//addr = （0 - 31）* 1024 ,擦除对应的Block地址
//调用前需要:
//NRM_securty_a,NRM_securty_b
//flash操作关闭总中断，操作完后会开启总中断(注意)
void e2rom_erase(unsigned int addr)
{
	union INTpattern flash_addr;
	bit ea_save;
	flash_addr.i = addr;
   	ea_save  =  EA;            // Save EA
	EA = 0;
	FLASH_ADDRH = flash_addr.b[0]; // point to the address you want to erase 
	FLASH_ADDRL = flash_addr.b[1]; 
	//-- 状态清除 --
	FLASH_ENA = 0x00;
	FLASH_ENB = 0x00;
	FLASH_ENC = 0x00;
	//-- 通过安全检验后才能启动 Flash 操作 --
	if((NRM_securty_a == 0xaa)&&(NRM_securty_b == 0x55))
		FLASH_CON = 0x03;	//状态清除，执行一次FLASH_CON，将安全状态复位
	_nop_();
	//-- 写安全码开启Flash操作许可 --
	FLASH_ENA = 0x05;
	FLASH_ENB = 0x0a;
	FLASH_ENC = 0x09;
	//-- 通过安全检验后才能启动 Flash 操作 --
	if((NRM_securty_a == 0xaa)&&(NRM_securty_b == 0x55))
		FLASH_CON = 0x0c;
	//-- 清除安全码 --
	FLASH_ENA = 0x00;
	FLASH_ENB = 0x00;
	FLASH_ENC = 0x00;
	EA = ea_save;	
}