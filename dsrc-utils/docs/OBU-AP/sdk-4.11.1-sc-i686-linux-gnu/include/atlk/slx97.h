/* Copyright (C) 2016 Autotalks Ltd. */
#ifndef _ATLK_SLX97_H
#define _ATLK_SLX97_H

#include <atlk/sdk.h>
#include <atlk/hsm.h>
#include <atlk/aes.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   SLx97 host API
*/

/** SLx97 host communication security parameters */
typedef struct {
  /**
     SLx97 communication security minimum version support.
     Valid values are 1 or 2.
  */
  uint32_t sec_version_min;

  /**
     SLx97 communication security session key usage limit.
     Valid values are between 8 and 63 (both inclusive).
     Will only apply after ::hsm_nvm_init is called.
  */
  uint8_t sec_session_key_usage_limit;

  /**
     Whether SLx97 communication security master key is externally generated.
   */
  int sec_master_key_external;

} slx97_host_sec_config_t;

/** SLx97 host communication security default initializer */
#define SLX97_HOST_SEC_CONFIG_INIT {     \
  .sec_version_min = 1,                  \
  .sec_session_key_usage_limit = 32,     \
  .sec_master_key_external = 0           \
}

/** SLx97 host communication security key */
typedef struct {
  /* AES-128 authentication secret key */
  aes_key_t authentication_key;

  /* AES-128 encryption secret key */
  aes_key_t encryption_key;

} slx97_host_sec_key_t;

/** SLx97 host communication security key default initializer */
#define SLX97_HOST_SEC_KEY_INIT {     \
  .authentication_key = AES_KEY_INIT, \
  .encryption_key = AES_KEY_INIT      \
}

/**
   Get HSM SLx97 service.

   @param[out] service_ptr HSM SLx97 service

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
slx97_host_hsm_service_get(hsm_service_t **service_ptr);

/**
   SLx97 chip information.

   Reference: SLE97 Programmer Reference User Manual, 10.2.1 "IFX-Mailbox Area".
*/
typedef struct {
  /** Chip Ident Mode Identification Byte */
  uint8_t cimIdentifier;

  /** Platform Identifier */
  uint8_t platformIdentifier;

  /** Chip Mode Identifier */
  uint8_t modeIdentifier;

  /** Individual coding of each ROM mask */
  uint8_t romCode[2];

  /** Silicon Identification Number */
  uint8_t chipType_1[2];

  /** Blocked Variants */
  uint8_t chipType_2[4];

  /** Design Step Code */
  uint8_t designStep[2];

  /** Fab Number */
  uint8_t batchNumber_1;

  /** Production Year */
  uint8_t batchNumber_2;

  /** Business Week + Lot Number */
  uint8_t batchNumber_3[2];

  /** rfu (Extension of Lot Number) */
  uint8_t batchNumber_4;

  /** Wafer Number */
  uint8_t batchNumber_5;

  /** X-position of die on wafer */
  uint8_t chipPositionX[2];

  /** Y-position of die on wafer */
  uint8_t chipPositionY[2];

  /** Tracking information */
  uint8_t trackingInfo[8];

  /** Firmware Version Identifier */
  uint8_t firmwareIdentifier[4];

  /** Boot System ROM Version */
  uint8_t bosRomVersion[4];

  /** Boot System Patch Version */
  uint8_t bosPatchVersion[4];

  /** Amount of individual bytes */
  uint8_t individualLength;

  /** Lower limit of temperature range */
  uint8_t indLowerTempLimit;

  /** Upper limit of temperature range */
  uint8_t indUpperTempLimit;

  /** Oscillator frequency in MHz */
  uint8_t indOscFreq;

  /** Individuell Features in Generic CIM */
  uint8_t indFeatures;

  /** Individuell Features in Generic CIM */
  uint8_t indFeatures1;

  /** Reserved for future coding of individual CIM data */
  uint8_t rfu[27];

  /**
     Autotalks firmware version tuple (little-endian 32-bit unsigned integer).
     Encoded as: (major * 1e7) + (minor * 1e3) + patch.
  */
  uint8_t atlk_firmware_ver_tuple[4];

  /** Autotalks firmware build number (little-endian 16-bit unsigned integer) */
  uint8_t atlk_firmware_build_num[2];

  /** Autotalks firmware build information */
  uint8_t atlk_firmware_build_info[4];

} slx97_chip_info_t;

/**
   Get HSM SLx97 chip information.

   @param[out] info SLx97 chip information

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
slx97_chip_info_get(slx97_chip_info_t *info);

/**
   Get Autotalks firmware information string.

   @param[in] info SLx97 chip information
   @param[out] firmware_info Firmware information string
   @param[in,out] firmware_info_size Maximum (in) and actual (out) firmware
                  information string in bytes

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
slx97_firmware_info_str_get(const slx97_chip_info_t *info,
            char *firmware_info,
            size_t firmware_info_size);

/** Device specific key size in bytes used for SLx97 communication security */
#define SLX97_DSK_SIZE 32

/** Device specific key used for SLx97 communication security */
typedef struct {
  uint8_t value[SLX97_DSK_SIZE];
} slx97_dsk_t;

/**
   Callback function for retrieving the value of a device specific key.

   @param[out] dsk Device specific key

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
typedef atlk_rc_t (*slx97_dsk_callback_t)(slx97_dsk_t *dsk);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_SLX97_H */
