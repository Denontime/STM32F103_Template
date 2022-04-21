/*************************************
  CAN_bus.C
	CAN ����������ã����ͽ��գ����ݴ���
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
 * ��������CAN_GPIO_Config
 * ����  ��CAN��GPIO ����,PB8�������룬PB9�������
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */
static void CAN_BUS_GPIO_Config(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
   	
	  	/*����ʱ������*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	/*IO����*/
	//GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
	/* Configure CAN pin: RX */									               // PA11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	             // ��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Configure CAN pin: TX */									               // PA12
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		         // �����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}

/*
 * ��������CAN_NVIC_Config
 * ����  ��CAN��NVIC ����,��1���ȼ��飬0��0���ȼ�
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */
static void CAN_BUS_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
		/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	 	/*�ж�����*/
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;	   //CAN1 RX0�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		   //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			   //�����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*
 * ��������CAN_Mode_Config
 * ����  ��CAN��ģʽ ����
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */
static void CAN_BUS_Mode_Config(void)
{
  CAN_InitTypeDef        CAN_InitStructure;
	 	/************************CANͨ�Ų�������**********************************/
	/*CAN�Ĵ�����ʼ��*/
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	
	/*CAN��Ԫ��ʼ��*/
	CAN_InitStructure.CAN_TTCM=DISABLE;			   //MCR-TTCM  �ر�ʱ�䴥��ͨ��ģʽʹ��
	CAN_InitStructure.CAN_ABOM=DISABLE;//ENABLE;			   //MCR-ABOM  �Զ����߹��� 
	CAN_InitStructure.CAN_AWUM=DISABLE;//ENABLE;			   //MCR-AWUM  ʹ���Զ�����ģʽ
	CAN_InitStructure.CAN_NART=DISABLE;			   //MCR-NART  ��ֹ�����Զ��ش�	  DISABLE-�Զ��ش�
	CAN_InitStructure.CAN_RFLM=DISABLE;			   //MCR-RFLM  ����FIFO ����ģʽ  DISABLE-���ʱ�±��ĻḲ��ԭ�б���  
	CAN_InitStructure.CAN_TXFP=DISABLE;			   //MCR-TXFP  ����FIFO���ȼ� DISABLE-���ȼ�ȡ���ڱ��ı�ʾ�� 
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;  //��������ģʽ
	CAN_InitStructure.CAN_SJW=CAN_SJW_2tq;		   //BTR-SJW ����ͬ����Ծ��� 2��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_BS1=CAN_BS1_6tq;		   //BTR-TS1 ʱ���1 ռ����6��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;		   //BTR-TS1 ʱ���2 ռ����3��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_Prescaler = 4;		   ////BTR-BRP �����ʷ�Ƶ��  ������ʱ�䵥Ԫ��ʱ�䳤�� 36/(1+6+3)/4=0.9Mbps
	CAN_Init(CAN1, &CAN_InitStructure);
}

/*
 * ��������CAN_Filter_Config
 * ����  ��CAN�Ĺ����� ����
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */
static void CAN_BUS_Filter_Config(void)
{
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	
	/*CAN��������ʼ��*/
	CAN_FilterInitStructure.CAN_FilterNumber=0;						//��������0
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	//�����ڱ�ʶ������λģʽ
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	//������λ��Ϊ����32λ��
	/* ʹ�ܱ��ı�ʾ�����������ձ�ʾ�������ݽ��бȶԹ��ˣ���չID�������µľ����������ǵĻ��������FIFO0�� */

	CAN_FilterInitStructure.CAN_FilterIdHigh= 0x0;//(((u32)0xaaa0<<3)&0xFFFF0000)>>16;				//Ҫ���˵�ID��λ 
	CAN_FilterInitStructure.CAN_FilterIdLow= 0x0;//(((u32)0xaaa0<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF; //Ҫ���˵�ID��λ 
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= 0x0;//0xFFFF;			//��������16λÿλ����ƥ��
	CAN_FilterInitStructure.CAN_FilterMaskIdLow= 0x0;//0xFFE7;			//��������16λÿλ����ƥ��  ƥ��ID����λ
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0 ;				//��������������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;			//ʹ�ܹ�����
	CAN_FilterInit(&CAN_FilterInitStructure);
	/*CANͨ���ж�ʹ��*/
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
	
}


/*
 * ��������CAN_Config
 * ����  ����������CAN�Ĺ���
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */
void CAN_BUS_Config(void)
{
  CAN_BUS_GPIO_Config();
  CAN_BUS_NVIC_Config();
  CAN_BUS_Mode_Config();
  CAN_BUS_Filter_Config();   
}


/*
 * ��������CAN_BusSetMsg
 * ����  ��CANͨ�ű�����������
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */	 
void CAN_BusSetMsg(_CAN_DATA* CAN_DATA)
{
	uint8_t i;
	TxMessage.StdId=0x00;	
  TxMessage.ExtId = CAN_DATA->device_ID;					 //ʹ�õ���չID
  TxMessage.IDE = CAN_ID_EXT;					 //��չģʽ  ��ʶ��ѡ��λ
  TxMessage.RTR = CAN_RTR_DATA;				 //���͵������� �Ƿ�ΪԶ��֡
  TxMessage.DLC = 8;							 //���ݳ���Ϊ8�ֽ�
	
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
 /*�������ж�������*/
  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

  /* �Ƚ�ID�������Ƿ�Ϊ0x1314��DCBA */ 
  if((RxMessage.IDE == CAN_ID_EXT) && (RxMessage.DLC == 8))   //��֤���յ�ַ
  {
    crc_can = 0;
		for (i = 0; i < 7; i ++)
		{
			crc_can += RxMessage.Data[i];
		}
		if (crc_can == RxMessage.Data[7])
		{
			test_can ++;
			flag = 0; 					       //���ճɹ�
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
			flag = 0xff;     //����ʧ��  У�鲻ͨ��
  }
  else
  {
    flag = 0xff; 					   //����ʧ��
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
		/*����Ҫͨ��CAN���͵���Ϣ*/
		
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


