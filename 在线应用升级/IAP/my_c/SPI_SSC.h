#include "stm32f10x.h"

#define         CSN_HIGH         GPIOA->BSRR= GPIO_Pin_4
#define         CSN_LOW          GPIOA->BRR = GPIO_Pin_4
#define         SCK_HIGH         GPIOA->BSRR= GPIO_Pin_5
#define         SCK_LOW          GPIOA->BRR = GPIO_Pin_5
#define         MOSI_HIGH        GPIOA->BSRR= GPIO_Pin_7
#define         MOSI_LOW         GPIOA->BRR = GPIO_Pin_7
#define         MISO_STATUS      GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)

void SPI_ssc_GpioInit(void);

uint16_t hw2000_write_reg(uint16_t addr, uint16_t data);
uint16_t hw2000_read_reg(uint16_t reg);
void hw2000_read_fifo(uint16_t addr, uint16_t *data, unsigned char length);
void hw2000_write_fifo(uint16_t addr, uint16_t *data, uint8_t length);

