/* Host-only pipe transport for protocol tests. Never enables a drive. */
#include "protocol.h"
#include <stdio.h>
#include <string.h>
static int nibble(char ch){
    if(ch>='0'&&ch<='9')return ch-'0';
    if(ch>='a'&&ch<='f')return ch-'a'+10;
    if(ch>='A'&&ch<='F')return ch-'A'+10;
    return -1;
}
int main(void){
    os_controller c;os_init(&c);
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
        size_t len=valid?os_dispatch(&c,in,n/2,out):0;
        if(!len)puts("-");
        else{for(size_t i=0;i<len;i++)printf("%02x",(unsigned)out[i]);putchar('\n');}
        fflush(stdout);
    }
    return 0;
}
