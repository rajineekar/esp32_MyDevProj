#ifndef _CONTROLFUN_H
#define _CONTROLFUN_H

#include "global.h"


typedef struct
{
    void           (*mod_ControlFunc)(en_ModActionReqType_t lst_ActionRequested, u32 lu32_Reason, st_MainTaskControlInfo_t *lst_TasKControlInfo);
    en_PwrTaskIdleType_t   men_TaskIdleValue;
} st_pwrTaskControllingFunc_t;


void execute_Control_Fun(void);



#endif
