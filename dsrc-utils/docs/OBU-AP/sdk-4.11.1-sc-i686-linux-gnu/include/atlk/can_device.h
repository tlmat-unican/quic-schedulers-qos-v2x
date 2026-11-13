/* Copyright (C) 2015 Autotalks Ltd. */
#ifndef _ATLK_CAN_DEVICE_H
#define _ATLK_CAN_DEVICE_H

#include <atlk/sdk.h>
#include <atlk/can.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   CAN device API
*/

/**
   CAN device transmission function prototype.

   @param[in] context Context pointer
   @param[in] data_ptr Pointer to start of data frame
   @param[in] data_size Data frame size in bytes
   @param[in] can_id CAN ID of frame
   @param[in] wait Wait option (optional)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
typedef atlk_rc_t
(*can_tx_handler_t)(void *context,
		    const void *data_ptr,
		    size_t data_size,
		    can_id_t can_id,
		    const atlk_wait_t *wait);

/** CAN device */
typedef struct {
  /** Transmission handler function */
  can_tx_handler_t tx_handler;

  /** Context pointer passed to all device functions */
  void *context;

} can_device_t;

/** CAN device default initializer */
#define CAN_DEVICE_INIT {       \
  .tx_handler = NULL,           \
  .context = NULL,              \
}

/**
   Attach CAN device object to CAN device ID.

   @param[in] service CAN service
   @param[in] device_id Device ID
   @param[in] device CAN device 

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
can_device_attach(can_service_t *service,
            can_device_id_t device_id,
            const can_device_t *device);

/**
   Handle CAN frame reception.

   @param[in] service CAN service
   @param[in] device_id CAN device ID
   @param[in] data_ptr Pointer to start of frame data
   @param[in] data_size Frame data size in bytes
   @param[in] can_id Frame's CAN ID

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
can_rx_handler(can_service_t *service,
	       can_device_id_t device_id,
	       const void *data_ptr,
	       size_t data_size,
	       can_id_t can_id);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_CAN_DEVICE_H */
