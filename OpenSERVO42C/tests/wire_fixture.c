/* Host-only pipe transport for protocol tests. Never enables a drive. */
#include "protocol.h"
#include "run_protocol.h"
#include <stdio.h>
#include <string.h>
static int nibble(char ch){
    if(ch>='0'&&ch<='9')return ch-'0';
    if(ch>='a'&&ch<='f')return ch-'a'+10;
    if(ch>='A'&&ch<='F')return ch-'A'+10;
    return -1;
}
static uint32_t read_zero(void *ctx,uint32_t addr){(void)ctx;(void)addr;return 0;}
static void ignore_write(void *ctx,uint32_t addr,uint32_t value){(void)ctx;(void)addr;(void)value;}
int main(int argc,char **argv){
    (void)argv;
    os_controller c;os_init(&c);
    os_board_io io={0,read_zero,ignore_write,0,0};os_run run;
    const os_drive_config cfg={16,1000,500,2048,300,25,false,600};
    const os_offsets offsets={2048,2048};
    if(argc>1&&!os_run_init(&run,&io,&cfg,&offsets))return 2;
    char line[OS_FRAME_MAX*2+4];uint8_t in[OS_FRAME_MAX],out[OS_FRAME_MAX];
    while(fgets(line,sizeof line,stdin)){
        size_t n=strcspn(line,"\r\n");
        if(n>sizeof(in)*2 || n%2){puts("-");fflush(stdout);continue;}
        bool valid=true;
        for(size_t i=0;i<n/2;i++){
            int a=nibble(line[2*i]),b=nibble(line[2*i+1]);
            if(a<0||b<0){valid=false;break;}
            in[i]=(uint8_t)(a*16+b);
        }
        size_t len=valid?(argc>1?os_run_dispatch(&run,&io,in,n/2,out):os_dispatch(&c,in,n/2,out)):0;
        if(!len)puts("-");
        else{for(size_t i=0;i<len;i++)printf("%02x",(unsigned)out[i]);putchar('\n');}
        fflush(stdout);
    }
    return 0;
}
