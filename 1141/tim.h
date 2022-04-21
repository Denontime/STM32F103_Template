/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
//#include "main.h"
//#include "Light_Controller.h"
#include "Controller_WCS.h"
extern uint8_t Usart1_Receive_flag;
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern TIM_HandleTypeDef htim2;
extern unsigned short int Usart2DataReceivingLength;
extern unsigned short int Usart1DataReceivingLength;
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_TIM2_Init(void);
void ClearPicReceiveTimer(void);
/* USER CODE BEGIN Prototypes */

#define TimerFree 0
#define TimerStart 1 
#define TimerTimeOut 2

struct TimerSturct
{
	unsigned int Number;
	unsigned char Status;
};	
	
	
	
	
#define UartReceiveFinish 1
#define UartReceiveStart 2
#define UartReceiveFree 3
#define TimerOut 5

//轮询计数时间
#define PollingCountTime 300
//重发计数时间
#define RetransmissionCountTime 300
#define WrapCountTime 500

#define PictureReceivingTime 3000
//重发计数完成
#define RetransmissionComplete 1
#define RetransmissionNotCompleted 0
//单次轮询计数完成
#define SingleollountComplete 1
#define SingleollountNotComplete 0
//图片接收超时
#define PictureReceptionCountComplete 1
#define PictureReceptionNotCountComplete 0

extern uint8_t Usart1_Receive_flag;
extern unsigned char Uart1TimerCounter;
extern unsigned char Uart2TimerCounter;
extern unsigned char CallCountStatus;
extern unsigned short int SinglePollCountStatus;
extern unsigned short int WrapCount;
extern struct TimerSturct Polling,WCSHeart;
extern unsigned short int PictureReceptionCountStatus;
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
