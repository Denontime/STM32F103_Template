/**
******************************************************************************
* @file    global.h
* @author  
* @version V1.0
* @brief   
******************************************************************************
**/
#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "stm32f1xx_hal.h"

extern uint8_t uchCRCHi; //CRC ¸ß×Ö½Ú
extern uint8_t uchCRCLo; //CRC µÍ×Ö½Ú

#define OK 1

#define SendHead(); printf("%c",0xA5);printf("%c",0x5A);
#define SendCRC(); printf("%c",uchCRCHi);printf("%c",uchCRCLo);
#define Length(x) sizeof(x)/sizeof(uint8_t)

#endif 

