/**************************************************************************/
/*                                                                        */
/*            Copyright (c) 1996-2010 by Express Logic Inc.               */
/*                                                                        */
/*  This software is copyrighted by and is the sole property of Express   */
/*  Logic, Inc.  All rights, title, ownership, or other interests         */
/*  in the software remain the property of Express Logic, Inc.  This      */
/*  software may only be used in accordance with the corresponding        */
/*  license agreement.  Any unauthorized use, duplication, transmission,  */
/*  distribution, or disclosure of this software is expressly forbidden.  */
/*                                                                        */
/*  This Copyright notice may not be removed or modified without prior    */
/*  written consent of Express Logic, Inc.                                */
/*                                                                        */
/*  Express Logic, Inc. reserves the right to modify this software        */
/*  without notice.                                                       */
/*                                                                        */
/*  Express Logic, Inc.                     info@expresslogic.com         */
/*  11423 West Bernardo Court               http://www.expresslogic.com   */
/*  San Diego, CA  92127                                                  */
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** ThreadX Component                                                     */
/**                                                                       */
/**   Execution Profile Kit                                               */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

#ifndef TX_EXECUTION_PROFILE_H
#define TX_EXECUTION_PROFILE_H

#include "tx_port.h"

#ifdef TX_ENABLE_EXECUTION_PROFILING

/* The thread execution profile kit is designed to track thread execution time
   based on the hardware timer defined by TX_EXECUTION_TIME_SOURCE and
   TX_EXECUTION_MAX_TIME_SOURCE. When the thread's total time reaches
   the maximum value, it remains there until the time is reset to 0 via a call
   to tx_thread_execution_time_reset. */

/* Define APIs of the execution profile kit.  */
struct TX_THREAD_STRUCT;
VOID  _tx_execution_thread_enter(void);
VOID  _tx_execution_thread_exit(void);
VOID  _tx_execution_isr_enter(void);
VOID  _tx_execution_isr_exit(void);
UINT  _tx_execution_thread_time_reset(struct TX_THREAD_STRUCT *thread_ptr);
UINT  _tx_execution_thread_total_time_reset(void);
UINT  _tx_execution_isr_time_reset(void);
UINT  _tx_execution_idle_time_reset(void);
UINT  _tx_execution_thread_time_get(struct TX_THREAD_STRUCT *thread_ptr, EXECUTION_TIME *total_time);
UINT  _tx_execution_thread_total_time_get(EXECUTION_TIME *total_time);
UINT  _tx_execution_isr_time_get(EXECUTION_TIME *total_time);
UINT  _tx_execution_idle_time_get(EXECUTION_TIME *total_time);
ULONG _tx_execution_time_rate_get(void);

#endif
#endif
