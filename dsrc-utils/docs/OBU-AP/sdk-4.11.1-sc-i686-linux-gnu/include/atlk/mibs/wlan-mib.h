/* Copyright (C) 2012-2016 Autotalks Ltd. */
/* File generated automatically; do not edit. */
/* Source: AUTOTALKS-WLAN-MIB revision 201608160000. */
#ifndef _ATLK_MIBS_WLAN_MIB_H
#define _ATLK_MIBS_WLAN_MIB_H

#include <atlk/mib_service.h>
#include <atlk/eui48.h>
#include <atlk/mibs/tc.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   AUTOTALKS-WLAN-MIB access API.

   CRATON WLAN MIB definition.
*/

/**
   Get wlanConfigSaveStatus value.

   WLAN configuration save status.

   @param[in] service Instance of MIB service
   @param[out] value wlanConfigSaveStatus value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanConfigSaveStatus(mib_service_t *service,
            mib_ConfigSaveStatus_t *value);

/**
   Set wlanConfigSaveStatus value.

   WLAN configuration save status.

   @param[in] service Instance of MIB service
   @param[in] value wlanConfigSaveStatus value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanConfigSaveStatus(mib_service_t *service,
            mib_ConfigSaveStatus_t value);

/** Minimum wlanDefaultTxDataRate value (units: 500 kbit/s) */
#define MIB_wlanDefaultTxDataRate_MIN 6

/** Maximum wlanDefaultTxDataRate value (units: 500 kbit/s) */
#define MIB_wlanDefaultTxDataRate_MAX 108

/**
   Get wlanDefaultTxDataRate value (units: 500 kbit/s).

   Default transmission data rate.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[out] value wlanDefaultTxDataRate value (units: 500 kbit/s)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanDefaultTxDataRate(mib_service_t *service,
            int32_t ifIndex,
            int32_t *value);

/**
   Set wlanDefaultTxDataRate value (units: 500 kbit/s).

   Default transmission data rate.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[in] value wlanDefaultTxDataRate value (units: 500 kbit/s)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanDefaultTxDataRate(mib_service_t *service,
            int32_t ifIndex,
            int32_t value);

/**
   Get wlanBssid value.

   802.11 BSSID address.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[out] value wlanBssid value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanBssid(mib_service_t *service,
            int32_t ifIndex,
            eui48_t *value);

/**
   Set wlanBssid value.

   802.11 BSSID address.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[in] value wlanBssid value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanBssid(mib_service_t *service,
            int32_t ifIndex,
            eui48_t value);

/** Minimum wlanDefaultTxPower value (units: dBm) */
#define MIB_wlanDefaultTxPower_MIN (-30)

/** Maximum wlanDefaultTxPower value (units: dBm) */
#define MIB_wlanDefaultTxPower_MAX 33

/**
   Get wlanDefaultTxPower value (units: dBm).

   Default transmission power.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[out] value wlanDefaultTxPower value (units: dBm)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanDefaultTxPower(mib_service_t *service,
            int32_t ifIndex,
            int32_t *value);

/**
   Set wlanDefaultTxPower value (units: dBm).

   Default transmission power.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[in] value wlanDefaultTxPower value (units: dBm)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanDefaultTxPower(mib_service_t *service,
            int32_t ifIndex,
            int32_t value);

/**
   Get wlanRandomBackoffEnabled value.

   Whether MAC transmission random backoff is enabled.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[out] value wlanRandomBackoffEnabled value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanRandomBackoffEnabled(mib_service_t *service,
            int32_t ifIndex,
            int *value);

/**
   Set wlanRandomBackoffEnabled value.

   Whether MAC transmission random backoff is enabled.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[in] value wlanRandomBackoffEnabled value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanRandomBackoffEnabled(mib_service_t *service,
            int32_t ifIndex,
            int value);

/**
   Get wlanMacAddress value.

   802.11 MAC Address.

   Please note that MAC frame queues are not flushed when the address
   is changed, meaning that the previous address value may appear as
   outgoing frame source address or incoming destination address (in the
   case of unicast frames) some time after the change.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[out] value wlanMacAddress value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanMacAddress(mib_service_t *service,
            int32_t ifIndex,
            eui48_t *value);

/**
   Set wlanMacAddress value.

   802.11 MAC Address.

   Please note that MAC frame queues are not flushed when the address
   is changed, meaning that the previous address value may appear as
   outgoing frame source address or incoming destination address (in the
   case of unicast frames) some time after the change.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[in] value wlanMacAddress value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanMacAddress(mib_service_t *service,
            int32_t ifIndex,
            eui48_t value);

/**
   Get wlanTxSaOverrideEnabled value.

   Whether source address override is enabled.

   If enabled, source MAC address (SA) can be set arbitrarily per frame
   transmission by upper layer, without changing wlanMacAddress.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[out] value wlanTxSaOverrideEnabled value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanTxSaOverrideEnabled(mib_service_t *service,
            int32_t ifIndex,
            int *value);

/**
   Set wlanTxSaOverrideEnabled value.

   Whether source address override is enabled.

   If enabled, source MAC address (SA) can be set arbitrarily per frame
   transmission by upper layer, without changing wlanMacAddress.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[in] value wlanTxSaOverrideEnabled value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanTxSaOverrideEnabled(mib_service_t *service,
            int32_t ifIndex,
            int value);

/**
   Get wlanRxUcastDaFilterEnabled value.

   Whether unicast destination address filter is enabled.

   If enabled, MAC will drop unicast frames which have destination
   MAC address different from the receiving station MAC address.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[out] value wlanRxUcastDaFilterEnabled value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanRxUcastDaFilterEnabled(mib_service_t *service,
            int32_t ifIndex,
            int *value);

/**
   Set wlanRxUcastDaFilterEnabled value.

   Whether unicast destination address filter is enabled.

   If enabled, MAC will drop unicast frames which have destination
   MAC address different from the receiving station MAC address.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[in] value wlanRxUcastDaFilterEnabled value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanRxUcastDaFilterEnabled(mib_service_t *service,
            int32_t ifIndex,
            int value);

/** Minimum wlanShortRetryLimit value */
#define MIB_wlanShortRetryLimit_MIN 1

