
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __dvr_scale
#define __dvr_scale

#include "./customer/keyboard.h"

/* --- Direcciones en la EEPROM de los valores */
#define	ADDR_SET_CALIBRATE 			ADDRESS_SCALE_CONFIG_PAGE + 0	/* 1 byte */
#define ADDR_CAPACITY_CALI 			ADDRESS_SCALE_CONFIG_PAGE + 1	/* 4 byte */
#define ADDR_POINT_ZERO 			ADDRESS_SCALE_CONFIG_PAGE + 5	/* 4 byte */  //SI SE USA
#define ADDR_FACTOR_CALIBRATE 	    ADDRESS_SCALE_CONFIG_PAGE + 9	/* 4 byte */
#define ADDR_LENGUAGE 				ADDRESS_SCALE_CONFIG_PAGE + 13	/* 1 byte */
#define ADDR_UNITS 					ADDRESS_SCALE_CONFIG_PAGE + 14	/* 1 byte */
#define ADDR_CAPACITY 				ADDRESS_SCALE_CONFIG_PAGE + 15	/* 2 byte */
#define ADDR_PORC_REF 				ADDRESS_SCALE_CONFIG_PAGE + 17	/* 2 byte */
#define ADDR_DIVISION_MINIMA		ADDRESS_SCALE_CONFIG_PAGE + 19	/* 2 byte */
#define ADDR_DECIMAL_TOTAL 			ADDRESS_SCALE_CONFIG_PAGE + 21	/* 1 byte */
#define ADDR_REDONDEO_TOTAL 		ADDRESS_SCALE_CONFIG_PAGE + 22	/* 1 byte */
#define ADDR_REDONDEO_VENTA 		ADDRESS_SCALE_CONFIG_PAGE + 23	/* 1 byte */
#define ADDR_DECIMAL_PRECIO 		ADDRESS_SCALE_CONFIG_PAGE + 24	/* 1 byte */
#define ADDR_ZERO_TRACKING 			ADDRESS_SCALE_CONFIG_PAGE + 25	/* 1 byte */
#define ADDR_FORMATO_IMPRESION	    ADDRESS_SCALE_CONFIG_PAGE + 26	/* 1 byte */
#define ADDR_MSG_UNLOAD 		    ADDRESS_SCALE_CONFIG_PAGE + 27	/* 1 byte */
#define ADDR_TIME_OFF 				ADDRESS_SCALE_CONFIG_PAGE + 28	/* 1 byte */
#define ADDR_MODELO					ADDRESS_SCALE_CONFIG_PAGE + 29 /* 1 byte */
//#define	ADDR_BACKLIGHT				ADDRESS_SCALE_CONFIG_PAGE + 30 /* 1 byte */
#define ADDR_MSGBAT					ADDRESS_SCALE_CONFIG_PAGE + 31 /* 1 byte */
//#define ADDR_DEBUG				ADDRESS_SCALE_CONFIG_PAGE + 43 /* 2 byte */
#define ADDR_COUNT_RANGE			ADDRESS_SCALE_CONFIG_PAGE + 32 /* 2 byte */
#define ADDR_MONEDA					ADDRESS_SCALE_CONFIG_PAGE + 34 /* 1 byte */
#define ADDR_DECIMAL_WEIGHT			ADDRESS_SCALE_CONFIG_PAGE + 35  /* 2 bytes */
#define ADDR_TEMPERATURE			ADDRESS_SCALE_CONFIG_PAGE + 37 	/* 4 Bytes */
#define	ADDR_VOLT_BATT				ADDRESS_SCALE_CONFIG_PAGE + 41  /* 4 Bytes */
#define	ADDR_VOLT_ADAP				ADDRESS_SCALE_CONFIG_PAGE + 45  /* 4 Bytes */
#define ADDR_SAVEBATT				ADDRESS_SCALE_CONFIG_PAGE + 49  /* 1 byte */
#define ADDR_DIVISION_MENOR			ADDRESS_SCALE_CONFIG_PAGE + 50 	/* 2 bytes */
#define ADDR_MULTIRANGO				ADDRESS_SCALE_CONFIG_PAGE + 52 	/* 1 byte */
#define ADDR_DIVISION_MENORMENOR    ADDRESS_SCALE_CONFIG_PAGE + 53 	/* 2 bytes */

