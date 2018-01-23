/*
 * USER LIBRARY
 * Before using please fill the #define section.
 * Library for config and pending peripherals faunction.
 *
 * */

#ifndef USERLIB
#define USERLIB

#include <stdbool.h>

#define MotorOUT 18
#define ledOUT 4
#define TIMER_DIVIDER 0
#define TIMER_INTR_SEL 0
#define TIMER_INTERVAL0_SEC 0
#define TIMER_SCALE 0
#define V_REF 0
#define ADC1_CHANNEL 0
#define ACCEPTABLE_ADC_DIFF 0

struct connData{
    uint16_t gatts_if;
    uint16_t app_id;
    uint16_t conn_id;
    uint16_t char_handle;

    bool connecting;
    bool isConnected;
    bool noConnection;
};

enum dir{
	UP,
	DOWN,
};

enum param{
	__GATT_IF,
	__APP_ID,
	__CONN_ID,
	__CHAR_HANDLE,
	__CONN,
	__IS_CONN,
	__NO_CONN,
};

typedef int paramAttr; 										/* SYSTEMvar : paramAttr accept <enum param>, */

void uv_motorInit	();										/* SYSTEMfnc : Motor initiation */
void uv_ledInit		();										/* SYSTEMfnc : LED initiation */
void uv_timer0Init	(char* timer_group0_isr);				/* SYSTEMfnc : Timer initiation */
void uv_adcInit		();										/* SYSTEMfnc : A/C initiation */
void uv_motorStop	();										/* MOTORfnc  : Use this function if you want to stop the motor immediately */
void uv_motorSet	(float duty_cycle);						/* MOTORfnc  : If you want set specific value 0 - 100 % */
void uv_ledUp		(enum dir direction, float step_val);	/* LEDfnc    : If you want perform dimming/brightening, provide direction [UP/DOWN] and value of the step */
void uv_ledSet		(float duty_cycle);						/* LEDfnc 	 : If you want set specific value 0 - 100 % */
uint32_t ux_diffCalculate	(uint32_t* preValue, uint32_t actualValue); 				/* USERfnc : Calculate different between 2 uint32 */
void uv_saveParam			(struct connData,paramAttr actual, uint32_t actualAttr);	/* USERfnc : Saving parameters */

#endif
