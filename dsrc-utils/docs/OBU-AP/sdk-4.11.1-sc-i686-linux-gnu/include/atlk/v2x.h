/* Copyright (C) 2014-2015 Autotalks Ltd. */
#ifndef _ATLK_V2X_H
#define _ATLK_V2X_H

#include <atlk/sdk.h>
#include <atlk/eui48.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   V2X API declarations
*/

/** V2X physical interface index */
typedef uint8_t v2x_if_index_t;

/** Value indicating that V2X physical interface index is N/A */
#define V2X_IF_INDEX_NA 0

/** Minimum valid V2X physical interface index */
#define V2X_IF_INDEX_MIN 1

/** Maximum valid V2X physical interface index */
#define V2X_IF_INDEX_MAX 2

/** V2X MAC User Priority */
typedef int8_t v2x_user_priority_t;

/** V2X MAC User Priority minimum value */
#define V2X_USER_PRIORITY_MIN 0

/** V2X MAC User Priority maximum value */
#define V2X_USER_PRIORITY_MAX 7

/** Value indicating that V2X MAC User Priority is N/A */
#define V2X_USER_PRIORITY_NA INT8_MIN

/** Data rates in units of 500 kbit/s */
typedef enum {
  /** Data rate is N/A */
  V2X_DATARATE_NA = 0,

  /** 3 Mbit/s */
  V2X_DATARATE_3MBPS = 6,

  /** 4.5 Mbit/s */
  V2X_DATARATE_4_5MBPS = 9,

  /** 6 Mbit/s */
  V2X_DATARATE_6MBPS = 12,

  /** 9 Mbit/s */
  V2X_DATARATE_9MBPS = 18,

  /** 12 Mbit/s */
  V2X_DATARATE_12MBPS = 24,

  /** 18 Mbit/s */
  V2X_DATARATE_18MBPS = 36,

  /** 24 Mbit/s */
  V2X_DATARATE_24MBPS = 48,

  /** 27 Mbit/s */
  V2X_DATARATE_27MBPS = 54,

  /** 36 Mbit/s */
  V2X_DATARATE_36MBPS = 72,

  /** 48 Mbit/s */
  V2X_DATARATE_48MBPS = 96,

  /** 54 Mbit/s */
  V2X_DATARATE_54MBPS = 108

} v2x_datarate_t;

/** Power in units of dBm */
typedef int8_t v2x_power_dbm_t;

/** Value indicating that power in units of dBm is N/A */
#define V2X_POWER_DBM_NA INT8_MIN

/** Power in units of 1/8 dBm */
typedef int16_t v2x_power_dbm8_t;

/** Value indicating that power in units of 1/8 dBm is N/A */
#define V2X_POWER_DBM8_NA INT16_MIN

/** 1/8 dBm to dBm conversion factor */
#define V2X_POWER_DBM8_PER_DBM 8

/** Indefinite access */
#define V2X_INDEFINITE_IMMEDIATE_ACCESS 255

/**
   Operating class

   @see IEEE Std 802.11-2012, Annex E.

   @remark Operating class numbers intentionally don't follow the standard.
*/
typedef enum {
  /** No operating class selected */
  V2X_OP_CLASS_NA = 0,

  /** United States ITS 5 GHz, 10 MHz channel spacing */
  V2X_OP_CLASS_US_ITS_5GHZ_SPACING_10MHZ = 1,

  /** United States ITS 5 GHz, 20 MHz channel spacing */
  V2X_OP_CLASS_US_ITS_5GHZ_SPACING_20MHZ = 2,

  /** Europe ITS 5 GHz, 10 MHz channel spacing */
  V2X_OP_CLASS_EUROPE_ITS_5GHZ_SPACING_10MHZ = 3,

  /** Europe ITS 5 GHz, 20 MHz channel spacing */
  V2X_OP_CLASS_EUROPE_ITS_5GHZ_SPACING_20MHZ = 4

} v2x_op_class_t;

/** V2X radio channel number */
typedef uint8_t v2x_channel_num_t;

/** Value indicating that radio channel number is N/A */
#define V2X_CHANNEL_NUM_NA 0

/** V2X radio channel identifier */
typedef struct {
  /** Operating class */
  v2x_op_class_t op_class;

  /** Radio channel number */
  v2x_channel_num_t channel_num;

} v2x_channel_id_t;

#define V2X_CHANNEL_ID_INIT {        \
 .op_class = V2X_OP_CLASS_NA,        \
 .channel_num = V2X_CHANNEL_NUM_NA   \
}

/** A set of time slot */
typedef enum {
  /** No time slot selected */
  V2X_TIME_SLOT_NA = 0,

  /** Time slot #0 */
  V2X_TIME_SLOT_0 = 1,

  /** Time slot #1 */
  V2X_TIME_SLOT_1 = 2,

  /** Either time slot #0 or #1 */
  V2X_TIME_SLOT_EITHER = 3

} v2x_time_slot_t;

/** IEEE Std 1609.4-2016 service primitive MLMEX-CHSTART.request parameters */
typedef struct {
  /** V2X physical interface index on which access to channel is requested */
  v2x_if_index_t if_index;

  /** Radio channel identifier to be made available for communications */
  v2x_channel_id_t channel_id;

  /** The time slot in which alternating access is requested  */
  v2x_time_slot_t time_slot;

  /**
     Number of sync intervals to immediately remain on the selected channel
     before starting channel switching schedule.

     The value 0 means "immediate access not requested".
     The value 255 means "indefinite access".
  */
  uint8_t immediate_access;

} v2x_dot4_channel_start_request_t;

