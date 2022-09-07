//*************************************************************************//
//* Name				:   SDI5219.h
//* Vision				:	V-Jun19_13
//* Project				:   All project
//* Function			:   
//* $Author				:   superc
//*
//* Copyright 2012 by solidic
//*
//* Recode:
//*						1: 2013-6-19  First Creat By Superc
//*
//*
//*
//*************************************************************************//
#ifndef _SDI5219_H_
#define _SDI5219_H_

//************************************************************************************************************************
// -- 5219 单片机特殊寄存器定义 部分 --
// -- 5219 硬件相关 变量定义
//************************************************************************************************************************

//************************************************************************************************************************
// -- 标准8051 部分 --
//************************************************************************************************************************
//------------ BYTE Registers ------------
//sfr P0    = 0x80;
//sfr P1    = 0x90;
//sfr P2    = 0xA0;
//sfr P3    = 0xB0;
//sfr PSW   = 0xD0;
//sfr ACC   = 0xE0;
//sfr B     = 0xF0;
//sfr SP    = 0x81;
//sfr DPL   = 0x82;
//sfr DPH   = 0x83;
//sfr PCON  = 0x87;
//sfr TCON  = 0x88;
//sfr TMOD  = 0x89;
//sfr TL0   = 0x8A;
//sfr TL1   = 0x8B;
//sfr TH0   = 0x8C;
//sfr TH1   = 0x8D;
//sfr IE    = 0xA8;
//sfr IP    = 0xB8;
//sfr SCON  = 0x98;
//sfr SBUF  = 0x99;
//------------ BIT Registers ------------
/*  PSW  */
//sbit CY    = PSW^7;
//sbit AC    = PSW^6;
//sbit F0    = PSW^5;
//sbit RS1   = PSW^4;
//sbit RS0   = PSW^3;
//sbit OV    = PSW^2;
//sbit P     = PSW^0; //8052 only
/*  TCON  */
//sbit TF1   = TCON^7;
//sbit TR1   = TCON^6;
//sbit TF0   = TCON^5;
//sbit TR0   = TCON^4;
//sbit IE1   = TCON^3;
//sbit IT1   = TCON^2;
//sbit IE0   = TCON^1;
//sbit IT0   = TCON^0;
/*  IE  */
//sbit EA    = IE^7;
//sbit ET2   = IE^5; //8052 only
//sbit ES    = IE^4;
//sbit ET1   = IE^3;
//sbit EX1   = IE^2;
//sbit ET0   = IE^1;
//sbit EX0   = IE^0;
/*  IP  */
//sbit PT2   = IP^5;
//sbit PS    = IP^4;
//sbit PT1   = IP^3;
//sbit PX1   = IP^2;
//sbit PT0   = IP^1;
//sbit PX0   = IP^0;
/*  SCON  */
//sbit SM0   = SCON^7;
//sbit SM1   = SCON^6;
//sbit SM2   = SCON^5;
//sbit REN   = SCON^4;
//sbit TB8   = SCON^3;
//sbit RB8   = SCON^2;
//sbit TI    = SCON^1;
//sbit RI    = SCON^0;


//************************************************************************************************************************
// -- SDI5219 部分 --
//************************************************************************************************************************
//------------ Flash Control Registers ------------
sfr FLASH_DATA = 0xc1;
sfr FLASH_ADDRL = 0xc2;
sfr FLASH_ADDRH = 0xc3;
sfr FLASH_ENA = 0xc4;
sfr FLASH_ENB = 0xc5;
sfr FLASH_ENC = 0xc6;
sfr FLASH_CON = 0xc7;

//------------ IO mode Registers ------------
sfr P0M0    = 0x92;
sfr P0M1    = 0x93;
sfr P1M0    = 0x94;
sfr P1M1    = 0x95;
sfr P2M0    = 0x96;
sfr P2M1    = 0x97;
//--SDI5219X1--
sfr P3M0    = 0x8f;
sfr P3M1    = 0x9f;
sfr IOM_LOCK    = 0xe7;
sfr IOLCD_CON    = 0xd6;
//------------ WatchDog Registers ------------
sfr WDCON   = 0xA9;
sfr WD_TA   = 0xAA;

//------------ SAR ADC Registers ------------
sfr SARCON  = 0xAB;
sfr SARDATA = 0xAC;

//------------ PWM Registers ------------
sfr PWMF_H  = 0x9A;
sfr PWMF_L  = 0x9B;
sfr PWM0  	= 0x9C;
sfr PWM1  	= 0x9D;
sfr PWMCON  = 0x9E;
//--SDI5219X1--
sfr PWM0_H    = 0xad;
sfr PWM1_H    = 0xae;
//------------ Sigma - Dleta ADC Registers ------------
sfr SGADCON = 0xB1;
sfr SGADC3  = 0xB2;
sfr SGADC2  = 0xB3;
sfr SGADC1  = 0xB4;

sfr SGADCON2  = 0xA1;
sfr PD_CON  = 0xA2;
//--SDI5219X1--
sfr SGADCON3  = 0xA3;
sfr SGADCON4  = 0xA4;
sfr SGADCON5  = 0xA6;
sfr PD_CON2  = 0xA5;


//------------ Ext Int Registers ------------
sfr EXIF    = 0x91;
sfr EICON   = 0xD8;
sfr EIE     = 0xE8;
sfr EIP    	= 0xF8;

//------------ LCD - Charge-pump Registers ------------
//--SDI5219X1--
sfr LCD_D0    = 0xf9;
sfr LCD_D1    = 0xfa;
sfr LCD_D2    = 0xfb;
sfr LCD_D3    = 0xfc;
sfr LCD_D4    = 0xfd;
sfr LCD_D5    = 0xfe;
sfr LCD_D6    = 0xff;
sfr LCD_D7    = 0xf1;
sfr LCD_CON    = 0xeb;
sfr LCD_SEL    = 0xec;

//------------ Other Registers ------------
sfr DPL1    = 0x84;
sfr DPH1    = 0x85;
sfr DPS    	= 0x86;

sfr CKCON   = 0x8E;

#endif