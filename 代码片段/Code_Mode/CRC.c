/**
  ******************************************************************************
  * @file    CRC.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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

/* Includes ------------------------------------------------------------------*/
#include "CRC.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */


/* USART1 init function */

uint16_t CRC16(uint8_t *pMsg, uint16_t Len)
{ 
    uint8_t i;
    uchCRCHi = 0xFF;
    uchCRCLo = 0xFF;
    while (Len--)
    {
        i = uchCRCHi ^ (*pMsg++);
        uchCRCHi = uchCRCLo ^ auchCRCHi[i];
        uchCRCLo = auchCRCLo[i];
    }
    return OK;
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
