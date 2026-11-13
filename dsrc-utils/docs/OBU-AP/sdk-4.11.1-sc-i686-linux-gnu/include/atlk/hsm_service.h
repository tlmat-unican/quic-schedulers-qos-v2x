/* Copyright (C) 2014-2016 Autotalks Ltd. */
#ifndef _ATLK_HSM_SERVICE_H
#define _ATLK_HSM_SERVICE_H

#include <atlk/sdk.h>
#include <atlk/ecc.h>
#include <atlk/ecdsa.h>
#include <atlk/hsm.h>
#include <atlk/sha.h>
#include <atlk/aes.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   HSM service API
*/

/**
   Get pointer to default HSM service.

   @param[out] service_ptr Pointer to HSM service

   @remark Should be implemented by system integration code.

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
hsm_default_service_get(hsm_service_t **service_ptr);

/**
   Delete HSM service.

   @param[in] service HSM service to be deleted

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t
hsm_service_delete(hsm_service_t *service);

/** HSM capability information */
typedef struct {
  /** Maximum number of NVM cells supported */
  uint32_t max_num_of_cells;

  /** Current number of NVM cells configured */
  uint32_t current_num_of_cells;

  /** 
     Maximum number of cell ranges supported by
     ::hsm_csr_ecdsa_public_keys_sign
  */
  uint32_t max_num_of_cell_ranges_for_csr;

  /** Maximum ECIES key size supported by ::hsm_ecies_key_derive */
  uint32_t max_ecies_key_size;

  /**
     Maximum ECIES key derivation parameter size supported by
     ::hsm_ecies_key_derive
  */
  uint32_t max_ecies_kdf_param_size;

} hsm_capability_info_t;

/** HSM capability info default initializer */
#define HSM_CAPABILITY_INFO_INIT {                      \
  .max_num_of_cells = 0,                                \
  .current_num_of_cells = 0,                            \
  .max_num_of_cell_ranges_for_csr = 0,                  \
  .max_ecies_key_size = 0,                              \
  .max_ecies_kdf_param_size = 0,                        \
}

/**
   Get HSM capability information.

   @param[in] service HSM service instance
   @param[out] capability_info HSM capability information

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
hsm_capability_info_get(hsm_service_t *service,
            hsm_capability_info_t *capability_info);

/** HSM NVM configuration */
typedef struct {
  /** Number of used NVM cells */
  uint32_t num_of_cells;

} hsm_nvm_config_t;

/** HSM NVM configuration default initializer */
#define HSM_NVM_CONFIG_INIT {   \
  .num_of_cells = 0,            \
}

/**
   Initialize or re-initialize HSM NVM.

   @param[in] service HSM service instance
   @param[in] config HSM NVM configuration

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
hsm_nvm_init(hsm_service_t *service,
            const hsm_nvm_config_t *config);

/** Private key type */
typedef enum {
  /** Private key that cannot interact with other private keys */
  HSM_PRIVATE_KEY_TYPE_ISOLATED = 0,

  /** Private key whose public key conterpart can be a member of a CSR */
  HSM_PRIVATE_KEY_TYPE_CSR_MEMBER = 1,

  /** Private key that can be used to sign a CSR */
  HSM_PRIVATE_KEY_TYPE_CSR_SIGNER = 2,

  /** Private key that can be input to ::hsm_ecc_private_key_multiply_add */
  HSM_PRIVATE_KEY_TYPE_MA_INPUT = 3,

  /** Private key that is the output of ::hsm_ecc_private_key_multiply_add */
  HSM_PRIVATE_KEY_TYPE_MA_OUTPUT = 4

} hsm_private_key_type_t;

/** Public key algorithm */
typedef enum {
  /** Public key for ECDSA */
  HSM_PUBLIC_KEY_ALGORITHM_ECDSA = 0,

  /** Public key for ECIES */
  HSM_PUBLIC_KEY_ALGORITHM_ECIES = 1

} hsm_public_key_algorithm_t;

/** Private key information */
typedef struct {
  /** Elliptic curve used with this key */
  ecc_curve_t key_curve;

  /** Type of key */
  hsm_private_key_type_t key_type;

  /** Intended algorithm for key */
  hsm_public_key_algorithm_t key_algorithm;

} hsm_ecc_private_key_info_t;

/** Private key information default initializer */
#define HSM_ECC_PRIVATE_KEY_INFO_INIT {             \
  .key_curve = ECC_CURVE_NIST_P224,                 \
  .key_type = HSM_PRIVATE_KEY_TYPE_ISOLATED,        \
  .key_algorithm = HSM_PUBLIC_KEY_ALGORITHM_ECDSA   \
}

