/*----------------------------------------------------------------------------------------------------------------------/
*              drv_AS5600.C 
----------------------------------------------------------------------------------------------------------------------*/
#include "drv_TLE5012B.h"
#include "SPI_SSC.h"

#define READ_STATUS					0x8001
#define READ_ANGLE_VALUE		0x8020
#define READ_SPEED_VALUE		0x8031
#define WRITE_MOD1_VALUE		0x5060
#define MOD1_VALUE	0x0001
#define WRITE_MOD2_VALUE		0x5080
#define MOD2_VALUE	0x0801
#define WRITE_MOD3_VALUE		0x5091
#define MOD3_VALUE	0x0000
#define WRITE_MOD4_VALUE		0x50E0
#define MOD4_VALUE	0x0098
#define WRITE_IFAB_VALUE		0x50B1
#define IFAB_VALUE 0x000D

float value_Angle;
uint16_t TLE5012B_Angle;
uint16_t TLE5012B_speed;


void TLE5012B_Init(void)
{
}

void TLE5012B_read_data(void)
{
	
	TLE5012B_Angle = 0x7fff & hw2000_read_reg(READ_ANGLE_VALUE);
	value_Angle = 360 * ((float)(TLE5012B_Angle) / 32767);
	
	TLE5012B_speed = 0x7fff & hw2000_read_reg(READ_SPEED_VALUE);
	
}



