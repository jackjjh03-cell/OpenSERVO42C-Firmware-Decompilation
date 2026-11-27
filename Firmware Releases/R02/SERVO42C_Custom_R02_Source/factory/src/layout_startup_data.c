#include "factory_layout.h"
/* Linker-owned workspace: initialize the first 0x628 bytes and clear the
 * following 0x42C bytes. */
__attribute__((section(".work_ram"),aligned(8))) unsigned char work_ram[WS_TOTAL_BYTES];
extern void __aeabi_memcpy(void *,const void *,size_t);
typedef uint32_t (*MenuCallback)(uint32_t);
typedef struct { char label[12]; MenuCallback callback; uint32_t options; } MenuRow;
_Static_assert(sizeof(MenuRow)==20,"menu record ABI");
extern uint32_t factory_calibrate(uint32_t);
extern uint32_t factory_menu_goto_origin(uint32_t);
extern uint32_t factory_menu_set_origin(uint32_t);
extern void factory_restore(void);
static uint32_t callback_factory_restore(uint32_t arg){(void)arg;factory_restore();return 0;}
extern uint32_t menu_acceleration(uint32_t);
extern uint32_t menu_autosdd(uint32_t);
extern uint32_t menu_control_mode(uint32_t);
extern uint32_t menu_current(uint32_t);
extern uint32_t menu_direction(uint32_t);
extern uint32_t menu_enable(uint32_t);
extern void menu_exit(void);
static uint32_t callback_menu_exit(uint32_t arg){(void)arg;menu_exit();return 0;}
extern uint32_t menu_interpolation(uint32_t);
extern uint32_t menu_microsteps(uint32_t);
extern uint32_t menu_motor_type(uint32_t);
extern uint32_t menu_origin_direction(uint32_t);
extern uint32_t menu_origin_mode(uint32_t);
extern uint32_t menu_origin_speed(uint32_t);
extern uint32_t menu_protection(uint32_t);
extern uint32_t menu_uart_address(uint32_t);
extern uint32_t menu_uart_baud(uint32_t);

/* Pack option strings in flash; expand into the exact original 9-byte RAM slots. */
static const uint16_t option_offsets[]={0x01c,0x037,0x05b,0x0f4,0x157,0x17b,0x196,0x1b1,0x1cc,0x1e7,0x22f,0x292,0x2b6,0x2ec,0x307};
static const char option_strings[]=
    "0.9?\0" "1.8?\0" "\0"
    "CR_OPEN\0" "CR_vFOC\0" "CR_UART\0" "\0"
    "0\0" "200\0" "400\0" "600\0" "800\0" "1000\0" "1200\0" "1400\0" "1600\0" "1800\0" "2000\0" "2200\0" "2400\0" "2600\0" "2800\0" "3000\0" "\0"
    "1\0" "2\0" "4\0" "8\0" "16\0" "32\0" "64\0" "128\0" "256\0" "\0"
    "L\0" "H\0" "Hold\0" "\0"
    "CW\0" "CCW\0" "\0"
    "Disable\0" "Eanble\0" "\0"
    "Disable\0" "Eanble\0" "\0"
    "Disable\0" "Eanble\0" "\0"
    "Disable\0" "9600\0" "19200\0" "25000\0" "38400\0" "57600\0" "115200\0" "\0"
    "0xe0\0" "0xe1\0" "0xe2\0" "0xe3\0" "0xe4\0" "0xe5\0" "0xe6\0" "0xe7\0" "0xe8\0" "0xe9\0" "\0"
    "Disable\0" "DirMode\0" "NearMode\0" "\0"
    "0\0" "1\0" "2\0" "3\0" "4\0" "\0"
    "CW\0" "CCW\0" "\0"
    "Disable\0" "286\0" "412\0" "538\0" "664\0" "790\0" "916\0" "1042\0" "\0";
static const MenuRow menu_main[] = {
    {"Cal", factory_calibrate, 0},
    {"MotType", menu_motor_type, WORK_RAM(0x01cu)},
    {"Mode", menu_control_mode, WORK_RAM(0x037u)},
    {"Ma", menu_current, WORK_RAM(0x05bu)},
    {"MStep", menu_microsteps, WORK_RAM(0x0f4u)},
    {"En", menu_enable, WORK_RAM(0x157u)},
    {"Dir", menu_direction, WORK_RAM(0x17bu)},
    {"AutoSDD", menu_autosdd, WORK_RAM(0x196u)},
    {"Protect", menu_protection, WORK_RAM(0x1b1u)},
    {"Mplyer", menu_interpolation, WORK_RAM(0x1ccu)},
    {"UartBaud", menu_uart_baud, WORK_RAM(0x1e7u)},
    {"UartAddr", menu_uart_address, WORK_RAM(0x22fu)},
    {"0_Mode", menu_origin_mode, WORK_RAM(0x292u)},
    {"Set 0", factory_menu_set_origin, 0},
    {"0_Speed", menu_origin_speed, WORK_RAM(0x2b6u)},
    {"0_Dir", menu_origin_direction, WORK_RAM(0x2ecu)},
    {"Goto 0", factory_menu_goto_origin, 0},
    {"ACC", menu_acceleration, WORK_RAM(0x307u)},
    {"Restore", callback_factory_restore, 0},
    {"Exit", callback_menu_exit, 0},
};
void initialize_factory_data(void){
    for(unsigned i=0;i<sizeof(work_ram);i++)work_ram[i]=0;
    work_ram[0x1]=0x09;
    work_ram[0x2]=0x3d;
    const char *src=option_strings;
    for(unsigned i=0;i<sizeof(option_offsets)/sizeof(option_offsets[0]);i++){
        char *dst=(char*)work_ram+option_offsets[i];
        while(*src){unsigned j=0;while(*src)dst[j++]=*src++;src++;dst+=9;}
        src++;
    }
    __aeabi_memcpy(work_ram+0x358,menu_main,sizeof(menu_main));
    __aeabi_memcpy(work_ram+0x4fc,menu_main,60);
    __aeabi_memcpy(work_ram+0x538,menu_main+19,20);
    __aeabi_memcpy(work_ram+0x560,menu_main+1,40);
    __aeabi_memcpy(work_ram+0x59c,menu_main+8,20);
    __aeabi_memcpy(work_ram+0x5b0,menu_main+19,20);
}