/** IEEE Std 1609.4-2016 service primitive MLMEX-CHEND.request parameters */
typedef struct {
  /** V2X physical interface index on which access is no longer required */
  v2x_if_index_t if_index;

  /** Radio channel identifier for which access is no longer required */
  v2x_channel_id_t channel_id;

} v2x_dot4_channel_end_request_t;

/** Reason for ending access to an IEEE Std 1609.4-2016 channel */
typedef enum {
  /** Unspecified reason */
  V2X_DOT4_CHANNEL_END_REASON_UNSPECIFIED = 0,

  /** Loss of time synchronization */
  V2X_DOT4_CHANNEL_END_REASON_LOSS_OF_SYNC = 1,

} v2x_dot4_channel_end_reason_t;

/** IEEE Std 1609.4-2016 service primitive MLMEX-CHEND.indication parameters */
typedef struct {
  /** V2X physical interface index on which access to channel was provided */
  v2x_if_index_t if_index;

  /** Radio channel identifier for which access is no longer provided */
  v2x_channel_id_t channel_id;

  /** Reason code */
  v2x_dot4_channel_end_reason_t reason;

} v2x_dot4_channel_end_indication_t;

/**
   Network interface V2X access profile.

   @see IEEE Std 1609.4-2016 MLMEX-REGISTERTXPROFILE.request and
   MLMEX-DELETETXPROFILE.request.
*/
typedef struct {
  /**
     V2X physical interface index to attach to.

     If equal to ::V2X_IF_INDEX_NA then the network interface is
     detached from any V2X physical interface it was attached to previously.
  */
  v2x_if_index_t if_index;

  /**
     Radio channel identifier to use.

     If @p channel_id.op_class is equal to ::V2X_OP_CLASS_NA, then the
     network interface will use not just one specific channel, but
     any channel accessed by the V2X physical interface.
  */
  v2x_channel_id_t channel_id;

  /** Transmission data rate for outgoing packets */
  v2x_datarate_t datarate;

  /** Transmission power level for outgoing packets */
  v2x_power_dbm8_t power_dbm8;

} v2x_netif_profile_t;

/** Network interface V2X access profile default initializer. */
#define V2X_NETIF_PROFILE_INIT {         \
  .if_index = V2X_IF_INDEX_NA,           \
  .channel_id = V2X_CHANNEL_ID_NA,       \
  .datarate = V2X_DATARATE_NA,           \
  .power_dbm8 = V2X_POWER_DBM_NA         \
}

/** V2X expiration time in milliseconds */
typedef uint16_t v2x_expiry_time_ms_t;

/** Value indicating maximum allowed expiration time in milliseconds */
#define V2X_EXPIRY_TIME_MS_MAX 0x7FFF

/** Value indicating that V2X expiration time is N/A */ 
#define V2X_EXPIRY_TIME_MS_NA 0

/** V2X MAC frame type */
typedef enum  {
  /**
     Data frame.
  */
  V2X_FRAME_TYPE_DATA = 0,

  /** IEEE 802.11 vendor-specific action frame */
  V2X_FRAME_TYPE_VSA = 1

} v2x_frame_type_t;

/**
   V2X protocol descriptor.

   For example:
   @code{.c}
   static const v2x_protocol_t wsmp_protocol = {
     .frame_type = V2X_FRAME_TYPE_DATA,
     .protocol_id = 0x88dc
   };
   @endcode
*/
typedef struct {
  /** V2X MAC frame type */
  v2x_frame_type_t frame_type;

  /**
     Protocol identifier.

     If @p frame_type is v2x_frame_type_t::V2X_FRAME_TYPE_DATA then this is
     the 5-octet SNAP protocol identifer.

     If @p frame_type is v2x_frame_type_t::V2X_FRAME_TYPE_VSA then this is the
     organizational identifier (OUI-36) and 4 least-significant bits specified
     by the identified organization.
  */
  uint64_t protocol_id;

} v2x_protocol_t;

/** Default protocol descriptor initializer */
#define V2X_PROTOCOL_INIT {             \
  .frame_type = V2X_FRAME_TYPE_DATA,    \
  .protocol_id = UINT64_MAX             \
}

/**
   Value indicating the protocol ID used to create a V2X socket which can
   send/receive frames with data which includes layer 2 (i.e. a "raw" socket
   above layer 1).

   @note Only one socket of this type can be created per physical interface
   index and frame type. Standard V2X sockets cannot be opened when this type
   of socket is open (and vice-versa).
*/
#define V2X_PROTOCOL_ID_ANY 0ULL

/** V2X sample type */
typedef enum {
  /** Sample type is N/A */
  V2X_SAMPLE_TYPE_NA = 0,

  /**
     CBR (channel busy ratio) sample.

     Receive samples via ::v2x_sample_int32_receive. Sample range
     and precision are the same as for ::mib_get_wlanChannelBusyRatio.
  */
  V2X_SAMPLE_TYPE_CBR = 1,

} v2x_sample_type_t;

/** V2X service instance */
typedef struct v2x_service v2x_service_t;

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_V2X_H */
