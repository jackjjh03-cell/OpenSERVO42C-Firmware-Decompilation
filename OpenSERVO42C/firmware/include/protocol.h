#ifndef OPEN_PROTOCOL_H
#define OPEN_PROTOCOL_H
#include <stddef.h>
#include <stdint.h>
#include "controller.h"
#define OS_PROTO_VERSION 1u
#define OS_MAX_PAYLOAD 48u
#define OS_FRAME_MAX (8u+OS_MAX_PAYLOAD)
enum { OS_HELLO=1, OS_STATUS=2, OS_GET_CONFIG=3, OS_SET_CONFIG=4,
       OS_DISABLE=5, OS_ACK=6, OS_GET_PID=7, OS_SET_PID=8, OS_GET_MOTION=9,
       OS_SET_MOTION=10, OS_GET_CONTROL=11 };
enum { OS_OK=0, OS_BAD_LENGTH=1, OS_REJECTED=2, OS_UNSUPPORTED=3 };
uint16_t os_crc16(const uint8_t *p,size_t n);
uint16_t os_wire_get16(const uint8_t *p);
uint32_t os_wire_get32(const uint8_t *p);
void os_wire_put16(uint8_t *p,uint16_t v);
void os_wire_put32(uint8_t *p,uint32_t v);
bool os_frame_valid(const uint8_t *in,size_t n);
size_t os_frame_reply(const uint8_t *in,uint8_t *out,uint8_t len);
/* Complete frame in, response frame out. Returns zero for malformed frame.
 * Input and output must not overlap. Caller must supply OS_FRAME_MAX bytes out.
 * Stream framing, UART ISR, and exclusive control ownership are not wired yet. */
size_t os_dispatch(os_controller *c,const uint8_t *in,size_t n,uint8_t *out);
#endif
