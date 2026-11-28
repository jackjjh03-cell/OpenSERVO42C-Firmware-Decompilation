#ifndef OS_TRACE_H
#define OS_TRACE_H
#include "controller.h"
#include "protocol.h"
/* 160 x 16-byte samples; bounded storage, no serial I/O in the control loop. */
#define OS_TRACE_CAPACITY 160u
enum { OS_TRACE_START=36, OS_TRACE_STOP=37, OS_TRACE_STATUS=38, OS_TRACE_READ=39 };
typedef struct {uint32_t tick;int32_t actual,target;int16_t demand;uint16_t load;} os_trace_sample;
typedef struct {os_trace_sample sample[OS_TRACE_CAPACITY];uint16_t count,requested,interval;uint32_t next;bool running,first,faulted,unqualified;} os_trace;
void os_trace_tick(os_trace *t,const os_controller *c,uint32_t now);
size_t os_trace_dispatch(os_trace *t,const uint8_t *in,size_t n,uint8_t *out);
#endif
