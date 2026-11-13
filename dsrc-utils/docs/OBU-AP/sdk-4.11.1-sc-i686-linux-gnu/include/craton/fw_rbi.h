/* Copyright (C) 2016 Autotalks Ltd. */
#ifndef _CRATON_FW_RBI_H
#define _CRATON_FW_RBI_H

#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   Firmware verification API
*/

/** Firmware verification result */
typedef enum {
  /** Firmware verified */
  FW_RBI_VERIFY_OK = 0,

  /** Invalid OEM public key signature */
  FW_RBI_VERIFY_E_PUBLIC_KEY_SIGNATURE = 1,

  /** Invalid image signature */
  FW_RBI_VERIFY_E_IMAGE_SIGNATURE = 2,

  /** Invalid argument */
  FW_RBI_VERIFY_E_INVALID_ARG = 3

} fw_rbi_verify_result_t;

/**
   Verify signed image.

   @param[in] image Signed image to be verified
   @param[in] image_size Image size in bytes

   @return ::FW_RBI_VERIFY_OK if image verification succeeded,
   otherwise return ::FW_RBI_VERIFY_E_*
*/
fw_rbi_verify_result_t
fw_rbis_verify(const void *image, size_t image_size);

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_FW_RBI_H */
