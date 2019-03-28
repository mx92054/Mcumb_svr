#include "stm32f10x.h"
#include <string.h>

#include "..\bsp\bsp_led.h"
#include "..\bsp\SysTick.h"
#include "..\bsp\bsp_innerflash.h"
#include "modbus_svr.h"
#include "plant.h"

//--------------------------------------------------------------------------------
int main(void)
{
	int i;

	SysTick_Init();

	LED_GPIO_Config();
	Modbu_COM1_init();
	Modbu_COM2_init();
	plant_init(&plant);

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
			if (mblock1.bSaved)
				memcpy(mblock2.ptrRegs, mblock1.ptrRegs, 200);
			if (mblock2.bSaved)
				memcpy(mblock1.ptrRegs, mblock2.ptrRegs, 200);

			ModbusSvr_save_para(&mblock1);
		}

		if (GetTimer(4))
		{
			plant_step(&plant, (float)mblock1.ptrRegs[80]);
			i = (int)(plant.angle * 100);
			mblock1.ptrRegs[81] = i > 16;
			mblock1.ptrRegs[82] = (short)i;
			i = (int)(plant.dangle * 100);
			mblock1.ptrRegs[83] = i > 16;
			mblock1.ptrRegs[84] = (short)i;

			mblock1.ptrRegs[85]++;
		}
	}
}
