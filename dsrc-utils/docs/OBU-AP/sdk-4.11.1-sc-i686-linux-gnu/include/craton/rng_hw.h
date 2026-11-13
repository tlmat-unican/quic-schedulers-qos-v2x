/* Copyright (C) 2014-2015 Autotalks Ltd. */
#ifndef _CRATON_RNG_HW_H
#define _CRATON_RNG_HW_H

#include <atlk/sdk.h>

#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   CRATON RNG HW API
*/

/**
   Init RNG module.

   @warning Should be called once only.

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
rng_hw_init(void);

/**
   Get a vector of uniformly distributed random 32-bit words.

   @param[in] vector Buffer to store the random 32-bit words
   @param[in] size Buffer size in 32-bit words

   @retval ::ATLK_OK if succeeded
   @retval ::ATLK_E_INVALID_STATE if RNG is not initialized
   @retval ::ATLK_E_INVALID_ARG if mandatory function argument is not specified
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
rng_hw_get(uint32_t *vector, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_RNG_HW_H */
