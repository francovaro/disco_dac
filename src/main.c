/**
  ******************************************************************************
  * @file    main.c
  * @author  Fra
  * @version V1.0
  * @date    01-08-2019
  * @brief   Default main file.
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
#include "uart.h"
#include <string.h>
			
static void _initLed(void);
static void setSysTick(uint32_t timeMs);
static void _converti(uint16_t dato,char str[]);
static void _sendData(uint16_t num);

static __IO uint8_t sysTickExpired = 0;

int main(void)
{
	_initLed();

	TIM2_Config();	/* TIM 2 init to output TIM_TRGOSource_Update */
	ADC_fv_Init();	/* set ADC3 channel 4 */

	DAC_FV_initPin();	/* init GPIOA 4 and GPIOA 5 */

	DAC_fv_init(e_dac_buffer, e_dac_channel_1);		/* init DAC ch 1 */
	DAC_fv_init(e_dac_triangle, e_dac_channel_2);	/* init DAC ch 2 */

	UART_fv_config(DISABLE);

	setSysTick (1000);
	GPIO_WriteBit(GPIOG, GPIO_Pin_13, SET);
	GPIO_WriteBit(GPIOG, GPIO_Pin_14, RESET);

	UART_fv_SendData("START\n", strlen("START\n"));

	while(1)
	{
		if (SET == gDMA_FT_event)
		{
			gDMA_FT_event = RESET;
			DMA_Feed_Buffer(ADC_return_val(1), e_dac_channel_1);
			DMA_Feed_Buffer(ADC_return_val(1), e_dac_channel_2);
			GPIO_ToggleBits(GPIOG, GPIO_Pin_13);
			_sendData(ADC_return_val(1));
		}

		if (SET == gDMA_HT_event)
		{
			gDMA_HT_event = RESET;
			DMA_Feed_Buffer(ADC_return_val(0), e_dac_channel_1);
			DMA_Feed_Buffer(ADC_return_val(0), e_dac_channel_2);
			GPIO_ToggleBits(GPIOG, GPIO_Pin_14);
			_sendData(ADC_return_val(0));
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
		}
	}
}

/**
 *
 */
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

/**
 *
 * @param timeMs
 */
void setSysTick(uint32_t timeMs)
{
	 RCC_ClocksTypeDef RCC_Clocks;

	 RCC_GetClocksFreq(&RCC_Clocks);
	 SysTick_Config((RCC_Clocks.SYSCLK_Frequency/180)/timeMs); // hz/s
}

/**
 *
 */
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

void _converti(uint16_t dato, char str[])
{
	unsigned char iniziato=0,quoz=0,index=0;
	unsigned int base,rest;

	for (base=1000;base;base/=10)
	{
		quoz=dato/base;
		rest=dato%base;
		if(!iniziato)
		{
			if(quoz)
			{
				str[index++]=quoz + '0';
				iniziato=1;
			}
		}
		else
		{
			str[index++]=quoz + '0';
		}
		dato=rest;
	}
	if(!iniziato)
	{
		str[index++]=quoz + '0';
	}

	str[index]=0;
}

void _sendData(uint16_t num)
{
	/*
	 * trasmettendo a 115.000 BPS posso tramettere circa 11.000 bytes in un secondo
	 * con 100 dati su strnga da 10 ho 1000 bytes , ce la faccio ...*/
	char str_dato[10];

	strcpy(str_dato,"         ");

	_converti(num,str_dato);
	strcat (str_dato,"\n");
	UART_fv_SendData(str_dato, strlen(str_dato));
}
