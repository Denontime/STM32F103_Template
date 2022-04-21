#ifndef __CONTROLLER_WCS_H__
#define __CONTROLLER_WCS_H__

#ifdef __cplusplus
extern "C" 
{
	
#endif
#include "ControllerMachine.h"
//#include "usart.h"	
#include "Light_Controller.h"
unsigned char VerifyWCSData(uint8_t *buf, uint16_t length);
unsigned char WCSDataProcess(void);
void WCSReplyAfterLightBinding(uint8_t *buf);
unsigned char  ControllerCallsWCS(void);
void WCSDistributionBindingCompleted(void);
void SaveWCSDistributionAddress(unsigned char *buf);
unsigned char LightBindingInformation(void);
void ProductInformationProcessing(unsigned char *buf);	
void ConfirmProductInformationProcessing(unsigned char *buf);
void ControllerReceivesDataReplyWCS(void );
void ClearWaveInformation(void);
unsigned char  ControllerUploadPolling(void);
void ClearConfirmationMessage(void);
void PollingUnacknowledged(void);
void EnterDownloadAddressSave(unsigned char *buf);
void QuitDownloadAddressSave(unsigned char *buf);

void SavePictureInstruction(unsigned char *buf);	
void EnterBackgroundDownloadToWCS(void);
void SendEnterChangeBackgroundToWCS(void);
void QuitBackgroundDownloadToWCS(void);
void PictureContentSave(unsigned char *buf);
void ImageContentReplyToWCS(void);
void PictureContentReply(void);
void SendsDownloadPictureStatusToWCS(void);
unsigned char Usart2ReceiveProcessing(void);
unsigned char  FaultInfoSending(void);

//����������Ϣ	
#define HandshakeInformation 0X45
//WCS�·�����
#define	DataDistributionAndClearing 0X43
//WCS�·�ȷ����Ϣ
#define	IssueConfirmationInformation 0X47
//����
#define EnterBind 0X44
//WCS���Ͱ������Ϣ
#define	BindingCompletionReply 0X06
//�˰�
#define ExitBind 0X46
//������Ϣ�ϱ�
#define	ErrorInformationReporting 0X08
//����������ʾ������ģʽ
#define EnterDownloadingPic 0x01
//�·�������ʾ������
#define PreDownloadingPic 0x02
//�·�����ͼƬ����
#define DownlaodingPic 0x03
//�˳�������ʾ������ģʽ
#define ExitDownloadingPic 0x04
//��ѯLCD����
#define CheckEEPROMProduct 0x05
//ɾ��LCD����ͼƬ
#define DeleteEEPROMProduct 0x07


//δ���յ�ַ
#define AddressNotUploaded 0
//���յ�ַ
#define AddressUploadCompleted 1
//���·��Ĳ�Ʒ����
#define MaxNumberProducts 100
//ʹ����������
#define MaxLightNumber 15


extern unsigned char ProductNameLength;
extern struct CtrlToLight WaveInfo[MaxNumberProducts];
extern struct product lightInfo[MaxLightNumber];
extern struct ConfirmationInformation ConfirmationInfo[MaxLightNumber];
extern struct Background ChangeBackground ;
extern struct PictureContent SavePictureContent[30]; 
extern unsigned char QuantityOfProductsIssued;
extern unsigned char Controller_WhetherCallWCS;
extern unsigned char NumberOfPackets;
extern unsigned char PictureDownloadStatus;

	
#ifdef __cplusplus
}
#endif

#endif
