/* Copyright (C) 2014-2016 Autotalks Ltd. */
#ifndef _ATLK_HSM_EMULATOR_H
#define _ATLK_HSM_EMULATOR_H

#include <atlk/sdk.h>
#include <atlk/aes.h>
#include <atlk/hsm.h>
#include <atlk/ecc_service.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   HSM emulator API
*/

/** HSM emulator configuration parameters */
typedef struct {
  /** ECC service pointer to be used by the HSM emulator */
  ecc_service_t *ecc_service_ptr;

  /** 
     NVM filename.

     Using NULL means RAM is used for storage of private keys.   
  */
  char *nvm_file_path;

  /** AES-128 key to provide host NVM storage integrity */
  aes_key_t host_nvm_authentication_key;

  /** AES-128 key to provide host NVM storage confidentiality */
  aes_key_t host_nvm_encryption_key;

} hsm_emulator_config_t;

/** HSM emulator configuration default initializer */
#define HSM_EMULATOR_CONFIG_INIT {              \
  .ecc_service_ptr = NULL,                      \
  .nvm_file_path = NULL,                        \
  .host_nvm_authentication_key = AES_KEY_INIT,  \
  .host_nvm_encryption_key = AES_KEY_INIT       \
}

/**
   Create HSM emulator service.

   @param[in] config HSM emulator configuration parameters
   @param[out] service_ptr HSM emulator service

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
hsm_emulator_create(const hsm_emulator_config_t *config,
            hsm_service_t **service_ptr);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_HSM_EMULATOR_H */
