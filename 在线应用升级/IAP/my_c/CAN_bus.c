/*************************************
  CAN_bus.C
	CAN 总线相关配置，发送接收，数据处理
**********************************************************************************/
#include "CAN_bus.h"
#include "usb_istr.h"

CanTxMsg TxMessage;
CanRxMsg RxMessage;

volatile  uint8_t  Pitch_RxMessage[8];
volatile  uint8_t  Roll_RxMessage[8];
volatile  uint8_t  YAW_RxMessage[8];

__IO uint32_t flag;

_CAN_DATA Pitch_CAN_DATA;
_CAN_DATA Roll_CAN_DATA;
_CAN_DATA Yaw_CAN_DATA;

/*
 * 函数名：CAN_GPIO_Config
 * 描述  ：CAN的GPIO 配置,PB8上拉输入，PB9推挽输出
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */
static void CAN_BUS_GPIO_Config(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
   	
	  	/*外设时钟设置*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	/*IO设置*/
	//GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
	/* Configure CAN pin: RX */									               // PA11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	             // 上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Configure CAN pin: TX */									               // PA12
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		         // 复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}

/*
 * 函数名：CAN_NVIC_Config
 * 描述  ：CAN的NVIC 配置,第1优先级组，0，0优先级
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */
static void CAN_BUS_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
		/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	 	/*中断设置*/
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;	   //CAN1 RX0中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		   //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			   //子优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*
 * 函数名：CAN_Mode_Config
 * 描述  ：CAN的模式 配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */
static void CAN_BUS_Mode_Config(void)
{
  CAN_InitTypeDef        CAN_InitStructure;
	 	/************************CAN通信参数设置**********************************/
	/*CAN寄存器初始化*/
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	
	/*CAN单元初始化*/
	CAN_InitStructure.CAN_TTCM=DISABLE;			   //MCR-TTCM  关闭时间触发通信模式使能
	CAN_InitStructure.CAN_ABOM=DISABLE;//ENABLE;			   //MCR-ABOM  自动离线管理 
	CAN_InitStructure.CAN_AWUM=DISABLE;//ENABLE;			   //MCR-AWUM  使用自动唤醒模式
	CAN_InitStructure.CAN_NART=DISABLE;			   //MCR-NART  禁止报文自动重传	  DISABLE-自动重传
	CAN_InitStructure.CAN_RFLM=DISABLE;			   //MCR-RFLM  接收FIFO 锁定模式  DISABLE-溢出时新报文会覆盖原有报文  
	CAN_InitStructure.CAN_TXFP=DISABLE;			   //MCR-TXFP  发送FIFO优先级 DISABLE-优先级取决于报文标示符 
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;  //正常工作模式
	CAN_InitStructure.CAN_SJW=CAN_SJW_2tq;		   //BTR-SJW 重新同步跳跃宽度 2个时间单元
	CAN_InitStructure.CAN_BS1=CAN_BS1_6tq;		   //BTR-TS1 时间段1 占用了6个时间单元
	CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;		   //BTR-TS1 时间段2 占用了3个时间单元
	CAN_InitStructure.CAN_Prescaler = 4;		   ////BTR-BRP 波特率分频器  定义了时间单元的时间长度 36/(1+6+3)/4=0.9Mbps
	CAN_Init(CAN1, &CAN_InitStructure);
}

/*
 * 函数名：CAN_Filter_Config
 * 描述  ：CAN的过滤器 配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */
static void CAN_BUS_Filter_Config(void)
{
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	
	/*CAN过滤器初始化*/
	CAN_FilterInitStructure.CAN_FilterNumber=0;						//过滤器组0
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	//工作在标识符屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	//过滤器位宽为单个32位。
	/* 使能报文标示符过滤器按照标示符的内容进行比对过滤，扩展ID不是如下的就抛弃掉，是的话，会存入FIFO0。 */

	CAN_FilterInitStructure.CAN_FilterIdHigh= 0x0;//(((u32)0xaaa0<<3)&0xFFFF0000)>>16;				//要过滤的ID高位 
	CAN_FilterInitStructure.CAN_FilterIdLow= 0x0;//(((u32)0xaaa0<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF; //要过滤的ID低位 
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= 0x0;//0xFFFF;			//过滤器高16位每位必须匹配
	CAN_FilterInitStructure.CAN_FilterMaskIdLow= 0x0;//0xFFE7;			//过滤器低16位每位必须匹配  匹配ID低两位
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0 ;				//过滤器被关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;			//使能过滤器
	CAN_FilterInit(&CAN_FilterInitStructure);
	/*CAN通信中断使能*/
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
	
}


/*
 * 函数名：CAN_Config
 * 描述  ：完整配置CAN的功能
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void CAN_BUS_Config(void)
{
  CAN_BUS_GPIO_Config();
  CAN_BUS_NVIC_Config();
  CAN_BUS_Mode_Config();
  CAN_BUS_Filter_Config();   
}


/*
 * 函数名：CAN_BusSetMsg
 * 描述  ：CAN通信报文内容设置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */	 
void CAN_BusSetMsg(_CAN_DATA* CAN_DATA)
{
	uint8_t i;
	TxMessage.StdId=0x00;	
  TxMessage.ExtId = CAN_DATA->device_ID;					 //使用的扩展ID
  TxMessage.IDE = CAN_ID_EXT;					 //扩展模式  标识符选择位
  TxMessage.RTR = CAN_RTR_DATA;				 //发送的是数据 是否为远程帧
  TxMessage.DLC = 8;							 //数据长度为8字节
	
	TxMessage.Data[0] = 0x000000ff & (CAN_DATA->Motor_Pos >> 8);
	TxMessage.Data[1] = 0x000000ff &  CAN_DATA->Motor_Pos;
	
	
	TxMessage.Data[2] = CAN_DATA->Dir;
	
  TxMessage.Data[3] = CAN_DATA->Power;
	
	TxMessage.Data[4] += 5;
  TxMessage.Data[5] += 6;
	TxMessage.Data[6] += 7;
	TxMessage.Data[7] = 0;
	for (i = 0; i < 7; i ++)
	{
		TxMessage.Data[7] += TxMessage.Data[i];
	}
}

volatile uint8_t test_can;
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  uint8_t crc_can,i;
	
	USB_Istr();
 /*从邮箱中读出报文*/
  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

  /* 比较ID和数据是否为0x1314及DCBA */ 
  if((RxMessage.IDE == CAN_ID_EXT) && (RxMessage.DLC == 8))   //验证接收地址
  {
    crc_can = 0;
		for (i = 0; i < 7; i ++)
		{
			crc_can += RxMessage.Data[i];
		}
		if (crc_can == RxMessage.Data[7])
		{
			test_can ++;
			flag = 0; 					       //接收成功
			if (RxMessage.ExtId == Pitch_ExtId)
			{
				for (i = 0; i < 8; i ++)
				{
					Pitch_RxMessage[i] = RxMessage.Data[i];
				}
			}
			else if (RxMessage.ExtId == Roll_ExtId)
			{
				for (i = 0; i < 8; i ++)
				{
					Roll_RxMessage[i] = RxMessage.Data[i];
				}
			}
			else if (RxMessage.ExtId == Yaw_ExtId)
			{
				for (i = 0; i < 8; i ++)
				{
					YAW_RxMessage[i] = RxMessage.Data[i];
				}
			}
		}
		else
			flag = 0xff;     //接收失败  校验不通过
  }
  else
  {
    flag = 0xff; 					   //接收失败
  }
}


