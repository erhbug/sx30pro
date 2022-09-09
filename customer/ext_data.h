//*************************************************************************//
//* Name				:   ext_data.h
//* Vision				:	V-Nov07_12
//* Project				:   ���ӼƼ۳� ϵ�з���
//* Function			:   ȫ�ֱ��� ͷ�ļ�
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
// -- ���� Ԥ���� ѡ�� ���� --
//************************************************************************************************************************

//--- �¶ȼ�� ---
#define TEMP_DET_EN					//���øú꣬��ʾ���� "�¶���ʾ����"

//------------------------------------------------------------------
#define ADC_OUT_20Hz	//ʹ�� �ú궨�� ������ SDI0819 �����Ƶ�ʣ�20Hz

//************************************************************************************************************************
// -- MCU IO �ӿڶ��嶨�� ���� --
//************************************************************************************************************************
//------------ ����adc�Ľӿ� ------------
sbit DRDY_SDO = P0^0;
sbit SCLK = P0^2;
//------------ ���÷������Ľӿ� ------------
sbit BUZZER_P = P1^5;
//------------ ����LCD�Ľӿ� ------------
sbit LCD_DATA = P1^2;
sbit LCD_WR_N = P1^0;
sbit LCD_CS_N = P1^1;
//------------ ����LCD-����Ľӿ� ------------
sbit LCD_LAMP = P1^4;
EXTERN bit Lcd_lamp_symbol;		//���� ȷ��Һ��������Ƿ���
EXTERN unsigned char Lcd_bright;	//��ʾ���ȱ���
#define  LCD_LAMP_B4		0xff	//�������(����Ϊ��)
#define  LCD_LAMP_B3		0x60	//3����	
#define  LCD_LAMP_B2		0x30	//2����
#define  LCD_LAMP_B1		0x10	//1����
//--��NPN--
#define  LCD_LAMP_ON		{LCD_LAMP = 1;Lcd_lamp_symbol = 1; PWMCON  |= 0x04;}		// (ͨ��PWM���Ʊ����)����ƴ�  (�������ܺͲ���������,�߼������Ƿ���,����귽���޸�)
#define  LCD_LAMP_OFF		{LCD_LAMP = 0;Lcd_lamp_symbol = 0; PWMCON  &= 0xfb;}		// (ͨ��PWM���Ʊ����)����ƹر�
//------------ ���� ���������Ľӿ� ------------
sbit CHARGE_SYMBOL = P1^3;
//------------ ���� ��ص�ѹ���Ľӿ� ------------
sbit BAT_DET_PIN = P0^5;
#define  BAT_DET_PIN_HIGH		(BAT_DET_PIN = 1)		// ��ѹ��� ���� ���
#define  BAT_DET_PIN_LOW		(BAT_DET_PIN = 0)		// ��ѹ��� ���� ���
//------------ ���� ���̵Ľӿ� ------------

//------------ IO�ڲ���(�궨��) ------------
//�ؼ��ܽŵ�IO����
//-- P1^3(CHARGE_SYMBOL)		���缫��· 
//-- P0^5(BAT_DET_PIN)			������
//-- P1^4(LCD_LAMP) 			ǿ����
//-- P1^5(BUZZER_P) 			ǿ����
//P0.0(DRDY_SDO)���缫��· P0.2(SCLK) ǿ����; 
#define  IO_MODE_RESTORE_A_EN {P0M0 &= 0xdf;P0M0 |= 0x05;P0M1 |= 0x21;P0M1 &= 0xfb;P1M0 |= 0x38;P1M1 &= 0xcf;P1M1 |= 0x08;};

//������ʾIO���� //P1.2(LCD_DATA-) P1.0(LCD_WR_N-) P1.1(LCD_CS_N-) ǿ����;
#define  IO_MODE_DSP_EN	{P1M0 |= 0x07;P1M1 &= 0xf8;}	
//�ر���ʾIO,�趨��ʾIO 
#define  IO_MODE_DSP_DIS {LCD_CS_N = 1;LCD_WR_N = 1;LCD_DATA = 1;}	

//���� ����IO����(������ⲿ��) // P2^2(KEY_PIN1),P2^1(KEY_PIN2),P2^3(KEY_PIN3),P2^0(KEY_PIN4) ��ͨ����;	
#define  IO_MODE_KEY_A_EN	 {P2M0 &= 0xf0;P2M1 &= 0xf0;}	
// KEY_PIN5-KEY_PIN10 ǿ����
#define  IO_MODE_KEY_B_EN	 {P0M0 |= 0x1a;P0M1 &= 0xe5;P1M0 |= 0x40;P1M1 &= 0xbf;P2M0 |= 0x10;P2M1 &= 0xef;}	
//�ر� ����IO // KEY_PIN1-KEY_PIN10����Ϊ��ģ������,���1; 
//����: (CHARGE_SYMBOL���缫��·) (BUZZER_P��������Ϊ1��ģ������)  (����ʾ���õ��߲�������Ϊģ������)
#define  IO_MODE_KEY_DIS {P0M0 &= 0xe5;P0M1 |= 0x1a;P1M0 &= 0xbf;P1M1 |= 0x40;P2M0 &= 0xe0;P2M1 |= 0x1f;KEY_PIN5 = 1;KEY_PIN6 = 1;KEY_PIN7 = 1;KEY_PIN8 = 1;KEY_PIN10 = 1;}	

