//*************************************************************************//
//* Name				:   main.c
//* Vision				:	V-Nov07_12
//* Project				:   All project
//* Function			:   Solidic ���ӳ� ���ļ�
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
	unsigned int i,j,num=90;
	P1M0 = 0xff;
	P1M1 = 0x00;
while(1){	
	P1 = 0x04;
  	for(i= 0;i<num;i++)	//(SDI5219) ��ʱ1ms
		for(j=0;j<164;j++)
			;
	P1 = 0x00;
  	for(i= 0;i<num;i++)	//(SDI5219) ��ʱ1ms
		for(j=0;j<164;j++)
			;
}

//************************************************************************************************************************
// -- ��ʼ��ϵͳ --
//************************************************************************************************************************
//--�����ػ�,����SDI5219X1��SDI5219�Ĵ���--Begin--
//--SDI5219X1���Ź���λ����������λ������Ϊ�˼���SDI5219�İ����ػ�0.2s���Ź���λ,����һ�����ͽ����Ź����õ�4��,�������һֱ����
//--��ΪSDI5219X1�����ػ�0.2s���Ź���λ��ԭ������Ҫ��0.5s��Ų������Ź������Կ��Ź����ڲ�����֮ǰ���¸�λ
	//---�򿪿��Ź�
	EA = 0;
	WD_TA = 0x05;
	WD_TA = 0x0a;
	WDCON = 0xbf;	//���Ź�ʱ�� 4s��,��Ƶ����0.2s
	//---�ж��Ƿ���SDI5219-X1
	if((LCD_CON == 0x5c) && (LCD_SEL == 0xd0))
		SDI5219X1_symbol = 1;
	else
		SDI5219X1_symbol = 0;
//--����SDI5219X1��SDI5219�Ĵ���--End--
	P0 = 0xff;
	P1 = 0xdf;	//P1.5(BUZZER_P) = 0; P1.4(LCD_LAMP) = 1;
	P2 = 0xff;
	SCLK = 0;	
//--��ʼ��PWM--
	PWMF_H  = 0x00;
	PWMF_L  = 0xff;
	PWM0  	= LCD_LAMP_B2;
	PWM1  	= 0xff;	
	PWMCON  = 0x04;	//PWM0-P1.4(LCD_LAMP)�������(��PWM0=0xffʱ,����ߵ�ƽ)
//--����IOģʽ 
	//����P1.1 P1.2Ϊ��������
	P1M0 = 0x00;
	P1M1 = 0x00;
	//����P0 Ϊ ��ģ������
	P0M0 = 0x00;
	P0M1 = 0xff;
	//����P1 Ϊ ��ģ������
	P1M0 = 0x00;
	P1M1 = 0xff;
	//����P2.0 P2.1 ֻ����
	P2M0 = 0x00;
	P2M1 = 0xff;
//--�ؼ��ܽŵ�IO����
	IO_MODE_RESTORE_A_EN;
//---����Sigma_delta ADC---
	//--����ADC---
//	(5216����Ҫ��SOC-SGADC�ж�)EIE |= 0x04;		//��SG ADC �ж�
	SGADCON = SGADCON_DEFAULT;
	PD_CON = PD_CON_DEFAULT;
//--��ʼ����ʱ��
	TMOD = 0x11;	//��ʱ��0��1��������16λ״̬��				
	TH0 = 0x80;                       
	TL0 = 0x00;
	//TH1 = 0x00;                       
	//TL1 = 0x00;
	//TR0  = 1;	//��������0,���ڿ��Ƽ���ɨ��					
	//TR1  = 1;	//��������1,����ʱ�����			
//--�����ж�
	IT0 = 0;		//�趨�ⲿ�ж�0 ���õ͵�ƽ����
	//IT1 = 0;		//�趨�ⲿ�ж�1 ���õ͵�ƽ����
	//IE   = 0x82;	//������ ��ʱ���ж�0
	IE   = 0x81;	//������ �ⲿ�ж�0
	//--ע�⣬���ܻ���STOP1-2���ж�Ҫ����Ϊ������ȼ�
	//--�����ж����ȼ�
	IP = 0x05;		//�ⲿ�ж�0,1Ϊ�����ȼ�
	EIP = 0x06;		//��Ƶ���ѣ�24λADC�ж�Ϊ�����ȼ�
//---������ʼ��---

//---Xdata ������ʼ�� ---
//--- ��������: �¶ȼ�� -- begin ---
#ifdef TEMP_DET_EN
	Temperature = 25;
	Temp_auto_det_counter = 0;
#endif
//--- ��������: �¶ȼ�� -- end ---
//--��ʱ
	delay_ms(500);
