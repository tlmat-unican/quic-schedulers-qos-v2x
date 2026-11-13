/**************************************************************************/
/*                                                                        */
/*            Copyright (c) 1996-2013 by Express Logic Inc.               */
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
/*  Express Logic, Inc.                                                   */
/*  11423 West Bernardo Court               info@expresslogic.com         */
/*  San Diego, CA  92127                    http://www.expresslogic.com   */
/*                                                                        */
/*                                                                        */
/**************************************************************************/

/**************************************************************************/
/**************************************************************************/
/*                                                                        */
/*    ThreadX Component                                                   */
/*                                                                        */
/*    POSIX Compliancy Wrapper (POSIX)                                    */
/*                                                                        */
/**************************************************************************/
/**************************************************************************/

/**************************************************************************/
/*                                                                        */
/*  EKP DEFINITIONS                                        RELEASE        */
/*                                                                        */
/*    tx_posix.h                                          PORTABLE C      */
/*                                                           5.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Express Logic, Inc.                                                 */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the constants, structures, etc.needed to          */
/*    implement the Evacuation Kit for POSIX Users (POSIX)                */
/*                                                                        */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-12-2005     Stefaan Kiebooms         Initial Version 5.0           */
/*  12-12-2013     Scott Larson             Modified comment(s),          */
/*                                            resulting in version 5.1    */
/*                                                                        */ 
/**************************************************************************/

/**
   @file
   POSIX API

   Overview

   The Autotalks POSIX Suite supports many of the basic POSIX calls, with
   some limitations, and utilizes ThreadX® primitives underneath.

   Each POSIX call is documented,
   including information about supported/unsupported options, limitations,
   deviations, and suggestions on how to work-around any limitations.

   2. Usage

   The file tx_posix.h must be included in the application source where POSIX
   calls are required, replacing the file pthread.h if such was included.
   Since the POSIX compliancy wrapper does not cover the complete standard,
   not all prototypes are provided.

   3. Supported features

   Autotalks POSIX suite supports the following POSIX features.
   See limitation section for unsupported features.
    - Pthread
    - Pthread Mutex
    - Message queue
    - Semaphore
    - Condition Variables
    - Timer - limited support

  4. Limitations

  All POSIX calls, excluding thread_create, must be called for a POSIX context.

  Due to performance and architecture issues, this POSIX suite
  does not support all the POSIX calls. A summary of the POSIX Compliancy
  limitations is as follows:

    - Configuration
    - Initialization
    - Driver and I/O model might require porting of current drivers.
    - Multi-processor extensions are not supported
    - Unsupported calls (please see below)
    - Calls supported with certain limitations (please see list below)
    - Only the following the routines can be used as pthread cancellation
      points:
        - mq_send
        - mq_receive
        - nanosleep
        - sleep
        - pthread_cond_timedwait
        - pthread_cond_wait
        - pthread_join
        - sem_wait

  In addition,
  there are also certain limitations with respect to some services.
  See below, function details, for limitations.

  5. Limitations on POSIX instances

     - SEM_NSEMS_MAX             128  Simultaneous POSIX semaphores

     - SEM_NAME_MAX              16   Maximum length of name of semaphore

     - SEM_VALUE_MAX             128  Max value of semaphore while initialization

     - POSIX_MAX_QUEUES          32   Maximum number of simultaneous POSIX
                                    message queues supported

     - PATH_MAX                  10   Maximum length of name of a message queue

     - PTHREAD_THREADS_MAX       128  Define the maximum number of simultaneous
                                    POSIX Pthreads supported.

     - POSIX_MAX_MUTEX           128  Define the maximum number of simultaneous
                                    POSIX mutexes sported.

     - POSIX_DEFAULT_STACK_SIZE  4KB  Default POSIX thread stack.

   6. List of supported POSIX API

        - pthread_cancel
        - pthread_create
        - pthread_detach
        - pthread_equal
        - pthread_exit
        - pthread_getschedparam
        - pthread_join
        - pthread_once
        - pthread_self
        - pthread_setcancelstate
        - pthread_setcanceltype
        - pthread_setschedparam
        - pthread_testcancel
        - pthread_yield
        - pthread_attr_init
        - pthread_attr_destroy         
        - pthread_attr_getdetachstate  
        - pthread_attr_getinheritsched 
        - pthread_attr_getschedparam   
        - pthread_attr_getschedpolicy  
        - pthread_attr_getstack
        - pthread_attr_getstackaddr
        - pthread_attr_getstacksize
        - pthread_attr_setdetachstate  
        - pthread_attr_setinheritsched 
        - pthread_attr_setschedparam
        - pthread_attr_setschedpolicy  
        - pthread_attr_setstack
        - pthread_attr_setstackaddr
        - pthread_attr_setstacksize
        - sched_get_priority_max
        - sched_get_priority_min
        - sched_yield
        - sem_close
        - sem_destroy
        - sem_getvalue
        - sem_init
        - sem_open
        - sem_post
        - sem_trywait                  
        - sem_unlink                   
        - sem_wait
        - pthread_mutex_destroy
        - pthread_mutex_init
        - pthread_mutex_lock
        - pthread_mutex_timedlock      
        - pthread_mutex_trylock
        - pthread_mutex_unlock
        - pthread_mutexattr_destroy    
        - pthread_mutexattr_getprotocol
        - pthread_mutexattr_getpshared 
        - pthread_mutexattr_gettype    
        - pthread_mutexattr_init       
        - pthread_mutexattr_setprotocol
        - pthread_mutexattr_setpshared 
        - pthread_mutexattr_settype    
        - mq_open
        - mq_send
        - mq_receive
        - mq_unlink
        - mq_close
        - pthread_cond_broadcast
        - pthread_cond_destroy
        - pthread_cond_init
        - pthread_cond_signal
        - pthread_cond_timedwait       
        - pthread_cond_wait
        - nanosleep

   7. Following is a list of unsupported POSIX API
        - pthread_sigmask
        - pthread_kill
        - clock_get
        - clock_set
        - clock_getres
        
*/

