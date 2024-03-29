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

/**
 * @brief Set of TIM7
 * @param period
 * @param prescaler
 */
void TIM6_Config(uint16_t period, uint16_t prescaler)
{
  /* TIM6CLK = HCLK / 4 = SystemCoreClock /2 = 90 MHz*/

  TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
  /* TIM6 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

  if (period == 0)
  {
	  period = 1u;
  }

  if (prescaler == 0)
  {
	  prescaler = 1u;
  }

  /* Time base configuration */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = period - 1;						/*  Specifies the period value to be loaded into the active Auto-Reload Register at the next update event.*/
  TIM_TimeBaseStructure.TIM_Prescaler = prescaler - 1;						/*Specifies the prescaler value used to divide the TIM clock.*/
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

  /* TIM6 TRGO selection */
  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);

  /* TIM6 enable counter */
  TIM_Cmd(TIM6, ENABLE);
}

/**
 * @brief Set of TIM7
 * @param prescaler
 * @param period
 */
void TIM7_Config(uint16_t period, uint16_t prescaler)
{
  /* TIM7CLK = HCLK / 4 = SystemCoreClock /2 = 90 MHz*/

  TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
  /* TIM7 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

  if (period == 0)
  {
	  period = 1u;
  }

  if (prescaler == 0)
  {
	  prescaler = 1u;
  }

  /* Time base configuration */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = period;
  TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);

  /* TIM7 TRGO selection */
  TIM_SelectOutputTrigger(TIM7, TIM_TRGOSource_Update);

  /* TIM7 enable counter */
  TIM_Cmd(TIM7, ENABLE);
}

/**
 * @brief TIM2 set to trigger ADC => F upd = 9KHz
 */
void TIM2_Config(void)
{
	/* TIM2CLK = HCLK / 4 = SystemCoreClock /2 = 90 MHz*/

	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	/* TIM8 Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/* Time base configuration */
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

	TIM_TimeBaseStructure.TIM_Period = (100 - 1);
	TIM_TimeBaseStructure.TIM_Prescaler = (90 - 1);
	// Sampling an Audio signal: Fsampl >= 7KHz
	// with this setting: 90E6/((100)*(90)) = 10KHz

	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* TIM8 TRGO selection */
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);

	/* TIM8 enable counter */
	TIM_Cmd(TIM2, ENABLE);
}

/**
 *
 * @param timConfig
 * @param newPrescaler
 * @param newARR
 */
void TIM_x_UpdateTimer(t_tim_number timConfig, uint16_t newPrescaler, uint16_t newARR)
{
  TIM_TypeDef *TIMx;

  switch(timConfig)
  {
      case e_tim_6:
      {
        TIMx = TIM6;
      }
      break;
      case e_tim_7:
      {
        TIMx = TIM7;
      }
      break;
      case e_tim_2:
      {
        TIMx = TIM2;
      }
      break;
      default:
      case e_tim_max:
      {

      }
      break;
  }

  TIM_PrescalerConfig(TIMx, newPrescaler, TIM_PSCReloadMode_Update);
  TIM_SetAutoreload(TIMx, newARR);
}
