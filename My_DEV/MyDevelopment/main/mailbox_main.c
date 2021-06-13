/*****************************************************************************
|"2016 Danlaw Inc. All rights reserved."
|
| File              :   mainbox_main.c
| Created           :   26 May 2016
| Application       :   Reusable
| Functions         :
| Author            :
| Target            :   Gen 3 - QCM
| Environment       :   DS5 and Cygwin
*****************************************************************************/
/*****************************************************************************
| Change History:
| Version       Date        Person        Change Description
| 0.1           20 Jan 2016             Initial version
|
*****************************************************************************/


/*****************************************************************************
| Included files
*****************************************************************************/
#include "global.h"
#include "mailbox_main_pri.h"
#include <string.h>

/*****************************************************************************
| private varible
*****************************************************************************/
static st_mbQueue_t sst_mbQueue;
/**************************************************************************
 * Function Name	:	mb_Open
 *
 * Description		:	Initailize mail Box
 *
 * Arguments		:	en_mbOpenArgument_t
 *
 *
 *
 * Return values	:   APIStatus_t
 *
 ****************************************************************************/
 u8 mailBox_Init(void)
 {
	en_apierrorState_t lu8_Return = E_API_SUCCESS;
	//osi_InitCritSect(&sst_mbQueue.mst_Mutex);
  sst_mbQueue.mst_Mutex = xSemaphoreCreateMutex();
	memset(sst_mbQueue.mstar_Msg,0,sizeof(sst_mbQueue.mstar_Msg));
	sst_mbQueue.mu32_RuningSeqNo = 1;
	sst_mbQueue.mu8_ValidMsg = 0;
	lu8_Return = E_API_SUCCESS;
	return lu8_Return;
 }

/**************************************************************************
 * Function Name	:	mb_Post
 *
 * Description		:	This function to post message in message Quene
 *
 * Arguments		:	u8 lu8_ModuleId - Task ID
 *						st_gMsgQueueEntry_t * lstp_Msg - message
 *
 *
 * Return values	:   APIStatus_t
 *
 ****************************************************************************/
 u8 mailbox_Statemachine_Change_Req(en_gModules_t lu8_SrcId,en_gModules_t lu8_DestId,u8 lu8_MsgId,
 					u8 lu8_Priority,u32 lu32_Attribute)
 {
	u8 lu8_Index = 0;
	en_apierrorState_t len_ReturnValue = E_API_FAILED;
	st_gMsgQueueEntry_t lst_Msg;
	/*Fill local Buffer*/
	lst_Msg.mu8_SourceId = lu8_SrcId;
	lst_Msg.mu8_MsgId = lu8_MsgId;
	lst_Msg.mu8_PriorityId = lu8_Priority;
	lst_Msg.mu32_Attribute = lu32_Attribute;
	/*Enter Critical section*/
  xSemaphoreTake( sst_mbQueue.mst_Mutex, ( TickType_t ) 50);
	//osi_EnterCritSect(&sst_mbQueue.mst_Mutex);
	for(;lu8_Index < U8_MB_TOTAL_MSG_ENTRY;lu8_Index++) {
		if( sst_mbQueue.mstar_Msg[lu8_Index].mu32_SeqNo == 0 )
		{
			sst_mbQueue.mstar_Msg[lu8_Index].mu32_SeqNo =
			sst_mbQueue.mu32_RuningSeqNo;
			sst_mbQueue.mu32_RuningSeqNo++;
			if(sst_mbQueue.mu32_RuningSeqNo == U32_MB_MAX_SEQ_NO)
			{
				sst_mbQueue.mu32_RuningSeqNo = 1;
			}
			/*added to remove warning */
			sst_mbQueue.mstar_Msg[lu8_Index].men_SrcId = lu8_DestId;
			memcpy(&sst_mbQueue.mstar_Msg[lu8_Index].mst_Msg,&lst_Msg,sizeof(lst_Msg));
			sst_mbQueue.mu8_ValidMsg++;
			len_ReturnValue = E_API_SUCCESS;
			break;
		}
	}
	/*Leave Critical section*/
	//osi_LeaveCritSect(&sst_mbQueue.mst_Mutex);
  xSemaphoreGive( sst_mbQueue.mst_Mutex );
	return len_ReturnValue;
 }


 /**************************************************************************
 * Function Name	:	mb_Get
 *
 * Description		:	This function to Get message in message Quene
 *
 * Arguments		:	u8 lu8_ModuleId - Task ID
 *						st_gMsgQueueEntry_t * lstp_Msg - message
 *
 *
 * Return values	:   APIStatus_t
 *
 ****************************************************************************/
