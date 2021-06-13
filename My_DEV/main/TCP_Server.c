

#include "TCP_Server_pri.h"
#include "TCP_Server_pub.h"
#include "global.h"
#include "mailbox_pub.h"

st_ServerRunningInfo_t gst_ServerRunningInfo;
st_ServerBufferInfo_t  gst_ServerBufferInfo[U8_MAX_SERVERCONNECTION_COUNT];


static st_gStateStatus_t sst_TcpServerCurrentStateInfo;
static const st_gStateTable_t scstar_TcpServerStateTable[] =
{
    { E_TCP_SERVER_IDLE_ST,    E_WIFI_MODULE,   E_TCP_SERVER_OPEN_SOCKFD,           E_TCP_SERVER_SOCKFD_ST,     Open_Tcp_Server_SockFd },

    { E_TCP_SERVER_SOCKFD_ST,  TCP_SERVER_MODULE,   E_TCP_SERVER_OPEN_SERVER,        E_TCP_SERVER_OPEN_ST,   Open_Tcp_Server_OpenServer },

    { E_TCP_SERVER_OPEN_ST,    TCP_SERVER_MODULE,   E_TCP_SERVER_ACCEPT_STATE,        E_TCP_SERVER_ACCEPT_ST,   Ready_To_Accept_Server_Connection },

    { E_TCP_SERVER_ACCEPT_ST,  TCP_SERVER_MODULE,   E_TCP_SERVER_LISTEN_STATE,        E_TCP_SERVER_READ_ST,   Data_Readng_FromSocks },

    { E_TCP_SERVER_READ_ST,    TCP_SERVER_MODULE,   E_TCP_SERVER_PROCESS_STATE,        E_TCP_SERVER_PROCESS_ST,   Data_Process_State },

    {E_TCP_SERVER_PROCESS_ST ,TCP_SERVER_MODULE ,   E_TCP_SERVER_RESPONSE_STATE,        E_TCP_SERVER_WRITE_ST, Data_Response_state },

    { E_TCP_SERVER_PROCESS_ST,    TCP_SERVER_MODULE,   E_TCP_SERVER_ACCEPT_STATE,     E_TCP_SERVER_ACCEPT_ST,   Ready_To_Accept_Server_Connection },

    { E_TCP_SERVER_ANY_STATE_ST,  TCP_SERVER_MODULE,   E_TCP_SERVER_ACCEPT_STATE,        E_TCP_SERVER_ANY_STATE_ST,   Ready_To_Accept_Server_Connection },

    {E_TCP_SERVER_INVALID_STATE_ST,    TCP_SERVER_MODULE,   E_TCP_SERVER_OPEN_SOCKFD,         E_TCP_SERVER_SOCKFD_ST,    NULL  },
};


void Open_Tcp_Server_SockFd(st_gMsgQueueEntry_t *lstp_Msg)
{

  printf("\r\n Open_Tcp_Server_SockFd:Opening\n");
  if(sst_TcpServerCurrentStateInfo.mu8_CurrentState != sst_TcpServerCurrentStateInfo.mu8_PreviousState)
  {
      ;
  }

    gst_ServerRunningInfo.ms32_RunningServerSockFd = socket(AF_INET, SOCK_STREAM, 0);
    printf("\r\n Open_Tcp_Server_SockFd:sockfd:%d\n",gst_ServerRunningInfo.ms32_RunningServerSockFd );
  	if (gst_ServerRunningInfo.ms32_RunningServerSockFd  < 0)
  	{
         	   printf("Open_Tcp_Server_SockFd:Creating the socket failed ,port(%d)\r\n",gst_ServerRunningInfo.mu32_ServerRunningPort);
  		       return;
    }
    printf("\r\n Open_Tcp_Server_SockFd:Expecting to next leel 1\n");
    mailbox_Statemachine_Change_Req(TCP_SERVER_MODULE,TCP_SERVER_MODULE,E_TCP_SERVER_OPEN_SERVER,0,0);
}


