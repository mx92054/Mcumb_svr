#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

/* ����LED���ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����Ĵ��뼴�ɸı���Ƶ�LED���� */
// R-��ɫ
#define LED1_GPIO_PORT GPIOB			   /* GPIO�˿� */
#define LED1_GPIO_CLK RCC_APB2Periph_GPIOB /* GPIO�˿�ʱ�� */
#define LED1_GPIO_PIN GPIO_Pin_5		   /* ���ӵ�SCLʱ���ߵ�GPIO */

#define DO_GPIO_PORT GPIOB
#define DO_GPIO_CLK RCC_APB2Periph_GPIOB
#define DO_GPIO_PINS GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | \
						 GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7

#define DI_GPIO_PORT GPIOB
#define DI_GPIO_CLK RCC_APB2Periph_GPIOB
#define DI_GPIO_PINS GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | \
						 GPIO_Pin_14 | GPIO_Pin_15

/** the macro definition to trigger the led on or off 1 - off 0 - on */
#define ON 0
#define OFF 1

#define LED1(a)                                      \
	if (a)                                           \
		GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN); \
	else                                             \
		GPIO_ResetBits(LED1_GPIO_PORT, LED1_GPIO_PIN)

/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define digitalHi(p, i) \
	{                   \
		p->BSRR = i;    \
	} //���Ϊ�ߵ�ƽ
#define digitalLo(p, i) \
	{                   \
		p->BRR = i;     \
	} //����͵�ƽ
#define digitalToggle(p, i) \
	{                       \
		p->ODR ^= i;        \
	} //�����ת״̬

/* �������IO�ĺ� */
#define LED1_TOGGLE digitalToggle(LED1_GPIO_PORT, LED1_GPIO_PIN)
#define LED1_OFF digitalLo(LED1_GPIO_PORT, LED1_GPIO_PIN)
#define LED1_ON digitalHi(LED1_GPIO_PORT, LED1_GPIO_PIN)

void LED_GPIO_Config(void);
void BKP_Init(void);
void IWDG_Configuration(void);
void IWDG_Feed(void);

#endif /* __LED_H */
