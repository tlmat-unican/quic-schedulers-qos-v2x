/* Copyright (C) 2015 Autotalks Ltd. */
#ifndef _ATLK_REMOTE_H
#define _ATLK_REMOTE_H

#include <atlk/sdk.h>
#include <atlk/eui48.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   Transport for remote service access
*/

/** Remote transport instance */
typedef struct remote_transport remote_transport_t;

/** IP remote transport configuration */
typedef struct {
  /** IPv4 server address in network order */
  uint32_t remote_ipv4_address;

  /** Max round-trip time in milliseconds */
  uint32_t max_rtt_ms;

  /** Local IPv4 address in network order */
  uint32_t local_ipv4_address;

} remote_ip_transport_config_t;

/** IP remote transport configuration default initializer */
#define REMOTE_IP_TRANSPORT_CONFIG_INIT {   \
  .remote_ipv4_address = 0,                 \
  .max_rtt_ms = 0,                          \
  .local_ipv4_address = 0,                  \
}

/**
   Create IP-based remote transport instance.

   @param[in] config IP remote transport configuration
   @param[out] transport_ptr Remote transport pointer

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
remote_ip_transport_create(const remote_ip_transport_config_t *config,
            remote_transport_t **transport_ptr);

/**
   Delete a remote transport instance.

   @param[in] transport_ptr Remote transport pointer

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t
remote_transport_delete(remote_transport_t *transport_ptr);

/**
   Get local IPv4 address of a Network interface.

   @param[in] interface_name Interface name (e.g. "eth0")
   @param[out] local_ipv4_address Local IPv4 address

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
remote_util_local_ipv4_address_get(const char *interface_name,
            uint32_t *local_ipv4_address);

/** Link layer driver operations. */
typedef struct  {
  /**
     Send link layer PDU.

     @param[in] device Link layer device context
     @param[in] pdu Link layer PDU pointer
     @param[in] pdu_size Link layer PDU size
  */
  atlk_rc_t
  (*ll_send)(void *device,
             const void *pdu,
             size_t pdu_size);

  /**
     Receive link layer PDU.

     @param[in] device Link layer device context
     @param[out] pdu Link layer PDU pointer
     @param[in,out] pdu_size_ptr Maximum (in) and actual (out)
         size of link layer PDU
  */
  atlk_rc_t
  (*ll_receive)(void *device,
                void *pdu,
                size_t *pdu_size_ptr);

  /**
     Get MAC address of link layer device.

     @param[in] device Link layer device context
     @param[out] address Device MAC address
  */
  atlk_rc_t
  (*ll_address_get)(void *device,
                    eui48_t *address);

} remote_ll_device_ops_t;

/** Link layer remote transport configuration */
typedef struct {
  /** Link layer device operations */
  const remote_ll_device_ops_t *device_ops;

  /** Link layer device context */
  void *device;

  /** Max round-trip time in milliseconds */
  uint32_t max_rtt_ms;

  /** MAC address of remote device */
  eui48_t remote_address;

} remote_ll_transport_config_t;

/** IP remote transport configuration default initializer */
#define REMOTE_LL_TRANSPORT_CONFIG_INIT {   \
  .device_ops = NULL,                       \
  .device = NULL,                           \
  .max_rtt_ms = 0,                          \
  .remote_address = EUI48_ZERO_INIT,        \
}

/**
   Create link layer based remote transport instance.

   @param[in] config Link layer transport configuration
   @param[out] transport_ptr Remote transport pointer

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
remote_ll_transport_create(const remote_ll_transport_config_t *config,
            remote_transport_t **transport_ptr);
#ifdef __cplusplus
}
#endif

#endif /* _ATLK_REMOTE_H */
