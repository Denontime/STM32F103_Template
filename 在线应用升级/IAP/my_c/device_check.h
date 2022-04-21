#ifndef __DEVICE_CHECK
#define	__DEVICE_CHECK

#include "stm32f10x.h"

#define FALSE 0
#define TRUE 1

#define POWER_KEY_ON 3000       //MS
#define POWER_KEY_OFF 3000       //MS

#define POWER_GPIO     GPIOB
#define POWER_PIN      GPIO_Pin_2  //PB2为BOOT1 配置IO口输入为下拉
#define POWER_GPIO_OUT GPIOB
#define POWER_GPIO_PIN GPIO_Pin_12

#define POWER_ON 1
#define POWER_OFF 0

#define LED00_GPIO  GPIOB
#define LED00_PIN   GPIO_Pin_7
#define LED01_GPIO  GPIOB
#define LED01_PIN   GPIO_Pin_8
#define LED10_GPIO  GPIOB
#define LED10_PIN   GPIO_Pin_0
#define LED11_GPIO  GPIOB
#define LED11_PIN   GPIO_Pin_1
#define LED20_GPIO  GPIOA
#define LED20_PIN   GPIO_Pin_3
#define LED21_GPIO  GPIOA
#define LED21_PIN   GPIO_Pin_4

#define SWITCH_GPIO  GPIOB
#define SWITCH_PIN   GPIO_Pin_3  //PB3为JTDO口，配置时需要加复用 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

#define KEY1_GPIO  GPIOB
#define KEY1_PIN   GPIO_Pin_10
#define KEY2_GPIO  GPIOB
#define KEY2_PIN   GPIO_Pin_5
#define KEY3_GPIO  GPIOB
#define KEY3_PIN   GPIO_Pin_6

#define ADC1_3D_DR_Address    ((u32)0x4001244C)

extern __IO uint16_t ADC_3D_ConvertedValue[3];
extern volatile _Bool key1_flag;
extern volatile _Bool key2_flag;
extern volatile _Bool key3_flag;

void GPIO_Clock_config(void);
void Power_control_Config(void);
void power_key_check(GPIO_TypeDef* GPIOx,u16 GPIO_Pin);

void ADC1_3D_Init(void);

void LED_gpio_config(void);

void KeySwitchConfig(void);

void nomal_key_check(void);

void Rock_Dispose(void);


#endif /* __DEVICE_CHECK */









