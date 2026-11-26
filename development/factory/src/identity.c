#include "factory_layout.h"
/* Factory hardware-transform interface, 0x064C. Preserve register order. */
typedef struct {
    uint32_t *output;
    const uint32_t *input;
    const uint32_t *key;
}
IdentityJob;
uint32_t identity_transform(const IdentityJob *job) {
    if (!job)return 4;
    for (unsigned i=0;i<4;i++) RAM32(0x40021420u+4*i)=job->key[i];
    for (unsigned i=0;i<4;i++) RAM32(0x40021410u+4*i)=job->input[i];
    RAM32(0x40021400u)&=~2u;
    RAM32(0x40021400u)|=1u;
    while (RAM32(0x40021400u)&1u) {
    }
    for (unsigned i=0;i<4;i++)job->output[i]=RAM32(0x40021410u+4*i);
    return 0;
}
extern uint32_t read_word(uint32_t), flash_erase_page(uint32_t), flash_program_halfword(uint32_t, uint32_t);
extern void factory_delay_ms(uint32_t);
/* preserve existing enrollment marker and equality gate. */
uint32_t factory_identity_check(void) {
    uint32_t input[4], output[4];
    input[0]=read_word(0x00100e78u);
    input[1]=read_word(0x00100e7cu);
    input[2]=read_word(0x00100e84u);
    input[3]=read_word(0x00100e88u);
    IdentityJob job={output, input, input};
    identity_transform(&job);
    if ((uint16_t)read_word(FACTORY_ENROLLMENT_MARKER)==0xcc82u) {
        factory_delay_ms(600);
        flash_erase_page(FACTORY_ENROLLMENT_MARKER);
        flash_program_halfword(IDENTITY_VALUE, (uint16_t)output[0]);
        flash_program_halfword((IDENTITY_VALUE+2u), output[0]>>16);
    }
    return read_word(IDENTITY_VALUE)==output[0];
}
