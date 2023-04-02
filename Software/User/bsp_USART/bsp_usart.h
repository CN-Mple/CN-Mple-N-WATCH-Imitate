#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "stm32f10x.h"
#include "stdio.h"

#define DEBUG_USARTx					USART1

#define DEBUG_USART_BaudRate			115200

#define DEBUG_USART_CLK 				RCC_APB2Periph_USART1
#define DEBUG_USART_GPIO_CLOCK			RCC_APB2Periph_GPIOA

#define DEBUG_USART_TX_GPIO_PORT		GPIOA
#define DEBUG_USART_TX_GPIO_Pin			GPIO_Pin_9
#define DEBUG_USART_RX_GPIO_PORT		GPIOA
#define DEBUG_USART_RX_GPIO_Pin			GPIO_Pin_10

#define DEBUG_USART_IRQn				USART1_IRQn


void USART_Config(void);
void USART_SendByte(USART_TypeDef* USARTx, uint8_t Data);
void USART_SendString(USART_TypeDef* USARTx, char *str);



#endif /* __BSP_USART_H */

