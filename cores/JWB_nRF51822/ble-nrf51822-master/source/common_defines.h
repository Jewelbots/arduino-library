#ifndef __common_defines_h__
#define __common_defines_h__

#define APP_TIMER_PRESCALER 0 /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_MAX_TIMERS                                                   \
  12 /**< Maximum number of simultaneously created timers. */
#define APP_TIMER_OP_QUEUE_SIZE 12 /**< Size of timer operation queues. */

#define HTONL(n)                                                               \
  (((((unsigned long)(n)&0xFF)) << 24) |                                       \
   ((((unsigned long)(n)&0xFF00)) << 8) |                                      \
   ((((unsigned long)(n)&0xFF0000)) >> 8) |                                    \
   ((((unsigned long)(n)&0xFF000000)) >> 24))

#endif
