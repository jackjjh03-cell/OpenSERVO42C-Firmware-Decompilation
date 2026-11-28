/* Link-only fixture. No peripheral initialization or drive activation.
 * Not intended for flashing. All firmware modules retained by the linker. */
#include "drive.h"
#include "stream.h"
#include "protocol.h"
static os_drive drive;
static os_stream stream;
static volatile uint32_t probe_result;
void os_target_panic(void){for(;;)__asm volatile("wfi");}
void os_target_main(void){
    const os_drive_config cfg={16,1000,500,2048,300,25,false,600};
    const os_offsets offsets={2048,2048};
    os_stream_reset(&stream);
    probe_result=os_drive_init(&drive,&cfg,&offsets)?sizeof(drive)+sizeof(stream):0;
    for(;;)__asm volatile("wfi");
}
