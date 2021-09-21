#ifndef _LED_H
#define _LED_H

#include "stm32f10x.h"

#define LED_PORT GPIOB

#define LED_R_PIN GPIO_Pin_5
#define LED_G_PIN GPIO_Pin_0
#define LED_B_PIN GPIO_Pin_1

#define LED_CLK RCC_APB2Periph_GPIOB

#define LED_RED 		1
#define LED_GREEN 	    2
#define LED_BLUE 		3
#define LED_YELLOW 	    4
#define LED_PURPLE 	    5
#define LED_CYAN 		6
#define LED_WHITE 	    7
#define LED_OFF 		8

#define IS_LED_ALL_PERIPH(PERIPH) (((PERIPH) == LED_RED)        || \
                                    ((PERIPH) == LED_GREEN)     || \
                                    ((PERIPH) == LED_BLUE)      || \
                                    ((PERIPH) == LED_YELLOW)    || \
                                    ((PERIPH) == LED_PURPLE)    || \
                                    ((PERIPH) == LED_CYAN)      || \
                                    ((PERIPH) == LED_WHITE))

extern int led_state;
extern int led_delay_ms;

void LED_Set_Color(int c);
void LED_GPIO_Confg(void);
void LED_Next_State(void);
void LED_Prev_State(void);
void LED_Blink(void);

#endif
