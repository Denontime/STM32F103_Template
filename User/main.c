#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_exti.h"
#include "bsp_usart.h"
#include "bsp_beep.h"

int led_state = 0;
int led_delay_ms = 1000;

void delay_ms(int t)
{
	for(int i=0;i<t;i++){
		for(int j=0;j<5000;j++);
	}
}

int main(void)
{
	// ���������ʱ��ϵͳ��ʱ���Ѿ������ó�72M��
    char recv_ch;
	
	LED_GPIO_Confg();
    BEEP_GPIO_Confg();
//	KEY_GPIO_Confg();
    EXTI_Key_Config();
    SysTick_Init();
    USART_Config();
	
	while(1){
//		if(Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON){
//			ColorState--;
//			if(ColorState <= 0) ColorState = 7;
//			LED_Set_Color(ColorState);
//		}
//		if(Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON){
//			ColorState++;
//			if(ColorState >= 8) ColorState = 1;
//			LED_Set_Color(ColorState);
//		}
        
        recv_ch = getchar();
        printf("���յ��ַ���%d \n", recv_ch);
        led_state = recv_ch;
        LED_Set_Color(led_state);
        led_delay_ms = 1000;
	}

}

void assert_failed(uint8_t* file, uint32_t line)
{
	// ���Դ���ʱִ�еĴ���
	LED_Set_Color(LED_CYAN);
    printf("\r\n �ļ���%s   �У�%d ����������󣡣���", file, line);
}