#ifndef TX_POSIX
#define TX_POSIX

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sched.h>
#include <time.h>
#include <signal.h>

#include <tx_api.h>
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_SIGNALS 32
#define SIGRTMAX    31

typedef struct signal_info_struct
{

    /* This is a flag. If TRUE, this thread is being used as a signal handler. 
     * If FALSE, it is a regular thread. */
    UINT                            signal_handler;                         
    /* A positive value indicates the level of nested signal handling the POSIX 
     * thread is currently processing.  */
    UINT                            signal_nesting_depth;                   
    /* Bit map of signals pending.   */ 
    sigset_t                        signal_pending;                         
    /* Signal mask, bit blocks the signal until cleared. */ 
    sigset_t                        signal_mask;                           
    /* Saved ThreadX state of the POSIX thread, at the time of the 
     * first signal. */
    UINT                            saved_thread_state;                     
    /* Pointer to the thread associated with the signal. */ 
    struct  pthread_control_block  *base_thread_ptr;                        
    /* Pointer to the top (most recent) signal thread. */ 
    struct  pthread_control_block  *top_signal_thread;   
    /* Pointer to the next most recent signal thread. */ 
    struct  pthread_control_block  *next_signal_thread; 
    /* Array of signal handlers for this thread. */ 
    void                            (*signal_func[MAX_SIGNALS])(int);       
    /* ThreadX event flag group used for sigwait. */ 
    TX_EVENT_FLAGS_GROUP            signal_event_flags;
    
} signal_info;

/* Define internal POSIX routines.  */
void  internal_signal_dispatch(ULONG id);

/** 
    See below for function limitations

    Suspend by nanosleep() calls can not be awakened
    by signals, once in the suspension call will complete the 
    suspension period

    Sleep time is convert clock ticks rounding up to the closes clock tick 
    so that the thread will sleep no less than the specified time
   
*/ 
INT nanosleep(const struct timespec *req, struct timespec *rem);

/************************************************************************/
/* Macros to convert between semaphore, queue, scheduler                */
/************************************************************************/
#define  MAKE_TX_SEM(sem)             ((TX_SEMAPHORE *)sem)
#define  MAKE_POSIX_QUEUE(queue)      ((POSIX_MSG_QUEUE *)queue)
#define  MAKE_POSIX_SEM(sem)          ((sem_t *)sem)

