/* Copyright (C) 2015 Autotalks Ltd. */
#ifndef _CRATON_UART_DRIVER_H
#define _CRATON_UART_DRIVER_H

#include <atlk/sdk.h>
#include <atlk/uart.h>

#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   UART driver API
*/

/** UART device ID */
typedef enum {
  /** CRATON UART0 device ID */
  UART_DEVICE_ID_0 = 0,

  /** CRATON UART1 device ID */
  UART_DEVICE_ID_1 = 1,

  /** UART over SPI device ID */
  UART_DEVICE_ID_2 = 2

} uart_device_id_t;

/* UART configuration structure */
typedef struct {
  uart_speed_bps_t uart_speed_bps;
} uart_config_t;

/**
   Set UART device speed in bits/s.

   @remark UART0/1 device speed change settling time is in the order of 10ms.

   @param[in] device_id UART device ID
   @param[in] speed_bps Speed in bits/s

   @retval ATLK_OK if succeeded
   @return Error code if failed

*/
atlk_rc_t atlk_must_check 
uart_speed_set(uart_device_id_t device_id,
            uart_speed_bps_t speed_bps);

/**
   Get UART device speed in bits/s.

   @param[in] device_id UART device ID
   @param[out] speed_bps Speed in bits/s

   @retval ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
uart_speed_get(uart_device_id_t device_id,
            uart_speed_bps_t *speed_bps);

/**
   Flush UART device receive buffer.

   Upon successful completion, any data received but not read by the device
   is discarded.

   @param[in] device_id UART device ID

   @retval ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
uart_rx_flush(uart_device_id_t device_id);

/** UART mode flag: Map NL to CR-NL on output */
#define UART_MODE_F_ONLCR (1U << 0)

/**
   Set UART mode.

   @param[in] device_id UART device ID
   @param[in] flags UART device flags

   @retval ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
uart_mode_set(uart_device_id_t device_id,
            unsigned int flags);

/**
   Get UART mode.

   @param[in] device_id UART device ID
   @param[out] flags_ptr UART device flags

   @retval ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
uart_mode_get(uart_device_id_t device_id,
            unsigned int *flags_ptr);

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_UART_DRIVER_H */
