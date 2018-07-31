
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"


#include "api_os.h"
#include "api_debug.h"
#include "api_event.h"
#include "api_sms.h"
#include "api_hal_uart.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////configuration//////////////////////////////////////////////////
#define TEST_PHONE_NUMBER "13415840120"
const uint8_t unicodeMsg[] = {0x00, 0x61, 0x00, 0x61, 0x00, 0x61, 0x6d, 0x4b, 0x8b, 0xd5, 0x77, 0xed, 0x4f, 0xe1}; //unicode:aaa测试短信
const uint8_t gbkMsg[]     = {0x62, 0x62, 0x62, 0xB0, 0xA1, 0xB0, 0xA1, 0xB0, 0xA1, 0xB0, 0xA1, 0x63, 0x63, 0x63 };//GBK    :bbb啊啊啊啊ccc
const uint8_t utf8Msg[]    = "utf-8测试短信";//Cause the encoding format of this file(sms.c) is UTF-8
                            // UTF-8 Bytes:75 74 66 2D 38 E6 B5 8B E8 AF 95 E7 9F AD E4 BF A1 
                            //(unicode:\u0075\u0074\u0066\u002d\u0038\u6d4b\u8bd5\u77ed\u4fe1) //you can convert here:https://r12a.github.io/apps/conversion/
/////////////////////////////////////////////////////////////////////////////////////////////

#define MAIN_TASK_STACK_SIZE    (2048 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME          "SMS Test Task"

static HANDLE mainTaskHandle = NULL;
static uint8_t flag = 0;

void SMSInit()
{
    if(!SMS_SetFormat(SMS_FORMAT_TEXT,SIM0))
    {
        Trace(1,"sms set format error");
        return;
    }
    SMS_Parameter_t smsParam = {
        .fo = 17 ,
        .vp = 167,
        .pid= 0  ,
        .dcs= 8  ,//0:English 7bit, 4:English 8 bit, 8:Unicode 2 Bytes
    };
    if(!SMS_SetParameter(&smsParam,SIM0))
    {
        Trace(1,"sms set parameter error");
        return;
    }
    if(!SMS_SetNewMessageStorage(SMS_STORAGE_SIM_CARD))
    {
        Trace(1,"sms set message storage fail");
        return;
    }
}

void UartInit()
{
    UART_Config_t config = {
        .baudRate = UART_BAUD_RATE_115200,
        .dataBits = UART_DATA_BITS_8,
        .stopBits = UART_STOP_BITS_1,
        .parity   = UART_PARITY_NONE,
        .rxCallback = NULL,
    };
    UART_Init(UART1,config);
}

void Init()
{
    UartInit();
    SMSInit();
}



void SendSmsUnicode()
{
    Trace(1,"sms start send unicode message");
    if(!SMS_SendMessage(TEST_PHONE_NUMBER,unicodeMsg,sizeof(unicodeMsg),SIM0))
    {
        Trace(1,"sms send message fail");
    }
}

void SendSmsGbk()
{
    uint8_t* unicode = NULL;
    uint32_t unicodeLen;

    Trace(1,"sms start send GBK message");

    if(!SMS_LocalLanguage2Unicode(gbkMsg,sizeof(gbkMsg),CHARSET_CP936,&unicode,&unicodeLen))
    {
        Trace(1,"local to unicode fail!");
        return;
    }
    if(!SMS_SendMessage(TEST_PHONE_NUMBER,unicode,unicodeLen,SIM0))
    {
        Trace(1,"sms send message fail");
    }
    OS_Free(unicode);
}

void SendUtf8()
{
    uint8_t* unicode = NULL;
    uint32_t unicodeLen;

    Trace(1,"sms start send UTF-8 message");

    if(!SMS_LocalLanguage2Unicode(utf8Msg,strlen(utf8Msg),CHARSET_UTF_8,&unicode,&unicodeLen))
    {
        Trace(1,"local to unicode fail!");
        return;
    }
    if(!SMS_SendMessage(TEST_PHONE_NUMBER,unicode,unicodeLen,SIM0))
    {
        Trace(1,"sms send message fail");
    }
    OS_Free(unicode);
}


void SendSMS()
{
    // SendSmsUnicode();
    // SendSmsGbk();
    SendUtf8();
}

void ServerCenterTest()
{
    uint8_t addr[32];
    uint8_t temp;
    SMS_Server_Center_Info_t sca;
    sca.addr = addr;
    SMS_GetServerCenterInfo(&sca);
    Trace(1,"server center address:%s,type:%d",sca.addr,sca.addrType);
    temp = sca.addr[strlen(sca.addr)-1];
    sca.addr[strlen(sca.addr)-1] = '0';
    if(!SMS_SetServerCenterInfo(&sca))
        Trace(1,"SMS_SetServerCenterInfo fail");
    else
        Trace(1,"SMS_SetServerCenterInfo success");
    SMS_GetServerCenterInfo(&sca);
    Trace(1,"server center address:%s,type:%d",sca.addr,sca.addrType);
    sca.addr[strlen(sca.addr)-1] = temp;
    if(!SMS_SetServerCenterInfo(&sca))
        Trace(1,"SMS_SetServerCenterInfo fail");
    else
        Trace(1,"SMS_SetServerCenterInfo success");
}

