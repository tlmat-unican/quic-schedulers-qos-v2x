/* Copyright (C) 2014-2016 Autotalks Ltd. */
#ifndef _ATLK_ECIES_H
#define _ATLK_ECIES_H

#include <atlk/sdk.h>
#include <atlk/ecc.h>
#include <atlk/sha.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   ECIES API

   Provides ECIES encryption/decryption API reflected from
   IEEE 1609.2-2016 (clause 5.3.5) and IEEE 1363a (clause 11.3).

   References:

     - IEEE 1609.2-2016:
       IEEE Standard for Wireless Access in Vehicular Environments -
       Security Services for Applications and Management Messages.

     - IEEE Std 1363a:
       IEEE Standard Specifications for Public-Key Cryptography -
       Amendment 1: Additional Techniques.
*/

/**
   Generate ECIES shared secret key from a public key.

   @p curve must belong to an elliptic curve with
   more than 224 bits (e.g. NIST P-256).

   @p kdf_param is an optional octet string used as a key derivation parameter.
   In order for the key derivation parameter to be the empty string,
   @p kdf_param_size should be 0.
   The key derivation parameter can be used to prevent misbinding attacks.
   Please refer to IEEE Std 1363a-2004 clause 11.3.2 where the key derivation
   parameter is denoted by P1.

   @param[in] curve Elliptic curve
   @param[in] peer_public_key Public key of ECIES peer
   @param[out] public_key Ephemeral public key to be sent to ECIES peer
   @param[out] key Derived ECIES key
   @param[in] key_size ECIES key size in octets
   @param[in] kdf_param Key derivation parameter (optional)
   @param[in] kdf_param_size Key derivation parameter size in octets
 
   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
ecies_key_create(ecc_curve_t curve,
              const ecc_point_t *peer_public_key,
              ecc_point_t *public_key,
              void *key,
              size_t key_size,
              const void *kdf_param,
              size_t kdf_param_size);

/**
   Maximum size of plaintext/ciphertext (in octets)
   for ECIES encryption/decryption
*/
#define ECIES_MAX_TEXT_SIZE 16

/**
   ECIES authentication tag size in octets as specified in
   IEEE 1609.2-2016 clause 5.3.5
*/
#define ECIES_AUTHENTICATION_TAG_SIZE 16

/** ECIES authentication tag */
typedef struct {
  uint8_t value[ECIES_AUTHENTICATION_TAG_SIZE];
} ecies_authentication_tag_t;

/** ECIES authentication tag default initializer */
#define ECIES_AUTHENTICATION_TAG_INIT { .value = { 0 } }

/**
   Encrypt with ECIES.

   Overlapping input and output buffers would result in undefined behavior.

   @plaintext_size must not be greater than ::ECIES_MAX_TEXT_SIZE.

   @param[in] sha_algorithm SHA algorithm to be used in MAC calculation
   @param[in] key ECIES key
   @param[in] key_size ECIES key size in octets
   @param[in] plaintext Plaintext to encrypt
   @param[in] plaintext_size Size of the plaintext in octets
   @param[out] ciphertext Ciphertext
   @param[in,out] ciphertext_size
      The maximum size (in) and resulting size (out) of the
      ciphertext in octets
   @param[out] tag Authentication tag

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
ecies_encrypt(sha_algorithm_t sha_algorithm,
            const void *key,
            size_t key_size,
            const void *plaintext,
            size_t plaintext_size,
            void *ciphertext,
            size_t *ciphertext_size,
            ecies_authentication_tag_t *tag);

/**
   Decrypt with ECIES.

   Overlapping input and output buffers would result in undefined behavior.

   User should compare the resulting authentication tag to the expected
   authentication tag in order to verify message integrity. In the case that
   tags don't match this function would still return ::ATLK_OK.

   @ciphertext_size must not be greater than ::ECIES_MAX_TEXT_SIZE.

   @param[in] sha_algorithm SHA algorithm to be used in MAC calculation
   @param[in] key ECIES key
   @param[in] key_size ECIES key size in octets
   @param[in] ciphertext Ciphertext to decrypt
   @param[in] ciphertext_size Size of the ciphertext in octets
   @param[out] plaintext Plaintext
   @param[in,out] plaintext_size
      The maximum size (in) and resulting size (out) of the
      plaintext in octets
   @param[out] tag Authentication tag

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
ecies_decrypt(sha_algorithm_t sha_algorithm,
            const void *key,
            size_t key_size,
            const void *ciphertext,
            size_t ciphertext_size,
            void *plaintext,
            size_t *plaintext_size,
            ecies_authentication_tag_t *tag);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_ECIES_H */
