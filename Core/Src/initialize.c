/*
 * initialize.c
 *
 *  Created on: 21 wrz 2021
 *      Author: x300
 */

#include "initialize.h"


void hardware_initialize(void)
{
	timers_initialize();
	dma_initialize();
	adc_initialize();
	adc_start_conv();
}

