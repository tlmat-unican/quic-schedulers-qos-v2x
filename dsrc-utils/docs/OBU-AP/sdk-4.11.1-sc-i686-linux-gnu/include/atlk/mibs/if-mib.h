/* Copyright (C) 2014-2015 Autotalks Ltd. */
#ifndef _ATLK_MIBS_IF_MIB_H
#define _ATLK_MIBS_IF_MIB_H

#include <atlk/mib_service.h>
#include <atlk/eui48.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   Network interface MIB API
*/

/**
   Get physical address.

   The Ethernet interface index number is 20.

   @param[in] service Instance of MIB service
   @param[in] if_index Interface index
   @param[in] value Physical address

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_ifPhysAddress(mib_service_t *service,
             int32_t if_index,
             eui48_t *value);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_MIBS_IF_MIB_H */
