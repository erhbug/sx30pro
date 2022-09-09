//*************************************************************************//
//* Name				:   ext_data.h
//* Vision				:	V-Nov07_12
//* Project				:   电子计价秤 系列方案
//* Function			:   全局变量 头文件
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
#ifndef	__DATA__
#define	__DATA__

#ifndef  EXTERN 
#define  EXTERN extern
#else 
	#define EXTERN 
#endif

//************************************************************************************************************************
// -- 定义 预编译 选项 部分 --
//************************************************************************************************************************

//--- 温度检测 ---
#define TEMP_DET_EN					//启用该宏，表示编译 "温度显示功能"

//------------------------------------------------------------------
#define ADC_OUT_20Hz	//使用 该宏定义 。定义 SDI0819 的输出频率：20Hz

//************************************************************************************************************************
// -- MCU IO 接口定义定义 部分 --
//************************************************************************************************************************
//------------ 配置adc的接口 ------------
sbit DRDY_SDO = P0^0;
sbit SCLK = P0^2;
//------------ 配置蜂鸣器的接口 ------------
sbit BUZZER_P = P1^5;
//------------ 配置LCD的接口 ------------
sbit LCD_DATA = P1^2;
sbit LCD_WR_N = P1^0;
sbit LCD_CS_N = P1^1;
//------------ 配置LCD-背光的接口 ------------
sbit LCD_LAMP = P1^4;
EXTERN bit Lcd_lamp_symbol;		//用于 确定液晶背光灯是否开启
EXTERN unsigned char Lcd_bright;	//显示亮度变量
#define  LCD_LAMP_B4		0xff	//最高亮度(持续为高)
#define  LCD_LAMP_B3		0x60	//3亮度	
#define  LCD_LAMP_B2		0x30	//2亮度
#define  LCD_LAMP_B1		0x10	//1亮度
//--有NPN--
#define  LCD_LAMP_ON		{LCD_LAMP = 1;Lcd_lamp_symbol = 1; PWMCON  |= 0x04;}		// (通过PWM控制背光灯)背光灯打开  (用三极管和不用三极管,逻辑正好是反的,定义宏方便修改)
#define  LCD_LAMP_OFF		{LCD_LAMP = 0;Lcd_lamp_symbol = 0; PWMCON  &= 0xfb;}		// (通过PWM控制背光灯)背光灯关闭
//------------ 配置 交流充电检测的接口 ------------
sbit CHARGE_SYMBOL = P1^3;
//------------ 配置 电池电压检测的接口 ------------
sbit BAT_DET_PIN = P0^5;
#define  BAT_DET_PIN_HIGH		(BAT_DET_PIN = 1)		// 电压检测 口线 变高
#define  BAT_DET_PIN_LOW		(BAT_DET_PIN = 0)		// 电压检测 口线 变低
//------------ 配置 键盘的接口 ------------

//------------ IO口操作(宏定义) ------------
//关键管脚的IO配置
//-- P1^3(CHARGE_SYMBOL)		集电极开路 
//-- P0^5(BAT_DET_PIN)			纯输入
//-- P1^4(LCD_LAMP) 			强驱动
//-- P1^5(BUZZER_P) 			强驱动
//P0.0(DRDY_SDO)集电极开路 P0.2(SCLK) 强驱动; 
#define  IO_MODE_RESTORE_A_EN {P0M0 &= 0xdf;P0M0 |= 0x05;P0M1 |= 0x21;P0M1 &= 0xfb;P1M0 |= 0x38;P1M1 &= 0xcf;P1M1 |= 0x08;};

//开启显示IO正常 //P1.2(LCD_DATA-) P1.0(LCD_WR_N-) P1.1(LCD_CS_N-) 强驱动;
#define  IO_MODE_DSP_EN	{P1M0 |= 0x07;P1M1 &= 0xf8;}	
//关闭显示IO,设定显示IO 
#define  IO_MODE_DSP_DIS {LCD_CS_N = 1;LCD_WR_N = 1;LCD_DATA = 1;}	

//开启 键盘IO正常(按键检测部分) // P2^2(KEY_PIN1),P2^1(KEY_PIN2),P2^3(KEY_PIN3),P2^0(KEY_PIN4) 普通上拉;	
#define  IO_MODE_KEY_A_EN	 {P2M0 &= 0xf0;P2M1 &= 0xf0;}	
// KEY_PIN5-KEY_PIN10 强驱动
#define  IO_MODE_KEY_B_EN	 {P0M0 |= 0x1a;P0M1 &= 0xe5;P1M0 |= 0x40;P1M1 &= 0xbf;P2M0 |= 0x10;P2M1 &= 0xef;}	
//关闭 键盘IO // KEY_PIN1-KEY_PIN10配置为纯模拟输入,输出1; 
//例外: (CHARGE_SYMBOL集电极开路) (BUZZER_P不能配置为1和模拟输入)  (和显示复用的线不能配置为模拟输入)
#define  IO_MODE_KEY_DIS {P0M0 &= 0xe5;P0M1 |= 0x1a;P1M0 &= 0xbf;P1M1 |= 0x40;P2M0 &= 0xe0;P2M1 |= 0x1f;KEY_PIN5 = 1;KEY_PIN6 = 1;KEY_PIN7 = 1;KEY_PIN8 = 1;KEY_PIN10 = 1;}	

