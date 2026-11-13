/* Copyright (C) 2014-2016 Autotalks Ltd. */
#ifndef _ATLK_ECC_SERVICE_H
#define _ATLK_ECC_SERVICE_H

#include <atlk/sdk.h>
#include <atlk/ecc.h>
#include <atlk/ecdsa.h>
#include <atlk/sha.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   ECC service API
*/

/**
   Get pointer to default ECC service.

   @param[out] service_ptr Pointer to ECC service

   @note Not supported by remote service library.

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
ecc_default_service_get(ecc_service_t **service_ptr);

/**
   Delete ECC service.

   @param[in] service ECC service to delete

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t
ecc_service_delete(ecc_service_t *service);

/** ECC request ID */
typedef uint32_t ecc_request_id_t;

/** ECC request type */
typedef enum {
  /** ECDSA verify */
  ECC_REQUEST_TYPE_VERIFY = 0,

  /** ECDSA sign */
  ECC_REQUEST_TYPE_SIGN = 1,

  /** Elliptic curve point multiply-add */
  ECC_REQUEST_TYPE_PMA = 2,

} ecc_request_type_t;

/** ECC request context */
typedef struct {
  /** Request ID */
  ecc_request_id_t request_id;

  /** Request type */
  ecc_request_type_t request_type;

  /** Elliptic curve identifier */
  ecc_curve_t curve;

} ecc_request_context_t;

/** ECDSA verification parameters */
typedef struct {
  /** Public key */
  ecc_point_t public_key;

  /** Hash digest */
  sha_digest_t digest;

  /** ECDSA signature */
  ecc_signature_t signature;

} ecc_verify_params_t;

/** ECDSA signing parameters */
typedef struct {
  /** Private key */
  ecc_scalar_t private_key;

  /** Hash digest */
  sha_digest_t digest;

} ecc_sign_params_t;

/** Elliptic curve point multiply-add parameters */
typedef struct {
  /** ECC point to be multiplied */
  ecc_point_t point;

  /** Scalar to multiply by */
  ecc_scalar_t multiplier;

  /** ECC point to add */
  ecc_point_t addend;

} ecc_pma_params_t;

/** ECC request */
typedef struct {
  /** Request context */
  ecc_request_context_t context;

  union {
    /** ECDSA verification parameters */
    ecc_verify_params_t verify_params;

    /** ECDSA signing parameters */
    ecc_sign_params_t sign_params;

    /** Elliptic curve point multiply-add parameters */
    ecc_pma_params_t pma_params;

  } params;

} ecc_request_t;

/** ECC request default initializer */
#define ECC_REQUEST_INIT {            \
  .context = { 0 },                   \
  .params.verify_params = {           \
    .public_key = ECC_POINT_INIT,     \
    .digest = SHA_DIGEST_INIT,        \
    .signature = ECC_SIGNATURE_INIT   \
  }                                   \
}

/** ECC response */
typedef struct {
  /** Original request context */
  ecc_request_context_t context;

  /**
     ECC return code.

     Specifically, if the original request was for ECDSA verification,
     then the supplied ECDSA signature was found valid if and only if
     the return code is equal to ::ECC_OK.
  */
  ecc_rc_t rc;

  /**
     Response result.

     Valid only if ecc_response_t::rc is equal to ::ECC_OK.
  */
  union {
    /**
       ECDSA signature for fast verification.

       Value is valid only if ecc_response_t::context::request_type
       is equal to ::ECC_REQUEST_TYPE_SIGN.
    */
    ecc_fast_verification_signature_t sign_result;

    /**
       Elliptic curve point after multiply-add operation.

       Value is valid only if ecc_response_t::context::request_type
       is equal to ::ECC_REQUEST_TYPE_PMA.
    */
    ecc_point_t pma_result;

  } result;

} ecc_response_t;

/** ECC response default initializer */
#define ECC_RESPONSE_INIT {                                     \
  .context = { 0 },                                             \
  .rc = ECC_OK,                                                 \
  .result.sign_result = ECC_FAST_VERIFICATION_SIGNATURE_INIT    \
}

/** ECC socket */
typedef struct ecc_socket ecc_socket_t;

/**
   Create ECC socket.

   @param[in] service ECC service
   @param[out] socket_ptr ECC socket

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
ecc_socket_create(ecc_service_t *service,
            ecc_socket_t **socket_ptr);

/**
   Delete ECC socket.

   @param[in] socket ECC socket to delete

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t
ecc_socket_delete(ecc_socket_t *socket);

/**
   Send ECC request.

   @see @ref wait_usage.

   @param[in] socket ECC socket
   @param[in] request ECC request
   @param[in] wait Wait specification (optional)

   @todo Currently @p wait is not supported -- operation is always non-blocking.

   @see ::ecdsa_verify_digest

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
ecc_request_send(ecc_socket_t *socket,
            const ecc_request_t *request,
            const atlk_wait_t *wait);

/**
   Receive ECC response.

   @see @ref wait_usage.

   @param[in] socket ECC socket
   @param[out] response ECC response
   @param[in] wait Wait specification (optional)

   @todo Currently @p wait supports non-blocking or wait-forever options only.
   @todo Currently @p wait supports wait-forever option only when using
   libsectonhost.

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
ecc_response_receive(ecc_socket_t *socket,
            ecc_response_t *response,
            const atlk_wait_t *wait);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_ECC_SERVICE_H */
