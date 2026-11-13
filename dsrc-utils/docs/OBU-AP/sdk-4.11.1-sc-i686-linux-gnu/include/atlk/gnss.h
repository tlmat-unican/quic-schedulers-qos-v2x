/* Copyright (C) 2015-2016 Autotalks Ltd. */
#ifndef _ATLK_GNSS_H
#define _ATLK_GNSS_H

#include <atlk/sdk.h>
#include <atlk/os.h>
#include <atlk/uart.h>
#include <atlk/nav.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   GNSS API
*/

/** GNSS model */
typedef enum {
  /** STMicroelectronics Teseo-II */
  GNSS_MODEL_STMICRO_TESEO_II = 0,

  /** STMicroelectronics Teseo-III */
  GNSS_MODEL_STMICRO_TESEO_III = 1,

  /** u-blox MAX-7 series */
  GNSS_MODEL_UBLOX_MAX_7 = 2,

  /** u-blox MAX-M8 series */
  GNSS_MODEL_UBLOX_MAX_M8 = 3,

  /** GNSS is not available */
  GNSS_MODEL_NA = 255

} gnss_model_t;

/**
   GNSS HW reset procedure.

   @note When using a Teseo device, this procedure should either toggle the
   reset pin or toggle the device's power supply. Performing Teseo firmware
   update procedure in recovery mode will likely fail without HW reset. 

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
typedef atlk_rc_t atlk_must_check
(*gnss_hw_reset_t)(void);

/** GNSS configuration parameters */
typedef struct {
  /** GNSS model */
  gnss_model_t model;

  /** GNSS HW reset (optional) */
  gnss_hw_reset_t hw_reset;

  /**
     Whether GNSS watchdog is enabled.
     
     When enabled, specifying ::gnss_config_t.hw_reset is mandatory.
  */
  int wd_enabled;

  /** GNSS watchdog thread scheduling parameters */
  atlk_thread_sched_t wd_sched_params;

  /** I/O device name */
  const char *dev_name;

  /** Expected NMEA speed in bps during init */
  uart_speed_bps_t nmea_speed_bps;
  
  /**
     NMEA sentence address for 10 Hz cycles.
     
     Expected NMEA cycle ender of all cycles except for cycles in which NMEA
     time-stamp is round. String must include '$' sign at its start.
     
     For example:
     @code{.c}
       gnss_config_t config = GNSS_CONFIG_INIT;
       config.nmea_cycle_ender_1hz = "$PSTMCPU";
       config.nmea_cycle_ender_10hz = "$XXGLL";
     @endcode

     @note Setting an invalid talker ID means "don't care" (for example, 
     setting to "$XXGLL", means GLL with any talker ID is cycle ender).
  */
  const char *nmea_cycle_ender_10hz;

  /** NMEA sentence address for 1 Hz cycles */
  const char *nmea_cycle_ender_1hz;

  /** Input thread scheduling parameters */
  atlk_thread_sched_t sched_params;

  /** Navigation data frame handler */
  nav_data_handler_t handler;

  /** Navigation service instance */
  nav_service_t *service;

} gnss_config_t;

/** GNSS configuration parameters default initializer */
#define GNSS_CONFIG_INIT {                      \
  .model = GNSS_MODEL_NA,                       \
  .hw_reset = NULL,                             \
  .wd_enabled = 0,                              \
  .wd_sched_params = ATLK_THREAD_SCHED_INIT,    \
  .dev_name = NULL,                             \
  .nmea_speed_bps = UART_SPEED_230400_BPS,      \
  .nmea_cycle_ender_10hz = NULL,                \
  .nmea_cycle_ender_1hz = NULL,                 \
  .sched_params = ATLK_THREAD_SCHED_INIT,       \
  .handler = NULL,                              \
  .service = NULL                               \
}

/**
   Initialize GNSS.

   @param[in] config GNSS configuration parameters

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
gnss_init(const gnss_config_t *config);

/**
   Predefined GNSS default wait option.
   
   This is the default recommended amount of time to wait in all GNSS API
   functions which receive a wait option.
   
   Waiting less than this amount of time might result in subsequent function
   call receiving a reply from GNSS which was not meant for it.
   
   Waiting more than this amount of time is pointless.
*/
extern const atlk_wait_t gnss_default_wait;

/**
   Get GNSS firmware version.

   @param[out] fw_version GNSS firmware version buffer
   @param[in,out] fw_version_size Maximum size (in) and actual (out) in chars
   @param[in] wait Wait specification (optional)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
gnss_fw_version_get(char *fw_version,
            size_t *fw_version_size,
            const atlk_wait_t *wait);

/** GNSS reset type */
typedef enum {
  /** Reset hardware */
  GNSS_RESET_TYPE_HW = 0,

  /** Reset software */
  GNSS_RESET_TYPE_SW = 1,

  /** Reset GNSS engine only */
  GNSS_RESET_TYPE_GNSS_ONLY = 2,

  /** Reset type is N/A */
  GNSS_RESET_TYPE_NA = 3

} gnss_reset_type_t;

/** GNSS start type */
typedef enum {
  /** Hot start */
  GNSS_START_TYPE_HOT = 0,

  /** Warm start */
  GNSS_START_TYPE_WARM = 1,

  /** Cold start */
  GNSS_START_TYPE_COLD = 2,

  /** Start type is N/A */
  GNSS_START_TYPE_NA = 3

} gnss_start_type_t;

/** Clear almanac during cold GNSS start */
#define GNSS_COLD_START_F_CLEAR_ALMANAC (1U << 0)

/** Clear ephemeris during cold GNSS start */
#define GNSS_COLD_START_F_CLEAR_EPHEMERIS (1U << 1)

/** Clear position during cold GNSS start */
#define GNSS_COLD_START_F_CLEAR_POSITION (1U << 2)

/** Clear time during cold GNSS start */
#define GNSS_COLD_START_F_CLEAR_TIME (1U << 3)

/** Clear all during cold reset */
#define GNSS_COLD_START_F_CLEAR_ALL UINT32_MAX

/** GNSS reset parameters */
typedef struct {
  /** GNSS reset type */
  gnss_reset_type_t reset_type;

  /** GNSS start type */
  gnss_start_type_t start_type;

  /** GNSS cold start flags bitmask */
  uint32_t cold_start_flags;

} gnss_reset_params_t;

/** GNSS reset parameters default initializer */
#define GNSS_RESET_PARAMS_INIT {        \
  .reset_type = GNSS_RESET_TYPE_NA,     \
  .start_type = GNSS_START_TYPE_NA,     \
  .cold_start_flags = 0                 \
}

/**
   GNSS reset.

   @param[in] params GNSS reset parameters
   @param[in] wait Wait specification (optional)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
gnss_reset(const gnss_reset_params_t *params,
            const atlk_wait_t *wait);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_GNSS_H */