void Open_Tcp_Server_OpenServer(st_gMsgQueueEntry_t *lstp_Msg)
{
  int ret,flags;
  unsigned int optval = 1;
  if(sst_TcpServerCurrentStateInfo.mu8_CurrentState != sst_TcpServerCurrentStateInfo.mu8_PreviousState)
  {
      ;
  }
  if(gst_ServerRunningInfo.ms32_RunningServerSockFd  < 0)
  {
    return;
  }
  printf("\r\n 1: port(%d),sockfd(%d)\r\n",gst_ServerRunningInfo.mu32_ServerRunningPort,gst_ServerRunningInfo.ms32_RunningServerSockFd );
  setsockopt (gst_ServerRunningInfo.ms32_RunningServerSockFd , SOL_SOCKET,SO_REUSEADDR, &optval,sizeof(optval));
	memset(&gst_ServerRunningInfo.mst_sock_addr, 0, sizeof(gst_ServerRunningInfo.mst_sock_addr));

	gst_ServerRunningInfo.mst_sock_addr.sin_family = AF_INET;
	gst_ServerRunningInfo.mst_sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);;
	gst_ServerRunningInfo.mst_sock_addr.sin_port = htons(gst_ServerRunningInfo.mu32_ServerRunningPort);

	ret = bind(gst_ServerRunningInfo.ms32_RunningServerSockFd , (struct sockaddr*)&gst_ServerRunningInfo.mst_sock_addr, sizeof(gst_ServerRunningInfo.mst_sock_addr));
	if (ret < 0)
	{
		printf("\r\n 2.Open_Tcp_Server:BIND failed\r\n");
		close(gst_ServerRunningInfo.ms32_RunningServerSockFd);
    gst_ServerRunningInfo.ms32_RunningServerSockFd = -1;
		return ;

	}

      switch(gst_ServerRunningInfo.mu32_ServerMode)
      {
        case E_SERVER_MODE_NON_BLOCK:
                  if((flags = fcntl(gst_ServerRunningInfo.ms32_RunningServerSockFd , F_GETFL, 0)) == -1)
                  {
                        printf("3. get flags error\n");
                        close(gst_ServerRunningInfo.ms32_RunningServerSockFd );
                        gst_ServerRunningInfo.ms32_RunningServerSockFd = -1;
                        return ;
                  }

                  /* Update flag to make the file descriptor non-blocking */
                  flags |= O_NONBLOCK;

                  /* Set the updated flag */
                  if(fcntl(gst_ServerRunningInfo.ms32_RunningServerSockFd, F_SETFL, flags) == -1)
                  {
                        printf("\r\n 4:: set flags error\n");
                        close(gst_ServerRunningInfo.ms32_RunningServerSockFd);
                        return ;
                  }
            break;
        case E_SERVER_MODE_BLOCK:
            break;
      }

      printf("Open_Tcp_Server:Listen \r\n");
    //  struct sockaddr_in sock_addr;
      ret = listen(gst_ServerRunningInfo.ms32_RunningServerSockFd, 3);
      if (ret < 0)
      {
          printf("\r\n Open_Tcp_Server:Listen failed");
          close(gst_ServerRunningInfo.ms32_RunningServerSockFd);
          gst_ServerRunningInfo.ms32_RunningServerSockFd = -1;
          return ;
      }

      mailbox_Statemachine_Change_Req(TCP_SERVER_MODULE,TCP_SERVER_MODULE,E_TCP_SERVER_ACCEPT_STATE,0,0);

}


