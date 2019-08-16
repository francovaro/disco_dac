/*
 * dac.c
 *
 *  Created on: 01 ago 2019
 *      Author: franc
 */

#include "stm32f4xx_dac.h"
#include "dac.h"
#include "tim.h"

#define 	DAC_BUFFER_SIZE	20

const uint16_t Sine12bit[32] = {
                      2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056,
                      3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909,
                      599, 344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647};

static void DMA_DAC_Config(t_dac_channel channel, t_dac_function function);
static void DMA_DAC_NVIC_Configuration(t_dac_channel channel);

__IO uint16_t 	_dac_buffer_ch1[DAC_BUFFER_SIZE];
__IO uint16_t 	_dac_buffer_ch2[DAC_BUFFER_SIZE];
static uint8_t _dac_buffer_index_ch1;
static uint8_t _dac_buffer_index_ch2;

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
			if (channel == e_dac_channel_1)
			{
				_dac_buffer_index_ch1 = 0;
				/* Set DAC channel1 DHR12RD register */
				DAC_SetChannel1Data(DAC_Align_12b_R, 0x050);
				TIM6_Config(0xFF, 1);
			}
			else if (channel == e_dac_channel_2)
			{
				_dac_buffer_index_ch2 = 0;
				/* Set DAC channel2 DHR12RD register */
				DAC_SetChannel2Data(DAC_Align_12b_R, 0x050);
				TIM7_Config(0xFF, 1);
			}

			/* DAC channelx Configuration */
			DAC_InitStructure.DAC_Trigger = trgo_source;	// probably TO CHANGE
			DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
			DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
			DAC_Init(DAC_Channel_var, &DAC_InitStructure);

			DMA_DAC_Config(channel, function);
		}
		break;
		case e_dac_noise:
		{
			if (channel == e_dac_channel_1)
			{
				/* Set DAC channel1 DHR12RD register */
				DAC_SetChannel1Data(DAC_Align_12b_R, 0x7FF0);
				TIM6_Config(90, 1);
			}
			else if (channel == e_dac_channel_2)
			{
				/* Set DAC channel2 DHR12RD register */
				DAC_SetChannel2Data(DAC_Align_12b_R, 0x7FF0);
				TIM7_Config(180, 1);
			}

			/* DAC channel Configuration */
			DAC_InitStructure.DAC_Trigger = trgo_source;
			DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_Noise;
			DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bits10_0;
			DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
			DAC_Init(DAC_Channel_var, &DAC_InitStructure);

			/* Enable DAC Channel */
			DAC_Cmd(DAC_Channel_var, ENABLE);
		}
		break;
		case e_dac_triangle:
		{
			if (channel == e_dac_channel_1)
			{
				/* Set DAC channel1 DHR12RD register */
				DAC_SetChannel1Data(DAC_Align_12b_R, 0x100);
				TIM6_Config(90, 1);
			}
			else if (channel == e_dac_channel_2)
			{
				/* Set DAC channel2 DHR12RD register */
				DAC_SetChannel2Data(DAC_Align_12b_R, 0x100);
				TIM7_Config(180, 1);
			}

			/* DAC channel Configuration */
			DAC_InitStructure.DAC_Trigger = trgo_source;
			DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_Triangle;
			DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_2047;
			DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
			DAC_Init(DAC_Channel_var, &DAC_InitStructure);

			/* Enable DAC Channel */
			DAC_Cmd(DAC_Channel_var, ENABLE);
		}
		break;
		case e_dac_buffer:
		{
			if (channel == e_dac_channel_1)
			{
				_dac_buffer_index_ch1 = 0;
				/* Set DAC channel1 DHR12RD register */
				DAC_SetChannel1Data(DAC_Align_12b_R, 0x050);
				TIM6_Config(1000, 1);
			}
			else if (channel == e_dac_channel_2)
			{
				_dac_buffer_index_ch2 = 0;
				/* Set DAC channel2 DHR12RD register */
				DAC_SetChannel2Data(DAC_Align_12b_R, 0x050);
				TIM7_Config(1000, 1);
			}

			/* DAC channel1 Configuration */
			DAC_InitStructure.DAC_Trigger = trgo_source;	// probably TO CHANGE
			DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
			//DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bits10_0;
			DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
			DAC_Init(DAC_Channel_var, &DAC_InitStructure);

			DMA_DAC_Config(channel, function);
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

/**
 * @brief Config DMA for DAC
 */
void DMA_DAC_Config(t_dac_channel channel, t_dac_function function)
{
	/*
	 * DMA1
	 * Channel 7
	 * Stream 5 => DAC1
	 * Stream 6 => DAC2
	 */
	DMA_InitTypeDef       DMA_InitStructure;
	uint32_t *pBuffer = 0;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	DMA_InitStructure.DMA_Channel = DMA_Channel_7;

	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;

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

	if (channel == e_dac_channel_1)
	{
		DMA_DeInit(DMA1_Stream5);

		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)DAC->DHR12R1;

		if (function == e_dac_sine)
		{
			DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&Sine12bit;
			DMA_InitStructure.DMA_BufferSize = 32;
		}
		else
		{
			DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&_dac_buffer_ch1[0];
			DMA_InitStructure.DMA_BufferSize = DAC_BUFFER_SIZE;
		}

		DMA_Init(DMA1_Stream5, &DMA_InitStructure);
		DMA_ITConfig(DMA1_Stream5, DMA_IT_TC | DMA_IT_HT,  ENABLE);

		DMA_Cmd(DMA1_Stream5, ENABLE);

		DMA_DAC_NVIC_Configuration(e_dac_channel_1);

		DAC_Cmd(DAC_Channel_1, ENABLE);

		DAC_DMACmd(DAC_Channel_1, ENABLE);	
	}
	else if (channel == e_dac_channel_2)
	{
		DMA_DeInit(DMA1_Stream6);

		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)DAC->DHR12R2;

		if (function == e_dac_sine)
		{
			DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&Sine12bit;
			DMA_InitStructure.DMA_BufferSize = 32;
		}
		else
		{
			DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&_dac_buffer_ch2[0];
			DMA_InitStructure.DMA_BufferSize = DAC_BUFFER_SIZE;
		}

		DMA_Init(DMA1_Stream6, &DMA_InitStructure);
		DMA_ITConfig(DMA1_Stream6, DMA_IT_TC | DMA_IT_HT,  ENABLE);

		DMA_Cmd(DMA1_Stream6, ENABLE);

		DMA_DAC_NVIC_Configuration(e_dac_channel_2);

		DAC_Cmd(DAC_Channel_2, ENABLE);

		DAC_DMACmd(DAC_Channel_2, ENABLE);	
	}
	
}

