/* Copyright (C) 2014-2016 Autotalks Ltd. */
#ifndef _ATLK_RNG_H
#define _ATLK_RNG_H

#include <atlk/sdk.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   RNG API
*/

/**
   Get random bytes.

   @param[in] ptr Buffer to store random bytes
   @param[in] size Buffer length in octets

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
rng_data_get(void *ptr, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_RNG_H */
