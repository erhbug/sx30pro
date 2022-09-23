
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __dvr_scale
#define __dvr_scale

//#include "./customer/keyboard.h"


enum UnitScale{ 
	UNIT_LB, UNIT_KG, UNIT_OZ 
};

struct Parameter{
	float fWeightScale;
	float fWeightScaleCount;
	float fWeightOverload;
	float fPrice_Unit;
	float fTotal_Venta;
	float fValueTara;						/* Valor actual de la tara */
	float fWeightScaleTara;
	float	fPointZeroCali;
	float fCapacityCali;
	float fFactorCalibrate;			/* Factor de Calibracion */	
	float fPointZero;
	float fPointZeroInitial;
	float fAddress_Plus;
	float fValueOverload;
	
	float fVoltage_Batt;
	float fVoltage_Adap;
	
	float fTemperature;
	
	float fVenta_Total_Scale;
	float fTotal_Venta_Articulos;
	
	unsigned int iCapacity;
	unsigned char 	cWeightDecimal;
	unsigned int iLoadPorcRefer;					/* % de la carga qe se indica para calibrar */
	unsigned int iDivisionMinima;
	unsigned int iDivisionMenor;
	unsigned int iDivisionMenorMenor;
	unsigned int iDebug;
	unsigned int iMultiplicador_Producto;
	unsigned int iNumber_Articulos_Venta;
		
	unsigned int iCounter_Calibration;
	unsigned int iCounter_Configuration;
	unsigned int iCountOverload;
	
	unsigned char cMultirango;
	unsigned char cLenguage;								/* Idioma 0->Ingle, 1->Espa?ol */
	enum UnitScale cUnits;			/* Unidades de uso en la bascula */
	unsigned char cPuntoDecimalTotal;
	unsigned char cRedondeoCifraTotal;
	unsigned char cRedondeoCifraVentaTotal;
	unsigned char cValorRedondeoCifraTotal;
	unsigned char cValorcRedondeoCifraVentaTotal;
	unsigned char cPuntoDecimalPrecio;
	unsigned char cNumberDecimalPrice;
	unsigned char cZeroTracking;
	unsigned char cMsgUnload;
	unsigned char cTimeAutooff;						
	unsigned char cModeloBascula;
	unsigned char fChangeRange;
	unsigned char cTypeBeeper;
	unsigned char cShowHighBat;
	unsigned char cModeIncDiv;
	unsigned char cCountRange;	
	unsigned char strNumSerieCalidad[11];	
	unsigned char cSaveBattery;
	
	unsigned char cFormatoImpresion;
	unsigned char cMoneda;
	
	unsigned char cSpecialAction;
	unsigned char cBacklight;
};
extern struct Parameter stScaleParam;

enum digi_key{ 
		KEY_NULL,
		KEY_CERO, KEY_TARA, KEY_M1, KEY_RCL, KEY_CHG, KEY_MAS, KEY_M2,
		KEY_MEM,KEY_7,KEY_4, KEY_1,KEY_C,KEY_8,KEY_5, KEY_2,KEY_0,KEY_9, 
		KEY_6, KEY_3,KEY_PUNTO,
		KEY_A, KEY_B, KEY_C1, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H,
	};	

struct FlagScale{
	unsigned char bScaleOn;			/* Indica si la bascula esta encendida o apagada */
	unsigned char bReadBattery;	/* Indica que ya se puede leer el valor de la bateria */
	unsigned char bBateriaLow;		/* Indica el estado de la bateria */
	unsigned char bMsgBatteryLow;
	unsigned char bBateriaCount;
	unsigned char bSourceVoltage; /* Indica de donde procede el voltaje de alimentacion
												bateria o eliminador */
	unsigned char bZeroTracking;	/* Inidica si la funcion ZeroTrcking esta activada */
	unsigned char bTara;					/* 1 -> Tara Activada, 0 -> Desactivada */
	unsigned char bZero;					/* Inidica si el sistema debe tomar una nueva referncia
												a cero */
	unsigned char bFlagFijarPRecio;
	unsigned char bOverLoad;
	unsigned char bFlagWeightNeg;
	
	unsigned char bCodeConfiguration;
	unsigned char bMultirango_Off;
	unsigned char bBacklight_On;
	
	unsigned char cWait_Time_Code;
	unsigned char cAuto_Off_Time;
		
