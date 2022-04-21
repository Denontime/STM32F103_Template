/*************************************
device_check.c
���ܣ������̨�����貢������ 

**********************************************************************************/
#include "device_check.h"

__IO uint16_t ADC_3D_ConvertedValue[3];
volatile _Bool key1_flag = FALSE;
volatile _Bool key2_flag = FALSE;
volatile _Bool key3_flag = FALSE;

void GPIO_Clock_config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //���������˿�PA��ʱ��
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); // ʹ��PB�˿�ʱ��
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE); // ʹ��PC�˿�ʱ�� 
}
 
/***************  ���Ƶ�Դ���� *******************/
void Power_control_Config(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = POWER_GPIO_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(POWER_GPIO_OUT, &GPIO_InitStructure);  //��ʼ��gpio�˿�
	GPIO_ResetBits(POWER_GPIO_OUT, POWER_GPIO_PIN);	 // �ر�power
	
	GPIO_InitStructure.GPIO_Pin = POWER_PIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(POWER_GPIO, &GPIO_InitStructure);	//��ʼ���˿�
}

/*
  ��Դ���ذ�����Ⲣ���򿪹رյ�Դ����
*/

void power_key_check(GPIO_TypeDef* GPIOx,u16 GPIO_Pin)
{
	static __IO uint64_t Power_key_onoff_time;
	static __IO uint8_t  key_debouce = 100;
	static _Bool power_on_flag = FALSE;
	static _Bool power_on_flag1 = TRUE;
	if (key_debouce > 110 || key_debouce < 90)
		key_debouce = 100;
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == POWER_ON ) 
	{
		if (key_debouce < 110)
		  key_debouce ++;
	}
	else
	{
		if (key_debouce > 90)
		  key_debouce --;
	}
	if (key_debouce == 110)
	{
		if (sysTickUptime - Power_key_onoff_time >= POWER_KEY_ON)
		{
			if (power_on_flag1)
			{
				if (power_on_flag == TRUE)
				{
					power_on_flag = FALSE;
					GPIO_ResetBits(POWER_GPIO_OUT, POWER_GPIO_PIN);
					
					GPIO_ResetBits(LED00_GPIO, LED00_PIN);
				}
				else if (power_on_flag == FALSE)
				{
					power_on_flag = TRUE;
					GPIO_SetBits(POWER_GPIO_OUT, POWER_GPIO_PIN);
					
					GPIO_SetBits(LED00_GPIO, LED00_PIN);
				}
		  }
			power_on_flag1 = FALSE;
		}
	}
	else if (key_debouce == 90)
	{
		power_on_flag1 = TRUE;
		Power_key_onoff_time = sysTickUptime;
	}
	GPIO_SetBits(POWER_GPIO_OUT, POWER_GPIO_PIN);
}


/*���ò���ͨ���˿� ʹ��GPIOʱ��	  ����ADC����PA0�˿��ź�*/
 void ADC1_3D_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		    //GPIO����Ϊģ������
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		    //GPIO����Ϊģ������
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		    //GPIO����Ϊģ������
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*����ADC1�Ĺ���ģʽΪMDAģʽ  */
 void ADC1_3D_Mode_Config(void)
{
  DMA_InitTypeDef DMA_InitStructure;
  ADC_InitTypeDef ADC_InitStructure;	
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //ʹ��MDA1ʱ��
	/* DMA channel1 configuration */
  DMA_DeInit(DMA1_Channel1);  //ָ��DMAͨ��
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_3D_DR_Address;//����DMA�����ַ
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_3D_ConvertedValue;	//����DMA�ڴ��ַ��ADCת�����ֱ�ӷ���õ�ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //����Ϊ����Ϊ���ݴ������Դ
  DMA_InitStructure.DMA_BufferSize = 3;	//DMA����������Ϊ1��
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //��������
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�趨�ڴ��ַ������� enable
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//�������ݿ��
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//�洢���ݿ��
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//��������ģʽ
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
//  
//  /* Enable DMA channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);  //ʹ��DMAͨ��

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	//ʹ��ADC1ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
     
  /* ADC1 configuration */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ʹ�ö���ģʽ��ɨ��ģʽ
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;  //���ͨ������ɨ��ģʽ
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //������Ӵ�����
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //ʹ�������Ҷ���
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 3;  // 2��ת��ͨ��
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel11 configuration */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_239Cycles5);

  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);	 //ʹ��ADC��DMA
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE); //ʹ��ADC1

  /* Enable ADC1 reset calibaration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
     
  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //��ʼת��
}

/*��ʼ��ADC1 */
void ADC1_3D_Init(void)
{
	ADC1_3D_Config();
	ADC1_3D_Mode_Config();
}

