/*************************************************************************
|"2016 Danlaw Inc. All rights reserved."
|
| File              :   mailbox_main_pri.h
| Created           :   March 22 2016
| Application       :
| Functions         :
| Author            :   Danlaw
| Target            :   Gen 3.x - QCM
| Environment       :   DS5 and Cygwin
*************************************************************************/

/************************************************************************
| Change History:
| Version      Date          		Person        	Change Description
| 0.1          March 22 2016         					Initial version
|
*************************************************************************/
#ifndef _MB_MAIN_PRI_H
#define _MB_MAIN_PRI_H

/************************************************************************
|          Include files
|************************************************************************/

/************************************************************************
|          Macro Definition
|************************************************************************/
#define U8_MB_TOTAL_MSG_ENTRY			100
#define U32_MB_MAX_SEQ_NO				0xFFFFFFFF

/************************************************************************
|          Enum Definition
|************************************************************************/


/************************************************************************
|          Structure Definition
|************************************************************************/
// Structure for Message queue
typedef struct
{
  u8 men_ModuleId;
	u8 mu8_mbIndex;
}st_mbQTable_t;

// Structure for Message queue
typedef struct
{
	u32 mu32_SeqNo;
  en_gModules_t men_SrcId;
	st_gMsgQueueEntry_t mst_Msg;
}st_mbEntry_t;

// Structure for Message queue
typedef struct
{
	u8 mu8_ValidMsg;
	u32 mu32_RuningSeqNo;
  st_mbEntry_t mstar_Msg[U8_MB_TOTAL_MSG_ENTRY];
  SemaphoreHandle_t mst_Mutex;
}st_mbQueue_t;


/************************************************************************
|          PROTOTYPE
|************************************************************************/


/************************************************************************
|          End File
|************************************************************************/
#endif
