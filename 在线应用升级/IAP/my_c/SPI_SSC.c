#include "SPI_SSC.h"

void SPI_ssc_GpioInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
 
  /*!< Configure SPI_FLASH_SPI pins: SCK */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*!< Configure SPI_FLASH_SPI pins: MOSI */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*!< Configure SPI_FLASH_SPI_CS_PIN pin: SPI_FLASH Card CS pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Deselect the FLASH: Chip Select high */
  CSN_HIGH;
}

void DATA_OUT(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7 ;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void DATA_IN(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//static void SSC_delay(uint8_t i)
//{
//	while(i--);
//}

uint16_t SPI_Write(uint16_t R_REG)
{
	unsigned char  i;
	for(i = 0; i < 16; i++)
	{
		SCK_LOW;
		if(R_REG & 0x8000)
			MOSI_HIGH;

		else
			MOSI_LOW;
		
		R_REG = R_REG << 1;
		SCK_HIGH;
//		if( MISO_STATUS )
//			R_REG = R_REG | 0x01;
	}
	SCK_LOW;
	return R_REG;
}

uint16_t SPI_Read(uint16_t R_REG)
{
	unsigned char  i;
	for(i = 0; i < 16; i++)
	{
		SCK_HIGH;
		R_REG = R_REG << 1;
		if( MISO_STATUS )
			R_REG = R_REG | 0x0001;
		SCK_LOW;
	}
	SCK_HIGH;
	return R_REG;
}
//----------------------------------------------------------------------------------------------------
//rfÐ´¼Ä´æÆ÷
//----------------------------------------------------------------------------------------------------
uint16_t hw2000_write_reg(uint16_t addr, uint16_t data)
{
	DATA_OUT();
//	SSC_delay(100);
	
	addr |= 0x80;
	
	CSN_LOW;//Ñ¡Í¨Æ÷¼þ
	SPI_Write(addr);//Ð´¼Ä´æÆ÷µØÖ·
	data = SPI_Write(data);//Ð´¸ß°ËÎ»Êý¾Ý
	CSN_HIGH;//½ûÖ¹Æ÷¼þ
	return data;
}	

uint16_t hw2000_read_reg(uint16_t reg)
{
	uint16_t tmp;
  DATA_OUT();
//	SSC_delay(100);
	
	CSN_LOW;
	SPI_Write(reg);
	
	DATA_IN();
//	SSC_delay(100);
	
	tmp = SPI_Read(0);
	CSN_HIGH;

	return tmp;
}

void hw2000_read_fifo(uint16_t addr, uint16_t *data, unsigned char length)
{
	unsigned char byte_ctr;
	DATA_OUT();
	
	CSN_LOW;                    		                                // Set CSN low, init SPI tranaction
	SPI_Write(addr);        		                                                // Select register to write to and read status byte
	DATA_IN();
  //data[0]=SPI_RW(0); 
	for(byte_ctr=0;byte_ctr<length;byte_ctr++)
	{
	   data[byte_ctr] = SPI_Read(0);                                                 // Perform SPI_RW to read byte from XN24L01
	}
	CSN_HIGH;                                                                   // Set CSN high again   
}

/******************************************************************************
 * @brief    hw2000_write_fifo
 *           
 * @note
 *
 * @param	 addr reg address
			 data
			 length
 * @retval	 None
 *
 * @version  1.0
 * @date     2017-11-02
 * @author   ZFM
 ******************************************************************************/
void hw2000_write_fifo(uint16_t addr, uint16_t *data, uint8_t length)
{
	unsigned char j;
	DATA_OUT();
	
	addr |= 0x80;
	CSN_LOW;//Ñ¡Í¨Ð¾Æ¬
	j = 0;
	SPI_Write(addr);//Ð´¼Ä´æÆ÷µØÖ·
	for(j = 0;j < length; j++)
		SPI_Write(data[j]);//Ð´Êý¾Ý
	j = 0;
	CSN_HIGH;
}


