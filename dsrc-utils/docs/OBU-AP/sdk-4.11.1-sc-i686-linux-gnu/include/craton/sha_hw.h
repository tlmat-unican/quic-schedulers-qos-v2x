/* Copyright (C) 2014-2015 Autotalks Ltd. */
#ifndef _CRATON_SHA_HW_H
#define _CRATON_SHA_HW_H

#include <atlk/sdk.h>
#include <atlk/sha.h>

#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   CRATON SHA HW API
*/

/**
   Initialize SHA hardware driver.
   
   @warning Should be called once only.

   @retval ::ATLK_OK if succeeded
   @retval ::ATLK_E_INVALID_STATE if SHA is already initialized
*/
atlk_rc_t atlk_must_check
sha_hw_init(void);

/**
   Compute SHA-224 using dedicated hardware.

   Only the first sha_digest_t::value_size octets of sha_digest_t::value
   are the calculated hash value.

   @param[in] data_ptr Data over which the hash will be computed
   @param[in] data_size Data length in octets
   @param[out] digest Calculated SHA-224 digest

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
sha_hw_sha224_compute(const void *data_ptr,
            size_t data_size,
            sha_digest_t *digest);

/**
   Compute SHA-256 using dedicated hardware.

   Only the first sha_digest_t::value_size octets of sha_digest_t::value
   are the calculated hash value.

   @param[in] data_ptr Data over which the hash will be computed
   @param[in] data_size Data length in octets
   @param[out] digest Calculated SHA-256 digest

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
sha_hw_sha256_compute(const void *data_ptr,
            size_t data_size,
            sha_digest_t *digest);

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_SHA_HW_H */
