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
#include "./_solidic/solidic_Vc_Jan10_13.h"							
#include "./_solidic/SDI5219_Vc_Sep02_15.h"	
#include "./_data_nvm/data_nvm_5219_Vc_Dec09_13.h"	
#include "./_scale/dvr_def.h"
#include "./_display/dvr_lcd_SDI1621.h"
#include "./customer/keyboard.h"

unsigned char NRM_securty_a;						// EEPROM 安全码A
unsigned char NRM_securty_b;	

void ClearThisButAll(unsigned int Address, unsigned int IntCount);

volatile void nop(void){
 unsigned int x=1;
 x<<=1;
}

unsigned char flash_read_u8(unsigned int addr)
{
	unsigned char val;
	val = CBYTE[addr];	
	return(val);
}

unsigned int  flash_read_u16(unsigned int addr)
{
	unsigned int val;
		unsigned char *ptr;
		unsigned char i;
	
		ptr=(unsigned char *)&val;
		for(i=0;i<2;i++)
		{  
		  *(ptr++) =flash_read_u8(addr++);
		}
		return(val);
}

unsigned long  flash_read_u32(unsigned int addr)
{
    unsigned long val;
		unsigned char *ptr;
		unsigned char i;
	
		ptr=(unsigned char *)&val;
		for(i=0;i<4;i++)
		{  
		  *(ptr++) =flash_read_u8(addr++);
		}
		return(val);
}

float flash_read_float32(unsigned int addr)
	{
	    float val;
		unsigned char *ptr;
		unsigned char i;
	
		ptr=(unsigned char *)&val;
		for(i=0;i<4;i++)
		{  
		  *(ptr++) =flash_read_u8(addr++);
		}
		return(val);
	}


void nvm_data_write_byte(unsigned int addr,unsigned char in_data)
{
	union INTpattern flash_addr;
	bit ea_save;
	unsigned char i=0;

    IWDG_KEY_REFRESH;
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
		nop();	//延时
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

	 

/*	if(flash_read_u8(addr)!=in_data)
	{  
	  LCD_GLASS_String("-MEM-", LCD_TOTAL);
	  while(1)delay_ms(1);

	}*/
}

void flash_write_u8(unsigned int addr,unsigned char in_data)
{
  unsigned char read=0;
	
	/****************************/
	read=flash_read_u8(addr);
	if( read!=0xff ){
	  ClearThisButAll(addr,1);	
	  LCD_GLASS_String("CTBAL1", LCD_TOTAL);delay_ms(3000);
	  }	
	/****************************/
	  nvm_data_write_byte(addr,in_data);	
}

void flash_write_u16(unsigned int addr,unsigned int in_data)
{
	unsigned char *ptr;
	unsigned char i;
	unsigned int aux;
	unsigned char txt[8];

	/****************************/
	aux=flash_read_u16(addr);
	if( aux!=0xffff ){
	  ClearThisButAll(addr,2);	
      sprintf(txt,"%X",addr);
      LCD_GLASS_String(txt, LCD_PESO);
	  LCD_GLASS_String("CTBAL2", LCD_TOTAL);
	  delay_ms(3000);
	}	
	/****************************/

	/****************************/	
	aux=flash_read_u16(addr);
    ptr=(unsigned char *)&aux;
	if( *(ptr++)!=0xFF || *(ptr++)!=0xFF)
	  ClearThisButAll(addr,2);		
	/****************************/

	ptr=(unsigned char *)&in_data;
	for(i=0;i<2;i++)
	{
	  nvm_data_write_byte(addr++,*(ptr++));
	}	
}

void flash_write_u32(unsigned int addr,unsigned long in_data)
{
	unsigned char *ptr;
	unsigned char i;
	unsigned long read=0;
	read=flash_read_u32(addr);

	/****************************/
	if( read!=0xffffffff ){
	  ClearThisButAll(addr,4);
	LCD_GLASS_String("CTBAL3", LCD_TOTAL);delay_ms(3000);
	  }		
	/****************************/

	ptr=(unsigned char *)&in_data;
	for(i=0;i<4;i++)
	{
	  nvm_data_write_byte(addr++,*(ptr++));
	}	
}

void flash_write_float32(unsigned int addr,float in_data)
{
	unsigned char *ptr;
	unsigned char i;
	float aux;
	unsigned long read=0;
	read=flash_read_u32(addr);

	/****************************/
	if( read!=0xffffffff ){
	  ClearThisButAll(addr,4);	
	  LCD_GLASS_String("CTBAL4", LCD_TOTAL);delay_ms(3000);
	  }	
	/****************************/

	ptr=(unsigned char *)&in_data;
	for(i=0;i<4;i++)
	{
	  nvm_data_write_byte(addr++,*(ptr++));
	}	
}

//EEPROM BLOCK(1k) 擦除
//addr = （0 - 31）* 1024 ,擦除对应的Block地址
//调用前需要:
//NRM_securty_a,NRM_securty_b
//flash操作关闭总中断，操作完后会开启总中断(注意)
void e2rom_erase(unsigned int addr)
{unsigned char i=0;
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
	nop();
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

void ClearThisButAll(unsigned int Address, unsigned int IntCount){
	unsigned char Aux=0;	
	unsigned int i=0;
	unsigned int Page;	
	//unsigned int SizePage=0x400;

	/*Determinar en que pagina se encuentra*/
	Page=Address/PAGE_SIZE;
	Page*=PAGE_SIZE;
	/**************************************/

	/*Verificar que la pagina auxiliar este limpia, de lo contrario borrarla*/
	for(i=0;i<10;i++){		
		if(flash_read_u8(ADDRESS_AUX_PAGE+i) != 0xFF)
			e2rom_erase(ADDRESS_AUX_PAGE);//vSend_String_Usart(&huart1,"Pag Auxiliar Limpia\r\n");		
	}	
	/***********************************************************************/
	
	/*Copiar de la pagina origen a la auxiliar*/
	for(i=0;i<PAGE_SIZE;i++){
		if(i+Page==Address)/*Copiar SizeToCopy enteros excepto IntCount enteros a partir de Address */
			i+=IntCount;	
		nvm_data_write_byte(ADDRESS_AUX_PAGE + i, flash_read_u8(Page + i));		
  }
	/****************************************/	
	
	/*Borrar pagina de origen*/
	e2rom_erase(Page);	
	/*************************/

	/*Regresar datos a pagina origen*/
	for(i=0;i<PAGE_SIZE;i++){
		nvm_data_write_byte(Page + i, flash_read_u8(ADDRESS_AUX_PAGE + i));		
	}
	/********************************/

	/*Borrar pagina auxiliar*/
	e2rom_erase(ADDRESS_AUX_PAGE);	
	/************************/
}