	unsigned char bReadPlus;
	unsigned char bWritePlus;
	
	unsigned char bDotDecimalPrice;
	
	unsigned char bTopeSobrePeso;
	unsigned char bShowCountADC;
	
	unsigned char bBL_Lpcr;
	
	unsigned char bActiveSaveBattery;	
	unsigned char bPWM_Duty_On;

	unsigned char bFlagNeg;
		
	unsigned char bEliminadorOn;
	unsigned char bShowErrorBat;
	
	unsigned char bIwdgConfig;
	unsigned char bStateBatDown;	//1 -> estado de bater?a muy bajo p/trabajar
										//0 -> estado OK de bater?a p/trabajar. EMP		

	unsigned char bErrorReadAdc;
	unsigned char bErrorResponseAdc;
	unsigned char bErrorNoiseAdc;
		
	unsigned char bTestZeroTracking;
	
	unsigned char bFunctionFactory;
	
	unsigned char bPrecioCero;
	
	unsigned char bFlagStable;
	unsigned char bCalidadTest;
	
	unsigned char bShowErroBat;
	
	unsigned char bPressVarios;
	unsigned char bPlsUnload_Enable;	//1 -> presente PLS UNLOAD, EMP
	unsigned char bAdd_Articulos;	// 1 -> modo de suma de articulos EMP
	unsigned char bArticulosTotalCero;	//1 -> permite saber que $Total y
													// #Artculos son cero
	
	unsigned char bMultiplicar_Producto;
	unsigned char bAdd_Producto_Sin_Peso;
	
	unsigned char bPrinter;
	
	unsigned char bStatusVoltajeRef;
	
	unsigned char bAhorroBateria;
	
	enum digi_key bFlagKeyNumber;
	
};  

extern struct FlagScale srFlagScale;

/* Global variables ----------------------------------------------------------*/
/*

extern struct Parameter stScaleParam;

struct FlagScale{
	unsigned char bScaleOn;			// Indica si la bascula esta encendida o apagada 
	unsigned char bReadBattery;	// Indica que ya se puede leer el valor de la bateria 
	unsigned char bBateriaLow;		// Indica el estado de la bateria 
	unsigned char bMsgBatteryLow;
	unsigned char bBateriaCount;
	unsigned char bSourceVoltage; // Indica de donde procede el voltaje de alimentacion
									//			bateria o eliminador 
	unsigned char bZeroTracking;	/// Inidica si la funcion ZeroTrcking esta activada 
	unsigned char bTara;					// 1 -> Tara Activada, 0 -> Desactivada 
	unsigned char bZero;					// Inidica si el sistema debe tomar una nueva referncia
											//	a cero 
	unsigned char bFlagFijarPRecio;
	unsigned char bOverLoad;
	unsigned char bFlagWeightNeg;
	
	unsigned char bCodeConfiguration;
	unsigned char bMultirango_Off;
	unsigned char bBacklight_On;
	
	unsigned char cWait_Time_Code;
	unsigned char cAuto_Off_Time;
		
	unsigned char bReadPlus;
	unsigned char bWritePlus;
	
	unsigned char bDotDecimalPrice;
	
	unsigned char bTopeSobrePeso;
	unsigned char bShowCountADC;
	
	unsigned char bBL_Lpcr;
	
	unsigned char bActiveSaveBattery;	
	unsigned char bPWM_Duty_On;

	unsigned char bFlagNeg;
		
	unsigned char bEliminadorOn;
	unsigned char bShowErrorBat;
	
	unsigned char bIwdgConfig;
	unsigned char bStateBatDown;	//1 -> estado de batería muy bajo p/trabajar
										//0 -> estado OK de batería p/trabajar. EMP		

	unsigned char bErrorReadAdc;
	unsigned char bErrorResponseAdc;
	unsigned char bErrorNoiseAdc;
		
	unsigned char bTestZeroTracking;
	
	unsigned char bFunctionFactory;
	
	unsigned char bPrecioCero;
	
	unsigned char bFlagStable;
	unsigned char bCalidadTest;
	
	unsigned char bShowErroBat;
	
	unsigned char bPressVarios;
	unsigned char bPlsUnload_Enable;	//1 -> presente PLS UNLOAD, EMP
	unsigned char bAdd_Articulos;	// 1 -> modo de suma de articulos EMP
	unsigned char bArticulosTotalCero;	//1 -> permite saber que $Total y
													// #Artculos son cero
	
	unsigned char bMultiplicar_Producto;
	unsigned char bAdd_Producto_Sin_Peso;
	
	unsigned char bPrinter;
	
	unsigned char bStatusVoltajeRef;
	
	unsigned char bAhorroBateria;
	
	//enum digi_key bFlagKeyNumber;
	
};  

extern struct FlagScale srFlagScale;
*/
struct strTimers{
	unsigned char cFLag_TimerA_Start;
	unsigned char cFLag_TimerA_On;
	unsigned char cFLag_TimerA_End;
	unsigned char cFLag_TimerA_Stop;

