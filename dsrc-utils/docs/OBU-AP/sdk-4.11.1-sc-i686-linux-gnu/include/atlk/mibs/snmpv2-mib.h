/* Copyright (C) 2014-2015 Autotalks Ltd. */
#ifndef _ATLK_MIBS_SNMPV2_MIB_H
#define _ATLK_MIBS_SNMPV2_MIB_H

#include <atlk/mib_service.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   SNMPv2 MIB API
*/

/**
   Get system description.
 
   On success, @p size will be set to actual length of description string.
   The system description string is guaranteed to be null-terminated.

   @param[in] service Instance of MIB service
   @param[in] value System description
   @param[in,out] size Description string length in octets

   @retval ::ATLK_OK if succeeded
   @retval ::ATLK_E_BUFFER_TOO_SMALL if @p size is too small
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_sysDescr(mib_service_t *service,
             char *value,
             size_t *size);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_MIBS_SNMPV2_MIB_H */
