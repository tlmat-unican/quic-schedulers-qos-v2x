/* Copyright (C) 2015 Autotalks Ltd. */
#ifndef _CRATON_V2X_EMULATOR_INIT_H
#define _CRATON_V2X_EMULATOR_INIT_H

#include <atlk/sdk.h>
#include <atlk/os.h>
#include <atlk/v2x_emulator.h> 

#include <craton/imq.h>
#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   CRATON V2X emulator initialization API
*/

/** V2X emulator configuration descriptor */
typedef struct {
  /** Thread scheduling parameters */
  atlk_thread_sched_t sched_params;

#ifndef __CRATON_NO_ARC
  /** IMQ address*/
  imq_address_t imq_address;
#endif /* __CRATON_NO_ARC */

} v2x_emulator_config_t;

/** V2X emulator internal configuration descriptor default initializer */
#ifndef __CRATON_NO_ARC
#define V2X_EMULATOR_CONFIG_INIT {             \
  .sched_params = ATLK_THREAD_SCHED_INIT,      \
  .imq_address = IMQ_ADDRESS_NA,               \
};
#else /* __CRATON_NO_ARC */
#define V2X_EMULATOR_CONFIG_INIT {             \
  .sched_params = ATLK_THREAD_SCHED_INIT,      \
};
#endif /* __CRATON_NO_ARC */

/**
   Initialize V2X emulator master.

   Should be called in the CPU in which the V2X emulator is used.

   @remarks
      1. Init should be called before any other emulator function call 
      2. Function should be called only once

   @param[in,out] emulator_ptr V2X emulator
   @param[in] config Emulator configuration

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
v2x_emulator_master_init(v2x_emulator_t **emulator_ptr,
            const v2x_emulator_config_t *config);

/**
   Initialize V2X emulator slave.

   Should be called in the CPU in which the V2X service is used.

   @remarks
      1. Init should be called before any other emulator function call 
      2. Function should be called only once

   @param[in] config Emulator configuration

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
v2x_emulator_slave_init(const v2x_emulator_config_t *config);

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_V2X_EMULATOR_INIT_H */
