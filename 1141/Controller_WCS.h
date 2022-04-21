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

//发送握手信息	
#define HandshakeInformation 0X45
//WCS下发数据
#define	DataDistributionAndClearing 0X43
//WCS下发确认信息
#define	IssueConfirmationInformation 0X47
//进绑定
#define EnterBind 0X44
//WCS发送绑定完成信息
#define	BindingCompletionReply 0X06
//退绑定
#define ExitBind 0X46
//错误信息上报
#define	ErrorInformationReporting 0X08
//进入下载显示屏背景模式
#define EnterDownloadingPic 0x01
//下发更换显示屏背景
#define PreDownloadingPic 0x02
//下发更换图片内容
#define DownlaodingPic 0x03
//退出下载显示屏背景模式
#define ExitDownloadingPic 0x04
//查询LCD背景
#define CheckEEPROMProduct 0x05
//删除LCD背景图片
#define DeleteEEPROMProduct 0x07


//未接收地址
#define AddressNotUploaded 0
//接收地址
#define AddressUploadCompleted 1
//已下发的产品数量
#define MaxNumberProducts 100
//使用最多灯数量
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