//---定义SG-ADC的工作模式--
//--5216--
#define SGADCON_DEFAULT	0x34	//(关闭SOC-SGADC,温度通道,160Hz)定义 "SGADCON" 的默认值
#define PD_CON_DEFAULT  0x18	//(锁定sgadcon，sgadcon2)(关闭ALDO,关闭温度)定义 "PD_CON" 的默认值 
//************************************************************************************************************************
// -- 系统配置变量 --
// -- EEPROM 空间  --
//************************************************************************************************************************
//--- EEPROM 块定义 --
#define ADDR_CAL_DATA 0x6800		// (26区)用于存放 核心标定数据
#define ADDR_CAL_BACKUP 0x6c00		// (27区) 该区用于 核心标定数据 备份 
#define ADDR_SYS_DATA 0x7000		// (28区) 该区用于存放   系统密码 厂家信息 等
#define ADDR_PRICE_DATA_A 0x7400	// (29区) 用于存放价格数据 PRICE_M1 - PRICE_M8

//--- Xdata ---
//--- SYS DATA 相关定义 ---
#define  SYS_DATA_LENTH 10		// SYS 数据的 字节数
#define  TEMP_INIT 3300			//(SDI5216-3.3) 默认的温度参考电压
union	SYS_pattern	//定义联合体,数据可以采用字节和字两种方式访问；
{
	struct
	{  
		unsigned long Sys_passwd;
		unsigned long Sys_telnum;
		unsigned int  Temp_vref;
	} sys_struct;
	unsigned char b[SYS_DATA_LENTH];
};
EXTERN union SYS_pattern xdata System_data;


//-----------------------------------------
//-- 电池电压检测 时间变量  -- (时间属性) ---
EXTERN unsigned int Vol_auto_det_counter;
EXTERN bit Power_det_mode; 					//== 1 表示 检测电压时，不关闭显示
//--
#ifdef ADC_OUT_20Hz	
	#define VOL_AUTO_DET_NUM 2400			//(时间属性)
#endif
#ifdef ADC_OUT_10Hz
	#define VOL_AUTO_DET_NUM 120			//(时间属性)
#endif

//*************************************************************************//
//----------  基本秤重变量 ------ 
//*************************************************************************//
EXTERN bit ADCcmd;

EXTERN bit Data_sycle_symbol;		//用于 定义是否是数据周期, = 1表示 新的数据周期

EXTERN unsigned char Key_code;

#define V_CO_PRE  984 	

//-----------------------------------------
//-- 温度检测定义  ---
//--参考电压3.31v的数据-- (760/1度)
//-10	23100xx
// 0	23172xx
//10	23249xx
//20	23322xx
//30	23401xx
#define TEMP_VREF_0	3310			//(3.31V参考)参考电压	
#define TMEP_ZERO_CODE_0 2317200	//(3.31V参考)温度0时的输出码
#define TEMP_PER_D_CODE_0 760			//(3.31V参考)温度变化1度时的输出码
//-----------
#define ZERO_SIGNAL_CODE 2097152	//0x800000 / 4 理论上信号为0时的输出码	
#define TEMP_CODE_DELTA (TMEP_ZERO_CODE_0 - ZERO_SIGNAL_CODE)

//--SDI5219X1--
//--参考电压3.31v的数据-- (730/1度)
//-10	23095xx
// 0	23162xx
//10	23232xx
//20	23302xx
//30	23376xx
#define TEMP_VREF_0_X1	3310			//(3.31V参考)参考电压	
#define TMEP_ZERO_CODE_0_X1 2316200	//(3.31V参考)温度0时的输出码
#define TEMP_PER_D_CODE_0_X1 730			//(3.31V参考)温度变化1度时的输出码
//-----------
#define ZERO_SIGNAL_CODE_X1 2097152	//0x800000 / 4 理论上信号为0时的输出码	
#define TEMP_CODE_DELTA_X1 (TMEP_ZERO_CODE_0_X1 - ZERO_SIGNAL_CODE_X1)


#ifdef TEMP_DET_EN
	EXTERN char xdata Temperature;	//温度变量
	EXTERN unsigned int xdata Temp_auto_det_counter;
	#ifdef ADC_OUT_20Hz	
//		#define TEMP_AUTO_DET_NUM 2400			//(时间属性) 两分钟
		#define TEMP_AUTO_DET_NUM 2400			//(时间属性) 两分钟
	#endif
	#ifdef ADC_OUT_10Hz
		#define TEMP_AUTO_DET_NUM 1200			//(时间属性) 两分钟
	#endif
EXTERN bit Cal_temp_sign; 						//输入补偿时确定正负符号
#endif



//-- SDI5219X1 兼容部分 ---
EXTERN bit SDI5219X1_symbol;				// = 1 是SDI5219X1系列; = 0是老版本的SDI5219

#endif

