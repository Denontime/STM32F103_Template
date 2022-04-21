#include "bsp_beep.h"

void BEEP_GPIO_Confg(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(BEEP_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = BEEP_PIN;
    GPIO_Init(BEEP_PORT, &GPIO_InitStructure);

    GPIO_ResetBits(BEEP_PORT, BEEP_PIN);
}

void beep_on(void)
{
    GPIO_SetBits(BEEP_PORT, BEEP_PIN);
}

void beep_off(void)
{
    GPIO_ResetBits(BEEP_PORT, BEEP_PIN);
}
