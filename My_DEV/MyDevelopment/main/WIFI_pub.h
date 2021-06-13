
#ifndef _WIFI_PUB_H
#define _WIFI_PUB_H

#include "global.h"



typedef enum
{
   E_WIFI_MSG_EMPTY = E_GLOBAL_EMPTY_MSG,
   E_WIFI_GO_ADHOC,            /*  Self Posted Message  */
   E_WIFI_GO_JOIN,         /*  Self Posted Message  */
   E_WIFI_GO_FOR_IP,
   E_WIFI_PROCESS,               /*  Self Posted Message  */
   E_WIFI_READY,             /*  Self Posted Message  */
   E_WIFI_LOST_IP,
   E_WIFI_INVALID = E_GLOABL_INVALID_ST
} en_WIFI_MsgType_t;

void WiFI_Process(en_ModActionReqType_t lst_ActionRequested, u32 lu32_Reason, st_MainTaskControlInfo_t *lst_TasKControlInfo);
void Wifi_PreCheckingInfo(void);


#endif
