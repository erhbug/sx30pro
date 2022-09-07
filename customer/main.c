//*************************************************************************//
//* Name				:   main.c
//* Vision				:	V-Nov07_12
//* Project				:   All project
//* Function			:   Solidic 电子称 主文件
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
#pragma code symbols debug oe
#define EXTERN
#include <REG52.H>
#include <math.h>
#include "./_solidic/head_file_version.h"


main()
{
//************************************************************************************************************************
// -- 初始化系统 --
//************************************************************************************************************************
//--按键关机,兼容SDI5219X1和SDI5219的代码--Begin--
//--SDI5219X1看门狗复位并不对自身复位，所以为了兼容SDI5219的按键关机0.2s看门狗复位,必须一开机就将看门狗配置到4秒,否则会有一直重启
//--因为SDI5219X1按键关机0.2s看门狗复位后，原来程序要到0.5s后才操作看门狗，所以看门狗会在操作它之前重新复位
	//---打开看门狗
	EA = 0;
	WD_TA = 0x05;
	WD_TA = 0x0a;
	WDCON = 0xbf;	//看门狗时间 4s钟,低频唤醒0.2s
	//---判断是否是SDI5219-X1
	if((LCD_CON == 0x5c) && (LCD_SEL == 0xd0))
		SDI5219X1_symbol = 1;
	else
		SDI5219X1_symbol = 0;
//--兼容SDI5219X1和SDI5219的代码--End--
	P0 = 0xff;
	P1 = 0xdf;	//P1.5(BUZZER_P) = 0; P1.4(LCD_LAMP) = 1;
	P2 = 0xff;
	SCLK = 0;	
//--初始化PWM--
	PWMF_H  = 0x00;
	PWMF_L  = 0xff;
	PWM0  	= LCD_LAMP_B2;
	PWM1  	= 0xff;	
	PWMCON  = 0x04;	//PWM0-P1.4(LCD_LAMP)输出波形(当PWM0=0xff时,输出高电平)
//--配置IO模式 
	//配置P1.1 P1.2为正常上拉
	P1M0 = 0x00;
	P1M1 = 0x00;
	//配置P0 为 纯模拟输入
	P0M0 = 0x00;
	P0M1 = 0xff;
	//配置P1 为 纯模拟输入
	P1M0 = 0x00;
	P1M1 = 0xff;
	//配置P2.0 P2.1 只输入
	P2M0 = 0x00;
	P2M1 = 0xff;
//--关键管脚的IO配置
	IO_MODE_RESTORE_A_EN;
//---配置Sigma_delta ADC---
	//--配置ADC---
//	(5216不需要打开SOC-SGADC中断)EIE |= 0x04;		//打开SG ADC 中断
	SGADCON = SGADCON_DEFAULT;
	PD_CON = PD_CON_DEFAULT;
//--初始化定时器
	TMOD = 0x11;	//定时器0、1都工作在16位状态下				
	TH0 = 0x80;                       
	TL0 = 0x00;
	//TH1 = 0x00;                       
	//TL1 = 0x00;
	//TR0  = 1;	//开计数器0,用于控制键盘扫描					
	//TR1  = 1;	//开计数器1,用于时间控制			
//--配置中断
	IT0 = 0;		//设定外部中断0 采用低电平触发
	//IT1 = 0;		//设定外部中断1 采用低电平触发
	//IE   = 0x82;	//仅仅开 计时器中断0
	IE   = 0x81;	//仅仅开 外部中断0
	//--注意，将能唤醒STOP1-2的中断要配置为最高优先级
	//--配置中断优先级
	IP = 0x05;		//外部中断0,1为高优先级
	EIP = 0x06;		//低频唤醒，24位ADC中断为高优先级
//---变量初始化---

//---Xdata 变量初始化 ---
//--- 条件编译: 温度检测 -- begin ---
#ifdef TEMP_DET_EN
	Temperature = 25;
	Temp_auto_det_counter = 0;
#endif
//--- 条件编译: 温度检测 -- end ---
//--延时
	delay_ms(500);
//--SDI5219---
	//---打开看门狗
	EA = 0;
	WD_TA = 0x05;
	WD_TA = 0x0a;
	WDCON = 0x1f;	//(SDI5216)看门狗时间 4s钟,(关闭)低频唤醒0.2s
	EA = 1;
	//--打开低频唤醒中断
//--- 条件编译: 温度检测 -- begin ---
#ifdef TEMP_DET_EN
	//--温度检测--
	temp_code_catch(0);
#endif
//--- 条件编译: 温度检测 -- end ---

//************************************************************************************************************************
// -- 处理主循环 --
//************************************************************************************************************************
	while(1)
	{
		//--关键管脚的IO配置
		IO_MODE_RESTORE_A_EN;
		//************************************************
		//---喂狗--及其他安全处理-begin-
		//************************************************
		EA = 0;
		WD_TA = 0x05;
		WD_TA = 0x0a;
		WDCON = 0x1f;	//(SDI5216)看门狗时间 4s钟,(关闭)低频唤醒0.2s
		EA = 1;
		//-- 开启ADC的外部中断 --
		EX0 = 1;

		//************************************************
		//---按键处理-----------------begin---------------
		//************************************************
		if( Key_press && (!Key_response) )
		{	
			key_scan();
			//Flash 安全验证码 A
			NRM_securty_a = 0xaa;	//Flash 安全验证码 A
			if(Key_code == TAPE)
			{
				unsigned char temp;
				//Flash 安全验证码 B
				NRM_securty_b = 0x55;	//Flash 安全验证码 B
				//--Flash 写入测试 --
				temp = nvm_data_read_byte(ADDR_SYS_DATA + 0);
				temp ++;
				e2rom_erase(ADDR_SYS_DATA); //擦除
				nvm_data_write_byte(ADDR_SYS_DATA + 0,55);
				Key_response = 1;
			}
		}
		//************************************************
		//---按键处理-----------------end---------------
		//************************************************


		//************************************************
		//---(时间相关功能块)-----------------begin-------
		//************************************************		
		if(Data_sycle_symbol)
		{
			Data_sycle_symbol = 0;
			//-- 测试电压 --
			if(Vol_auto_det_counter >= VOL_AUTO_DET_NUM) //电池电压检测
			{
				power_detect();
				Vol_auto_det_counter = 0;
			}
			else
			{
				Vol_auto_det_counter ++;
			}

		//--- 条件编译: 温度检测 -- begin ---
		#ifdef TEMP_DET_EN
			if(Temp_auto_det_counter >= TEMP_AUTO_DET_NUM) //电池电压检测
			{
				//--温度检测--
				temp_code_catch(1);
				Temp_auto_det_counter = 0;
			}
			else
			{
				Temp_auto_det_counter ++;
			}
		#endif
		//--- 条件编译: 温度检测 -- end ---
		}

		//************************************************
		//---(时间相关功能块)-----------------end-------
		//************************************************

//------ 清除 Flash 安全码 -- beign --
		NRM_securty_a = 0x00;	//Flash 安全验证码 A
		NRM_securty_b = 0x00;	//Flash 安全验证码 B
//------ 清除 Flash 安全码 -- end --

		//************************************************
		//---  休眠处理 ----------------- begin -------
		//************************************************

		//注意: 每次循环MCU停在此处，由外部中断唤醒
		PCON |= 0x10; //STOP2 ----MCU将停在此处	

		//************************************************
		//---  休眠处理 ----------------- end -------
		//************************************************

		//************************************************
		//---  键盘以及IO检测 ----------------- begin ----
		//************************************************
		EA = 0;
		//--IO口配置--
		IO_MODE_KEY_A_EN;
		// 防止出错，将 KEY_PIN1 - KEY_PIN4 写1
		KEY_PIN1 = 1;	
		KEY_PIN2 = 1;	
		KEY_PIN3 = 1;	
		KEY_PIN4 = 1;
		// 检测是否有健按下
		KEY_PIN5 = 0;
		KEY_PIN6 = 0;
		KEY_PIN7 = 0;
		KEY_PIN8 = 0;
		//KEY_PIN9 = 0;
		KEY_PIN10 = 0;
		delay_ms(0);
		// 如果 KEY_1,KEY_2,KEY_3,KEY_4为1,则说明没有键被按下
		if(KEY_PIN1 && KEY_PIN2 && KEY_PIN3 && KEY_PIN4)
		{
			Key_press = 0;
			Key_response = 0;
		}
		else
	 	{
			Key_press = 1;															
	   	}
		//--IO口配置--
		IO_MODE_KEY_DIS;
		EA = 1;
		//************************************************
		//---  键盘以及IO检测 ----------------- end ----
		//************************************************				
	}//---while(1)
}//----main()


