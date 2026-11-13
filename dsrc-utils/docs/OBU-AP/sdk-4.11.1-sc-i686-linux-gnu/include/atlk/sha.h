/* Copyright (C) 2014-2016 Autotalks Ltd. */
#ifndef _ATLK_SHA_H
#define _ATLK_SHA_H

#include <atlk/sdk.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   SHA common definitions
*/

/** SHA-224 digest size in octets */
#define SHA_224_DIGEST_SIZE 28

/** SHA-256 digest size in octets */
#define SHA_256_DIGEST_SIZE 32

/** SHA-384 digest size in octets */
#define SHA_384_DIGEST_SIZE 48

/** Maximum SHA digest size in octets */
#define SHA_DIGEST_MAX_SIZE SHA_384_DIGEST_SIZE

/** SHA digest */
typedef struct {
  uint8_t value[SHA_DIGEST_MAX_SIZE];
  size_t value_size;
} sha_digest_t;

/** SHA digest default initializer */
#define SHA_DIGEST_INIT { \
  .value = { 0 },         \
  .value_size = 0         \
}

/** SHA algorithm */
typedef enum {
  /** SHA-224 */
  SHA_224 = 0,

  /** SHA-256 */
  SHA_256 = 1,

  /** SHA-384 */
  SHA_384 = 2

} sha_algorithm_t;

/**
   Check if a given SHA algorithm is valid.

   @param[in] algorithm SHA algorithm

   @return 1 if SHA algorithm is valid, 0 otherwise
*/
atlk_inline int
sha_algorithm_valid(sha_algorithm_t algorithm)
{
  switch (algorithm) {
    case SHA_224:
    case SHA_256:
    case SHA_384:
      return 1;
  }
  return 0;
}

/**
   Get SHA digest size for a given SHA algorithm.

   @param[in] algorithm SHA algorithm

   @return SHA digest size for a valid SHA algorithm, 0 otherwise
*/
atlk_inline size_t
sha_algorithm_digest_size(sha_algorithm_t algorithm)
{
  switch (algorithm) {
    case SHA_224: return SHA_224_DIGEST_SIZE;
    case SHA_256: return SHA_256_DIGEST_SIZE;
    case SHA_384: return SHA_384_DIGEST_SIZE;
  }
  return 0;
}

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_SHA_H */