/**
   Import ECC private key.

   Store private key in the secure storage cell at @p private_key_index.

   @p private_key_info::key_type must be equal to
   ::HSM_PRIVATE_KEY_TYPE_ISOLATED.

   @param[in] service HSM service instance
   @param[in] private_key_index Index where private key should be stored
   @param[in] private_key_info Key information
   @param[in] private_key Private key value to be stored

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
hsm_ecc_private_key_import(hsm_service_t *service,
            hsm_cell_index_t private_key_index,
            const hsm_ecc_private_key_info_t *private_key_info,
            const ecc_scalar_t *private_key);

/**
   Create ECC private key.

   Private key is stored in the secure storage cell at @p private_key_index.

   @p private_key_info::key_type must be equal to one of
   ::HSM_PRIVATE_KEY_TYPE_ISOLATED, ::HSM_PRIVATE_KEY_TYPE_CSR_MEMBER,
   ::HSM_PRIVATE_KEY_TYPE_CSR_SIGNER or ::HSM_PRIVATE_KEY_TYPE_MA_INPUT.

   If @p private_key_info::key_type is equal to
   ::HSM_PRIVATE_KEY_TYPE_CSR_SIGNER then @p private_key_info::key_curve must
   be at least a 256-bit elliptic curve (e.g. NIST P-256) and
   @p private_key_info::key_algorithm must be equal to
   ::HSM_PUBLIC_KEY_ALGORITHM_ECDSA.

   @param[in] service HSM service instance
   @param[in] private_key_index Index where private key should be stored
   @param[in] private_key_info Key information

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
hsm_ecc_private_key_create(hsm_service_t *service,
            hsm_cell_index_t private_key_index,
            const hsm_ecc_private_key_info_t *private_key_info);

/**
   Get private key information.

   @param[in] service HSM service instance
   @param[in] private_key_index Index of private key
   @param[out] private_key_info Key information

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
hsm_ecc_private_key_info_get(hsm_service_t *service,
            hsm_cell_index_t private_key_index,
            hsm_ecc_private_key_info_t *private_key_info);

/**
   Return ECC public key that matches a stored private key.

   @param[in] service HSM service instance
   @param[in] private_key_index Index of stored private key
   @param[out] public_key Public key that matches the private key

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
hsm_ecc_public_key_get(hsm_service_t *service,
            hsm_cell_index_t private_key_index,
            ecc_point_t *public_key);

/**
   Generate ECDSA signature from a given hash digest.

   Note: when implementing standard ECDSA variants, if you use elliptic curve
   P-256 then @p digest should be computed using SHA-256; if you use P-224 then
   @p digest should be computed using SHA-224.

   The ::hsm_private_key_type_t of the key at @p private_key_index must be
   equal to one of ::HSM_PRIVATE_KEY_TYPE_CSR_MEMBER,
   ::HSM_PRIVATE_KEY_TYPE_MA_OUTPUT, or ::HSM_PRIVATE_KEY_TYPE_ISOLATED.

   The ::hsm_public_key_algorithm_t of the key at @p private_key_index must be
   equal to ::HSM_PUBLIC_KEY_ALGORITHM_ECDSA.

   @param[in] service HSM service instance
   @param[in] private_key_index Index of private key that should be used
   @param[in] digest To-be-signed hash digest
   @param[out] signature ECDSA fast verification signature

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
hsm_ecdsa_sign(hsm_service_t *service,
            hsm_cell_index_t private_key_index,
            const sha_digest_t *digest,
            ecc_fast_verification_signature_t *signature);

/**
   Derive ECIES key from a private key and peer public key.

   The ::ecc_curve_t of the key at @p private_key_index must be at least
   a 256-bit elliptic curve (e.g. NIST P-256).

   The ::hsm_private_key_type_t of the key at @p private_key_index must be
   equal to one of ::HSM_PRIVATE_KEY_TYPE_CSR_MEMBER,
   ::HSM_PRIVATE_KEY_TYPE_MA_OUTPUT, or ::HSM_PRIVATE_KEY_TYPE_ISOLATED.

   The ::hsm_public_key_algorithm_t of the key at @p private_key_index must be
   equal to ::HSM_PUBLIC_KEY_ALGORITHM_ECIES.

   @p kdf_param is an optional octet string used as a key derivation parameter.
   In order for the key derivation parameter to be the empty string,
   @p kdf_param_size must be 0.
   The key derivation parameter can be used to prevent misbinding attacks.
   Please refer to IEEE Std 1363a-2004 clause 11.3.2 where the key derivation
   parameter is denoted by P1.

   @param[in] service HSM service instance
   @param[in] private_key_index Index of private key that should be used
   @param[in] peer_public_key Public key of ECIES peer
   @param[out] key Derived ECIES key
   @param[in] key_size ECIES key size in octets
   @param[in] kdf_param Key derivation parameter (optional)
   @param[in] kdf_param_size Key derivation parameter size in octets

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
hsm_ecies_key_derive(hsm_service_t *service,
            hsm_cell_index_t private_key_index,
            const ecc_point_t *peer_public_key,
            void *key,
            size_t key_size,
            const void *kdf_param,
            size_t kdf_param_size);

/**
   Perform a modular multiply-add on stored private key and store the result.

   Does the following operation: k' := b + (a * k) mod n

   Where:
   * k is value at @p input_key_index
   * k' is value at @p output_key_index
   * b is @p key_addend
   * a is @p key_multiplier
   * n is order of the elliptic curve group that is specified by
     ::ecc_curve_t of the key at @p input_key_index

   The ::hsm_private_key_type_t of the key at @p input_key_index must be
   ::HSM_PRIVATE_KEY_TYPE_MA_INPUT.

   The newly created key at @p output_key_index will have the same
   ::ecc_curve_t and ::hsm_public_key_algorithm_t as the key at
   @p input_key_index but will have ::hsm_private_key_type_t equal to
   ::HSM_PRIVATE_KEY_TYPE_MA_OUTPUT.

   This operation can be used to implement PKI schemes such as SCMS.

   @remark @p key_multiplier is not allowed to be zero modulo the elliptic
   curve order (n) since allowing it will provide a method for importing
   private keys with ::hsm_private_key_type_t other than
   ::HSM_PRIVATE_KEY_TYPE_ISOLATED.

   @remark @p input_key_index is not allowed to be equal to @p output_key_index
   due to the idempotency requirement; i.e. invoking a procedure once and
   invoking the same procedure twice or more with the same inputs (without any
   other intervening procedure invocations) should be indistinguishable
   to the user.

   @param[in] service HSM service instance
   @param[in] input_key_index Index of input private key
   @param[in] output_key_index Index of output private key
   @param[in] key_addend Scalar to add
   @param[in] key_multiplier Scalar to multiply

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
hsm_ecc_private_key_multiply_add(hsm_service_t *service,
            hsm_cell_index_t input_key_index,
            hsm_cell_index_t output_key_index,
            const ecc_scalar_t *key_addend,
            const ecc_scalar_t *key_multiplier);

/** CSR random prefix size in bytes */
#define HSM_CSR_RANDOM_PREFIX_SIZE 16