//--SDI5219---
	//---�򿪿��Ź�
	EA = 0;
	WD_TA = 0x05;
	WD_TA = 0x0a;
	WDCON = 0x1f;	//(SDI5216)���Ź�ʱ�� 4s��,(�ر�)��Ƶ����0.2s
	EA = 1;
	//--�򿪵�Ƶ�����ж�
//--- ��������: �¶ȼ�� -- begin ---
#ifdef TEMP_DET_EN
	//--�¶ȼ��--
	temp_code_catch(0);
#endif
//--- ��������: �¶ȼ�� -- end ---

//************************************************************************************************************************
// -- ������ѭ�� --
//************************************************************************************************************************
	while(1)
	{
		//--�ؼ��ܽŵ�IO����
		IO_MODE_RESTORE_A_EN;
		//************************************************
		//---ι��--��������ȫ����-begin-
		//************************************************
		EA = 0;
		WD_TA = 0x05;
		WD_TA = 0x0a;
		WDCON = 0x1f;	//(SDI5216)���Ź�ʱ�� 4s��,(�ر�)��Ƶ����0.2s
		EA = 1;
		//-- ����ADC���ⲿ�ж� --
		EX0 = 1;

		//************************************************
		//---��������-----------------begin---------------
		//************************************************
		if( Key_press && (!Key_response) )
		{	
			key_scan();
			//Flash ��ȫ��֤�� A
			NRM_securty_a = 0xaa;	//Flash ��ȫ��֤�� A
			if(Key_code == TAPE)
			{
				unsigned char temp;
				//Flash ��ȫ��֤�� B
				NRM_securty_b = 0x55;	//Flash ��ȫ��֤�� B
				//--Flash д����� --
				temp = nvm_data_read_byte(ADDR_SYS_DATA + 0);
				temp ++;
				e2rom_erase(ADDR_SYS_DATA); //����
				nvm_data_write_byte(ADDR_SYS_DATA + 0,55);
				Key_response = 1;
			}
		}
		//************************************************
		//---��������-----------------end---------------
		//************************************************


		//************************************************
		//---(ʱ����ع��ܿ�)-----------------begin-------
		//************************************************		
		if(Data_sycle_symbol)
		{
			Data_sycle_symbol = 0;
			//-- ���Ե�ѹ --
			if(Vol_auto_det_counter >= VOL_AUTO_DET_NUM) //��ص�ѹ���
			{
				power_detect();
				Vol_auto_det_counter = 0;
			}
			else
			{
				Vol_auto_det_counter ++;
			}

		//--- ��������: �¶ȼ�� -- begin ---
		#ifdef TEMP_DET_EN
			if(Temp_auto_det_counter >= TEMP_AUTO_DET_NUM) //��ص�ѹ���
			{
				//--�¶ȼ��--
				temp_code_catch(1);
				Temp_auto_det_counter = 0;
			}
			else
			{
				Temp_auto_det_counter ++;
			}
		#endif
		//--- ��������: �¶ȼ�� -- end ---
		}

		//************************************************
		//---(ʱ����ع��ܿ�)-----------------end-------
		//************************************************

//------ ��� Flash ��ȫ�� -- beign --
		NRM_securty_a = 0x00;	//Flash ��ȫ��֤�� A
		NRM_securty_b = 0x00;	//Flash ��ȫ��֤�� B
//------ ��� Flash ��ȫ�� -- end --

		//************************************************
		//---  ���ߴ��� ----------------- begin -------
		//************************************************

		//ע��: ÿ��ѭ��MCUͣ�ڴ˴������ⲿ�жϻ���
		PCON |= 0x10; //STOP2 ----MCU��ͣ�ڴ˴�	

		//************************************************
		//---  ���ߴ��� ----------------- end -------
		//************************************************

		//************************************************
		//---  �����Լ�IO��� ----------------- begin ----
		//************************************************
		EA = 0;
		//--IO������--
		IO_MODE_KEY_A_EN;
		// ��ֹ������ KEY_PIN1 - KEY_PIN4 д1
		KEY_PIN1 = 1;	
		KEY_PIN2 = 1;	
		KEY_PIN3 = 1;	
		KEY_PIN4 = 1;
		// ����Ƿ��н�����
		KEY_PIN5 = 0;
		KEY_PIN6 = 0;
		KEY_PIN7 = 0;
		KEY_PIN8 = 0;
		//KEY_PIN9 = 0;
		KEY_PIN10 = 0;
		delay_ms(0);
		// ��� KEY_1,KEY_2,KEY_3,KEY_4Ϊ1,��˵��û�м�������
		if(KEY_PIN1 && KEY_PIN2 && KEY_PIN3 && KEY_PIN4)
		{
			Key_press = 0;
			Key_response = 0;
		}
		else
	 	{
			Key_press = 1;															
	   	}
		//--IO������--
		IO_MODE_KEY_DIS;
		EA = 1;
		//************************************************
		//---  �����Լ�IO��� ----------------- end ----
		//************************************************				
	}//---while(1)
}//----main()