/** Maximum wlanShortRetryLimit value */
#define MIB_wlanShortRetryLimit_MAX 255

/**
   Get wlanShortRetryLimit value.

   This attribute indicates the maximum number of transmission attempts
   of a frame, the length of which is less than or equal to RTSThreshold,
   that is made before a failure condition is indicated.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[out] value wlanShortRetryLimit value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanShortRetryLimit(mib_service_t *service,
            int32_t ifIndex,
            int32_t *value);

/**
   Set wlanShortRetryLimit value.

   This attribute indicates the maximum number of transmission attempts
   of a frame, the length of which is less than or equal to RTSThreshold,
   that is made before a failure condition is indicated.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[in] value wlanShortRetryLimit value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanShortRetryLimit(mib_service_t *service,
            int32_t ifIndex,
            int32_t value);

/** Minimum wlanDefaultTxPowerDbm8 value (units: 1/8 dBm) */
#define MIB_wlanDefaultTxPowerDbm8_MIN (-240)

/** Maximum wlanDefaultTxPowerDbm8 value (units: 1/8 dBm) */
#define MIB_wlanDefaultTxPowerDbm8_MAX 264

/**
   Get wlanDefaultTxPowerDbm8 value (units: 1/8 dBm).

   Default transmission power in 1/8 dBm.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[out] value wlanDefaultTxPowerDbm8 value (units: 1/8 dBm)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanDefaultTxPowerDbm8(mib_service_t *service,
            int32_t ifIndex,
            int32_t *value);

/**
   Set wlanDefaultTxPowerDbm8 value (units: 1/8 dBm).

   Default transmission power in 1/8 dBm.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[in] value wlanDefaultTxPowerDbm8 value (units: 1/8 dBm)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanDefaultTxPowerDbm8(mib_service_t *service,
            int32_t ifIndex,
            int32_t value);

/**
   Get wlanQosDataEnabled value.

   Whether 802.11 QoS data is enabled.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[out] value wlanQosDataEnabled value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanQosDataEnabled(mib_service_t *service,
            int32_t ifIndex,
            int *value);

/**
   Set wlanQosDataEnabled value.

   Whether 802.11 QoS data is enabled.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[in] value wlanQosDataEnabled value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanQosDataEnabled(mib_service_t *service,
            int32_t ifIndex,
            int value);

/**
   Get wlanFrameTxCnt value.

   This counter shall be incremented for each transmitted frame.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[out] value wlanFrameTxCnt value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanFrameTxCnt(mib_service_t *service,
            int32_t ifIndex,
            uint32_t *value);

/**
   Get wlanFrameRxCnt value.

   This counter shall be incremented for each correctly received frame.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[out] value wlanFrameRxCnt value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanFrameRxCnt(mib_service_t *service,
            int32_t ifIndex,
            uint32_t *value);

/**
   Get wlanTxFailCnt value.

   This counter shall be incremented for each failure during frame
   transmission (including allocation, queuing and other failures).

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[out] value wlanTxFailCnt value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanTxFailCnt(mib_service_t *service,
            int32_t ifIndex,
            uint32_t *value);

/**
   Get wlanTxAllocFailCnt value.

   This counter shall be incremented for each memory allocation failure
   during frame transmission.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[out] value wlanTxAllocFailCnt value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanTxAllocFailCnt(mib_service_t *service,
            int32_t ifIndex,
            uint32_t *value);

/**
   Get wlanTxQueueFailCnt value.

   This counter shall be incremented for each queuing failure during
   frame transmission.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[out] value wlanTxQueueFailCnt value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanTxQueueFailCnt(mib_service_t *service,
            int32_t ifIndex,
            uint32_t *value);

/**
   Get wlanRxFailCnt value.

   This counter shall be incremented for each failure during frame
   reception (including allocation, queuing, CRC and other failures).

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[out] value wlanRxFailCnt value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanRxFailCnt(mib_service_t *service,
            int32_t ifIndex,
            uint32_t *value);

/**
   Get wlanRxAllocFailCnt value.

   This counter shall be incremented for each memory allocation failure
   during frame reception.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[out] value wlanRxAllocFailCnt value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanRxAllocFailCnt(mib_service_t *service,
            int32_t ifIndex,
            uint32_t *value);

/**
   Get wlanRxQueueFailCnt value.

   This counter shall be incremented for each queuing failure during
   frame reception.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[out] value wlanRxQueueFailCnt value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanRxQueueFailCnt(mib_service_t *service,
            int32_t ifIndex,
            uint32_t *value);

/**
   Get wlanRxCrcFailCnt value.

   This counter shall be incremented for each CRC failure during
   frame reception.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[out] value wlanRxCrcFailCnt value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanRxCrcFailCnt(mib_service_t *service,
            int32_t ifIndex,
            uint32_t *value);

/**
   Get wlanRxDuplicateFrameFilteringEnabled value.

   Whether MAC duplicate frame filtering is enabled in RX diversity mode.

   @param[in] service Instance of MIB service
   @param[out] value wlanRxDuplicateFrameFilteringEnabled value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanRxDuplicateFrameFilteringEnabled(mib_service_t *service,
            int *value);

/**
   Set wlanRxDuplicateFrameFilteringEnabled value.

   Whether MAC duplicate frame filtering is enabled in RX diversity mode.

   @param[in] service Instance of MIB service
   @param[in] value wlanRxDuplicateFrameFilteringEnabled value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanRxDuplicateFrameFilteringEnabled(mib_service_t *service,
            int value);

/** Minimum wlanEdcaCWmin value */
#define MIB_wlanEdcaCWmin_MIN 0