/************************************************************************/
/* Define max values for message queue services                         */
/************************************************************************/
#define  MQ_MAXMSG                      128 /* MQ_MAXMSG 1024 (POSIX value). */
#define  MQ_MSGSIZE                     2048
#define  MQ_FLAGS                       0
#define  MQ_PRIO_MAX                    32  /* Maximum priority of message.  */

/************************************************************************/
/*                          Global Variables                            */
/************************************************************************/

#define posix_errno errno

/* Define the system configuration constants for the Evacuation Kit for 
   POSIX Users.This is where the number of system objects 
   (pthreads, message queues, semaphores etc.)are defined.              */

/************************************************************************/
/*               SYSTEM CONFIGURATION PARAMETERS                        */
/************************************************************************/

/* Define the maximum number of simultaneous POSIX semaphores 
    supported.  */
#define  SEM_NSEMS_MAX                  128

/* Define the maximum length of name of semaphore .  */
#define  SEM_NAME_MAX                   16

/* Max value of semaphore while initialization.  */
#define  SEM_VALUE_MAX                  128

/* Define the maximum number of simultaneous POSIX message queues supported. */
#define  POSIX_MAX_QUEUES               32

/* Define the maximum number of simultaneous POSIX pthreads supported.  */
#define  PTHREAD_THREADS_MAX            128

/* Define the maximum number of simultaneous POSIX mutexes supported.  */
#define  POSIX_MAX_MUTEX                128

/* Define size of default POSIX thread stack. */ 
#define POSIX_DEFAULT_STACK_SIZE    (1 << 12)

/* Define size of the POSIX heap memory segment.                        */
/* NOTE:  This region should be large enough to supply the memory       */
/*        for all pthread stacks, pthread control blocks in the system  */
#define  POSIX_HEAP_SIZE_IN_BYTES      (1 << 20)

/* Define number of CPU ticks per second */
#define  CPU_TICKS_PER_SECOND           TX_TICK_RATE
#define  NANOSECONDS_IN_CPU_TICK        (1000000000 / CPU_TICKS_PER_SECOND)
#define  MICROSECONDS_IN_CPU_TICK       (1000000 / CPU_TICKS_PER_SECOND)

/* Define queue control specific data definitions.  */
#define  TX_SEMAPHORE_ID                0x53454D41UL
#define  TX_QUEUE_ID                    0x51554555UL
#define  PX_QUEUE_ID                    0x51554555UL
#define  TX_MUTEX_ID                    0x4D555445UL

/************************************************************************/
/*             Misc. POSIX-related definitions .                        */
/************************************************************************/
#define  POSIX_STACK_PADDING            1024
#define  POSIX_SYSTEM_STACK_SIZE        1024
#define  POSIX_PTHREAD_STACK_SIZE       1024  

/************************************************************************/ 
/*               MISCELLANEOUS CONSTANTS                                */ 
/************************************************************************/ 
/* Requests/commands to SysMgr task.  */
#define   SYSMGR_DELETE_TASK            0

/* pthread name length */
#define   PTHREAD_NAME_LEN              4

#define   PTHREAD_CREATE_DETACHED       1
#define   PTHREAD_CREATE_JOINABLE       0 

/* scheduler related constants */
#define   SCHED_PRIO_MAX                31
#define   SCHED_PRIO_MIN                1

/* time slice value in ticks for round robin scheduler */
#define   SCHED_RR_TIME_SLICE           20

#define   PTHREAD_MUTEX_NORMAL          1
#define   PTHREAD_MUTEX_RECURSIVE       2
#define   PTHREAD_MUTEX_ERRORCHECK      3
#define   PTHREAD_MUTEX_DEFAULT         PTHREAD_MUTEX_RECURSIVE

#define   PTHREAD_PRIO_INHERIT          1

#define   PTHREAD_PROCESS_PRIVATE       1
#define   PTHREAD_PROCESS_SHARED        2

#define   PTHREAD_CANCEL_ENABLE         0           /* default */

#define   PTHREAD_CANCEL_DISABLE        1

#define   PTHREAD_CANCEL_DEFERRED       0          /* default */

