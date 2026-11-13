/* Copyright (C) 2014-2015 Autotalks Ltd. */
#ifndef _CRATON_SYSLOG_H
#define _CRATON_SYSLOG_H

#include <atlk/sdk.h>

#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   CRATON system logger API
*/

/** Log trace level */
typedef enum {
  /** Error conditions */
  LOG_ERR = 3,

  /** Warning conditions */
  LOG_WARNING = 4,

  /** Normal but significant condition */
  LOG_NOTICE = 5,

  /** Informational messages */
  LOG_INFO = 6,

  /** Debug-level messages */
  LOG_DEBUG = 7

} syslog_level_t;

/** Syslog sink console flag */
#define SYSLOG_SINK_F_CONSOLE (1 << 1)
/** Syslog sink TCP flag */
#define SYSLOG_SINK_F_TCP     (1 << 2)
/** Syslog sink UDP flag */
#define SYSLOG_SINK_F_UDP     (1 << 3)

/**
   Generate a log message.

   @param[in] level Message level
   @param[in] format Message format
*/
void
syslog(syslog_level_t level,
       const char *format, ...)
  atlk_format_printf(2, 3);

/**
   Set Syslog trace level value.

   @param[in] level Syslog trace level value.

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
syslog_level_set(syslog_level_t level);

/**
   Get Syslog trace level value.

   @param[out] level Syslog trace level value.

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
syslog_level_get(syslog_level_t *level);

/**
   Set Syslog trace sink value.

   @param[in] sink_mask Syslog sink mask value.

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
syslog_sink_set(uint32_t sink_mask);

/**
   Get Syslog trace sink value.

   @param[out] sink_mask Syslog sink mask value.

   @retval ::ATLK_OK if succeeded
   @return Error code if failed
*/
atlk_rc_t atlk_must_check
syslog_sink_get(uint32_t *sink_mask);

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_SYSLOG_H */
