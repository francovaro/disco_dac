/*
 * adc.h
 *
 *  Created on: 11 ago 2019
 *      Author: franc
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

#include <stdint.h>

__IO uint8_t gDMA_FT_event;
__IO uint8_t gDMA_HT_event;

void ADC_fv_Init(void);

#endif /* INC_ADC_H_ */
