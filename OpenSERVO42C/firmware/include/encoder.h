#ifndef OS_ENCODER_H
#define OS_ENCODER_H
#include <stdbool.h>
#include <stdint.h>
typedef enum { OS_ENC_OK, OS_ENC_TIMEOUT, OS_ENC_PARITY, OS_ENC_WEAK_MAGNET } os_encoder_result;
typedef struct {
    void *context;
    /* Exchange must be bounded. Select(true) asserts CS; false releases it. */
    bool (*exchange)(void *context,uint8_t tx,uint8_t *rx);
    void (*select)(void *context,bool active);
} os_encoder_bus;
os_encoder_result os_mt6816_decode(uint16_t word,uint16_t *angle);
os_encoder_result os_mt6816_read(const os_encoder_bus *bus,uint16_t *angle);
/* Original HC32 SPI register transport, requires separately validated bus init. */
os_encoder_bus os_hc32_encoder_bus(void);
typedef struct { int64_t position; uint16_t previous; bool initialized; } os_encoder_tracker;
void os_encoder_reset(os_encoder_tracker *t);
/* Input is a CALIBRATED 16-bit angle from the factory-compatible LUT. */
bool os_encoder_track(os_encoder_tracker *t,uint16_t angle,uint16_t max_step);
#endif
