/* Copyright (C) 2014-2016 Autotalks Ltd. */
#ifndef _ATLK_SHA_SW
#define _ATLK_SHA_SW

#include <atlk/sdk.h>
#include <atlk/sha.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   Autotalks SHA SW API
*/

/**
   Compute SHA digest using just software.

   Only the first sha_digest_t::value_size octets of sha_digest_t::value
   are the calculated hash value.

   @param[in] algorithm SHA algorithm
   @param[in] data_ptr Data over which the hash will be computed
   @param[in] data_size Data length in octets
   @param[out] digest Calculated SHA digest

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
sha_sw_compute(sha_algorithm_t algorithm,
               const void *data_ptr,
               size_t data_size,
               sha_digest_t *digest);

/**
   Compute SHA-224 using just software.

   Only the first sha_digest_t::value_size octets of sha_digest_t::value
   are the calculated hash value.

   @param[in] data_ptr Data over which the hash will be computed
   @param[in] data_size Data length in octets
   @param[out] digest Calculated SHA-224 digest

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
sha_sw_sha224_compute(const void *data_ptr,
            size_t data_size,
            sha_digest_t *digest);

/**
   Compute SHA-256 using just software.

   Only the first sha_digest_t::value_size octets of sha_digest_t::value
   are the calculated hash value.

   @param[in] data_ptr Data over which the hash will be computed
   @param[in] data_size Data length in octets
   @param[out] digest Calculated SHA-256 digest

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
sha_sw_sha256_compute(const void *data_ptr,
            size_t data_size,
            sha_digest_t *digest);

/**
   Compute SHA-384 using just software.

   Only the first sha_digest_t::value_size octets of sha_digest_t::value
   are the calculated hash value.

   @param[in] data_ptr Data over which the hash will be computed
   @param[in] data_size Data length in octets
   @param[out] digest Calculated SHA-384 digest

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
sha_sw_sha384_compute(const void *data_ptr,
            size_t data_size,
            sha_digest_t *digest);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_SHA_SW */
