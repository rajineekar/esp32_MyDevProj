#ifndef _UDP_SERVER_PUB_H
#define _UDP_SERVER_PUB_H

#include "global.h"

typedef enum
{
   E_UDP_SERVER_MSG_EMPTY = E_GLOBAL_EMPTY_MSG,
   E_UDP_SERVER_OPEN_SOCKFD,            /*  Self Posted Message  */
   E_UDP_SERVER_OPEN_SERVER,         /*  Self Posted Message  */
   E_UDP_SERVER_LISTEN_STATE,             /*  Self Posted Message  */
   E_UDP_SERVER_PROCESS_STATE,            /*  Self Posted Message  */
   E_UDP_SERVER_RESPONSE_STATE,
   E_UDP_SERVER_CLOSE_STATE,
   E_UDP_SERVER_INVALID
} en_UDPServer_MsgType_t;

typedef enum
{
    E_UDP_SERVER_IDLE_ST = 0,
    E_UDP_SERVER_SOCKFD_ST,
    E_UDP_SERVER_OPEN_ST,
    E_UDP_SERVER_READ_ST,
    E_UDP_SERVER_PROCESS_ST,
    E_UDP_SERVER_WRITE_ST,
    E_UDP_SERVER_CLOSE_ST,
    E_UDP_SERVER_ANY_STATE_ST = E_GLOBAL_ANY_ST,
    E_UDP_SERVER_INVALID_STATE_ST = E_GLOABL_INVALID_ST,
} en_UDP_SerStateType_t;


typedef struct
{
  u32 mu32_ServerPortNo;
  s32 ms32_ServerSockFd;
  u32 mu32_ServerMode;
	struct sockaddr_in mst_Ser_addr;
  struct sockaddr_in mstar_Client_addr;
  u32 mu32_Cli_AddrSize;
}st_UDPServerInfo_t;


void UDP_Server_Process(en_ModActionReqType_t lst_ActionRequested, u32 lu32_Reason, st_MainTaskControlInfo_t *lst_TasKControlInfo);

#endif
