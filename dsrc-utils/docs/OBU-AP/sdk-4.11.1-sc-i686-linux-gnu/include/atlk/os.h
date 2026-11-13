/* Copyright (C) 2014-2015 Autotalks Ltd. */
#ifndef _ATLK_OS_H
#define _ATLK_OS_H

#include <atlk/sdk.h>

#if (defined __linux__ && !defined __KERNEL__) || defined __QNXNTO__
#define ATLK_OS_POSIX 1
#endif

#if defined __THREADX__
#include <tx_port.h>
#elif defined ATLK_OS_POSIX
#include <pthread.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   Autotalks OS abstraction definitions
*/

/** Thread scheduling parameters */
typedef struct {
#if defined __THREADX__
  /** ThreadX scheduling priority */
  UINT priority;

  /**
     Time slice in ticks

     For FIFO scheduling use ::TX_NO_TIME_SLICE.
   */
  ULONG time_slice;

#elif defined ATLK_OS_POSIX
  /** Posix scheduling priority */
  int priority;
#endif

} atlk_thread_sched_t;

/** Thread scheduling parameters default initializer */
#if defined __THREADX__
#define ATLK_THREAD_SCHED_INIT {            \
  .priority = TX_MAX_PRIORITIES,            \
  .time_slice = 0                           \
}
#elif defined ATLK_OS_POSIX
#define ATLK_THREAD_SCHED_INIT {            \
  .priority = 0                             \
}
#endif

#ifdef __cplusplus
}
#endif

#endif /* _ATLK_OS_H */
