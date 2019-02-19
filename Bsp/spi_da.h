#ifndef __SPI_DA_H
#define __SPI_DA_H

#include "stm32f10x.h"

//-------------------SPI1 Define-----------------------------------------------
#define SPI1_DA SPI1
#define SPI1_CLK RCC_APB2Periph_SPI1
#define SPI1_GPIO_CLK (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC)
#define SPI1_SCK_PIN GPIO_Pin_5
#define SPI1_SCK_PORT GPIOA
#define SPI1_MISO_PIN GPIO_Pin_6
#define SPI1_MISO_PORT GPIOA
#define SPI1_MOSI_PIN GPIO_Pin_7
#define SPI1_MOSI_PORT GPIOA
#define SPI1_CS_PIN GPIO_Pin_4
#define SPI1_CS_PORT GPIOA

#define SPI1_LADC_PIN GPIO_Pin_4
#define SPI1_LADC_PORT GPIOC
#define SPI1_CLR_PIN GPIO_Pin_5
#define SPI1_CLR_PORT GPIOC
#define SPI1_RST_PIN GPIO_Pin_0
#define SPI1_RST_PORT GPIOB

#define SPI1_DA_CS_LOW() GPIO_ResetBits(SPI1_CS_PORT, SPI1_CS_PIN)
#define SPI1_DA_CS_HIGH() GPIO_SetBits(SPI1_CS_PORT, SPI1_CS_PIN)

#define SPI1_DA_LADC_LOW() GPIO_ResetBits(SPI1_LADC_PORT, SPI1_LADC_PIN)
#define SPI1_DA_LADC_HIGH() GPIO_SetBits(SPI1_LADC_PORT, SPI1_LADC_PIN)

#define SPI1_DA_CLR_LOW() GPIO_ResetBits(SPI1_CLR_PORT, SPI1_CLR_PIN)
#define SPI1_DA_CLR_HIGH() GPIO_SetBits(SPI1_CLR_PORT, SPI1_CLR_PIN)

#define SPI1_DA_RST_LOW() GPIO_ResetBits(SPI1_RST_PORT, SPI1_RST_PIN)
#define SPI1_DA_RST_HIGH() GPIO_SetBits(SPI1_RST_PORT, SPI1_RST_PIN)

//-------------------SPI2 Define-----------------------------------------------
#define SPI2_DA SPI2
#define SPI2_CLK RCC_APB1Periph_SPI2
#define SPI2_GPIO_CLK (RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD)

#define SPI2_SCK_PIN GPIO_Pin_13
#define SPI2_SCK_PORT GPIOB
#define SPI2_MISO_PIN GPIO_Pin_14
#define SPI2_MISO_PORT GPIOB
#define SPI2_MOSI_PIN GPIO_Pin_15
#define SPI2_MOSI_PORT GPIOB
#define SPI2_CS_PIN GPIO_Pin_12
#define SPI2_CS_PORT GPIOB

#define SPI2_LADC_PIN GPIO_Pin_8
#define SPI2_LADC_PORT GPIOD
#define SPI2_CLR_PIN GPIO_Pin_9
#define SPI2_CLR_PORT GPIOD
#define SPI2_RST_PIN GPIO_Pin_10
#define SPI2_RST_PORT GPIOD

#define SPI2_DA_CS_LOW() GPIO_ResetBits(SPI2_CS_PORT, SPI2_CS_PIN)
#define SPI2_DA_CS_HIGH() GPIO_SetBits(SPI2_CS_PORT, SPI2_CS_PIN)

#define SPI2_DA_LADC_LOW() GPIO_ResetBits(SPI2_LADC_PORT, SPI2_LADC_PIN)
#define SPI2_DA_LADC_HIGH() GPIO_SetBits(SPI2_LADC_PORT, SPI2_LADC_PIN)

#define SPI2_DA_CLR_LOW() GPIO_ResetBits(SPI2_CLR_PORT, SPI2_CLR_PIN)
#define SPI2_DA_CLR_HIGH() GPIO_SetBits(SPI2_CLR_PORT, SPI2_CLR_PIN)

#define SPI2_DA_RST_LOW() GPIO_ResetBits(SPI2_RST_PORT, SPI2_RST_PIN)
#define SPI2_DA_RST_HIGH() GPIO_SetBits(SPI2_RST_PORT, SPI2_RST_PIN)

//----------------------Function define--------------------------------------
void SPI1_Init(void);
void SPI2_Init(void);

//----------------------Function define--------------------------------------
void delayus(u16 i);
void SPI1_AD5761_Init(void);
void SPI1_AD5761_Value(u16 val);

void SPI2_AD5761_Init(void);
void SPI2_AD5761_Value(u16 val);

u16 SPI_Write_Read(SPI_TypeDef *SPIx, u8 uCmd, u16 uData);

#endif /* __SPI_DA_H */