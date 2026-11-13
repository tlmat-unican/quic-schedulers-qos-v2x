/* Copyright (C) 2015 Autotalks Ltd. */
#ifndef _ATLK_SNTP_CLIENT_H
#define _ATLK_SNTP_CLIENT_H

#include <atlk/sdk.h>
#include <atlk/os.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   SNTP client API
*/

/** NTP update info */
typedef struct {
  /** NTP time seconds */
  uint32_t seconds;
  
  /** NTP time fractions of a second */
  uint32_t fraction;

} sntp_info_t;

/** SNTP client update callback function */
typedef void (*sntp_client_update_handler_t)(const sntp_info_t *info);

/**
   Convert NTP timestamp to POSIX time.
   
   POSIX time is defined as the number of seconds that have elapsed
   since 1970-01-01T00:00:00Z, not counting leap seconds.
*/
atlk_inline double
sntp_time_to_posix_time(const sntp_info_t *info)
{
  return (double)info->seconds - 2208988800.0 + 
    (double)info->fraction / UINT32_MAX;
}

/** SNTP connection type */
typedef enum {
  /** Value indicating that SNTP connection type is N/A */
  SNTP_CONNECTION_TYPE_NA = 0,

  /** Listen to broadcast messages */
  SNTP_CONNECTION_TYPE_BROADCAST = 1,

  /** Listen to unicast messages */
  SNTP_CONNECTION_TYPE_UNICAST = 2

} sntp_connection_type_t;

/** Server stratum minimum value */
#define SNTP_SERVER_STRATUM_MIN 1

/** Server stratum maximum value */
#define SNTP_SERVER_STRATUM_MAX 15

/** SNTP client configuration parameters */
typedef struct {
  /** SNTP thread scheduling parameters */
  atlk_thread_sched_t sched_params;

  /** SNTP client update callback */
  sntp_client_update_handler_t update_handler;

  /** NTP IPv4 server address in network byte order */
  uint32_t ntp_server_address;

  /** SNTP connection type */
  sntp_connection_type_t type;

  /** 
     Upper limit of server clock dispersion in microseconds the client will
     accept. To disable this check, set this parameter to 0.
  */
  uint32_t max_root_dispersion_us;

  /**
     Minimum (numerically highest) stratum the client will accept.
     
     Valid range defined by ::SNTP_SERVER_STRATUM_MIN and
     ::SNTP_SERVER_STRATUM_MAX.
  */
  uint8_t min_server_stratum;

  /**
     SNTP client unicast poll interval in seconds.
     
     RFC-4330 section 10.1: "A client MUST NOT under any conditions use a
     poll interval less than 15 seconds."

     For testing purposes it might be desirable to set polling interval to
     less than 15 seconds. Please do so only when using a local server (i.e. 
     a server in your own Network).
  */
  uint16_t unicast_poll_interval_s;

} sntp_client_config_t;

/** SNTP client configuration parameters default initializer */
#define SNTP_CLIENT_CONFIG_INIT {               \
  .sched_params = ATLK_THREAD_SCHED_INIT,       \
  .update_handler = NULL,                       \
  .ntp_server_address = 0,                      \
  .type = SNTP_CONNECTION_TYPE_NA,              \
  .max_root_dispersion_us = 0,                  \
  .min_server_stratum = 2,                      \
  .unicast_poll_interval_s = 3600               \
}

/**
   Initialize SNTP client.

   @param[in] config SNTP client configuration parameters

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
sntp_client_init(const sntp_client_config_t *config);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_SNTP_CLIENT_H */
