/************************************
 * 文件名  ：usart1.c
 * 描述    ：将printf函数重定向到USART1。这样就可以用printf函数将单片机的数据
 *           打印到PC上的超级终端或串口调试助手。         
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 硬件连接：------------------------
 *          | PA9  - USART1(Tx)      |
 *          | PA10 - USART1(Rx)      |
 *           ------------------------
 * 库版本  ：ST3.0.0  *

**********************************************************************************/

#include "usart1.h"
#include <stdarg.h>

u8 USART_RX_BUF[USART_REC_LEN] __attribute__ ((at(0X20001000)));
u8 USART_RX_BUF1[USART_REC_LEN] __attribute__ ((at(0X20001000)));
u16 USART_RX_ALL_CNT = 0;       	//接收总字节数
u16 USART_RX_CNT = 0;			//接收的字节数
u16 USART_RX_CNTF = 0;
uint8_t page_k = 0;

volatile _Bool onek_complete = 0;  //完成 1K 数据接收


void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = 115200;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 
	
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;

//	/* 使能 USART1 时钟*/
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE); 

//	/* USART1 使用IO端口配置 */    
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);    
//  
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
//  GPIO_Init(GPIOA, &GPIO_InitStructure);   //初始化GPIOA
//	  
//	/* USART1 工作模式配置 */
//	USART_InitStructure.USART_BaudRate = 115200;	//波特率设置：115200
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//数据位数设置：8位
//	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//停止位设置：1位
//	USART_InitStructure.USART_Parity = USART_Parity_No ;  //是否奇偶校验：无
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//接收与发送都使能
//	USART_Init(USART1, &USART_InitStructure);  //初始化USART1
//	USART_Cmd(USART1, ENABLE);// USART1使能
}

uint8_t test_receive = 0;
void USART1_IRQHandler(void)
{
	u8 res;	
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		res = USART_ReceiveData(USART1);
		USART_RX_ALL_CNT ++;
		if (USART_RX_ALL_CNT - 1 != 0)
		{
			if (((USART_RX_ALL_CNT - 1) % FLASH_ONE_K) == 0)
			{
				page_k ++;
				onek_complete = 1;
			}
		}
		
		if (USART_RX_ALL_CNT == 932)
		{
			test_receive = 1;
		}
		if(USART_RX_CNT < USART_REC_LEN)
		{
			if (page_k % 2 == 0)
			{
				USART_RX_BUF[USART_RX_CNT] = res;
			}
			else
			{
				USART_RX_BUF1[USART_RX_CNT] = res;
			}
			USART_RX_CNT ++;
		}
		else
		{
			if (page_k % 2 == 0)
			{
				USART_RX_BUF[0] = res;
			}
			else
			{
				USART_RX_BUF1[0] = res;
			}
			USART_RX_CNT = 1;
		}
//		if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
//		{
//			if (Open_Send_Flag)
//			{
//				uart1_sendstring(temp_data);
//				USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
//				Open_Send_Flag = 0;
//			}
//			else
//				USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
//	  }
 }
} 


 /* 描述  ：重定向c库函数printf到USART1*/ 
int fputc(int ch, FILE *f)
{
/* 将Printf内容发往串口 */
  USART_SendData(USART1, (unsigned char) ch);
  while (!(USART1->SR & USART_FLAG_TXE));
 
  return (ch);
}