void EventDispatch(API_Event_t* pEvent)
{
    switch(pEvent->id)
    {
        case API_EVENT_ID_NO_SIMCARD:
            Trace(10,"!!NO SIM CARD%d!!!!",pEvent->param1);
            break;
        case API_EVENT_ID_SYSTEM_READY:
            Trace(1,"system initialize complete");
            flag |= 1;
            break;
        case API_EVENT_ID_NETWORK_REGISTERED_HOME:
        case API_EVENT_ID_NETWORK_REGISTERED_ROAMING:
            Trace(2,"network register success");
            flag |= 2;
            break;
        case API_EVENT_ID_SMS_SENT:
            Trace(2,"Send Message Success");
            break;
        case API_EVENT_ID_SMS_RECEIVED:
            Trace(2,"received message");
            SMS_Encode_Type_t encodeType = pEvent->param1;
            uint32_t contentLength = pEvent->param2;
            uint8_t* header = pEvent->pParam1;
            uint8_t* content = pEvent->pParam2;

            Trace(2,"message header:%s",header);
            Trace(2,"message content length:%d",contentLength);
            if(encodeType == SMS_ENCODE_TYPE_ASCII)
            {
                Trace(2,"message content:%s",content);
                UART_Write(UART1,content,contentLength);
            }
            else
            {
                uint8_t tmp[500];
                memset(tmp,0,500);
                for(int i=0;i<contentLength;i+=2)
                    sprintf(tmp+strlen(tmp),"\\u%02x%02x",content[i],content[i+1]);
                Trace(2,"message content(unicode):%s",tmp);//you can copy this string to http://tool.chinaz.com/tools/unicode.aspx and display as Chinese
                uint8_t* gbk = NULL;
                uint32_t gbkLen = 0;
                if(!SMS_Unicode2LocalLanguage(content,contentLength,CHARSET_CP936,&gbk,&gbkLen))
                    Trace(10,"convert unicode to GBK fail!");
                else
                {
                    memset(tmp,0,500);
                    for(int i=0;i<gbkLen;i+=2)
                        sprintf(tmp+strlen(tmp),"%02x%02x ",gbk[i],gbk[i+1]);
                    Trace(2,"message content(GBK):%s",tmp);//you can copy this string to http://m.3158bbs.com/tool-54.html# and display as Chinese
                    UART_Write(UART1,gbk,gbkLen);//use serial tool that support GBK decode if have Chinese, eg: https://github.com/Neutree/COMTool
                }
                OS_Free(gbk);
            }
            break;
        case API_EVENT_ID_SMS_LIST_MESSAGE:
        {
            SMS_Message_Info_t* messageInfo = (SMS_Message_Info_t*)pEvent->pParam1;
            Trace(1,"message header index:%d,status:%d,number type:%d,number:%s,time:\"%u/%02u/%02u,%02u:%02u:%02u+%02d\"", messageInfo->index, messageInfo->status,
                                                                                        messageInfo->phoneNumberType, messageInfo->phoneNumber,
                                                                                        messageInfo->time.year, messageInfo->time.month, messageInfo->time.day,
                                                                                        messageInfo->time.hour, messageInfo->time.minute, messageInfo->time.second,
                                                                                        messageInfo->time.timeZone);
            Trace(1,"message content len:%d,data:%s",messageInfo->dataLen,messageInfo->data);
            UART_Write(UART1, messageInfo->data, messageInfo->dataLen);//use serial tool that support GBK decode if have Chinese, eg: https://github.com/Neutree/COMTool
            UART_Write(UART1,"\r\n\r\n",4);
            //need to free data here
            OS_Free(messageInfo->data);
            break;
        }
        case API_EVENT_ID_SMS_ERROR:
            Trace(10,"SMS error occured! cause:%d",pEvent->param1);
        default:
            break;
    }

    //system initialize complete and network register complete, now can send message
    if(flag == 3)
    {
        SMS_Storage_Info_t storageInfo;
        // SendSMS();
        ServerCenterTest();
        SMS_GetStorageInfo(&storageInfo,SMS_STORAGE_SIM_CARD);
        Trace(1,"sms storage sim card info, used:%d,total:%d",storageInfo.used,storageInfo.total);
        SMS_GetStorageInfo(&storageInfo,SMS_STORAGE_FLASH);
        Trace(1,"sms storage flash info, used:%d,total:%d",storageInfo.used,storageInfo.total);
        if(!SMS_DeleteMessage(5,SMS_STATUS_ALL,SMS_STORAGE_SIM_CARD))
            Trace(1,"delete sms fail");
        else
            Trace(1,"delete sms success");
        SMS_ListMessageRequst(SMS_STATUS_ALL,SMS_STORAGE_SIM_CARD);
        flag = 0;
    }
}



void SMSTest(void* pData)
{
    API_Event_t* event=NULL;

    Init();

    while(1)
    {
        if(OS_WaitEvent(mainTaskHandle, (void**)&event, OS_TIME_OUT_WAIT_FOREVER))
        {
            EventDispatch(event);
            OS_Free(event->pParam1);
            OS_Free(event->pParam2);
            OS_Free(event);
        }
    }
}

void sms_Main()
{
    mainTaskHandle = OS_CreateTask(SMSTest,
        NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle);
}