/** Maximum wlanEdcaCWmin value */
#define MIB_wlanEdcaCWmin_MAX 255

/**
   Get wlanEdcaCWmin value.

   The minimum size of the window that is used for generating a random
   number for the backoff.

   The value of this attribute is such that it could always be expressed
   in the form of 2**X - 1, where X is an integer.

   @param[in] service Instance of MIB service
   @param[in] wlanEdcaIndex wlanEdcaIndex value
   @param[out] value wlanEdcaCWmin value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanEdcaCWmin(mib_service_t *service,
            int32_t wlanEdcaIndex,
            uint32_t *value);

/**
   Set wlanEdcaCWmin value.

   The minimum size of the window that is used for generating a random
   number for the backoff.

   The value of this attribute is such that it could always be expressed
   in the form of 2**X - 1, where X is an integer.

   @param[in] service Instance of MIB service
   @param[in] wlanEdcaIndex wlanEdcaIndex value
   @param[in] value wlanEdcaCWmin value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanEdcaCWmin(mib_service_t *service,
            int32_t wlanEdcaIndex,
            uint32_t value);

/** Minimum wlanEdcaCWmax value */
#define MIB_wlanEdcaCWmax_MIN 0

/** Maximum wlanEdcaCWmax value */
#define MIB_wlanEdcaCWmax_MAX 65535

/**
   Get wlanEdcaCWmax value.

   The maximum size of the window that is used for generating a random
   number for the backoff.

   The value of this attribute is such that it could always be expressed
   in the form of 2**X - 1, where X is an integer.

   @param[in] service Instance of MIB service
   @param[in] wlanEdcaIndex wlanEdcaIndex value
   @param[out] value wlanEdcaCWmax value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanEdcaCWmax(mib_service_t *service,
            int32_t wlanEdcaIndex,
            uint32_t *value);

/**
   Set wlanEdcaCWmax value.

   The maximum size of the window that is used for generating a random
   number for the backoff.

   The value of this attribute is such that it could always be expressed
   in the form of 2**X - 1, where X is an integer.

   @param[in] service Instance of MIB service
   @param[in] wlanEdcaIndex wlanEdcaIndex value
   @param[in] value wlanEdcaCWmax value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanEdcaCWmax(mib_service_t *service,
            int32_t wlanEdcaIndex,
            uint32_t value);

/** Minimum wlanCsIntervalA value (units: millisecond) */
#define MIB_wlanCsIntervalA_MIN 0

/** Maximum wlanCsIntervalA value (units: millisecond) */
#define MIB_wlanCsIntervalA_MAX 255

/** Default wlanCsIntervalA value (units: millisecond) */
#define MIB_wlanCsIntervalA_DEFVAL 50

/**
   Get wlanCsIntervalA value (units: millisecond).

   Channel A interval.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[out] value wlanCsIntervalA value (units: millisecond)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanCsIntervalA(mib_service_t *service,
            int32_t ifIndex,
            int32_t *value);

/**
   Set wlanCsIntervalA value (units: millisecond).

   Channel A interval.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[in] value wlanCsIntervalA value (units: millisecond)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanCsIntervalA(mib_service_t *service,
            int32_t ifIndex,
            int32_t value);

/** Minimum wlanCsIntervalB value (units: millisecond) */
#define MIB_wlanCsIntervalB_MIN 0

/** Maximum wlanCsIntervalB value (units: millisecond) */
#define MIB_wlanCsIntervalB_MAX 255

/** Default wlanCsIntervalB value (units: millisecond) */
#define MIB_wlanCsIntervalB_DEFVAL 50

/**
   Get wlanCsIntervalB value (units: millisecond).

   Channel B interval.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[out] value wlanCsIntervalB value (units: millisecond)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanCsIntervalB(mib_service_t *service,
            int32_t ifIndex,
            int32_t *value);

/**
   Set wlanCsIntervalB value (units: millisecond).

   Channel B interval.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[in] value wlanCsIntervalB value (units: millisecond)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanCsIntervalB(mib_service_t *service,
            int32_t ifIndex,
            int32_t value);

/** Minimum wlanCsSyncTolerance value (units: milliseconds) */
#define MIB_wlanCsSyncTolerance_MIN 0

