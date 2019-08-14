/*
 * tim.h
 *
 *  Created on: 01 ago 2019
 *      Author: franc
 */

#ifndef INC_TIM_H_
#define INC_TIM_H_

#include <stdint.h>

typedef enum
{
	e_tim_6 	= 0,
	e_tim_7		= 1u,
	e_tim_2 	= 2u,
	e_tim_max
} t_tim_number;

/*
 * TODO
 * just one with param to set?
 * maybe in the future...
 */
extern void TIM6_Config(uint16_t prescaler, uint16_t period);
extern void TIM7_Config(uint16_t prescaler, uint16_t period);
extern void TIM8_Config(void);
extern void TIM_x_UpdateTimer(t_tim_number timConfig, uint16_t newPrescaler, uint16_t newARR);

#endif /* INC_TIM_H_ */
