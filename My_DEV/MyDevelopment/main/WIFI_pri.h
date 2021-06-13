#ifndef _WIFI_PRI_H
#define _WIFI_PRI_H

#include "global.h"


typedef enum
{
    E_WIFI_IDLE_ST = 0,
    E_WIFI_ADHOC_ST,
    E_WIFI_PROCESS_ST,
    E_WIFI_JOIN_ST,
    E_WIFI_GOT_IP_ST,
    E_WIFI_READY_ST,
    E_WIFI_DISCONNECT_ST,
    E_WIFI_ANY_STATE = E_GLOBAL_ANY_ST,
    E_WIFI_INVALID_STATE = E_GLOABL_INVALID_ST,
} en_WIFI_StateType_t;

void WiF_Adhoc_Mode(st_gMsgQueueEntry_t *lstp_Msg);
void wifi_Join_Process(st_gMsgQueueEntry_t *lstp_Msg);
void wifi_gotIp_State(st_gMsgQueueEntry_t *lstp_Msg);
void wifi_ReadyStateProcess(st_gMsgQueueEntry_t *lstp_Msg);

#endif
