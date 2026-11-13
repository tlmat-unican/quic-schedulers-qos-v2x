/* Copyright (C) 2015-2016 Autotalks Ltd. */
/* File generated automatically; do not edit. */
/* Source: AUTOTALKS-RSVC-MIB revision 201608110000. */
#ifndef _ATLK_MIBS_RSVC_MIB_H
#define _ATLK_MIBS_RSVC_MIB_H

#include <atlk/mib_service.h>
#include <atlk/mibs/snmpv2-tc.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   AUTOTALKS-RSVC-MIB access API.

   Autotalks Remote Services MIB.
*/

/**
   Get rsvcDefaultDestIpAddressIPv4 value.

   Default destination IPv4 address for Remote Services messages.

   @param[in] service Instance of MIB service
   @param[out] value rsvcDefaultDestIpAddressIPv4 value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_rsvcDefaultDestIpAddressIPv4(mib_service_t *service,
            uint32_t *value);

/**
   Set rsvcDefaultDestIpAddressIPv4 value.

   Default destination IPv4 address for Remote Services messages.

   @param[in] service Instance of MIB service
   @param[in] value rsvcDefaultDestIpAddressIPv4 value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_rsvcDefaultDestIpAddressIPv4(mib_service_t *service,
            uint32_t value);

/**
   Get rsvcWlanFwdRowStatus value.

   WLAN packet forwarding conceptual row status.

   @param[in] service Instance of MIB service
   @param[in] rsvcWlanFwdIndex rsvcWlanFwdIndex value
   @param[out] value rsvcWlanFwdRowStatus value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_rsvcWlanFwdRowStatus(mib_service_t *service,
            int32_t rsvcWlanFwdIndex,
            mib_RowStatus_t *value);

/**
   Set rsvcWlanFwdRowStatus value.

   WLAN packet forwarding conceptual row status.

   @param[in] service Instance of MIB service
   @param[in] rsvcWlanFwdIndex rsvcWlanFwdIndex value
   @param[in] value rsvcWlanFwdRowStatus value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_rsvcWlanFwdRowStatus(mib_service_t *service,
            int32_t rsvcWlanFwdIndex,
            mib_RowStatus_t value);

/**
   Get rsvcWlanFwdDestPortNumber value.

   Destination port for forwarded WLAN packets.

   @param[in] service Instance of MIB service
   @param[in] rsvcWlanFwdIndex rsvcWlanFwdIndex value
   @param[out] value rsvcWlanFwdDestPortNumber value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_rsvcWlanFwdDestPortNumber(mib_service_t *service,
            int32_t rsvcWlanFwdIndex,
            uint16_t *value);

/**
   Set rsvcWlanFwdDestPortNumber value.

   Destination port for forwarded WLAN packets.

   @param[in] service Instance of MIB service
   @param[in] rsvcWlanFwdIndex rsvcWlanFwdIndex value
   @param[in] value rsvcWlanFwdDestPortNumber value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_rsvcWlanFwdDestPortNumber(mib_service_t *service,
            int32_t rsvcWlanFwdIndex,
            uint16_t value);

/**
   Get rsvcWlanFwdIfIndex value.

   WLAN MAC interface index.

   @param[in] service Instance of MIB service
   @param[in] rsvcWlanFwdIndex rsvcWlanFwdIndex value
   @param[out] value rsvcWlanFwdIfIndex value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_rsvcWlanFwdIfIndex(mib_service_t *service,
            int32_t rsvcWlanFwdIndex,
            int32_t *value);

/**
   Set rsvcWlanFwdIfIndex value.

   WLAN MAC interface index.

   @param[in] service Instance of MIB service
   @param[in] rsvcWlanFwdIndex rsvcWlanFwdIndex value
   @param[in] value rsvcWlanFwdIfIndex value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_rsvcWlanFwdIfIndex(mib_service_t *service,
            int32_t rsvcWlanFwdIndex,
            int32_t value);

/** Enumeration of rsvcWlanFwdFrameType values */
typedef enum {
  MIB_rsvcWlanFwdFrameType_data = 0,
  MIB_rsvcWlanFwdFrameType_vsa = 1
} mib_rsvcWlanFwdFrameType_t;

