/**
  ******************************************************************************
  * @�ļ���     �� bsp.c
  * @����       �� strongerHuang
  * @�汾       �� V1.0.0
  * @����       �� 2018��08��07��
  * @ժҪ       �� BSP�ײ�Դ�ļ�
  ******************************************************************************/
/*----------------------------------------------------------------------------
  ������־:
  2018-08-07 V1.0.0:��ʼ�汾
  ----------------------------------------------------------------------------*/
/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "bsp.h"


/************************************************
�������� �� RCC_Configuration
��    �� �� ʱ������
��    �� �� ��
�� �� ֵ �� ��
��    �� �� strongerHuang
*************************************************/
void RCC_Configuration(void)
{
  /* ʹ��APB2ʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB  | 
                         RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD  | 
                         RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF  | 
                         RCC_APB2Periph_GPIOG | RCC_APB2Periph_AFIO, ENABLE);

  /* ʹ��APB1ʱ�� */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* ʹ��AHBʱ�� */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}

/************************************************
�������� �� LED_Initializes
��    �� �� LED��ʼ��
��    �� �� ��
�� �� ֵ �� ��
��    �� �� strongerHuang
*************************************************/
void LED_Initializes(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* ʹ��ʱ�� */
  RCC_APB2PeriphClockCmd(LED_GPIO_CLK, ENABLE);

  /* �������� */
  GPIO_InitStructure.GPIO_Pin   = LED_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);

  LED_OFF();

#if 0
  /* MCOʱ����� */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  RCC_MCOConfig(RCC_MCO_SYSCLK);
#endif
}

/************************************************
�������� �� BSP_Initializes
��    �� �� �ײ��ʼ��
��    �� �� ��
�� �� ֵ �� ��
��    �� �� strongerHuang
*************************************************/
void BSP_Initializes(void)
{
  LED_Initializes();                             //LED��ʼ��
}


/**** Copyright (C)2017 strongerHuang. All Rights Reserved **** END OF FILE ****/
