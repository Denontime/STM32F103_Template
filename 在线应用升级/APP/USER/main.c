/*************************************
 * �ļ���  ��main.c
 * ����    ������1(USART1)����Եĳ����ն���1sΪ�����ӡ��ǰADC1��ת����ѹֵ         
 * ʵ��ƽ̨��MINI STM32������ ����STM32F103C8T6
 * ��汾  ��ST3.0.0

**********************************************************************************/

#include "stm32f10x.h"
#include "usart1.h"

volatile uint64_t system_period;


int main(void)
{
  	  
  /* ����ϵͳʱ��Ϊ72M */      
  SystemInit();	
  /* ���ô��� */
  USART1_Config();
  /* ��ʼ��ADC1 */

	 			
	  while(1)                            
	  {
			
			if (sysTickUptime - system_period >= 4)
			{	
				system_period = sysTickUptime;				
				
				printf("APPִ��FLASH�û�������!!\r\n");
					
			}
	  }
}


