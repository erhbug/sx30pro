/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DVR_BATTERY
#define __DVR_BATTERY
	
	#define SOURCE_ADAPTER					0
	#define SOURCE_BATTERY_OK				1
	#define SOURCE_BATTERY_HIGH			2
	#define SOURCE_BATTERY_LOW			3
	#define SOURCE_BATTERY_VERY_LOW	4
	#define SOURCE_BATTERY					5
	#define SOURCE_ADAPTER_HIGH			6
	#define SOURCE_NO_ON						7

	void vGestorBateria(void);
	void vGet_Volts_Ref(void);
	void vShowDataBatLcd(void);
	void vVbatHigh(void);
	void vVadapHigh(void);
	void vVbatVeryLow(void);
	unsigned char uGet_Status_Volt(void);
	void vSet_Volts_System(void);
	void vGet_Volts_System(void);
	void vShowDataCargador(void);
	
	void driver_symbol(void);
	void driver_symbol_init(void);
	
#endif /* __DVR_BATTERY */