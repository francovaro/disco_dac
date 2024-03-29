/*
 * uart.c
 *
 *  Created on: 05 ott 2019
 *      Author: franc
 */

#include "uart.h"

static void USART1_NVIC_Config(void);

/**
 *
 */
void UART_fv_config(uint8_t irqEnabled)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStructure;

	usart_serialReceived = 0;
	usart_nrOfByte = 0;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	GPIO_StructInit(&GPIO_InitStruct);
	USART_StructInit(&USART_InitStructure);

	// rx PA10
	// tx PA9
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	// Initialize USART1_Tx
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOA , &GPIO_InitStruct);

	 /*!< This member configures the USART communication baud rate.
	     The baud rate is computed using the following formula:

	     - IntegerDivider = ((PCLKx) / (8 * (OVR8+1) * (USART_InitStruct->USART_BaudRate)))
	     - FractionalDivider = ((IntegerDivider - ((u32) IntegerDivider)) * 8 * (OVR8+1)) + 0.5
	 Where OVR8 is the "oversampling by 8 mode" configuration bit in the CR1 register. */

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1 ,&USART_InitStructure);

	if(irqEnabled == ENABLE)
	{
		//USART1
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); /* ENABLE INTERRUPT ON rx */
		USART1_NVIC_Config();
	}

	USART_Cmd(USART1 , ENABLE);
}

/**
 *
 */
void UART_fv_SendData(const char * strToSend, uint16_t byteToSend)
{
	uint16_t count;

	for(count = 0 ; count < byteToSend ; count++)
	{
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		USART_SendData(USART1, *strToSend++);

	}
}

int uart_putc(int c)
{
  assert_param(IS_USART_123_PERIPH(USARTx));

  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
  USART1->DR =  (c & 0xff);

  return 0;
}

/**
 *
 */
static void USART1_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
 *
 */
void USART1_IRQHandler(void)
{
	 /*
	  * TODO
	  * handle request from PC !
	  */

	 if( USART_GetITStatus(USART1,USART_IT_RXNE ) != RESET)
	 {
		usart_rxBuffer[usart_nrOfByte] = USART_ReceiveData(USART1);
		usart_nrOfByte++;
		usart_serialReceived = SET;
	 }
}
