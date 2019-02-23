/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   led应用函数接口
  */

#include "bsp_led.h"

/**
  * @brief  初始化控制LED的IO
  * @param  无
  * @retval 无
  */
void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(LED1_GPIO_CLK | DO_GPIO_CLK | DI_GPIO_CLK, ENABLE);

	//AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// JTAG-DP禁用  SW-DP使能
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

	//--------------Lamp-------------------------------------
	GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);

}


//----------------------------------------------------------------------------------
void BKP_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	BKP_ClearFlag();
}

//--------------------------------------------------------------
//      Watchdong config
//			cycle = load_val*Prescaler/40k
//---------------------------------------------------------------
void IWDG_Configuration(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_16);
	IWDG_SetReload(0xFFF); // 0xfff*16/40k = 1.6s
	IWDG_ReloadCounter();
	IWDG_Enable();
}

//--------------------------------------------------------------
//      feed dog function
//---------------------------------------------------------------
void IWDG_Feed(void)
{
	IWDG->KR = 0XAAAA;
}

/*********************************************END OF FILE**********************/
