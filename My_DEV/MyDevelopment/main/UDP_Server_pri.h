#ifndef _UDP_SERVER_PRI_H
#define _UDP_SERVER_PRI_H

#include "global.h"

#define U16_UDP_BUFFER_LEN 20

typedef enum
{
  E_SERVER_MODE_BLOCK,
  E_SERVER_MODE_NON_BLOCK
}en_Server_ConnectionMode_t;


void UDP_Server_SockFd_Open(st_gMsgQueueEntry_t *lstp_Msg);
void UDP_Server_Bind(st_gMsgQueueEntry_t *lstp_Msg);
void UDP_Server_Listen(st_gMsgQueueEntry_t *lstp_Msg);
void UDP_Server_Data_Process(st_gMsgQueueEntry_t *lstp_Msg);
void UDP_Server_Send(st_gMsgQueueEntry_t *lstp_Msg);
void UDP_Server_Close(st_gMsgQueueEntry_t *lstp_Msg);

static void UDP_Server_Connection(void *pvParameters);


#endif
