/* Copyright (C) 2014-2016 Autotalks Ltd. */
#ifndef _ATLK_AES_H
#define _ATLK_AES_H

#include <atlk/sdk.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   AES API

   Provides AES-CCM, AES-CBC, AES-ECB and AES-CMAC APIs.

   References:

     - IEEE 1609.2-2016:
       IEEE Standard for Wireless Access in Vehicular Environments -
       Security Services for Applications and Management Messages

     - NIST Special Publication 800-38A:
       Recommendation for Block Cipher Modes of Operation:
       Methods and Techniques

     - NIST Special Publication 800-38B:
       Recommendation for Block Cipher Modes of Operation:
       The CMAC Mode for Authentication

     - NIST Special Publication 800-38C:
       Recommendation for Block Cipher Modes of Operation:
       The CCM Mode for Authentication and Confidentiality
*/

/** AES secret key size in octets */
#define AES_KEY_SIZE 16

/** AES secret key */
typedef struct {
  uint8_t value[AES_KEY_SIZE];
} aes_key_t;

/** AES secret key default initializer */
#define AES_KEY_INIT { .value = { 0 } }

/**
   AES-CCM nonce size in octets as specified in
   IEEE 1609.2-2016 clause 5.3.8
*/
#define AES_CCM_NONCE_SIZE 12

/** AES-CCM nonce */
typedef struct {
  uint8_t value[AES_CCM_NONCE_SIZE];
} aes_ccm_nonce_t;

/** AES-CCM nonce default initializer */
#define AES_CCM_NONCE_INIT { .value = { 0 } }

/**
   AES-CCM authentication tag size in octets as specified in
   IEEE 1609.2-2016 clause 5.3.8
*/
#define AES_CCM_AUTHENTICATION_TAG_SIZE 16

/** AES-CCM authentication tag */
typedef struct {
  uint8_t value[AES_CCM_AUTHENTICATION_TAG_SIZE];
} aes_ccm_authentication_tag_t;

/** AES-CCM authentication tag default initializer */
#define AES_CCM_AUTHENTICATION_TAG_INIT { .value = { 0 } }

/**
   Encrypt with AES-CCM.

   Parameters were chosen as specified in IEEE 1609.2-2016 clause 5.3.8:
     - No session header (associated data).
     - The message authentication tag length is 16 octets.
     - The nonce length is 12 octets.

   It is allowed that @p plaintext be equal to @p ciphertext in order to
   encrypt data in-place. Any other overlapping input and output buffers
   would result in undefined behavior.

   @param[in] key Encryption key
   @param[in] nonce Session nonce (must be used once!)
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
aes_ccm_encrypt(const aes_key_t *key,
            const aes_ccm_nonce_t *nonce,
            const void *plaintext,
            size_t plaintext_size,
            void *ciphertext,
            size_t *ciphertext_size,
            aes_ccm_authentication_tag_t *tag);

/**
   Decrypt with AES-CCM.

   Parameters were chosen as specified in IEEE 1609.2-2016 clause 5.3.8:
     - No session header (associated data).
     - The message authentication tag length is 16 octets.
     - The nonce length is 12 octets.

   It is allowed that @p plaintext be equal to @p ciphertext in order to
   decrypt data in-place. Any other overlapping input and output buffers
   would result in undefined behavior.

   User should compare the resulting authentication tag to the expected
   authentication tag in order to verify message integrity. In the case that
   tags don't match this function would still return ::ATLK_OK.

   @param[in] key Decryption key
   @param[in] nonce Session nonce
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
aes_ccm_decrypt(const aes_key_t *key,
            const aes_ccm_nonce_t *nonce,
            const void *ciphertext,
            size_t ciphertext_size,
            void *plaintext,
            size_t *plaintext_size,
            aes_ccm_authentication_tag_t *tag);

/**
   Encrypt with AES-CCM.

   It is allowed that @p plaintext be equal to @p ciphertext in order to
   encrypt data in-place. Any other overlapping input and output buffers
   would result in undefined behavior.

   @param[in] key Encryption key
   @param[in] nonce Session nonce (must be used once!)
   @param[in] nonce_size Session nonce size in octets
   @param[in] header Session header (optional)
   @param[in] header_size Size of the session header in octets
   @param[in] plaintext Plaintext to encrypt
   @param[in] plaintext_size Size of the plaintext in octets
   @param[out] ciphertext Ciphertext
   @param[in,out] ciphertext_size
      The maximum size (in) and resulting size (out) of the
      ciphertext in octets
   @param[out] tag Authentication tag
   @param[in,out] tag_size
      The maximum size (in) and resulting size (out) of the
      authentication tag in octets

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
aes_ccm_encrypt_2(const aes_key_t *key,
            const void *nonce,
            size_t nonce_size,
            const void *header,
            size_t header_size,
            const void *plaintext,
            size_t plaintext_size,
            void *ciphertext,
            size_t *ciphertext_size,
            void *tag,
            size_t *tag_size);

/**
   Decrypt with AES-CCM.

   It is allowed that @p plaintext be equal to @p ciphertext in order to
   decrypt data in-place. Any other overlapping input and output buffers
   would result in undefined behavior.

   User should compare the resulting authentication tag to the expected
   authentication tag in order to verify message integrity. In the case that
   tags don't match this function would still return ::ATLK_OK.

   @param[in] key Decryption key
   @param[in] nonce Session nonce (must be used once!)
   @param[in] nonce_size Session nonce size in octets
   @param[in] header Session header (optional)
   @param[in] header_size Size of the session header in octets
   @param[in] ciphertext Ciphertext to decrypt
   @param[in] ciphertext_size Size of the ciphertext in octets
   @param[out] plaintext Plaintext
   @param[in,out] plaintext_size
      The maximum size (in) and resulting size (out) of the
      plaintext in octets
   @param[out] tag Authentication tag
   @param[in,out] tag_size
      The maximum size (in) and resulting size (out) of the
      authentication tag in octets

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
aes_ccm_decrypt_2(const aes_key_t *key,
            const void *nonce,
            size_t nonce_size,
            const void *header,
            size_t header_size,
            const void *ciphertext,
            size_t ciphertext_size,
            void *plaintext,
            size_t *plaintext_size,
            void *tag,
            size_t *tag_size);

/** AES block size in octets */
#define AES_BLOCK_SIZE 16

