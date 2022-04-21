#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>

#define FLASH_ONE_K       128
#define USART_REC_LEN  			128 //�����������ֽ��� 1.5K
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

extern volatile _Bool onek_complete;
extern uint8_t page_k;
	  	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8  USART_RX_BUF1[USART_REC_LEN];
extern u16 USART_RX_STA;         		//����״̬���	
extern u16 USART_RX_CNT;				//���յ��ֽ���
extern u16 USART_RX_CNTF;    //�ñ�����һ���ֽ���
extern u16 USART_RX_ALL_CNT;

void USART1_Config(void);
int fputc(int ch, FILE *f);
void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...);

#endif /* __USART1_H */