	unsigned char cFLag_TimerB_Start;
	unsigned char cFLag_TimerB_On;
	unsigned char cFLag_TimerB_End;
	unsigned char cFLag_TimerB_Stop;

	unsigned char cFLag_TimerD_Start;
	unsigned char cFLag_TimerD_On;
	unsigned char cFLag_TimerD_End;
	unsigned char cFLag_TimerD_Stop;

	unsigned char cFLag_TimerE_Start;
	unsigned char cFLag_TimerE_On;
	unsigned char cFLag_TimerE_End;
	unsigned char cFLag_TimerE_Stop;

	unsigned char cFLag_TimerJ_On;
	unsigned char cFLag_TimerJ_End;
	unsigned char cFLag_TimerJ_Stop;
	unsigned char cFLag_TimerJ_Start;

/*	
	unsigned char cFLag_TimerC_Start;
	unsigned char cFLag_TimerC_On;
	unsigned char cFLag_TimerC_End;
	unsigned char cFLag_TimerC_Stop;
	
	
	unsigned char cFLag_TimerF_Start;
	unsigned char cFLag_TimerF_On;
	unsigned char cFLag_TimerF_End;
	unsigned char cFLag_TimerF_Stop;
	unsigned char cFLag_TimerH_On;
	unsigned char cFLag_TimerH_End;
	unsigned char cFLag_TimerH_Stop;
	unsigned char cFLag_TimerH_Start;

	unsigned char cFLag_TimerI_On;
	unsigned char cFLag_TimerI_End;
	unsigned char cFLag_TimerI_Stop;
	unsigned char cFLag_TimerI_Start;
	
	

	unsigned char cFLag_TimerCharge_Start;
	unsigned char cFLag_TimerCharge_On;
	unsigned char cFLag_TimerCharge_End;
	unsigned int iTimerCharge;
	
	unsigned char cFLag_TimerG_On;*/
	
	
	int iTimerA;
    unsigned int iTimerE;					//5s						//200mS
/*	int iTimerB;						//500mS
	long int iTimerC;					//1s
	int iTimerD;						//3s
	
	int iTimerF;						//1s
	int iTimerMinuteF;				    //1 minuto
	int iTimerH;
	
	int iTimerG;					    //1 minuto
	int iTimerMinuteG;				//1 minuto
	
	int iTimerI;							//1 minuto
	
	int iTimerJ;							//  1s
	int iTimerSegundosJ;     // 15s*/
	
};
#define TimerEend 5000

extern struct strTimers strTimer;

/*
enum ActionScale{ 
	ScaleOff, scaleStandBy, ScaleBattery, ScalePreOn, ScaleOn, ScaleZero, ScaleCalibrate, ScaleWait, 
	ScaleRun, ScaleTara, ScaleCode, ScaleMenu, scalePreOnDc, scalePreOnBat, ScalePreOff
};

extern struct strTimers strTimer;

// Global function prototypes -----------------------------------------------
void vReadParamScale(void);
void vSaveParamScale(unsigned char cType_Parameter);

void vSetZero(void);
unsigned char cSetZeroPoint(void);
void cOnOffModeTara(float fWeightTara);
void vCalibrate_Scale(void);
void vPreConfiguration(unsigned char cPreConfiguration);
unsigned char cRun_Scale(unsigned char bEnableKeys);
void vComunicacion_Serial(void);
//void vSend_Data_Serial(void);
void vWdg_Config(void);
void vCalculate_Weight (void);
void vPrintParameters(void);
float fCuentasToPeso(float fCountADC);*/

float fStablePoint(unsigned char cSetCountBack, unsigned char cShowCount, unsigned char cRunStable);
void vPreConfiguration(unsigned char cPreConfiguration);

#endif