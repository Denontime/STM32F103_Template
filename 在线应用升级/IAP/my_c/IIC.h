
#ifndef _IIC_H_
#define _IIC_H_

#include "stm32f10x.h"

#define SCL_H         GPIOB->BSRR = GPIO_Pin_6
#define SCL_L         GPIOB->BRR  = GPIO_Pin_6  
#define SDA_H         GPIOB->BSRR = GPIO_Pin_7 
#define SDA_L         GPIOB->BRR  = GPIO_Pin_7 
#define SCL_read      GPIOB->IDR  & GPIO_Pin_6 
#define SDA_read      GPIOB->IDR  & GPIO_Pin_7 

//#define SDA_IN()        {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=0<<7*2;}	//PE0输入模式
//#define SDA_OUT()       {GPIOB->MODER&=~(3<<(7*2));GPIOB->MODER|=1<<7*2;}   //PE0输出模式

void	I2C_Start(void);
void	I2C_Stop(void);
void	I2C_Ack(void);
void I2C_NoAck(void);
uint8_t	I2C_WaitAck(void);
void	I2C_SendByte(uint8_t SendByte);
uint8_t	I2C_ReceiveByte(void);
uint8_t	I2C_Write(uint8_t sla_address, uint8_t sub_address, uint8_t data_w);
uint8_t i2c_len_Read(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);
uint8_t	I2C_Read(uint8_t sla_address, uint8_t sub_address);

void SDA_OUT(void);
void SDA_IN(void);
void IIC_GPIO_Config(void);

#endif