#define ADDR_COUNTER_CALIBRATION    ADDRESS_QLTY_AND_CNTRS_PAGE + 0 	/* 2 byte */
#define ADDR_COUNTER_CONFIGURATION  ADDRESS_QLTY_AND_CNTRS_PAGE + 2 	/* 2 byte */
#define ADDR_COUNT_OVERLOAD		    ADDRESS_QLTY_AND_CNTRS_PAGE + 4  /* 2 bytes */
#define ADDR_VALUE_OVERLOAD		    ADDRESS_QLTY_AND_CNTRS_PAGE + 6  /* 4 bytes */
#define ADDR_ERRORBATT			    ADDRESS_QLTY_AND_CNTRS_PAGE + 10  /* 1 byte */
#define ADDR_VENTA_TOTAL		    ADDRESS_QLTY_AND_CNTRS_PAGE + 11   /* 4 bytes */

enum UnitScale{ 
	UNIT_LB, UNIT_KG, UNIT_OZ 
};

typedef struct {
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
	
//	float fTemperature;
	
	float fVenta_Total_Scale;
	float fTotal_Venta_Articulos;
	
	unsigned int iCapacity;
	unsigned char 	cWeightDecimal;
	unsigned int iLoadPorcRefer;					/* % de la carga qe se indica para calibrar */
	unsigned int iDivisionMinima;
	unsigned int iDivisionMenor;
	unsigned int iDivisionMenorMenor;
//	unsigned int iDebug;
	unsigned int iMultiplicador_Producto;
	unsigned int iNumber_Articulos_Venta;
		
	unsigned int iCounter_Calibration;
	unsigned int iCounter_Configuration;
	unsigned int iCountOverload;
	
	unsigned char cMultirango;
	unsigned char cLenguage;								// Idioma 0->Ingle, 1->Espa?ol 
	enum UnitScale cUnits;			// Unidades de uso en la bascula 
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
//	unsigned char strNumSerieCalidad[11];	
	unsigned char cSaveBattery;
	
	unsigned char cFormatoImpresion;
	unsigned char cMoneda;
	
	//unsigned char cSpecialAction;
	unsigned char cBacklight;
	
}Parameter;
extern Parameter stScaleParam;

enum digi_key{ 
		KEY_NULL=0,
		KEY_CERO = 'Z', KEY_TARA = 'T', KEY_M1 = 'A', KEY_RCL = 'R', KEY_CHG = '=', KEY_MAS = '+', KEY_M2 = 'B',
		KEY_MEM = 'M',KEY_7 = '7',KEY_4 = '4', KEY_1 = '1',KEY_C = 'C',KEY_8 = '8',KEY_5 = '5', KEY_2 = '2',KEY_0 = '0',KEY_9 = '9', 
		KEY_6 = '6', KEY_3 = '3',KEY_PUNTO = '.',
//		KEY_A = 'A', KEY_B = '', KEY_C1 = '', KEY_D = '', KEY_E = '', KEY_F = '', KEY_G = '', KEY_H = '',
	};	


