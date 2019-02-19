#include "stm32f10x.h"
#include <math.h>

#include "..\bsp\bsp_led.h"
#include "..\bsp\SysTick.h"
#include "..\bsp\bsp_exti.h"
#include "..\bsp\bsp_innerflash.h"

#include "modbus_svr.h"

//--------------------------------------------------------------------------------
int main(void)
{
	SysTick_Init();
	InternalFlashRead(wReg, 200);
	BOOTNUM++;
	bSaved = 1;

	LED_GPIO_Config();
	Modbus_init();

	SetTimer(1, 200);
	SetTimer(2, 500);
	SetTimer(3, 1000);
	SetTimer(4, 100);

	IWDG_Configuration();
	LED1_OFF;
	while (1)
	{
		Modbus_task();

		//------------------------------------------------------------------
		if (GetTimer(2))
		{
			LED1_TOGGLE;
			IWDG_Feed();
		}

		if (GetTimer(3) && bSaved)
		{
			InternalFlashWrite(wReg, 200);
			bSaved = 0;
		}
	}
}
