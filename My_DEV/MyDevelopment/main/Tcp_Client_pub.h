#ifndef _TCP_CLIENT_PUB_H
#define _TCP_CLIENT_PUB_H

#include "global.h"

typedef enum
{
   E_TCP_CLIENT_MSG_EMPTY = E_GLOBAL_EMPTY_MSG,
   E_TCP_CLIENT_OPEN_SOCKFD,            /*  Self Posted Message  */
   E_TCP_CLIENT_CONNECT_TO_SERVER,         /*  Self Posted Message  */
   E_TCP_CLIENT_WRITE,               /*  Self Posted Message  */
   E_TCP_CLIENT_READ,             /*  Self Posted Message  */
   E_TCP_CLIENT_PROCESS,            /*  Self Posted Message  */
   E_TCP_CLIENT_CLOSE,
   E_TCP_CLIENT_RESPONSE,
   E_TCP_CLIENT_INVALID
} en_TCP_Client_MsgType_t;

typedef enum
{
    E_TCP_CLIENT_IDLE_ST = 0,
    E_TCP_CLIENT_SOCKFD_ST,
    E_TCP_CLIENT_CONNECT_ST,
    E_TCP_CLIENT_WRITE_ST,
    E_TCP_CLIENT_READ_ST,
    E_TCP_CLIENT_PROCESS_ST,
    E_TCP_CLIENT_CLOSE_ST,
    E_TCP_CLIENT_ANY_STATE_ST = E_GLOBAL_ANY_ST,
    E_TCP_CLIENT_INVALID_STATE_ST = E_GLOABL_INVALID_ST,
} en_TCP_ClientStateType_t;


void TCP_Client_Process(en_ModActionReqType_t lst_ActionRequested, u32 lu32_Reason, st_MainTaskControlInfo_t *lst_TasKControlInfo);

#endif
