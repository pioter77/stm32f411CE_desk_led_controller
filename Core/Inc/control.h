/*
 * control.h
 *
 *  Created on: Sep 19, 2021
 *      Author: x300
 */

#ifndef INC_CONTROL_H_
#define INC_CONTROL_H_

#include "main.h"
#include "defines.h"

enum ledstripsStates{MANUAL=1, AUTOSENSOR, AUTOTIME};

typedef struct t_system_timebase{
	_Bool system_1ms_tick;
}t_System_timebase;

extern volatile uint16_t adc_readouts_bufer[ADC_CONV_NUMBERS];

//structure for handling light sensor readouts
typedef struct t_light_params{
	volatile uint16_t readout_act;		//actual readout value (raw)
	uint16_t readout_med;				//readout value processed (after median filtration)
	uint16_t array_med[8];				//array for holding values to filter
	uint16_t dividerOhm;				// voltage divider resistor value
	uint16_t sensorOhm;				// photoresistor nominal resistance value
	uint8_t isEnabled   :1;		//flag that shows whether we should process the value and perform action on it
	uint8_t dummy1		:1;
	uint8_t dummy2		:1;
	uint8_t dummy3		:1;
}T_light_params;

typedef struct t_thermal_sensor{
	volatile uint16_t readout_act;		//actual readout value (raw)
	uint16_t *conversion_table;			//pointer to the array that holds *C values that corresonds to analog readouts
	uint16_t readout_med;				//readout value processed (after median filtration)
	uint16_t array_med[8];				//array for holding values to filter
	uint16_t dividerOhm;				// voltage divider resistor value
	uint16_t sensorOhm;				// photoresistor nominal resistance value
	uint8_t isEnabled    :1;		//flag that shows whether we should process the value and perform action on it
	uint8_t isNTC		:1;		//ntc or ptc type
	uint8_t dummy2		:1;
	uint8_t dummy3		:1;
}T_thermal_sensor;

typedef struct t_encoder{
	volatile unsigned short isBtnPressed			:1;			//set in ButtonCallback
	unsigned short dummy1;
	unsigned short dummy2;
	unsigned short dummy3;
	uint16_t position;											//counter value form timer
	uint16_t acceleration;
	//accleration function handler for other fast timer
	//pointer to calllback function here
}T_encoder;

typedef struct t_process_params{
	unsigned short isLightControlOn			:1;		//flag that determines whether light control mode is active
	unsigned short isPwmOutputOn			:1;		//flag for enabling power on mosfet gate pin
	unsigned short isDeviceOn				:1;		//flag for turning on the device in selected mode after button was pressed and device goes from standby mode
	unsigned short dummy1					:1;
	uint16_t light_sens_val;				//raw light intesivity readout from adc		(first EOC)
	uint16_t rotary_sens_val;				//raw potentiometer rotation readout from adc (second EOC)
	uint16_t rotary_sens_offset;			//adc value below which rotary raw readout is forced to 0 (potentiometer when full off still shows some value)
	_Bool adc_eoc_count;					//if 0 save readout to light if 1 save readout to potentiometer, negate at each EOC, start with 0
	uint16_t pwm_timer_fill;				//pwm capture compare value set based on light readouts if flag active
	//pvd detection:
//	uint8_t pvd_val;
	//light hystheresis
	uint16_t light_lower_thrshld;			//hystheresis lower level threshold value
	uint16_t light_upper_thrshld;			//hystheresis upper level threshold value
	uint16_t light_thrshld_deadarea;		//deadarea for hystheresis switching based on light readouts

}t_Process_params;

typedef struct t_pwm_led{
	uint8_t activeMode			:2;		//1 for manual 2 for automatic based on light sensor 3 automatic based on predefined rtc time values
	uint8_t isAutoCtrlLight		:1;		//maual or auto (light sensor) control
	uint8_t isOn				:1;		//is chanel activated
	uint8_t dummy2				:1;
	uint8_t dummy3				:1;
	uint16_t fill;					//actual oc compare value

	//for auto mode:
	uint16_t sensitivity_threshold_low;		//adc readout value after which leds start to light up
	uint16_t sensitivity_threshold_high;	//adc readout vlaue after which leds are fully on
}T_pwm_led;

typedef struct t_rel_params{
	_Bool isOn;								//status flag
	_Bool isAutoCtrl;						//auto (timeout) or manual (button) control
	_Bool timeoutOccured;					//flag set when rel timeout reached
	uint16_t timeNowOn;						//count of seconds passed since turn on, when this == timeoutval then turn off light
	uint16_t timeOutVal;					//seconds after which light will be shut down (reinit value)
	GPIO_TypeDef* relPort;					//associated port for rel output pin
	uint32_t relPin;						//associated pin for rel output pin

}T_rel_params;

extern t_System_timebase SYS_TIMEBASE;	//struct for holding timing ticks
extern t_Process_params PROCESS_PARAMS;				//struct for holding process variables

extern uint16_t lightFill;

extern T_light_params PHOTO1A;		//A-analog D-digital sensor type
extern T_light_params PHOTO2A;
extern T_thermal_sensor THERMI1A;
extern T_encoder ENCODER1;

extern T_pwm_led LEDSTRIP1;
extern T_pwm_led LEDSTRIP2;
extern T_pwm_led LEDSTRIP3;
extern T_pwm_led LEDSTRIP4;

extern T_rel_params REL0STAT;
extern T_rel_params REL1STAT;
extern T_rel_params REL2STAT;

void system_ctrl(void);		//function that refreshes cotnroled parameters after timer tick occured (currently every 1ms)
void adc_vals_assign(void);			//copy adc values from raw buffer to proprietary structs
void pwm_ctrl(void);				//control pwm period fill based on flag manual or auto in process for each channel separetelly eg. ch1 auto ch2 manual ch3 manual etc

uint16_t calculate_light_output(uint16_t sensor,T_pwm_led *led,_Bool linearOutput);
void set_ligth_output(T_pwm_led *led, uint16_t sensor);
void encoder_ctrl(T_encoder *encoder);
void encoder_ctrl(T_encoder *encoder);
void rel_ctrl(T_rel_params *rel);
void rel_callback(T_rel_params *rel);

#endif /* INC_CONTROL_H_ */
