/* Copyright (C) 2014-2015 Autotalks Ltd. */
#ifndef _ATLK_V2X_SERVICE_H
#define _ATLK_V2X_SERVICE_H

#include <atlk/sdk.h>
#include <atlk/eui48.h>
#include <atlk/v2x.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   V2X service API
*/

/**
   Get pointer to default V2X service.

   @param[out] service_ptr Pointer to V2X service

   @note New implementation of this getter will override default
   getter (declared as a weak symbol).

   @note Not supported by remote service library.

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
v2x_default_service_get(v2x_service_t **service_ptr);

/**
   Delete V2X service instance.

   @param[in] service V2X service instance

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t
v2x_service_delete(v2x_service_t *service);

/**
   Send IEEE Std 1609.4-2016 service primitive MLMEX-CHSTART.request and
   receive MLMEX-CHSTART.confirm.
*/
atlk_rc_t atlk_must_check
v2x_dot4_channel_start(v2x_service_t *service,
                       const v2x_dot4_channel_start_request_t *request,
                       const atlk_wait_t *wait);

/**
   Send IEEE Std 1609.4-2016 service primitive MLMEX-CHEND.request and receive
   MLMEX-CHEND.confirm.
*/
atlk_rc_t atlk_must_check
v2x_dot4_channel_end(v2x_service_t *service,
                     const v2x_dot4_channel_end_request_t *request,
                     const atlk_wait_t *wait);

/**
   Receive IEEE Std 1609.4-2016 service primitive MLMEX-CHEND.indication.
*/
atlk_rc_t atlk_must_check
v2x_dot4_channel_end_receive(v2x_service_t *service,
                             v2x_dot4_channel_end_indication_t *indication,
                             const atlk_wait_t *wait);

/**
   Set network interface's V2X access profile.

   @param[in] service V2X service instance
   @param[in] netif_index Network interface index
   @param[in] profile Network interface V2X access profile

   @remark The method to obtain @p netif_index that refers to a V2X-enabled
   network interface is specific to the type of host operating system. For
   example, on some systems if_nametoindex() could be used.

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
v2x_netif_profile_set(v2x_service_t *service,
                      int netif_index,
                      const v2x_netif_profile_t *profile);

/** V2X socket */
typedef struct v2x_socket v2x_socket_t;

/** V2X socket configuration */
typedef struct {
  /** Ingress/egress physical interface index */
  v2x_if_index_t if_index;

  /** V2X protocol descriptor */
  v2x_protocol_t protocol;

} v2x_socket_config_t;

/** V2X socket configuration default initializer */
#define V2X_SOCKET_CONFIG_INIT {            \
  .if_index = V2X_IF_INDEX_NA,              \
  .protocol = V2X_PROTOCOL_INIT,            \
}

/**
   Create V2X socket.

   @param[in] service V2X service instance
   @param[out] socket_ptr V2X socket pointer
   @param[in] config V2X socket configuration

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
v2x_socket_create(v2x_service_t *service,
            v2x_socket_t **socket_ptr,
            const v2x_socket_config_t *config);

/**
   Delete V2X socket.

   @param[in] socket V2X socket to delete

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t
v2x_socket_delete(v2x_socket_t *socket);

/** V2X send parameters */
typedef struct {
  /**
     Source MAC address

     If not set to all zeros (::EUI48_ZERO_INIT) and wlanTxSaOverrideEnabled is
     set to "true", the value of this field will determine the MAC frame's
     source address. Otherwise it has no effect.
  */
  eui48_t source_address;

  /** Destination MAC address */
  eui48_t dest_address;

  /**
     MAC User Priority

     @todo Currently unsupported.
  */
  v2x_user_priority_t user_priority;

  /**
     Radio channel on which the frame should be transmitted.
  */
  v2x_channel_id_t channel_id;

  /** Transmission data rate */
  v2x_datarate_t datarate;

  /** Transmission power level in units of 1/8 dBm */
  v2x_power_dbm8_t power_dbm8;

  /** 
     Expiration time in milliseconds.
     
     Given ::v2x_send is called at time t0, frames which were not transmitted
     until t0 + expiry_time_ms will be dropped.

     @note Only supported when non-blocking wait option is used.

     @todo Currently not supported in remote V2X service.
  */
  v2x_expiry_time_ms_t expiry_time_ms; 

} v2x_send_params_t;

/** V2X send parameters default initializer */
#define V2X_SEND_PARAMS_INIT {                      \
  .source_address = EUI48_ZERO_INIT,                \
  .dest_address = EUI48_BCAST_INIT,                 \
  .user_priority = V2X_USER_PRIORITY_NA,            \
  .channel_id = V2X_CHANNEL_ID_INIT,                \
  .datarate = V2X_DATARATE_NA,                      \
  .power_dbm8 = V2X_POWER_DBM8_NA,                  \
  .expiry_time_ms = V2X_EXPIRY_TIME_MS_NA,          \
}

