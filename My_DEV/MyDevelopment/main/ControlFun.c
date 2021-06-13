#include "ControlFun.h"
#include "Wifi_Dev.h"
#include "global.h"
#include "Connection_Pub.h"

#include "TCP_Server_pub.h"
#include "UDP_Server_pub.h"
#include "Tcp_Client_pub.h"
#include "WIFI_pub.h"

st_pwrTaskControllingFunc_t const scstar_pwrTaskControllingTable[] =
{
    {WiFI_Process,    E_WIFI_TASK_IDLE   },
    {TCP_Server_Process,    E_TCP_SERVER_IDLE   },
    {TCP_Client_Process,E_TCP_CLIENT_IDLE},
  {UDP_Server_Process,E_UDP_SERVER_IDLE},
};

st_MainTaskControlInfo_t gstar_MainTaskControllingInfo[5];


/**************************************************************************
* Function Name :   pwr_ControlFunc
 *
* Description   :   Initialization of power manager variables and structures
 *
* Arguments     :
*
* Return values :   None
 *
 ****************************************************************************/




void execute_Control_Fun(void)
{
  u8 lu8_TaskIndex;
  for (lu8_TaskIndex = E_WIFI_MODULE; lu8_TaskIndex < E_MAX_MODULE;  lu8_TaskIndex ++)
  {
      if(NULL != scstar_pwrTaskControllingTable[lu8_TaskIndex].mod_ControlFunc)
      {
          scstar_pwrTaskControllingTable[lu8_TaskIndex].mod_ControlFunc(E_APP_INIT, 0x00,&gstar_MainTaskControllingInfo[lu8_TaskIndex]);

      }
  }
  for (lu8_TaskIndex = E_WIFI_MODULE; lu8_TaskIndex < E_MAX_MODULE;  lu8_TaskIndex ++)
  {
      if(NULL != scstar_pwrTaskControllingTable[lu8_TaskIndex].mod_ControlFunc)
      {
          scstar_pwrTaskControllingTable[lu8_TaskIndex].mod_ControlFunc(E_APP_TASK_START, 0x00,&gstar_MainTaskControllingInfo[lu8_TaskIndex]);

      }
  }
}
