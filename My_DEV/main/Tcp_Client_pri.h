#ifndef _TCP_CLIENT_PRI_H
#define _TCP_CLIENT_PRI_H

#include "global.h"

#define TCP_CLIENT_PORT_NO 54545
#define TCP_CLIENT_SERVERIP "192.168.0.2"

typedef struct
{
  u32 mu32_ClientPortNo;
  s32 ms32_TCPClientSockFd;
  char ms8_TcpCliHostByName[20];
  u32 mu32_TcpServerIpAdd[20];
	struct sockaddr_in mst_sock_addr;
}st_TCPClientRunningInfo_t;



void Tcp_Client_SockFd_Open(st_gMsgQueueEntry_t *lstp_Msg);
void Tcp_Client_Connect(st_gMsgQueueEntry_t *lstp_Msg);
void Tcp_Client_WriteToServer(st_gMsgQueueEntry_t *lstp_Msg);
 void Tcp_Client_ReadFromServer(st_gMsgQueueEntry_t *lstp_Msg);
 void Tcp_Client_Process(st_gMsgQueueEntry_t *lstp_Msg);
   void Tcp_Client_Close(st_gMsgQueueEntry_t *lstp_Msg);
#endif
