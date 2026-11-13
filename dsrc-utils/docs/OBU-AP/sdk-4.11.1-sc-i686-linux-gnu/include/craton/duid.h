/* Copyright (C) 2015 Autotalks Ltd. */
#ifndef _CRATON_DUID_H
#define _CRATON_DUID_H

#include <atlk/sdk.h>

#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   Device Unique ID API
*/

/** Maximum length of Device Unique ID in octets */
#define DUID_MAX_LEN 32

/**
   Get Device Unique ID (DUID).

   @p buf_len must be less or equal to ::DUID_MAX_LEN.

   @note If an engineering sample of CRATON is used, this function invokes
   SHA256-based KDF2 on boot parameter `duid_str` and the result is returned
   as DUID. 
   
   @param[out] buf Buffer to store the DUID
   @param[in] buf_len Length of desired DUID

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
duid_get(void *buf, size_t buf_len);

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_DUID_H */
