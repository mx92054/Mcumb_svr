
#ifndef __MODBUS_COM__
#define __MODBUS_COM__

#include "stm32f10x.h"
#include "Mbsvr_comm.h"

//-------------------串口1宏定义------------------------------------
// 串口1-USART1
#define COM1 USART1
#define COM1_CLK RCC_APB2Periph_USART1
#define COM1_APBxClkCmd RCC_APB2PeriphClockCmd

// USART GPIO 引脚宏定义
#define COM1_GPIO_CLK (RCC_APB2Periph_GPIOA)
#define COM1_GPIO_APBxClkCmd RCC_APB2PeriphClockCmd

#define COM1_TX_GPIO_PORT GPIOA
#define COM1_TX_GPIO_PIN GPIO_Pin_9
#define COM1_RX_GPIO_PORT GPIOA
#define COM1_RX_GPIO_PIN GPIO_Pin_10

#define COM1_IRQ USART1_IRQn
#define COM1_IRQHandler USART1_IRQHandler

//-------------------串口2宏定义------------------------------------
#define COM2 USART2
#define COM2_CLK RCC_APB1Periph_USART2
#define COM2_APBxClkCmd RCC_APB1PeriphClockCmd

// USART GPIO 引脚宏定义
#define COM2_GPIO_CLK (RCC_APB2Periph_GPIOA)
#define COM2_GPIO_APBxClkCmd RCC_APB2PeriphClockCmd

#define COM2_TX_GPIO_PORT GPIOA
#define COM2_TX_GPIO_PIN GPIO_Pin_2
#define COM2_RX_GPIO_PORT GPIOA
#define COM2_RX_GPIO_PIN GPIO_Pin_3

#define COM2_IRQ USART2_IRQn
#define COM2_IRQHandler USART2_IRQHandler

extern Modbus_block mblock1;
extern Modbus_block mblock2;
//------------------Function Define ----------------------------------
void Modbu_COM1_init(void);
void Modbus_COM1_task(void);
void ModbusTimer_COM1(void);
void COM1_IRQHandler(void);

//------------------Function Define ----------------------------------
void Modbu_COM2_init(void);
void Modbus_COM2_task(void);
void ModbusTimer_COM2(void);
void COM2_IRQHandler(void);


#endif
    //------------end fo file----------------------------------
