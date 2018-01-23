/*
 * USER LIBRARY
 * Before using please fill the #define section.
 * Library for config and pending peripherals faunction.
 *
 * */

#ifndef userLib_h
#define userLib_h

#include <stdbool.h>

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

#define MotorOUT 18
#define ledOUT 5
#define TIMER_DIVIDER 0
#define TIMER_INTR_SEL 0
#define TIMER_INTERVAL0_SEC 0
#define TIMER_SCALE 0
#define V_REF 0
#define ADC1_CHANNEL 0
#define ACCEPTABLE_ADC_DIFF 0
#define GPIO_INPUT_IO_0     	4
#define GPIO_INPUT_IO_1     	5
#define GPIO_INPUT_PIN_SEL  	((1<<GPIO_INPUT_IO_0) | (1<<GPIO_INPUT_IO_1))
#define ESP_INTR_FLAG_DEFAULT 	0

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

void uv_motorInit	();										/* SYSTEMfnc : Motor initiation */
void uv_ledInit		();										/* SYSTEMfnc : LED initiation */
void uv_timer0Init	(char* timer_group0_isr);				/* SYSTEMfnc : Timer initiation witch interrupt */
void uv_adcInit		();										/* SYSTEMfnc : A/C initiation */
void uv_gpioInit	(char *gpio_isr_handler);				/* SYSTEMfnc : GPIO initiation with interrupt */
void uv_motorStop	();										/* MOTORfnc  : Use this function if you want to stop the motor immediately */
void uv_motorSet	(float duty_cycle);						/* MOTORfnc  : If you want set specific value 0 - 100 % */
void uv_ledUp		(enum dir direction, float step_val);	/* LEDfnc    : If you want perform dimming/brightening, provide direction [UP/DOWN] and value of the step */
void uv_ledSet		(float duty_cycle);						/* LEDfnc 	 : If you want set specific value 0 - 100 % */
void uv_print		(char *text_log, char *text);			/* USERfnc	 : If you want to use log output, 1 variable : log_name, 2 variable : text */

#endif
