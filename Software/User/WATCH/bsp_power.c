#include "bsp_power.h"

uint16_t ADC_ValueConvertion;

static void Delay(__IO uint32_t nCount) //简单的延时函数
{
	for (; nCount != 0; nCount--);
}

void BSP_POWER_GPIO_Config(void)
{	
	Delay(5000000);
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(POWER_GPIO_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = POWER_GPIO_PIN;
	
	GPIO_Init(POWER_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
		GPIO_SetBits(GPIOA, GPIO_Pin_0);
	Delay(5000000);
}

static void ADC_MODE_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_APB2PeriphClockCmd(ADC_CLK, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_DeInit(ADC_DMA_CHANNEL);
	//外设到寄存器
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADCx->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ValueConvertion;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	//外设地址不自增 存储地址自增
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	// 缓冲区大小，应该等于数据目的地的大小
	DMA_InitStructure.DMA_BufferSize = 1;
	//存储器到存储器不使能
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	//16位 半字
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	//循环模式 DMA高优先级
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	
	ADC_Init(ADCx, &ADC_InitStructure);
	
		// 配置ADC时钟Ｎ狿CLK2的8分频，即9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	
	// 配置ADC 通道的转换顺序和采样时间
	ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);

	
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);
	DMA_Cmd(ADC_DMA_CHANNEL, ENABLE);
	
	// 使能ADC DMA 请求
	ADC_DMACmd(ADCx, ENABLE);
	// 开启ADC ，并开始转换
	ADC_Cmd(ADCx, ENABLE);
	
	// 初始化ADC 校准寄存器  
	ADC_ResetCalibration(ADCx);
	// 等待校准寄存器初始化完成
	while(ADC_GetResetCalibrationStatus(ADCx));
	
	// ADC开始校准
	ADC_StartCalibration(ADCx);
	// 等待校准完成
	while(ADC_GetCalibrationStatus(ADCx));
	
	// 由于没有采用外部触发，所以使用软件触发ADC转换 
	ADC_SoftwareStartConvCmd(ADCx, ENABLE);
}

void POWER_Init(void)
{
	BSP_POWER_GPIO_Config();
	ADC_MODE_Config();
}


