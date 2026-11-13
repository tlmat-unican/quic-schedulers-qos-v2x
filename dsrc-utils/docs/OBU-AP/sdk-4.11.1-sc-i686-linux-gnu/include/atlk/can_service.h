/* Copyright (C) 2014-2015 Autotalks Ltd. */
#ifndef _ATLK_CAN_SERVICE_H
#define _ATLK_CAN_SERVICE_H

#include <atlk/sdk.h>
#include <atlk/can.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   CAN service API
*/

/**
   Get pointer to default CAN service.

   @param[out] service_ptr Pointer to CAN service

   @note New implementation of this getter will override default
   getter (declared as a weak symbol).

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
can_default_service_get(can_service_t **service_ptr);

/**
   Delete CAN service instance.

   @param[in] service CAN service instance

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t
can_service_delete(can_service_t *service);

/** CAN ID mask that masks all ID bits */
#define CAN_ID_MASK_ALL_BITS 0xffffffffU

/** CAN ID filter */
typedef struct {
  /** CAN ID value */
  can_id_t can_id;

  /**
     CAN ID mask

     A frame with CAN ID some_can_id matches the filter if
     (some_can_id & can_id_mask) == (can_id & can_id_mask).
  */
  can_id_t can_id_mask;

} can_id_filter_t;

/** Initializer of CAN ID filter that filters one ID */
#define CAN_ID_FILTER_ONE_ID(id) {    \
  .can_id = id,                       \
  .can_id_mask = CAN_ID_MASK_ALL_BITS \
}

/** CAN socket */
typedef struct can_socket can_socket_t;

/** CAN socket configuration parameters */
typedef struct {
  /** Ingress/egress CAN device ID */
  can_device_id_t device_id;

  /** Receive-side CAN message ID filter array */
  const can_id_filter_t *filter_array_ptr;

  /** Receive-side CAN message ID filter array size */
  size_t filter_array_size;

} can_socket_config_t;

/** CAN socket configuration parameters default initializer */
#define CAN_SOCKET_CONFIG_INIT {    \
  .device_id = CAN_DEVICE_ID_NA,    \
  .filter_array_ptr = NULL,         \
  .filter_array_size = 0            \
}

/**
   Create CAN socket.

   When ::can_socket_config_t::device_id is set to ::CAN_DEVICE_ID_NA,
   ::can_receive will receive frames from both CAN devices and ::can_send
   cannot be used.

   @param[in] service CAN service instance
   @param[out] socket_ptr CAN socket pointer
   @param[in] config CAN socket configuration

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
can_socket_create(can_service_t *service,
            can_socket_t **socket_ptr,
            const can_socket_config_t *config);

/**
   Delete CAN socket.

   @param[in] socket CAN socket to delete

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t
can_socket_delete(can_socket_t *socket);

/**
   Send CAN frame.

   @see @ref wait_usage.

   @param[in] socket CAN socket
   @param[in] data_ptr Pointer to start of data
   @param[in] data_size Size of data in bytes
   @param[in] can_id CAN ID of frame
   @param[in] wait Wait specification (optional)

   @todo Currently @p wait is not supported -- operation is always non-blocking.

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
can_send(can_socket_t *socket,
            const void *data_ptr,
            size_t data_size,
            can_id_t can_id,
            const atlk_wait_t *wait);

/**
   Receive CAN frame.

   @see @ref wait_usage.

   @param[in] socket CAN socket
   @param[out] data_ptr Pointer to start of frame data
   @param[in,out] data_size_ptr
            Maximum (in) and actual (out) frame data size in bytes
   @param[out] can_id_ptr Pointer to received frame's CAN ID
   @param[in] wait Wait specification (optional)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
can_receive(can_socket_t *socket,
            void *data_ptr,
            size_t *data_size_ptr,
            can_id_t *can_id_ptr,
            const atlk_wait_t *wait);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_CAN_SERVICE_H */
