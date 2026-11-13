/* Copyright (C) 2016 Autotalks Ltd. */
#ifndef _CRATON_V2X_CONFIG_H
#define _CRATON_V2X_CONFIG_H

#include <atlk/sdk.h>

#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/** V2X configuration */
typedef struct {
  /** Number of sockets in V2X socket pool */
  size_t socket_pool_size;

  /** Number of packets in V2X packet pool */
  size_t packet_pool_size;

} v2x_config_t;

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_V2X_CONFIG_H */