#define   PTHREAD_CANCEL_ASYNCHRONOUS   1

#define   PTHREAD_INHERIT_SCHED         1

#define   PTHREAD_EXPLICIT_SCHED        0

#define   PTHREAD_ONCE_INIT             {0, 0, {0,NULL,0,0,NULL,0,NULL,NULL,0,NULL}}

enum pth_once_state {
  PTH_ONCE_INIT      = 0x0,
  PTH_ONCE_DONE      = 0x1,
  PTH_ONCE_STARTED   = 0x2,
  PTH_ONCE_CANCELLED = 0x3
};

/************************************************************************/ 
/*               ERROR CODES (those defined outside of POSIX)           */ 
/************************************************************************/ 
#ifdef   ERROR
#undef   ERROR
#define  ERROR                  -1
#else
#define  ERROR                  -1
#endif

#define NO_ERROR        0

#ifndef  OK
#define  OK                     0
#endif

#ifndef  FALSE
#define  FALSE                  0
#endif

#ifndef  TRUE
#define  TRUE                   1
#endif

#ifndef  NULL
#define  NULL                   0
#endif

/* these constants control internal working of the systemmanager thread */

#define   WORK_REQ_SIZE         TX_2_ULONG
#define   WORK_QUEUE_DEPTH      10

#define   SYSMGR_PRIORITY       0
#define   SYSMGR_THRESHOLD      0


/* STRUCTURES RELATED TO pthreads  */



typedef struct pthread_attr_obj
{
     ULONG                pthread_flags;
     INT                  detach_state; 
     INT                  inherit_sched;
     INT                  sched_policy;
     struct sched_param   sched_attr;
     VOID                *stack_address;
     ULONG                stack_size;
     INT                  inuse;
} pthread_attr_t; 

typedef  ULONG  pthread_t;

/* Define POSIX Pthread control block tructure.  */
typedef struct pthread_control_block
{
    /* This pthread's ThreadX TCB.  */ 
    TX_THREAD            thread_info;
    /* This pthread's unique identifier */
    pthread_t            pthreadID;
    /* To check if posix Pthread is in use.  */
    UINT                 in_use;
    /* All pthread attributes contained in the a pthread_attr_t object */
    ULONG                pthread_flags;  
    INT                  detach_state;   
    INT                  inherit_sched;
    INT                  sched_policy;
    struct sched_param   sched_attr;
    VOID                *stack_address;
    ULONG                stack_size;
    INT                  cancel_state;
    INT                  cancel_type;     
    /* Identifier of the target thread to which this pthread is joined */
    pthread_t            joined_to_pthreadID;   
    /* Identifier of the caller thread which has joined to this thread*/
    pthread_t            joined_by_pthreadID;
    /* To check if posix pthread is joined to any other pthread */
        UINT             is_joined_to;
    /* To check if posix Pthread is joined by any other pthread */
        UINT             is_joined_by;
    /* To check if posix Pthread is in detached state or not */
        UINT             is_detached;
    /* Value returned by the terminating thread which is joined to this thread */
    VOID                 *value_ptr;
    /* Define the original pthread priority.  */ 
    ULONG                orig_priority;
    /* Define the current pthread priority.  */ 
    ULONG                current_priority;
    /* Define the pthread's pre-emption threshold.  */ 
    ULONG                threshold;
    /* Define the pthread's timeslice.  */ 
    ULONG                time_slice;
    /* specify pthread start routine */
    VOID                 *(*start_routine)(VOID *); 
    /* specify argument for start up routine */
    ULONG                *entry_parameter;
    /* to hold pthread cancel request */
    UINT                 cancel_request;

    /* Signal information follows.  */
    signal_info          signals;

}POSIX_TCB;

typedef struct pthread_mutex_attr_obj
{
     INT                 type;
     INT                 protocol;
     INT                 pshared;
     INT                 in_use;
     
} pthread_mutexattr_t;

/* Define POSIX mutex structure.  */

typedef struct pthread_mutex_control_block
{
    /* This mutex's ThreadX Control block  */ 
    TX_MUTEX      mutex_info;   
    /* This mutex's attributes */
    INT           type;
    /* Is this Mutex object is in use?  */
    INT           in_use;

} pthread_mutex_t;


