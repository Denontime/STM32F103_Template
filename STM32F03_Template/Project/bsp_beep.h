#ifndef __BEEP_H
#define	__BEEP_H


#include "stm32f10x.h"

#define BEEP_PORT GPIOA

#define BEEP_PIN GPIO_Pin_8

#define BEEP_CLK RCC_APB2Periph_GPIOA

void BEEP_GPIO_Confg(void);
void beep_on(void);
void beep_off(void);



#endif
