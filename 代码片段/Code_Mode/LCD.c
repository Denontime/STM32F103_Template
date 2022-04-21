/**
  ******************************************************************************
  * @file    LCD.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "LCD.h"

/* USER CODE BEGIN 0 */
uint8_t head[2] = {0xA5, 0x5A};
uint8_t Background[] = {0x06, 0x80, 0x03, 0x00, 0x00};
uint8_t variable[] = {0x07, 0x82, 0x00, 0x10, 0x00, 0x00};
/* USER CODE END 0 */


/* USART1 init function */

/**
* @brief 发送传入消息缓存数组
  * @retval None
  */
uint8_t Background_change(uint8_t index)
{
  Background[4] = index;
  SendHead();
  for(int i = 0; i < Length(Background); i++)
  {
      printf("%c", Background[i]);
  }
  CRC16(&Background[1],Length(Background)-1);
  SendCRC();

  return OK;
}

uint8_t Variable_change(uint8_t address, uint8_t value)
{
  variable[3] = address;
  variable[5] = value;
  SendHead();
  for(int i = 0; i < Length(variable); i++)
  {
      printf("%c", variable[i]);
  }
  CRC16(&variable[1],Length(variable)-1);
  SendCRC();

  return OK;
}


/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
