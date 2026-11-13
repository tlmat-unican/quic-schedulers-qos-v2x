/* Copyright (C) 2015 Autotalks Ltd. */
#ifndef _ATLK_VCA_H
#define _ATLK_VCA_H

/**
   Note: This header is not final and will change in future SDK versions.
*/

#include <atlk/sdk.h>
#include <nxd_bsd.h>

#ifdef __cplusplus
extern "C" {
#endif

#define VCA_MAX_NUM_WLAN_IF 2

struct vca_connection;

typedef struct {
  int (*read)(struct vca_connection *,
              void *buf,
              size_t size);

  int (*write)(struct vca_connection *,
               const void *buf,
               size_t size);

} vca_connection_ops_t;

#define VCA_CONNECTION_OPS_INIT { \
  .read = NULL,                   \
  .write = NULL,                  \
}

typedef struct vca_connection {
  vca_connection_ops_t *connection_ops;

} vca_connection_t;

#define VCA_CONNECTION_INIT {     \
  .connection_ops = NULL,         \
}
/* Get only instance of default TCP connection object.
 * Initialize object on first call */
vca_connection_t *vca_get_default_tcp_connection(void);

/* Create and return default UDP connection object */
vca_connection_t *
vca_create_default_udp_connection_object(uint32_t ip, uint16_t port);

/** VCA service configuration descriptor */
typedef struct {
  /* log to host ip and port are only relvant when using
   * default data connection object */
  uint32_t log_to_host_ip;
  uint16_t log_to_host_port;
  int tcp_server_enabled;
  int disable_sd_card;
  int socket_enable[VCA_MAX_NUM_WLAN_IF];
  /* Null Connection object means use default */
  vca_connection_t *control_connection_object;
  vca_connection_t *data_connection_object;
} vca_srv_config_t;

#define VCA_SRV_CONFIG_INIT { \
  .log_to_host_ip = NX_BSD_LOCAL_IF_INADDR_ANY, \
  .log_to_host_port = 0,                        \
  .tcp_server_enabled = 0,                      \
  .disable_sd_card = 0,                         \
  .socket_enable = {0},                         \
  .control_connection_object = NULL,            \
  .data_connection_object = NULL                \
}

/**
  Reads from boot parameters and fill configuration values

  @param[out] config Configuration values set from boot parameters

  @retval ATLK_OK for success
  @return ATLK failure value
*/
atlk_rc_t atlk_must_check
vca_srv_bootparam_read(vca_srv_config_t *config);

/**
  Initialize VCA service

  @param[in] config VCA service configuration parameters

  @retval ATLK_OK for success
  @return ATLK failure value
*/
atlk_rc_t atlk_must_check
vca_srv_init(vca_srv_config_t *config);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_VCA_H */
