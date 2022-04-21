/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LITHG_CONTROLLER_H__
#define __LITHG_CONTROLLER_H__

#ifdef __cplusplus
extern "C" {
#endif
	
#include "main.h"
#include "Controller_WCS.h"
#include "usart.h"	
extern unsigned char NumberFaultyLight;
extern unsigned char PollingStatusReturn;
extern unsigned char testXor;
extern unsigned short int LightDataHeader;
extern unsigned char ConfirmQuantity;
unsigned char  LightDataProcess(uint8_t *buf);
unsigned char LightDataReply(void);	
unsigned char  Usart1ReceiveProcessing(void);
extern unsigned char LightAddress[3];	
unsigned char VerifyLightData(uint8_t *buf,uint16_t len);	
void ControllerDataProcess(uint8_t *buf);	
void SendWaveInfoToLight(void);
void LightSendsWaveConfirmationMessage(void);
void LightGetIntoBinding(void);
void SaveDistributionAddress(unsigned char *buf);
void ControllerSendExitBinding(void );
unsigned char AddrComper(unsigned char *addr);
unsigned char AddressIntegration(unsigned char *addr);
void PollingLightDataSaving(unsigned char *buf);
void ClearPollingData(void);
unsigned char  XORCheck(uint8_t *buf,uint16_t length);
void ConfirmedProducts (void);	
void productBeenConfirmedUploadedAgain(void);
void ControllerPollingLight(void);
void FautInfoSaving(unsigned char Faulty ,unsigned char CommunicateStatus  );
void EnterDownloadInterface(void);
void QuitDownloadInterface(void);
void SendPictureReplacementInstructionToLight(void);
void SendPictureContentLight(void);
//WCS下发数据回复
#define	DataDistributionReply 0X01
//Controller轮询数据后Light回复
#define PollingReply 0X02
//Controller向Light发送确认数据，Light回复
#define ConfirmDataReply 0X04
//Controller轮询Light故障，Light回复。
#define MalfunctionReply 0X05
//Light上传绑定信息
#define BindingInformationUpload 0X07
//地址修改回复
#define AddressModificationReply 0X0C
//进入LCD下载背景后Light回复
#define EnterDownloadLCDBackgroundModeReply 0X0E
//Controller下发更换LCD背景回复 
#define ReplaceBackgroundReply 0X0F
//Controller下发更换图片内容，Light回复
#define ChangeBackgroundPictureContentReply 0X10
//退出LCD下载背景Light回复
#define ExitDownloadingLCDBackgroundInterfaceReply 0X11
//查询LCD背景图片Light回复
#define QueryLCDBackgroundReply 0X12
//删除LCD背景图片Light回复
#define DeleteLCDBackgroundReply 0X13

//Light拣货是否完成
#define Incomplete 0X00
#define Await 0X01
#define Complete 0X02

//轮询错误状态是否返回
#define CommunicationNormal 0X01
#define CommunicationAbnormal 0X00

#endif
	
	


