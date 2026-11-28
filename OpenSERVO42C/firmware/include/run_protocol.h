#ifndef OS_RUN_PROTOCOL_H
#define OS_RUN_PROTOCOL_H
#include "run.h"
#include "protocol.h"
enum { OS_ARM=13, OS_SELECT_MODE=14, OS_MOVE_ABS=15, OS_GET_RUN=16, OS_GET_LIMITS=17, OS_SET_LIMITS=18, OS_CONFIRM_REFERENCE=19, OS_SET_LIVE_PID=20, OS_GET_LIVE_PID=21 };
/* Only the serialized control owner may dispatch. Do not call from the UART
 * ISR. Motion planning must be scheduled without starving current regulation. */
size_t os_run_dispatch(os_run *r,const os_board_io *io,const uint8_t *in,size_t n,uint8_t *out);
#endif
