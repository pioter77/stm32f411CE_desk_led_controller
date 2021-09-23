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
		 .activeMode = AUTOSENSOR,
		 .sensitivity_threshold_low=400,
		 .sensitivity_threshold_high=1800,
 };
 T_pwm_led LEDSTRIP2={
		 .fill=0,
		 .activeMode = AUTOSENSOR,
		 .sensitivity_threshold_low=400,
		 .sensitivity_threshold_high=1800,
 };
 T_pwm_led LEDSTRIP3={
		 .fill=0,
		 .activeMode = MANUAL,
		 .sensitivity_threshold_low=400,
		 .sensitivity_threshold_high=1800,
 };
 T_pwm_led LEDSTRIP4={
		 .fill=0,
		 .activeMode = MANUAL,
		 .sensitivity_threshold_low=400,
		 .sensitivity_threshold_high=1800,
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
	//set ledd fillout depending on the set mode
	set_ligth_output(&LEDSTRIP1, PHOTO1A.readout_act);
	set_ligth_output(&LEDSTRIP2, PHOTO1A.readout_act);
	set_ligth_output(&LEDSTRIP3, PHOTO1A.readout_act);
	set_ligth_output(&LEDSTRIP4, PHOTO1A.readout_act);
}

//linear aproximation or exponent aproximation between 2 predefined levels of sensitivity
uint16_t calculate_light_output(uint16_t sensor,T_pwm_led *led,_Bool linearOutput)
{
//	sensor=4095-sensor;
	if(sensor>4095)		return 0;	//readout error, sensor is 12 bit only
	if(sensor<led->sensitivity_threshold_low) 		return 0;				//full off
	else if(sensor>led->sensitivity_threshold_high) return (uint16_t)LEDSTRIPS_ARR;	//full on
	else if(linearOutput){													//when calculating output from linear function
		if(led->sensitivity_threshold_low>=led->sensitivity_threshold_high || led->sensitivity_threshold_high==0 || led->sensitivity_threshold_low==0) return 0;	//threshold values set incorrectly, ignore, cut off power
		return ((LEDSTRIPS_ARR*(sensor-led->sensitivity_threshold_low))/(led->sensitivity_threshold_high-led->sensitivity_threshold_low));
	}else{																	//when calculating output from exponent function
		return 0;//todo:implement
	}
//	return 0;//out;
}

//function that based on current mode for invoked led sets the led's fill
void set_ligth_output(T_pwm_led *led, uint16_t sensor)
{
	uint16_t encoder_set=TIM4->CNT;
	uint16_t encoder_max=LL_TIM_GetAutoReload(TIM4);
	uint16_t led_max= LL_TIM_GetAutoReload(TIM3);

	//todo: make sure led is enabled with :
//	if(led->isOn)
	//when manual mode:
	if(led->activeMode==MANUAL) led->fill= led_max-(uint32_t)((encoder_set*led_max)/(float)encoder_max);
	else if(led->activeMode==AUTOSENSOR) led->fill = calculate_light_output(sensor,led,1);
	else if(led->activeMode==AUTOTIME)	1;	//todo:yet to implement
	else led->fill = 0;					//error:mode not set
}

//encoder action function, handdles encoder control in main.c
void encoder_ctrl(T_encoder *encoder)
{
	if(encoder->isBtnPressed)
	{
		encoder->isBtnPressed=0;	//clar action flag
		//action to perform when encoder pressed here:
	}
}

//isr button pressed on encoder callback
void encoder_btn_callback(T_encoder *encoder)
{
	encoder->isBtnPressed=1;
}
