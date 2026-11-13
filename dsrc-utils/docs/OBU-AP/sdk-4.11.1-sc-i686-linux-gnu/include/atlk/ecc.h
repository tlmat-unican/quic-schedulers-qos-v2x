/* Copyright (C) 2014-2016 Autotalks Ltd. */
#ifndef _ATLK_ECC_H
#define _ATLK_ECC_H

#include <atlk/sdk.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   ECC API declarations
*/

/** Supported elliptic curves */
typedef enum {
  /** NIST P-224 */
  ECC_CURVE_NIST_P224 = 0,

  /** NIST P-224 alias */
  ECC_CURVE_P224 = ECC_CURVE_NIST_P224,

  /** NIST P-256 */
  ECC_CURVE_NIST_P256 = 1,

  /** NIST P-256 alias */
  ECC_CURVE_P256 = ECC_CURVE_NIST_P256,

  /** Brainpool P-256t1 */
  ECC_CURVE_BRAINPOOL_P256t1 = 2,

  /** Brainpool P-256r1 */
  ECC_CURVE_BRAINPOOL_P256r1 = 3,

  /** NIST P-384 */
  ECC_CURVE_NIST_P384 = 4,

  /** Brainpool P-384t1 */
  ECC_CURVE_BRAINPOOL_P384t1 = 5,

  /** Brainpool P-384r1 */
  ECC_CURVE_BRAINPOOL_P384r1 = 6,

} ecc_curve_t;

/** Size of ECC P-224 private key in octets */
#define ECC_CURVE_P224_KEY_SIZE 28

/** Size of ECC P-256 private key in octets */
#define ECC_CURVE_P256_KEY_SIZE 32

/** Size of ECC P-384 private key in octets */
#define ECC_CURVE_P384_KEY_SIZE 48

/** Number of 32-bit words in ECC big integer */
#define ECC_SCALAR_NUM_OF_UINT32 12

/** Big integer type for use with ECC */
typedef struct {
  /**
    Unsigned integer in base 2^32

    Note: Least significant word appears first, most significant word appears
    last. All words are in native endianness.
  */
  uint32_t value[ECC_SCALAR_NUM_OF_UINT32];

} ecc_scalar_t;

/** ECC scalar default initializer */
#define ECC_SCALAR_INIT { .value = { 0 } }

/** Elliptic curve point representation type */
typedef enum {
  /** Compressed, LSB of Y coordinate is 0 */
  ECC_POINT_COMPRESSED_LSB_Y_0 = 2,

  /** Compressed, LSB of Y coordinate is 1 */
  ECC_POINT_COMPRESSED_LSB_Y_1 = 3,

  /** Uncompressed, Y coordinate is stored as-is */
  ECC_POINT_UNCOMPRESSED = 4

} ecc_point_type_t;

/** Point on an elliptic curve */
typedef struct {
  /** Point representation type */
  ecc_point_type_t point_type;

  /** X coordinate */
  ecc_scalar_t x_coordinate;

  /**
    Y coordinate

    Valid only if @p type is equal to ecc_point_type_t::ECC_POINT_UNCOMPRESSED.
  */
  ecc_scalar_t y_coordinate;

} ecc_point_t;

/** ECC point default initializer */
#define ECC_POINT_INIT {                    \
  .point_type = ECC_POINT_UNCOMPRESSED,     \
  .x_coordinate = ECC_SCALAR_INIT,          \
  .y_coordinate = ECC_SCALAR_INIT           \
}

/** ECDSA signature */
typedef struct {
  /** r scalar */
  ecc_scalar_t r_scalar;

  /** s scalar */
  ecc_scalar_t s_scalar;

} ecc_signature_t;

/** ECDSA signature default initializer */
#define ECC_SIGNATURE_INIT {        \
  .r_scalar = ECC_SCALAR_INIT,      \
  .s_scalar = ECC_SCALAR_INIT       \
}

/** ECDSA signature for fast verification */
typedef struct {
  /** R point */
  ecc_point_t R_point;

  /** s scalar */
  ecc_scalar_t s_scalar;

} ecc_fast_verification_signature_t;

/** ECDSA signature for fast verification default initializer */
#define ECC_FAST_VERIFICATION_SIGNATURE_INIT {    \
  .R_point = ECC_POINT_INIT,                      \
  .s_scalar = ECC_SCALAR_INIT                     \
}

/** ECC service */
typedef struct ecc_service ecc_service_t;

/** ECC return code */
typedef enum {
  /** Operation successful */
  ECC_OK = 0,

  /** Unspecified error */
  ECC_E_UNSPECIFIED = 1,

  /** Signature verification failure */
  ECC_E_VERIFY_FAILED = 2,

  /** Signature generation failure (r = 0 or s = 0) */
  ECC_E_SIGN_FAILED = 3,

  /** Point validation failure - point not on the curve */
  ECC_E_INVALID_POINT = 4,

  /** Point at infinity */
  ECC_E_POINT_AT_INFINITY = 5,

  /** Invalid modulus for point decompression */
  ECC_E_DECOMPRESSION_FAILED = 6

} ecc_rc_t;

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_ECC_H */