/*     STRUCTURES RELATED TO POSIX MESSAGE QUEUE   */
struct mq_attr
{
    /* No. of maximum messages.  */
    ULONG         mq_maxmsg;
    /* Size of the message.  */
    ULONG         mq_msgsize;
    /* Flags are ignored as these are passed separately in open().  */
    ULONG         mq_flags;
}; 

/* Define POSIX message queue structure.  */
typedef struct msg_que
{
    /* Define ThreadX queue.  */
    TX_QUEUE                      queue;
    /* To check if posix queue is in use.  */
    UINT                          in_use;
    /* To check if queue is unlinked.  */
    UINT                          unlink_flag;
    /* Name of queue.  */
    CHAR                        * name;
    /* Attribute of queue.  */
    struct mq_attr                q_attr;
    /* To check no. of times queue is opened.  */
    UINT                          open_count;
    /* Address for variable length message.  */
    VOID                        * storage;
    /* Byte pool for variable length message.  */
    TX_BYTE_POOL                  vq_message_area;
    /* POSIX queue ID.  */
    ULONG                         px_queue_id;

}POSIX_MSG_QUEUE;

/* Define Queue Descriptor.  */
typedef struct mq_des
{
    /* Queue FLAGS.  */ 
    ULONG                         f_flag;
    /* message Queue structure.  */
    POSIX_MSG_QUEUE             * f_data;
    
} *mqd_t;


/* STRUCTURES RELATED TO POSIX SEMAPHORES  */

typedef struct POSIX_SEMAPHORE_STRUCT         
{
    /* ThreadX semaphore.  */
    TX_SEMAPHORE                  sem;
    /* To check if semaphore is in use.  */
    UINT                          in_use;
    /* semaphore identifier  */
    ULONG                         psemId;
    /* number of attachments  */
    ULONG                         refCnt;        /* previously it was int  */
     /* name of semaphore  */
    char                        * sem_name;
    /* Open Count.  */
    ULONG                         count;
    /* For unlink flag.  */
    ULONG                         unlink_flag;

} sem_t;

typedef sem_t             *SEM_ID;         

/* this constant needs sem_t deceleration */
/* No successful return from sem_open()  */
#define  SEM_FAILED ((sem_t *) 0)

typedef struct pthread_cond_obj
{
    /* This pthread condition variable's internal counting Semaphore  */ 
    TX_SEMAPHORE        cond_semaphore;
     
    INT                 type;
    INT                 in_use;
     
} pthread_cond_t;

typedef struct pthread_condattr_obj
{
/*     INT                 type; */
     INT                 in_use;
     
} pthread_condattr_t;

typedef struct  pthread_once_obj
{
  UINT          state;
  ULONG          flags;
  TX_EVENT_FLAGS_GROUP   event;
}pthread_once_t;

/* Define POSIX API function prototypes.  */ 

/**
    See below for function limitations

    -# In POSIX : If more than one mq_send() is blocked on a queue and
       space becomes available in that queue, the message with the highest
       priority will be unblocked. THIS FEATURE IS NOT IMPLEMENTED.
    
    -# If a message is sent (or received) to a queue with out opening the named 
       queue, in such a  case mqdes (message queue descriptor) pointer is 
       invalid and may result in erratic behavior.
 
*/ 
INT                   mq_send(mqd_t mqdes, const char * msg_ptr,
                                size_t msg_len,ULONG msg_prio ); 

/**
    See below for function limitations

    If a receive (or send) message from queue with out it being opened, 
    erratic behavior may ensue.
    
*/ 
ssize_t               mq_receive(mqd_t mqdes, VOID *pMsg, size_t msgLen,
                                   ULONG *pMsgPrio );
INT                   mq_unlink(const char * mqName);
INT                   mq_close(mqd_t mqdes);

/**
    See below for function limitations

     -# The value of mode (mode_t) has no effect in this implementation.
     -# If pAttr is NULL, the message queue is created with
        implementation-defined default message queue attributes.
        The default message queue attributes selected are :
    
           - MQ_MAXMSG 128             [MQ_MAXMSG 1024 (POSIX value)]
           - MQ_MSGSIZE 512            [MQ_MSGSIZE 4096 (POSIX value)]
           - MQ_FLAGS 0
    
        This is due to limitation of size of posix_region0_byte_pool (64KB ).
*/
mqd_t                 mq_open(const CHAR * mqName, ULONG oflags,...);

