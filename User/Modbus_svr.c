#include <string.h>
#include "Modbus_svr.h"
#include "..\bsp\bsp_led.h"
#include "Mbsvr_comm.h"
#include "stdio.h"

Modbus_block mblock1;
//-------------------------------------------------------------------------------
//	@brief	中断初始化
//	@param	None
//	@retval	None
//-------------------------------------------------------------------------------
static void MODBUS_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = MODBUS_USART_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//-------------------------------------------------------------------------------
//	@brief	串口初始化
//	@param	None
//	@retval	None
//-------------------------------------------------------------------------------
static void MODBUS_Config(u32 baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	MODBUS_USART_GPIO_APBxClkCmd(MODBUS_USART_GPIO_CLK, ENABLE);
	MODBUS_USART_APBxClkCmd(MODBUS_USART_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = MODBUS_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MODBUS_USART_TX_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = MODBUS_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(MODBUS_USART_RX_GPIO_PORT, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(MODBUS_USARTx, &USART_InitStructure);

	MODBUS_NVIC_Configuration();

	USART_ITConfig(MODBUS_USARTx, USART_IT_RXNE, ENABLE);

	USART_Cmd(MODBUS_USARTx, ENABLE);
}

//-------------------------------------------------------------------------------
//	@brief	协议栈初始化
//	@param	None
//	@retval	None
//-------------------------------------------------------------------------------
void Modbus_init(void)
{
	char msg[100];
	int baud;

	ModbusSvr_block_init(&mblock1);

	MODBUS_NVIC_Configuration();
	baud = mblock1.baudrate * 100;
	MODBUS_Config(baud);

	sprintf(msg, "\r\nStation No: %d, Baudrate: %d", mblock1.station, baud);
	Usart_SendString(MODBUS_USARTx, msg);
}

//-------------------------------------------------------------------------------
//	@brief	协议任务调度
//	@param	None
//	@retval	None
//-------------------------------------------------------------------------------
void Modbus_task(void)
{
	ModbusSvr_task(&mblock1, MODBUS_USARTx);
}

//-------------------------------------------------------------------------------
//	@brief	modbus recieve counter
//	@param	None
//	@retval	None
//-------------------------------------------------------------------------------
void ModbusTimer(void)
{
	mblock1.nMBInterval++;
}

//-------------------------------------------------------------------------------
//	@brief	串口中断服务程序
//	@param	None
//	@retval	None
//-------------------------------------------------------------------------------
void MODBUS_USART_IRQHandler(void)
{
	u8 ch;

	if (USART_GetITStatus(MODBUS_USARTx, USART_IT_RXNE) != RESET) //判断读寄存器是否非空
	{
		ch = USART_ReceiveData(MODBUS_USARTx); //将读寄存器的数据缓存到接收缓冲区里
		ModbusSvr_isr(&mblock1, ch);
	}

	if (USART_GetITStatus(MODBUS_USARTx, USART_IT_TXE) != RESET)
	{
		USART_ITConfig(MODBUS_USARTx, USART_IT_TXE, DISABLE);
	}
}