/** Maximum wlanCsSyncTolerance value (units: milliseconds) */
#define MIB_wlanCsSyncTolerance_MAX 255

/** Default wlanCsSyncTolerance value (units: milliseconds) */
#define MIB_wlanCsSyncTolerance_DEFVAL 2

/**
   Get wlanCsSyncTolerance value (units: milliseconds).

   This attribute is equivalent to SyncTolerance as defined in IEEE
   1609.4-2010 clause 6.2.5.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[out] value wlanCsSyncTolerance value (units: milliseconds)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanCsSyncTolerance(mib_service_t *service,
            int32_t ifIndex,
            int32_t *value);

/**
   Set wlanCsSyncTolerance value (units: milliseconds).

   This attribute is equivalent to SyncTolerance as defined in IEEE
   1609.4-2010 clause 6.2.5.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[in] value wlanCsSyncTolerance value (units: milliseconds)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanCsSyncTolerance(mib_service_t *service,
            int32_t ifIndex,
            int32_t value);

/**
   Get wlanEpdEnabled value.

   Whether IEEE Std 802-2014 EtherType Protocol Discrimination (EPD) is
   used in the LLC sublayer.

   @param[in] service Instance of MIB service
   @param[out] value wlanEpdEnabled value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanEpdEnabled(mib_service_t *service,
            int *value);

/**
   Set wlanEpdEnabled value.

   Whether IEEE Std 802-2014 EtherType Protocol Discrimination (EPD) is
   used in the LLC sublayer.

   @param[in] service Instance of MIB service
   @param[in] value wlanEpdEnabled value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanEpdEnabled(mib_service_t *service,
            int value);

/**
   Get wlanTxDiversityEnabled value.

   Whether TX diversity is enabled.

   @param[in] service Instance of MIB service
   @param[out] value wlanTxDiversityEnabled value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanTxDiversityEnabled(mib_service_t *service,
            int *value);

/**
   Set wlanTxDiversityEnabled value.

   Whether TX diversity is enabled.

   @param[in] service Instance of MIB service
   @param[in] value wlanTxDiversityEnabled value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanTxDiversityEnabled(mib_service_t *service,
            int value);

/** Minimum wlanTxCsd value (units: samples) */
#define MIB_wlanTxCsd_MIN 0

/** Maximum wlanTxCsd value (units: samples) */
#define MIB_wlanTxCsd_MAX 4

/**
   Get wlanTxCsd value (units: samples).

   Cyclic shift delay to the transmitted OFDM symbol.

   @param[in] service Instance of MIB service
   @param[out] value wlanTxCsd value (units: samples)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanTxCsd(mib_service_t *service,
            int32_t *value);

/**
   Set wlanTxCsd value (units: samples).

   Cyclic shift delay to the transmitted OFDM symbol.

   @param[in] service Instance of MIB service
   @param[in] value wlanTxCsd value (units: samples)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanTxCsd(mib_service_t *service,
            int32_t value);

/**
   Get wlanRxDiversityEnabled value.

   Whether RX diversity is enabled.

   @param[in] service Instance of MIB service
   @param[out] value wlanRxDiversityEnabled value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanRxDiversityEnabled(mib_service_t *service,
            int *value);

/**
   Set wlanRxDiversityEnabled value.

   Whether RX diversity is enabled.

   @param[in] service Instance of MIB service
   @param[in] value wlanRxDiversityEnabled value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanRxDiversityEnabled(mib_service_t *service,
            int value);

/**
   Get wlanRxDiversityCnt value.

   RX diversity counter.

   @param[in] service Instance of MIB service
   @param[out] value wlanRxDiversityCnt value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanRxDiversityCnt(mib_service_t *service,
            uint32_t *value);

/** Minimum wlanChannelProbingInterval value (units: milliseconds) */
#define MIB_wlanChannelProbingInterval_MIN 1

/** Maximum wlanChannelProbingInterval value (units: milliseconds) */
#define MIB_wlanChannelProbingInterval_MAX 1000

/**
   Get wlanChannelProbingInterval value (units: milliseconds).

   Channel load probing interval.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[out] value wlanChannelProbingInterval value (units: milliseconds)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanChannelProbingInterval(mib_service_t *service,
            int32_t ifIndex,
            int32_t *value);

/**
   Set wlanChannelProbingInterval value (units: milliseconds).

   Channel load probing interval.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[in] value wlanChannelProbingInterval value (units: milliseconds)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanChannelProbingInterval(mib_service_t *service,
            int32_t ifIndex,
            int32_t value);

/** Minimum wlanChannelLoadThreshold value (units: dBm) */
#define MIB_wlanChannelLoadThreshold_MIN (-95)

/** Maximum wlanChannelLoadThreshold value (units: dBm) */
#define MIB_wlanChannelLoadThreshold_MAX (-35)

