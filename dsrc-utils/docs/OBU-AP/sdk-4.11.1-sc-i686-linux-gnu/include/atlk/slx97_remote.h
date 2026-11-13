/* Copyright (C) 2016 Autotalks Ltd. */
#ifndef _ATLK_SLX97_REMOTE_H
#define _ATLK_SLX97_REMOTE_H

#include <atlk/sdk.h>
#include <atlk/slx97.h>
#include <atlk/remote.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   Remote SLx97 host API
*/

/**
   Create remote HSM service.

   @remark Use ::slx97_host_hsm_service_get to get the HSM SLx97 service
   after creation.

   @param[in] sec_config SLx97 host communication security parameters
   @param[in] dsk_callback Callback function for retrieving the value of
                           a device specific key

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
slx97_remote_hsm_service_create(remote_transport_t *transport,
            const slx97_host_sec_config_t *sec_config,
            slx97_dsk_callback_t dsk_callback);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_SLX97_REMOTE_H */
