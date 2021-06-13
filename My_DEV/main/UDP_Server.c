#include "UDP_Server_pub.h"
#include "UDP_Server_pri.h"
#include "global.h"
#include "mailbox_pub.h"

st_UDPServerInfo_t gst_UDPServerInfo;
static char ss8ar_UdpDataRecBuf[U16_UDP_BUFFER_LEN];
static char ss8ar_UdpDataTxBuf[U16_UDP_BUFFER_LEN];

static st_gStateStatus_t sst_UDPSerCur_StateInfo;
static const st_gStateTable_t scstar_UDPSer_StateTable[] =
{
    { E_UDP_SERVER_IDLE_ST,    E_WIFI_MODULE,   E_UDP_SERVER_OPEN_SOCKFD,           E_UDP_SERVER_SOCKFD_ST,     UDP_Server_SockFd_Open },

    { E_UDP_SERVER_SOCKFD_ST,  UDP_SERVER_MODULE,   E_UDP_SERVER_OPEN_SERVER,        E_UDP_SERVER_OPEN_ST,   UDP_Server_Bind },

    { E_UDP_SERVER_OPEN_ST,    UDP_SERVER_MODULE,   E_UDP_SERVER_LISTEN_STATE,        E_UDP_SERVER_READ_ST,   UDP_Server_Listen },

    { E_UDP_SERVER_READ_ST,  UDP_SERVER_MODULE,   E_UDP_SERVER_PROCESS_STATE,        E_UDP_SERVER_PROCESS_ST,   UDP_Server_Data_Process },

    { E_UDP_SERVER_PROCESS_ST,    UDP_SERVER_MODULE,   E_UDP_SERVER_RESPONSE_STATE,        E_UDP_SERVER_WRITE_ST,   UDP_Server_Send },

    {E_UDP_SERVER_PROCESS_ST ,UDP_SERVER_MODULE ,   E_UDP_SERVER_CLOSE_STATE,        E_UDP_SERVER_CLOSE_ST, UDP_Server_Close },

    { E_UDP_SERVER_READ_ST,    UDP_SERVER_MODULE,   E_UDP_SERVER_CLOSE_STATE,     E_UDP_SERVER_CLOSE_ST,   UDP_Server_Close },

    { E_UDP_SERVER_ANY_STATE_ST,  UDP_SERVER_MODULE,   E_UDP_SERVER_CLOSE_STATE,        E_UDP_SERVER_CLOSE_ST,   UDP_Server_Close },

    //{ E_UDP_SERVER_CLOSE_ST,  UDP_SERVER_MODULE,   E_TCP_SERVER_ACCEPT_STATE,        E_UDP_SERVER_IDLE_ST,   Ready_To_Accept_Server_Connection },

    {E_UDP_SERVER_INVALID_STATE_ST,    UDP_SERVER_MODULE,   E_UDP_SERVER_OPEN_SOCKFD,         E_UDP_SERVER_SOCKFD_ST,    NULL  },
};


void UDP_Server_SockFd_Open(st_gMsgQueueEntry_t *lstp_Msg)
{

  printf("\r\n UDP_Server_SockFd_Open:Opening\n");
  if(sst_UDPSerCur_StateInfo.mu8_CurrentState != sst_UDPSerCur_StateInfo.mu8_PreviousState)
  {
      ;
  }

    gst_UDPServerInfo.ms32_ServerSockFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    printf("\r\n UDP_Server_SockFd_Open:sockfd:%d\n",gst_UDPServerInfo.ms32_ServerSockFd );
  	if (gst_UDPServerInfo.ms32_ServerSockFd  < 0)
  	{
         	   printf("UDP_Server_SockFd_Open:Creating the socket failed ,port(%d)\r\n",gst_UDPServerInfo.mu32_ServerPortNo);
  		       return;
    }
    printf("\r\n UDP_Server_SockFd_Open:Expecting to next leel 1\n");
    mailbox_Statemachine_Change_Req(UDP_SERVER_MODULE,UDP_SERVER_MODULE,E_UDP_SERVER_OPEN_SERVER,0,0);
}




