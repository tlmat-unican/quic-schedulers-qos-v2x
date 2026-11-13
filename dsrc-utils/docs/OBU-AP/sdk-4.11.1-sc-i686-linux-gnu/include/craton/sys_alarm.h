/* Copyright (C) 2016 Autotalks Ltd. */
#ifndef _CRATON_SYS_ALARM_H
#define _CRATON_SYS_ALARM_H

#include <atlk/sdk.h>
#include <atlk/os.h>

#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/** System alarm gauges */
typedef struct {
  /** 
    Percent of non-idle CPU time in the last polling
    interval (5 seconds by default).
  */
  uint8_t cpu_utilization_percent;

  /** 
    Percent of allocated heap memory area, maximum achieved
    in the last polling interval (5 seconds by default).
  */
  uint8_t heap_utilization_percent;

} sys_alarm_gauges_t;

/** System alarm gauges default initializer */
#define SYS_ALARM_GAUGES_INIT {     \
  .cpu_utilization_percent = 100,   \
  .heap_utilization_percent = 100   \
}

/** System alarm configuration parameters */
typedef struct {
  /**
    System alarm thresholds.
     
    Valid values for ::sys_alarm_gauges_t.cpu_utilization_percent and
    ::sys_alarm_gauges_t.heap_utilization_percent is in the range [1..99].
  */
  sys_alarm_gauges_t alarm_thresholds;

  /** 
    Alarm handler.
    
    Handler is called at the end of a polling interval when configured
    thresholds are breached. Measured gague values of last polling interval
    are available at @p gauges.
  */
  void (*alarm_handler)(const sys_alarm_gauges_t *gauges);

  /** System alarm thread scheduling parameters */
  atlk_thread_sched_t sched_params;

} sys_alarm_config_t;

/** System alarm configuration parameters default initializer */
#define SYS_ALARM_CONFIG_INIT {                 \
  .alarm_thresholds = SYS_ALARM_GAUGES_INIT,    \
  .alarm_handler = NULL,                        \
  .sched_params = ATLK_THREAD_SCHED_INIT        \
}

/**
   Initialize system alarm.

   @param[in] config System alarm configuration parameters

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
sys_alarm_init(const sys_alarm_config_t *config);

/**
   Destroy previously initialized system alarm.

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
sys_alarm_destroy(void);

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_SYS_ALARM_H */
