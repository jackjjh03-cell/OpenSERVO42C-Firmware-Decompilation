/* Register order and polling bound from original encoder_io.c.
 * No peripheral initialization or power-stage access here. Timing unverified. */
#include "encoder.h"
#define REG(a) (*(volatile uint32_t *)(uintptr_t)(a))
#define SPI_SELECT REG(0x40000804u)
#define SPI_STATUS REG(0x40000808u)
#define SPI_DATA REG(0x4000080cu)
static bool wait_mask(uint32_t mask){
    for(unsigned i=0;i<401u;i++)if(SPI_STATUS&mask)return true;
    return false;
}
static bool exchange(void *context,uint8_t tx,uint8_t *rx){
    (void)context;
    if(!wait_mask(4u))return false;
    SPI_DATA=tx;
    if(!wait_mask(2u))return false;
    *rx=(uint8_t)SPI_DATA;return true;
}
static void select(void *context,bool active){(void)context;SPI_SELECT=active?0u:1u;}
os_encoder_bus os_hc32_encoder_bus(void){return (os_encoder_bus){0,exchange,select};}
