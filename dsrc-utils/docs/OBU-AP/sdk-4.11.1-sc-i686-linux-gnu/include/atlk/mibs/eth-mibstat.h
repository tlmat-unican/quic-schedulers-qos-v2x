/* Copyright (C) 2016 Autotalks Ltd. */
#ifndef _ATLK_MIBS_ETH_MIBSTAT_H
#define _ATLK_MIBS_ETH_MIBSTAT_H

#include <atlk/mib_service.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   Ethernet Status API.
*/

/** Ethernet status */
typedef struct {
  /** Count of transmitted frames */
  uint32_t ethTxFrameCnt;

  /** Count of transmitted frames with no errors */
  uint32_t ethTxGoodFrameCnt;

  /** Count of received frames */
  uint32_t ethRxFrameCnt;

  /** Count of received frames with CRC errors */
  uint32_t ethRxCrcErrorCnt;

} mibstat_ethMib_t;

/**
   Get Ethernet status.

   @param[in] service Instance of MIB service
   @param[out] value Ethernet status value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mibstat_get_ethMib(mib_service_t *service,
            mibstat_ethMib_t *value);

/**
   Reset Ethernet status.

   @param[in] service Instance of MIB service

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mibstat_reset_ethMib(mib_service_t *service);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_MIBS_ETH_MIBSTAT_H */