/**
   Get wlanChannelLoadThreshold value (units: dBm).

   Threshold of received signal strength above which the channel will
   be considered busy.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[out] value wlanChannelLoadThreshold value (units: dBm)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanChannelLoadThreshold(mib_service_t *service,
            int32_t ifIndex,
            int32_t *value);

/**
   Set wlanChannelLoadThreshold value (units: dBm).

   Threshold of received signal strength above which the channel will
   be considered busy.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[in] value wlanChannelLoadThreshold value (units: dBm)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanChannelLoadThreshold(mib_service_t *service,
            int32_t ifIndex,
            int32_t value);

/** Minimum wlanChannelBusyRatio value (units: %) */
#define MIB_wlanChannelBusyRatio_MIN 0

/** Maximum wlanChannelBusyRatio value (units: %) */
#define MIB_wlanChannelBusyRatio_MAX 100

/**
   Get wlanChannelBusyRatio value (units: %).

   The percentage of time during which the channel was busy in the last
   probing interval.

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[out] value wlanChannelBusyRatio value (units: %)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanChannelBusyRatio(mib_service_t *service,
            int32_t ifIndex,
            int32_t *value);

/**
   Get wlanPhyHeaderErrCnt value.

   This counter shall be incremented for each error in PHY header

   @param[in] service Instance of MIB service
   @param[in] ifIndex ifIndex value
   @param[out] value wlanPhyHeaderErrCnt value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanPhyHeaderErrCnt(mib_service_t *service,
            int32_t ifIndex,
            uint32_t *value);

/**
   Get wlanDcocEnabled value.

   Whether periodic DCOC is enabled.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[out] value wlanDcocEnabled value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanDcocEnabled(mib_service_t *service,
            int32_t wlanRfIndex,
            int *value);

/**
   Set wlanDcocEnabled value.

   Whether periodic DCOC is enabled.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[in] value wlanDcocEnabled value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanDcocEnabled(mib_service_t *service,
            int32_t wlanRfIndex,
            int value);

/**
   Get wlanRssiLatestFrame value (units: dBm).

   RSSI of latest frame received at PHY.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[out] value wlanRssiLatestFrame value (units: dBm)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanRssiLatestFrame(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t *value);

/** Minimum wlanRficTemperature value (units: degrees Celsius) */
#define MIB_wlanRficTemperature_MIN (-200)

/** Maximum wlanRficTemperature value (units: degrees Celsius) */
#define MIB_wlanRficTemperature_MAX 200

/**
   Get wlanRficTemperature value (units: degrees Celsius).

   RFIC temperature.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[out] value wlanRficTemperature value (units: degrees Celsius)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanRficTemperature(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t *value);

/** Minimum wlanRcpiLatestFrame value (units: dBm) */
#define MIB_wlanRcpiLatestFrame_MIN 0

/** Maximum wlanRcpiLatestFrame value (units: dBm) */
#define MIB_wlanRcpiLatestFrame_MAX 255

/**
   Get wlanRcpiLatestFrame value (units: dBm).

   RCPI of latest frame received at PHY.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[out] value wlanRcpiLatestFrame value (units: dBm)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanRcpiLatestFrame(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t *value);

/**
   Get wlanAntennaStatus value.

   Current status of WLAN (DSRC) antenna.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[out] value wlanAntennaStatus value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanAntennaStatus(mib_service_t *service,
            int32_t wlanRfIndex,
            mib_AntennaStatus_t *value);

/** Minimum wlanFrequency value (units: MHz) */
#define MIB_wlanFrequency_MIN 740

/** Maximum wlanFrequency value (units: MHz) */
#define MIB_wlanFrequency_MAX 5920

/**
   Get wlanFrequency value (units: MHz).

   Current frequency.

   Should not be used when channel switching is enabled.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[out] value wlanFrequency value (units: MHz)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanFrequency(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t *value);

/**
   Set wlanFrequency value (units: MHz).

   Current frequency.

   Should not be used when channel switching is enabled.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[in] value wlanFrequency value (units: MHz)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanFrequency(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t value);

/** Enumeration of wlanDcocStatus values */
typedef enum {
  MIB_wlanDcocStatus_notStarted = 0,
  MIB_wlanDcocStatus_inProgress = 1,
  MIB_wlanDcocStatus_success = 2,
  MIB_wlanDcocStatus_failure = 3
} mib_wlanDcocStatus_t;

/**
   Get wlanDcocStatus value.

   Indicates status of DCOC (DC Offset Cancellation) process.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[out] value wlanDcocStatus value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanDcocStatus(mib_service_t *service,
            int32_t wlanRfIndex,
            mib_wlanDcocStatus_t *value);

/**
   Get wlanRfFrontEndConnected value.

   Whether an external RF front-end is connected.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[out] value wlanRfFrontEndConnected value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanRfFrontEndConnected(mib_service_t *service,
            int32_t wlanRfIndex,
            int *value);

/**
   Set wlanRfFrontEndConnected value.

   Whether an external RF front-end is connected.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[in] value wlanRfFrontEndConnected value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanRfFrontEndConnected(mib_service_t *service,
            int32_t wlanRfIndex,
            int value);

/**
   Get wlanRfEnabled value.

   Whether RF interface is enabled. Once an interface has been
   disabled, it can only be re-enabled by rebooting the unit.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[out] value wlanRfEnabled value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanRfEnabled(mib_service_t *service,
            int32_t wlanRfIndex,
            int *value);

/**
   Set wlanRfEnabled value.

   Whether RF interface is enabled. Once an interface has been
   disabled, it can only be re-enabled by rebooting the unit.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[in] value wlanRfEnabled value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanRfEnabled(mib_service_t *service,
            int32_t wlanRfIndex,
            int value);

/** Minimum wlanRfFrontEndOffset value (units: dBm) */
#define MIB_wlanRfFrontEndOffset_MIN 0

