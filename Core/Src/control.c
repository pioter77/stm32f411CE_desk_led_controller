/*
 * control.c
 *
 *  Created on: Sep 19, 2021
 *      Author: x300
 */


#include "control.h"

volatile uint16_t adc_readouts_bufer[4]={[0 ... 3]=0};

t_System_timebase SYS_TIMEBASE={
	.system_1ms_tick=0,
};

t_Process_params PROCESS_PARAMS={
		.adc_eoc_count=0,
		.rotary_sens_val=0,
		.light_sens_val=0,
};

 T_light_params PHOTO1A={
		 	//A-analog D-digital sensor type
 };
 T_light_params PHOTO2A={

 };
 T_thermal_sensor THERMI1A={

 };
 T_encoder ENCODER1={

 };
 T_pwm_led LEDSTRIP1={
		 .fill=0,
 };
 T_pwm_led LEDSTRIP2={
		 .fill=0,
 };
 T_pwm_led LEDSTRIP3={
		 .fill=0,
 };
 T_pwm_led LEDSTRIP4={
		 .fill=0,
 };



void system_ctrl(void)
{
//	if(tick1ms)
//	{
//		//do control stuff here
//		tick1ms=0;
//	}
//	PROCESS_PARAMS.pwm_timer_fill = (uint16_t)(PROCESS_PARAMS.rotary_sens_val/4095.0*(1280.0-1.0));
}

//uint16_t lightFill=0;

void adc_vals_assign(void)
{
	PHOTO1A.readout_act=adc_readouts_bufer[0];
	PHOTO2A.readout_act=adc_readouts_bufer[1];
	THERMI1A.readout_act=adc_readouts_bufer[2];
//	=adc_readouts_bufer[3];	// for rain sensor
}

void pwm_ctrl(void){
	uint16_t encoder_set=TIM4->CNT;
	uint16_t encoder_max=LL_TIM_GetAutoReload(TIM4);
	uint16_t led_max= LL_TIM_GetAutoReload(TIM3);

	//when manual mode:
//	LEDSTRIP1.fill= led_max-(uint32_t)((encoder_set*led_max)/(float)encoder_max);
	LEDSTRIP2.fill= led_max-(uint32_t)((encoder_set*led_max)/(float)encoder_max);
	LEDSTRIP3.fill= led_max-(uint32_t)((encoder_set*led_max)/(float)encoder_max);
	LEDSTRIP4.fill= led_max-(uint32_t)((encoder_set*led_max)/(float)encoder_max);

//	when auto mode:
	LEDSTRIP1.fill= led_max-(uint32_t)(((PHOTO1A.readout_act-2047)*led_max)/(float)2047);
}

