 /**
  ******************************************************************************
  * @file    bsp_xxx.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   spi flash 底层应用函数bsp 
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 ISO-MINI STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "drv_spi.h"

/* Private typedef -----------------------------------------------------------*/

_as5048_data AS5048_Temp;


void Angle_Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
/*******************************************************************************
* Function Name  : SPI_FLASH_Init
* Description    : Initializes the peripherals used by the SPI FLASH driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable SPI1 and GPIO clocks */
  /*!< SPI_FLASH_SPI_CS_GPIO, SPI_FLASH_SPI_MOSI_GPIO, 
       SPI_FLASH_SPI_MISO_GPIO, SPI_FLASH_SPI_DETECT_GPIO 
       and SPI_FLASH_SPI_SCK_GPIO Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD, ENABLE);

  /*!< SPI_FLASH_SPI Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
 
  
  /*!< Configure SPI_FLASH_SPI pins: SCK */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*!< Configure SPI_FLASH_SPI pins: MISO */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*!< Configure SPI_FLASH_SPI pins: MOSI */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*!< Configure SPI_FLASH_SPI_CS_PIN pin: SPI_FLASH Card CS pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  /* SPI1 configuration */
  // W25X16: data input on the DIO pin is sampled on the rising edge of the CLK. 
  // Data on the DO and DIO pins are clocked out on the falling edge of CLK.
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 15;
  SPI_Init(SPI1, &SPI_InitStructure);

  /* Enable SPI1  */
  SPI_Cmd(SPI1, ENABLE);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_ReadByte
* Description    : Reads a byte from the SPI Flash.
*                  This function must be used only if the Start_Read_Sequence
*                  function has been previously called.
* Input          : None
* Output         : None
* Return         : Byte Read from the SPI Flash.
*******************************************************************************/
uint16_t SPI_FLASH_ReadByte(void)
{
  return (SPI_FLASH_SendByte(CMD_ANGLE));
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SendByte
* Description    : Sends a byte through the SPI interface and return the byte
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
uint16_t SPI_FLASH_SendByte(uint16_t byte)
{
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(SPI1, byte);

  /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI1);
}

// as5048读取接收函数
uint16_t SPI1_Read5048Data(uint16_t TxData)
{
  uint16_t data;
	SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(CMD_ANGLE);
	SPI_FLASH_CS_HIGH();
	Angle_Delay(100);
	SPI_FLASH_CS_LOW();
	data=SPI_FLASH_SendByte(CMD_NOP);
	SPI_FLASH_CS_HIGH();
  data = data & 0x3fff;	//屏蔽高两位
  return data;

}

_as5048_data CollectData(void)
{
    uint16_t anglereg = 0, magreg = 0, agcreg = 0;
    uint16_t mag = 0, value = 0;
    double angle = 0.0;
    uint8_t agc = 0;
    _as5048_data Temp = {1,0,0,0,0.0};

//    SPI1_Read5048Data(CMD_ANGLE);SPI1_Read5048Data(CMD_ANGLE);
    anglereg = SPI1_Read5048Data(CMD_ANGLE); value = anglereg & 0x3fff;
    magreg = SPI1_Read5048Data(CMD_AGC);   mag = magreg & 0x3fff;
    agcreg = SPI1_Read5048Data(CMD_NOP);   agc = (uint8_t)agcreg & 0x00ff;
    angle = (value * 360.0)/16384.0;
    
    if ((anglereg & 0x4000) | (magreg & 0x4000) | (agcreg & 0x4000))
    {
        ClearAndNop();
        //rt_kprintf("There is and error!\n");
        Temp.iserror = 1;
    }
    else
    {
        Temp.iserror = 0;
        Temp.angle = angle;
        Temp.mag = mag;
        Temp.agc = agc;
        Temp.value = value;
    }
    return Temp;
}

// as5048清除错误标志位
u16 ClearAndNop(void)
{

    SPI1_Read5048Data(0x4001);              // 附加偶校验的错误标志位清除命令
    Angle_Delay(10000);              // 两次命令之间有350ns的间隔，源自官方datasheet
    SPI1_Read5048Data(0xc000);               // 附加偶校验的错误标志位清除命令
}

/*******************************************************************************
* Function Name  : SPI_FLASH_WriteEnable
* Description    : Enables the write access to the FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_WriteEnable(void)
{
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Write Enable" instruction */
  //SPI_FLASH_SendByte(W25X_WriteEnable);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
}

   
/*********************************************END OF FILE**********************/