//--------------------------------
//----- 延时函数,num=1,延时1ms ---
//--SDI5219---
void delay_ms(unsigned int num)
{ 	
	unsigned int i,j;
  	for(i= 0;i<num;i++)	//(SDI5219) 延时1ms
		for(j=0;j<164;j++)
			;
}

//----------------------------------------------------------
//---   检测电池电压   ----
unsigned int power_detect(void)
{
	unsigned int v;
	EA = 0;
//--- 
	BAT_DET_PIN_HIGH; // 电池电压检测脚变高, 否则电池电压检测将不正常
	//--测试通道1的电压
	SARCON  = 0x09;  //通道1 速度最快 打开Sar-ADC
	delay_ms(1);
	
	if(!(SARCON & 0x04)) // ADC不忙则开始转换
	{
		SARCON |= 0x04;
		while(SARCON & 0x04) // 等待转换结束
		{
		}
	}

	v = SARDATA;
	v = (unsigned long)v * (unsigned long)V_CO_PRE / 0xff; // 计算电池电压

//--- 恢复 相关IO口配置 ----
	//-- 如果 电压检测管脚和键盘管脚公用的话，需要将键盘所有检测IO变为 [仅输入模式]

	SARCON  &= 0xf7;; //关闭ADC
	EA = 1;
	return(v);
}


