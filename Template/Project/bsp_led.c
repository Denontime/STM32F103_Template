#include "bsp_led.h"

void LED_GPIO_Confg(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(LED_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = LED_R_PIN;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LED_G_PIN;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LED_B_PIN;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);

    GPIO_SetBits(LED_PORT, LED_R_PIN);
    GPIO_SetBits(LED_PORT, LED_G_PIN);
    GPIO_SetBits(LED_PORT, LED_B_PIN);
}

void LED_Set_Color(int ledMode)
{
    assert_param(IS_LED_ALL_PERIPH(ledMode));
    switch(ledMode)
    {
        case 1:
            GPIO_ResetBits(LED_PORT, LED_R_PIN);
            GPIO_SetBits(LED_PORT, LED_G_PIN);
            GPIO_SetBits(LED_PORT, LED_B_PIN);
            break;
        case 2:
            GPIO_SetBits(LED_PORT, LED_R_PIN);
            GPIO_ResetBits(LED_PORT, LED_G_PIN);
            GPIO_SetBits(LED_PORT, LED_B_PIN);
            break;
        case 3:
            GPIO_SetBits(LED_PORT, LED_R_PIN);
            GPIO_SetBits(LED_PORT, LED_G_PIN);
            GPIO_ResetBits(LED_PORT, LED_B_PIN);
            break;
        case 4:
            GPIO_ResetBits(LED_PORT, LED_R_PIN);
            GPIO_ResetBits(LED_PORT, LED_G_PIN);
            GPIO_SetBits(LED_PORT, LED_B_PIN);
            break;
        case 5:
            GPIO_ResetBits(LED_PORT, LED_R_PIN);
            GPIO_SetBits(LED_PORT, LED_G_PIN);
            GPIO_ResetBits(LED_PORT, LED_B_PIN);
            break;
        case 6:
            GPIO_SetBits(LED_PORT, LED_R_PIN);
            GPIO_ResetBits(LED_PORT, LED_G_PIN);
            GPIO_ResetBits(LED_PORT, LED_B_PIN);
            break;
        case 7:
            GPIO_ResetBits(LED_PORT, LED_R_PIN);
            GPIO_ResetBits(LED_PORT, LED_G_PIN);
            GPIO_ResetBits(LED_PORT, LED_B_PIN);
            break;
        case 8:
            GPIO_SetBits(LED_PORT, LED_R_PIN);
            GPIO_SetBits(LED_PORT, LED_G_PIN);
            GPIO_SetBits(LED_PORT, LED_B_PIN);
            break;
        default:
            GPIO_SetBits(LED_PORT, LED_R_PIN);
            GPIO_SetBits(LED_PORT, LED_G_PIN);
            GPIO_SetBits(LED_PORT, LED_B_PIN);
            break;

    }
}

void LED_Next_State(void)
{
    led_state++;
    if(led_state >= 8) led_state = 1;
    LED_Set_Color(led_state);
}

void LED_Prev_State(void)
{
    led_state--;
	if(led_state <= 0) led_state = 7;
	LED_Set_Color(led_state);
}

void LED_Blink(void)
{
    if(!led_delay_ms){
        LED_Next_State();
        led_delay_ms = 1000;
    }
    else{
        led_delay_ms--;
    }
}