/** 
    See below for function limitations

    -# If operation is done before creating or opening (sem_open()) the named 
       semaphore, erratic behavior may result.
    -# This routine does not deallocate any system resources.

*/
INT                   sem_close(sem_t  * sem);

/** 
    See below for function limitations

    If operation is done before creating or opening (sem_open()) the named 
    semaphore, erratic behavior may result.

*/
INT                   sem_getvalue(sem_t * sem,ULONG * sval);
sem_t                *sem_open(const char * name, ULONG oflag, ...);

/** 
    See below for function limitations

    If operation is done before creating or opening (sem_open()) the named 
    semaphore, erratic behavior may result.

*/
INT                   sem_post(sem_t * sem);

/** 
    See below for function limitations

    If operation is done before creating or opening (sem_open()) the named 
    semaphore, erratic behavior may result.

*/
INT                   sem_trywait(sem_t * sem);

/** 
    See below for function limitations

    -# If operation is done before creating or opening (sem_open()) the named 
       semaphore, erratic behavior may result.
    -# EDEADLKA :->[ This is a return value when deadlock condition is detected; 
                     i.e., two separate processes are waiting for an available
                     resource to be released via a semaphore "held" by the other 
                     process.] 
                    This is not implemented.

    -# EINTR    :->[ This is a return value when sem_wait() was interrupted by 
                     a signal.]
                   This is not implemented.

*/
INT                   sem_unlink(const char * name);

/** 
    See below for function limitations

    If operation is done before creating or opening (sem_open()) the named 
    semaphore, erratic behavior may result.

*/
INT                   sem_wait( sem_t * sem );
INT                   sem_init(sem_t *sem , INT pshared, UINT value);
INT                   sem_destroy(sem_t *sem);

INT                   pthread_create (pthread_t *thread,  pthread_attr_t *attr, 
                                      VOID *(*start_routine)(VOID*),VOID *arg);

/** 
    See below for function limitations

    Call to function does not have any effect on system.
*/
INT                   pthread_detach(pthread_t thread);
INT                   pthread_join(pthread_t thread, VOID **value_ptr);
INT                   pthread_equal(pthread_t thread1, pthread_t thread2);
VOID                  pthread_exit(VOID *value_ptr);
pthread_t             pthread_self(VOID);
INT                   pthread_attr_destroy(pthread_attr_t *attr);
INT                   pthread_attr_getdetachstate( pthread_attr_t *attr,INT *detachstate);

/** 
    See below for function limitations

    Setting detach has no effect on system.
*/
INT                   pthread_attr_setdetachstate(pthread_attr_t *attr,INT detachstate);
INT                   pthread_attr_getinheritsched(pthread_attr_t *attr, INT *inheritsched);

/** 
    See below for function limitations

    PTHREAD_INHERIT_SCHED can be set only for threads created from 
    within a POSIX thread. 
*/
INT                   pthread_attr_setinheritsched(pthread_attr_t *attr, INT inheritsched);
INT                   pthread_attr_getschedparam(pthread_attr_t *attr,struct sched_param *param);
INT                   pthread_attr_setschedparam(pthread_attr_t *attr,struct sched_param *param);
INT                   pthread_attr_getschedpolicy(pthread_attr_t *attr, INT *policy);
INT                   pthread_attr_setschedpolicy(pthread_attr_t *attr, INT policy);
INT                   pthread_attr_init(pthread_attr_t *attr);
INT                   pthread_attr_getstackaddr( pthread_attr_t *attr,VOID **stackaddr);
INT                   pthread_attr_setstackaddr(pthread_attr_t *attr,VOID *stackaddr);
INT                   pthread_attr_getstacksize( pthread_attr_t *attr, size_t *stacksize);
INT                   pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
INT                   pthread_attr_getstack( pthread_attr_t *attr,VOID **stackaddr,
                                           size_t *stacksize);
INT                   pthread_attr_setstack( pthread_attr_t *attr,VOID *stackaddr,
                                            size_t stacksize);