void UDP_Server_Bind(st_gMsgQueueEntry_t *lstp_Msg)
{
 u32 flags = 0;
    printf("\r\n UDP_Server_Bind:Opening\n");
    if(sst_UDPSerCur_StateInfo.mu8_CurrentState != sst_UDPSerCur_StateInfo.mu8_PreviousState)
    {
        ;
    }
    if(gst_UDPServerInfo.ms32_ServerSockFd < 0)
    {
      printf("\r\n UDP_Server_Bind:Invalid socfd");
    }


  gst_UDPServerInfo.mst_Ser_addr.sin_family = AF_INET;
  gst_UDPServerInfo.mst_Ser_addr.sin_port = htons(gst_UDPServerInfo.mu32_ServerPortNo);
  gst_UDPServerInfo.mst_Ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  memset(gst_UDPServerInfo.mst_Ser_addr.sin_zero, '\0', sizeof(gst_UDPServerInfo.mst_Ser_addr.sin_zero));

  /*
    Bind socket with address struct
  */
  if (bind(gst_UDPServerInfo.ms32_ServerSockFd, (struct sockaddr *) &gst_UDPServerInfo.mst_Ser_addr, sizeof(gst_UDPServerInfo.mst_Ser_addr))<0)
  {
    printf("UDP_Server_Bind:BINDING failed\r\n");
    close(gst_UDPServerInfo.ms32_ServerSockFd);
    gst_UDPServerInfo.ms32_ServerSockFd = -1;
    return ;
  }

    switch(gst_UDPServerInfo.mu32_ServerMode)
    {
      case E_SERVER_MODE_BLOCK:
        break;
      case E_SERVER_MODE_NON_BLOCK:
              if((flags = fcntl(gst_UDPServerInfo.ms32_ServerSockFd, F_GETFL, 0)) < 0)
              {
                printf("\r\nUDP_Server_Bind:fcntl:get flags error\n");
                close(gst_UDPServerInfo.ms32_ServerSockFd);
                gst_UDPServerInfo.ms32_ServerSockFd = -1;
                return ;

              }
              flags |= O_NONBLOCK;
              if(fcntl(gst_UDPServerInfo.ms32_ServerSockFd, F_SETFL, flags) < 0)
              {
                printf("\r\nUDP_Server_Bind:fcntl:set flags error\n");
                close(gst_UDPServerInfo.ms32_ServerSockFd);
                gst_UDPServerInfo.ms32_ServerSockFd = -1;
                return ;
              }
        break;
      default:
        break;
    }
    mailbox_Statemachine_Change_Req(UDP_SERVER_MODULE,UDP_SERVER_MODULE,E_UDP_SERVER_LISTEN_STATE,0,0);
}

void UDP_Server_Listen(st_gMsgQueueEntry_t *lstp_Msg)
{
    u32 lu32_NoOfRecv = 0;
    printf("\r\n UDP_Server_Bind:Opening\n");
    if(sst_UDPSerCur_StateInfo.mu8_CurrentState != sst_UDPSerCur_StateInfo.mu8_PreviousState)
    {
        ;
    }
    if(gst_UDPServerInfo.ms32_ServerSockFd < 0)
    {
      printf("\r\n UDP_Server_Bind:Invalid socfd");
    }

    lu32_NoOfRecv = recvfrom(gst_UDPServerInfo.ms32_ServerSockFd ,ss8ar_UdpDataRecBuf,U16_UDP_BUFFER_LEN,0,(struct sockaddr *)&gst_UDPServerInfo.mstar_Client_addr, &gst_UDPServerInfo.mu32_Cli_AddrSize);;
    if(lu32_NoOfRecv < 0)
    {
      return;
    }
    mailbox_Statemachine_Change_Req(UDP_SERVER_MODULE,UDP_SERVER_MODULE,E_UDP_SERVER_PROCESS_STATE,0,0);
}

void UDP_Server_Data_Process(st_gMsgQueueEntry_t *lstp_Msg)
{
    u32 lu32_NoOfRecv = 0;
    printf("\r\n UDP_Server_Bind:Opening\n");
    if(sst_UDPSerCur_StateInfo.mu8_CurrentState != sst_UDPSerCur_StateInfo.mu8_PreviousState)
    {
        ;
    }
    if(gst_UDPServerInfo.ms32_ServerSockFd < 0)
    {
      printf("\r\n UDP_Server_Bind:Invalid socfd");
    }
    printf("UDP_Server_Process:%s",ss8ar_UdpDataRecBuf);
    mailbox_Statemachine_Change_Req(UDP_SERVER_MODULE,UDP_SERVER_MODULE,E_UDP_SERVER_RESPONSE_STATE,0,0);
}


