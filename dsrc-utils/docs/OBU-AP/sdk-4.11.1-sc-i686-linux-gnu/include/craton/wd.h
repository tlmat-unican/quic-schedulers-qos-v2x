/* Copyright (C) 2015 Autotalks Ltd. */
#ifndef _CRATON_WD_H
#define _CRATON_WD_H

#include <atlk/sdk.h>

#include <craton/exception_arm.h>
#include <craton/exception_arc.h>

#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   Watchdog (WD) API
*/

/** Number of ARC cores */
#define WD_ARC_CORES_COUNT 2

/** WD expiration mode */
typedef enum {
  /** Restart system upon WD expiration */
  WD_MODE_RESTART = 0,

  /** Invoke user callback upon WD expiration */
  WD_MODE_CB = 1

} wd_mode_t;

/**
   WD expiration callback for ARC.

   @param[in] arc_num ARC number, one of {1,2}
   @param[out] info Exception info
*/
typedef void
(*wd_arc_exception_handler_t)(uint8_t arc_num,
            const exception_arc_info_t *info);

/**
   WD expiration callback for ARM.

   @warning Callback is called from ISR context.

   @param[out] info Exception info
*/
typedef void
(*wd_arm_exception_handler_t)(const exception_arm_info_t *info);

/** WD configuration for ARC */
typedef struct {
  /** Enable WD on ARC */
  int wd_enabled;

  /**
    WD expiration callback on ARC.
    
    Default handler is used when set to NULL and WD is enabled.
  */
  wd_arc_exception_handler_t handler;

} wd_arc_config_t;

/** WD configuration for ARM */
typedef struct {
  /** WD expiration mode */
  wd_mode_t wd_mode;

  /**
    WD expiration callback on ARM.
    
    Default handler is used when set to NULL and WD is enabled.
  */
  wd_arm_exception_handler_t handler;

  /** ARC cores WD configuration */
  wd_arc_config_t arc_configs[WD_ARC_CORES_COUNT];

} wd_config_t;

#ifdef __CRATON_ARM

/**
   Initialize WD on ARM.

   @param[in] config WD configuration parameters

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
wd_init(const wd_config_t *config);

#else /* __CRATON_ARM */

/**
   Initialize WD on ARC.

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
wd_init(void);

#endif /* __CRATON_ARM */

/**
   Set whether WD is enabled.

   @param[in] enabled WD is enabled
   
   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
wd_enabled_set(int enabled);

/**
   Get whether WD is enabled.

   @param[out] enabled WD is enabled

   @retval ::ATLK_OK if succeeded
   @return Error if failed
*/
atlk_rc_t atlk_must_check
wd_enabled_get(int *enabled);

/**
   Trigger system restart via WD.
*/
void
wd_chip_reset(void) atlk_no_return;

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_WD_H */
