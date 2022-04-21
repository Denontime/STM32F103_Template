/************************************
 * 文件名  ：DataToPC.c
 * 描述    ：打印数据 通过USB口传输到上位机      
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 库版本  ：ST3.0.0  *

**********************************************************************************/

#include "DataToPC.h"
#include "hw_config.h"

#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)      ) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

uint8_t data_to_send[50];
uint8_t usb_test[3];
float piddata[20];
/***********************************************************************/
void ANO_DT_Send_Check_USE_USB(u8 head, u8 check_sum)
{
  u8 sum = 0,i=0;
  data_to_send[0]=0xAA;
  data_to_send[1]=0xAA;
  data_to_send[2]=0xEF;
  data_to_send[3]=2;
  data_to_send[4]=head;
  data_to_send[5]=check_sum;
  for(i=0;i<6;i++)
    sum += data_to_send[i];
  data_to_send[6]=sum;
  USB_TxWrite(data_to_send, 7);
}

void ANO_DT_Send_Senser_USE_USB(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z)
{
  u8 _cnt=0;
  vs16 _temp;
  u8 sum = 0;
  u8 i=0;
  data_to_send[_cnt++]=0xAA;
  data_to_send[_cnt++]=0xAA;
  data_to_send[_cnt++]=0x02;
  data_to_send[_cnt++]=0;
  
  _temp = a_x;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = a_y;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = a_z;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  
  _temp = g_x;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = g_y;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = g_z;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  
  _temp = m_x;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = m_y;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  _temp = m_z;
  data_to_send[_cnt++]=BYTE1(_temp);
  data_to_send[_cnt++]=BYTE0(_temp);
  
  data_to_send[3] = _cnt-4;
  
  sum = 0;
  for(i=0;i<_cnt;i++)
    sum += data_to_send[i];
  data_to_send[_cnt++] = sum;
  USB_TxWrite(data_to_send, _cnt);
}

