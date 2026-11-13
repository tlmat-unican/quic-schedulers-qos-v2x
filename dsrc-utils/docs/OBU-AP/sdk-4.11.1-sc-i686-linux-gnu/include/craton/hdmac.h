/* Copyright (C) 2014-2015 Autotalks Ltd. */
#ifndef _CRATON_HDMAC_H
#define _CRATON_HDMAC_H

#include <atlk/sdk.h>

#include <craton/check.h>

/**
   @file
   CRATON HDMAC (DMA controller) definitions
*/

/** HDMAC channel ID */
typedef uint8_t hdmac_channel_id_t;

/** HDMAC minimum valid channel ID */
#define HDMAC_CHANNEL_ID_MIN 0

/** HDMAC maximum valid channel ID */
#define HDMAC_CHANNEL_ID_MAX 7

/** Value indicating that HDMAC channel ID is N/A */
#define HDMAC_CHANNEL_ID_NA 0xff

#endif /* _CRATON_HDMAC_H */
