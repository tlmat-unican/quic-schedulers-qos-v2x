/* Copyright (C) 2015-2016 Autotalks Ltd. */
#ifndef _ATLK_GNSS_TESEO_H
#define _ATLK_GNSS_TESEO_H

#include <atlk/sdk.h>
#include <atlk/os.h>
#include <atlk/uart.h>
#include <atlk/sensor.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   GNSS Teseo API
*/

/** UART speed used to send FW Upgrade command is automatically chosen */
#define GNSS_TESEO_FW_UPDATE_NMEA_SPEED_BPS_AUTO 0

/** Teseo firmware update parameters */
typedef struct {
  /** Pointer to Teseo firmware image */
  const void *fw_image;

  /** Teseo firmware image size in bytes */
  size_t fw_image_size;

  /**
     UART speed used to send FW Upgrade command in bits/s.
     
     Speed should be set to the speed currently configured in Teseo firmware.
     At the end of the update, UART speed will be reconfigured to this speed.
     
     When speed is set to ::GNSS_TESEO_FW_UPDATE_NMEA_SPEED_BPS_AUTO, speed
     is automatically chosen (both during and after the update). Generally,
     automatically choosing speed is less reliable.
  */
  uart_speed_bps_t nmea_speed_bps;

  /** UART speed used to download firmware image in bits/s */
  uart_speed_bps_t download_speed_bps;

  /** Whether to erase NVM area */
  int erase_nvm_area;

  /**
     NVM area size in kilobytes.
     
     Default value is chosen when
     gnss_teseo_fw_update_params_t::nvm_area_size_kb equals zero.
  */
  size_t nvm_area_size_kb;

  /**
     Whether to run in recovery mode.
     
     @note Recovery mode will likely fail if ::gnss_config_t.hw_reset
     procedure is not registered.
  */
  int recovery_mode;

  /** Input thread scheduling parameters */
  atlk_thread_sched_t sched_params;

} gnss_teseo_fw_update_params_t;

/** Teseo firmware update parameters default initializer */
#define GNSS_TESEO_FW_UPDATE_PARAMS_INIT {      \
  .fw_image = NULL,                             \
  .fw_image_size = 0,                           \
  .nmea_speed_bps = 0,                          \
  .download_speed_bps = 0,                      \
  .erase_nvm_area = 1,                          \
  .nvm_area_size_kb = 0,                        \
  .recovery_mode = 0,                           \
  .sched_params = ATLK_THREAD_SCHED_INIT        \
}

/**
   Update Teseo firmware.

   Function call is blocking and may take several minutes to return. It is
   recommended to set ::gnss_teseo_fw_update_params_t.sched_params to the same
   scheduling parameters of the thread from which the function is called.

   @param[in] params Teseo firmware update parameters

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
gnss_teseo_fw_update(const gnss_teseo_fw_update_params_t *params);

/**
   Teseo SOU operating mode.

   Chosen operating mode must be aligned with the operating mode configured
   in DR firmware. Which sensors to feed depends on chosen mode.

   Please contact Autotalks support for further details.
*/
typedef enum {
  /**
     CAN gyro, DWP and reverse from CAN bus are selected as DR inputs.
     
     The following sensor feeders shall be used with this mode: reverse
     gear status, gyro_1axis and wheels speed.
  */
  GNSS_TESEO_SOU_OPERATING_MODE_20 = 0x14,

  /** GNSS operating mode is not available */
  GNSS_TESEO_SOU_OPERATING_MODE_NA = 0xff

} gnss_teseo_sou_operating_mode_t;

/** Teseo SOU configuration parameters */
typedef struct {
  /** SOU operating mode */
  gnss_teseo_sou_operating_mode_t operating_mode;

  /** Pointer to gyro 1-axis parameters */
  const sensor_value_params_t *gyro_1axis_params_ptr;

  /** Pointer to wheels speed parameters */
  const sensor_value_params_t *wheels_speed_params_ptr;

  /** SOU feeder thread scheduling parameters */
  atlk_thread_sched_t sched_params;
  
} gnss_teseo_sou_config_t;

/** Teseo SOU configuration parameters default initializer */
#define GNSS_TESEO_SOU_CONFIG_INIT {                    \
  .operating_mode = GNSS_TESEO_SOU_OPERATING_MODE_NA,   \
  .gyro_1axis_params_ptr = NULL,                        \
  .wheels_speed_params_ptr = NULL,                      \
  .sched_params = ATLK_THREAD_SCHED_INIT                \
}

/**
   Init Teseo SOU.

   @note Teseo SOU requires a Teseo device flashed with DR firmware (whether
   DR firmware is flashed or not is not checked in code).

   @param[in] config Teseo SOU configuration parameters

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
gnss_teseo_sou_init(const gnss_teseo_sou_config_t *config);

/**
   Feed reverse gear data via SOU.

   If @p value equals 0, reverse gear is not enabled.

   @param[in] value Reverse gear status value (boolean)

   @retval ::ATLK_OK if succeeded
   @retval ::ATLK_E_OUT_OF_DOMAIN if fed value is N/A
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
gnss_teseo_sou_reverse_gear_data_feed(int value);

/**
   Feed gyro 1-axis data via SOU.

   @param[in] value Gyro 1-axis rate value

   @retval ::ATLK_OK if succeeded
   @retval ::ATLK_E_OUT_OF_DOMAIN if fed value is N/A
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
gnss_teseo_sou_gyro_1axis_data_feed(sensor_value_t value);

/**
   Feed wheels speed data via SOU.

   @param[in] value Wheels speed value

   @retval ::ATLK_OK if succeeded
   @retval ::ATLK_E_OUT_OF_DOMAIN if fed value is N/A
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
gnss_teseo_sou_wheels_speed_data_feed(sensor_wheels_speed_t value);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_GNSS_TESEO_H */
