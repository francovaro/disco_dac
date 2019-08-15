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

/*
Granular synthesis
In digital music circles, granular synthesis is a technique of joining and layering lots of very short
audio samples (or “grains”) — on the order of one to a few milliseconds — to build up more complex
sounds or instruments. Now picture just a single “grain,” 10 milliseconds or so...and we continually
refresh this one grain from a live microphone. By time-compressing or -stretching this one tiny loop,
repeating or dropping short segments to keep up with realtime, we have the basis for a realtime
pitch shifter. It really seems like this shouldn’t work...but it does! Speech waveforms tend to repeat
over the very short term, and we can drop or repeat some of those waves with only minor

The frequency range of human voice covers about 300 Hz to 3,500 Hz (and harmonics may extend
above this). The Nyquist sampling theorem (http://adafru.it/aQ3) states that a minimum 2X sample
rate is needed to faithfully reconstruct a signal. For human voice, that means 7 KHz sampling...but
a little more wouldn’t hurt.
Repeatedly calling the Arduino’s standard analogRead() function in a loop is way, WAY too slow for
this. We need to get deeper into the works of the Arduino’s analog-to-digital converter, fiddling
directly with special registers and modes. A capability called free-run mode collects analog samples
at a fast, fixed interval without repeated polling in our code. An interrupt handler is automatically
called each time a new sample is ready, which happens like clockwork. Running full tilt, a 16 MHz
Arduino can capture 9,615 10-bit samples per second. More than enough for sampling voice!
The audio samples are stored in a circular buffer, which is really just big fancy computer science
words for “when you reach the end of the buffer, roll back around to the beginning and write over it.”
But conceptually, it helps to think of it as a literal circle:

The frequency of recorded sound will seldom match the buffer length exactly, and audio samples
are stored and read at different rates. This can produce a sharp discontinuity — a popping noise —
each time the “in” and “out” points cross. A small extra buffer is used to store some of the prior
audio samples, and the code cross-fades the audio over this boundary to reduce the “pop.”
Because our audio “grain” is relatively short (about 10 milliseconds), the RAM requirements should
be fairly modest, a few hundred bytes. Problem is, we’d also like to continue doing those things that
the Wave Shield was designed for — namely, playing back WAV files. That requires reading files
from an SD card, and that in turn consumes lots of RAM. Fortunately the design of the WAV-playing
code lets us gain access that library’s memory and recycle it for our own needs.
The technical details are all well-commented in the source code. So if you’re curious about the
specifics of this implementation...use the source, Luke!
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

	ADC_fv_Init();

	//TIM6_Config();
	//TIM7_Config();
	
	TIM8_Config();

	DAC_FV_initPin();

	DAC_fv_init(e_dac_triangle, e_dac_channel_2);
	DAC_fv_init(e_dac_triangle, e_dac_channel_1);

	setSysTick (1000);
	GPIO_WriteBit(GPIOG, GPIO_Pin_13, SET);
	GPIO_WriteBit(GPIOG, GPIO_Pin_14, RESET);

	while(1)
	{
		if (SET == gDMA_FT_event)
		{
			gDMA_FT_event = RESET;
			DMA_Feed_Buffer(ADC_return_val(1));
			GPIO_ToggleBits(GPIOG, GPIO_Pin_13);
		}

		if (SET == gDMA_HT_event)
		{
			gDMA_HT_event = RESET;
			DMA_Feed_Buffer(ADC_return_val(0));
			GPIO_ToggleBits(GPIOG, GPIO_Pin_13);
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
			GPIO_ToggleBits(GPIOG, GPIO_Pin_14);
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
