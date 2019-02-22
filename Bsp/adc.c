/*********************************************************************************
 * �ļ���  ��adc.c
**********************************************************************************/
#include "adc.h"
#include "bsp_led.h"

#include <string.h>

#define ADC1_DR_Address ((u32)0x40012400 + 0x4c)

__IO uint16_t ADC_ConvertedValue[CYCLE_LEN * 3];
uint16_t wGraph[CYCLE_LEN * 3];
int bEffectHandle = 0;

/*
 * ��������ADC1_GPIO_Config
 * ����  ��ʹ��ADC1��DMA1��ʱ�ӣ���ʼ��PA.1 PA.2 PA.3
 * ����  : ��
 * ���  ����
 * ����  ���ڲ�����
 */
static void ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable DMA clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	RCC_APB2PeriphClockCmd(AI1_GPIO_CLK, ENABLE);
	/* Configure PA4  as analog input */
	GPIO_InitStructure.GPIO_Pin = AI1_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(AI1_GPIO_PORT, &GPIO_InitStructure); // AI1

	RCC_APB2PeriphClockCmd(AI2_GPIO_CLK, ENABLE);
	/* Configure PA5  as analog input */
	GPIO_InitStructure.GPIO_Pin = AI2_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(AI2_GPIO_PORT, &GPIO_InitStructure); // AI2

	RCC_APB2PeriphClockCmd(AI3_GPIO_CLK, ENABLE);
	/* Configure PA6 as analog input */
	GPIO_InitStructure.GPIO_Pin = AI3_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(AI3_GPIO_PORT, &GPIO_InitStructure); // AI3
}

/* ��������ADC1_Mode_Config
 * ����  ������ADC1�Ĺ���ģʽΪMDAģʽ
 * ����  : ��
 * ���  ����
 * ����  ���ڲ�����
 */
static void ADC1_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	/* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;		 //ADC��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue; //�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = CYCLE_LEN * 3;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//�����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//�ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //����
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //ѭ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	/* Enable DMA Intrupet */
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
	DMA_ClearITPendingBit(DMA_IT_TC);

	/* Enable DMA channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);

	/* ADC1 configuration */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;					//����ADCģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;						//����ɨ��ģʽ��ɨ��ģʽ���ڶ�ͨ���ɼ�
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;					//��������ת��ģʽ������ͣ�ؽ���ADCת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //��ʹ���ⲿ����ת��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;				//�ɼ������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 3;								//Ҫת����ͨ����Ŀ3
	ADC_Init(ADC1, &ADC_InitStructure);

	/*����ADCʱ�ӣ�ΪPCLK2��8��Ƶ����9MHz*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	/*����ADC1��ͨ��11Ϊ55.	5���������ڣ�����Ϊ1 */
	ADC_RegularChannelConfig(ADC1, AI1_CHANNEL, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, AI2_CHANNEL, 2, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, AI3_CHANNEL, 3, ADC_SampleTime_239Cycles5);

	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/*��λУ׼�Ĵ��� */
	ADC_ResetCalibration(ADC1);
	/*�ȴ�У׼�Ĵ�����λ��� */
	while (ADC_GetResetCalibrationStatus(ADC1))
		;

	/* ADCУ׼ */
	ADC_StartCalibration(ADC1);
	/* �ȴ�У׼���*/
	while (ADC_GetCalibrationStatus(ADC1))
		;

	/* ����û�в����ⲿ����������ʹ���������ADCת�� */
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/*
 * ��������DMA_NVIC_Configuration
 * ����  ��DMA�ж�����
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
static void DMA_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*
 * ��������ADC1_Init
 * ����  ����
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void ADC1_Init(void)
{
	DMA_NVIC_Configuration();
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
}

/*******************************************************************************
* Function Name  : DMA1_Channel1_IRQHandler
* Description    : This function handles DMA1 Channel 1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel1_IRQHandler(void)
{
	if (DMA_GetITStatus(DMA_IT_TC) == SET)
	{
		DMA_ClearITPendingBit(DMA_IT_TC);
		if (bEffectHandle == 0)
		{
			memcpy((void *)wGraph, (void *)ADC_ConvertedValue, CYCLE_LEN * 3 * 2);
			bEffectHandle = 1;
		}
	}
}

/************************END OF FILE************/