void LED_gpio_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = LED00_PIN;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LED00_GPIO, &GPIO_InitStructure);  //��ʼ��gpio�˿�
	
	GPIO_InitStructure.GPIO_Pin = LED01_PIN;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LED01_GPIO, &GPIO_InitStructure);  //��ʼ��gpio�˿�
	
	GPIO_InitStructure.GPIO_Pin = LED10_PIN;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LED10_GPIO, &GPIO_InitStructure);  //��ʼ��gpio�˿�
	
	GPIO_InitStructure.GPIO_Pin = LED11_PIN;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LED11_GPIO, &GPIO_InitStructure);  //��ʼ��gpio�˿�
	
	GPIO_InitStructure.GPIO_Pin = LED20_PIN;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LED20_GPIO, &GPIO_InitStructure);  //��ʼ��gpio�˿�
	
	GPIO_InitStructure.GPIO_Pin = LED21_PIN;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LED21_GPIO, &GPIO_InitStructure);  //��ʼ��gpio�˿�
	
  GPIO_ResetBits(LED00_GPIO, LED00_PIN);
	GPIO_ResetBits(LED01_GPIO, LED01_PIN);
	GPIO_ResetBits(LED10_GPIO, LED10_PIN);
	GPIO_ResetBits(LED11_GPIO, LED11_PIN);
	GPIO_ResetBits(LED20_GPIO, LED20_PIN);
	GPIO_ResetBits(LED21_GPIO, LED21_PIN);
}

//void Rock_Dispose(void)
//{
//	Pitch_CAN_DATA.RockData = (uint8_t)(ADC_3D_ConvertedValue[1] >> 4);
//	Roll_CAN_DATA.RockData = (uint8_t)(ADC_3D_ConvertedValue[2] >> 4);
//	if (Pitch_CAN_DATA.RockData > 0x70 && Pitch_CAN_DATA.RockData < 0x90)
//		Pitch_CAN_DATA.RockData = 0x80;
//	if (Roll_CAN_DATA.RockData > 0x70 && Roll_CAN_DATA.RockData < 0x90)
//		Roll_CAN_DATA.RockData = 0x80;
//}


void KeySwitchConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);  //�ͷ�PA15 PB3 PB4����ͨIO��
	
	GPIO_InitStructure.GPIO_Pin = SWITCH_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //�˿�����Ϊ��������
	GPIO_Init(SWITCH_GPIO, &GPIO_InitStructure);	//��ʼ���˿�
	
	GPIO_InitStructure.GPIO_Pin = KEY1_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //�˿�����Ϊ��������
	GPIO_Init(KEY1_GPIO, &GPIO_InitStructure);	//��ʼ���˿�
	
	GPIO_InitStructure.GPIO_Pin = KEY2_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //�˿�����Ϊ��������
	GPIO_Init(KEY2_GPIO, &GPIO_InitStructure);	//��ʼ���˿�
	
	GPIO_InitStructure.GPIO_Pin = KEY3_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //�˿�����Ϊ��������
	GPIO_Init(KEY3_GPIO, &GPIO_InitStructure);	//��ʼ���˿�
}

/*
  ��ͨ�������
*/
void nomal_key_check(void)
{
	static uint8_t switch_debouce = 100;
	static uint8_t key1_debouce = 100;
	static uint8_t key2_debouce = 100;
	static uint8_t key3_debouce = 100;
	if (switch_debouce > 110 || switch_debouce < 90)
		switch_debouce = 100;
	if(GPIO_ReadInputDataBit(SWITCH_GPIO,SWITCH_PIN) == 1 ) 
	{
		if (switch_debouce < 110)
		  switch_debouce ++;
	}
	else
	{
		if (switch_debouce > 90)
		  switch_debouce --;
	}
	
	if (key1_debouce > 110 || key1_debouce < 90)
		key1_debouce = 100;
	if(GPIO_ReadInputDataBit(KEY1_GPIO,KEY1_PIN) == 1 ) 
	{
		if (key1_debouce < 110)
		  key1_debouce ++;
	}
	else
	{
		if (key1_debouce == 92)
		  key1_flag = TRUE;
		if (key1_debouce > 90)
		  key1_debouce --;
	}
	
	if (key2_debouce > 110 || key2_debouce < 90)
		key2_debouce = 100;
	if(GPIO_ReadInputDataBit(KEY2_GPIO,KEY2_PIN) == 1 ) 
	{
		if (key2_debouce < 110)
		  key2_debouce ++;
	}
	else
	{
		if (key2_debouce == 92)
		  key2_flag = TRUE;
		if (key2_debouce > 90)
		  key2_debouce --;
	}
	
	if (key3_debouce > 110 || key3_debouce < 90)
		key3_debouce = 100;
	if(GPIO_ReadInputDataBit(KEY3_GPIO,KEY3_PIN) == 1 ) 
	{
		if (key3_debouce < 110)
		  key3_debouce ++;
	}
	else
	{
		if (key3_debouce == 92)
		  key3_flag = TRUE;
		if (key3_debouce > 90)
		  key3_debouce --;
	}
}
 
 
 















