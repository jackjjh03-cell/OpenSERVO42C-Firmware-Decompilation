#ifndef OS_STREAM_H
#define OS_STREAM_H
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#define OS_STREAM_CAPACITY 56u
#define OS_STREAM_TIMEOUT_MS 100u
typedef struct { uint8_t bytes[OS_STREAM_CAPACITY], used; uint32_t last_ms; } os_stream;
void os_stream_reset(os_stream *s);
/* Returns a complete frame length or zero. Frame remains valid until next feed. */
size_t os_stream_feed(os_stream *s,uint8_t byte,uint32_t now_ms);
#endif
