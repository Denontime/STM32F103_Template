/**
  ******************************************************************************
  * @文件名     ： main.c
  * @作者       ： strongerHuang
  * @版本       ： V1.0.0
  * @日期       ： 2018年08月07日
  * @摘要       ： 主函数 - Demo软件工程
  ******************************************************************************/
/*----------------------------------------------------------------------------
  更新日志:
  2018-08-07 V1.0.0:初始版本
  ----------------------------------------------------------------------------*/
/* 包含的头文件 --------------------------------------------------------------*/
#include "bsp.h"


/************************************************
函数名称 ： Delay
功    能 ： 软件延时
参    数 ： Cnt --- 延时计数
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
static void Delay(uint32_t Cnt)
{
  uint32_t i;

  while(Cnt--)
  {
    for(i=0; i<0x80000; i++);
  }
}

/************************************************
函数名称 ： System_Initializes
功    能 ： 系统初始化
参    数 ： 无
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void System_Initializes(void)
{
  BSP_Initializes();
}

/************************************************
函数名称 ： main
功    能 ： 主函数入口
参    数 ： 无
返 回 值 ： int
作    者 ： strongerHuang
*************************************************/
int main(void)
{
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x3000);

  System_Initializes();

  while(1)
  {
    LED_TOGGLE();                                //LED变化
    Delay(5);                                    //Demo延时
  }
}


/**** Copyright (C)2017 strongerHuang. All Rights Reserved **** END OF FILE ****/
