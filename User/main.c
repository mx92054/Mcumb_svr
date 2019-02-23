#include "stm32f10x.h"
#include <string.h>

#include "..\bsp\bsp_led.h"
#include "..\bsp\SysTick.h"
#include "..\bsp\bsp_exti.h"
#include "..\bsp\bsp_innerflash.h"
#include "..\bsp\spi_da.h"
#include "modbus_svr.h"

//--------------------------------------------------------------------------------
int main(void)
{
	SysTick_Init();

	LED_GPIO_Config();
	Modbu_COM1_init();
	Modbu_COM2_init();

	SetTimer(1, 200);
	SetTimer(2, 1000);
	SetTimer(3, 1000); 
	SetTimer(4, 100);

	IWDG_Configuration();
	LED1_OFF;
	while (1)
	{
		Modbus_COM1_task();
		Modbus_COM2_task();

		//------------------------------------------------------------------
		if (GetTimer(2))
		{
			LED1_TOGGLE;
			IWDG_Feed();
		}

		if (GetTimer(3))
		{
			if ( mblock1.bSaved )
				memcpy(mblock2.wReg, mblock1.wReg, 200);
			if ( mblock2.bSaved )
				memcpy(mblock1.wReg, mblock2.wReg, 200);
				
			ModbusSvr_save_para(&mblock1);
			ModbusSvr_save_para(&mblock1);
		}
	}
}
