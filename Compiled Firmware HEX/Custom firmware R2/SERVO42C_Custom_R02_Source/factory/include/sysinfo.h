#ifndef SYSINFO_H
#define SYSINFO_H
#include <stdint.h>
uint32_t sysinfo_read(uint32_t index);
void sysinfo_open(void);
void sysinfo_cancel(void);
uint32_t sysinfo_poll(void);
#endif
