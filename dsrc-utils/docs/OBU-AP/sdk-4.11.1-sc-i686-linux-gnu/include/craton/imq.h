/* Copyright (C) 2014-2015 Autotalks Ltd. */
#ifndef _CRATON_IMQ_H
#define _CRATON_IMQ_H

#include <atlk/sdk.h>

#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   IMQ API
*/

/**
   IMQ configuration parameters.
   
   @todo Currently unused.
*/
typedef struct imq_config imq_config_t;

/**
   Initialize IMQ on a specific CPU.

   @param[in] config IMQ configuration parameters (optional)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
imq_init(const imq_config_t *config);

/** IMQ address */
typedef uint16_t imq_address_t;

/** Value indicating that IMQ address is N/A */
#define IMQ_ADDRESS_NA UINT16_MAX

/** IMQ address maximum value */
#define IMQ_ADDRESS_MAX 31

/** IMQ socket */
typedef struct {
  /* IMQ address */
  imq_address_t __address;

  /* Is the socket a server socket */
  int __is_server;

  /* Is the socket connected */
  int __is_connected;

} imq_socket_t;

/** IMQ socket default initializer */
#define IMQ_SOCKET_INIT {        \
  .__address = IMQ_ADDRESS_NA,   \
  .__is_server = 0,              \
  .__is_connected = 0            \
}

/**
   Bind IMQ socket.

   @param[in] socket IMQ socket
   @param[in] address IMQ address

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
imq_bind(imq_socket_t *socket,
            imq_address_t address);

/** IMQ queue configuration */
typedef struct {
  /** IMQ queue MTU */
  uint16_t queue_mtu;
  
  /** IMQ queue length */
  uint32_t queue_length;

} imq_queue_config_t;

/** IMQ queue configuration default initializer */
#define IMQ_QUEUE_CONFIG_INIT {     \
  .queue_mtu = 0,                   \
  .queue_length = 0                 \
}

/** Maximum length for IMQ socket name. */
#define IMQ_SERVICE_NAME_LENGTH_MAX 16

/** IMQ service configuration */
typedef struct {
  /** Client to server queue configuration */
  imq_queue_config_t client_to_server_config;
  
  /** Server to client queue configuration */
  imq_queue_config_t server_to_client_config;

  /** IMQ service name */
  char *service_name;

} imq_service_config_t;

/** IMQ service configuration default initializer */
#define IMQ_SERVICE_CONFIG_INIT {                       \
  .client_to_server_config = IMQ_QUEUE_CONFIG_INIT,     \
  .server_to_client_config = IMQ_QUEUE_CONFIG_INIT,     \
  .service_name = NULL                                  \
}

/**
   Listen for incoming IMQ connections.
 
   @param[in] socket IMQ socket
   @param[in] config Service configuration
   
   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
imq_listen(imq_socket_t *socket,
            const imq_service_config_t *config);

/**
   Accept IMQ connection.

   @see @ref wait_usage.

   @param[in] socket IMQ socket
   @param[out] connected_socket Connected IMQ socket
   @param[in] wait Wait specification (optional)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
imq_accept(imq_socket_t *socket,
            imq_socket_t *connected_socket,
            const atlk_wait_t *wait);

/**
   Connect to IMQ server.

   @see @ref wait_usage.

   @param[in] socket IMQ socket
   @param[in] address IMQ server address
   @param[in] wait Wait specification (optional)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
imq_connect(imq_socket_t *socket,
            imq_address_t address,
            const atlk_wait_t *wait);

/**
   Send IMQ message.

   @see @ref wait_usage.

   @param[in] socket IMQ socket
   @param[in] data_ptr Pointer to start of data
   @param[in] data_size Size of data in bytes
   @param[in] wait Wait specification (optional)

   @todo Currently @p wait supports non-blocking or wait-forever options only.

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
imq_send(imq_socket_t *socket,
            const void *data_ptr,
            size_t data_size,
            const atlk_wait_t *wait);

/**
   Send IMQ scatter-gather message.

   @see @ref wait_usage.

   @param[in] socket IMQ socket
   @param[in] fragment_array_ptr Pointer to array of data fragments
   @param[in] fragment_array_size Number of data fragments to send
   @param[in] wait Wait specification (optional)

   @todo Currently @p wait supports non-blocking or wait-forever options only.

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
imq_fragmented_send(imq_socket_t *socket,
            const atlk_const_fragment_t *fragment_array_ptr,
            size_t fragment_array_size,
            const atlk_wait_t *wait);

/**
   Receive IMQ message.

   @see @ref wait_usage.

   @param[in] socket IMQ socket
   @param[in] data_ptr Pointer to start of data
   @param[in,out] data_size_ptr Maximum (in) and actual (out) data size in bytes
   @param[in] wait Wait specification (optional)
  
   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
imq_receive(imq_socket_t *socket,
            void *data_ptr,
            size_t *data_size_ptr,
            const atlk_wait_t *wait);

/**
   Receive IMQ message into a scatter-gather buffer.

   @see @ref wait_usage.

   @param[in] socket IMQ socket
   @param[in] fragment_array_ptr Pointer to array of data fragments
   @param[in] fragment_array_size Number of data fragments
   @param[out] data_size_ptr Data size in bytes
   @param[in] wait Wait specification (optional)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
imq_fragmented_receive(imq_socket_t *socket,
            const atlk_fragment_t *fragment_array_ptr,
            size_t fragment_array_size,
            size_t *data_size_ptr,
            const atlk_wait_t *wait);

/**
   Close IMQ socket.

   @param[in] socket IMQ socket
   
   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t
imq_close(imq_socket_t *socket);

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_IMQ_H */

