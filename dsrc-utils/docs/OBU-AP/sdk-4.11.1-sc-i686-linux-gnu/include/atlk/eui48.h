/* Copyright (C) 2012-2015 Autotalks Ltd. */
#ifndef _ATLK_EUI48_H
#define _ATLK_EUI48_H

#include <atlk/sdk.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   48-bit Extended Unique Identifier declarations and macros
*/

/** Length of EUI-48 in octets */
#define EUI48_LEN 6

/** 48-bit Extended Unique Identifier */
typedef struct {
  uint8_t octets[EUI48_LEN];
} eui48_t;

/** Initializer that represents an invalid MAC address */
#define EUI48_ZERO_INIT { .octets = { 0 } }

/** Initializer that represents a broadcast MAC address */
#define EUI48_BCAST_INIT { \
  .octets = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff } }

/**
   Initializer that takes the EUI-48 octets as arguments

   Example:
   @code{.c}
   eui48_t my_addr = EUI48_INIT(0x90, 0x56, 0x92, 0x0, 0x0, 0x1);
   @endcode
*/
#define EUI48_INIT(_0, _1, _2, _3, _4, _5) { \
  .octets = { _0, _1, _2, _3, _4, _5 } }

/** Format string for EUI-48 */
#define EUI48_FMT "%02x:%02x:%02x:%02x:%02x:%02x"

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_EUI48_H */
