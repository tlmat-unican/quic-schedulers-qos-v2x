/* Copyright (C) 2015-2016 Autotalks Ltd. */
#ifndef _ATLK_ECC_REMOTE_H
#define _ATLK_ECC_REMOTE_H

#include <atlk/sdk.h>
#include <atlk/remote.h>
#include <atlk/ecc.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   ECC remote service API
*/

/** ECC remote service configuration parameters */
typedef struct ecc_remote_service_config ecc_remote_service_config_t;

/**
   Create ECC remote service.

   @param[in] transport Remote transport instance
   @param[in] config ECC remote service configuration (optional)
   @param[out] service_ptr ECC service

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
ecc_remote_service_create(remote_transport_t *transport,
            const ecc_remote_service_config_t *config,
            ecc_service_t **service_ptr);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_ECC_REMOTE_H */
