
#include "Wifi_Dev.h"
#include "WIFI_pub.h"
#include "WIFI_pri.h"
#include "global.h"
#include "mailbox_pub.h"
#include "TCP_Server_pub.h"

static const char *TAG = "wifi station";

static st_gStateStatus_t sst_WiFiCurrentStateInfo;

typedef struct
{
  u8 mu8_IpState;
  ip_event_got_ip_t mst_ip_event_got_ip;
}st_IPInfo_State_t;

volatile st_IPInfo_State_t gst_IPInfo_State;

static const st_gStateTable_t scstar_WifiStateTable[] =
{
    { E_WIFI_IDLE_ST,    E_WIFI_MODULE,   E_WIFI_GO_ADHOC,           E_WIFI_ADHOC_ST,    WiF_Adhoc_Mode  },

    { E_WIFI_ADHOC_ST,  E_WIFI_MODULE,   E_WIFI_PROCESS,        E_WIFI_PROCESS_ST,   NULL /*Open_Tcp_Server_OpenServer*/  },

    { E_WIFI_PROCESS_ST,    E_WIFI_MODULE,   E_WIFI_GO_JOIN,        E_WIFI_JOIN_ST,   wifi_Join_Process },

    { E_WIFI_JOIN_ST,    E_WIFI_MODULE,   E_WIFI_GO_FOR_IP,        E_WIFI_GOT_IP_ST,   wifi_gotIp_State },

    { E_WIFI_GOT_IP_ST,  E_WIFI_MODULE,   E_WIFI_READY,        E_WIFI_READY_ST,   wifi_ReadyStateProcess },

    { E_WIFI_ANY_STATE,    E_WIFI_MODULE,   E_WIFI_GO_ADHOC,        E_WIFI_ADHOC_ST,   WiF_Adhoc_Mode },

    { E_WIFI_ANY_STATE,    E_WIFI_MODULE,   E_WIFI_GO_JOIN,     E_WIFI_JOIN_ST,   wifi_Join_Process },

    { E_WIFI_READY_ST,    E_WIFI_MODULE,   E_WIFI_GO_FOR_IP,     E_WIFI_GOT_IP_ST,   wifi_gotIp_State },

    {E_WIFI_INVALID,    E_WIFI_MODULE,   E_WIFI_GO_ADHOC,         E_WIFI_ADHOC_ST,    WiF_Adhoc_Mode  },

};

void Wifi_PreCheckingInfo(void)
{
  memset(&sst_WiFiCurrentStateInfo,0x00,sizeof(sst_WiFiCurrentStateInfo));
  sst_WiFiCurrentStateInfo.mu8_CurrentState = E_WIFI_ANY_STATE;
  mailbox_Statemachine_Change_Req(E_WIFI_MODULE,E_WIFI_MODULE,E_WIFI_GO_JOIN,0,0);
}

 static void WIFI_ConnectionProcess(void *pvParameters)
 {
   st_gMsgQueueEntry_t lst_Message;
   u32 lu32_msgcount = 0;

    while(1)
    {
          if(E_API_SUCCESS == mailbox_Statemachine_Change_Accept(E_WIFI_MODULE,&lst_Message))
          {
                processStateTable(scstar_WifiStateTable,&sst_WiFiCurrentStateInfo,&lst_Message);
                lu32_msgcount++;
                // Handle upto 5 messages only to allow other tasks. also to throttle.
                if ( lu32_msgcount < 5)
                {
                    continue;
                }

            }
            //No message , Loop the same state -- not needed in case of no self loop SM
            else if (NULL != sst_WiFiCurrentStateInfo.g_MsgProcessingFunc)
            {
                lst_Message.mu8_MsgId = E_WIFI_MSG_EMPTY;
                sst_WiFiCurrentStateInfo.g_MsgProcessingFunc(&lst_Message);
            }
            lu32_msgcount = 0;
            vTaskDelay(100);
    }
 }

 void WiFI_Process(en_ModActionReqType_t lst_ActionRequested, u32 lu32_Reason, st_MainTaskControlInfo_t *lst_TasKControlInfo)
 {

     switch(lst_ActionRequested)
     {
         case E_APP_INIT:
 		/*No control for Power Manager*/
 		lst_TasKControlInfo -> mod_HealthCheckFunc = NULL;
 		lst_TasKControlInfo -> mu32_RestartMsg = 0;
    printf("\nWIFI INI STATE");

             break;
         case E_APP_DEINIT:

             break;
         case E_APP_TASK_START:
                 printf("\nWIFI INI STATE");
                 xTaskCreate(&WIFI_ConnectionProcess, "WIFI_ConnectionProcess", 4096, NULL, 5, NULL);
               	      printf("WIFI_ConnectionProcess:created Succesfully\n");

                // wifi_Join_Process();
             break;
         default:
         /* Do Nothing */
             break;
     }
 }


