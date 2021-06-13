
#ifndef _TCP_SERVER_PUB_H
#define _TCP_SERVER_PUB_H

#include "global.h"

typedef enum
{
   E_TCP_SERVER_MSG_EMPTY = E_GLOBAL_EMPTY_MSG,
   E_TCP_SERVER_OPEN_SOCKFD,            /*  Self Posted Message  */
   E_TCP_SERVER_OPEN_SERVER,         /*  Self Posted Message  */
   E_TCP_SERVER_ACCEPT_STATE,               /*  Self Posted Message  */
   E_TCP_SERVER_LISTEN_STATE,             /*  Self Posted Message  */
   E_TCP_SERVER_PROCESS_STATE,            /*  Self Posted Message  */
   E_TCP_SERVER_RESPONSE_STATE,
   E_TCP_SERVER_INVALID
} en_TCPServer_MsgType_t;

typedef enum
{
    E_TCP_SERVER_IDLE_ST = 0,
    E_TCP_SERVER_SOCKFD_ST,
    E_TCP_SERVER_OPEN_ST,
    E_TCP_SERVER_ACCEPT_ST,
    E_TCP_SERVER_READ_ST,
    E_TCP_SERVER_PROCESS_ST,
    E_TCP_SERVER_WRITE_ST,
    E_TCP_SERVER_ANY_STATE_ST = E_GLOBAL_ANY_ST,
    E_TCP_SERVER_INVALID_STATE_ST = E_GLOABL_INVALID_ST,
} en_TCP_SerStateType_t;

typedef struct
{
  u32 mu32_ServerRunningPort;
  s32 ms32_RunningServerSockFd;
  u32 mu32_RunningServerClientCount;
  u32 mu32_ServerMode;
	struct sockaddr_in mst_sock_addr;
  s32 ms32_RunningServerClientSockFd[U8_MAX_SERVERCONNECTION_COUNT];
  struct sockaddr_in mstar_ClientSock[U8_MAX_SERVERCONNECTION_COUNT];
}st_ServerRunningInfo_t;

extern st_ServerRunningInfo_t gst_ServerRunningInfo;


void TCP_Server_Process(en_ModActionReqType_t lst_ActionRequested, u32 lu32_Reason, st_MainTaskControlInfo_t *lst_TasKControlInfo);

#endif
