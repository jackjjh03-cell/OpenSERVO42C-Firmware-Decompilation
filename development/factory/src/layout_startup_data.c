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

static const char options_01c[][9] = {"0.9?", "1.8?", ""};
static const char options_037[][9] = {"CR_OPEN", "CR_vFOC", "CR_UART", ""};
static const char options_05b[][9] = {"0", "200", "400", "600", "800", "1000", "1200", "1400", "1600", "1800", "2000", "2200", "2400", "2600", "2800", "3000", ""};
static const char options_0f4[][9] = {"1", "2", "4", "8", "16", "32", "64", "128", "256", ""};
static const char options_157[][9] = {"L", "H", "Hold", ""};
static const char options_17b[][9] = {"CW", "CCW", ""};
static const char options_196[][9] = {"Disable", "Eanble", ""};
static const char options_1b1[][9] = {"Disable", "Eanble", ""};
static const char options_1cc[][9] = {"Disable", "Eanble", ""};
static const char options_1e7[][9] = {"Disable", "9600", "19200", "25000", "38400", "57600", "115200", ""};
static const char options_22f[][9] = {"0xe0", "0xe1", "0xe2", "0xe3", "0xe4", "0xe5", "0xe6", "0xe7", "0xe8", "0xe9", ""};
static const char options_292[][9] = {"Disable", "DirMode", "NearMode", ""};
static const char options_2b6[][9] = {"0", "1", "2", "3", "4", ""};
static const char options_2ec[][9] = {"CW", "CCW", ""};
static const char options_307[][9] = {"Disable", "286", "412", "538", "664", "790", "916", "1042", ""};
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
static const MenuRow menu_limited[] = {
    {"Cal", factory_calibrate, 0},
    {"MotType", menu_motor_type, WORK_RAM(0x01cu)},
    {"Mode", menu_control_mode, WORK_RAM(0x037u)},
    {"Exit", callback_menu_exit, 0},
};
static const MenuRow menu_restricted[] = {
    {"MotType", menu_motor_type, WORK_RAM(0x01cu)},
    {"Mode", menu_control_mode, WORK_RAM(0x037u)},
};
static const MenuRow menu_group_protection[] = {
    {"Protect", menu_protection, WORK_RAM(0x1b1u)},
    {"Exit", callback_menu_exit, 0},
};
void initialize_factory_data(void){
    for(unsigned i=0;i<sizeof(work_ram);i++)work_ram[i]=0;
    work_ram[0x1]=0x09;
    work_ram[0x2]=0x3d;
    __aeabi_memcpy(work_ram+0x01c,options_01c,sizeof(options_01c));
    __aeabi_memcpy(work_ram+0x037,options_037,sizeof(options_037));
    __aeabi_memcpy(work_ram+0x05b,options_05b,sizeof(options_05b));
    __aeabi_memcpy(work_ram+0x0f4,options_0f4,sizeof(options_0f4));
    __aeabi_memcpy(work_ram+0x157,options_157,sizeof(options_157));
    __aeabi_memcpy(work_ram+0x17b,options_17b,sizeof(options_17b));
    __aeabi_memcpy(work_ram+0x196,options_196,sizeof(options_196));
    __aeabi_memcpy(work_ram+0x1b1,options_1b1,sizeof(options_1b1));
    __aeabi_memcpy(work_ram+0x1cc,options_1cc,sizeof(options_1cc));
    __aeabi_memcpy(work_ram+0x1e7,options_1e7,sizeof(options_1e7));
    __aeabi_memcpy(work_ram+0x22f,options_22f,sizeof(options_22f));
    __aeabi_memcpy(work_ram+0x292,options_292,sizeof(options_292));
    __aeabi_memcpy(work_ram+0x2b6,options_2b6,sizeof(options_2b6));
    __aeabi_memcpy(work_ram+0x2ec,options_2ec,sizeof(options_2ec));
    __aeabi_memcpy(work_ram+0x307,options_307,sizeof(options_307));
    __aeabi_memcpy(work_ram+0x358,menu_main,sizeof(menu_main));
    __aeabi_memcpy(work_ram+0x4fc,menu_limited,sizeof(menu_limited));
    __aeabi_memcpy(work_ram+0x560,menu_restricted,sizeof(menu_restricted));
    __aeabi_memcpy(work_ram+0x59c,menu_group_protection,sizeof(menu_group_protection));
}
