
#include "Tcp_Client_pub.h"
#include "Tcp_Client_pri.h"
#include "global.h"
#include "mailbox_pub.h"

st_TCPClientRunningInfo_t gst_TCPClientRunningInfo;

static char gs8ar_DataWriteBuffer[20];
static char gs8ar_DataReadBuffer[20];

static st_gStateStatus_t sst_TcpClientCurrentStateInfo;
static const st_gStateTable_t scstar_TcpClientStateTable[] =
{
    { E_TCP_CLIENT_IDLE_ST,    E_WIFI_MODULE,   E_TCP_CLIENT_OPEN_SOCKFD,           E_TCP_CLIENT_SOCKFD_ST,     Tcp_Client_SockFd_Open },

    { E_TCP_CLIENT_SOCKFD_ST,  TCP_CLIENT_MODULE,   E_TCP_CLIENT_CONNECT_TO_SERVER,        E_TCP_CLIENT_CONNECT_ST,   Tcp_Client_Connect },

    { E_TCP_CLIENT_CONNECT_ST,    TCP_CLIENT_MODULE,   E_TCP_CLIENT_WRITE,        E_TCP_CLIENT_WRITE_ST,   Tcp_Client_WriteToServer },

    { E_TCP_CLIENT_WRITE_ST,  TCP_CLIENT_MODULE,   E_TCP_CLIENT_READ,        E_TCP_CLIENT_READ_ST,   Tcp_Client_ReadFromServer },

    { E_TCP_CLIENT_READ_ST,    TCP_CLIENT_MODULE,   E_TCP_CLIENT_PROCESS,        E_TCP_CLIENT_PROCESS_ST,   Tcp_Client_Process },

    {E_TCP_CLIENT_PROCESS_ST ,TCP_CLIENT_MODULE ,   E_TCP_CLIENT_WRITE,        E_TCP_CLIENT_WRITE_ST, Tcp_Client_WriteToServer},

    {E_TCP_CLIENT_PROCESS_ST ,TCP_CLIENT_MODULE ,   E_TCP_CLIENT_CLOSE,        E_TCP_CLIENT_IDLE_ST, Tcp_Client_Close},

    {E_TCP_CLIENT_PROCESS_ST ,TCP_CLIENT_MODULE ,   E_TCP_CLIENT_RESPONSE,        E_TCP_CLIENT_IDLE_ST, Tcp_Client_WriteToServer},

    { E_TCP_CLIENT_ANY_STATE_ST,    TCP_CLIENT_MODULE,   E_TCP_CLIENT_WRITE,     E_TCP_CLIENT_WRITE_ST,   Tcp_Client_WriteToServer },

    { E_TCP_CLIENT_ANY_STATE_ST,  TCP_CLIENT_MODULE,   E_TCP_CLIENT_READ,        E_TCP_CLIENT_READ_ST,   Tcp_Client_ReadFromServer },

    {E_TCP_CLIENT_INVALID_STATE_ST,    TCP_CLIENT_MODULE,   E_TCP_CLIENT_MSG_EMPTY,         E_TCP_CLIENT_SOCKFD_ST,    NULL  },
};



void Tcp_Client_SockFd_Open(st_gMsgQueueEntry_t *lstp_Msg)
{

  printf("\r\n Open_Tcp_Client_SockFd:Opening\n");
  if(sst_TcpClientCurrentStateInfo.mu8_CurrentState != sst_TcpClientCurrentStateInfo.mu8_PreviousState)
  {
      ;
  }

    gst_TCPClientRunningInfo.ms32_TCPClientSockFd = socket(AF_INET, SOCK_STREAM, 0);
    printf("\r\n Open_Tcp_Client_SockFd:sockfd:%d\n",gst_TCPClientRunningInfo.ms32_TCPClientSockFd );
  	if (gst_TCPClientRunningInfo.ms32_TCPClientSockFd  < 0)
  	{
         	   printf("\r\nOpen_Tcp_Client_SockFd:Creating the socket failed ,port(%d)",gst_TCPClientRunningInfo.mu32_ClientPortNo);
  		       return;
    }
    mailbox_Statemachine_Change_Req(TCP_CLIENT_MODULE,TCP_CLIENT_MODULE,E_TCP_CLIENT_CONNECT_TO_SERVER,0,0);
}


void Tcp_Client_Connect(st_gMsgQueueEntry_t *lstp_Msg)
{

  printf("\r\n Tcp_Client_Connect\n");
  if(sst_TcpClientCurrentStateInfo.mu8_CurrentState != sst_TcpClientCurrentStateInfo.mu8_PreviousState)
  {
      ;
  }

		memset(&gst_TCPClientRunningInfo.mst_sock_addr, '0', sizeof(gst_TCPClientRunningInfo.mst_sock_addr));
		gst_TCPClientRunningInfo.mst_sock_addr.sin_family = AF_INET;
		gst_TCPClientRunningInfo.mst_sock_addr.sin_port = htons(gst_TCPClientRunningInfo.mu32_ClientPortNo);

		/*
			Convert IPv4 and IPv6 addresses from text to binary form
		*/
		if(inet_pton(AF_INET, gst_TCPClientRunningInfo.ms8_TcpCliHostByName , &gst_TCPClientRunningInfo.mst_sock_addr.sin_addr)<=0)
		{
			printf("\r\nOpen_Tcp_Client:Invalid address/ Address not supported");
			return ;
		}

		/*
			connect: create a connection with the server
		*/
		if (connect(gst_TCPClientRunningInfo.ms32_TCPClientSockFd, &gst_TCPClientRunningInfo.mst_sock_addr, sizeof(gst_TCPClientRunningInfo.mst_sock_addr)) < 0)
		{
			printf("Open_Tcp_Client:connect failed\n");
			return ;
		}
    mailbox_Statemachine_Change_Req(TCP_CLIENT_MODULE,TCP_CLIENT_MODULE,E_TCP_CLIENT_WRITE,0,0);

  }