/**
   Get rsvcWlanFwdFrameType value.

   WLAN frame type.

   Value 'vsa' is not supported.

   @param[in] service Instance of MIB service
   @param[in] rsvcWlanFwdIndex rsvcWlanFwdIndex value
   @param[out] value rsvcWlanFwdFrameType value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_rsvcWlanFwdFrameType(mib_service_t *service,
            int32_t rsvcWlanFwdIndex,
            mib_rsvcWlanFwdFrameType_t *value);

/**
   Set rsvcWlanFwdFrameType value.

   WLAN frame type.

   Value 'vsa' is not supported.

   @param[in] service Instance of MIB service
   @param[in] rsvcWlanFwdIndex rsvcWlanFwdIndex value
   @param[in] value rsvcWlanFwdFrameType value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_rsvcWlanFwdFrameType(mib_service_t *service,
            int32_t rsvcWlanFwdIndex,
            mib_rsvcWlanFwdFrameType_t value);

/**
   Get rsvcWlanFwdProtocolId value.

   WLAN packet protocol ID.

   @param[in] service Instance of MIB service
   @param[in] rsvcWlanFwdIndex rsvcWlanFwdIndex value
   @param[out] value rsvcWlanFwdProtocolId value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_rsvcWlanFwdProtocolId(mib_service_t *service,
            int32_t rsvcWlanFwdIndex,
            uint64_t *value);

/**
   Set rsvcWlanFwdProtocolId value.

   WLAN packet protocol ID.

   @param[in] service Instance of MIB service
   @param[in] rsvcWlanFwdIndex rsvcWlanFwdIndex value
   @param[in] value rsvcWlanFwdProtocolId value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_rsvcWlanFwdProtocolId(mib_service_t *service,
            int32_t rsvcWlanFwdIndex,
            uint64_t value);

/**
   Get rsvcWlanBridgeEnabled value.

   Whether bridging of WLAN to local VLAN is enabled.

   @param[in] service Instance of MIB service
   @param[out] value rsvcWlanBridgeEnabled value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_rsvcWlanBridgeEnabled(mib_service_t *service,
            int *value);

/**
   Set rsvcWlanBridgeEnabled value.

   Whether bridging of WLAN to local VLAN is enabled.

   @param[in] service Instance of MIB service
   @param[in] value rsvcWlanBridgeEnabled value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_rsvcWlanBridgeEnabled(mib_service_t *service,
            int value);

/**
   Get rsvcWlanBridgeIfIndex value.

   WLAN interface index that's bridged to a local VLAN ID.

   @param[in] service Instance of MIB service
   @param[out] value rsvcWlanBridgeIfIndex value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_rsvcWlanBridgeIfIndex(mib_service_t *service,
            int32_t *value);

/**
   Set rsvcWlanBridgeIfIndex value.

   WLAN interface index that's bridged to a local VLAN ID.

   @param[in] service Instance of MIB service
   @param[in] value rsvcWlanBridgeIfIndex value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_rsvcWlanBridgeIfIndex(mib_service_t *service,
            int32_t value);

/**
   Get rsvcWlanBridgeVlanId value.

   VLAN ID of the VLAN that's bridged to WLAN interface selected by
   rsvcWlanBridgeIfIndex.

   @param[in] service Instance of MIB service
   @param[out] value rsvcWlanBridgeVlanId value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_rsvcWlanBridgeVlanId(mib_service_t *service,
            int32_t *value);

/**
   Set rsvcWlanBridgeVlanId value.

   VLAN ID of the VLAN that's bridged to WLAN interface selected by
   rsvcWlanBridgeIfIndex.

   @param[in] service Instance of MIB service
   @param[in] value rsvcWlanBridgeVlanId value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_rsvcWlanBridgeVlanId(mib_service_t *service,
            int32_t value);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_MIBS_RSVC_MIB_H */
