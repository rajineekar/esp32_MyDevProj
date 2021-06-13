#ifndef _MAILBOX_PUB_H
#define _MAILBOX_PUB_H

#include "global.h"

u8 mailBox_Init(void);
u8 mailbox_Statemachine_Change_Req(en_gModules_t lu8_SrcId,en_gModules_t lu8_DestId,u8 lu8_MsgId,
         u8 lu8_Priority,u32 lu32_Attribute);
u8 mailbox_Statemachine_Change_Accept(en_gModules_t len_DestId,st_gMsgQueueEntry_t *lstp_Msg);
u8 mailbox_Deinit(void);
void processStateTable(const st_gStateTable_t *lstp_tblptr ,st_gStateStatus_t *lstp_StateInfo ,st_gMsgQueueEntry_t *lstp_Msg );

#endif
