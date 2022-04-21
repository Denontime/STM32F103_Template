#include "Controller_WCS.h"
#include "Light_Controller.h"
void ControrllMachine(void);

#define AskLight 0X00
#define SendMessageToWCS 0X01
#define SendConfirmToLight 0X02
#define SendProductToLight 0X03
#define SendBindingToLight 0X04
#define SendExitBindingToLight 0X05
#define SendBindingCompleteToLight 0X06
#define SendBindingToWCS 0X07
#define Binding 0X08
#define SendEnterBackgroundDownloadToLight 0X09
#define SendReplacePictureCommandToLight 0X10
#define BackgroundDownload 0X11
#define SendPictureContentToLight 0X12
#define SendQuitBackgroundDownloadToLight 0X13


#define NoLightNumber 0
#define HaveLightNumber 1
#define IssueBindingComplete 1
#define EndSendBinding 0
extern unsigned char Process;

extern unsigned char PollingStatus;


