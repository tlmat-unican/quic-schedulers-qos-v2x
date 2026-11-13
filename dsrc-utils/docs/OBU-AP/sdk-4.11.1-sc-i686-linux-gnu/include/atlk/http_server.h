/* Copyright (C) 2016 Autotalks Ltd. */
#ifndef _ATLK_HTTP_SERVER_H
#define _ATLK_HTTP_SERVER_H

#include <atlk/sdk.h>
#include <atlk/os.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   HTTP Server API

   Support is limited to HTTP/1.0. The following are not supported:

     - Persistent connections
     - Request pipelining
     - Content compression
     - TRACE, OPTIONS and CONNECT requests
*/

/** Maximum response content size */
#define HTTP_RESPONSE_CONTENT_MAX_LEN 1024

/** HTTP request type */
typedef enum {
  /** HTTP GET request */
  HTTP_REQUEST_TYPE_GET = 0,

  /** HTTP POST request */
  HTTP_REQUEST_TYPE_POST = 1,

  /** HTTP HEAD request */
  HTTP_REQUEST_TYPE_HEAD = 2,

  /** HTTP PUT request */
  HTTP_REQUEST_TYPE_PUT = 3,

  /** HTTP DELETE request */
  HTTP_REQUEST_TYPE_DELETE = 4

} http_request_type_t;

/**
   URL handler.

   When request is not handled (::ATLK_E_NOT_FOUND is returned), HTTP server
   will look for a page stored on FS with the same URL.

   @param[in] resource Resource name
   @param[in] request_type HTTP request type
   @param[in] request_content HTTP request content
   @param[in] request_content_size HTTP request content size
   @param[out] response_content HTTP response content
   @param[in,out] response_content_size HTTP response content size
   @param[out] status_code HTTP status code

   @retval ::ATLK_OK if succeeded
   @retval ::ATLK_E_NOT_FOUND if request is not handled by handler
   @return Error code if failed
 */
typedef atlk_rc_t atlk_must_check
(*http_url_handler_t)(const char *resource,
            http_request_type_t request_type,
            const void *request_content,
            size_t request_content_size,
            void *response_content,
            size_t *response_content_size,
            uint16_t *status_code);

/** URL entry */
typedef struct {
  /** URL address */
  const char *url;

  /** URL handler */
  http_url_handler_t url_handler;

} http_url_entry_t;

/** HTTP server configuration parameters */
typedef struct {
  /** Default path prefix (currently unsupported) */
  const char *default_path_prefix;

  /** HTTP thread scheduling parameters */
  atlk_thread_sched_t sched_params;

} http_server_config_t;

/** HTTP server configuration parameters default initializer */
#define HTTP_SERVER_CONFIG_INIT {         \
  .default_path_prefix = NULL,            \
  .sched_params = ATLK_THREAD_SCHED_INIT  \
}

/**
   Initialize HTTP server.

   @param[in] config HTTP server configuration parameters (optional)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
http_server_init(const http_server_config_t *config);

/**
   Register HTTP module URL entries.

   @param[in] module_name Module name (must be unique)
   @param[in] module_url_entry_array_ptr Pointer to URL entries array
   @param[in] module_url_entry_array_count URL entries array count

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
 */
atlk_rc_t atlk_must_check 
http_server_module_register(const char *module_name,
            const http_url_entry_t *module_url_entry_array_ptr,
            size_t module_url_entry_array_count);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_HTTP_SERVER_H */