u8 mailbox_Statemachine_Change_Accept(en_gModules_t len_DestId,st_gMsgQueueEntry_t *lstp_Msg)
 {
	u8 lu8_Index = 0;
	u8 lu8_EntryIndex = 255;
	u32 lu32_MinSeqNo = U32_MB_MAX_SEQ_NO;
	en_apierrorState_t  len_ReturnValue = E_API_FAILED;
	/*No valid Message available*/
    printf("\r\n mailbox_Statemachine_Change_Accept:%u",sst_mbQueue.mu8_ValidMsg );
	if(sst_mbQueue.mu8_ValidMsg != 0) {

		for(;lu8_Index < U8_MB_TOTAL_MSG_ENTRY;lu8_Index++) {
			if( ( sst_mbQueue.mstar_Msg[lu8_Index].mu32_SeqNo != 0 ) &&\
				( sst_mbQueue.mstar_Msg[lu8_Index].men_SrcId == len_DestId ) )
			{
				if( lu32_MinSeqNo == U32_MB_MAX_SEQ_NO )
				{
					lu32_MinSeqNo = sst_mbQueue.mstar_Msg[lu8_Index].mu32_SeqNo;
					lu8_EntryIndex = lu8_Index;
				}
				else if( lu32_MinSeqNo > sst_mbQueue.mstar_Msg[lu8_Index].mu32_SeqNo)
				{
					lu32_MinSeqNo = sst_mbQueue.mstar_Msg[lu8_Index].mu32_SeqNo;
					lu8_EntryIndex = lu8_Index;
				}
			}
		}
		if(lu8_EntryIndex < U8_MB_TOTAL_MSG_ENTRY)
		{
			if(lstp_Msg != NULL)
			{
				memcpy(lstp_Msg,\
						&sst_mbQueue.mstar_Msg[lu8_EntryIndex].mst_Msg,\
						sizeof(st_gMsgQueueEntry_t));
				/*Enter in Critical section*/
        xSemaphoreTake( sst_mbQueue.mst_Mutex, ( TickType_t ) 50);
				//osi_EnterCritSect(&sst_mbQueue.mst_Mutex);
				memset(&sst_mbQueue.mstar_Msg[lu8_EntryIndex].mst_Msg,0,\
						sizeof(st_gMsgQueueEntry_t));
				sst_mbQueue.mstar_Msg[lu8_EntryIndex].mu32_SeqNo = 0;
				sst_mbQueue.mu8_ValidMsg--;
				/*Leave in Critical section*/
        xSemaphoreGive( sst_mbQueue.mst_Mutex );
				//osi_LeaveCritSect(&sst_mbQueue.mst_Mutex);
			}
			len_ReturnValue = E_API_SUCCESS;
		}
	}
	return len_ReturnValue;
 }
  /**************************************************************************
 * Function Name	:	mb_Close
 *
 * Description		:	This function to Delete mail box
 *
 * Arguments		:	None
 *
 *
 * Return values	:   None
 *
 ****************************************************************************/
u8 mailbox_Deinit(void)
{
	u8 lu8_Return = 0;
	//osi_DeInitCritSect(&sst_mbQueue.mst_Mutex);
  vSemaphoreDelete(sst_mbQueue.mst_Mutex);
	memset(sst_mbQueue.mstar_Msg,0,sizeof(sst_mbQueue.mstar_Msg));
	sst_mbQueue.mu32_RuningSeqNo = 1;
	sst_mbQueue.mu8_ValidMsg = 0;
	return lu8_Return;
}
/*****************************************************************************
 * Function Name    :    ProcessStateTable
 *
 * Description      :   Searches the state table for the given message and calls
 *                     the functions pointer to perform the necessary action
 *
 * Arguments        :   lstp_tblptr - Pointer to State Table
 *						lstp_StateInfo - Pointer to Current State
 *						lstp_Mgs - Pointer to message
 *
 * Return values    :   APIStatus_t - E_API_SUCCESS/E_API_FAILED
 *
 *****************************************************************************/
void processStateTable
	(
		const st_gStateTable_t *lstp_tblptr ,
		st_gStateStatus_t *lstp_StateInfo ,
		st_gMsgQueueEntry_t *lstp_Msg
	)
{
	u8 lu8_Msg_Found = 1;
	u8 lu8_LoopCnt = 0;
	u8 lu8_Return = 0;

	//Process State Table
	for (lu8_LoopCnt = 0;lstp_tblptr->mu8_CurrentState != E_GLOABL_INVALID_ST ; lu8_LoopCnt++)
	{
    printf("\r\n stateinfo_Cur :%d,Table :%d Msg Source%d, Table%d",lstp_StateInfo->mu8_CurrentState,
              lstp_tblptr->mu8_CurrentState,lstp_Msg->mu8_SourceId, lstp_tblptr->men_MsgSource);
		if ( ( ( lstp_StateInfo->mu8_CurrentState == lstp_tblptr->mu8_CurrentState )
				|| ( E_GLOBAL_ANY_ST == lstp_tblptr->mu8_CurrentState ) )
			&& ( lstp_Msg->mu8_SourceId == lstp_tblptr->men_MsgSource )

			&& ( lstp_Msg->mu8_MsgId == lstp_tblptr->mu8_Msg ) )
		{
			lu8_Msg_Found = 1;
			break;
		}
		else
		{
			lstp_tblptr++;
		}
	}

	if ( 1 == lu8_Msg_Found)
	{
		//No state change , but execute the state table funtion
		if( (E_GLOBAL_ANY_ST == lstp_tblptr->mu8_CurrentState ) && ( E_GLOBAL_ANY_ST == lstp_tblptr->mu8_NextState ))
		{
			if (NULL != lstp_tblptr->g_MsgProcessingFunc)
			{
				lstp_tblptr->g_MsgProcessingFunc(lstp_Msg);
			}
		}
		else
		{
			lstp_StateInfo->mu8_PreviousState = lstp_StateInfo->mu8_CurrentState;
			lstp_StateInfo->mu8_CurrentState = lstp_tblptr->mu8_NextState;
			lstp_StateInfo->g_MsgProcessingFunc = lstp_tblptr->g_MsgProcessingFunc;
			if (NULL != lstp_tblptr->g_MsgProcessingFunc)
			{
				lstp_tblptr->g_MsgProcessingFunc(lstp_Msg);
			}
			lstp_StateInfo->mu8_PreviousState = lstp_StateInfo->mu8_CurrentState;
		}
		lu8_Return = 0;
	}
	return lu8_Return;
}
