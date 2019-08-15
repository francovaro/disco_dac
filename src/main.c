/**
  ******************************************************************************
  * @file    main.c
  * @author  Fra
  * @version V1.0
  * @date    01-08-2019
  * @brief   Default main function.
  ******************************************************************************
*/


/*
 * Base use of DAC
 * SYSCLK 180 MHz
 * HCLK = 180 MHz
 * APB1 = 45 MHz
 * APB2 = 90 MHz
 * AHB  = 180 MHz
 * APB1 & APB2 TIM CLK = 90 MHz
 */

#include "stm32f4xx.h"
#include "adc.h"
#include "dac.h"
#include "tim.h"
//#include "button.h"
			
static void _initLed(void);
static void setSysTick(uint32_t timeMs);

static __IO uint8_t sysTickExpired = 0;

int main(void)
{
	SystemCoreClockUpdate();
	_initLed();

	TIM2_Config();
	ADC_fv_Init();

	DAC_FV_initPin();

	DAC_fv_init(e_dac_triangle, e_dac_channel_2);
	DAC_fv_init(e_dac_buffer, e_dac_channel_1);

	setSysTick (1000);
	GPIO_WriteBit(GPIOG, GPIO_Pin_13, RESET);
	GPIO_WriteBit(GPIOG, GPIO_Pin_14, SET);

	while(1)
	{
		if (SET == gDMA_FT_event)
		{
			gDMA_FT_event = RESET;
			DMA_Feed_Buffer(ADC_return_val(1), e_dac_channel_1);
			DMA_Feed_Buffer(ADC_return_val(1), e_dac_channel_2);
			GPIO_ToggleBits(GPIOG, GPIO_Pin_13);
		}

		if (SET == gDMA_HT_event)
		{
			gDMA_HT_event = RESET;
			DMA_Feed_Buffer(ADC_return_val(0), e_dac_channel_1);
			DMA_Feed_Buffer(ADC_return_val(0), e_dac_channel_2);
			GPIO_ToggleBits(GPIOG, GPIO_Pin_14);
		}
/*
		if (SET == gGPIO_UP)
		{
			gGPIO_UP = RESET;
		}

		if (SET == gGPIO_DOWN)
		{
			gGPIO_DOWN = RESET;
		}
*/
		if (sysTickExpired)
		{
			sysTickExpired = 0;			
			//GPIO_ToggleBits(GPIOG, GPIO_Pin_14);
		}
	}
}


void _initLed(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = ( GPIO_Pin_13 | GPIO_Pin_14);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOG , &GPIO_InitStructure);
}

void setSysTick(uint32_t timeMs)
{
	 RCC_ClocksTypeDef RCC_Clocks;
	 SystemCoreClockUpdate();
	 RCC_GetClocksFreq(&RCC_Clocks);
	 SysTick_Config((RCC_Clocks.SYSCLK_Frequency/180)/timeMs); // hz/s
}

void SysTick_Handler(void)
{
	static uint16_t  count;
	count++;
	if(count == 5000)
	{
		sysTickExpired = 1;
		count = 0;
	}
}
