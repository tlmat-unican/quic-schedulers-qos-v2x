/* Copyright (C) 2015-2016 Autotalks Ltd. */
#ifndef _ATLK_SENSOR_H
#define _ATLK_SENSOR_H

#include <atlk/sdk.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   Vehicle Sensors API
*/

/** Sensor physical units */
typedef enum {
  /** Units of meters per second */
  SENSOR_UNITS_MPS = 0,

  /** Units of radians per second */
  SENSOR_UNITS_RADPS = 1,

  /** Value indicating that units are N/A */
  SENSOR_UNITS_NA = 255

} sensor_units_t;

/** Sensor value */
typedef int32_t sensor_value_t;

/** Value indicating that a sensor value is N/A */
#define SENSOR_VALUE_NA INT32_MIN

/**
   Sensor value parameters.
   
   @note Sensor values which represent rotational movement (e.g. gyro) are
   expected to be clockwise-positive.
*/
typedef struct {
  /** Units of (scaled) sensor value */
  sensor_units_t units;

  /** Minimum (unscaled) sensor value */
  sensor_value_t min;
  
  /** Maximum (unscaled) sensor value */
  sensor_value_t max;

  /**
     Sensor value inverse scaling.
     
     Sensor value must be divided by @p inverse_scaling to be expressed
     in units of @p units.
  */
  uint16_t inverse_scaling;

} sensor_value_params_t;

/** Sensor value parameters default initializer */
#define SENSOR_VALUE_PARAMS_INIT {  \
  .units = SENSOR_UNITS_NA,         \
  .min = INT32_MIN,                 \
  .max = INT32_MAX,                 \
  .inverse_scaling = 1              \
}

/** Vehicle wheels speed */
typedef struct {
  /** Speed of rear left wheel */
  sensor_value_t rear_left;

  /** Speed of rear right wheel */
  sensor_value_t rear_right;

  /** Speed of front left wheel */
  sensor_value_t front_left;

  /** Speed of front right wheel */
  sensor_value_t front_right;

} sensor_wheels_speed_t;

/** Vehicle wheels speed default initializer */
#define SENSOR_WHEELS_SPEED_INIT {  \
  .rear_left = SENSOR_VALUE_NA,     \
  .rear_right = SENSOR_VALUE_NA,    \
  .front_left = SENSOR_VALUE_NA,    \
  .front_right = SENSOR_VALUE_NA    \
}

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_SENSOR_H */
