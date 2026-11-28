#ifndef OS_BOARD_H
#define OS_BOARD_H
#include <stdbool.h>
#include <stdint.h>
#include "current.h"
/* Register access is injectable for native tests. Delay callback must be bounded. */
typedef struct {
    void *ctx;
    uint32_t (*read)(void *,uint32_t);
    void (*write)(void *,uint32_t,uint32_t);
    uint16_t (*trim)(void *,uint32_t);
    void (*delay_us)(void *,uint32_t);
} os_board_io;
typedef enum { OS_BOARD_OK,OS_BOARD_CLOCK,OS_BOARD_BUTTON,OS_BOARD_ADC,OS_BOARD_OFFSET } os_board_result;
os_board_result os_board_prepare(const os_board_io *io,os_offsets *offsets);
void os_board_step_init(const os_board_io *io,bool reverse);
os_board_io os_hc32_board_io(void);
#endif
