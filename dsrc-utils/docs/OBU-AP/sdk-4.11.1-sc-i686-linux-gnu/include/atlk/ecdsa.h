/* Copyright (C) 2014-2016 Autotalks Ltd. */
#ifndef _ATLK_ECDSA_H
#define _ATLK_ECDSA_H

#include <atlk/sdk.h>
#include <atlk/ecc.h>
#include <atlk/sha.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   ECDSA API
*/

/** ECDSA signature alias */
typedef ecc_signature_t ecdsa_signature_t;

/** ECDSA signature default initializer alias */
#define ECDSA_SIGNATURE_INIT ECC_SIGNATURE_INIT

/** ECDSA signature for fast verification alias */
typedef ecc_fast_verification_signature_t ecdsa_fast_verification_signature_t;

/** ECDSA signature for fast verification default initializer alias */
#define ECDSA_FAST_VERIFICATION_SIGNATURE_INIT \
  ECC_FAST_VERIFICATION_SIGNATURE_INIT

/**
   Convert ECDSA signature for fast verification to a regular ECDSA signature.

   @param[in] curve Elliptic curve associated with the signature
   @param[in] fv_signature ECDSA signature for fast verification
   @param[out] signature ECDSA signature

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
ecdsa_signature_convert(ecc_curve_t curve,
            const ecc_fast_verification_signature_t *fv_signature,
            ecc_signature_t *signature);

/**
   Generate ECDSA fast verification signature.

   @param[in] curve Elliptic curve associated with the private key
   @param[in] private_key ECDSA private key to use for signing
   @param[in] digest SHA digest to be signed
   @param[out] fv_signature ECDSA signature for fast verification

   @remark This function may be implemented using just software or accelerated
   using dedicated hardware, depending on library implementation.

   @remark This function expects to receive the private key in plain and thus
   is not suitable for a tamper-resistant implementation. Tamper-resistant
   implementations should use ::hsm_ecdsa_sign instead.

   @see ::hsm_ecdsa_sign
   @see ::ecc_request_send
   @see ::ecc_response_receive

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
ecdsa_digest_sign(ecc_curve_t curve,
            const ecc_scalar_t *private_key,
            const sha_digest_t *digest,
            ecc_fast_verification_signature_t *fv_signature);

/**
   Verify ECDSA signature.

   @param[in] curve Elliptic curve associated with the public key
   @param[in] public_key ECDSA public key to use for verification
   @param[in] digest SHA digest to be verified
   @param[in] signature ECDSA signature to be verified
   @param[in] rc ECC return code

   @remark A return value of ::ATLK_OK only indicates that the verification
   completed with some result. The user must inspect *rc and
   compare it against ::ECC_OK in order to know whether
   the signature is correct or incorrect.

   @remark This function may be implemented using just software or accelerated
   using dedicated hardware, depending on library implementation.

   @see ::ecc_request_send
   @see ::ecc_response_receive

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
ecdsa_digest_verify(ecc_curve_t curve,
            const ecc_point_t *public_key,
            const sha_digest_t *digest,
            const ecc_signature_t *signature,
            ecc_rc_t *rc);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_ECDSA_H */
