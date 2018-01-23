/*
 * FILE userLib.c
 * Author : Jan Glos
 * Date : 2.01.2018
 * User library for project require
 * */


#include "userLib.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"
#include "driver/timer.h"
#include "soc/timer_group_struct.h"
#include "driver/adc.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_adc_cal.h"

void uv_motorInit()
{
	printf("initializing motor gpio...\nMotorOUT = GPIO18\n");
	mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, MotorOUT);

	printf("Configuring Initial Parameters of MotorOUT...\n");
    mcpwm_config_t pwm_config;
    pwm_config.frequency = 1000;    	//frequency = 500Hz,
    pwm_config.cmpr_a = 0;    			//duty cycle of PWMxA = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
	pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
	mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);
}

void uv_ledInit()
{
	printf("initializing motor gpio...\nledOUT = GPIO4\n");
	mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, ledOUT);

	printf("Configuring Initial Parameters of MotorOUT...\n");
    mcpwm_config_t pwm_config;
    pwm_config.frequency = 1000;    	//frequency = 500Hz,
    pwm_config.cmpr_b = 0;    			//duty cycle of PWMxB = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
	pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
	mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_1, &pwm_config);
}

void uv_motorStop()
{
    mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A);
}

void uv_motorSet(float duty_cycle)
{
	mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, duty_cycle);
	mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, MCPWM_DUTY_MODE_0); //call this each time, if operator was previously in low/high state
}

void uv_ledUp(enum dir direction, float step_val)
{
	float act_val = mcpwm_get_duty(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_B);
	switch (direction){
	case UP:
		printf("LED brightening\n");
		mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_B, (act_val + step_val));
		break;
	case DOWN:
		if((act_val - step_val) > 0)
		{
			printf("LED dimming\n");
			mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_B, (act_val - step_val));
		} else
		{
			printf("EROR : full blackout !\n");
		}
		break;
	}
}

void uv_ledSet(float duty_cycle)
{
	mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, duty_cycle);
	mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, MCPWM_DUTY_MODE_0);
}

void uv_timer0Init(char* timer_group0_isr)
{
	int timer_group = TIMER_GROUP_0;
	int timer_idx = TIMER_0;
	timer_config_t config;
	config.alarm_en = 1;
	config.auto_reload = 1;
	config.counter_dir = TIMER_COUNT_UP;
	config.divider = TIMER_DIVIDER;
	config.intr_type = TIMER_INTR_SEL;
	config.counter_en = TIMER_PAUSE;
	/*Configure timer*/
	timer_init(timer_group, timer_idx, &config);
	/*Stop timer counter*/
	timer_pause(timer_group, timer_idx);
	/*Load counter value */
	timer_set_counter_value(timer_group, timer_idx, 0x00000000ULL);
	/*Set alarm value*/
	timer_set_alarm_value(timer_group, timer_idx,
	TIMER_INTERVAL0_SEC * TIMER_SCALE);
	/*Enable timer interrupt*/
	timer_enable_intr(timer_group, timer_idx);
	/*Set ISR handler*/
	timer_isr_register(timer_group, timer_idx, timer_group0_isr,
			(void*) timer_idx,
			ESP_INTR_FLAG_IRAM, NULL);
	/*Start timer counter*/
	timer_start(timer_group, timer_idx);
}

void uv_adcInit()
{
    //Init ADC and Characteristics
    esp_adc_cal_characteristics_t characteristics;
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL, ADC_ATTEN_DB_0);
    esp_adc_cal_get_characteristics(V_REF, ADC_ATTEN_DB_0, ADC_WIDTH_BIT_12, &characteristics);
}

uint32_t ux_diffCalculate(uint32_t* preValue, uint32_t actualValue)
{
	if(((actualValue + ACCEPTABLE_ADC_DIFF) < *preValue) || ((actualValue - ACCEPTABLE_ADC_DIFF) > *preValue))
	{
		if((actualValue + ACCEPTABLE_ADC_DIFF) < *preValue)
		{
			return(*preValue - actualValue);
		} else if((actualValue - ACCEPTABLE_ADC_DIFF) > *preValue)
		{
			return(actualValue - *preValue);
		} else
		{
			return(-1);
		}
	}else
	{
		return(0);
	}
}

void uv_saveParam(struct connData *__dts,paramAttr actual, uint32_t actualAttr)
{
	switch(actual){
	case __GATT_IF:
		__dts->gatts_if = actualAttr;
		break;
	case __APP_ID:
		__dts->app_id = actualAttr;
		break;
	case __CONN_ID:
		__dts->conn_id = actualAttr;
		break;
	case __CHAR_HANDLE:
		__dts->char_handle = actualAttr;
		break;
	case __CONN:
		__dts->connecting = actualAttr;
		break;
	case __IS_CONN:
		__dts->isConnected = actualAttr;
		break;
	case __NO_CONN:
		__dts->noConnection = actualAttr;
		break;
	default:

		break;
	}
}