typedef struct {
//	unsigned bScaleOn : 1;			/* Indica si la bascula esta encendida o apagada */
//	unsigned bReadBattery : 1;	/* Indica que ya se puede leer el valor de la bateria */
	unsigned bBateriaLow : 1;		/* Indica el estado de la bateria */
//	unsigned bMsgBatteryLow : 1;
//	unsigned bBateriaCount : 1;
	unsigned bSourceVoltage : 1; /* Indica de donde procede el voltaje de alimentacion	bateria o eliminador */
//	unsigned bZeroTracking : 1;	/* Inidica si la funcion ZeroTrcking esta activada */
	unsigned bTara : 1;					/* 1 -> Tara Activada, 0 -> Desactivada */
//	unsigned bZero : 1;					/* Inidica si el sistema debe tomar una nueva referncia	a cero */
	unsigned bFlagFijarPRecio : 1;
	unsigned bOverLoad : 1;
	unsigned bFlagWeightNeg : 1;
	
	unsigned bCodeConfiguration : 1;
//	unsigned bMultirango_Off : 1;
	unsigned bBacklight_On : 1;
	
//	unsigned cWait_Time_Code : 1;
//	unsigned cAuto_Off_Time : 1;
		
	unsigned bReadPlus : 1;
	unsigned bWritePlus : 1;
	
	unsigned bDotDecimalPrice : 1;
	
	unsigned bTopeSobrePeso : 1;
	unsigned bShowCountADC : 1;
	
//	unsigned bBL_Lpcr : 1;
	
	unsigned bActiveSaveBattery : 1;	
	//unsigned bPWM_Duty_On : 1;

	unsigned bFlagNeg : 1;



		
//	unsigned bEliminadorOn : 1;
//	unsigned bShowErrorBat : 1;
	
//	unsigned bIwdgConfig : 1;
//	unsigned bStateBatDown : 1;	//1 -> estado de bater?a muy bajo p/trabajar
										//0 -> estado OK de bater?a p/trabajar. EMP		

//	unsigned cErrorReadAdc : 1;
//	unsigned cErrorResponseAdc : 1;

//	unsigned bErrorNoiseAdc : 1;
		
//	unsigned bTestZeroTracking : 1;
	
//	unsigned bFunctionFactory : 1;
	
	unsigned bPrecioCero : 1;
	
	unsigned bFlagStable : 1;
//	unsigned bCalidadTest : 1;
	
	unsigned bShowErroBat : 1;
	
//	unsigned bPressVarios : 1;
	unsigned bPlsUnload_Enable : 1;	//1 -> presente PLS UNLOAD, EMP
	unsigned bAdd_Articulos : 1;	// 1 -> modo de suma de articulos EMP
	unsigned bArticulosTotalCero : 1;	//1 -> permite saber que $Total y
													// #Artculos son cero
	
	unsigned bMultiplicar_Producto : 1;
	unsigned bAdd_Producto_Sin_Peso : 1;
	
	//unsigned bPrinter : 1;
	
	//unsigned bStatusVoltajeRef : 1;
	
	//unsigned bAhorroBateria : 1;
	
	enum digi_key bFlagKeyNumber : 1;
	
}FlagScale;  

extern idata FlagScale srFlagScale;

/* Global variables ----------------------------------------------------------*/
typedef struct {
	unsigned char iTimerA;//200mS
	unsigned int iTimerE;					//5
	unsigned int iTimerJ;							//  1s
}strTimers;
extern idata strTimers strTimer;

#define TimerAend 200
#define TimerEend 5000
#define TimerJend 60000



enum ActionScale{ 
	ScaleBattery, ScalePreOn, ScaleWait, 
	ScaleRun,
};
//extern enum ActionScale eAccionScale;


// Global function prototypes -----------------------------------------------
void vReadParamScale(void);
void vSaveParamScale(unsigned char cType_Parameter);
void vPreConfiguration(unsigned char cPreConfiguration);
void vCalibrate_Scale(void);
float fStablePoint(unsigned char cSetCountBack, unsigned char cShowCount, unsigned char cRunStable);
unsigned char cRun_Scale(void);
unsigned char cSetZeroPoint(void);
void cOnOffModeTara(float fWeightTara);
void vSetZero(void);
float fCuentasToPeso(float fCountADC);
float fRoundFloat(float fNumber, unsigned char cDecimal, int cRedondeo);
void vCalculate_Weight (void);
/*
float fSleep_Run(void);


void vWeight_Negative(void);
void vWeight_Positive(void);
void vGetZeroTracking(float fActualWeightScale);



*/


#endif