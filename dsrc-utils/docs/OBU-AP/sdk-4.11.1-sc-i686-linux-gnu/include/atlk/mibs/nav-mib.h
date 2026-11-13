/* Copyright (C) 2013-2016 Autotalks Ltd. */
/* File generated automatically; do not edit. */
/* Source: AUTOTALKS-NAV-MIB revision 201605220000. */
#ifndef _ATLK_MIBS_NAV_MIB_H
#define _ATLK_MIBS_NAV_MIB_H

#include <atlk/mib_service.h>
#include <atlk/mibs/inet-address-mib.h>
#include <atlk/mibs/tc.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   AUTOTALKS-NAV-MIB access API.

   Navigation MIB.
*/

/** Minimum navGpsSatelliteCnt value */
#define MIB_navGpsSatelliteCnt_MIN 0

/** Maximum navGpsSatelliteCnt value */
#define MIB_navGpsSatelliteCnt_MAX 255

/**
   Get navGpsSatelliteCnt value.

   Number of GPS satellites in view.

   Currently this value includes both GPS and SBAS satellites (PRNs 1-64).

   @param[in] service Instance of MIB service
   @param[out] value navGpsSatelliteCnt value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_navGpsSatelliteCnt(mib_service_t *service,
            uint32_t *value);

/**
   Set navGpsSatelliteCnt value.

   Number of GPS satellites in view.

   Currently this value includes both GPS and SBAS satellites (PRNs 1-64).

   @param[in] service Instance of MIB service
   @param[in] value navGpsSatelliteCnt value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_navGpsSatelliteCnt(mib_service_t *service,
            uint32_t value);

/** Minimum navGlonassSatelliteCnt value */
#define MIB_navGlonassSatelliteCnt_MIN 0

/** Maximum navGlonassSatelliteCnt value */
#define MIB_navGlonassSatelliteCnt_MAX 255

/**
   Get navGlonassSatelliteCnt value.

   Number of GLONASS satellites in view.

   @param[in] service Instance of MIB service
   @param[out] value navGlonassSatelliteCnt value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_navGlonassSatelliteCnt(mib_service_t *service,
            uint32_t *value);

/**
   Set navGlonassSatelliteCnt value.

   Number of GLONASS satellites in view.

   @param[in] service Instance of MIB service
   @param[in] value navGlonassSatelliteCnt value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_navGlonassSatelliteCnt(mib_service_t *service,
            uint32_t value);

/**
   Get navGnssRxBytesCnt value.

   Count of bytes read via NMEA I/O.

   @param[in] service Instance of MIB service
   @param[out] value navGnssRxBytesCnt value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_navGnssRxBytesCnt(mib_service_t *service,
            uint32_t *value);

/**
   Get navGnssRxNmeaSentencesCnt value.

   Count of valid NMEA sentences read via NMEA I/O.

   @param[in] service Instance of MIB service
   @param[out] value navGnssRxNmeaSentencesCnt value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_navGnssRxNmeaSentencesCnt(mib_service_t *service,
            uint32_t *value);

/**
   Get navGnssTxBytesCnt value.

   Count of bytes written via NMEA I/O.

   @param[in] service Instance of MIB service
   @param[out] value navGnssTxBytesCnt value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_navGnssTxBytesCnt(mib_service_t *service,
            uint32_t *value);

/**
   Get navGnssTxNmeaSentencesCnt value.

   Count of valid NMEA sentences written via NMEA I/O.

   @param[in] service Instance of MIB service
   @param[out] value navGnssTxNmeaSentencesCnt value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_navGnssTxNmeaSentencesCnt(mib_service_t *service,
            uint32_t *value);

/**
   Get navGnssRxNmeaSentencesErrorCnt value.

   Count of invalid NMEA sentences dropped at NMEA I/O before being
   parsed.

   @param[in] service Instance of MIB service
   @param[out] value navGnssRxNmeaSentencesErrorCnt value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_navGnssRxNmeaSentencesErrorCnt(mib_service_t *service,
            uint32_t *value);

/**
   Get navGnssTxNmeaSentencesErrorCnt value.

   Count of invalid NMEA sentences dropped at NMEA I/O before being
   written.

   @param[in] service Instance of MIB service
   @param[out] value navGnssTxNmeaSentencesErrorCnt value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_navGnssTxNmeaSentencesErrorCnt(mib_service_t *service,
            uint32_t *value);

/**
   Get navGnssAntennaStatus value.

   Current status of GNSS antenna.

   @param[in] service Instance of MIB service
   @param[out] value navGnssAntennaStatus value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_navGnssAntennaStatus(mib_service_t *service,
            mib_AntennaStatus_t *value);

/**
   Get navGnssAntennaOffsetX value (units: cm).

   Antenna offset on axis X in centimeters, relative to vehicles
   position reference. Axis X is positive towards vehicles front.

   @param[in] service Instance of MIB service
   @param[out] value navGnssAntennaOffsetX value (units: cm)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_navGnssAntennaOffsetX(mib_service_t *service,
            int32_t *value);

/**
   Set navGnssAntennaOffsetX value (units: cm).

   Antenna offset on axis X in centimeters, relative to vehicles
   position reference. Axis X is positive towards vehicles front.

   @param[in] service Instance of MIB service
   @param[in] value navGnssAntennaOffsetX value (units: cm)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_navGnssAntennaOffsetX(mib_service_t *service,
            int32_t value);

/**
   Get navGnssAntennaOffsetY value (units: cm).

   Antenna offset on axis Y in centimeters, relative to vehicles
   position reference. Axis Y is positive towards vehicles right
   hand side.

   @param[in] service Instance of MIB service
   @param[out] value navGnssAntennaOffsetY value (units: cm)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_navGnssAntennaOffsetY(mib_service_t *service,
            int32_t *value);

/**
   Set navGnssAntennaOffsetY value (units: cm).

   Antenna offset on axis Y in centimeters, relative to vehicles
   position reference. Axis Y is positive towards vehicles right
   hand side.

   @param[in] service Instance of MIB service
   @param[in] value navGnssAntennaOffsetY value (units: cm)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_navGnssAntennaOffsetY(mib_service_t *service,
            int32_t value);

/**
   Get navGnssAntennaOffsetZ value (units: cm).

   Antenna offset on axis Z in centimeters, relative to vehicles
   position reference. Axis Z is positive towards ground (i.e. down).

   @param[in] service Instance of MIB service
   @param[out] value navGnssAntennaOffsetZ value (units: cm)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_navGnssAntennaOffsetZ(mib_service_t *service,
            int32_t *value);

/**
   Set navGnssAntennaOffsetZ value (units: cm).

   Antenna offset on axis Z in centimeters, relative to vehicles
   position reference. Axis Z is positive towards ground (i.e. down).

   @param[in] service Instance of MIB service
   @param[in] value navGnssAntennaOffsetZ value (units: cm)

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_navGnssAntennaOffsetZ(mib_service_t *service,
            int32_t value);

/**
   Get navFixAvailable value.

   Whether a navigation fix is available (via satellite navigation and/or
   another method).

   @param[in] service Instance of MIB service
   @param[out] value navFixAvailable value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_navFixAvailable(mib_service_t *service,
            int *value);

/**
   Set navFixAvailable value.

   Whether a navigation fix is available (via satellite navigation and/or
   another method).

   @param[in] service Instance of MIB service
   @param[in] value navFixAvailable value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_navFixAvailable(mib_service_t *service,
            int value);

/** Enumeration of navDataSource values */
typedef enum {
  MIB_navDataSource_gnss = 0,
  MIB_navDataSource_gpsd = 1,
  MIB_navDataSource_none = 2
} mib_navDataSource_t;