static void Wifi_event_Connect_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
      switch(event_id)
      {
          case WIFI_EVENT_WIFI_READY:
              break;
          case WIFI_EVENT_SCAN_DONE:
              break;
          case WIFI_EVENT_STA_START:
                gst_IPInfo_State.mu8_IpState = 0;
                esp_wifi_connect();
              break;
          case WIFI_EVENT_STA_STOP:
              break;
          case WIFI_EVENT_STA_CONNECTED:
              break;
          case WIFI_EVENT_STA_DISCONNECTED:
                  gst_IPInfo_State.mu8_IpState = 0;
                  esp_wifi_connect();
              break;
          case WIFI_EVENT_STA_AUTHMODE_CHANGE:
              break;
          case WIFI_EVENT_STA_WPS_ER_SUCCESS:
          case WIFI_EVENT_STA_WPS_ER_FAILED:
          case WIFI_EVENT_STA_WPS_ER_TIMEOUT:
          case WIFI_EVENT_STA_WPS_ER_PIN:
          case WIFI_EVENT_STA_WPS_ER_PBC_OVERLAP:
          case WIFI_EVENT_AP_START:
          case WIFI_EVENT_AP_STOP:
          case WIFI_EVENT_AP_STACONNECTED:
          case WIFI_EVENT_AP_STADISCONNECTED:
          case WIFI_EVENT_AP_PROBEREQRECVED:
              break;
          default:
              break;
        }
}

static void wifi_event_IP_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    ip_event_got_ip_t* event = NULL;
    switch(event_id)
    {
      case IP_EVENT_STA_GOT_IP:
             event = (ip_event_got_ip_t*) event_data;
            ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
              gst_IPInfo_State.mu8_IpState = 1;
              memcpy(&gst_IPInfo_State.mst_ip_event_got_ip,event_data,sizeof(gst_IPInfo_State.mst_ip_event_got_ip));
          break;
      case IP_EVENT_STA_LOST_IP:
        gst_IPInfo_State.mu8_IpState = 0;
        memcpy(&gst_IPInfo_State.mst_ip_event_got_ip,event_data,sizeof(gst_IPInfo_State.mst_ip_event_got_ip));
          break;
      case IP_EVENT_AP_STAIPASSIGNED:
          break;
      case IP_EVENT_GOT_IP6:
          break;
      case IP_EVENT_ETH_GOT_IP:
            break;
     default:
          break;
    }
}
void WiF_Adhoc_Mode(st_gMsgQueueEntry_t *lstp_Msg)
{

  if(sst_WiFiCurrentStateInfo.mu8_CurrentState != sst_WiFiCurrentStateInfo.mu8_PreviousState)
	{
      ;
	}
  else
  {
    return;
  }

	printf("WiF_Adhoc_Mode:Configuring the adhoc mode\n");
    	wifi_config_t wifi_adhoc_config = {
					.ap = {
						.ssid = "ADHOC_SSID",
						.password = ADHOC_PWD,
						.ssid_len = ADHOC_SSID_LEN,
						.channel  = ADHOC_CHANNEL,
						.authmode = ADHOC_AUTHMODE,
						.ssid_hidden = 0,
						.max_connection = 1,
						.beacon_interval = 100
					},
   			 };
	ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_adhoc_config));
	ESP_ERROR_CHECK(esp_wifi_start());
	printf("WiF_Adhoc_Mode:Configured success\n");
}


void wifi_Join_Process(st_gMsgQueueEntry_t *lstp_Msg)
{
  printf("\r\n wifi_Join_Process");
    if(sst_WiFiCurrentStateInfo.mu8_CurrentState != sst_WiFiCurrentStateInfo.mu8_PreviousState)
  	{
        ;
  	}
    esp_netif_init();
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));


    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &Wifi_event_Connect_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, ESP_EVENT_ANY_ID, &wifi_event_IP_handler, NULL));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .password = EXAMPLE_ESP_WIFI_PASS
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start() );

    ESP_LOGI(TAG, "wifi_Join_Process finished.");
    ESP_LOGI(TAG, "connect to ap SSID:%s password:%s",
             EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    mailbox_Statemachine_Change_Req(E_WIFI_MODULE,E_WIFI_MODULE,E_WIFI_GO_FOR_IP,0,0);
}


void wifi_gotIp_State(st_gMsgQueueEntry_t *lstp_Msg)
{
  printf("\r\n wifi_gotIp_State");
    if(sst_WiFiCurrentStateInfo.mu8_CurrentState != sst_WiFiCurrentStateInfo.mu8_PreviousState)
    {
        ;
    }
   if(gst_IPInfo_State.mu8_IpState == 1)
   {
      printf("wifi_gotIp_State:Got IP State and moving to Ready State");
      mailbox_Statemachine_Change_Req(E_WIFI_MODULE,E_WIFI_MODULE,E_WIFI_READY,0,0);
   }
}

void wifi_ReadyStateProcess(st_gMsgQueueEntry_t *lstp_Msg)
{
  static u8 slu8_SateMachine = 0;

    printf("\r\n wifi_ReadyStateProcess:slu8_SateMachine:%u",slu8_SateMachine);
    if(sst_WiFiCurrentStateInfo.mu8_CurrentState != sst_WiFiCurrentStateInfo.mu8_PreviousState)
  	{
        printf("\r\n wifi_ReadyStateProcess:------------------------------------------");
        slu8_SateMachine = 1;
  	}
    switch(slu8_SateMachine)
    {
      case 1:
            printf("\r\n wifi_ReadyStateProcess:Trying to open TCP server");
            mailbox_Statemachine_Change_Req(E_WIFI_MODULE,TCP_SERVER_MODULE,E_TCP_SERVER_OPEN_SOCKFD,0,0);
            slu8_SateMachine = 2;
        break;
      case 2:
          if(gst_IPInfo_State.mu8_IpState == 0)
          {
             printf("wifi_ReadyStateProcess:Lost the IP address");
             mailbox_Statemachine_Change_Req(E_WIFI_MODULE,E_WIFI_MODULE,E_WIFI_GO_FOR_IP,0,0);
          }
        break;
      case 3:
          break;
      default:
          break;
    }
}
