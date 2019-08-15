/*
 * tim.c
 *
 *  Created on: 01 ago 2019
 *      Author: franc
 */

/**
  * @brief  TIM6 Configuration
  * @note   TIM6 configuration is based on CPU @180MHz and APB1 @45MHz
  * @note   TIM6 Update event occurs each TIM6CLK(MHz)/256
  * @param  None
  * @retval None
  */

#include "stm32f4xx_tim.h"
#include "tim.h"

void TIM6_Config(void)
{
  /* TIM6CLK = HCLK / 4 = SystemCoreClock /2 = 90 MHz*/

  TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
  /* TIM6 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

  /* Time base configuration */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = 90-1; //(1 - 1);						/*  Specifies the period value to be loaded into the active Auto-Reload Register at the next update event.*/
  TIM_TimeBaseStructure.TIM_Prescaler = 0; //(90 - 1);						/*Specifies the prescaler value used to divide the TIM clock.*/
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

  /* TIM6 TRGO selection */
  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);

  /* TIM6 enable counter */
  TIM_Cmd(TIM6, ENABLE);
}

void TIM7_Config(void)
{
  /* TIM7CLK = HCLK / 4 = SystemCoreClock /2 = 90 MHz*/

  TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
  /* TIM7 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

  /* Time base configuration */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = 180-1;//(1 -1);
  TIM_TimeBaseStructure.TIM_Prescaler = 0;//(180- 1);
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);

  /* TIM7 TRGO selection */
  TIM_SelectOutputTrigger(TIM7, TIM_TRGOSource_Update);

  /* TIM7 enable counter */
  TIM_Cmd(TIM7, ENABLE);
}
