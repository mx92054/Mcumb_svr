
#ifndef __MODBUS_COM__
#define __MODBUS_COM__

#include "stm32f10x.h"
#include "Mbsvr_comm.h"

#define USE_USART1

//-------------------����1�궨��------------------------------------
#ifdef USE_USART1
// ����1-USART1
#define MODBUS_USARTx USART1
#define MODBUS_USART_CLK RCC_APB2Periph_USART1
#define MODBUS_USART_APBxClkCmd RCC_APB2PeriphClockCmd
#define MODBUS_USART_BAUDRATE 115200

// USART GPIO ���ź궨��
#define MODBUS_USART_GPIO_CLK (RCC_APB2Periph_GPIOA)
#define MODBUS_USART_GPIO_APBxClkCmd RCC_APB2PeriphClockCmd

#define MODBUS_USART_TX_GPIO_PORT GPIOA
#define MODBUS_USART_TX_GPIO_PIN GPIO_Pin_9
#define MODBUS_USART_RX_GPIO_PORT GPIOA
#define MODBUS_USART_RX_GPIO_PIN GPIO_Pin_10

#define MODBUS_USART_IRQ USART1_IRQn
#define MODBUS_USART_IRQHandler USART1_IRQHandler
#endif

//-------------------����2�궨��------------------------------------
#ifdef USE_USART2
// ����1-USART1
#define MODBUS_USARTx USART2
#define MODBUS_USART_CLK RCC_APB1Periph_USART2
#define MODBUS_USART_APBxClkCmd RCC_APB1PeriphClockCmd
#define MODBUS_USART_BAUDRATE 115200

// USART GPIO ���ź궨��
#define MODBUS_USART_GPIO_CLK (RCC_APB2Periph_GPIOA)
#define MODBUS_USART_GPIO_APBxClkCmd RCC_APB2PeriphClockCmd

#define MODBUS_USART_TX_GPIO_PORT GPIOA
#define MODBUS_USART_TX_GPIO_PIN GPIO_Pin_2
#define MODBUS_USART_RX_GPIO_PORT GPIOA
#define MODBUS_USART_RX_GPIO_PIN GPIO_Pin_3

#define MODBUS_USART_IRQ USART2_IRQn
#define MODBUS_USART_IRQHandler USART2_IRQHandler
#endif

extern Modbus_block mblock1;
//------------------Function Define ----------------------------------
void Modbus_init(void);
void Modbus_task(void);
void ModbusTimer(void);
void MODBUS_USART_IRQHandler(void);

#endif
//------------end fo file----------------------------------
