/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DVR_BATTERY
#define __DVR_BATTERY

	#define SOURCE_ADAPTER_HIGH			6
	#define SOURCE_ADAPTER_OK			5
	#define SOURCE_ADAPTER_LOW			4
	#define SOURCE_BATTERY_HIGH			3
	#define SOURCE_BATTERY_OK			2
	#define SOURCE_BATTERY_LOW			1
	#define SOURCE_BATTERY_VERY_LOW	    0

	#define LEVEL_ADAPTER_HIGH			(float)10
	#define LEVEL_ADAPTER_OK			(float)7.0
	#define LEVEL_ADAPTER_LOW			(float)5.5
	#define LEVEL_BATTERY_HIGH			(float)5.0
	#define LEVEL_BATTERY_OK			(float)4.5
	#define LEVEL_BATTERY_LOW			(float)4.0
	#define LEVEL_BATTERY_VERY_LOW	    (float)3.9

	#define DIODE_VOLTAGE	 		   (float)0.2

	void vGestorBateria(void);
	void vVadapHigh(void);
	void vVbatVeryLow(void);
	unsigned char uGet_Status_Volt(void);
	void vSet_Volts_System(void);
	void vGet_Volts_System(void);
	
	void driver_symbol(void);
	//void driver_symbol_init(void);
	
#endif /* __DVR_BATTERY */