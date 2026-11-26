#ifndef SERVO42C_PERIPHERALS_H
#define SERVO42C_PERIPHERALS_H
#include "factory_layout.h"
/* Memory-mapped peripheral register access. Some field names describe
 * observed operations rather than confirmed register definitions. Preserve
 * volatile access order. */
#define ADC_BASE 0x40002400u
#define ADC_AUX  0x40002480u
#define ADC_RESULT_BASE 0x40002490u
#define TIMER_BASE 0x40000C00u
#define TIMER_STRIDE 0x100u
#define TIMER_REG(t,off) RAM32(TIMER_BASE+TIMER_STRIDE*(uint32_t)(t)+(off))
#define ADC_REG(off) RAM32(ADC_BASE+(off))
#define ADC_AUX_REG(off) RAM32(ADC_AUX+(off))
#endif