INT                   pthread_mutexattr_gettype(pthread_mutexattr_t *attr, INT *type);

/** 
    See below for function limitations

    Mutex attribute is limited to the following protocol types 
    -# PTHREAD_MUTEX_RECURSIVE
*/
INT                   pthread_mutexattr_settype(pthread_mutexattr_t *attr, INT type);
INT                   pthread_mutexattr_destroy(pthread_mutexattr_t *attr);
INT                   pthread_mutexattr_init(pthread_mutexattr_t *attr);
INT                   pthread_mutex_destroy(pthread_mutex_t *mutex);
INT                   pthread_mutex_init(pthread_mutex_t *mutex, pthread_mutexattr_t *attr);
INT                   pthread_mutex_lock(pthread_mutex_t *mutex );
INT                   pthread_mutex_unlock(pthread_mutex_t *mutex );
INT                   pthread_mutex_trylock(pthread_mutex_t *mutex);
INT                   pthread_mutexattr_getprotocol( pthread_mutexattr_t *attr, INT *protocol);

/** 
    See below for function limitations

    Mutex attribute is limited to the following protocol types 
    -# PTHREAD_PRIO_INHERIT
*/
INT                   pthread_mutexattr_setprotocol(pthread_mutexattr_t *attr, INT protocol);
INT                   pthread_mutexattr_getpshared (pthread_mutexattr_t *attr, INT *pshared);
INT                   pthread_mutexattr_setpshared (pthread_mutexattr_t *attr, INT pshared);
INT                   pthread_mutex_timedlock(pthread_mutex_t *mutex, struct timespec *abs_timeout);
INT                   pthread_setcancelstate (INT state, INT *oldstate);
INT                   pthread_setcanceltype (INT type, INT *oldtype);

/**
    See below for function limitations

    When the pthread_cancel( ) function is called the target thread is canceled
    with immediate effect. (provided cancelability is enabled for the target 
    pthread)

    The cancellation processing in the target thread shall run asynchronously
    with respect to the ailing thread returning from pthread_cancel( ).
*/
INT                   pthread_cancel(pthread_t thread);
VOID                  pthread_yield(VOID);

/**
    See below for function limitations

    There is no provision if the init_routine contains a cancellation point.
*/
INT                   pthread_once (pthread_once_t * once_control, VOID (*init_routine) (VOID));
VOID                  pthread_testcancel(VOID);
INT                   pthread_setschedparam(pthread_t thread, INT policy, const struct sched_param *param);
INT                   pthread_getschedparam(pthread_t thread, INT *policy, struct sched_param *param);

INT                   pthread_cond_destroy(pthread_cond_t *cond);

/**
    See below for function limitations

    No attributes are supported for condition variable in this implementation.
*/
INT                   pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *attr);
INT                   pthread_cond_broadcast(pthread_cond_t *cond);
INT                   pthread_cond_signal(pthread_cond_t *cond);
INT                   pthread_cond_timedwait(pthread_cond_t *cond,pthread_mutex_t *mutex,
                                             const struct timespec *abstime);
INT                   pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);

/**
   Scheduler functions
 */
INT                   sched_get_priority_max(INT policy);
INT                   sched_get_priority_min(INT policy);
INT                   sched_yield(VOID);

/* static mutex initializer */

#define PTHREAD_MUTEX_INITIALIZER  {{TX_MUTEX_ID, "PMTX", 0, NULL, 0, 0, NULL, 0, NULL , NULL, 0, NULL, NULL}, PTHREAD_MUTEX_RECURSIVE, TX_TRUE}

/* static conditional variable initializer */
#define PTHREAD_COND_INITIALIZER  {{TX_SEMAPHORE_ID, "CSEM", 0, NULL, 0, NULL, NULL, NULL}, 0, TX_TRUE}

/** Error Codes for posix_error_handler */
enum  {
  Pthread_FlagsCreate_Error = 3333,
  Pthread_Create_Error,
  Pthread_EntryExitNotify_Error,
  Pthread_Resume_Error,
  Pthread_Kill_Error,
};

#ifdef __cplusplus
}
#endif

#endif      /* TX_POSIX  */