//---����SG-ADC�Ĺ���ģʽ--
//--5216--
#define SGADCON_DEFAULT	0x34	//(�ر�SOC-SGADC,�¶�ͨ��,160Hz)���� "SGADCON" ��Ĭ��ֵ
#define PD_CON_DEFAULT  0x18	//(����sgadcon��sgadcon2)(�ر�ALDO,�ر��¶�)���� "PD_CON" ��Ĭ��ֵ 
//************************************************************************************************************************
// -- ϵͳ���ñ��� --
// -- EEPROM �ռ�  --
//************************************************************************************************************************
//--- EEPROM �鶨�� --
#define ADDR_CAL_DATA 0x6800		// (26��)���ڴ�� ���ı궨����
#define ADDR_CAL_BACKUP 0x6c00		// (27��) �������� ���ı궨���� ���� 
#define ADDR_SYS_DATA 0x7000		// (28��) �������ڴ��   ϵͳ���� ������Ϣ ��
#define ADDR_PRICE_DATA_A 0x7400	// (29��) ���ڴ�ż۸����� PRICE_M1 - PRICE_M8

//--- Xdata ---
//--- SYS DATA ��ض��� ---
#define  SYS_DATA_LENTH 10		// SYS ���ݵ� �ֽ���
#define  TEMP_INIT 3300			//(SDI5216-3.3) Ĭ�ϵ��¶Ȳο���ѹ
union	SYS_pattern	//����������,���ݿ��Բ����ֽں������ַ�ʽ���ʣ�
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
//-- ��ص�ѹ��� ʱ�����  -- (ʱ������) ---
EXTERN unsigned int Vol_auto_det_counter;
EXTERN bit Power_det_mode; 					//== 1 ��ʾ ����ѹʱ�����ر���ʾ
//--
#ifdef ADC_OUT_20Hz	
	#define VOL_AUTO_DET_NUM 2400			//(ʱ������)
#endif
#ifdef ADC_OUT_10Hz
	#define VOL_AUTO_DET_NUM 120			//(ʱ������)
#endif

//*************************************************************************//
//----------  �������ر��� ------ 
//*************************************************************************//
EXTERN bit ADCcmd;

EXTERN bit Data_sycle_symbol;		//���� �����Ƿ�����������, = 1��ʾ �µ���������

EXTERN unsigned char Key_code;

#define V_CO_PRE  984 	

//-----------------------------------------
//-- �¶ȼ�ⶨ��  ---
//--�ο���ѹ3.31v������-- (760/1��)
//-10	23100xx
// 0	23172xx
//10	23249xx
//20	23322xx
//30	23401xx
#define TEMP_VREF_0	3310			//(3.31V�ο�)�ο���ѹ	
#define TMEP_ZERO_CODE_0 2317200	//(3.31V�ο�)�¶�0ʱ�������
#define TEMP_PER_D_CODE_0 760			//(3.31V�ο�)�¶ȱ仯1��ʱ�������
//-----------
#define ZERO_SIGNAL_CODE 2097152	//0x800000 / 4 �������ź�Ϊ0ʱ�������	
#define TEMP_CODE_DELTA (TMEP_ZERO_CODE_0 - ZERO_SIGNAL_CODE)

//--SDI5219X1--
//--�ο���ѹ3.31v������-- (730/1��)
//-10	23095xx
// 0	23162xx
//10	23232xx
//20	23302xx
//30	23376xx
#define TEMP_VREF_0_X1	3310			//(3.31V�ο�)�ο���ѹ	
#define TMEP_ZERO_CODE_0_X1 2316200	//(3.31V�ο�)�¶�0ʱ�������
#define TEMP_PER_D_CODE_0_X1 730			//(3.31V�ο�)�¶ȱ仯1��ʱ�������
//-----------
#define ZERO_SIGNAL_CODE_X1 2097152	//0x800000 / 4 �������ź�Ϊ0ʱ�������	
#define TEMP_CODE_DELTA_X1 (TMEP_ZERO_CODE_0_X1 - ZERO_SIGNAL_CODE_X1)


#ifdef TEMP_DET_EN
	EXTERN char xdata Temperature;	//�¶ȱ���
	EXTERN unsigned int xdata Temp_auto_det_counter;
	#ifdef ADC_OUT_20Hz	
//		#define TEMP_AUTO_DET_NUM 2400			//(ʱ������) ������
		#define TEMP_AUTO_DET_NUM 2400			//(ʱ������) ������
	#endif
	#ifdef ADC_OUT_10Hz
		#define TEMP_AUTO_DET_NUM 1200			//(ʱ������) ������
	#endif
EXTERN bit Cal_temp_sign; 						//���벹��ʱȷ����������
#endif



//-- SDI5219X1 ���ݲ��� ---
EXTERN bit SDI5219X1_symbol;				// = 1 ��SDI5219X1ϵ��; = 0���ϰ汾��SDI5219

#endif

