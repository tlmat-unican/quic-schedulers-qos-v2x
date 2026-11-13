/* Copyright (C) 2016 Autotalks Ltd. */
#ifndef _ATLK_MIBS_SPI2UART_MIBSTAT_H
#define _ATLK_MIBS_SPI2UART_MIBSTAT_H

#include <atlk/mib_service.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   SPI2UART Status API.
*/

/** SPI2UART status */
typedef struct {
  /** Count of transmitted frames */
  uint32_t spi2uartTxFrameCnt;

  /** Count of received frames */
  uint32_t spi2uartRxFrameCnt;

  /** Count of overrun error due to received frames */
  uint32_t spi2uartRxOverrunErrorCnt;

  /** Count of parity error due to received frames */
  uint32_t spi2uartRxParityErrorCnt;

  /** Count of framing error due to received frames */
  uint32_t spi2uartRxFramingErrorCnt;

} mibstat_spi2uartMib_t;

/**
   Get SPI2UART status.

   @param[in] service Instance of MIB service
   @param[out] value SPI2UART status value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mibstat_get_spi2uartMib(mib_service_t *service,
            mibstat_spi2uartMib_t *value);

/**
   Reset SPI2UART status.

   @param[in] service Instance of MIB service

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mibstat_reset_spi2uartMib(mib_service_t *service);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_MIBS_SPI2UART_MIBSTAT_H */
