/* Copyright (C) 2014-2015 Autotalks Ltd. */
#ifndef _ATLK_CAN_H
#define _ATLK_CAN_H

#include <atlk/sdk.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   CAN common definitions
*/

/** CAN device ID (starts at zero) */
typedef uint8_t can_device_id_t;

/** Value indicating that CAN device ID is N/A */
#define CAN_DEVICE_ID_NA 0xffU

/** Maximum size of CAN message data in octets */
#define CAN_DATA_SIZE_MAX 8

/** CAN ID + flag bits */
typedef uint32_t can_id_t;

/** CAN ID length for base frame format */
#define CAN_ID_NUM_BITS_BASE 11

/** CAN ID length for extended frame format */
#define CAN_ID_NUM_BITS_EXTENDED 29

/** If this CAN ID bit is set, the frame is an error frame */
#define CAN_ID_ERROR_BIT 29

/**
   If this CAN ID bit is set, the frame is a remote
   transmission request (RTR) frame
*/
#define CAN_ID_RTR_BIT 30

/** If this CAN ID bit is set, the frame is an extended format frame */
#define CAN_ID_EXTENDED_BIT 31

/** CAN service instance */
typedef struct can_service can_service_t;

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_CAN_H */
