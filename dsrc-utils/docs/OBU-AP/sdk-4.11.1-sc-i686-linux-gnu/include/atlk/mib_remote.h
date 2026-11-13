/* Copyright (C) 2015 Autotalks Ltd. */
#ifndef _ATLK_MIB_REMOTE_H
#define _ATLK_MIB_REMOTE_H

#include <atlk/sdk.h>
#include <atlk/remote.h>
#include <atlk/mib_service.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   MIB remote service API
*/

/** MIB remote service configuration parameters */
typedef struct mib_remote_service_config mib_remote_service_config_t;

/**
   Create MIB remote service.

   @param[in] transport Remote transport instance
   @param[in] config MIB remote service configuration (optional)
   @param[out] service_ptr MIB service

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_remote_service_create(remote_transport_t *transport,
            const mib_remote_service_config_t *config,
            mib_service_t **service_ptr);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_MIB_REMOTE_H */
