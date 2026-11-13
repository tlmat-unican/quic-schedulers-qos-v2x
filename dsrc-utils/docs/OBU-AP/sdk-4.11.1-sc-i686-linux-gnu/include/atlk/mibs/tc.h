/* Copyright (C) 2014-2015 Autotalks Ltd. */
#ifndef _ATLK_MIBS_TC_H
#define _ATLK_MIBS_TC_H

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   Mapping of types in AUTOTALKS-TC
*/

/** Configuration save status */
typedef enum  {
  /** Saved configuration is up-to-date */
  MIB_ConfigSaveStatus_upToDate = 0,

  /** Saved configuration is not up-to-date */
  MIB_ConfigSaveStatus_notUpToDate = 1,

  /** Configuration save operation is requested */
  MIB_ConfigSaveStatus_save = 2,

  /** Configuration save operation is in progress */
  MIB_ConfigSaveStatus_saveInProgress = 3,

  /** Latest save operation failed */
  MIB_ConfigSaveStatus_saveError = 4,

  /** Device doesn't support save operation */
  MIB_ConfigSaveStatus_notSupported = 5,

  MIB_ConfigSaveStatus_maxStatus = MIB_ConfigSaveStatus_notSupported,

} mib_ConfigSaveStatus_t;

/** Antenna status */
typedef enum  {
  /** Antenna status sensing is not supported */
  MIB_AntennaStatus_notSupported = 0,

  /** Antenna is connected */
  MIB_AntennaStatus_connected = 1,

  /** Antenna is not connected */
  MIB_AntennaStatus_notConnected = 2,

  /** Antenna has been electrically shorted */
  MIB_AntennaStatus_shorted = 3,

  /** An error occurred during antenna status sensing */
  MIB_AntennaStatus_error = 4,

} mib_AntennaStatus_t;

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_MIBS_TC_H */
