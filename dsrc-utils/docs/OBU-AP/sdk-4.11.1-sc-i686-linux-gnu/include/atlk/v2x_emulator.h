/* Copyright (C) 2014-2015 Autotalks Ltd. */
#ifndef _ATLK_V2X_EMULATOR_H
#define _ATLK_V2X_EMULATOR_H

#include <atlk/sdk.h>
#include <atlk/v2x.h>
#include <atlk/v2x_service.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   V2X emulator API
*/

/** V2X emulator instance */
typedef struct v2x_emulator v2x_emulator_t;

/**
   Get pointer to V2X emulator service.

   @param[out] service_ptr V2X service

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
v2x_emulator_service_get(v2x_service_t **service_ptr);

/**
   Send V2X frame to emulated V2X service.

   @see @ref wait_usage.

   @param[in] emulator V2X emulator
   @param[in] if_index Ingress MAC interface index
   @param[in] protocol V2X protocol descriptor
   @param[in] data_ptr Pointer to start of data
   @param[in] data_size Size of data in bytes
   @param[in] params Output parameters of V2X receive operation
   @param[in] wait Wait specification (optional)

   @todo Currently @p wait is not supported -- operation is always non-blocking.

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
v2x_emulator_send(v2x_emulator_t *emulator,
            v2x_if_index_t if_index,
            const v2x_protocol_t *protocol,
            const void *data_ptr,
            size_t data_size,
            const v2x_receive_params_t *params,
            const atlk_wait_t *wait);

/**
   Receive V2X frame from emulated V2X service.

   @see @ref wait_usage.

   @param[in] emulator V2X emulator
   @param[out] if_index Egress MAC interface index
   @param[out] protocol V2X protocol descriptor
   @param[out] data_ptr Pointer to start of data
   @param[in,out] data_size_ptr Maximum (in) and actual (out) data size in bytes
   @param[out] params Input parameters of V2X send operation
   @param[in] wait Wait specification (optional)

   @todo Currently @p wait supports non-blocking or wait-forever options only.

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
v2x_emulator_receive(v2x_emulator_t *emulator,
            v2x_if_index_t *if_index,
            v2x_protocol_t *protocol,
            void *data_ptr,
            size_t *data_size_ptr,
            v2x_send_params_t *params,
            const atlk_wait_t *wait);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_V2X_EMULATOR_H */
