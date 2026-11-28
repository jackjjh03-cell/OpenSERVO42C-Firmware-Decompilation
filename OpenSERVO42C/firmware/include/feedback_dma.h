#ifndef OS_FEEDBACK_DMA_H
#define OS_FEEDBACK_DMA_H
#include "board.h"
/* A dedicated, word-aligned three-word SRAM destination for the factory DMA sequence. */
typedef struct { uint32_t scale,slot1,slot2; } os_adc_words;
void os_feedback_dma_start(const os_board_io *io,uint32_t destination);
void os_feedback_dma_stop(const os_board_io *io);
bool os_feedback_dma_complete(const os_board_io *io);
#endif
