/*************************************
 * 文件名  ：main.c
 * 描述    ：串口1(USART1)向电脑的超级终端以1s为间隔打印当前ADC1的转换电压值         
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 库版本  ：ST3.0.0

**********************************************************************************/

#include "stm32f10x.h"
#include "usart1.h"
#include "iap.h"

//#include "adc.h"       
//#include "can.h"

//#include "led.h"
//#include "key.h"

//#include "device_check.h"
//#include "CAN_bus.h"

//#include "hw_config.h"

//#include "SPI_SSC.h"
//#include "drv_TLE5012B.h"

//#include "DataToPC.h"
//#include "Queue.h"

volatile uint64_t system_period;
volatile uint64_t usart_receive_T;

volatile _Bool receive_flag = 0;
volatile uint8_t test1 = 0;

_Bool Download_Firmware = 1;
volatile FLASH_Status FLASHStatus1;

// 软件延时
//void Delay(unsigned long time)
//{unsigned long i,j;
  
//	for(j=0; j<time; j++)
//	{
//	   for(i=0;i<12000;i++);
//	}
//}

int main(void)
{
	uint8_t i;
//	u16 oldcount = 0;				//老的串口接收数据值
//	u16 applenth = 0;				//接收到的app代码长度
	
  /* 配置系统时钟为72M */      
  SystemInit();	
  /* 配置串口 */
  USART1_Config();
  /* 初始化ADC1 */
//  ADC1_Init();

//  LED_GPIO_Config(); //LED 端口初始化   	
//  Key_GPIO_Config();//按键端口初始化
		
 	/* 配置CAN模块 */
//	CAN_Config();
//	CAN_BUS_Config();
	
//	GPIO_Clock_config();
//	Power_control_Config();
//	ADC1_3D_Init();
//	LED_gpio_config();
//	KeySwitchConfig();

  //TLE5012B  SSC
//	SPI_ssc_GpioInit();

  //USB
//	USB_Config();

	 			
	  while(1)                            
	  {
			//update page by page
			
			if (Download_Firmware == 1)  //需要更新固件，先擦除APP应用代码区
			{
				Download_Firmware = 0;
				FLASH_Unlock();
				for (i = 0; i < 44; i ++)
				{
					FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
					FLASHStatus1 = FLASH_ErasePage(FLASH_APP1_ADDR + i * 1024);
				}
				FLASH_Lock();
			}
			
			if (USART_RX_CNTF != USART_RX_ALL_CNT)  //数据有更新  时间需要更新
			{
				usart_receive_T = sysTickUptime;
				USART_RX_CNTF = USART_RX_ALL_CNT;
				system_period = sysTickUptime;
			}
			if (USART_RX_ALL_CNT == 0)
			{
				usart_receive_T = sysTickUptime;
			}
			
			if (sysTickUptime - usart_receive_T >= 100 && USART_RX_ALL_CNT != 0)  //100ms 没有收到数据 认为数据接收完毕
			{
				
				if (USART_RX_ALL_CNT - 1 % FLASH_ONE_K != 0)
				{
					printf("开始传输第 %d shuju \r\n", USART_RX_ALL_CNT - FLASH_ONE_K * page_k);
					if ((page_k + 1) % 2 == 0)
					{
						iap_write_appbin(FLASH_APP1_ADDR + FLASH_ONE_K * page_k, USART_RX_BUF1, USART_RX_ALL_CNT - FLASH_ONE_K * page_k);
					}
					else
					{
						iap_write_appbin(FLASH_APP1_ADDR + FLASH_ONE_K * page_k, USART_RX_BUF, USART_RX_ALL_CNT - FLASH_ONE_K * page_k);
					}
				  printf("yigong %d byte \r\n", USART_RX_ALL_CNT);
				
					USART_RX_ALL_CNT = 0;
					page_k = 0;
				}
			}
			
			if (onek_complete == 1)   //if the first page  check the address 0X08XXXXXX
			{
				if (page_k == 1)
				{
					if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
					{	 
						iap_write_appbin(FLASH_APP1_ADDR, USART_RX_BUF, FLASH_ONE_K);//更新FLASH代码   
						printf("开始传输第 %d 页\r\n", page_k);
					}
				}
				else
				{
					if (page_k % 2 == 0)
					{
						iap_write_appbin(FLASH_APP1_ADDR + FLASH_ONE_K * (page_k - 1), USART_RX_BUF1, FLASH_ONE_K);
					}
					else
					{
						iap_write_appbin(FLASH_APP1_ADDR + FLASH_ONE_K * (page_k - 1), USART_RX_BUF, FLASH_ONE_K);
					}
					printf("开始传输第 %d 页\r\n", page_k);
				}
				onek_complete = 0;   //update one K complete clear flag
			}
			
			if (sysTickUptime - system_period >= 10000)
			{
				system_period = sysTickUptime;
				
//					printf("开始更新固件...\r\n");
//					
//					if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
//					{	 
//						iap_write_appbin(FLASH_APP1_ADDR,USART_RX_BUF,applenth);//更新FLASH代码   
//						printf("固件更新完成!\r\n");	
//					}
//					else 
//					{   
//						printf("非FLASH应用程序!\r\n");
//					}
//					
//					receive_flag = 0;
//					applenth = 0;
				
//				SysTick->CTRL = 0x00000004;//关闭定时器
//				USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//开启串口接受中断
//				USART_Cmd(USART1, DISABLE);
//				RCC_RTCCLKCmd(DISABLE);

					GPIO_DeInit(GPIOC);
					USART_DeInit(USART1);
					SysTick->CTRL = 0x00000004;
				
//				SCB->VTOR = 0x08000000U |0x5000;
					
//					printf("开始执行FLASH用户代码!!\r\n");
					if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
					{	 
						iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码
					}else 
					{
//						printf("非FLASH应用程序,无法执行!\r\n");   
					}
					while(1);
					
//					printf("开始执行SRAM用户代码!!\r\n");
//					if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x20000000)//判断是否为0X20XXXXXX.
//					{	 
//						iap_load_app(0X20001000);//SRAM地址
//					}else 
//					{
//						printf("非SRAM应用程序,无法执行!\r\n");   
//					}		
			}
	  }
}