void ANO_DT_Data_Receive_Anl_USE_USB(u8 *data_buf,u8 num)
{
  u8 sum = 0,i=0;
  for(i=0;i<(num-1);i++)
    sum += *(data_buf+i);
  if(!(sum==*(data_buf+num-1)))       {usb_test[0]++;return;    } //判断sum
  if(!(*(data_buf)==0xAA && *(data_buf+1)==0xAF))     {usb_test[1]++;return;}//判断帧头
  usb_test[2]++;
  if(*(data_buf+2)==0X01)
  {
    if(*(data_buf+4)==0X01)
      ;//mpu6050.Acc_CALIBRATE = 1;
    if(*(data_buf+4)==0X02)
      ;//mpu6050.Gyro_CALIBRATE = 1;
    if(*(data_buf+4)==0X03)
    {
      ;//mpu6050.Acc_CALIBRATE = 1;
      ;//mpu6050.Gyro_CALIBRATE = 1;
    }
  }
  
  if(*(data_buf+2)==0X02)
  {
    if(*(data_buf+4)==0X01)
    {
//      ANO_Send_PID_Flag_USB[0]=1;
//      ANO_Send_PID_Flag_USB[1]=1;
//      ANO_Send_PID_Flag_USB[2]=1;
//      ANO_Send_PID_Flag_USB[3]=1;
//      ANO_Send_PID_Flag_USB[4]=1;
//      ANO_Send_PID_Flag_USB[5]=1;
//      Bling_Set(&Light_1,1000,50,0.5,0,GPIOB,GPIO_Pin_12,0);
//      Bling_Set(&Light_2,1000,50,0.5,0,GPIOB,GPIO_Pin_13,0);
//      Bling_Set(&Light_3,1000,50,0.5,0,GPIOB,GPIO_Pin_14,0);
    }
    if(*(data_buf+4)==0X02)
    {
      
    }
    if(*(data_buf+4)==0XA0)     //读取版本信息
    {
      ;//f.send_version = 1;
    }
    if(*(data_buf+4)==0XA1)     //恢复默认参数
    {
//      Sort_PID_Flag=3;
//      Bling_Set(&Light_1,1000,50,0.5,0,GPIOB,GPIO_Pin_12,0);
//      Bling_Set(&Light_2,1000,50,0.5,0,GPIOB,GPIO_Pin_13,0);
//      Bling_Set(&Light_3,1000,50,0.5,0,GPIOB,GPIO_Pin_14,0);
    }
  }
  
  if(*(data_buf+2)==0X10)                             //PID1
  {
    piddata[0]  = 0.001*( (vs16)(*(data_buf+4)<<8)|*(data_buf+5) );
    piddata[1]  = 0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
    piddata[2]  = 0.01*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );
    piddata[3]   = 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
    piddata[4]   = 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
    piddata[5]   = 0.01*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );
    piddata[6]    = 0.001*( (vs16)(*(data_buf+16)<<8)|*(data_buf+17) );
    piddata[7]    = 0.001*( (vs16)(*(data_buf+18)<<8)|*(data_buf+19) );
    piddata[8]    = 0.01*( (vs16)(*(data_buf+20)<<8)|*(data_buf+21) );
    ANO_DT_Send_Check_USE_USB(*(data_buf+2),sum);
    
    
  }
  if(*(data_buf+2)==0X11)                             //PID2
  {
//    Total_Controller.Roll_Angle_Control.Kp  = 0.001*( (vs16)(*(data_buf+4)<<8)|*(data_buf+5) );
//    Total_Controller.Roll_Angle_Control.Ki  = 0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
//    Total_Controller.Roll_Angle_Control.Kd  = 0.01*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );
//    Total_Controller.Pitch_Angle_Control.Kp   = 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
//    Total_Controller.Pitch_Angle_Control.Ki   = 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
//    Total_Controller.Pitch_Angle_Control.Kd   = 0.01*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );
//    Total_Controller.Yaw_Angle_Control.Kp    = 0.001*( (vs16)(*(data_buf+16)<<8)|*(data_buf+17) );
//    Total_Controller.Yaw_Angle_Control.Ki    = 0.001*( (vs16)(*(data_buf+18)<<8)|*(data_buf+19) );
//    Total_Controller.Yaw_Angle_Control.Kd    = 0.01*( (vs16)(*(data_buf+20)<<8)|*(data_buf+21) );
    ANO_DT_Send_Check_USE_USB(*(data_buf+2),sum);
    
  }
  if(*(data_buf+2)==0X12)                             //PID3
  {
//    Total_Controller.High_Speed_Control.Kp    = 0.001*( (vs16)(*(data_buf+4)<<8)|*(data_buf+5) );
//    Total_Controller.High_Speed_Control.Ki    = 0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
//    Total_Controller.High_Speed_Control.Kd    = 0.01*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );
//    Total_Controller.High_Position_Control.Kp = 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
//    Total_Controller.High_Position_Control.Ki = 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
//    Total_Controller.High_Position_Control.Kd = 0.01*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );
//    Total_Controller.Latitude_Speed_Control.Kp= 0.001*( (vs16)(*(data_buf+16)<<8)|*(data_buf+17) );
//    Total_Controller.Latitude_Speed_Control.Ki= 0.001*( (vs16)(*(data_buf+18)<<8)|*(data_buf+19) );
//    Total_Controller.Latitude_Speed_Control.Kd= 0.01*( (vs16)(*(data_buf+20)<<8)|*(data_buf+21) );
//    /***********************位置控制：位置、速度参数共用一组PID参数**********************************************************/
//    Total_Controller.Longitude_Speed_Control.Kp=Total_Controller.Latitude_Speed_Control.Kp;
//    Total_Controller.Longitude_Speed_Control.Ki=Total_Controller.Latitude_Speed_Control.Ki;
//    Total_Controller.Longitude_Speed_Control.Kd=Total_Controller.Latitude_Speed_Control.Kd;
    ANO_DT_Send_Check_USE_USB(*(data_buf+2),sum);
    
  }
  if(*(data_buf+2)==0X13)                             //PID4
  {
//    Total_Controller.Latitude_Position_Control.Kp    = 0.001*( (vs16)(*(data_buf+4)<<8)|*(data_buf+5) );
//    Total_Controller.Latitude_Position_Control.Ki    = 0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
//    Total_Controller.Latitude_Position_Control.Kd    = 0.01*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );
//    Total_Controller.High_Acce_Control.Kp            = 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
//    Total_Controller.High_Acce_Control.Ki            = 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
//    Total_Controller.High_Acce_Control.Kd            = 0.01*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );
//    /***********************位置控制：位置、速度参数共用一组PID参数**********************************************************/
//    Total_Controller.Longitude_Position_Control.Kp=Total_Controller.Latitude_Position_Control.Kp;
//    Total_Controller.Longitude_Position_Control.Ki=Total_Controller.Latitude_Position_Control.Ki;
//    Total_Controller.Longitude_Position_Control.Kd=Total_Controller.Latitude_Position_Control.Kd;
    ANO_DT_Send_Check_USE_USB(*(data_buf+2),sum);
    
  }
  if(*(data_buf+2)==0X14)                             //PID5
  {
//    Total_Controller.Optical_Position_Control.Kp = 0.001*( (vs16)(*(data_buf+4)<<8)|*(data_buf+5) );
//    Total_Controller.Optical_Position_Control.Ki = 0.001*( (vs16)(*(data_buf+6)<<8)|*(data_buf+7) );
//    Total_Controller.Optical_Position_Control.Kd = 0.01*( (vs16)(*(data_buf+8)<<8)|*(data_buf+9) );
//    Total_Controller.Optical_Speed_Control.Kp = 0.001*( (vs16)(*(data_buf+10)<<8)|*(data_buf+11) );
//    Total_Controller.Optical_Speed_Control.Ki = 0.001*( (vs16)(*(data_buf+12)<<8)|*(data_buf+13) );
//    Total_Controller.Optical_Speed_Control.Kd = 0.01*( (vs16)(*(data_buf+14)<<8)|*(data_buf+15) );
    
    ANO_DT_Send_Check_USE_USB(*(data_buf+2),sum);
  }
  if(*(data_buf+2)==0X15)                             //PID6
  {
    ANO_DT_Send_Check_USE_USB(*(data_buf+2),sum);
//    Sort_PID_Cnt++;
//    Sort_PID_Flag=1;
//    Bling_Set(&Light_1,1000,50,0.5,0,GPIOB,GPIO_Pin_12,0);
//    Bling_Set(&Light_2,1000,50,0.5,0,GPIOB,GPIO_Pin_13,0);
//    Bling_Set(&Light_3,1000,50,0.5,0,GPIOB,GPIO_Pin_14,0);
  }
}


