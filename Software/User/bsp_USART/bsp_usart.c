#include "bsp_usart.h"

static void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}

void USART_Config(void)
{
	
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* 使能IO口时钟 */
	RCC_APB2PeriphClockCmd(DEBUG_USART_GPIO_CLOCK, ENABLE);
	
	/* 使能串口时钟 */
	RCC_APB2PeriphClockCmd(DEBUG_USART_CLK, ENABLE);
	
	/* 配置TX引脚 复用推挽输出 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);
	/* 配置RX引脚 浮空输入 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_Pin;
	
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	/* 配置 USART  */
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BaudRate;
	
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	
	USART_InitStructure.USART_Parity = USART_Parity_No;
	
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
	/* 中断优先级配置 */
	NVIC_Config();
	/* 使能接收中断 */
	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);
	/* 使能串口 */
	USART_Cmd(DEBUG_USARTx, ENABLE);
}


void USART_SendByte(USART_TypeDef* USARTx, uint8_t Data)
{
	USART_SendData(USARTx, Data);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}

void USART_SendString(USART_TypeDef* USARTx, char *str)
{
	unsigned int k=0;
	do 
  {
      USART_SendByte( USARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}


///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		USART_SendData(DEBUG_USARTx, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USARTx);
}
