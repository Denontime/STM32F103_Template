/*----------------------------------------------------------------------------------------------------------------------/
*              drv_AS5600.C 
----------------------------------------------------------------------------------------------------------------------*/
#include "drv_AS5600.h"
#include "IIC.h"

#define AS5600_Adress 0x36

uint8_t AS5600_Angle[2];

void AS5600_Init(void)
{
	while(!I2C_Write(AS5600_Adress, 0x07, 0x01));
	while(!I2C_Write(AS5600_Adress, 0x08, 0x40));
}

void AS5600_read_data(void)
{
	i2c_len_Read(AS5600_Adress, 0x0E, 2, AS5600_Angle);
}



