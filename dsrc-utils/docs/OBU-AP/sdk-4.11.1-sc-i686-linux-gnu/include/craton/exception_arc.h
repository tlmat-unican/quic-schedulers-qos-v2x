/* Copyright (C) 2015 Autotalks Ltd. */
#ifndef _CRATON_EXCEPTION_ARC_H
#define _CRATON_EXCEPTION_ARC_H

#include <stdint.h>

#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   CRATON ARC CPU exception API
*/

/**
   ARC exception registers
*/
typedef struct {
  long bta;
  long lp_start;
  long lp_end;
  long lp_count;
  long erstatus;
  long ret;
  long blink;
  long fp;
  long r26;
  long r25;
  long r24;
  long r23;
  long r22;
  long r21;
  long r20;
  long r19;
  long r18;
  long r17;
  long r16;
  long r15;
  long r14;
  long r13;
  long r12;
  long r11;
  long r10;
  long r9;
  long r8;
  long r7;
  long r6;
  long r5;
  long r4;
  long r3;
  long r2;
  long r1;
  long r0;
  long sp;
  long ecr;
  long aux_align_addr;
  long aux_align_size;

} exception_arc_regs_t;

/**
   ARC exception type
*/
typedef enum {
  /** Memory exception */
  EXC_ARC_TYPE_MEMORY = 0,

  /** Instruction exception */
  EXC_ARC_TYPE_INSTRUCTION = 1,

  /** Watchdog expiration exception */
  EXC_ARC_TYPE_WD_TIMER = 2,

  /** Abnormal exit - bad return code from ARC */
  EXC_ARC_TYPE_ABNORMAL_EXIT = 3

} exception_arc_type_t;

/**
   Maximum size of thread stack upon exception
*/
#define EXC_ARC_THREAD_STACK_MAX_SIZE 512

/**
   Maximum size of thread name
*/
#define EXC_ARC_THREAD_NAME_MAX_SIZE 128

/**
   ARC exception structure containing all necessary information
   upon exception.
 */
typedef struct {
  /** Exception mode flag. Set when ARC enters exeption mode. */
  uint32_t mode;

  /** Exception type */
  exception_arc_type_t type;

  /** Exception address */
  uint32_t address;

  /** SW watchdog counter upon exception */
  uint32_t watchdog_counter;

  /** Exception registers */
  exception_arc_regs_t regs;

  /** Pointer to thread structure upon exception */
  uint32_t thread_address;

  /** Thread name */
  char thread_name[EXC_ARC_THREAD_NAME_MAX_SIZE];

  /** Thread stack size */
  size_t thread_stack_size;

  /** Thread stack */
  uint8_t thread_stack[EXC_ARC_THREAD_STACK_MAX_SIZE];

} exception_arc_info_t;

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_EXCEPTION_ARC_H */
