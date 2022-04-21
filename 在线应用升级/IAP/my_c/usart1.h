#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>

#define FLASH_ONE_K       128
#define USART_REC_LEN  			128 //定义最大接收字节数 1.5K
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收

extern volatile _Bool onek_complete;
extern uint8_t page_k;
	  	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8  USART_RX_BUF1[USART_REC_LEN];
extern u16 USART_RX_STA;         		//接收状态标记	
extern u16 USART_RX_CNT;				//接收的字节数
extern u16 USART_RX_CNTF;    //用保存上一个字节数
extern u16 USART_RX_ALL_CNT;

void USART1_Config(void);
int fputc(int ch, FILE *f);
void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...);

#endif /* __USART1_H */
