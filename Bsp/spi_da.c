/**********************************************************************************
**********************************************************************************/
#include "spi_da.h"

//--------------INPUT SHIFT REGISTER------------------------------------------
#define AD5761_UPDATE_DAC 0x02
#define AD5761_SETTING 0x04
#define AD5761_LOAD_VAL 0x03

//--------------INPUT SHIFT REGISTER------------------------------------------
#define AD5761_CONTROL_REG1 0x04
#define AD5761_CONTROL_REG2 0x00
#define AD5761_CONTROL_REG3 0x02

/*******************************************************************************
* Function Name  : SPI1_Init
* Description    : Initializes the peripherals used by the SPI FLASH driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI1_Init(void)
{
  SPI_InitTypeDef SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable SPI1 and GPIO clocks */
  RCC_APB2PeriphClockCmd(SPI1_GPIO_CLK, ENABLE);

  /*!< SPI_DA_SPI Periph clock enable */
  RCC_APB2PeriphClockCmd(SPI1_CLK, ENABLE);

  /*!< Configure SPI_DA_SPI pins: SCK MISO MOSI */
  GPIO_InitStructure.GPIO_Pin = SPI1_SCK_PIN | SPI1_MISO_PIN | SPI1_MOSI_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(SPI1_SCK_PORT, &GPIO_InitStructure);

  /*!< Configure SPI_DA_SPI_CS_PIN pin: SPI_FLASH Card CS pin */
  GPIO_InitStructure.GPIO_Pin = SPI1_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(SPI1_CS_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = SPI1_LADC_PIN;
  GPIO_Init(SPI1_LADC_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = SPI1_CLR_PIN;
  GPIO_Init(SPI1_CLR_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = SPI1_RST_PIN;
  GPIO_Init(SPI1_RST_PORT, &GPIO_InitStructure);
  /* Deselect the FLASH: Chip Select high */
  SPI1_DA_CS_HIGH();

  /* SPI1 configuration */
  // W25X16: data input on the DIO pin is sampled on the rising edge of the CLK.
  // Data on the DO and DIO pins are clocked out on the falling edge of CLK.
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1_DA, &SPI_InitStructure);

  /* Enable SPI1  */
  SPI_Cmd(SPI1_DA, ENABLE);
}

/*******************************************************************************
* Function Name  : SPI1_Init
* Description    : Initializes the peripherals used by the SPI FLASH driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI2_Init(void)
{
  SPI_InitTypeDef SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable SPI2 and GPIO clocks */
  RCC_APB2PeriphClockCmd(SPI2_GPIO_CLK, ENABLE);

  /*!< SPI_DA_SPI Periph clock enable */
  RCC_APB1PeriphClockCmd(SPI2_CLK, ENABLE);

  /*!< Configure SPI_DA_SPI pins: SCK MISO MOSI */
  GPIO_InitStructure.GPIO_Pin = SPI2_SCK_PIN | SPI2_MISO_PIN | SPI2_MOSI_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(SPI2_SCK_PORT, &GPIO_InitStructure);

  /*!< Configure SPI_DA_SPI_CS_PIN pin: SPI_FLASH Card CS pin */
  GPIO_InitStructure.GPIO_Pin = SPI2_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(SPI2_CS_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = SPI2_LADC_PIN;
  GPIO_Init(SPI2_LADC_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = SPI2_CLR_PIN;
  GPIO_Init(SPI2_CLR_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = SPI2_RST_PIN;
  GPIO_Init(SPI2_RST_PORT, &GPIO_InitStructure);
  /* Deselect the FLASH: Chip Select high */
  SPI2_DA_CS_HIGH();

  /* SPI2 configuration */
  // W25X16: data input on the DIO pin is sampled on the rising edge of the CLK.
  // Data on the DO and DIO pins are clocked out on the falling edge of CLK.
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI2_DA, &SPI_InitStructure);

  /* Enable SPI2  */
  SPI_Cmd(SPI2_DA, ENABLE);
}

//-------------------------------------------------------------------
void delayus(u16 i)
{
  u32 t = 8 * i;
  while (t--)
    ;
}

/*******************************************************************************
* Function Name  : SPI_DA_Value
* Description    : Erases the specified FLASH sector.
* Input          : SectorAddr: address of the sector to erase.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI1_AD5761_Init(void)
{
  SPI1_Init();

  SPI1_DA_CS_HIGH();
  SPI1_DA_RST_LOW();
  SPI1_DA_CLR_HIGH();

  delayus(20);
  SPI1_DA_RST_HIGH();

  delayus(20);
  SPI1_DA_CS_LOW();

  SPI_Write_Read(SPI1, AD5761_SETTING, 0x0002);

  delayus(10);
  SPI1_DA_CS_HIGH();
  SPI1_DA_LADC_LOW();
}

//-------------------------------------------------------------------
void SPI1_AD5761_Value(u16 val)
{
  u16 direction;
  u16 voltage;

  direction = val >> 12;
  voltage = val << 4;
  if (direction == 0)
    voltage = 0x8000 - voltage;
  else
    voltage = 0x8000 + voltage;

  SPI1_DA_CS_LOW();

  SPI_Write_Read(SPI1, AD5761_LOAD_VAL, voltage);

  delayus(1);
  SPI1_DA_CS_HIGH();
}

/*******************************************************************************
* Function Name  : SPI_DA_Value
* Description    : Erases the specified FLASH sector.
* Input          : SectorAddr: address of the sector to erase.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI2_AD5761_Init(void)
{
  SPI2_Init();

  SPI2_DA_CS_HIGH();
  SPI2_DA_RST_LOW();
  SPI2_DA_CLR_HIGH();

  delayus(20);
  SPI2_DA_RST_HIGH();

  delayus(20);
  SPI2_DA_CS_LOW();

  SPI_Write_Read(SPI2, AD5761_SETTING, 0x0002);
  delayus(10);

  SPI2_DA_CS_HIGH();
  SPI2_DA_LADC_LOW();
}

//-------------------------------------------------------------------
void SPI2_AD5761_Value(u16 val)
{
  u16 direction;
  u16 voltage;

  direction = val >> 12;
  voltage = val << 4;
  if (direction == 0)
    voltage = 0x8000 - voltage;
  else
    voltage = 0x8000 + voltage;

  SPI2_DA_CS_LOW();
  SPI_Write_Read(SPI2, AD5761_LOAD_VAL, voltage);

  delayus(1);
  SPI2_DA_CS_HIGH();
}

//----------------------------------------------------------------
u16 SPI_Write_Read(SPI_TypeDef *SPIx, u8 uCmd, u16 uData)
{
  u16 j;
  u16 data[3];

  j = 0;
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET && (j++ < 36000))
    ;
  SPI_I2S_SendData(SPIx, uCmd);
  j = 0;
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET && (j++ < 36000))
    ;
  SPI_I2S_SendData(SPIx, (uData >> 8));
  j = 0;
  while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET && (j++ < 36000))
    ;
  SPI_I2S_SendData(SPIx, (uData & 0x00FF));

  return (u16)(data[1] << 8 | data[2]);
}

/******************************END OF FILE*****************************/
