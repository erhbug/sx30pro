//*************************************************************************//
//*
//* Project				:   All project
//* Function				:   Solidic 通用 头文件
//* $Author				:   superc
//* $Name				:   head_file_version
//* Copyright 2012 by solidic
//*
//* Recode:
//*						1: 2012-11-07  First Creat By Superc
//*
//*
//*
//*************************************************************************//
#ifndef _HEAD_FILE_VERSION_H_
#define _HEAD_FILE_VERSION_H_

//************************************************************************************************************************
// -- 全部头文件 集中管理 ---- ，版本修改文件名会变动 ---  必须修改 这个文件
// -- 客户 程序 文件 没有版本 号
// -- 只修改 系统 以及 通用模块
//************************************************************************************************************************

#include "./_solidic/solidic_Vc_Jan10_13.h"								//solidic 系统 头文件
#include "./_solidic/SDI5219_Vc_Sep02_15.h"								//solidic 系统 头文件
//#include "./_adc/adc_0819_Vc_Jan10_13.h"								//驱动-程序 高精度ADC 头文件
#include "./_data_nvm/data_nvm_5219_Vc_Dec09_13.h"								//solidic 系统 头文件

#include "./_display/dvr_lcd_SDI1621.h"
#include "./_weight/dvr_HX712.h"
#include "./customer/keyboard.h"
#include "./_scale/dvr_scale.h"


#endif // _HEAD_FILE_VERSION_H_
