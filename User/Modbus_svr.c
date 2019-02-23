#include <string.h>
#include "Modbus_svr.h"
#include "..\bsp\bsp_led.h"
#include "Mbsvr_comm.h"
#include "stdio.h"

Modbus_block mblock1;
Modbus_block mblock2 ;
//-------------------------------------------------------------------------------
//	@brief	串口初始化
//	@param	None
//	@retval	None
//-------------------------------------------------------------------------------
static void port_config_COM1(u32 baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	COM1_GPIO_APBxClkCmd(COM1_GPIO_CLK, ENABLE);
	COM1_APBxClkCmd(COM1_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = COM1_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(COM1_TX_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = COM1_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(COM1_RX_GPIO_PORT, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(COM1, &USART_InitStructure);

	ModbusSvr_NVIC_Configuration(COM1_IRQ);
	USART_ITConfig(COM1, USART_IT_RXNE, ENABLE);
	USART_Cmd(COM1, ENABLE);
}

//-------------------------------------------------------------------------------
//	@brief	协议栈初始化
//	@param	None
//	@retval	None
//-------------------------------------------------------------------------------
void Modbu_COM1_init(void)
{
	char msg[100];
	int baud;

	ModbusSvr_block_init(&mblock1);
	baud = mblock1.baudrate * 100;
	port_config_COM1(baud);

	sprintf(msg, "\r\nStation No: %d, Baudrate: %d", mblock1.station, baud);
	Usart_SendString(COM1, msg);
}

//-------------------------------------------------------------------------------
//	@brief	协议任务调度
//	@param	None
//	@retval	None
//-------------------------------------------------------------------------------
void Modbus_COM1_task(void)
{
	ModbusSvr_task(&mblock1, COM1);
}

//-------------------------------------------------------------------------------
//	@brief	modbus recieve counter
//	@param	None
//	@retval	None
//-------------------------------------------------------------------------------
void ModbusTimer_COM1(void)
{
	mblock1.nMBInterval++;
}

//-------------------------------------------------------------------------------
//	@brief	串口中断服务程序
//	@param	None
//	@retval	None
//-------------------------------------------------------------------------------
void COM1_IRQHandler(void)
{
	ModbusSvr_isr(&mblock1, COM1);
}

/********************************************************************************
 * 
 *	@brief	串口初始化
 *	@param	None
 *	@retval	None
 *------------------------------------------------------------------------------*/
static void port_config_COM2(u32 baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	COM2_GPIO_APBxClkCmd(COM2_GPIO_CLK, ENABLE);
	COM2_APBxClkCmd(COM2_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = COM2_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(COM2_TX_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = COM2_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(COM2_RX_GPIO_PORT, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(COM2, &USART_InitStructure);

	ModbusSvr_NVIC_Configuration(COM2_IRQ);
	USART_ITConfig(COM2, USART_IT_RXNE, ENABLE);
	USART_Cmd(COM2, ENABLE);
}

//-------------------------------------------------------------------------------
//	@brief	协议栈初始化
//	@param	None
//	@retval	None
//-------------------------------------------------------------------------------
void Modbu_COM2_init(void)
{
	char msg[100];
	int baud;

	ModbusSvr_block_init(&mblock2);
	baud = mblock1.baudrate * 100;
	port_config_COM2(baud);

	sprintf(msg, "\r\nStation No: %d, Baudrate: %d", mblock2.station, baud);
	Usart_SendString(COM2, msg);
}

//-------------------------------------------------------------------------------
//	@brief	协议任务调度
//	@param	None
//	@retval	None
//-------------------------------------------------------------------------------
void Modbus_COM2_task(void)
{
	ModbusSvr_task(&mblock2, COM2);
}

//-------------------------------------------------------------------------------
//	@brief	modbus recieve counter
//	@param	None
//	@retval	None
//-------------------------------------------------------------------------------
void ModbusTimer_COM2(void)
{
	mblock2.nMBInterval++;
}

//-------------------------------------------------------------------------------
//	@brief	串口中断服务程序
//	@param	None
//	@retval	None
//-------------------------------------------------------------------------------
void COM2_IRQHandler(void)
{
	ModbusSvr_isr(&mblock2, COM2);
}

