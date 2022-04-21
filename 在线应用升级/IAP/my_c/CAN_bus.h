#ifndef __CAN_BUS
#define	__CAN_BUS

#include "stm32f10x.h"

#define Pitch_ExtId 0xaaa1
#define Roll_ExtId  0xaaa2
#define Yaw_ExtId   0xaaa3

typedef struct
{
	uint16_t device_ID;
	uint32_t Motor_Pos;
	uint8_t  RockData;
	uint8_t  Dir;
	uint8_t  Power;
}_CAN_DATA;

extern  CanTxMsg TxMessage;
extern 	CanRxMsg RxMessage;

extern _CAN_DATA Pitch_CAN_DATA;
extern _CAN_DATA Roll_CAN_DATA;
extern _CAN_DATA Yaw_CAN_DATA;

extern __IO uint32_t flag;


static void CAN_BUS_GPIO_Config(void);
static void CAN_BUS_NVIC_Config(void);
static void CAN_BUS_Mode_Config(void);
static void CAN_BUS_Filter_Config(void);
void CAN_BUS_Config(void);

void CAN_BusSetMsg(_CAN_DATA* CAN_DATA);
void CAN_BUS_Send(void);


#endif /* __CAN_BUS */









