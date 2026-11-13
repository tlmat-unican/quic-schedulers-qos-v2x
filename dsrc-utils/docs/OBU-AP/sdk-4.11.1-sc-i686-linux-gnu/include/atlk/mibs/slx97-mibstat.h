/* Copyright (C) 2016 Autotalks Ltd. */
#ifndef _ATLK_MIBS_SLX97_MIBSTAT_H
#define _ATLK_MIBS_SLX97_MIBSTAT_H

#include <atlk/mib_service.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   SLx97 Status API.
*/

/** SLx97 status */
typedef struct {
  /** Count of commands written */
  uint32_t slx97CmdWriteCnt;

  /** Count of executed commands */
  uint32_t slx97CmdExeCnt;

  /** Count of responses read */
  uint32_t slx97RspReadCnt;

  /** Count of command CRC errors */
  uint32_t slx97CmdCrcErrorCnt;

  /** Count of response CRC errors */
  uint32_t slx97RspCrcErrorCnt;

  /** Count of commands with no transport security */
  uint32_t slx97CmdNoSecCnt;

  /** Count of commands with transport security */
  uint32_t slx97CmdSecCnt;

  /** Count of responses with no transport security */
  uint32_t slx97RspNoSecCnt;

  /** Count of responses with transport security */
  uint32_t slx97RspSecCnt;

  /** Count of error responses */
  uint32_t slx97RspErrorCnt;

} mibstat_slx97Mib_t;

/**
   Get SLx97 status.

   @param[in] service Instance of MIB service
   @param[out] value SLx97 status value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mibstat_get_slx97Mib(mib_service_t *service,
         mibstat_slx97Mib_t *value);

/**
   Reset SLx97 status.

   @param[in] service Instance of MIB service

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mibstat_reset_slx97Mib(mib_service_t *service);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_MIBS_SLX97_MIBSTAT_H */
