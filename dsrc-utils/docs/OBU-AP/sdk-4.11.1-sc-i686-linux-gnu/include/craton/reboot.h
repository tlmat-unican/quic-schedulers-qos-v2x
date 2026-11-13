/* Copyright (C) 2015-2016 Autotalks Ltd. */
#ifndef _CRATON_REBOOT_H
#define _CRATON_REBOOT_H

#include <atlk/sdk.h>

#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   Reboot API
*/

/** Reboot method specifier */
typedef enum {
  /** System-On-Chip reboot - reboot CPU only */
  SYSTEM_REBOOT_SOC = 0,

  /** PCB reboot - reboot entire system (not supported on all boards) */
  SYSTEM_REBOOT_PCB = 1

} system_reboot_t;

/**
   Reboot the system.

   @param[in] reboot_type Reboot method specifier

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
system_reboot(system_reboot_t reboot_type);

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_REBOOT_H */
