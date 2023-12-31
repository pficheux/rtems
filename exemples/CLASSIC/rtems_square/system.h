/*
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 */
/*  updated for triple test, 20003/11/06, Erik Adli */


#include <inttypes.h>
#include <rtems.h>

/* functions */

rtems_task Init(
  rtems_task_argument argument
);

rtems_task Task_Absolute_Period(
  rtems_task_argument argument
);

rtems_task Task_Rate_Monotonic_Period(
  rtems_task_argument argument
);

rtems_task Task_Relative_Period(
  rtems_task_argument argument
);

/* global variables */

/*
 *  Keep the names and IDs in global variables so another task can use them.
 */ 
extern rtems_id   Task_id[ 2 ];         /* array of task ids */
extern rtems_name Task_name[ 2 ];       /* array of task names */



/* configuration information */

#include <bsp.h> /* for device driver prototypes */

#define CONFIGURE_APPLICATION_NEEDS_SIMPLE_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

// <GPIO>
#define CONFIGURE__MAXIMUM_FILE_DESCRIPTORS 10
#define CONFIGURE_UNLIMITED_OBJECTS
#define CONFIGURE_UNIFIED_WORK_AREAS
// </GPIO>

#define CONFIGURE_MICROSECONDS_PER_TICK     500   // NB: 10 and lower gives system failure for erc32 simulator

#define CONFIGURE_MAXIMUM_TASKS             10

#define CONFIGURE_EXTRA_TASK_STACKS         (6 * RTEMS_MINIMUM_STACK_SIZE)

// Needed for RM Mangager
#define CONFIGURE_MAXIMUM_PERIODS           1

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE


/* Needed for erc32 simulator.. */
/* ..for using "CPU_usage_Dump", since it uses printf("%f") if your processor has floating points) */
/* If you want to take away FP support (to avoid heavy context switch), you must rewrite CPU_usage_Dump instead */
#define CONFIGURE_INIT_TASK_ATTRIBUTES RTEMS_FLOATING_POINT


#include <rtems/confdefs.h>

/*
 *  Handy macros and static inline functions
 */

/*
 *  Macro to hide the ugliness of printing the time.
 */

#define print_time(_s1, _tb, _s2) \
  do { \
    printf( "%s%02" PRIu32 ":%02" PRIu32 ":%02" PRIu32 \
       "   %02" PRIu32 "/%02" PRIu32 "/%04" PRIu32 "%s", \
       _s1, (_tb)->hour, (_tb)->minute, (_tb)->second, \
       (_tb)->month, (_tb)->day, (_tb)->year, _s2 ); \
    fflush(stdout); \
  } while ( 0 )

/*
 *  Macro to print an task name that is composed of ASCII characters.
 *
 */

#define put_name( _name, _crlf ) \
  do { \
    uint32_t c0, c1, c2, c3; \
    \
    c0 = ((_name) >> 24) & 0xff; \
    c1 = ((_name) >> 16) & 0xff; \
    c2 = ((_name) >> 8) & 0xff; \
    c3 = (_name) & 0xff; \
    putchar( (char)c0 ); \
    if ( c1 ) putchar( (char)c1 ); \
    if ( c2 ) putchar( (char)c2 ); \
    if ( c3 ) putchar( (char)c3 ); \
    if ( (_crlf) ) \
      putchar( '\n' ); \
  } while (0)

/*
 *  This allows us to view the "Test_task" instantiations as a set
 *  of numbered tasks by eliminating the number of application
 *  tasks created.
 *
 *  In reality, this is too complex for the purposes of this
 *  example.  It would have been easier to pass a task argument. :)
 *  But it shows how rtems_id's can sometimes be used.
 */

#define task_number( tid ) \
  ( rtems_get_index( tid ) - \
     rtems_configuration_get_rtems_api_configuration()->number_of_initialization_tasks )

/* end of include file */