/**
   Send V2X frame.

   @see @ref wait_usage.

   @param[in] socket V2X socket
   @param[in] data_ptr Pointer to start of data
   @param[in] data_size Size of data in bytes
   @param[in] params Input parameters of send operation
   @param[in] wait Wait specification (optional)

   @todo Currently @p wait is not supported -- operation is always non-blocking.

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
v2x_send(v2x_socket_t *socket,
         const void *data_ptr,
         size_t data_size,
         const v2x_send_params_t *params,
         const atlk_wait_t *wait);

/** V2X receive parameters */
typedef struct {
  /** Source MAC address */
  eui48_t source_address;

  /** Destination MAC address */
  eui48_t dest_address;

  /**
     MAC User Priority

     @todo Currently unsupported.
  */
  v2x_user_priority_t user_priority;

  /**
     Radio channel on which the frame was received. 
  */
  v2x_channel_id_t channel_id;

  /**
     Data rate

     @todo Currently unsupported.
  */
  v2x_datarate_t datarate;

  /** Average input power of frame in units of 1/8 dBm */
  v2x_power_dbm8_t power_dbm8;

  /**
     Receive time in microseconds

     Format: number of TAI microseconds since 2004-01-01T00:00:00Z (UTC).
  */
  uint64_t receive_time_us;

} v2x_receive_params_t;

/** V2X receive parameters default initializer */
#define V2X_RECEIVE_PARAMS_INIT {                \
  .source_address = EUI48_ZERO_INIT,             \
  .dest_address = EUI48_ZERO_INIT,               \
  .user_priority = V2X_USER_PRIORITY_NA,         \
  .channel_id = V2X_CHANNEL_ID_INIT,             \
  .datarate = V2X_DATARATE_NA,                   \
  .power_dbm8 = V2X_POWER_DBM8_NA,               \
  .receive_time_us = 0,                          \
}

/**
   Receive V2X frame.

   @see @ref wait_usage.

   @param[in] socket V2X socket
   @param[out] data_ptr Pointer to start of data
   @param[in,out] data_size_ptr Maximum (in) and actual (out) data size in bytes
   @param[out] params Output parameters of receive operation
   @param[in] wait Wait specification (optional)

   @todo Currently @p wait supports non-blocking or wait-forever options only.

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
v2x_receive(v2x_socket_t *socket,
            void *data_ptr,
            size_t *data_size_ptr,
            v2x_receive_params_t *params,
            const atlk_wait_t *wait);

/** V2X sample subscriber */
typedef struct v2x_sample_subscriber v2x_sample_subscriber_t;

/** V2X sample subscriber configuration */
typedef struct {
  /** Ingress/egress physical interface index */
  v2x_if_index_t if_index;

  /** Subscription sample type */
  v2x_sample_type_t type; 

} v2x_sample_subscriber_config_t;

/** V2X sample subscriber configuration default initializer */
#define V2X_SAMPLE_SUBSCRIBER_CONFIG_INIT {     \
  .if_index = V2X_IF_INDEX_NA,                  \
  .type = V2X_SAMPLE_TYPE_NA,                   \
}

/** 
   Create V2X sample subscriber.

   @param[in] service V2X service instance
   @param[out] subscriber_ptr V2X sample subscriber pointer
   @param[in] config V2X sample subscriber configuration

   @todo Currently this function is not supported by remote service.

   @retval ::ATLK_OK if succeeded
   @return Error code if failed 
*/
atlk_rc_t atlk_must_check
v2x_sample_subscriber_create(v2x_service_t *service,
            v2x_sample_subscriber_t **subscriber_ptr,
            const v2x_sample_subscriber_config_t *config); 

/**
   Delete V2X sample subscriber.

   @param[in] subscriber V2X sample subscriber to delete

   @todo Currently this function is not supported by remote service.

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t
v2x_sample_subscriber_delete(v2x_sample_subscriber_t *subscriber);

/**
   Receive a V2X sample.

   @param[in] subscriber V2X sample subscriber to delete
   @param[out] value Value of the requested sample
   @param[in] wait Wait specification (optional)

   @todo Currently @p wait supports non-blocking or wait-forever options only.
   @todo Currently this function is not supported by remote service.

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
v2x_sample_int32_receive(v2x_sample_subscriber_t *subscriber,
            int32_t *value, 
            const atlk_wait_t *wait);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_V2X_SERVICE_H */
