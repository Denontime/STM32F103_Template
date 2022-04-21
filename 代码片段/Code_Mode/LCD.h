/**
  ******************************************************************************
  * @file    LCD.h
  * @brief   This file is used for CRC verification
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_H__
#define __LCD_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

uint8_t Background_change(uint8_t index);
uint8_t Variable_change(uint8_t address, uint8_t value);

/* USER CODE END Private defines */



/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __CRC_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
