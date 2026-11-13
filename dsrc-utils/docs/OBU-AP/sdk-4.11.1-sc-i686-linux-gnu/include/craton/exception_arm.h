/* Copyright (C) 2015 Autotalks Ltd. */
#ifndef _CRATON_EXCEPTION_ARM_H
#define _CRATON_EXCEPTION_ARM_H

#include <stdint.h>

#include <craton/check.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   @file
   CRATON ARM CPU exception API
*/

/**
   ARM exception registers
*/
typedef struct {
  uint32_t r0;
  uint32_t r1;
  uint32_t r2;
  uint32_t r3;
  uint32_t r4;
  uint32_t r5;
  uint32_t r6;
  uint32_t r7;
  uint32_t r8;
  uint32_t r9;
  uint32_t r10;
  uint32_t fp;
  uint32_t ip;
  uint32_t sp;
  uint32_t lr;
  uint32_t pc;
  uint32_t cpsr;

} exception_arm_regs_t;

/**
   ARM exception fault operation
*/
typedef enum {
  /** Fault upon read operation */
  EXC_ARM_FAULT_OP_READ = 0,

  /** Fault upon write operation */
  EXC_ARM_FAULT_OP_WRITE = 1,

  /** Fault upon pre-fetch operation */
  EXC_ARM_FAULT_OP_FETCH = 2,

  /** Fault upon execution operation */
  EXC_ARM_FAULT_OP_EXECUTE = 3,

  /** Unavailable operation upon fault */
  EXC_ARM_FAULT_OP_NA = 4

} exception_arm_fault_operation_t;

/**
   ARM exception type
*/
typedef enum {
  /** Data exception */
  EXC_ARM_TYPE_DATA = 0,

  /** Prefetch exception */
  EXC_ARM_TYPE_PREFETCH = 1,

  /** Instruction exception */
  EXC_ARM_TYPE_INSTRUCTION = 2,

  /** Watchdog expiration exception */
  EXC_ARM_TYPE_WD_TIMER = 3,

  /** Unavailable exception type */
  EXC_ARM_TYPE_NA = 4

} exception_arm_type_t;

/**
   ARM exception fault reason
*/
typedef enum {
  /** Background MPU exception reason: access to undefined memory area in MPU */
  EXC_ARM_REASON_BACKGROUND = 0,

  /** Unaligned memory access exception reason */
  EXC_ARM_REASON_ALIGNMENT = 1,

  /** Debug exception reason when in debug mode */
  EXC_ARM_REASON_DEBUG_EVENT = 2,

  /** Synchronous external abort exception reason */
  EXC_ARM_REASON_SYNC_EXTERNAL = 3,

  /** Asynchronous external abort exception reason */
  EXC_ARM_REASON_ASYNC_EXTERNAL = 4,

  /** Permission exception reason */
  EXC_ARM_REASON_PERMISSION = 5,

  /** Asynchronous Parity/Error Correction Code (ECC) exception reason */
  EXC_ARM_REASON_ASYNC_ECC = 6,

  /** Synchronous Parity/Error Correction Code (ECC) exception reason */
  EXC_ARM_REASON_SYNC_ECC = 7,

  /** Abnormal exit - Caused by assert/abort/BUG... */
  EXC_ARM_REASON_ABNORMAL_EXIT = 8,

  /** Unavailable exception reason */
  EXC_ARM_REASON_NA = 9

} exception_arm_reason_t;

/**
   Maximum size of thread stack upon exception
*/
#define EXC_ARM_THREAD_STACK_MAX_SIZE 512

/**
   Maximum size of thread name
*/
#define EXC_ARM_THREAD_NAME_MAX_SIZE 128

/**
   ARM exception structure containing all necessary information
   upon exception.
 */
typedef struct {
  /** Operation upon exception */
  exception_arm_fault_operation_t operation;

  /** Exception type */
  exception_arm_type_t type;

  /** Exception reason */
  exception_arm_reason_t reason;

  /** Exception address */
  uint32_t address;

  /** Exception registers */
  exception_arm_regs_t arm_regs;

  /** Pointer to thread structure upon exception */
  uint32_t thread_address;

  /** Thread name */
  char thread_name[EXC_ARM_THREAD_NAME_MAX_SIZE];

  /** Thread stack size */
  size_t thread_stack_size;

  /** Thread stack */
  uint8_t thread_stack[EXC_ARM_THREAD_STACK_MAX_SIZE];

} exception_arm_info_t;

#ifdef __cplusplus
}
#endif

#endif /* _CRATON_EXCEPTION_ARM_H */