/**
   Get navDataSource value.

   Source of navigation data fix.

   @param[in] service Instance of MIB service
   @param[out] value navDataSource value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_navDataSource(mib_service_t *service,
            mib_navDataSource_t *value);

/**
   Set navDataSource value.

   Source of navigation data fix.

   @param[in] service Instance of MIB service
   @param[in] value navDataSource value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_navDataSource(mib_service_t *service,
            mib_navDataSource_t value);

/**
   Get navGpsdServerPort value.

   Port number used by GPSD server.

   @param[in] service Instance of MIB service
   @param[out] value navGpsdServerPort value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_navGpsdServerPort(mib_service_t *service,
            uint16_t *value);

/**
   Set navGpsdServerPort value.

   Port number used by GPSD server.

   @param[in] service Instance of MIB service
   @param[in] value navGpsdServerPort value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_navGpsdServerPort(mib_service_t *service,
            uint16_t value);

/**
   Get navGpsdServerIpAddressType value.

   Address type used by GPSD server.

   @param[in] service Instance of MIB service
   @param[out] value navGpsdServerIpAddressType value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_navGpsdServerIpAddressType(mib_service_t *service,
            mib_InetAddressType_t *value);

/**
   Get navGpsdServerIpAddressIPv4 value.

   IPv4 address used by GPSD server.

   @param[in] service Instance of MIB service
   @param[out] value navGpsdServerIpAddressIPv4 value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_navGpsdServerIpAddressIPv4(mib_service_t *service,
            uint32_t *value);

/**
   Set navGpsdServerIpAddressIPv4 value.

   IPv4 address used by GPSD server.

   @param[in] service Instance of MIB service
   @param[in] value navGpsdServerIpAddressIPv4 value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_navGpsdServerIpAddressIPv4(mib_service_t *service,
            uint32_t value);

/**
   Get navSysTimeUpdateEnabled value.

   Whether updating system time from navigation fix is enabled.

   @param[in] service Instance of MIB service
   @param[out] value navSysTimeUpdateEnabled value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_navSysTimeUpdateEnabled(mib_service_t *service,
            int *value);

/**
   Set navSysTimeUpdateEnabled value.

   Whether updating system time from navigation fix is enabled.

   @param[in] service Instance of MIB service
   @param[in] value navSysTimeUpdateEnabled value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_navSysTimeUpdateEnabled(mib_service_t *service,
            int value);

/**
   Get navSysTimePpsSyncEnabled value.

   Whether syncing system time with external 1-PPS is enabled.

   @param[in] service Instance of MIB service
   @param[out] value navSysTimePpsSyncEnabled value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_navSysTimePpsSyncEnabled(mib_service_t *service,
            int *value);

/**
   Set navSysTimePpsSyncEnabled value.

   Whether syncing system time with external 1-PPS is enabled.

   @param[in] service Instance of MIB service
   @param[in] value navSysTimePpsSyncEnabled value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_navSysTimePpsSyncEnabled(mib_service_t *service,
            int value);

/** Enumeration of navSysTimeStatus values */
typedef enum {
  MIB_navSysTimeStatus_notSet = 0,
  MIB_navSysTimeStatus_set = 1
} mib_navSysTimeStatus_t;

