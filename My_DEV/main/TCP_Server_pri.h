#ifndef _TCP_SERVER_PRI_H
#define _TCP_SERVER_PRI_H

#include "global.h"

typedef enum
{
  E_SERVER_MODE_BLOCK,
  E_SERVER_MODE_NON_BLOCK
}en_Server_ConnectionMode_t;


void Open_Tcp_Server_SockFd(st_gMsgQueueEntry_t *lstp_Msg);
void Open_Tcp_Server_OpenServer(st_gMsgQueueEntry_t *lstp_Msg);
void Ready_To_Accept_Server_Connection(st_gMsgQueueEntry_t *lstp_Msg);
void Data_Readng_FromSocks(st_gMsgQueueEntry_t *lstp_Msg);
static void TCP_Server_Connection(void *pvParameters);
void Data_Process_State(st_gMsgQueueEntry_t *lstp_Msg);
void Data_Response_state(st_gMsgQueueEntry_t *lstp_Msg);


#endif