void Ready_To_Accept_Server_Connection(st_gMsgQueueEntry_t *lstp_Msg)
{
      u8 lu8_Index = 0;
      socklen_t addr_len;
      static u8 slu8_ListenorAccepState = 0;
      int ret;
      struct hostent *hostp; /* client host info */
      char *hostaddrp; /* dotted decimal host addr string */
      if(sst_TcpServerCurrentStateInfo.mu8_CurrentState != sst_TcpServerCurrentStateInfo.mu8_PreviousState)
      {
          slu8_ListenorAccepState =2;
      }
      switch(slu8_ListenorAccepState)
      {
        case 1:
              slu8_ListenorAccepState = 2;
          break;
        case 2:
                for(lu8_Index=0;lu8_Index < U8_MAX_SERVERCONNECTION_COUNT;lu8_Index ++)
                {
                  if(gst_ServerRunningInfo.ms32_RunningServerClientSockFd[lu8_Index] == 0xff)
                  {
                     break;
                  }
                }
              gst_ServerRunningInfo.ms32_RunningServerClientSockFd[lu8_Index] = accept(gst_ServerRunningInfo.ms32_RunningServerSockFd, (struct sockaddr *)&gst_ServerRunningInfo.mstar_ClientSock[lu8_Index], &addr_len);
              if (gst_ServerRunningInfo.ms32_RunningServerClientSockFd[lu8_Index] < 0)
              {
                printf("\r\n Open_Tcp_Server:accept failed");
                gst_ServerRunningInfo.ms32_RunningServerClientSockFd[lu8_Index]  = 0xff;
                return ;
              }
              printf("Server Accepted on client :fd is (%d) index(%d)",gst_ServerRunningInfo.ms32_RunningServerClientSockFd[lu8_Index],lu8_Index);

              /*
               * gethostbyaddr: determine who sent the message
               */
            /*  hostp = gethostbyaddr((const char *)&gst_ServerRunningInfo.mstar_ClientSock[lu8_Index].sin_addr.s_addr,
          			  sizeof(gst_ServerRunningInfo.mstar_ClientSock[lu8_Index].sin_addr.s_addr), AF_INET);
              if (hostp == NULL)
                printf("\r\nERROR on gethostbyaddr");
              hostaddrp = inet_ntoa(gst_ServerRunningInfo.mstar_ClientSock[lu8_Index].sin_addr);
              if (hostaddrp == NULL)
                printf("\r\n ERROR on inet_ntoa\n");
              printf("\r\nserver established connection with %s (%s)\n",
          	   hostp->h_name, hostaddrp); */
              slu8_ListenorAccepState = 3;
          break;
        case 3:
        printf("\r\n 777777777777777777");
          mailbox_Statemachine_Change_Req(TCP_SERVER_MODULE,TCP_SERVER_MODULE,E_TCP_SERVER_LISTEN_STATE,0,0);
          break;
        default:
          break;
      }
}


void Data_Readng_FromSocks(st_gMsgQueueEntry_t *lstp_Msg)
{
  u8 lu8_Index = 0;
  u16 lu16_NoOfByteRead  = 0;
  if(sst_TcpServerCurrentStateInfo.mu8_CurrentState != sst_TcpServerCurrentStateInfo.mu8_PreviousState)
  {
      ;
  }
  printf("\r\nData_Readng_FromSocks: Reading from soc");
  for(lu8_Index=0;lu8_Index < U8_MAX_SERVERCONNECTION_COUNT;lu8_Index ++)
  {
    if(gst_ServerRunningInfo.ms32_RunningServerClientSockFd[lu8_Index] != 0xff)
    {
        lu16_NoOfByteRead =read(gst_ServerRunningInfo.ms32_RunningServerClientSockFd[lu8_Index],gst_ServerBufferInfo[lu8_Index].mu8ar_ReceivedBuffer,U8_MAX_SERVER_RECEIVE_BUFF);
        if(lu16_NoOfByteRead < 0)
        {
          printf("\r\n Read Fail on Fd :%d ,Index :%d",gst_ServerRunningInfo.ms32_RunningServerClientSockFd[lu8_Index],lu8_Index);
          continue;
        }
        mailbox_Statemachine_Change_Req(TCP_SERVER_MODULE,TCP_SERVER_MODULE,E_TCP_SERVER_PROCESS_STATE,0,0);
      //  printf("\r\nRading the packet from external,%s\n",(char *)gst_ServerBufferInfo[lu8_Index].mu8ar_ReceivedBuffer);
      //  write(gst_ServerRunningInfo.ms32_RunningServerClientSockFd[lu8_Index],gst_ServerBufferInfo[lu8_Index].mu8ar_TxBuffer,U8_MAX_SERVER_RECEIVE_BUFF);
        //close(gst_ServerRunningInfo.ms32_RunningServerClientSockFd[lu8_Index]);
      //  gst_ServerRunningInfo.ms32_RunningServerClientSockFd[lu8_Index] = 0xff;
   }
  }
}

void Data_Process_State(st_gMsgQueueEntry_t *lstp_Msg)
{
  u8 lu8_Index = 0;
  u16 lu16_NoOfByteRead  = 0;
  if(sst_TcpServerCurrentStateInfo.mu8_CurrentState != sst_TcpServerCurrentStateInfo.mu8_PreviousState)
  {
      ;
  }

  for(lu8_Index=0;lu8_Index < U8_MAX_SERVERCONNECTION_COUNT;lu8_Index ++)
  {
    if(gst_ServerRunningInfo.ms32_RunningServerClientSockFd[lu8_Index] != 0xff)
    {
        printf("\r\nRading the packet from external,%s\n",(char *)gst_ServerBufferInfo[lu8_Index].mu8ar_ReceivedBuffer);
   }
  }
  mailbox_Statemachine_Change_Req(TCP_SERVER_MODULE,TCP_SERVER_MODULE,E_TCP_SERVER_RESPONSE_STATE,0,0);
}


