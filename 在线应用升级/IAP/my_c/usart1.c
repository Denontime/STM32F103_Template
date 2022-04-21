/************************************
 * �ļ���  ��usart1.c
 * ����    ����printf�����ض���USART1�������Ϳ�����printf��������Ƭ��������
 *           ��ӡ��PC�ϵĳ����ն˻򴮿ڵ������֡�         
 * ʵ��ƽ̨��MINI STM32������ ����STM32F103C8T6
 * Ӳ�����ӣ�------------------------
 *          | PA9  - USART1(Tx)      |
 *          | PA10 - USART1(Rx)      |
 *           ------------------------
 * ��汾  ��ST3.0.0  *

**********************************************************************************/

#include "usart1.h"
#include <stdarg.h>

u8 USART_RX_BUF[USART_REC_LEN] __attribute__ ((at(0X20001000)));
u8 USART_RX_BUF1[USART_REC_LEN] __attribute__ ((at(0X20001000)));
u16 USART_RX_ALL_CNT = 0;       	//�������ֽ���
u16 USART_RX_CNT = 0;			//���յ��ֽ���
u16 USART_RX_CNTF = 0;
uint8_t page_k = 0;

volatile _Bool onek_complete = 0;  //��� 1K ���ݽ���


void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = 115200;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 
	
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;

//	/* ʹ�� USART1 ʱ��*/
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE); 

//	/* USART1 ʹ��IO�˿����� */    
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);    
//  
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
//  GPIO_Init(GPIOA, &GPIO_InitStructure);   //��ʼ��GPIOA
//	  
//	/* USART1 ����ģʽ���� */
//	USART_InitStructure.USART_BaudRate = 115200;	//���������ã�115200
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//����λ�����ã�8λ
//	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//ֹͣλ���ã�1λ
//	USART_InitStructure.USART_Parity = USART_Parity_No ;  //�Ƿ���żУ�飺��
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�����뷢�Ͷ�ʹ��
//	USART_Init(USART1, &USART_InitStructure);  //��ʼ��USART1
//	USART_Cmd(USART1, ENABLE);// USART1ʹ��
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


 /* ����  ���ض���c�⺯��printf��USART1*/ 
int fputc(int ch, FILE *f)
{
/* ��Printf���ݷ������� */
  USART_SendData(USART1, (unsigned char) ch);
  while (!(USART1->SR & USART_FLAG_TXE));
 
  return (ch);
}



