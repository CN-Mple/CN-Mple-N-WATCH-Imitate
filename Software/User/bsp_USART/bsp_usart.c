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
	
	/* ʹ��IO��ʱ�� */
	RCC_APB2PeriphClockCmd(DEBUG_USART_GPIO_CLOCK, ENABLE);
	
	/* ʹ�ܴ���ʱ�� */
	RCC_APB2PeriphClockCmd(DEBUG_USART_CLK, ENABLE);
	
	/* ����TX���� ����������� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);
	/* ����RX���� �������� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_Pin;
	
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	/* ���� USART  */
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BaudRate;
	
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	
	USART_InitStructure.USART_Parity = USART_Parity_No;
	
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
	/* �ж����ȼ����� */
	NVIC_Config();
	/* ʹ�ܽ����ж� */
	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);
	/* ʹ�ܴ��� */
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


///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ����� */
		USART_SendData(DEBUG_USARTx, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USARTx);
}
