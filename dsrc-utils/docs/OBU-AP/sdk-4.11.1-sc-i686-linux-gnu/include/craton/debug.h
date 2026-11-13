/* Copyright (C) 2014-2015 Autotalks Ltd. */
#ifndef _CRATON_DEBUG_H
#define _CRATON_DEBUG_H

#include <craton/exception.h>
#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   CRATON debug API
*/

/**
   Print formatted message to debug console.

   @param[in] format printf() compatible format string
*/
void
debug_printf(const char *format, ...);

/**
   Print debug message describing thread state of calling function thread.

   Prints stack dump of calling function thread, along with other
   registers values.

   Debug message will be written to console.
*/
void
debug_thread_state_print(void);

/**
   Dump exception info to debug console.

   The dumped data is a pre-formatted text message containing
   information extracted from @p info structure.

   @param[in] info Exception information
*/
void
debug_exception_info_print(const exception_info_t *info);

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_DEBUG_H */
