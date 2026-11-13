/* Copyright (C) 2016 Autotalks Ltd. */
#ifndef _CRATON_WAVE_IPV6_H
#define _CRATON_WAVE_IPV6_H

#include <atlk/sdk.h>

#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   WAVE IPv6 API
*/

/**
   Set WAVE IPv6 enabled.

   @param[in] if_index MAC interface index
   @param[in] enabled Whether WAVE IPv6 is enabled

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
wave_ipv6_enabled_set(int32_t if_index,
            int enabled);

/**
   Get WAVE IPv6 enabled.

   @param[in] if_index MAC interface index
   @param[out] enabled Whether WAVE IPv6 is enabled

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
wave_ipv6_enabled_get(int32_t if_index,
            int *enabled);

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_WAVE_IPV6_H */