/** Maximum wlanRfFrontEndOffset value (units: dBm) */
#define MIB_wlanRfFrontEndOffset_MAX 30

/**
   Get wlanRfFrontEndOffset value (units: dBm).

   Attenuation of the first transmitted frame's output power.
   Relevant only when a RF front-end is used.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[out] value wlanRfFrontEndOffset value (units: dBm)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanRfFrontEndOffset(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t *value);

/**
   Set wlanRfFrontEndOffset value (units: dBm).

   Attenuation of the first transmitted frame's output power.
   Relevant only when a RF front-end is used.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[in] value wlanRfFrontEndOffset value (units: dBm)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanRfFrontEndOffset(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t value);

/** Enumeration of wlanPhyOFDMChannelWidth values */
typedef enum {
  MIB_wlanPhyOFDMChannelWidth_width10MHz = 1,
  MIB_wlanPhyOFDMChannelWidth_width20MHz = 2
} mib_wlanPhyOFDMChannelWidth_t;

/**
   Get wlanPhyOFDMChannelWidth value.

   Current PHY OFDM channel width.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[out] value wlanPhyOFDMChannelWidth value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanPhyOFDMChannelWidth(mib_service_t *service,
            int32_t wlanRfIndex,
            mib_wlanPhyOFDMChannelWidth_t *value);

/**
   Set wlanPhyOFDMChannelWidth value.

   Current PHY OFDM channel width.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[in] value wlanPhyOFDMChannelWidth value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanPhyOFDMChannelWidth(mib_service_t *service,
            int32_t wlanRfIndex,
            mib_wlanPhyOFDMChannelWidth_t value);

/** Minimum wlanPresetFrequency0 value (units: MHz) */
#define MIB_wlanPresetFrequency0_MIN 5180

/** Maximum wlanPresetFrequency0 value (units: MHz) */
#define MIB_wlanPresetFrequency0_MAX 5930

/**
   @deprecated
   Get wlanPresetFrequency0 value (units: MHz).

   Preset frequency 0.

   Should not be used when channel switching is enabled.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[out] value wlanPresetFrequency0 value (units: MHz)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanPresetFrequency0(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t *value);

/**
   @deprecated
   Set wlanPresetFrequency0 value (units: MHz).

   Preset frequency 0.

   Should not be used when channel switching is enabled.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[in] value wlanPresetFrequency0 value (units: MHz)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanPresetFrequency0(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t value);

/** Minimum wlanPresetFrequency1 value (units: MHz) */
#define MIB_wlanPresetFrequency1_MIN 5180

/** Maximum wlanPresetFrequency1 value (units: MHz) */
#define MIB_wlanPresetFrequency1_MAX 5930

/**
   @deprecated
   Get wlanPresetFrequency1 value (units: MHz).

   Preset frequency 1.

   Should not be used when channel switching is enabled.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[out] value wlanPresetFrequency1 value (units: MHz)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanPresetFrequency1(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t *value);

/**
   @deprecated
   Set wlanPresetFrequency1 value (units: MHz).

   Preset frequency 1.

   Should not be used when channel switching is enabled.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[in] value wlanPresetFrequency1 value (units: MHz)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanPresetFrequency1(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t value);

/** Enumeration of wlanRfTestMode values */
typedef enum {
  MIB_wlanRfTestMode_off = 0,
  MIB_wlanRfTestMode_random = 1
} mib_wlanRfTestMode_t;

/**
   Get wlanRfTestMode value.

   RF interface test mode.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[out] value wlanRfTestMode value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanRfTestMode(mib_service_t *service,
            int32_t wlanRfIndex,
            mib_wlanRfTestMode_t *value);

/**
   Set wlanRfTestMode value.

   RF interface test mode.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[in] value wlanRfTestMode value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanRfTestMode(mib_service_t *service,
            int32_t wlanRfIndex,
            mib_wlanRfTestMode_t value);

/** Minimum wlanTxIqImbalanceAmplitude value (units: 0.1 dB) */
#define MIB_wlanTxIqImbalanceAmplitude_MIN (-60)

/** Maximum wlanTxIqImbalanceAmplitude value (units: 0.1 dB) */
#define MIB_wlanTxIqImbalanceAmplitude_MAX 60

/**
   Get wlanTxIqImbalanceAmplitude value (units: 0.1 dB).

   Transmitted signal I/Q imbalance amplitude correction factor.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[out] value wlanTxIqImbalanceAmplitude value (units: 0.1 dB)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanTxIqImbalanceAmplitude(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t *value);

/**
   Set wlanTxIqImbalanceAmplitude value (units: 0.1 dB).

   Transmitted signal I/Q imbalance amplitude correction factor.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[in] value wlanTxIqImbalanceAmplitude value (units: 0.1 dB)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanTxIqImbalanceAmplitude(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t value);

/** Minimum wlanTxIqImbalancePhase value (units: 0.1 degree) */
#define MIB_wlanTxIqImbalancePhase_MIN (-100)

