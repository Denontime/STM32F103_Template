#ifndef __DataToPC
#define	__DataToPC

#include "stm32f10x.h"

void ANO_DT_Send_Check_USE_USB(u8 head, u8 check_sum);
void ANO_DT_Send_Senser_USE_USB(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z);
extern void ANO_DT_Data_Receive_Anl_USE_USB(u8 *data_buf,u8 num);

#endif /* __DataToPC */
