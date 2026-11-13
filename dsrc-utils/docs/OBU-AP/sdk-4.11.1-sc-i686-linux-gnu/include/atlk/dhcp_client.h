/* Copyright (C) 2016 Autotalks Ltd. */
#ifndef _ATLK_DHCP_CLIENT_H
#define _ATLK_DHCP_CLIENT_H

#include <atlk/sdk.h>
#include <atlk/os.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   DHCP client API
*/


/**
   DHCP client bound handler.
   
   Handler is called when leasing address was established.
*/
typedef void 
(*dhcp_client_bound_handler_t)(void);

/** DHCP client configuration parameters */
typedef struct {
  /** DHCP client bound handler */
  dhcp_client_bound_handler_t bound_handler;

  /** DHCP client thread scheduling parameters */
  atlk_thread_sched_t sched_params;

} dhcp_client_config_t;

/** DHCP client configuration parameters default initializer */
#define DHCP_CLIENT_CONFIG_INIT {           \
  .bound_handler = NULL,                    \
  .sched_params = ATLK_THREAD_SCHED_INIT    \
}

/**
   Initialize DHCP client.

   @param[in] config DHCP client configuration parameters

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
dhcp_client_init(const dhcp_client_config_t *config);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_DHCP_CLIENT_H */
