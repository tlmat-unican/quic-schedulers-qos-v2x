/* Copyright (C) 2012-2015 Autotalks Ltd. */
#ifndef _ATLK_SDK_H
#define _ATLK_SDK_H

#include <stdint.h>
#include <stddef.h>

#include <atlk/verinfo.h>
#include <atlk/compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   Autotalks SDK common declarations and macros
*/

/** Return code type */
typedef unsigned int atlk_rc_t;

/* API function return code constructor */
#define ATLK_RC(code) ((atlk_rc_t)(code))

/** Operation successful */
#define ATLK_OK ATLK_RC(0)

/** Unspecified error */
#define ATLK_E_UNSPECIFIED ATLK_RC(1)

/** Invalid argument */
#define ATLK_E_INVALID_ARG ATLK_RC(2)

/** Operation not supported */
#define ATLK_E_UNSUPPORTED ATLK_RC(3)

/** Object in invalid state */
#define ATLK_E_INVALID_STATE ATLK_RC(4)

/** Object not found */
#define ATLK_E_NOT_FOUND ATLK_RC(5)

/** Object already exists */
#define ATLK_E_EXISTS ATLK_RC(6)

/** Not ready to perform operation */
#define ATLK_E_NOT_READY ATLK_RC(7)

/** Operation timed out */
#define ATLK_E_TIMEOUT ATLK_RC(8)

/** Numerical argument out of domain */
#define ATLK_E_OUT_OF_DOMAIN ATLK_RC(9)

/** Numerical result out of range */
#define ATLK_E_OUT_OF_RANGE ATLK_RC(10)

/** Failed to allocate memory */
#define ATLK_E_OUT_OF_MEMORY ATLK_RC(11)

/** Address already in use */
#define ATLK_E_ADDRESS_IN_USE ATLK_RC(12)

/** Connection refused */
#define ATLK_E_CONNECTION_REFUSED ATLK_RC(13)

/** Connection lost */
#define ATLK_E_CONNECTION_LOST ATLK_RC(14)

/** Protocol error */
#define ATLK_E_PROTOCOL_ERROR ATLK_RC(15)

/** Protocol version mismatch */
#define ATLK_E_PROTOCOL_MISMATCH ATLK_RC(16)

/** Array access out of bounds */
#define ATLK_E_OUT_OF_BOUNDS ATLK_RC(17)

/** Address not aligned as required */
#define ATLK_E_BAD_ALIGNMENT ATLK_RC(18)

/** Buffer is too small */
#define ATLK_E_BUFFER_TOO_SMALL ATLK_RC(19)

/** Input/output error */
#define ATLK_E_IO_ERROR ATLK_RC(20)

/**
   Convert ::atlk_rc_t to human-readable error message.

   Error string must @b not be freed by the caller.

   @param[in] rc Return code

   @return Error message string
*/
const char *atlk_rc_to_str(atlk_rc_t rc);

/**
   Tell whether a return code indicates an error.

   @retval 0 @c rc indicates success
   @retval 1 @c rc indicates error
*/
atlk_inline int atlk_error(atlk_rc_t rc)
{
  return atlk_unlikely(rc != ATLK_OK);
}

/** Wait option type */
typedef enum {
  /** Wait a time interval */
  ATLK_WAIT_INTERVAL = 0,

  /** Wait indefinitely */
  ATLK_WAIT_FOREVER = 1

} atlk_wait_type_t;

/**
   Wait option

   If atlk_wait_t::wait_type is ::ATLK_WAIT_INTERVAL then the maximum amount
   of time spent waiting by the calling thread will be atlk_wait_t::wait_usec
   microseconds, rounded up to an implementation-dependent timer resolution.

   If atlk_wait_t::wait_type is ::ATLK_WAIT_FOREVER then the calling thread
   will wait indefinitely.
*/
typedef struct {
  /** Wait option type */
  atlk_wait_type_t wait_type;

  /** Number of microseconds */
  uint32_t wait_usec;

} atlk_wait_t;

/** Wait option default initializer */
#define ATLK_WAIT_INIT {            \
  .wait_type = ATLK_WAIT_INTERVAL,  \
  .wait_usec = 0                    \
}

/** Predefined "wait forever" wait option */
extern const atlk_wait_t atlk_wait_forever;

/** Data fragment */
typedef struct {
  /** Pointer to start of fragment */
  void *fragment_ptr;

  /** Size of fragment in bytes */
  size_t fragment_size;

} atlk_fragment_t;

/** Read-only data fragment */
typedef struct {
  /** Pointer to start of fragment */
  const void *fragment_ptr;

  /** Size of fragment in bytes */
  size_t fragment_size;

} atlk_const_fragment_t;

/** Default initializer for atlk_fragment_t and atlk_const_fragment_t */
#define ATLK_FRAGMENT_INIT {        \
  .fragment_ptr = NULL,             \
  .fragment_size = 0                \
}

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_SDK_H */
