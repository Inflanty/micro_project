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

void ux_mcpwnInit(){
	printf("initializing mcpwm gpio...\n");
	mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, GPIO_PWM0A);

	printf("Configuring Initial Parameters of mcpwm...\n");
    mcpwm_config_t pwm_config;
    pwm_config.frequency = 1000;    	//frequency = 500Hz,
    pwm_config.cmpr_a = 0;    			//duty cycle of PWMxA = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
	pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
	mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);
}


void ux_motorFrward(float duty_cycle)
{
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, duty_cycle);
    mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, MCPWM_DUTY_MODE_0); //call this each time, if operator was previously in low/high state
}


void ux_motorStop()
{
    mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A);
}
