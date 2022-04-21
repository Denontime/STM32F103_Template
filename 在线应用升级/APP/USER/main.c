/*************************************
 * 文件名  ：main.c
 * 描述    ：串口1(USART1)向电脑的超级终端以1s为间隔打印当前ADC1的转换电压值         
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 库版本  ：ST3.0.0

**********************************************************************************/

#include "stm32f10x.h"
#include "usart1.h"

volatile uint64_t system_period;


int main(void)
{
  	  
  /* 配置系统时钟为72M */      
  SystemInit();	
  /* 配置串口 */
  USART1_Config();
  /* 初始化ADC1 */

	 			
	  while(1)                            
	  {
			
			if (sysTickUptime - system_period >= 4)
			{	
				system_period = sysTickUptime;				
				
				printf("APP执行FLASH用户代码中!!\r\n");
					
			}
	  }
}