void Tcp_Client_WriteToServer(st_gMsgQueueEntry_t *lstp_Msg)
{
    u16 lu16_dataLength = 0;
    u32 lu32_NoOfBytesWritten;

    printf("\r\n Tcp_Client_Connect\n");
    if(sst_TcpClientCurrentStateInfo.mu8_CurrentState != sst_TcpClientCurrentStateInfo.mu8_PreviousState)
    {
        ;
    }

        lu32_NoOfBytesWritten =  send(gst_TCPClientRunningInfo.ms32_TCPClientSockFd, gs8ar_DataWriteBuffer, lu16_dataLength, 0);
        if (lu32_NoOfBytesWritten < 0)
        {
          printf("Open_Tcp_Client:writing to server failed\r\n");
          return ;
        }
        mailbox_Statemachine_Change_Req(TCP_CLIENT_MODULE,TCP_CLIENT_MODULE,E_TCP_CLIENT_READ,0,0);

 }

 void Tcp_Client_ReadFromServer(st_gMsgQueueEntry_t *lstp_Msg)
 {
     u32 lu32_NoOfBytesRcv;

     printf("\r\n Tcp_Client_Connect\n");
     if(sst_TcpClientCurrentStateInfo.mu8_CurrentState != sst_TcpClientCurrentStateInfo.mu8_PreviousState)
     {
         ;
     }
         lu32_NoOfBytesRcv = recv(gst_TCPClientRunningInfo.ms32_TCPClientSockFd, gs8ar_DataReadBuffer, 20, 0);
         if (lu32_NoOfBytesRcv < 0)
         {
           printf("Open_Tcp_Client:Reading from server failed\r\n");
           return ;
         }
         mailbox_Statemachine_Change_Req(TCP_CLIENT_MODULE,TCP_CLIENT_MODULE,E_TCP_CLIENT_PROCESS,0,0);
  }

  void Tcp_Client_Process(st_gMsgQueueEntry_t *lstp_Msg)
  {

      printf("\r\n Tcp_Client_Connect\n");
      if(sst_TcpClientCurrentStateInfo.mu8_CurrentState != sst_TcpClientCurrentStateInfo.mu8_PreviousState)
      {
          ;
      }
      printf("\r\n received Data is:%s",gs8ar_DataReadBuffer);
      mailbox_Statemachine_Change_Req(TCP_CLIENT_MODULE,TCP_CLIENT_MODULE,E_TCP_CLIENT_CLOSE,0,0);
    }


    void Tcp_Client_Close(st_gMsgQueueEntry_t *lstp_Msg)
    {
        printf("\r\n Tcp_Client_Connect\n");
        if(sst_TcpClientCurrentStateInfo.mu8_CurrentState != sst_TcpClientCurrentStateInfo.mu8_PreviousState)
        {
            ;
        }
        close(gst_TCPClientRunningInfo.ms32_TCPClientSockFd);
    }




/***********************************************************************************************/

 static void TCP_Client_Connection(void *pvParameters)
 {
   st_gMsgQueueEntry_t lst_Message;
   u32 lu32_msgcount = 0;

   printf("\r\n TCP_Client_Connection:Created");

    while(1)
    {
       printf("\r\n TCP_Client_Connection");
          if(E_API_SUCCESS == mailbox_Statemachine_Change_Accept(TCP_CLIENT_MODULE,&lst_Message))
          {
              printf("\r\n TCP_Client_Connection:1\n");
              processStateTable(scstar_TcpClientStateTable,&sst_TcpClientCurrentStateInfo,&lst_Message);
              printf("\r\n TCP_Client_Connection:2\n");
              lu32_msgcount++;
              // Handle upto 5 messages only to allow other tasks. also to throttle.
              if ( lu32_msgcount < 5)
              {
                  continue;
              }

          }
          //No message , Loop the same state -- not needed in case of no self loop SM
          else if (NULL != sst_TcpClientCurrentStateInfo.g_MsgProcessingFunc)
          {
              lst_Message.mu8_MsgId = E_TCP_CLIENT_MSG_EMPTY;
              sst_TcpClientCurrentStateInfo.g_MsgProcessingFunc(&lst_Message);
          }
          lu32_msgcount = 0;
          vTaskDelay(100);
    }
 }

 void TCP_Client_Process(en_ModActionReqType_t lst_ActionRequested, u32 lu32_Reason, st_MainTaskControlInfo_t *lst_TasKControlInfo)
 {


     switch(lst_ActionRequested)
     {
         case E_APP_INIT:
           		/*No control for Power Manager*/
           		lst_TasKControlInfo -> mod_HealthCheckFunc = NULL;
           		lst_TasKControlInfo -> mu32_RestartMsg = 0;
              memset(&sst_TcpClientCurrentStateInfo,0x00,sizeof(sst_TcpClientCurrentStateInfo));
              memset(&gst_TCPClientRunningInfo,0x00,sizeof(gst_TCPClientRunningInfo));
              gst_TCPClientRunningInfo.mu32_ClientPortNo = TCP_CLIENT_PORT_NO;
              memcpy(gst_TCPClientRunningInfo.ms8_TcpCliHostByName,TCP_CLIENT_SERVERIP,sizeof(TCP_CLIENT_SERVERIP));
             break;
         case E_APP_DEINIT:
             break;
         case E_APP_TASK_START:
         xTaskCreate(&TCP_Client_Connection, "TCP_Client_Connection", 4096, NULL, 5, NULL);
       	      printf("TCP_Client_Connection:created Succesfully\n");
             break;
         default:
         /* Do Nothing */
             break;
     }
 }
