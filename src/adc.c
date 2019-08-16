/*
 * adc.c
 *
 *  Created on: 11 ago 2019
 *      Author: franc
 */

#include "stm32f4xx_dac.h"
#include "adc.h"

#define			ADC_BUFFER_SIZE	25

static void DMA_ADC_Config(void);
static void DMA_NVIC_Configuration(void);

__IO uint16_t 	adc_buffer[ADC_BUFFER_SIZE];
__IO uint16_t	_adc_read;

/**
 * @brief Init ADC peripheral
 * @param void
 * @return void
 */
void ADC_fv_Init(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	DMA_ADC_Config();
	// PA1 ADc1 channel 1
}

/**
 * @brief Init DMA for ADC
 * @param void
 * @return void
 */
void DMA_ADC_Config(void)
{
	DMA_InitTypeDef       DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	/* ADC1: DMA2 stream 0 channel 0*/

	/* DMA2 Stream0 channel2 configuration **************************************/
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&ADC1->DR);
	/* destination ! */
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&_adc_read;

	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;

	DMA_InitStructure.DMA_BufferSize = 1;

	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	DMA_InitStructure.DMA_MemoryInc = DMA_PeripheralInc_Disable;


	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;

	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;

	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;	// not sure
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	DMA_Init(DMA2_Stream0, &DMA_InitStructure);

	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC /*| DMA_IT_HT*/,  ENABLE);
	DMA_Cmd(DMA2_Stream0, ENABLE);

	_DMA_FT_event = RESET;
	_DMA_HT_event = RESET;

	//while(DMA_GetCmdStatus(DMA2_Stream0)!=ENABLE);		//controllo se il DMA è a posto

	DMA_NVIC_Configuration();
}

/**
 * @brief Init DMA for ADC
 * @param void
 * @return void
 */
void DMA_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
