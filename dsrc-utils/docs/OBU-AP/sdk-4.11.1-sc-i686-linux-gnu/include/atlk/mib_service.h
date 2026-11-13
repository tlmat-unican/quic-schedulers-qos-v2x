/* Copyright (C) 2013-2015 Autotalks Ltd. */
#ifndef _ATLK_MIB_SERVICE_H
#define _ATLK_MIB_SERVICE_H

#include <atlk/sdk.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   MIB service API
*/

/** MIB service */
typedef struct mib_service mib_service_t;

/**
   Delete MIB service instance.

   @param[in] service MIB service instance

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t
mib_service_delete(mib_service_t *service);

/**
   Get pointer to default MIB service.

   @param[out] service_ptr MIB service

   @note Not supported by remote service library.

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_default_service_get(mib_service_t **service_ptr);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_MIB_SERVICE_H */