void UDP_Server_Send(st_gMsgQueueEntry_t *lstp_Msg)
{
    u32 lu32_BytesToWrite = 0;
    u32 nBytes = 0;
    printf("\r\n UDP_Server_Send:Opening\n");
    if(sst_UDPSerCur_StateInfo.mu8_CurrentState != sst_UDPSerCur_StateInfo.mu8_PreviousState)
    {
        ;
    }
    if(gst_UDPServerInfo.ms32_ServerSockFd < 0)
    {
      printf("\r\n UDP_Server_Send:Invalid socfd");
    }
    lu32_BytesToWrite = sendto(gst_UDPServerInfo.ms32_ServerSockFd ,ss8ar_UdpDataTxBuf,nBytes,0,(struct sockaddr *)&gst_UDPServerInfo.mstar_Client_addr,gst_UDPServerInfo.mu32_Cli_AddrSize);
    if(lu32_BytesToWrite < 0)
    {
      return;
    }

    mailbox_Statemachine_Change_Req(UDP_SERVER_MODULE,UDP_SERVER_MODULE,E_UDP_SERVER_CLOSE_STATE,0,0);
}

void UDP_Server_Close(st_gMsgQueueEntry_t *lstp_Msg)
{
  printf("\r\n UDP_Server_Close:Opening\n");
  if(sst_UDPSerCur_StateInfo.mu8_CurrentState != sst_UDPSerCur_StateInfo.mu8_PreviousState)
  {
      ;
  }
  if(gst_UDPServerInfo.ms32_ServerSockFd < 0)
  {
    printf("\r\n UDP_Server_Close:Invalid socfd");
  }
  close(gst_UDPServerInfo.ms32_ServerSockFd);
}


static void UDP_Server_Connection(void *pvParameters)
{
  st_gMsgQueueEntry_t lst_Message;
  u32 lu32_msgcount = 0;

  printf("\r\n UDP_Server_Connection:Created");

   while(1)
   {
      printf("\r\n UDP_Server_Connection");
         if(E_API_SUCCESS == mailbox_Statemachine_Change_Accept(UDP_SERVER_MODULE,&lst_Message))
         {
             printf("\r\n UDP_Server_Connection:1\n");
             processStateTable(scstar_UDPSer_StateTable,&sst_UDPSerCur_StateInfo,&lst_Message);
             printf("\r\n UDP_Server_Connection:2\n");
             lu32_msgcount++;
             // Handle upto 5 messages only to allow other tasks. also to throttle.
             if ( lu32_msgcount < 5)
             {
                 continue;
             }

         }
         //No message , Loop the same state -- not needed in case of no self loop SM
         else if (NULL != sst_UDPSerCur_StateInfo.g_MsgProcessingFunc)
         {
             lst_Message.mu8_MsgId = E_UDP_SERVER_MSG_EMPTY;
             sst_UDPSerCur_StateInfo.g_MsgProcessingFunc(&lst_Message);
         }
         lu32_msgcount = 0;
         vTaskDelay(100);
   }
}

void UDP_Server_Process(en_ModActionReqType_t lst_ActionRequested, u32 lu32_Reason, st_MainTaskControlInfo_t *lst_TasKControlInfo)
{

    switch(lst_ActionRequested)
    {
        case E_APP_INIT:
             /*No control for Power Manager*/
             lst_TasKControlInfo -> mod_HealthCheckFunc = NULL;
             lst_TasKControlInfo -> mu32_RestartMsg = 0;
             memset(&sst_UDPSerCur_StateInfo,0x00,sizeof(sst_UDPSerCur_StateInfo));
             memset(&gst_UDPServerInfo,0x00,sizeof(gst_UDPServerInfo));
             gst_UDPServerInfo.mu32_ServerPortNo = 54545;
             gst_UDPServerInfo.ms32_ServerSockFd = -1;
             gst_UDPServerInfo.mu32_ServerMode = E_SERVER_MODE_BLOCK ;//E_SERVER_MODE_NON_BLOCK;
            break;
        case E_APP_DEINIT:
            break;
        case E_APP_TASK_START:
        xTaskCreate(&UDP_Server_Connection, "UDP_Server_Connection", 4096, NULL, 5, NULL);
             printf("UDP_Server_Connection:created Succesfully\n");
            break;
        default:
        /* Do Nothing */
            break;
    }
}
