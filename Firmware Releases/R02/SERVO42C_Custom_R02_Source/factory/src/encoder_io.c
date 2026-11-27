#include "factory_layout.h"
/* Encoder SPI transfers. Timeout is a polling iteration count, not a time in
 * milliseconds. Keep the dummy reads and chip-select transitions in order. */
#define SPI_SELECT RAM32(0x40000804u)
#define SPI_STATUS RAM32(0x40000808u)
#define SPI_DATA RAM32(0x4000080cu)
static inline __attribute__((always_inline)) int spi_wait(uint32_t mask) {
    for (unsigned i=0;i<401u;++i)if (SPI_STATUS&mask)return 1;
    return 0;
}
static inline __attribute__((always_inline)) int read_pair(uint32_t *hi, uint32_t *lo) {
    SPI_SELECT=0;
    if (!spi_wait(4))return 0;
    SPI_DATA=0x83;
    if (!spi_wait(2))return 0;
    (void)SPI_DATA;
    if (!spi_wait(4))return 0;
    SPI_DATA=0x55;
    if (!spi_wait(2))return 0;
    *hi=SPI_DATA;
    SPI_SELECT=1;
    SPI_SELECT=0;
    if (!spi_wait(4))return 0;
    SPI_DATA=0x84;
    if (!spi_wait(2))return 0;
    (void)SPI_DATA;
    if (!spi_wait(4))return 0;
    SPI_DATA=0xa0;
    if (!spi_wait(2))return 0;
    *lo=SPI_DATA;
    SPI_SELECT=1;
    return 1;
}
/* Read the final SPI data before releasing chip select. */
uint32_t encoder_status_word(void) {
    uint32_t hi, lo;
    if (!read_pair(&hi, &lo))return 0;
    return ((hi&255u)<<8)|(lo&255u);
}
uint32_t factory_read_encoder(void) {
    uint32_t hi, lo;
    if (!read_pair(&hi, &lo))return 0;
    return (uint16_t)(((hi&1023u)<<6)+((lo&65535u)>>2));
}
/* Retry a zero encoder status once; status bit 1 reports fault code 3. */
uint32_t encoder_startup_status(void) {
    RAM8(WORK_RAM(WS_ENCODER_STATUS))=0;
    uint32_t v=encoder_status_word();
    if (!v) {
        v=encoder_status_word();
        if (!v)RAM8(WORK_RAM(WS_ENCODER_STATUS))=4;
    }
    if (v&2u)RAM8(WORK_RAM(WS_ENCODER_STATUS))=3;
    return RAM8(WORK_RAM(WS_ENCODER_STATUS));
}
extern int32_t factory_signed_divide(int32_t, int32_t);
/* trimmed encoder mean with the original negative-average quirk.
 * Valid firmware call sites supply 3..65535 samples. */
uint32_t encoder_trimmed_mean(uint32_t n) {
    int32_t previous=0, minimum=0, maximum=0;
    uint32_t sum=0;
    for (uint16_t i=0;i<n;++i) {
        int32_t v=(int32_t)factory_read_encoder();
        if (i==0)previous=minimum=maximum=v;
        if (previous-v>8192)v+=16384;
        else if (v-previous>8192)v-=16384;
        previous=v;
        if (v>maximum)maximum=v;
        else if (v<minimum)minimum=v;
        sum+=(uint32_t)v;
    }
    int32_t mean=factory_signed_divide((int32_t)(sum-(uint32_t)minimum-(uint32_t)maximum), (int32_t)(n-2u));
    if (mean<0)return (uint16_t)((uint32_t)mean*2u+16384u);
    return (uint16_t)(mean>16383?mean-16384:mean);
}
extern void save_calibration_knots(uint32_t);
extern void factory_generate_lut(void);
/* Find calibration knot limits, save the knots and generate the flash lookup
 * table. */
void calibration_record_bounds(void) {
    uint16_t v=RAM16(WORK_RAM(WS_CALIBRATION_KNOTS));
    RAM16(WORK_RAM(WS_CALIBRATION_META_RAW))=v;
    RAM16(WORK_RAM(WS_CALIBRATION_WRAP_THRESHOLD))=v;
    for (unsigned i=1;i<200;i++) {
        v=RAM16(WORK_RAM(WS_CALIBRATION_KNOTS)+2*i);
        if (v<RAM16(WORK_RAM(WS_CALIBRATION_WRAP_THRESHOLD)))RAM16(WORK_RAM(WS_CALIBRATION_WRAP_THRESHOLD))=v;
        else if (v>RAM16(WORK_RAM(WS_CALIBRATION_META_RAW)))RAM16(WORK_RAM(WS_CALIBRATION_META_RAW))=v;
    }
    RAM16(WORK_RAM(WS_CALIBRATION_KNOT_VALID))=1;
    save_calibration_knots(WORK_RAM(WS_CALIBRATION_KNOTS));
    factory_generate_lut();
}
