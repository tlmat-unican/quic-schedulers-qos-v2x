/* Copyright (C) 2016 Autotalks Ltd. */
#ifndef _ATLK_MIBS_PROFILING_MIBSTAT_H
#define _ATLK_MIBS_PROFILING_MIBSTAT_H

#include <atlk/mib_service.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   Profiling Status API.
*/

/** Profiling thread name max length */
#define MIB_profilingThreadName_SIZE_MAX 32

/** Profiling thread status */
typedef struct {
  /** Count of cycles used by thread */
  uint64_t profilingThreadCyclesCnt;

  /** Thread name */
  char profilingThreadName[MIB_profilingThreadName_SIZE_MAX];

} mibstat_profilingThreadEntry_t;

/** Profiling threads table size */
#define MIB_profilingThreadsTable_SIZE 32

/** Profiling status */
typedef struct {
  /** Total number of cycles since last reset on ARM CPU */
  uint64_t profilingTotalArmCnt;

  /** Cycles in idle state on ARM CPU */
  uint64_t profilingIdleArmCnt;

  /** Cycles consumed by ISRs on ARM CPU */
  uint64_t profilingIsrArmCnt;

  /** Status per thread on ARM CPU **/
  mibstat_profilingThreadEntry_t
    profilingThreadsArm[MIB_profilingThreadsTable_SIZE];

  /** Total number of cycles since last reset on ARC1 CPU */
  uint64_t profilingTotalArc1Cnt;

  /** Cycles in idle state on ARC1 CPU */
  uint64_t profilingIdleArc1Cnt;

  /** Cycles consumed by ISRs on ARC1 CPU */
  uint64_t profilingIsrArc1Cnt;

  /** Status per thread on ARC1 CPU **/
  mibstat_profilingThreadEntry_t
    profilingThreadsArc1[MIB_profilingThreadsTable_SIZE];

  /** Total number of cycles since last reset on ARC2 CPU */
  uint64_t profilingTotalArc2Cnt;

  /** Cycles in idle state on ARC2 CPU */
  uint64_t profilingIdleArc2Cnt;

  /** Cycles consumed by ISRs on ARC2 CPU */
  uint64_t profilingIsrArc2Cnt;

  /** Status per thread on ARC2 CPU **/
  mibstat_profilingThreadEntry_t
    profilingThreadsArc2[MIB_profilingThreadsTable_SIZE];

} mibstat_profilingMib_t;

/**
   Get profiling status.

   @param[in] service Instance of MIB service
   @param[out] value Profiling status value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mibstat_get_profilingMib(mib_service_t *service,
            mibstat_profilingMib_t *value);

/**
   Reset profiling status.

   @param[in] service Instance of MIB service

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mibstat_reset_profilingMib(mib_service_t *service);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_MIBS_PROFILING_MIBSTAT_H */