//--------------------------------
//----- ��ʱ����,num=1,��ʱ1ms ---
//--SDI5219---
void delay_ms(unsigned int num)
{ 	
	unsigned int i,j;
  	for(i= 0;i<num;i++)	//(SDI5219) ��ʱ1ms
		for(j=0;j<164;j++)
			;
}

//----------------------------------------------------------
//---   ����ص�ѹ   ----
unsigned int power_detect(void)
{
	unsigned int v;
	EA = 0;
//--- 
	BAT_DET_PIN_HIGH; // ��ص�ѹ���ű��, �����ص�ѹ��⽫������
	//--����ͨ��1�ĵ�ѹ
	SARCON  = 0x09;  //ͨ��1 �ٶ���� ��Sar-ADC
	delay_ms(1);
	
	if(!(SARCON & 0x04)) // ADC��æ��ʼת��
	{
		SARCON |= 0x04;
		while(SARCON & 0x04) // �ȴ�ת������
		{
		}
	}

	v = SARDATA;
	v = (unsigned long)v * (unsigned long)V_CO_PRE / 0xff; // �����ص�ѹ

//--- �ָ� ���IO������ ----
	//-- ��� ��ѹ���ܽźͼ��̹ܽŹ��õĻ�����Ҫ���������м��IO��Ϊ [������ģʽ]

	SARCON  &= 0xf7;; //�ر�ADC
	EA = 1;
	return(v);
}


//0819 24bit SG ADC �ж�,��ȡadc���ݣ�*****************
static void int0_serve(void) interrupt 0
{		
	EA = 0;	 // ��ֹ�ж�
	ADCcmd = 0;
	data_receive(); // ��ȡ SDI0818 �� ת������
	ADcode_pre &= 0x00ffffff;
	ADcode_pre >>= 2;
	Data_sycle_symbol = 1;	//�µ���������
	IE0 = 0;	// ����д����ʱ ���½��� �������ж���Ϣ ȥ��
	EA = 1;  // ���ж�
}


//--- ��������: �¶ȼ�� -- begin ---
#ifdef TEMP_DET_EN
// -- �¶ȼ�� ͨ�� --
// mode = 1: ��Ҫͨ��1�ӳ�
// mode = 0: ����Ҫͨ��1�ӳ�
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
		
	EA = 0; //��ֹ�ж�
	ADCcmd = 0;	
	//-- �¶�ͨ�� -- 80Hz --
	PD_CON &= 0xe7;		//���� SGADCON,SGADCON2
	PD_CON |= 0x07;		//�����¶�ͨ��
	SGADCON = 0xf4; 	//�¶�ͨ��,160Hz
	EXIF &= 0xbf;		//�����־
//	delay_ms(1);
	//-- �õ��¶ȱ��� --
	ADcode_pre = 0;
	while (1)
	{
		//---ι��--��������ȫ����-begin-
		WD_TA = 0x05;
		WD_TA = 0x0a;
		WDCON = 0x1f;	//���Ź�ʱ�� 4s��,(�ر�)��Ƶ����0.2s
		//---ι��--��������ȫ����-end-
		
		while((EXIF & 0x40) == 0x00)	//���û������
			;
		EXIF &= 0xbf;		//�����־
		//-----------
		// ��ȡ  �� ת������
		temp.b[1] = SGADC3;
		temp.b[2] = SGADC2;
		temp.b[3] = SGADC1;
		temp.b[0] = 0;
		temp.w ^= 0x800000;			// ��Ϊ���Ϊ˫���ԣ�+0x800000������ƽ������
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
	//-- �¶ȼ��� --
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
	if(adcode_last >= temp.w)	//�������
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
	if(Temperature >= 70)	//������+/- 70�ȵķ�Χ
		Temperature = 70;
	if(temp_sign)
		Temperature = -Temperature;
	//-- ͨ���ָ� --
	SGADCON = SGADCON_DEFAULT; 	//1ͨ��,20Hz
	PD_CON = PD_CON_DEFAULT;
	EXIF &= 0xbf;		//�����־
	i = 0;
	if(mode)	//��Ҫͨ��1�ӳ�
	{
		while (i<5)
		{
			//---ι��--��������ȫ����-begin-
			WD_TA = 0x05;
			WD_TA = 0x0a;
			WDCON = 0x1f;	//���Ź�ʱ�� 4s��,(�ر�)��Ƶ����0.2s
			//---ι��--��������ȫ����-end-
			while(!IE0)	//���û������
				;
			IE0 = 0;		//�����־
			i++;
	  	}
	}
	EA = 1; //�򿪰����ж�		
}
#endif
//--- ��������: �¶ȼ�� -- end ---