/** CSR random prefix */
typedef struct {
  uint8_t value[HSM_CSR_RANDOM_PREFIX_SIZE];
} hsm_csr_random_prefix_t;

/** CSR random prefix default initializer */
#define HSM_CSR_RANDOM_PREFIX_INIT { .value = { 0 } }

/**
   ECDSA sign externally on CSR (Certificate Signing Request).

   The ::ecc_curve_t of the key at @p private_key_index must belong to
   at least a 256-bit elliptic curve (e.g. NIST P-256).

   The ::hsm_private_key_type_t of the key at @p private_key_index must be
   equal to ::HSM_PRIVATE_KEY_TYPE_CSR_SIGNER.

   The ::hsm_public_key_algorithm_t of the key at @p private_key_index must be
   equal to ::HSM_PUBLIC_KEY_ALGORITHM_ECDSA.

   Reference: "Pseudonym CSRs in ITS (Europe)", June 4, 2014.

   @param[in] service HSM service instance
   @param[in] private_key_index Index of private key that should be used
   @param[in] digest To-be-signed hash digest of CSR
   @param[out] prefix Random prefix
   @param[out] signature ECDSA fast verification signature

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
hsm_csr_ecdsa_external_sign(hsm_service_t *service,
            hsm_cell_index_t private_key_index,
            const sha_digest_t *digest,
            hsm_csr_random_prefix_t *prefix,
            ecc_fast_verification_signature_t *signature);

/**
   ECDSA sign internally on public keys.

   The ::ecc_curve_t of the key at @p private_key_index must belong to
   at least a 256-bit elliptic curve (e.g. NIST P-256).

   The ::hsm_private_key_type_t of the key at @p private_key_index must be
   equal to ::HSM_PRIVATE_KEY_TYPE_CSR_SIGNER.

   The ::hsm_public_key_algorithm_t of the key at @p private_key_index must be
   equal to ::HSM_PUBLIC_KEY_ALGORITHM_ECDSA.

   All the private keys in the specified cell ranges must have
   ::hsm_private_key_type_t equal to ::HSM_PRIVATE_KEY_TYPE_CSR_MEMBER.

   Reference: "Pseudonym CSRs in ITS (Europe)", June 4, 2014.

   @param[in] service HSM service instance
   @param[in] private_key_index Index of private key that should be used
   @param[in] range_array_ptr Array of cell ranges of private keys
   @param[in] range_array_size Size of cell ranges array
   @param[out] prefix Random prefix
   @param[out] signature ECDSA fast verification signature

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
hsm_csr_ecdsa_public_keys_sign(hsm_service_t *service,
            hsm_cell_index_t private_key_index,
            const hsm_cell_range_t *range_array_ptr,
            size_t range_array_size,
            hsm_csr_random_prefix_t *prefix,
            ecc_fast_verification_signature_t *signature);

/**
   Encrypt with AES-128-CBC data to be stored on a non-secure NVM.

   Intended use case is to provide confidentiality for data (e.g. pseudonym
   certificates) to be stored on a non-secure host NVM.

   The encryption key is internally generated with the following properties:
     - Non-volatile: stays the same across power-cycles.
     - Unique per unit: discovering one will not put other units at risk.
     - Not stored on a non-secure NVM.

   It is allowed that @p plaintext be equal to @p ciphertext in order to
   encrypt data in-place. Any other overlapping input and output buffers
   would result in undefined behavior.

   @p plaintext_size must be a multiple of ::AES_BLOCK_SIZE.

   @param[in] service HSM service instance
   @param[in] plaintext Plaintext to encrypt
   @param[in] plaintext_size Size of the plaintext in octets
   @param[out] iv Initialization vector
   @param[out] ciphertext Ciphertext
   @param[in,out] ciphertext_size
      The maximum size (in) and resulting size (out) of the
      ciphertext in octets

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
hsm_host_nvm_aes_cbc_encrypt(hsm_service_t *service,
            const void *plaintext,
            size_t plaintext_size,
            aes_cbc_iv_t *iv,
            void *ciphertext,
            size_t *ciphertext_size);

/**
   Decrypt with AES-128-CBC data that is stored on a non-secure NVM.

   Intended use case is to provide confidentiality for data (e.g. pseudonym
   certificates) that is stored on a non-secure host NVM.

   The decryption key is internally generated with the following properties:
     - Non-volatile: stays the same across power-cycles.
     - Unique per unit: discovering one will not put other units at risk.
     - Not stored on a non-secure NVM.

   It is allowed that @p plaintext be equal to @p ciphertext in order to
   decrypt data in-place. Any other overlapping input and output buffers
   would result in undefined behavior.

   @p ciphertext_size must be a multiple of ::AES_BLOCK_SIZE.

   @param[in] service HSM service instance
   @param[in] iv Initialization vector
   @param[in] ciphertext Ciphertext to decrypt
   @param[in] ciphertext_size Size of the ciphertext in octets
   @param[out] plaintext Plaintext
   @param[in,out] plaintext_size
      The maximum size (in) and resulting size (out) of the
      plaintext in octets

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
hsm_host_nvm_aes_cbc_decrypt(hsm_service_t *service,
            const aes_cbc_iv_t *iv,
            const void *ciphertext,
            size_t ciphertext_size,
            void *plaintext,
            size_t *plaintext_size);

/**
   Compute AES-128-CMAC authentication tag of data stored on a non-secure NVM.

   Intended use case is to provide integrity protection for data (e.g. root
   certificates) that is stored on a non-secure host NVM.

   The authentication key is internally generated with the following properties:
     - Non-volatile: stays the same across power-cycles.
     - Unique per unit: discovering one will not put other units at risk.
     - Not stored on a non-secure NVM.

   @param[in] service HSM service instance
   @param[in] message Message to compute CMAC
   @param[in] message_size Size of the message in octets
   @param[out] tag AES-CMAC authentication tag

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
hsm_host_nvm_aes_cmac_compute(hsm_service_t *service,
            const void *message,
            size_t message_size,
            aes_cmac_tag_t *tag);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_HSM_SERVICE_H */
