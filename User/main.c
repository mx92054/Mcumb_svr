#include "stm32f10x.h"   
#include <math.h>

#include "..\bsp\bsp_led.h"
#include "..\bsp\SysTick.h"
#include "..\bsp\bsp_exti.h"
#include "..\bsp\bsp_innerflash.h"
#include "..\bsp\adc.h"

#include "modbus_svr.h"
#include "SBG_Comm.h"

#define ZEROVAL  1542

#define BIT_VALUE(val,bit)    ((val & (0x0001 << bit)) >> bit)

short wReg[200] ;
short coils[200] ;
short nEffect[30] ;

extern u8 bSaved ;

static void Handle_Effect(int n) ;
void sort(short array[], int length) ;

//--------------------------------------------------------------------------------
int main(void)
{
	int n ;
	int i ;
	
	SysTick_Init() ;
	InternalFlashRead(wReg, 200) ;	
	
	LED_GPIO_Config() ;
	Modbus_init() ;
	SBG_Init() ;
	ADC1_Init() ;	

	wReg[110]++ ;			// 启动次数加1
	bSaved = 1 ;	
	for(n = 10 ; n < 16 ; n++)
		wReg[n] = 0 ;

	SetTimer(1,200) ;
	SetTimer(2,500) ;
	SetTimer(3,1000) ;
	SetTimer(4,100) ;
	
	IWDG_Configuration() ;
	LED1_OFF ;
	n = 0 ;
	while(1)
	{
		Modbus_task() ;
		SBG_Task() ;
		
		//-----------------------------------------------------------------
		if ( GetTimer(1) )	
		{
			OutputDigital() ;
			InputDigital() ;			
		}		
		
		//------------------------------------------------------------------
		if ( GetTimer(2) )
		{
			LED1_TOGGLE;
			IWDG_Feed() ;
		}
		
		if ( GetTimer(3) && bSaved )
		{
			InternalFlashWrite(wReg, 200) ;
			bSaved = 0 ;
		}
		
		if ( GetTimer(4) && bEffectHandle)
			{
				Handle_Effect(n) ;
				bEffectHandle = 0 ;
				n++ ;
				if ( n > 9 )
				{
					for( i = 0 ; i < 3 ; i++)
					{
						sort(&nEffect[i*10],10) ;
						wReg[i] = nEffect[i*10 + 5] ;
					}
					n = 0 ;
				}
			}			
	}	
}

//--------------------------------------------------------------
//        有效值计算程序
//---------------------------------------------------------------
static void Handle_Effect(int n)
{
	uint16_t 	i,j ;
	uint16_t  *pData, Temp, val, len ;
	uint32_t	lEff[3] ;					//Effective value : sum of square
	uint16_t	min[3], minpos[3], max[3], maxpos[3], bFindMin[3], bFindMax[3] ;

	for(i = 0 ; i < 3 ; i++)
	{
		lEff[i] = 0 ;
		min[i] = wGraph[i] ;
		minpos[i] = 0 ;
		max[i] = wGraph[i] ;
		maxpos[i] = 0 ;
		bFindMin[i] = 1 ;
		bFindMax[i] = 1 ;
	}	
	
	pData = wGraph ;	
	for(i = 0 ; i < 100 ; i++)
	{
		for( j = 0 ; j < 3 ; j++)
		{
			Temp = *pData++ ;
			
			//---------------Find Min value and position----------------------
			if ( bFindMin[j] > 0  )
			{
				if ( Temp < min[j] )
				{
					min[j] = Temp ;
					minpos[j] = i ;
					bFindMin[j] = 2 ;
				}
				
				if ( bFindMin[j] == 2 && Temp > min[j] )
				{
					bFindMin[j] = 0 ;
					max[j] = Temp ;
				}
			}
			
			//---------------Compute------------------------------------------
			if ( bFindMin[j] == 0 && bFindMax[j] == 1 )
			{
				if ( Temp > ZEROVAL ) 
					val = Temp - ZEROVAL ;
				else
					val = ZEROVAL - Temp ;
			
				lEff[j] += val*val ;
			}		
			
			//---------------Find Max value and position----------------------
			if ( bFindMin[j] == 0 && bFindMax[j] > 0 )
			{
				if ( Temp > max[j] )
				{
					max[j] = Temp ;
					maxpos[j] = i ;
					bFindMax[j] = 2 ;
				}
				
				if ( bFindMax[j] == 2 && Temp < max[j] )
				{
					bFindMax[j] = 0 ;
				}		
			}
		}
	}	
	
	for(i = 0 ; i < 3 ; i++)
	{
		len = maxpos[i] - minpos[i] + 1 ;
		if ( len > 10 )
			nEffect[10*i + n] = (short)sqrt(lEff[i]/len) ;		//Effective value is square root
		else
			nEffect[10*i + n] = 0 ;
	}
	
	wReg[31] = minpos[0] ;
	wReg[32] = maxpos[0] ;
	wReg[33] = maxpos[0] - minpos[0] + 1 ;
	wReg[34] = min[0] ;
	wReg[35] = max[0] ;
	
	for(i = minpos[0] ; i <= maxpos[0] ; i ++)
	{
		j = i + 40 - minpos[0] ;
		if ( j < 100 )
			wReg[j] = wGraph[3*i] ;
	}
}


void sort(short array[], int length)
{
	int i, j ;
  short temp = 0;
  for (i = 0; i < length; i++)
  {
		for (j = length - 1; j > i; j--)
		{
				if (array[j] < array[j-1])
				{
						temp = array[j];
						array[j] = array[j-1];
						array[j-1] = temp;
				}
		}
	}
}


