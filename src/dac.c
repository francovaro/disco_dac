/*
 * dac.c
 *
 *  Created on: 01 ago 2019
 *      Author: franc
 */

#include "stm32f4xx_dac.h"
#include "dac.h"

uint8_t DAC_fv_init(t_dac_function function, t_dac_channel channel)
{
	uint8_t retVal = 0;
	DAC_InitTypeDef  DAC_InitStructure;
	uint32_t DAC_Channel_var;

	DAC_StructInit(&DAC_InitStructure);

	// DAC_DeInit();

	if (channel == e_dac_channel_1)
	{
		DAC_Channel_var = DAC_Channel_1;
	}
	else if (channel == e_dac_channel_2)
	{
		DAC_Channel_var = DAC_Channel_2;
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
			 /* DAC channel1 Configuration */
			  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
			  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_Noise;
			  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bits10_0;
			  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
			  DAC_Init(DAC_Channel_var, &DAC_InitStructure);

			  /* Enable DAC Channel1 */
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
			 /* DAC channel2 Configuration */
			  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
			  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_Triangle;
			  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_1023;
			  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
			  DAC_Init(DAC_Channel_var, &DAC_InitStructure);

			  /* Enable DAC Channel2 */
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
		default:
		{

		}
		break;

	}

	return retVal;
}

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
