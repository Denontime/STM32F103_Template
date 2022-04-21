/*----------------------------------------------------------------------------------------------------------------------/
*              IIC.C 
----------------------------------------------------------------------------------------------------------------------*/
#include "IIC.h"

void IIC_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_6 | GPIO_Pin_7 ;
  GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
//  delay_ms(10);
//  GPIO_SetBits(GPIOB,GPIO_Pin_6 | GPIO_Pin_7);
}

void SDA_OUT(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7 ;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void SDA_IN(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

static	void	I2C_delay(void){volatile int i=1;while(i--);}
void	I2C_Start(void) 
{ 
	SDA_OUT();     //sda线输出
	SDA_H;	  	  
	SCL_H;
	I2C_delay();
 	SDA_L;      //START:when CLK is high,DATA change form high to low 
	I2C_delay();
	SCL_L;      //钳住I2C总线，准备发送或接收数据 
	
} 
void	I2C_Stop(void) 
{ 
	SDA_OUT();//sda线输出
	SCL_L;
	SDA_L;//STOP:when CLK is high DATA change form low to high
 	I2C_delay();
	SCL_H; 
	SDA_H;//发送I2C总线结束信号
	I2C_delay();	
} 
#if	1//weifang
void	I2C_Ack(void) 
{ 
	SCL_L;
	SDA_OUT();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
} 
#endif
void I2C_NoAck(void) 
{ 
	SCL_L;
	SDA_OUT();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
} 
uint8_t	I2C_WaitAck(void)   //返回为:=1有ACK,=0无ACK 
{ 
	uint8_t ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	SDA_H;
    I2C_delay();	   
	SCL_H;
    I2C_delay();	 
	while(SDA_read)
	{
		ucErrTime++;
		if(ucErrTime>50)
		{
			I2C_Stop();
			return 0;
		}
        I2C_delay();
	}
	SCL_L;//时钟输出0 	   
	return 1;
} 
void	I2C_SendByte(uint8_t SendByte) //数据从高位到低位// 
{ 
	 uint8_t t;   
	SDA_OUT(); 	    
    SCL_L;//拉低时钟开始数据传输
	I2C_delay();
	for(t=0;t<8;t++)
	{              
		if(SendByte&0x80) 
			SDA_H;
		else
			SDA_L;
		SendByte<<=1; 	  
	I2C_delay();  
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();       
	}
} 
uint8_t	I2C_ReceiveByte(void)  //数据从高位到低位// 
{  
	uint8_t i=0,receive=0;
    
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        SCL_L; 
        I2C_delay();
		SCL_H;
        receive<<=1;
        if(SDA_read) receive++;   
		I2C_delay();
    }					 
    return receive;
} 
uint8_t	I2C_Write(uint8_t sla_address, uint8_t sub_address, uint8_t data_w)
{ 
//	if (!I2C_Start()) return 0; 
	I2C_Start();
	I2C_SendByte(sla_address<<1);  
	if (!I2C_WaitAck()) 
	{ 
		I2C_Stop();  
		return 0; 
	} 
	I2C_SendByte(sub_address);         
	I2C_WaitAck(); 

	I2C_SendByte(data_w); 
	I2C_WaitAck(); 
	I2C_Stop(); 

	return 1; 
} 


         
uint8_t	I2C_Read(uint8_t sla_address, uint8_t sub_address)
{ 
	uint8_t data_r=0;

//	if (!I2C_Start()) return 0; 
	I2C_Start();
	I2C_SendByte(sla_address<<1);  
	if (!I2C_WaitAck())  
	{ 
		I2C_Stop();  
		return 0; 
	} 

	I2C_SendByte(sub_address);      
	I2C_WaitAck(); 
	I2C_Start(); 
	I2C_SendByte((sla_address<<1)+1); 
	I2C_WaitAck(); 
	data_r = I2C_ReceiveByte(); 
	I2C_NoAck(); 
	I2C_Stop(); 
	return data_r; 
}

uint8_t i2c_len_Read(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    I2C_Start();
    I2C_SendByte(addr << 1 | I2C_Direction_Transmitter);
    if (!I2C_WaitAck()) {
        I2C_Stop();
        return 0;
    }
    I2C_SendByte(reg);
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte((addr << 1)+1);
    I2C_WaitAck();
    while (len) {
        *buf = I2C_ReceiveByte();
        if (len == 1)
            I2C_NoAck();
        else
            I2C_Ack();
        buf++;
        len--;
    }
    I2C_Stop();
    return 1;
}