/** Maximum wlanTxIqImbalancePhase value (units: 0.1 degree) */
#define MIB_wlanTxIqImbalancePhase_MAX 100

/**
   Get wlanTxIqImbalancePhase value (units: 0.1 degree).

   Transmitted signal I/Q imbalance phase correction factor.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[out] value wlanTxIqImbalancePhase value (units: 0.1 degree)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanTxIqImbalancePhase(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t *value);

/**
   Set wlanTxIqImbalancePhase value (units: 0.1 degree).

   Transmitted signal I/Q imbalance phase correction factor.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[in] value wlanTxIqImbalancePhase value (units: 0.1 degree)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanTxIqImbalancePhase(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t value);

/** Minimum wlanPantLutIndex value */
#define MIB_wlanPantLutIndex_MIN 0

/** Maximum wlanPantLutIndex value */
#define MIB_wlanPantLutIndex_MAX 4

/**
   Get wlanPantLutIndex value.

   Which power antenna LUT entry is used.

   Index 0 denotes the defaut power antenna LUT. Index 1-4 denotes one
   of four power antenna LUTs defined at wlanPantLutDbm8Table.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[out] value wlanPantLutIndex value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanPantLutIndex(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t *value);

/**
   Set wlanPantLutIndex value.

   Which power antenna LUT entry is used.

   Index 0 denotes the defaut power antenna LUT. Index 1-4 denotes one
   of four power antenna LUTs defined at wlanPantLutDbm8Table.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[in] value wlanPantLutIndex value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanPantLutIndex(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t value);

/**
   Get wlanTssiDetectorReading value.

   ADC TSSI feedback detector reading.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[out] value wlanTssiDetectorReading value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanTssiDetectorReading(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t *value);

/**
   Get wlanRfCalibrationRequired value.

   Whether calibration is required on next system boot.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[out] value wlanRfCalibrationRequired value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanRfCalibrationRequired(mib_service_t *service,
            int32_t wlanRfIndex,
            int *value);

/**
   Set wlanRfCalibrationRequired value.

   Whether calibration is required on next system boot.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[in] value wlanRfCalibrationRequired value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanRfCalibrationRequired(mib_service_t *service,
            int32_t wlanRfIndex,
            int value);

/**
   Get wlanTssiInterval value (units: sec).

   TSSI sampling interval.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[out] value wlanTssiInterval value (units: sec)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanTssiInterval(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t *value);

/**
   Set wlanTssiInterval value (units: sec).

   TSSI sampling interval.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[in] value wlanTssiInterval value (units: sec)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanTssiInterval(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t value);

/** Minimum wlanRxSampleGainLow value (units: 0.1 dB) */
#define MIB_wlanRxSampleGainLow_MIN (-1280)

/** Maximum wlanRxSampleGainLow value (units: 0.1 dB) */
#define MIB_wlanRxSampleGainLow_MAX 1270

/**
   Get wlanRxSampleGainLow value (units: 0.1 dB).

   Low-range input power gain correction factor.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[out] value wlanRxSampleGainLow value (units: 0.1 dB)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanRxSampleGainLow(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t *value);

/**
   Set wlanRxSampleGainLow value (units: 0.1 dB).

   Low-range input power gain correction factor.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[in] value wlanRxSampleGainLow value (units: 0.1 dB)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanRxSampleGainLow(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t value);

/** Minimum wlanRxSampleGainMid value (units: 0.1 dB) */
#define MIB_wlanRxSampleGainMid_MIN (-1280)

/** Maximum wlanRxSampleGainMid value (units: 0.1 dB) */
#define MIB_wlanRxSampleGainMid_MAX 1270

/**
   Get wlanRxSampleGainMid value (units: 0.1 dB).

   Mid-range input power gain correction factor.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[out] value wlanRxSampleGainMid value (units: 0.1 dB)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanRxSampleGainMid(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t *value);

/**
   Set wlanRxSampleGainMid value (units: 0.1 dB).

   Mid-range input power gain correction factor.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[in] value wlanRxSampleGainMid value (units: 0.1 dB)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanRxSampleGainMid(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t value);

/** Minimum wlanRxSampleGainHigh value (units: 0.1 dB) */
#define MIB_wlanRxSampleGainHigh_MIN (-1280)

/** Maximum wlanRxSampleGainHigh value (units: 0.1 dB) */
#define MIB_wlanRxSampleGainHigh_MAX 1270

/**
   Get wlanRxSampleGainHigh value (units: 0.1 dB).

   High-range input power gain correction factor.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[out] value wlanRxSampleGainHigh value (units: 0.1 dB)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanRxSampleGainHigh(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t *value);

/**
   Set wlanRxSampleGainHigh value (units: 0.1 dB).

   High-range input power gain correction factor.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[in] value wlanRxSampleGainHigh value (units: 0.1 dB)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanRxSampleGainHigh(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t value);

/** Minimum wlanGrfiSignalDelayResolution value (units: 0.1 usec) */
#define MIB_wlanGrfiSignalDelayResolution_MIN 0

/** Maximum wlanGrfiSignalDelayResolution value (units: 0.1 usec) */
#define MIB_wlanGrfiSignalDelayResolution_MAX 256

