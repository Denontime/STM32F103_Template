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
//WCS�·����ݻظ�
#define	DataDistributionReply 0X01
//Controller��ѯ���ݺ�Light�ظ�
#define PollingReply 0X02
//Controller��Light����ȷ�����ݣ�Light�ظ�
#define ConfirmDataReply 0X04
//Controller��ѯLight���ϣ�Light�ظ���
#define MalfunctionReply 0X05
//Light�ϴ�����Ϣ
#define BindingInformationUpload 0X07
//��ַ�޸Ļظ�
#define AddressModificationReply 0X0C
//����LCD���ر�����Light�ظ�
#define EnterDownloadLCDBackgroundModeReply 0X0E
//Controller�·�����LCD�����ظ� 
#define ReplaceBackgroundReply 0X0F
//Controller�·�����ͼƬ���ݣ�Light�ظ�
#define ChangeBackgroundPictureContentReply 0X10
//�˳�LCD���ر���Light�ظ�
#define ExitDownloadingLCDBackgroundInterfaceReply 0X11
//��ѯLCD����ͼƬLight�ظ�
#define QueryLCDBackgroundReply 0X12
//ɾ��LCD����ͼƬLight�ظ�
#define DeleteLCDBackgroundReply 0X13

//Light����Ƿ����
#define Incomplete 0X00
#define Await 0X01
#define Complete 0X02

//��ѯ����״̬�Ƿ񷵻�
#define CommunicationNormal 0X01
#define CommunicationAbnormal 0X00

#endif
	
	


