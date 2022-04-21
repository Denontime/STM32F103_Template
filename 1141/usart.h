/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "Controller_WCS.h"
//#include "main.h"


#define BUFFSIZE 100
#define USART1_MAX_RECV_LEN 1000    //最大接收长度
#define USART1_MAX_SEND_LEN 100    //最大发送长度
#define USART1_MAX_ReceiveSave_LEN	1000
////warning there need to be checked 
#define USART2_MAX_RECV_LEN 1000
#define USART2_MAX_ReceiveSave_LEN	1000

/* USER CODE BEGIN Includes */

//待拣货
#define PickFree 0
//拣货完成
#define PickFinished 1
//拣货缺货
#define PickOutStock 2 
//空闲状态	
#define ConfirmFree 0
//确认中
#define Confirming 1
//确认完成
#define Confirmed 2 	
//轮询最多产品数量
#define MaxNumberPolls 100

//图片长度信息还未上传
#define PictureReceiveFree 0
//图片预上传
#define PicturePreReceive 3
//图片开始上传
#define PictureReceiveStart 1
//图片上传完成
#define PictureReceived 2
//图片指令长度
#define PicMaxLength 525

struct CTL_Info
{
	unsigned char LightAddress[3];
	unsigned char ProductID[11];
	//产品数量
	unsigned short ProductNum;
	//产品名称
	unsigned char ProductName[80];
	unsigned char ProductNameLength;
	unsigned char ConfirmStatus;
};
struct CtrlToLight
{
	struct CTL_Info CtlInfo;
};

struct ConfirmedProductInfo
{
	//Light地址
	unsigned char LightAddress[3];
	//产品ID
	unsigned char ProductID[11];
};

struct ConfirmInfo
{
	//确认产品个数
	unsigned char ConfirmLightQuantity; 
	//结构体中产品信息
	struct ConfirmedProductInfo ConfirmedProduct[MaxNumberPolls];
};

struct product
{
	unsigned char lightAddress[3];
};

struct ConfirmationInformation
{
	unsigned char lightAddress[3];
	
};
struct PollingInfo
{
	unsigned char lightAddress[3];
	unsigned char ProductID[11];
	unsigned char ProductCompletionStatus[2];
};

struct PollingFailure
{
	unsigned char FaultInformation;
	unsigned char lightAddress[3];
};
struct Background
{
	unsigned char ProductID[11];
	unsigned char PictureLength[4];
	unsigned char lightAddress[3];
};
struct PictureContent
{
	//总数据包数量
	unsigned char TotalNumberPackets;
	//当前包数
	unsigned char CurrentNumberPackets;
	//图片内容
	unsigned char  ContentPicture[522];
	unsigned char lightAddress[3];
	unsigned short int PictureContentLength;
};
struct PictureDowning 
{
	unsigned char Picture[1024];
	unsigned char PictureReceiveStatus;
	unsigned short int  PictureReceiveNumber;
};

/* USER CODE END Includes */

extern unsigned char LightNumber;
extern unsigned char ConfirmedQuantity;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern uint16_t USART1_RX_STA;
//HAL库使用的串口接收缓冲
extern uint8_t  ARxBuffer[BUFFSIZE];
extern uint8_t  USART1_RX_BUF[USART1_MAX_RECV_LEN]; 				//接收缓冲,最大USART1_MAX_RECV_LEN个字节.
extern uint8_t  USART1_TX_BUF[USART1_MAX_SEND_LEN]; 				//发送缓冲,最大USART3_MAX_SEND_LEN字节
extern unsigned char  ReceiveLength;
extern uint8_t ReceiveFish;	



extern uint8_t  Usart2DataStorage[USART2_MAX_ReceiveSave_LEN]; 
extern uint8_t  Usart1DataStorage[USART1_MAX_ReceiveSave_LEN]; 


/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */
void UART1_Send(uint8_t *Buffer, uint32_t Length);
void UART2_Send(uint8_t *Buffer, uint32_t Length);
void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void InitPicDownloadPara(void);

extern struct PollingFailure PollingStoppage[15];
extern struct PollingInfo PollingInformation[MaxNumberPolls];
extern struct PictureDowning PicDownload; 

extern unsigned char Usart2_Receive_flag;
extern unsigned short int Uart2ReceiveLength;
extern unsigned char Uart2RxBuffer[BUFFSIZE];
extern unsigned char USART2_RX_BUF[USART2_MAX_RECV_LEN];
extern unsigned char LightAddress[3];
extern struct ConfirmInfo ConfirmInfomation ;
extern unsigned char WCSSendingAddress[3];
extern unsigned char LightUploadAddress[3];
extern unsigned char EnterDownloadInterfaceSaveAddress[3];
extern unsigned char QuitDownloadInterfaceSaveAddress[3];



/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
