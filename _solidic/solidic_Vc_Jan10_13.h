//*************************************************************************//
//*
//* Project				:   All project
//* Function			:   Solidic 通用 头文件
//* $Author				:   superc
//* $Name				:   solidic.h
//* Copyright 2012 by solidic
//*
//* Recode:
//*						1: 2012-11-07  First Creat By Superc
//*
//*
//*
//*************************************************************************//
#ifndef _SOLIDIC_H_
#define _SOLIDIC_H_



/*****************基本数据类型***************************/
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;

/*****************其他数据类型***************************/
union	ADpattern				//定义联合体,数据可以采用字节和字两种方式访问；
{
	unsigned long	w ;
	unsigned char   b[4];
};

//---------
union	INTpattern				//定义联合体,数据可以采用字节和字两种方式访问；
{
	unsigned int	i ;
	unsigned char   b[2];
};

//---------
typedef enum _bool
{
    FALSE,TURE
} boolean;
#define TURE ( 1 == 1 )
#define FALSE ( 1 == 0 )




#endif // _SOLIDIC_H_
