/**
  ******************************************************************************
  * @file    dma.c
  * @brief   This file provides code for the configuration
  *          of all the requested memory to memory DMA transfers.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "dma.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure DMA                                                              */
/*----------------------------------------------------------------------------*/

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * Enable DMA controller clock
  */
void MX_DMA_Init(void)
{

  /* Init with LL driver */
  /* DMA controller clock enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2);

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  NVIC_SetPriority(DMA2_Stream0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

/* USER CODE BEGIN 2 */
void dma_initialize(void)
{
	  //Dma Adc enable
	  LL_ADC_Disable(ADC1);
	  LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_0);
	  LL_DMA_ConfigAddresses(DMA2,
			  	  	  	  	  LL_DMA_STREAM_0,
							  LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA),
							  (uint32_t)&adc_readouts_bufer,
							  LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
	  LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_0, ADC_CONV_NUMBERS);
	  LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_0);
	  LL_DMA_EnableIT_TE(DMA2, LL_DMA_STREAM_0);
	  LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_0);
}
/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
