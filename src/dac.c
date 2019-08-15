/*
 * dac.c
 *
 *  Created on: 01 ago 2019
 *      Author: franc
 */

#include "stm32f4xx_dac.h"
#include "dac.h"

/**
 * @brief Init DAC peripheral
 * @param function The ouput function to output
 * @param channel The channel to configure
 * @return 0 if OK
 */
uint8_t DAC_fv_init(t_dac_function function, t_dac_channel channel)
{
	uint8_t retVal = 0;
	DAC_InitTypeDef  DAC_InitStructure;
	uint32_t DAC_Channel_var;
	uint32_t trgo_source;

	DAC_StructInit(&DAC_InitStructure);

	// DAC_DeInit();

	if (channel == e_dac_channel_1)
	{
		DAC_Channel_var = DAC_Channel_1;
		trgo_source = DAC_Trigger_T6_TRGO;
	}
	else if (channel == e_dac_channel_2)
	{
		DAC_Channel_var = DAC_Channel_2;
		trgo_source = DAC_Trigger_T7_TRGO;
	}
	else
	{
		return 1;
	}

	switch(function)
	{
		case e_dac_escalator:
		{

		}
		break;
		case e_dac_sine:
		{

		}
		break;
		case e_dac_noise:
		{
			/* DAC channel Configuration */
			DAC_InitStructure.DAC_Trigger = trgo_source;
			DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_Noise;
			DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bits10_0;
			DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
			DAC_Init(DAC_Channel_var, &DAC_InitStructure);

			/* Enable DAC Channel */
			DAC_Cmd(DAC_Channel_var, ENABLE);

			if (channel == e_dac_channel_1)
			{
				/* Set DAC channel2 DHR12RD register */
				DAC_SetChannel1Data(DAC_Align_12b_R, 0x7FF0);
			}
			else if (channel == e_dac_channel_2)
			{
				/* Set DAC channel2 DHR12RD register */
				DAC_SetChannel2Data(DAC_Align_12b_R, 0x7FF0);
			}
		}
		break;
		case e_dac_triangle:
		{
			/* DAC channel Configuration */
			DAC_InitStructure.DAC_Trigger = trgo_source;
			DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_Triangle;
			DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_2047;
			DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
			DAC_Init(DAC_Channel_var, &DAC_InitStructure);

			/* Enable DAC Channel */
			DAC_Cmd(DAC_Channel_var, ENABLE);

			if (channel == e_dac_channel_1)
			{
				/* Set DAC channel2 DHR12RD register */
				DAC_SetChannel1Data(DAC_Align_12b_R, 0x100);
			}
			else if (channel == e_dac_channel_2)
			{
				/* Set DAC channel2 DHR12RD register */
				DAC_SetChannel2Data(DAC_Align_12b_R, 0x100);
			}

		}
		break;
		case e_dac_buffer:
		{
			/* DAC channel1 Configuration */
			DAC_InitStructure.DAC_Trigger = trgo_source;	// probably TO CHANGE
			DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
			DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bits10_0;
			DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
			DAC_Init(DAC_Channel_var, &DAC_InitStructure);

		}
		break;
		default:
		{

		}
		break;

	}

	return retVal;
}

/**
 * @brief Init DAC GPIO
 */
void DAC_FV_initPin(void)
{
	/* Preconfiguration before using DAC----------------------------------------*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/* DMA1 clock and GPIOA clock enable (to be used with DAC) */
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1 | RCC_AHB1Periph_GPIOA, ENABLE);

	/* DAC Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	/* DAC channel 1 & 2 (DAC_OUT1 = PA.4)(DAC_OUT2 = PA.5) configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}


/*

  DMA_DeInit(DMA1_Stream5);
  DMA_InitStructure.DMA_Channel = DMA_Channel_7;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)DAC_DHR12R2_ADDRESS;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&DAC_buffer;
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStructure.DMA_BufferSize = DAC_BUFFERS_SIZE;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA1_Stream5, &DMA_InitStructure);


  DMA_Cmd(DMA1_Stream5, ENABLE);


  DAC_Cmd(DAC_Channel_2, ENABLE);


  DAC_DMACmd(DAC_Channel_2, ENABLE);
 */
