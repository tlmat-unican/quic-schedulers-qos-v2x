/* Copyright (C) 2014-2015 Autotalks Ltd. */
#ifndef _CRATON_WLAN_DRIVER_H
#define _CRATON_WLAN_DRIVER_H

#include <atlk/sdk.h>
#include <atlk/v2x.h>

#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   WLAN Driver API
*/

/** WLAN frame */
typedef struct {
  /** Pointer to frame header */
  const void *frame_header_ptr;

  /** Frame header size */
  size_t frame_header_size;

  /**
     Frame body pointer.

     @remark In TX mode, Sequence Control field in MAC header is calculated
     by HW upon frame transmission, hence Sequence Control field does not
     reflect true value in TX mode.

     @remark In TX mode, last 4 bytes are reserved for FCS calculation
     which is done by HW upon frame transmission. MAC level does not see
     real FCS calculation, hence last 4 bytes do not reflect true FCS in
     TX mode.
   */
  const void *frame_body_ptr;

  /** Frame body size */
  size_t frame_body_size;

} wlan_frame_t;

/** WLAN frame default initializer */
#define WLAN_FRAME_INIT {       \
  .frame_header_ptr = NULL,     \
  .frame_header_size = 0,       \
  .frame_body_ptr = NULL,       \
  .frame_body_size = 0          \
}

/** WLAN RX frame info */
typedef struct {
  /** WLAN device ID on which frame was received */
  uint8_t device_id;

  /** Data rate in units of 500 kbit/s */
  v2x_datarate_t datarate;

  /** Average input power of frame in units of 1/8 dBm */
  v2x_power_dbm8_t power_dbm8; 

  /**
     Receive time in microseconds.

     Format: number of TAI microseconds since 2004-01-01T00:00:00Z (UTC).
  */
  uint64_t rx_time_us;

  /** 
     Time at RX complete interrupt in microseconds.

     Format: number of TAI microseconds since 2004-01-01T00:00:00Z (UTC).
  */
  uint64_t rx_isr_time_us;

} wlan_rx_frame_info_t;

/** WLAN RX frame info default initializer */
#define WLAN_RX_FRAME_INFO_INIT {       \
  .device_id = 0,                       \
  .datarate = V2X_DATARATE_NA,          \
  .power_dbm8 = V2X_POWER_DBM8_NA,      \
  .rx_time_us = 0,                      \
  .rx_isr_time_us = 0                   \
}

/** Traffic monitor RX callback function */
typedef void
(*wlan_rx_traffic_monitor_t)(const wlan_frame_t *frame,
            const wlan_rx_frame_info_t *info);

/**
   Set RX callback.

   RX callback is called for each MPDU received (even when there are no open
   V2X sockets).

   @remark Callbacks for different interfaces might be called simultaneously.
   Hence, callbacks should be designed to be reentrant.

   @remark When a callback is set to NULL, it clears previously set callback.

   @param[in] device_id WLAN device ID
   @param[in] rx_callback RX callback function

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
 */
atlk_rc_t atlk_must_check
wlan_rx_traffic_monitor_set(uint8_t device_id,
            wlan_rx_traffic_monitor_t rx_callback);

/** WLAN TX frame info */
typedef struct {
  /** WLAN device ID on which frame was transmitted */
  uint8_t device_id;

  /** Data rate in units of 500 kbit/s */
  v2x_datarate_t datarate;

  /** Transmission power level in units of 1/8 dBm */
  v2x_power_dbm8_t power_dbm8; 

  /**
     Time when frame was added to transmit queue in microseconds.

     Format: number of TAI microseconds since 2004-01-01T00:00:00Z (UTC).
  */
  uint64_t tx_queue_time_us;

  /** 
     Time at TX complete interrupt in microseconds.

     Format: number of TAI microseconds since 2004-01-01T00:00:00Z (UTC).
  */
  uint64_t tx_isr_time_us;

} wlan_tx_frame_info_t;

/** WLAN TX frame info default initializer */
#define WLAN_TX_FRAME_INFO_INIT {       \
  .device_id = 0,                       \
  .datarate = V2X_DATARATE_NA,          \
  .power_dbm8 = V2X_POWER_DBM8_NA,      \
  .tx_queue_time_us = 0,                \
  .tx_isr_time_us = 0                   \
}

/** Traffic monitor TX callback function */
typedef void
(*wlan_tx_traffic_monitor_t)(const wlan_frame_t *frame,
            const wlan_tx_frame_info_t *info);

/**
   Set TX callback.

   TX callback is called for each MPDU transmitted.

   @remark Callbacks for different interfaces might be called simultaneously.
   Hence, callbacks should be designed to be reentrant.

   @remark When a callback is set to NULL, it clears previously set callback.

   @param[in] device_id WLAN device ID
   @param[in] tx_callback TX callback function

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
 */
atlk_rc_t atlk_must_check
wlan_tx_traffic_monitor_set(uint8_t device_id,
            wlan_tx_traffic_monitor_t tx_callback);

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_WLAN_DRIVER_H */
