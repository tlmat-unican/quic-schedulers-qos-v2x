/* Copyright (C) 2016 Autotalks Ltd. */
#ifndef _ATLK_SNMP_AGENT_H
#define _ATLK_SNMP_AGENT_H

#include <atlk/sdk.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   SNMP agent API
*/

/**
   Start SNMP agent.

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
snmp_agent_start(void);

/**
   Stop SNMP agent.

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
snmp_agent_stop(void);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_SNMP_AGENT_H */