volatile uint64_t can_test_period;
volatile uint8_t test11;
volatile uint8_t test2;
void CAN_BUS_Send(void)
{
	uint8_t transmit_mailbox = 0;
	if (sysTickUptime - can_test_period >= 4)
	{	
		can_test_period = sysTickUptime;				
		/*设置要通过CAN发送的信息*/
		
//		Rock_Dispose();
		//PITCH
		if (Pitch_CAN_DATA.RockData > 0x80)
		{
			Pitch_CAN_DATA.Motor_Pos = 1000;
			Pitch_CAN_DATA.Dir = 3;
		}
		else if (Pitch_CAN_DATA.RockData < 0x80)
		{
			Pitch_CAN_DATA.Motor_Pos = 1000;
			Pitch_CAN_DATA.Dir = 1;
		}
		else
		{
			Pitch_CAN_DATA.Motor_Pos = 0;
			Pitch_CAN_DATA.Dir = 2;
		}
		//ROLL
		if (Roll_CAN_DATA.RockData > 0x80)
		{
			Roll_CAN_DATA.Motor_Pos = 1000;
			Roll_CAN_DATA.Dir = 3;
		}
		else if (Roll_CAN_DATA.RockData < 0x80)
		{
			Roll_CAN_DATA.Motor_Pos = 1000;
			Roll_CAN_DATA.Dir = 1;
		}
		else
		{
			Roll_CAN_DATA.Motor_Pos = 0;
			Roll_CAN_DATA.Dir = 2;
		}
		
		test11 ++;
		if (test11 > 2)
			test11 = 1;
		if (test11 == 1)
		{
			Pitch_CAN_DATA.device_ID = Pitch_ExtId;
			Pitch_CAN_DATA.Power = 50;
			CAN_BusSetMsg(&Pitch_CAN_DATA);
		}
		else if (test11 == 2)
		{
			Roll_CAN_DATA.device_ID = Roll_ExtId;
			Roll_CAN_DATA.Power = 50;
			CAN_BusSetMsg(&Roll_CAN_DATA);
		}
		else if (test11 == 3)
		{
			Yaw_CAN_DATA.device_ID = Yaw_ExtId;
			Yaw_CAN_DATA.Power = 50;
			CAN_BusSetMsg(&Yaw_CAN_DATA);
		}
//		if (key2_flag == TRUE || key3_flag == TRUE)
		{
			test2++;
			transmit_mailbox = CAN_Transmit(CAN1, &TxMessage);
//			key2_flag = FALSE;
//			key3_flag = FALSE;
		}
	}
}


/**************************END OF FILE************************************/