void Data_Response_state(st_gMsgQueueEntry_t *lstp_Msg)
{
  u8 lu8_Index = 0;
  u16 lu16_NoOfByteRead  = 0;
  if(sst_TcpServerCurrentStateInfo.mu8_CurrentState != sst_TcpServerCurrentStateInfo.mu8_PreviousState)
  {
      ;
  }
  for(lu8_Index=0;lu8_Index < U8_MAX_SERVERCONNECTION_COUNT;lu8_Index ++)
  {
    if(gst_ServerRunningInfo.ms32_RunningServerClientSockFd[lu8_Index] != 0xff)
    {
        write(gst_ServerRunningInfo.ms32_RunningServerClientSockFd[lu8_Index],gst_ServerBufferInfo[lu8_Index].mu8ar_TxBuffer,U8_MAX_SERVER_RECEIVE_BUFF);
        close(gst_ServerRunningInfo.ms32_RunningServerClientSockFd[lu8_Index]);
        gst_ServerRunningInfo.ms32_RunningServerClientSockFd[lu8_Index] = 0xff;
    }
  }
mailbox_Statemachine_Change_Req(TCP_SERVER_MODULE,TCP_SERVER_MODULE,E_TCP_SERVER_LISTEN_STATE,0,0);
}


 static void TCP_Server_Connection(void *pvParameters)
 {
   st_gMsgQueueEntry_t lst_Message;
   u32 lu32_msgcount = 0;

   printf("\r\n TCP_Server_Connection:Created");

    while(1)
    {
       printf("\r\n TCP_Server_Connection");
          if(E_API_SUCCESS == mailbox_Statemachine_Change_Accept(TCP_SERVER_MODULE,&lst_Message))
          {
              printf("\r\n TCP_Server_Connection:1\n");
              processStateTable(scstar_TcpServerStateTable,&sst_TcpServerCurrentStateInfo,&lst_Message);
              printf("\r\n TCP_Server_Connection:2\n");
              lu32_msgcount++;
              // Handle upto 5 messages only to allow other tasks. also to throttle.
              if ( lu32_msgcount < 5)
              {
                  continue;
              }

          }
          //No message , Loop the same state -- not needed in case of no self loop SM
          else if (NULL != sst_TcpServerCurrentStateInfo.g_MsgProcessingFunc)
          {
              lst_Message.mu8_MsgId = E_TCP_SERVER_MSG_EMPTY;
              sst_TcpServerCurrentStateInfo.g_MsgProcessingFunc(&lst_Message);
          }
          lu32_msgcount = 0;
          vTaskDelay(100);
    }
 }

 void TCP_Server_Process(en_ModActionReqType_t lst_ActionRequested, u32 lu32_Reason, st_MainTaskControlInfo_t *lst_TasKControlInfo)
 {


     switch(lst_ActionRequested)
     {
         case E_APP_INIT:
           		/*No control for Power Manager*/
           		lst_TasKControlInfo -> mod_HealthCheckFunc = NULL;
           		lst_TasKControlInfo -> mu32_RestartMsg = 0;
              memset(&sst_TcpServerCurrentStateInfo,0x00,sizeof(sst_TcpServerCurrentStateInfo));
              memset(&gst_ServerRunningInfo,0x00,sizeof(st_ServerRunningInfo_t));
              gst_ServerRunningInfo.mu32_ServerRunningPort = 54545;
              gst_ServerRunningInfo.ms32_RunningServerSockFd = -1;
              gst_ServerRunningInfo.mu32_ServerMode = E_SERVER_MODE_BLOCK ;//E_SERVER_MODE_NON_BLOCK;
             break;
         case E_APP_DEINIT:
             break;
         case E_APP_TASK_START:
         xTaskCreate(&TCP_Server_Connection, "TCP_Server_Connection", 9192, NULL, 5, NULL);
       	      printf("TCP_Server_Connection:created Succesfully\n");
             break;
         default:
         /* Do Nothing */
             break;
     }
 }