//0819 24bit SG ADC 中断,读取adc数据；*****************
static void int0_serve(void) interrupt 0
{		
	EA = 0;	 // 禁止中断
	ADCcmd = 0;
	data_receive(); // 读取 SDI0818 的 转换数据
	ADcode_pre &= 0x00ffffff;
	ADcode_pre >>= 2;
	Data_sycle_symbol = 1;	//新的数据周期
	IE0 = 0;	// 将读写数据时 的下降沿 触发的中断信息 去除
	EA = 1;  // 打开中断
}


//--- 条件编译: 温度检测 -- begin ---
#ifdef TEMP_DET_EN
// -- 温度检测 通道 --
// mode = 1: 需要通道1延迟
// mode = 0: 不需要通道1延迟
//-----------
void temp_code_catch(bit mode)
{ 
	unsigned long  adcode_last = 0;
	union  ADpattern xdata temp;
	unsigned char i = 0;
	unsigned char j = 0;
	bit temp_sign;
//-----------------
	unsigned long  per_temp_code_acture = TEMP_PER_D_CODE_0;	
		
	EA = 0; //禁止中断
	ADCcmd = 0;	
	//-- 温度通道 -- 80Hz --
	PD_CON &= 0xe7;		//解锁 SGADCON,SGADCON2
	PD_CON |= 0x07;		//开启温度通道
	SGADCON = 0xf4; 	//温度通道,160Hz
	EXIF &= 0xbf;		//清除标志
//	delay_ms(1);
	//-- 得到温度编码 --
	ADcode_pre = 0;
	while (1)
	{
		//---喂狗--及其他安全处理-begin-
		WD_TA = 0x05;
		WD_TA = 0x0a;
		WDCON = 0x1f;	//看门狗时间 4s钟,(关闭)低频唤醒0.2s
		//---喂狗--及其他安全处理-end-
		
		while((EXIF & 0x40) == 0x00)	//如果没有数据
			;
		EXIF &= 0xbf;		//清除标志
		//-----------
		// 读取  的 转换数据
		temp.b[1] = SGADC3;
		temp.b[2] = SGADC2;
		temp.b[3] = SGADC1;
		temp.b[0] = 0;
		temp.w ^= 0x800000;			// 因为输出为双极性，+0x800000将负端平移上来
		temp.w &= 0x00ffffff;
		temp.w >>= 2;

		//-----------
		j ++;
		if(labs(temp.w - adcode_last) <= 0x200)	
		{
			ADcode_pre += temp.w;
			i++;
		}
		else 
		{
			adcode_last = temp.w;
			ADcode_pre = 0;
			i = 0;
		}

		if(i >= 4)
		{
			adcode_last = ADcode_pre >> 2;
			break;
		}
		else
		if(j >= 10)
		{
			adcode_last = temp.w;
			break;
		}
	}
	//-- 温度计算 --
	if(SDI5219X1_symbol)
	{
	temp.w = (unsigned long)(TEMP_CODE_DELTA_X1) * (unsigned long)(TEMP_VREF_0_X1) / (unsigned long)(System_data.sys_struct.Temp_vref);
	temp.w += ZERO_SIGNAL_CODE_X1;
	per_temp_code_acture = (unsigned long)(TEMP_PER_D_CODE_0_X1) * (unsigned long)(TEMP_VREF_0_X1) / (unsigned long)(System_data.sys_struct.Temp_vref);
	}
	else
	{
	temp.w = (unsigned long)(TEMP_CODE_DELTA) * (unsigned long)(TEMP_VREF_0) / (unsigned long)(System_data.sys_struct.Temp_vref);
	temp.w += ZERO_SIGNAL_CODE;
	per_temp_code_acture = (unsigned long)(TEMP_PER_D_CODE_0) * (unsigned long)(TEMP_VREF_0) / (unsigned long)(System_data.sys_struct.Temp_vref);
	}
	//--test-
	//Price = per_temp_code_acture; //temp.w / 100;
	//--test-

	Temperature = 0x00;
	if(adcode_last >= temp.w)	//零度以上
	{
		ADcode_pre = 	adcode_last - temp.w;
		temp_sign = 0;
	}
	else
	{
		ADcode_pre = 	temp.w - adcode_last;
		temp_sign = 1;
	}
	Temperature = ADcode_pre / per_temp_code_acture;
	if(Temperature >= 70)	//限制在+/- 70度的范围
		Temperature = 70;
	if(temp_sign)
		Temperature = -Temperature;
	//-- 通道恢复 --
	SGADCON = SGADCON_DEFAULT; 	//1通道,20Hz
	PD_CON = PD_CON_DEFAULT;
	EXIF &= 0xbf;		//清除标志
	i = 0;
	if(mode)	//需要通道1延迟
	{
		while (i<5)
		{
			//---喂狗--及其他安全处理-begin-
			WD_TA = 0x05;
			WD_TA = 0x0a;
			WDCON = 0x1f;	//看门狗时间 4s钟,(关闭)低频唤醒0.2s
			//---喂狗--及其他安全处理-end-
			while(!IE0)	//如果没有数据
				;
			IE0 = 0;		//清除标志
			i++;
	  	}
	}
	EA = 1; //打开按键中断		
}
#endif
//--- 条件编译: 温度检测 -- end ---
