/* Copyright (C) 2014-2015 Autotalks Ltd. */
#ifndef _CRATON_NET_H
#define _CRATON_NET_H

#include <nx_api.h>

#include <atlk/sdk.h>

#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   CRATON TCP/IP network stack API
*/

/**
   Get pointer to NetX-Duo trusted IP instance.

   @param[out] ip_ptr Pointer to IP instance

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
net_ip_trusted_instance_get(NX_IP **ip_ptr);

/**
   Get pointer to NetX-Duo untrusted IP instance.

   @param[out] ip_ptr Pointer to IP instance

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
net_ip_untrusted_instance_get(NX_IP **ip_ptr);

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_NET_H */
