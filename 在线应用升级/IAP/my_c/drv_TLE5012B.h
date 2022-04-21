
#ifndef _TLE5012B
#define _TLE5012B

#include "stm32f10x.h"

extern uint16_t TLE5012B_Angle;

void TLE5012B_Init(void);
void TLE5012B_read_data(void);


#endif