/**
   Get navSysTimeStatus value.

   System time status.

   @param[in] service Instance of MIB service
   @param[out] value navSysTimeStatus value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_navSysTimeStatus(mib_service_t *service,
            mib_navSysTimeStatus_t *value);

/**
   Set navSysTimeStatus value.

   System time status.

   @param[in] service Instance of MIB service
   @param[in] value navSysTimeStatus value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_navSysTimeStatus(mib_service_t *service,
            mib_navSysTimeStatus_t value);

/**
   Get navSysTimeLeapSeconds value.

   Net amount of UTC leap seconds, between 2004-01-01T00:00:00Z and
   current time, which shall be used in conversion between TAI and UTC
   times in set/gettimeofday function calls.

   @param[in] service Instance of MIB service
   @param[out] value navSysTimeLeapSeconds value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_navSysTimeLeapSeconds(mib_service_t *service,
            int32_t *value);

/**
   Set navSysTimeLeapSeconds value.

   Net amount of UTC leap seconds, between 2004-01-01T00:00:00Z and
   current time, which shall be used in conversion between TAI and UTC
   times in set/gettimeofday function calls.

   @param[in] service Instance of MIB service
   @param[in] value navSysTimeLeapSeconds value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_navSysTimeLeapSeconds(mib_service_t *service,
            int32_t value);

/**
   Get navSysTimePpsCnt value.

   This counter shall be incremented after each pulse of
   the pulse-per-second (PPS) signal.

   @param[in] service Instance of MIB service
   @param[out] value navSysTimePpsCnt value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_navSysTimePpsCnt(mib_service_t *service,
            uint32_t *value);

/** Enumeration of navSysTimeAccuracy values */
typedef enum {
  MIB_navSysTimeAccuracy_milliSec1 = 41,
  MIB_navSysTimeAccuracy_second1 = 47,
  MIB_navSysTimeAccuracy_second10 = 48,
  MIB_navSysTimeAccuracy_secondGreater10 = 49,
  MIB_navSysTimeAccuracy_notKnown = 254
} mib_navSysTimeAccuracy_t;

/**
   Get navSysTimeAccuracy value.

   System time accuracy.

   -- 41: The time is accurate to within 1 ms
   -- 48: The time is accurate to within 1 s
   -- 48: The time is accurate to within 10 s
   -- 49: The time is accurate to within > 10 s
   -- 254: Default indicating unknown

   @param[in] service Instance of MIB service
   @param[out] value navSysTimeAccuracy value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_navSysTimeAccuracy(mib_service_t *service,
            mib_navSysTimeAccuracy_t *value);

/**
   Set navSysTimeAccuracy value.

   System time accuracy.

   -- 41: The time is accurate to within 1 ms
   -- 48: The time is accurate to within 1 s
   -- 48: The time is accurate to within 10 s
   -- 49: The time is accurate to within > 10 s
   -- 254: Default indicating unknown

   @param[in] service Instance of MIB service
   @param[in] value navSysTimeAccuracy value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_navSysTimeAccuracy(mib_service_t *service,
            mib_navSysTimeAccuracy_t value);

/**
   Get navConfigSaveStatus value.

   Navigation MIB configuration save status.

   @param[in] service Instance of MIB service
   @param[out] value navConfigSaveStatus value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_navConfigSaveStatus(mib_service_t *service,
            mib_ConfigSaveStatus_t *value);

/**
   Set navConfigSaveStatus value.

   Navigation MIB configuration save status.

   @param[in] service Instance of MIB service
   @param[in] value navConfigSaveStatus value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_navConfigSaveStatus(mib_service_t *service,
            mib_ConfigSaveStatus_t value);

/**
   Get navGnssInputDeviceIndex value.

   Index of currently selected GNSS input device.

   @param[in] service Instance of MIB service
   @param[out] value navGnssInputDeviceIndex value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_get_navGnssInputDeviceIndex(mib_service_t *service,
            uint16_t *value);

/**
   Set navGnssInputDeviceIndex value.

   Index of currently selected GNSS input device.

   @param[in] service Instance of MIB service
   @param[in] value navGnssInputDeviceIndex value

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
mib_set_navGnssInputDeviceIndex(mib_service_t *service,
            uint16_t value);

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_MIBS_NAV_MIB_H */
