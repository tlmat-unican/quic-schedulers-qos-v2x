/* Copyright (C) 2014-2015 Autotalks Ltd. */
#ifndef _ATLK_NAV_SERVICE_H
#define _ATLK_NAV_SERVICE_H

#include <atlk/sdk.h>
#include <atlk/nav.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   Navigation service API

   @note Service is (optionally) implemented by user on host.
*/

/**
   Get pointer to default navigation service.

   @param[out] service_ptr Pointer to navigation service

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
nav_default_service_get(nav_service_t **service_ptr);

/**
   Delete navigation service.

   @param[in] service Navigation service

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t
nav_service_delete(nav_service_t *service);

/** Navigation fix subscriber */
typedef struct nav_subscriber nav_fix_subscriber_t;

/**
   Create navigation fix subscriber.

   @param[in] service Navigation service
   @param[out] subscriber_ptr Navigation fix subscriber pointer

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
nav_fix_subscriber_create(nav_service_t *service,
            nav_fix_subscriber_t **subscriber_ptr);

/**
   Delete navigation fix subscriber.

   @param[in] subscriber Navigation fix subscriber

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t
nav_fix_subscriber_delete(nav_fix_subscriber_t *subscriber);

/**
   Receive new navigation fix via @p subscriber.

   @see @ref wait_usage.

   @param[in] subscriber Navigation fix subscriber
   @param[out] fix Navigation fix
   @param[in] wait Wait specification (optional)

   @retval ::ATLK_OK if succeeded
   @retval ::ATLK_E_NOT_READY
           if new navigation fix is not available and @p wait is NULL
   @retval ::ATLK_E_TIMEOUT
           if new navigation fix is not available and @p wait is of type
           ::ATLK_WAIT_INTERVAL.
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
nav_fix_receive(nav_fix_subscriber_t *subscriber,
            nav_fix_t *fix,
            const atlk_wait_t *wait);

/**
   Publish navigation fix for subscribers of @p service.

   @param[in] service Navigation service instance
   @param[in] fix Navigation fix to publish

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
nav_fix_publish(nav_service_t *service,
            nav_fix_t *fix);

/**
   Navigation fix processing callback function.

   @param[in,out] fix Navigation data fix to be published
   @param[in,out] context Callback context
*/
typedef void
(*nav_fix_processor_t)(nav_fix_t *fix,
            void *context);

/**
   Register navigation fix processing callback.
  
   Registering the processing callback is optional. When registered, callback
   is called on every fix before it is published. Publishing of fix is delayed
   by the callbacks running time.

   @param[in] service Navigation service instance
   @param[in] callback Navigation fix processing callback function
   @param[in,out] context Callback context (optional)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
nav_fix_process_set(nav_service_t *service,
            nav_fix_processor_t callback,
            void *context);

/** Navigation data subscriber */
typedef struct nav_subscriber nav_data_subscriber_t;

/**
   Create navigation data subscriber.

   @note @p data_mask is a bitmask of ::NAV_DATA_TYPE_FIX and
   ::NAV_DATA_TYPE_SATELLITE_REPORT.

   @param[in] service Navigation service
   @param[in] data_mask Navigation data subscription mask
   @param[out] subscriber_ptr Navigation data subscriber pointer

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
nav_data_subscriber_create(nav_service_t *service,
            uint32_t data_mask,
            nav_data_subscriber_t **subscriber_ptr);

/**
   Delete navigation data subscriber.

   @param[in] subscriber Navigation data subscriber

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t
nav_data_subscriber_delete(nav_data_subscriber_t *subscriber);

/**
   Receive new navigation data via @p subscriber.

   @see @ref wait_usage.

   @param[in] subscriber Navigation data subscriber
   @param[out] data Navigation data frame
   @param[in] wait Wait specification (optional)

   @retval ::ATLK_OK if succeeded
   @retval ::ATLK_E_NOT_READY
           if new navigation data is not available and @p wait is NULL
   @retval ::ATLK_E_TIMEOUT
           if new navigation data is not available and @p wait is of type
           ::ATLK_WAIT_INTERVAL.
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
nav_data_receive(nav_data_subscriber_t *subscriber,
            nav_data_t *data,
            const atlk_wait_t *wait);

/**
   Publish navigation data for subscribers of @p service.

   @param[in] service Navigation service instance
   @param[in] data Navigation data frame to publish

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
nav_data_publish(nav_service_t *service,
            nav_data_t *data);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_NAV_SERVICE_H */
