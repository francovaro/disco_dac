/*
 * dac.h
 *
 *  Created on: 01 ago 2019
 *      Author: franc
 */

#ifndef INC_DAC_H_
#define INC_DAC_H_

#include <stdint.h>

typedef enum
{
	e_dac_escalator 	= 0,
	e_dac_sine 			= 1u,
	e_dac_noise 		= 2u,
	e_dac_triangle	 	= 3u,
	e_dac_buffer	 	= 4u,
	e_dac_max
} t_dac_function;

typedef enum
{
	e_dac_channel_1 	= 0,
	e_dac_channel_2		= 1u,
	e_dac_channel_max
} t_dac_channel;

extern void DAC_FV_initPin(void);
extern uint8_t DAC_fv_init(t_dac_function function, t_dac_channel channel);

#endif /* INC_DAC_H_ */