/**
   Encrypt with AES-ECB.

   It is allowed that @p plaintext be equal to @p ciphertext in order to
   encrypt data in-place. Any other overlapping input and output buffers
   would result in undefined behavior.

   @p plaintext_size must be a multiple of ::AES_BLOCK_SIZE.

   @param[in] key Encryption key
   @param[in] plaintext Plaintext to encrypt
   @param[in] plaintext_size Size of the plaintext in octets
   @param[out] ciphertext Ciphertext
   @param[in,out] ciphertext_size
      The maximum size (in) and resulting size (out) of the
      ciphertext in octets

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
aes_ecb_encrypt(const aes_key_t *key,
            const void *plaintext,
            size_t plaintext_size,
            void *ciphertext,
            size_t *ciphertext_size);

/**
   Decrypt with AES-ECB.

   It is allowed that @p plaintext be equal to @p ciphertext in order to
   decrypt data in-place. Any other overlapping input and output buffers
   would result in undefined behavior.

   @p ciphertext_size must be a multiple of ::AES_BLOCK_SIZE.

   @param[in] key Decryption key
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
aes_ecb_decrypt(const aes_key_t *key,
            const void *ciphertext,
            size_t ciphertext_size,
            void *plaintext,
            size_t *plaintext_size);

/** AES-CBC initialization vector */
typedef struct {
  uint8_t value[AES_BLOCK_SIZE];
} aes_cbc_iv_t;

/**
   Encrypt with AES-CBC.

   It is allowed that @p plaintext be equal to @p ciphertext in order to
   encrypt data in-place. Any other overlapping input and output buffers
   would result in undefined behavior.

   @p plaintext_size must be a multiple of ::AES_BLOCK_SIZE.

   @remark According to NIST SP 800-38A clause 5.3:
   "The IV need not be secret; however, for the CBC and CFB modes, the IV for
   any particular execution of the encryption process must be unpredictable."

   @remark According to NIST SP 800-38A Appendix C:
   "There are two recommended methods for generating unpredictable IVs. The first
   method is to apply the forward cipher function, under the same key that is
   used for the encryption of the plaintext, to a nonce. The nonce must be a
   data block that is unique to each execution of the encryption operation. For
   example, the nonce may be a counter, as described in Appendix B, or a message
   number. The second method is to generate a random data block using a FIPS
   approved random number generator."

   @param[in] key Encryption key
   @param[in] iv Initialization vector
   @param[in] plaintext Plaintext to encrypt
   @param[in] plaintext_size Size of the plaintext in octets
   @param[out] ciphertext Ciphertext
   @param[in,out] ciphertext_size
      The maximum size (in) and resulting size (out) of the
      ciphertext in octets

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
aes_cbc_encrypt(const aes_key_t *key,
            const aes_cbc_iv_t *iv,
            const void *plaintext,
            size_t plaintext_size,
            void *ciphertext,
            size_t *ciphertext_size);

/**
   Decrypt with AES-CBC.

   It is allowed that @p plaintext be equal to @p ciphertext in order to
   decrypt data in-place. Any other overlapping input and output buffers
   would result in undefined behavior.

   @p ciphertext_size must be a multiple of ::AES_BLOCK_SIZE.

   @param[in] key Decryption key
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
aes_cbc_decrypt(const aes_key_t *key,
            const aes_cbc_iv_t *iv,
            const void *ciphertext,
            size_t ciphertext_size,
            void *plaintext,
            size_t *plaintext_size);

/** AES-CMAC authentication tag size in octets */
#define AES_CMAC_TAG_SIZE 16

/** AES-CMAC authentication tag */
typedef struct {
  uint8_t value[AES_CMAC_TAG_SIZE];
} aes_cmac_tag_t;

/** AES-CMAC authentication tag default initializer */
#define AES_CMAC_TAG_INIT { .value = { 0 } }

/**
   Compute AES-CMAC authentication tag.

   @param[in] key Secret key
   @param[in] message Message to compute CMAC
   @param[in] message_size Size of the message in octets
   @param[out] tag AES-CMAC authentication tag

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
aes_cmac_compute(const aes_key_t *key,
            const void *message,
            size_t message_size,
            aes_cmac_tag_t *tag);

/**
   Compute AES CCM Plain-Text MAC.

   Calculate an authentication tag which is the value T described in
   step 4 of section 6.1 in NIST SP 800-38C.

   @param[in] key Secret key
   @param[in] nonce Session nonce (must be used once!)
   @param[in] nonce_size Session nonce size in octets
   @param[in] header Session header (optional)
   @param[in] header_size Size of the session header in octets
   @param[in] message Message to compute authentication tag
   @param[in] message_size Size of the message in octets
   @param[out] tag Authentication tag
   @param[in,out] tag_size
      The maximum size (in) and resulting size (out) of the
      authentication tag in octets

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
aes_ccmptmac_compute(const aes_key_t *key,
            const void *nonce,
            size_t nonce_size,
            const void *header,
            size_t header_size,
            const void *message,
            size_t message_size,
            void *tag,
            size_t *tag_size);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_AES_H */
