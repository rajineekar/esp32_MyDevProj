/*************************************************************************
|"2016 Danlaw Inc. All rights reserved."
|
| File              :   mailbox_pub.h
| Created           :   March 22 2016
| Application       :
| Functions         :
| Author            :   Danlaw
| Target            :   Gen 3.x - QCM
| Environment       :   DS5 and Cygwin
*************************************************************************/

/************************************************************************
| Change History:
| Version      Date          		Person        	Change Description
| 0.1          March 22 2016         					Initial version
|
*************************************************************************/
#ifndef _GLOBAL_H
#define _GLOBAL_H

/************************************************************************
|          Include files
|************************************************************************/
#include <string.h>


#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

#include "tcpip_adapter.h"
#include "esp_log.h"


#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_spiffs.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>


/************************************************************************
|          Macro Definition
|************************************************************************/
typedef unsigned char boolean_t;
//typedef unsigned char bool;
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;

/************************************************************************
|          Enum Definition
|************************************************************************/
typedef enum
{
	E_NON_ATOMIC_STATE = 0x00,
	E_ATOMIC_STATE
}en_gStateType_t;

typedef enum
{
	E_WIFI_MODULE = 0x00,
	TCP_SERVER_MODULE,
	TCP_CLIENT_MODULE,
	UDP_SERVER_MODULE,
	E_MAX_MODULE
}en_gModules_t;

typedef enum
{
    E_ALL_TASK_ACTIVE  = 0x0000,
    E_WIFI_TASK_IDLE   = 0x0001,
    E_TCP_SERVER_IDLE  = 0x0002,
		E_TCP_CLIENT_IDLE  = 0x0004,
		E_UDP_SERVER_IDLE  = 0x0008,
    E_ALL_TASK_IDLE    = 0x0007
}en_PwrTaskIdleType_t;

typedef enum
{
   E_NO_ACTION = 0,
   E_APP_INIT,
   E_APP_DEINIT,
   E_APP_TASK_START,
   E_INVALID_REQUEST
} en_ModActionReqType_t;

typedef enum
{
	E_GLOBAL_ANY_ST = 0xFD,
	E_GLOABL_INVALID_ST = 0xFE,
}en_gTaskState_t;

typedef enum
{
	E_GLOBAL_EMPTY_MSG = 0,
}en_gTaskMsg_t;


typedef struct
{
    u32                    mu32_MonitorInterval;
    u32                    mu32_FaultName;
    u32                    mu32_RestartMsg;
    void                   (*mod_HealthCheckFunc)(u32 *module_counter, u16 *module_State);
} st_MainTaskControlInfo_t;

typedef enum
{
  E_API_SUCCESS,
  E_API_FAILED
}en_apierrorState_t;

#define U8_MAX_SERVERCONNECTION_COUNT 10
#define U8_MAX_SERVER_RECEIVE_BUFF 50


typedef struct
{
u8 mu8ar_ReceivedBuffer[U8_MAX_SERVER_RECEIVE_BUFF];
u8 mu8ar_TxBuffer[U8_MAX_SERVER_RECEIVE_BUFF];
}st_ServerBufferInfo_t;


/************************************************************************
|          Structure Definition
|************************************************************************/
typedef struct
{
    en_gModules_t mu8_SourceId;
    u8 mu8_MsgId;
    u8 mu8_PriorityId;
    u32 mu32_Attribute;
} st_gMsgQueueEntry_t;

typedef struct
{
    u8 mu8_CurrentState;
    u8 mu8_PreviousState;
    en_gStateType_t		men_StateType;
    void (*g_MsgProcessingFunc)(st_gMsgQueueEntry_t *lstp_Message);
} st_gStateStatus_t;

typedef struct
{
    u8  mu8_CurrentState;
    en_gModules_t	men_MsgSource;
    u8	mu8_Msg;
    u8  mu8_NextState;
    void (*g_MsgProcessingFunc)(st_gMsgQueueEntry_t *lstp_Message);
} st_gStateTable_t;

/************************************************************************
|          PROTOTYPE
|************************************************************************/

/************************************************************************
|          End File
|************************************************************************/
#endif
