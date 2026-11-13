/* Copyright (C) 2015 Autotalks Ltd. */
#ifndef _ATLK_V2X_REMOTE_H
#define _ATLK_V2X_REMOTE_H

#include <atlk/sdk.h>
#include <atlk/remote.h>
#include <atlk/v2x.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   V2X remote service API
*/

/** V2X remote service configuration parameters */
typedef struct v2x_remote_service_config v2x_remote_service_config_t;

/**
   Create V2X remote service.

   @param[in] transport Remote transport instance
   @param[in] config V2X remote service configuration (optional)
   @param[out] service_ptr V2X service

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
v2x_remote_service_create(remote_transport_t *transport,
            const v2x_remote_service_config_t *config,
            v2x_service_t **service_ptr);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_V2X_REMOTE_H */
