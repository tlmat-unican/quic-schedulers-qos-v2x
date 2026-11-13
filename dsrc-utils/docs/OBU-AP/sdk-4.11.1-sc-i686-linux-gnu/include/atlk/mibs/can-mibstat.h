/* Copyright (C) 2016 Autotalks Ltd. */
#ifndef _ATLK_MIBS_CAN_MIBSTAT_H
#define _ATLK_MIBS_CAN_MIBSTAT_H

#include <atlk/mib_service.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   CAN Status API.
*/

/** CAN device status */
typedef struct {
  /** Count of transmitted frames */
  uint64_t canTxFrameCnt;

  /** Count of received frames */
  uint64_t canRxFrameCnt;

  /** Count of received frames which were filtered by SW */
  uint64_t canRxSwFilteredCnt;

  /** Count of errors due to transmitted frames */
  uint32_t canTxErrorCnt;

  /** Count of errors due to received frames */
  uint32_t canRxErrorCnt;

  /** Count of HW errors due to transmitted frames */
  uint32_t canTxHwErrorCnt;

  /** Count of HW errors due to received frames */
  uint32_t canRxHwErrorCnt;

  /** Count of generated error interrupts */
  uint32_t canInterruptErrorCnt;

  /** Count of block allocation failures */
  uint32_t canBlockAllocErrorCnt;

  /** Count of transmission queueing failures */
  uint32_t canTxQueueErrorCnt;

} mibstat_canDevEntry_t;

/** CAN device table size */
#define MIB_canDevTable_SIZE 2

/** CAN status */
typedef struct {
  mibstat_canDevEntry_t canDevTable[MIB_canDevTable_SIZE];
} mibstat_canMib_t;

/**
   Get CAN status.

   @param[in] service Instance of MIB service
   @param[out] value CAN status value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mibstat_get_canMib(mib_service_t *service,
            mibstat_canMib_t *value);

/**
   Reset CAN status.

   @param[in] service Instance of MIB service

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mibstat_reset_canMib(mib_service_t *service);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_MIBS_CAN_MIBSTAT_H */