/**
   Get wlanGrfiSignalDelayResolution value (units: 0.1 usec).

   Timing resolution of the delay between transmitted packets to PA
   and T/R RF switch.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[out] value wlanGrfiSignalDelayResolution value (units: 0.1 usec)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanGrfiSignalDelayResolution(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t *value);

/**
   Set wlanGrfiSignalDelayResolution value (units: 0.1 usec).

   Timing resolution of the delay between transmitted packets to PA
   and T/R RF switch.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[in] value wlanGrfiSignalDelayResolution value (units: 0.1 usec)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanGrfiSignalDelayResolution(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t value);

/** Minimum wlanRxIqImbalanceAmplitude value (units: 0.1 dB) */
#define MIB_wlanRxIqImbalanceAmplitude_MIN (-60)

/** Maximum wlanRxIqImbalanceAmplitude value (units: 0.1 dB) */
#define MIB_wlanRxIqImbalanceAmplitude_MAX 60

/**
   Get wlanRxIqImbalanceAmplitude value (units: 0.1 dB).

   Received signal I/Q imbalance amplitude correction factor.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[out] value wlanRxIqImbalanceAmplitude value (units: 0.1 dB)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanRxIqImbalanceAmplitude(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t *value);

/**
   Set wlanRxIqImbalanceAmplitude value (units: 0.1 dB).

   Received signal I/Q imbalance amplitude correction factor.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[in] value wlanRxIqImbalanceAmplitude value (units: 0.1 dB)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanRxIqImbalanceAmplitude(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t value);

/** Minimum wlanRxIqImbalancePhase value (units: 0.1 degree) */
#define MIB_wlanRxIqImbalancePhase_MIN (-100)

/** Maximum wlanRxIqImbalancePhase value (units: 0.1 degree) */
#define MIB_wlanRxIqImbalancePhase_MAX 100

/**
   Get wlanRxIqImbalancePhase value (units: 0.1 degree).

   Received signal I/Q imbalance phase correction factor.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[out] value wlanRxIqImbalancePhase value (units: 0.1 degree)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanRxIqImbalancePhase(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t *value);

/**
   Set wlanRxIqImbalancePhase value (units: 0.1 degree).

   Received signal I/Q imbalance phase correction factor.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[in] value wlanRxIqImbalancePhase value (units: 0.1 degree)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanRxIqImbalancePhase(mib_service_t *service,
            int32_t wlanRfIndex,
            int32_t value);

/**
   @deprecated
   Get wlanPantLut array.

   Lookup table (LUT) for conversion of power detector output
   into 1 dBm.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[out] value wlanPantLut array
   @param[in,out] size Maximum (in) and actual (out) size of wlanPantLut array

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanPantLut(mib_service_t *service,
            int32_t wlanRfIndex,
            char *value, size_t *size);

/**
   @deprecated
   Set wlanPantLut array.

   Lookup table (LUT) for conversion of power detector output
   into 1 dBm.

   @param[in] service Instance of MIB service
   @param[in] wlanRfIndex wlanRfIndex value
   @param[in] value wlanPantLut array
   @param[in] size Size of wlanPantLut array

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanPantLut(mib_service_t *service,
            int32_t wlanRfIndex,
            const char *value, size_t size);

/** Minimum wlanLoLeakage value */
#define MIB_wlanLoLeakage_MIN 0

/** Maximum wlanLoLeakage value */
#define MIB_wlanLoLeakage_MAX 65535

/**
   Get wlanLoLeakage value.

   LO leakage cancellation per gain.

   @param[in] service Instance of MIB service
   @param[in] wlanLoLeakageIndex wlanLoLeakageIndex value
   @param[out] value wlanLoLeakage value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanLoLeakage(mib_service_t *service,
            int32_t wlanLoLeakageIndex,
            int32_t *value);

/**
   Set wlanLoLeakage value.

   LO leakage cancellation per gain.

   @param[in] service Instance of MIB service
   @param[in] wlanLoLeakageIndex wlanLoLeakageIndex value
   @param[in] value wlanLoLeakage value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanLoLeakage(mib_service_t *service,
            int32_t wlanLoLeakageIndex,
            int32_t value);

/** Minimum wlanPantLutDbm8 value */
#define MIB_wlanPantLutDbm8_MIN (-240)

/** Maximum wlanPantLutDbm8 value */
#define MIB_wlanPantLutDbm8_MAX 320

/**
   Get wlanPantLutDbm8 value.

   Conversion factor of power detector output into 1/8 dBm LUT.

   @param[in] service Instance of MIB service
   @param[in] wlanPantLutDbm8Index wlanPantLutDbm8Index value
   @param[out] value wlanPantLutDbm8 value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_wlanPantLutDbm8(mib_service_t *service,
            int32_t wlanPantLutDbm8Index,
            int32_t *value);

/**
   Set wlanPantLutDbm8 value.

   Conversion factor of power detector output into 1/8 dBm LUT.

   @param[in] service Instance of MIB service
   @param[in] wlanPantLutDbm8Index wlanPantLutDbm8Index value
   @param[in] value wlanPantLutDbm8 value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_wlanPantLutDbm8(mib_service_t *service,
            int32_t wlanPantLutDbm8Index,
            int32_t value);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_MIBS_WLAN_MIB_H */