void DMA_DAC_NVIC_Configuration(t_dac_channel channel)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	if (channel == e_dac_channel_1)
	{
		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream5_IRQn;
	}
	else if (channel == e_dac_channel_2)
	{
		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
	}

	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);
}

void DMA_Feed_Buffer(uint16_t newSample, t_dac_channel channel)
{
	if (channel == e_dac_channel_1)
	{
		_dac_buffer_ch1[_dac_buffer_index_ch1] = newSample;
		_dac_buffer_index_ch1 = (_dac_buffer_index_ch1 + 1) % DAC_BUFFER_SIZE;
	}
	else
	{
		_dac_buffer_ch2[_dac_buffer_index_ch2] = newSample;
		_dac_buffer_index_ch2 = (_dac_buffer_index_ch2 + 1) % DAC_BUFFER_SIZE;
	}

}

void DMA1_Stream5_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_Stream5, DMA_IT_HTIF0))
	{
		DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_HTIF0);
	}

	if(DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF0))
	{
		DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF0);
	}
}

void DMA1_Stream6_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_Stream6, DMA_IT_HTIF0))
	{
		DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_HTIF0);
	}

	if(DMA_GetITStatus(DMA1_Stream6, DMA_IT_TCIF0))
	{
		DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF0);
	}
}
