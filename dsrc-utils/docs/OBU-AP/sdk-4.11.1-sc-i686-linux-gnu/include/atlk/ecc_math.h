/* Copyright (C) 2016 Autotalks Ltd. */
#ifndef _ATLK_ECC_MATH_H
#define _ATLK_ECC_MATH_H

#include <atlk/sdk.h>
#include <atlk/ecc.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   ECC API functions
*/

/**
   Check if elliptic curve point is valid.

   @param[in] curve Elliptic curve associated with the point
   @param[in] point Elliptic curve point to check

   @retval 1 if @p point is valid on @p curve, 0 otherwise
*/
int
ecc_point_valid(ecc_curve_t curve,
            const ecc_point_t *point);

/**
   Compress representation of point on an elliptic curve.

   If point is already compressed, copy it's X coordinate value as-is.
   It is allowed that @p uncompressed be equal to @p compressed in order to
   compress a point in-place.

   @param[in] uncompressed Elliptic curve point to compress
   @param[out] compressed Compressed elliptic curve point

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
ecc_point_compress(const ecc_point_t *uncompressed,
            ecc_point_t *compressed);

/**
   Decompress representation of point on an elliptic curve.

   If point is already uncompressed, copy it's value as-is.
   It is allowed that @p compressed be equal to @p decompressed in order to
   decompress a point in-place.

   @param[in] curve Elliptic curve associated with the points
   @param[in] compressed Compressed elliptic curve point
   @param[out] decompressed Decompressed elliptic curve point

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
ecc_point_decompress(ecc_curve_t curve,
            const ecc_point_t *compressed,
            ecc_point_t *decompressed);

/**
   Perform a multiply-add on elliptic curve point.

   Does the following operation: Q := R + (e * P)

   This operation can be used for public key extraction in ECQV PKI scheme.

   @param[in] curve Elliptic curve associated with the points
   @param[in] P Elliptic curve point
   @param[in] e Scalar for multiplication
   @param[in] R Elliptic curve point for addition
   @param[out] Q Calculated elliptic curve point

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
ecc_point_multiply_add(ecc_curve_t curve,
            const ecc_point_t *P,
            const ecc_scalar_t *e,
            const ecc_point_t *R,
            ecc_point_t *Q);

/**
   Perform a modular multiply-add a private key.

   Does the following operation: k' := b + (a * k) mod n

   Where:
   * k is @p private_key
   * k' is @p result
   * b is @p addend
   * a is @p multiplier
   * n is order of the elliptic curve group that is specified by @p curve

   This operation can be used to implement PKI schemes such as SCMS.

   @remark @p multiplier is not allowed to be zero modulo the elliptic
   curve order (n).

   @param[in] curve Elliptic curve
   @param[in] private_key Private key
   @param[in] addend Scalar for addition
   @param[in] multiplier Scalar for multiplication
   @param[out] result Calculated private key

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
ecc_private_key_multiply_add(ecc_curve_t curve,
            const ecc_scalar_t *private_key,
            const ecc_scalar_t *addend,
            const ecc_scalar_t *multiplier,
            ecc_scalar_t *result);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_ECC_MATH_H */
