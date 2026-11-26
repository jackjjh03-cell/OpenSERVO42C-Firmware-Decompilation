/* 01_application: 0x00C0–0x5E15; fixed layout, not original source. */
.org 0x00c0

/* runtime_entry */
L_00c0:
    ldr r0, [pc, #12]                       /* @00C0 0348 ldr r0, [pc, #12] literal 0x00D0 -> 0x200015F8 */
L_00c2:
    mov sp, r0                              /* @00C2 8546 mov sp, r0  */
L_00c4:
    bl L_0314                               /* @00C4 00f026f9 bl #588 -> scatter_load_init */

/* runtime_main_jump */
L_00c8:
    ldr r0, [pc, #0]                        /* @00C8 0048 ldr r0, [pc, #0] literal 0x00CC -> 0x00002CC1 */
L_00ca:
    bx r0                                   /* @00CA 0047 bx r0  */
L_00cc:
    .byte 0xc1,0x2c,0x00,0x00 /* @00CC literal */
L_00d0:
    .byte 0xf8,0x15,0x00,0x20 /* @00D0 literal */

/* reset_handler */
L_00d4:
    ldr r0, [pc, #36]                       /* @00D4 0948 ldr r0, [pc, #36] literal 0x00FC -> 0x20000000 */
L_00d6:
    ldr r2, [pc, #40]                       /* @00D6 0a4a ldr r2, [pc, #40] literal 0x0100 -> 0x00000000 */
L_00d8:
    movs r1, #0                             /* @00D8 0021 movs r1, #0  */
L_00da:
    adr r1, L_00dc                          /* @00DA 00a1 adr r1, #0 address 0x00DC -> 0x000000DC */
L_00dc:
    cmp r1, r0                              /* @00DC 8142 cmp r1, r0  */
L_00de:
    bls L_00e2                              /* @00DE 00d9 bls #0 -> 0x00E2 */
L_00e0:
    add r2, r0                              /* @00E0 0244 add r2, r0  */
L_00e2:
    ldr r0, [pc, #32]                       /* @00E2 0848 ldr r0, [pc, #32] literal 0x0104 -> 0xE000ED08 */
L_00e4:
    str r2, [r0]                            /* @00E4 0260 str r2, [r0]  */
L_00e6:
    ldr r0, [pc, #32]                       /* @00E6 0848 ldr r0, [pc, #32] literal 0x0108 -> 0x00001A8D */
L_00e8:
    blx r0                                  /* @00E8 8047 blx r0  */
L_00ea:
    ldr r0, [pc, #32]                       /* @00EA 0848 ldr r0, [pc, #32] literal 0x010C -> 0x000000C1 */
L_00ec:
    bx r0                                   /* @00EC 0047 bx r0  */

/* nmi_default */
L_00ee:
    b L_00ee                                /* @00EE fee7 b #-4 -> nmi_default */
    .byte 0xfe,0xe7 /* @00F0 unclassified */

/* sub_00F2 */
L_00f2:
    b L_00f2                                /* @00F2 fee7 b #-4 -> sub_00F2 */

/* sub_00F4 */
L_00f4:
    b L_00f4                                /* @00F4 fee7 b #-4 -> sub_00F4 */
    .byte 0xfe,0xe7,0xfe,0xe7,0x00,0x00 /* @00F6 unclassified */
L_00fc:
    .byte 0x00,0x00,0x00,0x20 /* @00FC literal */
L_0100:
    .byte 0x00,0x00,0x00,0x00 /* @0100 literal */
L_0104:
    .byte 0x08,0xed,0x00,0xe0 /* @0104 literal */
L_0108:
    .byte 0x8d,0x1a,0x00,0x00 /* @0108 literal */
L_010c:
    .byte 0xc1,0x00,0x00,0x00 /* @010C literal */

/* udivmod32 */
L_0110:
    push {r4, r5, lr}                       /* @0110 30b5 push {r4, r5, lr}  */
L_0112:
    mov r3, r1                              /* @0112 0b46 mov r3, r1  */
L_0114:
    mov r1, r0                              /* @0114 0146 mov r1, r0  */
L_0116:
    movs r0, #0                             /* @0116 0020 movs r0, #0  */
L_0118:
    movs r2, #32                            /* @0118 2022 movs r2, #32  */
L_011a:
    movs r4, #1                             /* @011A 0124 movs r4, #1  */
L_011c:
    b L_0132                                /* @011C 09e0 b #18 -> 0x0132 */
L_011e:
    mov r5, r1                              /* @011E 0d46 mov r5, r1  */
L_0120:
    lsrs r5, r2                             /* @0120 d540 lsrs r5, r2  */
L_0122:
    cmp r5, r3                              /* @0122 9d42 cmp r5, r3  */
L_0124:
    blo L_0132                              /* @0124 05d3 blo #10 -> 0x0132 */
L_0126:
    mov r5, r3                              /* @0126 1d46 mov r5, r3  */
L_0128:
    lsls r5, r2                             /* @0128 9540 lsls r5, r2  */
L_012a:
    subs r1, r1, r5                         /* @012A 491b subs r1, r1, r5  */
L_012c:
    mov r5, r4                              /* @012C 2546 mov r5, r4  */
L_012e:
    lsls r5, r2                             /* @012E 9540 lsls r5, r2  */
L_0130:
    adds r0, r0, r5                         /* @0130 4019 adds r0, r0, r5  */
L_0132:
    mov r5, r2                              /* @0132 1546 mov r5, r2  */
L_0134:
    subs r2, r2, #1                         /* @0134 521e subs r2, r2, #1  */
L_0136:
    cmp r5, #0                              /* @0136 002d cmp r5, #0  */
L_0138:
    bgt L_011e                              /* @0138 f1dc bgt #-30 -> 0x011E */
L_013a:
    pop {r4, r5, pc}                        /* @013A 30bd pop {r4, r5, pc}  */

/* sdivmod32 */
L_013c:
    push {r4, r5, r6, lr}                   /* @013C 70b5 push {r4, r5, r6, lr}  */
L_013e:
    movs r4, #0                             /* @013E 0024 movs r4, #0  */
L_0140:
    mov r5, r4                              /* @0140 2546 mov r5, r4  */
L_0142:
    cmp r0, #0                              /* @0142 0028 cmp r0, #0  */
L_0144:
    bge L_014a                              /* @0144 01da bge #2 -> 0x014A */
L_0146:
    movs r4, #1                             /* @0146 0124 movs r4, #1  */
L_0148:
    rsbs r0, r0, #0                         /* @0148 4042 rsbs r0, r0, #0  */
L_014a:
    cmp r1, #0                              /* @014A 0029 cmp r1, #0  */
L_014c:
    bge L_0152                              /* @014C 01da bge #2 -> 0x0152 */
L_014e:
    movs r5, #1                             /* @014E 0125 movs r5, #1  */
L_0150:
    rsbs r1, r1, #0                         /* @0150 4942 rsbs r1, r1, #0  */
L_0152:
    bl L_0110                               /* @0152 fff7ddff bl #-70 -> udivmod32 */
L_0156:
    cmp r4, r5                              /* @0156 ac42 cmp r4, r5  */
L_0158:
    beq L_015c                              /* @0158 00d0 beq #0 -> 0x015C */
L_015a:
    rsbs r0, r0, #0                         /* @015A 4042 rsbs r0, r0, #0  */
L_015c:
    cmp r4, #0                              /* @015C 002c cmp r4, #0  */
L_015e:
    beq L_0162                              /* @015E 00d0 beq #0 -> 0x0162 */
L_0160:
    rsbs r1, r1, #0                         /* @0160 4942 rsbs r1, r1, #0  */
L_0162:
    pop {r4, r5, r6, pc}                    /* @0162 70bd pop {r4, r5, r6, pc}  */

/* signed_divmod64_wrapper */
L_0164:
    push {r3, r4, r5, r6, r7, lr}           /* @0164 f8b5 push {r3, r4, r5, r6, r7, lr}  */
L_0166:
    movs r4, #0                             /* @0166 0024 movs r4, #0  */
L_0168:
    mov r5, r4                              /* @0168 2546 mov r5, r4  */
L_016a:
    cmp r1, #0                              /* @016A 0029 cmp r1, #0  */
L_016c:
    bge L_0178                              /* @016C 04da bge #8 -> 0x0178 */
L_016e:
    mov r6, r1                              /* @016E 0e46 mov r6, r1  */
L_0170:
    movs r4, #1                             /* @0170 0124 movs r4, #1  */
L_0172:
    movs r1, #0                             /* @0172 0021 movs r1, #0  */
L_0174:
    rsbs r0, r0, #0                         /* @0174 4042 rsbs r0, r0, #0  */
L_0176:
    sbcs r1, r6                             /* @0176 b141 sbcs r1, r6  */
L_0178:
    cmp r3, #0                              /* @0178 002b cmp r3, #0  */
L_017a:
    bge L_0186                              /* @017A 04da bge #8 -> 0x0186 */
L_017c:
    mov r6, r3                              /* @017C 1e46 mov r6, r3  */
L_017e:
    movs r3, #0                             /* @017E 0023 movs r3, #0  */
L_0180:
    rsbs r2, r2, #0                         /* @0180 5242 rsbs r2, r2, #0  */
L_0182:
    movs r5, #1                             /* @0182 0125 movs r5, #1  */
L_0184:
    sbcs r3, r6                             /* @0184 b341 sbcs r3, r6  */
L_0186:
    bl L_0292                               /* @0186 00f084f8 bl #264 -> sub_0292 */
L_018a:
    mov r12, r1                             /* @018A 8c46 mov r12, r1  */
L_018c:
    mov r7, r2                              /* @018C 1746 mov r7, r2  */
L_018e:
    mov r6, r3                              /* @018E 1e46 mov r6, r3  */
L_0190:
    cmp r4, r5                              /* @0190 ac42 cmp r4, r5  */
L_0192:
    beq L_019c                              /* @0192 03d0 beq #6 -> 0x019C */
L_0194:
    movs r5, #0                             /* @0194 0025 movs r5, #0  */
L_0196:
    rsbs r0, r0, #0                         /* @0196 4042 rsbs r0, r0, #0  */
L_0198:
    sbcs r5, r1                             /* @0198 8d41 sbcs r5, r1  */
L_019a:
    mov r12, r5                             /* @019A ac46 mov r12, r5  */
L_019c:
    cmp r4, #0                              /* @019C 002c cmp r4, #0  */
L_019e:
    beq L_01a6                              /* @019E 02d0 beq #4 -> 0x01A6 */
L_01a0:
    movs r6, #0                             /* @01A0 0026 movs r6, #0  */
L_01a2:
    rsbs r7, r2, #0                         /* @01A2 5742 rsbs r7, r2, #0  */
L_01a4:
    sbcs r6, r3                             /* @01A4 9e41 sbcs r6, r3  */
L_01a6:
    mov r1, r12                             /* @01A6 6146 mov r1, r12  */
L_01a8:
    mov r2, r7                              /* @01A8 3a46 mov r2, r7  */
L_01aa:
    mov r3, r6                              /* @01AA 3346 mov r3, r6  */
L_01ac:
    add sp, #4                              /* @01AC 01b0 add sp, #4  */
L_01ae:
    pop {r4, r5, r6, r7, pc}                /* @01AE f0bd pop {r4, r5, r6, r7, pc}  */

/* sub_01B0 */
L_01b0:
    push {r4, r5, r6, r7, lr}               /* @01B0 f0b5 push {r4, r5, r6, r7, lr}  */
L_01b2:
    push {r0, r1, r2, r3, r4}               /* @01B2 1fb4 push {r0, r1, r2, r3, r4}  */
L_01b4:
    sub sp, #24                             /* @01B4 86b0 sub sp, #24  */
L_01b6:
    movs r0, #0                             /* @01B6 0020 movs r0, #0  */
L_01b8:
    str r0, [sp]                            /* @01B8 0090 str r0, [sp]  */
L_01ba:
    str r0, [sp, #4]                        /* @01BA 0190 str r0, [sp, #4]  */
L_01bc:
    str r0, [sp, #8]                        /* @01BC 0290 str r0, [sp, #8]  */
L_01be:
    ldr r0, [sp, #24]                       /* @01BE 0698 ldr r0, [sp, #24]  */
L_01c0:
    ldr r1, [sp, #24]                       /* @01C0 0699 ldr r1, [sp, #24]  */
L_01c2:
    uxth r0, r0                             /* @01C2 80b2 uxth r0, r0  */
L_01c4:
    str r0, [sp, #16]                       /* @01C4 0490 str r0, [sp, #16]  */
L_01c6:
    ldr r0, [sp, #28]                       /* @01C6 0798 ldr r0, [sp, #28]  */
L_01c8:
    lsrs r1, r1, #16                        /* @01C8 090c lsrs r1, r1, #16  */
L_01ca:
    lsls r2, r0, #16                        /* @01CA 0204 lsls r2, r0, #16  */
L_01cc:
    asrs r0, r0, #16                        /* @01CC 0014 asrs r0, r0, #16  */
L_01ce:
    orrs r1, r2                             /* @01CE 1143 orrs r1, r2  */
L_01d0:
    str r0, [sp, #28]                       /* @01D0 0790 str r0, [sp, #28]  */
L_01d2:
    movs r7, #0                             /* @01D2 0027 movs r7, #0  */
L_01d4:
    ldr r5, [sp, #32]                       /* @01D4 089d ldr r5, [sp, #32]  */
L_01d6:
    ldr r0, [sp, #36]                       /* @01D6 0998 ldr r0, [sp, #36]  */
L_01d8:
    str r1, [sp, #24]                       /* @01D8 0691 str r1, [sp, #24]  */
L_01da:
    mov r6, r7                              /* @01DA 3e46 mov r6, r7  */
L_01dc:
    mov r4, r7                              /* @01DC 3c46 mov r4, r7  */
L_01de:
    str r0, [sp, #12]                       /* @01DE 0390 str r0, [sp, #12]  */
L_01e0:
    ldr r0, [sp, #12]                       /* @01E0 0398 ldr r0, [sp, #12]  */
L_01e2:
    uxth r1, r5                             /* @01E2 a9b2 uxth r1, r5  */
L_01e4:
    lsls r2, r0, #16                        /* @01E4 0204 lsls r2, r0, #16  */
L_01e6:
    lsrs r0, r0, #16                        /* @01E6 000c lsrs r0, r0, #16  */
L_01e8:
    str r0, [sp, #12]                       /* @01E8 0390 str r0, [sp, #12]  */
L_01ea:
    ldr r0, [sp, #16]                       /* @01EA 0498 ldr r0, [sp, #16]  */
L_01ec:
    lsrs r5, r5, #16                        /* @01EC 2d0c lsrs r5, r5, #16  */
L_01ee:
    orrs r5, r2                             /* @01EE 1543 orrs r5, r2  */
L_01f0:
    muls r0, r1, r0                         /* @01F0 4843 muls r0, r1, r0  */
L_01f2:
    movs r1, #0                             /* @01F2 0021 movs r1, #0  */
L_01f4:
    mov r2, r4                              /* @01F4 2246 mov r2, r4  */
L_01f6:
    bl L_02f2                               /* @01F6 00f07cf8 bl #248 -> sub_02F2 */
L_01fa:
    adds r7, r0, r7                         /* @01FA c719 adds r7, r0, r7  */
L_01fc:
    adcs r1, r6                             /* @01FC 7141 adcs r1, r6  */
L_01fe:
    adds r4, #16                            /* @01FE 1034 adds r4, #16  */
L_0200:
    mov r6, r1                              /* @0200 0e46 mov r6, r1  */
L_0202:
    cmp r4, #64                             /* @0202 402c cmp r4, #64  */
L_0204:
    blt L_01e0                              /* @0204 ecdb blt #-40 -> 0x01E0 */
L_0206:
    mov r0, r7                              /* @0206 3846 mov r0, r7  */
L_0208:
    ldr r2, [sp, #8]                        /* @0208 029a ldr r2, [sp, #8]  */
L_020a:
    bl L_02f2                               /* @020A 00f072f8 bl #228 -> sub_02F2 */
L_020e:
    ldr r2, [sp]                            /* @020E 009a ldr r2, [sp]  */
L_0210:
    ldr r3, [sp, #4]                        /* @0210 019b ldr r3, [sp, #4]  */
L_0212:
    adds r0, r0, r2                         /* @0212 8018 adds r0, r0, r2  */
L_0214:
    str r0, [sp]                            /* @0214 0090 str r0, [sp]  */
L_0216:
    adcs r1, r3                             /* @0216 5941 adcs r1, r3  */
L_0218:
    ldr r0, [sp, #8]                        /* @0218 0298 ldr r0, [sp, #8]  */
L_021a:
    str r1, [sp, #4]                        /* @021A 0191 str r1, [sp, #4]  */
L_021c:
    adds r0, #16                            /* @021C 1030 adds r0, #16  */
L_021e:
    str r0, [sp, #8]                        /* @021E 0290 str r0, [sp, #8]  */
L_0220:
    cmp r0, #64                             /* @0220 4028 cmp r0, #64  */
L_0222:
    blt L_01be                              /* @0222 ccdb blt #-104 -> 0x01BE */
L_0224:
    ldr r0, [sp]                            /* @0224 0098 ldr r0, [sp]  */
L_0226:
    add sp, #44                             /* @0226 0bb0 add sp, #44  */
L_0228:
    pop {r4, r5, r6, r7, pc}                /* @0228 f0bd pop {r4, r5, r6, r7, pc}  */

/* sub_022A */
L_022a:
    mov r3, r0                              /* @022A 0346 mov r3, r0  */
L_022c:
    orrs r3, r1                             /* @022C 0b43 orrs r3, r1  */
L_022e:
    lsls r3, r3, #30                        /* @022E 9b07 lsls r3, r3, #30  */
L_0230:
    beq L_023a                              /* @0230 03d0 beq #6 -> 0x023A */
L_0232:
    b L_0248                                /* @0232 09e0 b #18 -> 0x0248 */
L_0234:
    ldmia r1!, {r3}                         /* @0234 08c9 ldm r1!, {r3}  */
L_0236:
    subs r2, r2, #4                         /* @0236 121f subs r2, r2, #4  */
L_0238:
    stmia r0!, {r3}                         /* @0238 08c0 stm r0!, {r3}  */
L_023a:
    cmp r2, #4                              /* @023A 042a cmp r2, #4  */
L_023c:
    bhs L_0234                              /* @023C fad2 bhs #-12 -> 0x0234 */
L_023e:
    b L_0248                                /* @023E 03e0 b #6 -> 0x0248 */
L_0240:
    ldrb r3, [r1]                           /* @0240 0b78 ldrb r3, [r1]  */
L_0242:
    strb r3, [r0]                           /* @0242 0370 strb r3, [r0]  */
L_0244:
    adds r0, r0, #1                         /* @0244 401c adds r0, r0, #1  */
L_0246:
    adds r1, r1, #1                         /* @0246 491c adds r1, r1, #1  */
L_0248:
    subs r2, r2, #1                         /* @0248 521e subs r2, r2, #1  */
L_024a:
    bhs L_0240                              /* @024A f9d2 bhs #-14 -> 0x0240 */
L_024c:
    bx lr                                   /* @024C 7047 bx lr  */

/* runtime_shared_entry_024e */
L_024e:
    uxtb r2, r2                             /* @024E d2b2 uxtb r2, r2  */
L_0250:
    b L_0256                                /* @0250 01e0 b #2 -> 0x0256 */
L_0252:
    strb r2, [r0]                           /* @0252 0270 strb r2, [r0]  */
L_0254:
    adds r0, r0, #1                         /* @0254 401c adds r0, r0, #1  */
L_0256:
    subs r1, r1, #1                         /* @0256 491e subs r1, r1, #1  */
L_0258:
    bhs L_0252                              /* @0258 fbd2 bhs #-10 -> 0x0252 */
L_025a:
    bx lr                                   /* @025A 7047 bx lr  */

/* sub_025C */
L_025c:
    movs r2, #0                             /* @025C 0022 movs r2, #0  */
L_025e:
    b L_024e                                /* @025E f6e7 b #-20 -> 0x024E */

/* runtime_wrapper_0260 */
L_0260:
    push {r4, lr}                           /* @0260 10b5 push {r4, lr} Ghidra-discovered wrapper */
L_0262:
    mov r3, r2                              /* @0262 1346 mov r3, r2 Ghidra-discovered wrapper */
L_0264:
    mov r2, r1                              /* @0264 0a46 mov r2, r1 Ghidra-discovered wrapper */
L_0266:
    mov r4, r0                              /* @0266 0446 mov r4, r0 Ghidra-discovered wrapper */
L_0268:
    mov r1, r3                              /* @0268 1946 mov r1, r3 Ghidra-discovered wrapper */
L_026a:
    bl L_024e                               /* @026A fff7f0ff bl #-32 Ghidra-discovered wrapper */
L_026e:
    mov r0, r4                              /* @026E 2046 mov r0, r4 Ghidra-discovered wrapper */
L_0270:
    pop {r4, pc}                            /* @0270 10bd pop {r4, pc} Ghidra-discovered wrapper */

/* sub_0272 */
L_0272:
    adds r2, r0, #1                         /* @0272 421c adds r2, r0, #1  */
L_0274:
    ldrb r1, [r0]                           /* @0274 0178 ldrb r1, [r0]  */
L_0276:
    adds r0, r0, #1                         /* @0276 401c adds r0, r0, #1  */
L_0278:
    cmp r1, #0                              /* @0278 0029 cmp r1, #0  */
L_027a:
    bne L_0274                              /* @027A fbd1 bne #-10 -> 0x0274 */
L_027c:
    subs r0, r0, r2                         /* @027C 801a subs r0, r0, r2  */
L_027e:
    bx lr                                   /* @027E 7047 bx lr  */

/* sub_0280 */
L_0280:
    mov r3, r0                              /* @0280 0346 mov r3, r0  */
L_0282:
    ldrb r2, [r1]                           /* @0282 0a78 ldrb r2, [r1]  */
L_0284:
    strb r2, [r0]                           /* @0284 0270 strb r2, [r0]  */
L_0286:
    adds r0, r0, #1                         /* @0286 401c adds r0, r0, #1  */
L_0288:
    adds r1, r1, #1                         /* @0288 491c adds r1, r1, #1  */
L_028a:
    cmp r2, #0                              /* @028A 002a cmp r2, #0  */
L_028c:
    bne L_0282                              /* @028C f9d1 bne #-14 -> 0x0282 */
L_028e:
    mov r0, r3                              /* @028E 1846 mov r0, r3  */
L_0290:
    bx lr                                   /* @0290 7047 bx lr  */

/* sub_0292 */
L_0292:
    push {r4, r5, r6, r7, lr}               /* @0292 f0b5 push {r4, r5, r6, r7, lr}  */
L_0294:
    push {r0, r1, r2, r3, r4}               /* @0294 1fb4 push {r0, r1, r2, r3, r4}  */
L_0296:
    mov r6, r0                              /* @0296 0646 mov r6, r0  */
L_0298:
    movs r0, #0                             /* @0298 0020 movs r0, #0  */
L_029a:
    sub sp, #8                              /* @029A 82b0 sub sp, #8  */
L_029c:
    mov r5, r0                              /* @029C 0546 mov r5, r0  */
L_029e:
    movs r4, #64                            /* @029E 4024 movs r4, #64  */
L_02a0:
    str r1, [sp, #4]                        /* @02A0 0191 str r1, [sp, #4]  */
L_02a2:
    str r0, [sp]                            /* @02A2 0090 str r0, [sp]  */
L_02a4:
    b L_02de                                /* @02A4 1be0 b #54 -> 0x02DE */
L_02a6:
    ldr r1, [sp, #4]                        /* @02A6 0199 ldr r1, [sp, #4]  */
L_02a8:
    mov r2, r4                              /* @02A8 2246 mov r2, r4  */
L_02aa:
    mov r7, r1                              /* @02AA 0f46 mov r7, r1  */
L_02ac:
    mov r0, r6                              /* @02AC 3046 mov r0, r6  */
L_02ae:
    bl L_0338                               /* @02AE 00f043f8 bl #134 -> logical_shift_right64 */
L_02b2:
    ldr r2, [sp, #16]                       /* @02B2 049a ldr r2, [sp, #16]  */
L_02b4:
    ldr r3, [sp, #20]                       /* @02B4 059b ldr r3, [sp, #20]  */
L_02b6:
    subs r0, r0, r2                         /* @02B6 801a subs r0, r0, r2  */
L_02b8:
    sbcs r1, r3                             /* @02B8 9941 sbcs r1, r3  */
L_02ba:
    blo L_02de                              /* @02BA 10d3 blo #32 -> 0x02DE */
L_02bc:
    mov r0, r2                              /* @02BC 1046 mov r0, r2  */
L_02be:
    mov r1, r3                              /* @02BE 1946 mov r1, r3  */
L_02c0:
    mov r2, r4                              /* @02C0 2246 mov r2, r4  */
L_02c2:
    bl L_02f2                               /* @02C2 00f016f8 bl #44 -> sub_02F2 */
L_02c6:
    subs r6, r6, r0                         /* @02C6 361a subs r6, r6, r0  */
L_02c8:
    sbcs r7, r1                             /* @02C8 8f41 sbcs r7, r1  */
L_02ca:
    str r7, [sp, #4]                        /* @02CA 0197 str r7, [sp, #4]  */
L_02cc:
    mov r2, r4                              /* @02CC 2246 mov r2, r4  */
L_02ce:
    movs r0, #1                             /* @02CE 0120 movs r0, #1  */
L_02d0:
    movs r1, #0                             /* @02D0 0021 movs r1, #0  */
L_02d2:
    ldr r7, [sp]                            /* @02D2 009f ldr r7, [sp]  */
L_02d4:
    bl L_02f2                               /* @02D4 00f00df8 bl #26 -> sub_02F2 */
L_02d8:
    adds r0, r7, r0                         /* @02D8 3818 adds r0, r7, r0  */
L_02da:
    adcs r5, r1                             /* @02DA 4d41 adcs r5, r1  */
L_02dc:
    str r0, [sp]                            /* @02DC 0090 str r0, [sp]  */
L_02de:
    mov r0, r4                              /* @02DE 2046 mov r0, r4  */
L_02e0:
    subs r4, r4, #1                         /* @02E0 641e subs r4, r4, #1  */
L_02e2:
    cmp r0, #0                              /* @02E2 0028 cmp r0, #0  */
L_02e4:
    bgt L_02a6                              /* @02E4 dfdc bgt #-66 -> 0x02A6 */
L_02e6:
    ldr r3, [sp, #4]                        /* @02E6 019b ldr r3, [sp, #4]  */
L_02e8:
    ldr r0, [sp]                            /* @02E8 0098 ldr r0, [sp]  */
L_02ea:
    mov r1, r5                              /* @02EA 2946 mov r1, r5  */
L_02ec:
    mov r2, r6                              /* @02EC 3246 mov r2, r6  */
L_02ee:
    add sp, #28                             /* @02EE 07b0 add sp, #28  */
L_02f0:
    pop {r4, r5, r6, r7, pc}                /* @02F0 f0bd pop {r4, r5, r6, r7, pc}  */

/* sub_02F2 */
L_02f2:
    push {r4, lr}                           /* @02F2 10b5 push {r4, lr}  */
L_02f4:
    cmp r2, #32                             /* @02F4 202a cmp r2, #32  */
L_02f6:
    blt L_0302                              /* @02F6 04db blt #8 -> 0x0302 */
L_02f8:
    mov r1, r0                              /* @02F8 0146 mov r1, r0  */
L_02fa:
    subs r2, #32                            /* @02FA 203a subs r2, #32  */
L_02fc:
    lsls r1, r2                             /* @02FC 9140 lsls r1, r2  */
L_02fe:
    movs r0, #0                             /* @02FE 0020 movs r0, #0  */
L_0300:
    pop {r4, pc}                            /* @0300 10bd pop {r4, pc}  */
L_0302:
    lsls r1, r2                             /* @0302 9140 lsls r1, r2  */
L_0304:
    movs r3, #32                            /* @0304 2023 movs r3, #32  */
L_0306:
    subs r4, r3, r2                         /* @0306 9c1a subs r4, r3, r2  */
L_0308:
    mov r3, r0                              /* @0308 0346 mov r3, r0  */
L_030a:
    lsrs r3, r4                             /* @030A e340 lsrs r3, r4  */
L_030c:
    orrs r1, r3                             /* @030C 1943 orrs r1, r3  */
L_030e:
    lsls r0, r2                             /* @030E 9040 lsls r0, r2  */
L_0310:
    pop {r4, pc}                            /* @0310 10bd pop {r4, pc}  */
    .byte 0x00,0x00 /* @0312 unclassified */

/* scatter_load_init */
L_0314:
    ldr r4, [pc, #24]                       /* @0314 064c ldr r4, [pc, #24] literal 0x0330 -> 0x00002250 */
L_0316:
    movs r5, #1                             /* @0316 0125 movs r5, #1  */
L_0318:
    ldr r6, [pc, #24]                       /* @0318 064e ldr r6, [pc, #24] literal 0x0334 -> 0x00002270 */
L_031a:
    b L_0328                                /* @031A 05e0 b #10 -> 0x0328 */
L_031c:
    ldr r3, [r4, #12]                       /* @031C e368 ldr r3, [r4, #12]  */
L_031e:
    ldmia r4!, {r0, r1, r2}                 /* @031E 07cc ldm r4!, {r0, r1, r2}  */
L_0320:
    orrs r3, r5                             /* @0320 2b43 orrs r3, r5  */
L_0322:
    subs r4, #12                            /* @0322 0c3c subs r4, #12  */
L_0324:
    blx r3                                  /* @0324 9847 blx r3  */
L_0326:
    adds r4, #16                            /* @0326 1034 adds r4, #16  */
L_0328:
    cmp r4, r6                              /* @0328 b442 cmp r4, r6  */
L_032a:
    blo L_031c                              /* @032A f7d3 blo #-18 -> 0x031C */
L_032c:
    bl L_00c8                               /* @032C fff7ccfe bl #-616 -> runtime_main_jump */
L_0330:
    .byte 0x50,0x22,0x00,0x00 /* @0330 literal */
L_0334:
    .byte 0x70,0x22,0x00,0x00 /* @0334 literal */

/* logical_shift_right64 */
L_0338:
    push {r4, lr}                           /* @0338 10b5 push {r4, lr}  */
L_033a:
    cmp r2, #32                             /* @033A 202a cmp r2, #32  */
L_033c:
    blt L_0348                              /* @033C 04db blt #8 -> 0x0348 */
L_033e:
    mov r0, r1                              /* @033E 0846 mov r0, r1  */
L_0340:
    subs r2, #32                            /* @0340 203a subs r2, #32  */
L_0342:
    lsrs r0, r2                             /* @0342 d040 lsrs r0, r2  */
L_0344:
    movs r1, #0                             /* @0344 0021 movs r1, #0  */
L_0346:
    pop {r4, pc}                            /* @0346 10bd pop {r4, pc}  */
L_0348:
    mov r3, r1                              /* @0348 0b46 mov r3, r1  */
L_034a:
    lsrs r3, r2                             /* @034A d340 lsrs r3, r2  */
L_034c:
    lsrs r0, r2                             /* @034C d040 lsrs r0, r2  */
L_034e:
    movs r4, #32                            /* @034E 2024 movs r4, #32  */
L_0350:
    subs r2, r4, r2                         /* @0350 a21a subs r2, r4, r2  */
L_0352:
    lsls r1, r2                             /* @0352 9140 lsls r1, r2  */
L_0354:
    orrs r0, r1                             /* @0354 0843 orrs r0, r1  */
L_0356:
    mov r1, r3                              /* @0356 1946 mov r1, r3  */
L_0358:
    pop {r4, pc}                            /* @0358 10bd pop {r4, pc}  */

/* scatter_decompress */
L_035a:
    push {r4, r5, r6, lr}                   /* @035A 70b5 push {r4, r5, r6, lr}  */
L_035c:
    adds r5, r1, r2                         /* @035C 8d18 adds r5, r1, r2  */
L_035e:
    ldrb r4, [r0]                           /* @035E 0478 ldrb r4, [r0]  */
L_0360:
    adds r0, r0, #1                         /* @0360 401c adds r0, r0, #1  */
L_0362:
    lsls r2, r4, #30                        /* @0362 a207 lsls r2, r4, #30  */
L_0364:
    lsrs r2, r2, #30                        /* @0364 920f lsrs r2, r2, #30  */
L_0366:
    bne L_036c                              /* @0366 01d1 bne #2 -> 0x036C */
L_0368:
    ldrb r2, [r0]                           /* @0368 0278 ldrb r2, [r0]  */
L_036a:
    adds r0, r0, #1                         /* @036A 401c adds r0, r0, #1  */
L_036c:
    asrs r3, r4, #4                         /* @036C 2311 asrs r3, r4, #4  */
L_036e:
    bne L_037e                              /* @036E 06d1 bne #12 -> 0x037E */
L_0370:
    ldrb r3, [r0]                           /* @0370 0378 ldrb r3, [r0]  */
L_0372:
    adds r0, r0, #1                         /* @0372 401c adds r0, r0, #1  */
L_0374:
    b L_037e                                /* @0374 03e0 b #6 -> 0x037E */
L_0376:
    ldrb r6, [r0]                           /* @0376 0678 ldrb r6, [r0]  */
L_0378:
    strb r6, [r1]                           /* @0378 0e70 strb r6, [r1]  */
L_037a:
    adds r0, r0, #1                         /* @037A 401c adds r0, r0, #1  */
L_037c:
    adds r1, r1, #1                         /* @037C 491c adds r1, r1, #1  */
L_037e:
    subs r2, r2, #1                         /* @037E 521e subs r2, r2, #1  */
L_0380:
    bne L_0376                              /* @0380 f9d1 bne #-14 -> 0x0376 */
L_0382:
    cmp r3, #0                              /* @0382 002b cmp r3, #0  */
L_0384:
    beq L_03b2                              /* @0384 15d0 beq #42 -> 0x03B2 */
L_0386:
    ldrb r2, [r0]                           /* @0386 0278 ldrb r2, [r0]  */
L_0388:
    movs r6, #12                            /* @0388 0c26 movs r6, #12  */
L_038a:
    subs r2, r1, r2                         /* @038A 8a1a subs r2, r1, r2  */
L_038c:
    ands r4, r6                             /* @038C 3440 ands r4, r6  */
L_038e:
    adds r0, r0, #1                         /* @038E 401c adds r0, r0, #1  */
L_0390:
    cmp r4, #12                             /* @0390 0c2c cmp r4, #12  */
L_0392:
    beq L_039c                              /* @0392 03d0 beq #6 -> 0x039C */
L_0394:
    lsls r4, r4, #6                         /* @0394 a401 lsls r4, r4, #6  */
L_0396:
    subs r2, r2, r4                         /* @0396 121b subs r2, r2, r4  */
L_0398:
    adds r3, r3, #2                         /* @0398 9b1c adds r3, r3, #2  */
L_039a:
    b L_03ae                                /* @039A 08e0 b #16 -> 0x03AE */
L_039c:
    ldrb r4, [r0]                           /* @039C 0478 ldrb r4, [r0]  */
L_039e:
    lsls r4, r4, #8                         /* @039E 2402 lsls r4, r4, #8  */
L_03a0:
    subs r2, r2, r4                         /* @03A0 121b subs r2, r2, r4  */
L_03a2:
    adds r0, r0, #1                         /* @03A2 401c adds r0, r0, #1  */
L_03a4:
    b L_0398                                /* @03A4 f8e7 b #-16 -> 0x0398 */
L_03a6:
    ldrb r4, [r2]                           /* @03A6 1478 ldrb r4, [r2]  */
L_03a8:
    strb r4, [r1]                           /* @03A8 0c70 strb r4, [r1]  */
L_03aa:
    adds r1, r1, #1                         /* @03AA 491c adds r1, r1, #1  */
L_03ac:
    adds r2, r2, #1                         /* @03AC 521c adds r2, r2, #1  */
L_03ae:
    subs r3, r3, #1                         /* @03AE 5b1e subs r3, r3, #1  */
L_03b0:
    bpl L_03a6                              /* @03B0 f9d5 bpl #-14 -> 0x03A6 */
L_03b2:
    cmp r1, r5                              /* @03B2 a942 cmp r1, r5  */
L_03b4:
    blo L_035e                              /* @03B4 d3d3 blo #-90 -> 0x035E */
L_03b6:
    movs r0, #0                             /* @03B6 0020 movs r0, #0  */
L_03b8:
    pop {r4, r5, r6, pc}                    /* @03B8 70bd pop {r4, r5, r6, pc}  */
    .byte 0x00,0x00 /* @03BA unclassified */

/* sub_03BC */
L_03bc:
    push {r4, r5, r6, lr}                   /* @03BC 70b5 push {r4, r5, r6, lr}  */
L_03be:
    ldr r5, [pc, #144]                      /* @03BE 244d ldr r5, [pc, #144] literal 0x0450 -> 0x40002480 */
L_03c0:
    ldr r0, [r5, #48]                       /* @03C0 286b ldr r0, [r5, #48]  */
L_03c2:
    lsls r0, r0, #26                        /* @03C2 8006 lsls r0, r0, #26  */
L_03c4:
    bpl L_0444                              /* @03C4 3ed5 bpl #124 -> 0x0444 */
L_03c6:
    movs r0, #0                             /* @03C6 0020 movs r0, #0  */
L_03c8:
    bl L_074c                               /* @03C8 00f0c0f9 bl #896 -> adc_read_result */
L_03cc:
    movs r2, #93                            /* @03CC 5d22 movs r2, #93  */
L_03ce:
    lsls r2, r2, #3                         /* @03CE d200 lsls r2, r2, #3  */
L_03d0:
    ldr r1, [pc, #128]                      /* @03D0 2049 ldr r1, [pc, #128] literal 0x0454 -> 0x0000FFFF */
L_03d2:
    ldr r4, [pc, #132]                      /* @03D2 214c ldr r4, [pc, #132] literal 0x0458 -> 0x200006E0 */
L_03d4:
    cmp r0, r2                              /* @03D4 9042 cmp r0, r2  */
L_03d6:
    ble L_0442                              /* @03D6 34dd ble #104 -> 0x0442 */
L_03d8:
    ldrh r0, [r4, #2]                       /* @03D8 6088 ldrh r0, [r4, #2]  */
L_03da:
    cmp r0, r1                              /* @03DA 8842 cmp r0, r1  */
L_03dc:
    bne L_03e2                              /* @03DC 01d1 bne #2 -> 0x03E2 */
L_03de:
    movs r0, #0                             /* @03DE 0020 movs r0, #0  */
L_03e0:
    strh r0, [r4, #2]                       /* @03E0 6080 strh r0, [r4, #2]  */
L_03e2:
    ldrh r0, [r4, #2]                       /* @03E2 6088 ldrh r0, [r4, #2]  */
L_03e4:
    cmp r0, #99                             /* @03E4 6328 cmp r0, #99  */
L_03e6:
    bls L_0430                              /* @03E6 23d9 bls #70 -> 0x0430 */
L_03e8:
    ldrh r0, [r4, #2]                       /* @03E8 6088 ldrh r0, [r4, #2]  */
L_03ea:
    cmp r0, #116                            /* @03EA 7428 cmp r0, #116  */
L_03ec:
    bhs L_0430                              /* @03EC 20d2 bhs #64 -> 0x0430 */
L_03ee:
    movs r0, #2                             /* @03EE 0220 movs r0, #2  */
L_03f0:
    bl L_074c                               /* @03F0 00f0acf9 bl #856 -> adc_read_result */
L_03f4:
    ldr r1, [r4, #12]                       /* @03F4 e168 ldr r1, [r4, #12]  */
L_03f6:
    adds r0, r0, r1                         /* @03F6 4018 adds r0, r0, r1  */
L_03f8:
    str r0, [r4, #12]                       /* @03F8 e060 str r0, [r4, #12]  */
L_03fa:
    movs r0, #1                             /* @03FA 0120 movs r0, #1  */
L_03fc:
    bl L_074c                               /* @03FC 00f0a6f9 bl #844 -> adc_read_result */
L_0400:
    ldr r1, [r4, #16]                       /* @0400 2169 ldr r1, [r4, #16]  */
L_0402:
    adds r0, r0, r1                         /* @0402 4018 adds r0, r0, r1  */
L_0404:
    str r0, [r4, #16]                       /* @0404 2061 str r0, [r4, #16]  */
L_0406:
    ldrh r0, [r4, #2]                       /* @0406 6088 ldrh r0, [r4, #2]  */
L_0408:
    cmp r0, #115                            /* @0408 7328 cmp r0, #115  */
L_040a:
    blo L_0430                              /* @040A 11d3 blo #34 -> 0x0430 */
L_040c:
    bl L_2410                               /* @040C 02f000f8 bl #8192 -> sub_2410 */
L_0410:
    ldr r0, [r4, #12]                       /* @0410 e068 ldr r0, [r4, #12]  */
L_0412:
    ldr r1, [r4, #4]                        /* @0412 6168 ldr r1, [r4, #4]  */
L_0414:
    asrs r0, r0, #4                         /* @0414 0011 asrs r0, r0, #4  */
L_0416:
    subs r0, r0, r1                         /* @0416 401a subs r0, r0, r1  */
L_0418:
    bl L_373a                               /* @0418 03f08ff9 bl #13086 -> abs32_candidate */
L_041c:
    str r0, [r4, #12]                       /* @041C e060 str r0, [r4, #12]  */
L_041e:
    ldr r0, [r4, #16]                       /* @041E 2069 ldr r0, [r4, #16]  */
L_0420:
    ldr r1, [r4, #8]                        /* @0420 a168 ldr r1, [r4, #8]  */
L_0422:
    asrs r0, r0, #4                         /* @0422 0011 asrs r0, r0, #4  */
L_0424:
    subs r0, r0, r1                         /* @0424 401a subs r0, r0, r1  */
L_0426:
    bl L_373a                               /* @0426 03f088f9 bl #13072 -> abs32_candidate */
L_042a:
    str r0, [r4, #16]                       /* @042A 2061 str r0, [r4, #16]  */
L_042c:
    movs r0, #1                             /* @042C 0120 movs r0, #1  */
L_042e:
    strb r0, [r4]                           /* @042E 2070 strb r0, [r4]  */
L_0430:
    ldrh r0, [r4, #2]                       /* @0430 6088 ldrh r0, [r4, #2]  */
L_0432:
    adds r0, r0, #1                         /* @0432 401c adds r0, r0, #1  */
L_0434:
    strh r0, [r4, #2]                       /* @0434 6080 strh r0, [r4, #2]  */
L_0436:
    ldrh r0, [r4, #2]                       /* @0436 6088 ldrh r0, [r4, #2]  */
L_0438:
    cmp r0, #115                            /* @0438 7328 cmp r0, #115  */
L_043a:
    bls L_0444                              /* @043A 03d9 bls #6 -> 0x0444 */
L_043c:
    movs r0, #116                           /* @043C 7420 movs r0, #116  */
L_043e:
    strh r0, [r4, #2]                       /* @043E 6080 strh r0, [r4, #2]  */
L_0440:
    b L_0444                                /* @0440 00e0 b #0 -> 0x0444 */
L_0442:
    strh r1, [r4, #2]                       /* @0442 6180 strh r1, [r4, #2]  */
L_0444:
    ldr r0, [r5, #52]                       /* @0444 686b ldr r0, [r5, #52]  */
L_0446:
    movs r1, #32                            /* @0446 2021 movs r1, #32  */
L_0448:
    bics r0, r1                             /* @0448 8843 bics r0, r1  */
L_044a:
    str r0, [r5, #52]                       /* @044A 6863 str r0, [r5, #52]  */
L_044c:
    pop {r4, r5, r6, pc}                    /* @044C 70bd pop {r4, r5, r6, pc}  */
    .byte 0x00,0x00 /* @044E unclassified */
L_0450:
    .byte 0x80,0x24,0x00,0x40 /* @0450 literal */
L_0454:
    .byte 0xff,0xff,0x00,0x00 /* @0454 literal */
L_0458:
    .byte 0xe0,0x06,0x00,0x20 /* @0458 literal */

/* sub_045C */
L_045c:
    push {r4, lr}                           /* @045C 10b5 push {r4, lr}  */
L_045e:
    mov r8, r8                              /* @045E c046 mov r8, r8  */
L_0460:
    mov r8, r8                              /* @0460 c046 mov r8, r8  */
L_0462:
    pop {r4, pc}                            /* @0462 10bd pop {r4, pc}  */

/* sub_0464 */
L_0464:
    push {r4, lr}                           /* @0464 10b5 push {r4, lr}  */
L_0466:
    ldr r4, [pc, #36]                       /* @0466 094c ldr r4, [pc, #36] literal 0x048C -> 0x40021000 */
L_0468:
    ldr r0, [r4, #20]                       /* @0468 6069 ldr r0, [r4, #20]  */
L_046a:
    lsls r0, r0, #13                        /* @046A 4003 lsls r0, r0, #13  */
L_046c:
    lsrs r0, r0, #29                        /* @046C 400f lsrs r0, r0, #29  */
L_046e:
    cmp r0, #5                              /* @046E 0528 cmp r0, #5  */
L_0470:
    bne L_0476                              /* @0470 01d1 bne #2 -> 0x0476 */
L_0472:
    bl L_3890                               /* @0472 03f00dfa bl #13338 -> sub_3890 */
L_0476:
    ldr r0, [r4, #20]                       /* @0476 6069 ldr r0, [r4, #20]  */
L_0478:
    movs r1, #7                             /* @0478 0721 movs r1, #7  */
L_047a:
    lsls r1, r1, #16                        /* @047A 0904 lsls r1, r1, #16  */
L_047c:
    bics r0, r1                             /* @047C 8843 bics r0, r1  */
L_047e:
    str r0, [r4, #20]                       /* @047E 6061 str r0, [r4, #20]  */
L_0480:
    ldr r0, [pc, #12]                       /* @0480 0348 ldr r0, [pc, #12] literal 0x0490 -> 0x40002480 */
L_0482:
    ldr r1, [r0, #52]                       /* @0482 416b ldr r1, [r0, #52]  */
L_0484:
    movs r2, #32                            /* @0484 2022 movs r2, #32  */
L_0486:
    bics r1, r2                             /* @0486 9143 bics r1, r2  */
L_0488:
    str r1, [r0, #52]                       /* @0488 4163 str r1, [r0, #52]  */
L_048a:
    pop {r4, pc}                            /* @048A 10bd pop {r4, pc}  */
L_048c:
    .byte 0x00,0x10,0x02,0x40 /* @048C literal */
L_0490:
    .byte 0x80,0x24,0x00,0x40 /* @0490 literal */

/* sub_0494 */
L_0494:
    push {r4, lr}                           /* @0494 10b5 push {r4, lr}  */
L_0496:
    mov r8, r8                              /* @0496 c046 mov r8, r8  */
L_0498:
    mov r8, r8                              /* @0498 c046 mov r8, r8  */
L_049a:
    pop {r4, pc}                            /* @049A 10bd pop {r4, pc}  */

/* sub_049C */
L_049c:
    push {r4, lr}                           /* @049C 10b5 push {r4, lr}  */
L_049e:
    mov r8, r8                              /* @049E c046 mov r8, r8  */
L_04a0:
    mov r8, r8                              /* @04A0 c046 mov r8, r8  */
L_04a2:
    pop {r4, pc}                            /* @04A2 10bd pop {r4, pc}  */

/* sub_04A4 */
L_04a4:
    push {r4, lr}                           /* @04A4 10b5 push {r4, lr}  */
L_04a6:
    mov r8, r8                              /* @04A6 c046 mov r8, r8  */
L_04a8:
    mov r8, r8                              /* @04A8 c046 mov r8, r8  */
L_04aa:
    pop {r4, pc}                            /* @04AA 10bd pop {r4, pc}  */

/* sub_04AC */
L_04ac:
    push {r4, lr}                           /* @04AC 10b5 push {r4, lr}  */
L_04ae:
    mov r8, r8                              /* @04AE c046 mov r8, r8  */
L_04b0:
    mov r8, r8                              /* @04B0 c046 mov r8, r8  */
L_04b2:
    pop {r4, pc}                            /* @04B2 10bd pop {r4, pc}  */

/* sub_04B4 */
L_04b4:
    push {r4, lr}                           /* @04B4 10b5 push {r4, lr}  */
L_04b6:
    mov r8, r8                              /* @04B6 c046 mov r8, r8  */
L_04b8:
    mov r8, r8                              /* @04B8 c046 mov r8, r8  */
L_04ba:
    pop {r4, pc}                            /* @04BA 10bd pop {r4, pc}  */

/* sub_04BC */
L_04bc:
    push {r0, r1, r2, r3, r4, r5, r6, lr}   /* @04BC 7fb5 push {r0, r1, r2, r3, r4, r5, r6, lr}  */
L_04be:
    sub sp, #8                              /* @04BE 82b0 sub sp, #8  */
L_04c0:
    mov r4, r0                              /* @04C0 0446 mov r4, r0  */
L_04c2:
    ldr r0, [pc, #124]                      /* @04C2 1f48 ldr r0, [pc, #124] literal 0x0540 -> 0x00005E16 */
L_04c4:
    lsls r1, r3, #1                         /* @04C4 5900 lsls r1, r3, #1  */
L_04c6:
    ldr r5, [pc, #124]                      /* @04C6 1f4d ldr r5, [pc, #124] literal 0x0544 -> 0x200006C0 */
L_04c8:
    ldrsh r1, [r0, r1]                      /* @04C8 415e ldrsh r1, [r0, r1]  */
L_04ca:
    str r1, [r5, #20]                       /* @04CA 6961 str r1, [r5, #20]  */
L_04cc:
    adds r3, #255                           /* @04CC ff33 adds r3, #255  */
L_04ce:
    adds r3, #1                             /* @04CE 0133 adds r3, #1  */
L_04d0:
    lsls r1, r3, #1                         /* @04D0 5900 lsls r1, r3, #1  */
L_04d2:
    ldrsh r0, [r0, r1]                      /* @04D2 405e ldrsh r0, [r0, r1]  */
L_04d4:
    str r0, [r5, #16]                       /* @04D4 2861 str r0, [r5, #16]  */
L_04d6:
    ldr r1, [sp, #16]                       /* @04D6 0499 ldr r1, [sp, #16]  */
L_04d8:
    ldr r0, [r5, #20]                       /* @04D8 6869 ldr r0, [r5, #20]  */
L_04da:
    muls r1, r0, r1                         /* @04DA 4143 muls r1, r0, r1  */
L_04dc:
    ldr r0, [sp, #12]                       /* @04DC 0398 ldr r0, [sp, #12]  */
L_04de:
    ldr r2, [r5, #16]                       /* @04DE 2a69 ldr r2, [r5, #16]  */
L_04e0:
    muls r0, r2, r0                         /* @04E0 5043 muls r0, r2, r0  */
L_04e2:
    adds r0, r1, r0                         /* @04E2 0818 adds r0, r1, r0  */
L_04e4:
    str r0, [sp]                            /* @04E4 0090 str r0, [sp]  */
L_04e6:
    movs r6, #1                             /* @04E6 0126 movs r6, #1  */
L_04e8:
    ldr r0, [sp]                            /* @04E8 0098 ldr r0, [sp]  */
L_04ea:
    lsls r6, r6, #13                        /* @04EA 7603 lsls r6, r6, #13  */
L_04ec:
    cmp r0, #0                              /* @04EC 0028 cmp r0, #0  */
L_04ee:
    beq L_0508                              /* @04EE 0bd0 beq #22 -> 0x0508 */
L_04f0:
    ldr r0, [sp]                            /* @04F0 0098 ldr r0, [sp]  */
L_04f2:
    bl L_373a                               /* @04F2 03f022f9 bl #12868 -> abs32_candidate */
L_04f6:
    adds r0, r0, r6                         /* @04F6 8019 adds r0, r0, r6  */
L_04f8:
    asrs r1, r0, #14                        /* @04F8 8113 asrs r1, r0, #14  */
L_04fa:
    ldr r0, [sp]                            /* @04FA 0098 ldr r0, [sp]  */
L_04fc:
    cmp r0, #0                              /* @04FC 0028 cmp r0, #0  */
L_04fe:
    ble L_0504                              /* @04FE 01dd ble #2 -> 0x0504 */
L_0500:
    str r1, [sp]                            /* @0500 0091 str r1, [sp]  */
L_0502:
    b L_0508                                /* @0502 01e0 b #2 -> 0x0508 */
L_0504:
    rsbs r0, r1, #0                         /* @0504 4842 rsbs r0, r1, #0  */
L_0506:
    str r0, [sp]                            /* @0506 0090 str r0, [sp]  */
L_0508:
    ldr r0, [sp, #16]                       /* @0508 0498 ldr r0, [sp, #16]  */
L_050a:
    ldr r1, [r5, #16]                       /* @050A 2969 ldr r1, [r5, #16]  */
L_050c:
    muls r0, r1, r0                         /* @050C 4843 muls r0, r1, r0  */
L_050e:
    ldr r1, [sp, #12]                       /* @050E 0399 ldr r1, [sp, #12]  */
L_0510:
    ldr r2, [r5, #20]                       /* @0510 6a69 ldr r2, [r5, #20]  */
L_0512:
    muls r1, r2, r1                         /* @0512 5143 muls r1, r2, r1  */
L_0514:
    subs r0, r0, r1                         /* @0514 401a subs r0, r0, r1  */
L_0516:
    str r0, [sp, #4]                        /* @0516 0190 str r0, [sp, #4]  */
L_0518:
    ldr r0, [sp, #4]                        /* @0518 0198 ldr r0, [sp, #4]  */
L_051a:
    cmp r0, #0                              /* @051A 0028 cmp r0, #0  */
L_051c:
    beq L_0536                              /* @051C 0bd0 beq #22 -> 0x0536 */
L_051e:
    ldr r0, [sp, #4]                        /* @051E 0198 ldr r0, [sp, #4]  */
L_0520:
    bl L_373a                               /* @0520 03f00bf9 bl #12822 -> abs32_candidate */
L_0524:
    adds r0, r0, r6                         /* @0524 8019 adds r0, r0, r6  */
L_0526:
    asrs r1, r0, #14                        /* @0526 8113 asrs r1, r0, #14  */
L_0528:
    ldr r0, [sp, #4]                        /* @0528 0198 ldr r0, [sp, #4]  */
L_052a:
    cmp r0, #0                              /* @052A 0028 cmp r0, #0  */
L_052c:
    ble L_0532                              /* @052C 01dd ble #2 -> 0x0532 */
L_052e:
    str r1, [sp, #4]                        /* @052E 0191 str r1, [sp, #4]  */
L_0530:
    b L_0536                                /* @0530 01e0 b #2 -> 0x0536 */
L_0532:
    rsbs r0, r1, #0                         /* @0532 4842 rsbs r0, r1, #0  */
L_0534:
    str r0, [sp, #4]                        /* @0534 0190 str r0, [sp, #4]  */
L_0536:
    ldr r0, [sp]                            /* @0536 0098 ldr r0, [sp]  */
L_0538:
    ldr r1, [sp, #4]                        /* @0538 0199 ldr r1, [sp, #4]  */
L_053a:
    stmia r4!, {r0, r1}                     /* @053A 03c4 stm r4!, {r0, r1}  */
L_053c:
    add sp, #24                             /* @053C 06b0 add sp, #24  */
L_053e:
    pop {r4, r5, r6, pc}                    /* @053E 70bd pop {r4, r5, r6, pc}  */
L_0540:
    .byte 0x16,0x5e,0x00,0x00 /* @0540 literal */
L_0544:
    .byte 0xc0,0x06,0x00,0x20 /* @0544 literal */

/* sub_0548 */
L_0548:
    push {r4, lr}                           /* @0548 10b5 push {r4, lr}  */
L_054a:
    mov r8, r8                              /* @054A c046 mov r8, r8  */
L_054c:
    mov r8, r8                              /* @054C c046 mov r8, r8  */
L_054e:
    pop {r4, pc}                            /* @054E 10bd pop {r4, pc}  */

/* sub_0550 */
L_0550:
    push {r4, lr}                           /* @0550 10b5 push {r4, lr}  */
L_0552:
    mov r8, r8                              /* @0552 c046 mov r8, r8  */
L_0554:
    mov r8, r8                              /* @0554 c046 mov r8, r8  */
L_0556:
    pop {r4, pc}                            /* @0556 10bd pop {r4, pc}  */

/* sub_0558 */
L_0558:
    push {r4, lr}                           /* @0558 10b5 push {r4, lr}  */
L_055a:
    subs r4, r0, r1                         /* @055A 441a subs r4, r0, r1  */
L_055c:
    ldr r0, [pc, #56]                       /* @055C 0e48 ldr r0, [pc, #56] literal 0x0598 -> 0x200005F0 */
L_055e:
    ldrh r0, [r0]                           /* @055E 0088 ldrh r0, [r0]  */
L_0560:
    muls r4, r0, r4                         /* @0560 4443 muls r4, r0, r4  */
L_0562:
    cmp r4, #0                              /* @0562 002c cmp r4, #0  */
L_0564:
    beq L_057e                              /* @0564 0bd0 beq #22 -> 0x057E */
L_0566:
    mov r0, r4                              /* @0566 2046 mov r0, r4  */
L_0568:
    bl L_373a                               /* @0568 03f0e7f8 bl #12750 -> abs32_candidate */
L_056c:
    movs r1, #1                             /* @056C 0121 movs r1, #1  */
L_056e:
    lsls r1, r1, #10                        /* @056E 8902 lsls r1, r1, #10  */
L_0570:
    adds r0, r0, r1                         /* @0570 4018 adds r0, r0, r1  */
L_0572:
    asrs r0, r0, #11                        /* @0572 c012 asrs r0, r0, #11  */
L_0574:
    cmp r4, #0                              /* @0574 002c cmp r4, #0  */
L_0576:
    ble L_057c                              /* @0576 01dd ble #2 -> 0x057C */
L_0578:
    mov r4, r0                              /* @0578 0446 mov r4, r0  */
L_057a:
    b L_057e                                /* @057A 00e0 b #0 -> 0x057E */
L_057c:
    rsbs r4, r0, #0                         /* @057C 4442 rsbs r4, r0, #0  */
L_057e:
    ldr r0, [pc, #28]                       /* @057E 0748 ldr r0, [pc, #28] literal 0x059C -> 0x200006C0 */
L_0580:
    ldr r1, [r0, #8]                        /* @0580 8168 ldr r1, [r0, #8]  */
L_0582:
    cmp r1, r4                              /* @0582 a142 cmp r1, r4  */
L_0584:
    bgt L_058a                              /* @0584 01dc bgt #2 -> 0x058A */
L_0586:
    ldr r0, [r0, #8]                        /* @0586 8068 ldr r0, [r0, #8]  */
L_0588:
    pop {r4, pc}                            /* @0588 10bd pop {r4, pc}  */
L_058a:
    ldr r1, [r0, #12]                       /* @058A c168 ldr r1, [r0, #12]  */
L_058c:
    cmp r1, r4                              /* @058C a142 cmp r1, r4  */
L_058e:
    blt L_0594                              /* @058E 01db blt #2 -> 0x0594 */
L_0590:
    ldr r0, [r0, #12]                       /* @0590 c068 ldr r0, [r0, #12]  */
L_0592:
    pop {r4, pc}                            /* @0592 10bd pop {r4, pc}  */
L_0594:
    mov r0, r4                              /* @0594 2046 mov r0, r4  */
L_0596:
    pop {r4, pc}                            /* @0596 10bd pop {r4, pc}  */
L_0598:
    .byte 0xf0,0x05,0x00,0x20 /* @0598 literal */
L_059c:
    .byte 0xc0,0x06,0x00,0x20 /* @059C literal */

/* sub_05A0 */
L_05a0:
    push {r4, lr}                           /* @05A0 10b5 push {r4, lr}  */
L_05a2:
    subs r4, r0, r1                         /* @05A2 441a subs r4, r0, r1  */
L_05a4:
    ldr r0, [pc, #56]                       /* @05A4 0e48 ldr r0, [pc, #56] literal 0x05E0 -> 0x200005F0 */
L_05a6:
    ldrh r0, [r0]                           /* @05A6 0088 ldrh r0, [r0]  */
L_05a8:
    muls r4, r0, r4                         /* @05A8 4443 muls r4, r0, r4  */
L_05aa:
    cmp r4, #0                              /* @05AA 002c cmp r4, #0  */
L_05ac:
    beq L_05c6                              /* @05AC 0bd0 beq #22 -> 0x05C6 */
L_05ae:
    mov r0, r4                              /* @05AE 2046 mov r0, r4  */
L_05b0:
    bl L_373a                               /* @05B0 03f0c3f8 bl #12678 -> abs32_candidate */
L_05b4:
    movs r1, #1                             /* @05B4 0121 movs r1, #1  */
L_05b6:
    lsls r1, r1, #10                        /* @05B6 8902 lsls r1, r1, #10  */
L_05b8:
    adds r0, r0, r1                         /* @05B8 4018 adds r0, r0, r1  */
L_05ba:
    asrs r0, r0, #11                        /* @05BA c012 asrs r0, r0, #11  */
L_05bc:
    cmp r4, #0                              /* @05BC 002c cmp r4, #0  */
L_05be:
    ble L_05c4                              /* @05BE 01dd ble #2 -> 0x05C4 */
L_05c0:
    mov r4, r0                              /* @05C0 0446 mov r4, r0  */
L_05c2:
    b L_05c6                                /* @05C2 00e0 b #0 -> 0x05C6 */
L_05c4:
    rsbs r4, r0, #0                         /* @05C4 4442 rsbs r4, r0, #0  */
L_05c6:
    ldr r0, [pc, #28]                       /* @05C6 0748 ldr r0, [pc, #28] literal 0x05E4 -> 0x200006C0 */
L_05c8:
    ldr r1, [r0, #8]                        /* @05C8 8168 ldr r1, [r0, #8]  */
L_05ca:
    cmp r1, r4                              /* @05CA a142 cmp r1, r4  */
L_05cc:
    bgt L_05d2                              /* @05CC 01dc bgt #2 -> 0x05D2 */
L_05ce:
    ldr r0, [r0, #8]                        /* @05CE 8068 ldr r0, [r0, #8]  */
L_05d0:
    pop {r4, pc}                            /* @05D0 10bd pop {r4, pc}  */
L_05d2:
    ldr r1, [r0, #12]                       /* @05D2 c168 ldr r1, [r0, #12]  */
L_05d4:
    cmp r1, r4                              /* @05D4 a142 cmp r1, r4  */
L_05d6:
    blt L_05dc                              /* @05D6 01db blt #2 -> 0x05DC */
L_05d8:
    ldr r0, [r0, #12]                       /* @05D8 c068 ldr r0, [r0, #12]  */
L_05da:
    pop {r4, pc}                            /* @05DA 10bd pop {r4, pc}  */
L_05dc:
    mov r0, r4                              /* @05DC 2046 mov r0, r4  */
L_05de:
    pop {r4, pc}                            /* @05DE 10bd pop {r4, pc}  */
L_05e0:
    .byte 0xf0,0x05,0x00,0x20 /* @05E0 literal */
L_05e4:
    .byte 0xc0,0x06,0x00,0x20 /* @05E4 literal */

/* sub_05E8 */
L_05e8:
    push {r4, lr}                           /* @05E8 10b5 push {r4, lr}  */
L_05ea:
    mov r8, r8                              /* @05EA c046 mov r8, r8  */
L_05ec:
    mov r8, r8                              /* @05EC c046 mov r8, r8  */
L_05ee:
    pop {r4, pc}                            /* @05EE 10bd pop {r4, pc}  */

/* sub_05F0 */
L_05f0:
    bx lr                                   /* @05F0 7047 bx lr  */

/* sub_05F2 */
L_05f2:
    push {r4, lr}                           /* @05F2 10b5 push {r4, lr}  */
L_05f4:
    mov r8, r8                              /* @05F4 c046 mov r8, r8  */
L_05f6:
    mov r8, r8                              /* @05F6 c046 mov r8, r8  */
L_05f8:
    pop {r4, pc}                            /* @05F8 10bd pop {r4, pc}  */

/* sub_05FA */
L_05fa:
    push {r4, lr}                           /* @05FA 10b5 push {r4, lr}  */
L_05fc:
    mov r8, r8                              /* @05FC c046 mov r8, r8  */
L_05fe:
    mov r8, r8                              /* @05FE c046 mov r8, r8  */
L_0600:
    pop {r4, pc}                            /* @0600 10bd pop {r4, pc}  */

/* sub_0602 */
L_0602:
    push {r4, lr}                           /* @0602 10b5 push {r4, lr}  */
L_0604:
    mov r8, r8                              /* @0604 c046 mov r8, r8  */
L_0606:
    mov r8, r8                              /* @0606 c046 mov r8, r8  */
L_0608:
    pop {r4, pc}                            /* @0608 10bd pop {r4, pc}  */

/* sub_060A */
L_060a:
    push {r4, lr}                           /* @060A 10b5 push {r4, lr}  */
L_060c:
    mov r8, r8                              /* @060C c046 mov r8, r8  */
L_060e:
    mov r8, r8                              /* @060E c046 mov r8, r8  */
L_0610:
    pop {r4, pc}                            /* @0610 10bd pop {r4, pc}  */

/* sub_0612 */
L_0612:
    push {r4, lr}                           /* @0612 10b5 push {r4, lr}  */
L_0614:
    mov r8, r8                              /* @0614 c046 mov r8, r8  */
L_0616:
    mov r8, r8                              /* @0616 c046 mov r8, r8  */
L_0618:
    pop {r4, pc}                            /* @0618 10bd pop {r4, pc}  */

/* sub_061A */
L_061a:
    push {r4, lr}                           /* @061A 10b5 push {r4, lr}  */
L_061c:
    mov r8, r8                              /* @061C c046 mov r8, r8  */
L_061e:
    mov r8, r8                              /* @061E c046 mov r8, r8  */
L_0620:
    pop {r4, pc}                            /* @0620 10bd pop {r4, pc}  */

/* sub_0622 */
L_0622:
    push {r4, lr}                           /* @0622 10b5 push {r4, lr}  */
L_0624:
    mov r8, r8                              /* @0624 c046 mov r8, r8  */
L_0626:
    mov r8, r8                              /* @0626 c046 mov r8, r8  */
L_0628:
    pop {r4, pc}                            /* @0628 10bd pop {r4, pc}  */

/* sub_062A */
L_062a:
    push {r4, lr}                           /* @062A 10b5 push {r4, lr}  */
L_062c:
    mov r8, r8                              /* @062C c046 mov r8, r8  */
L_062e:
    mov r8, r8                              /* @062E c046 mov r8, r8  */
L_0630:
    pop {r4, pc}                            /* @0630 10bd pop {r4, pc}  */

/* sub_0632 */
L_0632:
    push {r4, lr}                           /* @0632 10b5 push {r4, lr}  */
L_0634:
    mov r8, r8                              /* @0634 c046 mov r8, r8  */
L_0636:
    mov r8, r8                              /* @0636 c046 mov r8, r8  */
L_0638:
    pop {r4, pc}                            /* @0638 10bd pop {r4, pc}  */

/* sub_063A */
L_063a:
    bx lr                                   /* @063A 7047 bx lr  */

/* sub_063C */
L_063c:
    push {r4, lr}                           /* @063C 10b5 push {r4, lr}  */
L_063e:
    mov r8, r8                              /* @063E c046 mov r8, r8  */
L_0640:
    mov r8, r8                              /* @0640 c046 mov r8, r8  */
L_0642:
    pop {r4, pc}                            /* @0642 10bd pop {r4, pc}  */

/* sub_0644 */
L_0644:
    push {r4, lr}                           /* @0644 10b5 push {r4, lr}  */
L_0646:
    mov r8, r8                              /* @0646 c046 mov r8, r8  */
L_0648:
    mov r8, r8                              /* @0648 c046 mov r8, r8  */
L_064a:
    pop {r4, pc}                            /* @064A 10bd pop {r4, pc}  */

/* sub_064C */
L_064c:
    cmp r0, #0                              /* @064C 0028 cmp r0, #0  */
L_064e:
    beq L_06b4                              /* @064E 31d0 beq #98 -> 0x06B4 */
L_0650:
    ldr r1, [r0, #8]                        /* @0650 8168 ldr r1, [r0, #8]  */
L_0652:
    ldr r2, [r1]                            /* @0652 0a68 ldr r2, [r1]  */
L_0654:
    ldr r1, [pc, #96]                       /* @0654 1849 ldr r1, [pc, #96] literal 0x06B8 -> 0x40021400 */
L_0656:
    str r2, [r1, #32]                       /* @0656 0a62 str r2, [r1, #32]  */
L_0658:
    ldr r2, [r0, #8]                        /* @0658 8268 ldr r2, [r0, #8]  */
L_065a:
    ldr r2, [r2, #4]                        /* @065A 5268 ldr r2, [r2, #4]  */
L_065c:
    str r2, [r1, #36]                       /* @065C 4a62 str r2, [r1, #36]  */
L_065e:
    ldr r2, [r0, #8]                        /* @065E 8268 ldr r2, [r0, #8]  */
L_0660:
    ldr r2, [r2, #8]                        /* @0660 9268 ldr r2, [r2, #8]  */
L_0662:
    str r2, [r1, #40]                       /* @0662 8a62 str r2, [r1, #40]  */
L_0664:
    ldr r2, [r0, #8]                        /* @0664 8268 ldr r2, [r0, #8]  */
L_0666:
    ldr r2, [r2, #12]                       /* @0666 d268 ldr r2, [r2, #12]  */
L_0668:
    str r2, [r1, #44]                       /* @0668 ca62 str r2, [r1, #44]  */
L_066a:
    ldr r2, [r0, #4]                        /* @066A 4268 ldr r2, [r0, #4]  */
L_066c:
    ldr r2, [r2]                            /* @066C 1268 ldr r2, [r2]  */
L_066e:
    str r2, [r1, #16]                       /* @066E 0a61 str r2, [r1, #16]  */
L_0670:
    ldr r2, [r0, #4]                        /* @0670 4268 ldr r2, [r0, #4]  */
L_0672:
    ldr r2, [r2, #4]                        /* @0672 5268 ldr r2, [r2, #4]  */
L_0674:
    str r2, [r1, #20]                       /* @0674 4a61 str r2, [r1, #20]  */
L_0676:
    ldr r2, [r0, #4]                        /* @0676 4268 ldr r2, [r0, #4]  */
L_0678:
    ldr r2, [r2, #8]                        /* @0678 9268 ldr r2, [r2, #8]  */
L_067a:
    str r2, [r1, #24]                       /* @067A 8a61 str r2, [r1, #24]  */
L_067c:
    ldr r2, [r0, #4]                        /* @067C 4268 ldr r2, [r0, #4]  */
L_067e:
    ldr r2, [r2, #12]                       /* @067E d268 ldr r2, [r2, #12]  */
L_0680:
    str r2, [r1, #28]                       /* @0680 ca61 str r2, [r1, #28]  */
L_0682:
    ldr r2, [r1]                            /* @0682 0a68 ldr r2, [r1]  */
L_0684:
    movs r3, #2                             /* @0684 0223 movs r3, #2  */
L_0686:
    bics r2, r3                             /* @0686 9a43 bics r2, r3  */
L_0688:
    str r2, [r1]                            /* @0688 0a60 str r2, [r1]  */
L_068a:
    ldr r2, [r1]                            /* @068A 0a68 ldr r2, [r1]  */
L_068c:
    movs r3, #1                             /* @068C 0123 movs r3, #1  */
L_068e:
    orrs r2, r3                             /* @068E 1a43 orrs r2, r3  */
L_0690:
    str r2, [r1]                            /* @0690 0a60 str r2, [r1]  */
L_0692:
    ldr r2, [r1]                            /* @0692 0a68 ldr r2, [r1]  */
L_0694:
    lsls r2, r2, #31                        /* @0694 d207 lsls r2, r2, #31  */
L_0696:
    bne L_0692                              /* @0696 fcd1 bne #-8 -> 0x0692 */
L_0698:
    ldr r2, [r1, #16]                       /* @0698 0a69 ldr r2, [r1, #16]  */
L_069a:
    ldr r3, [r0]                            /* @069A 0368 ldr r3, [r0]  */
L_069c:
    str r2, [r3]                            /* @069C 1a60 str r2, [r3]  */
L_069e:
    ldr r2, [r1, #20]                       /* @069E 4a69 ldr r2, [r1, #20]  */
L_06a0:
    ldr r3, [r0]                            /* @06A0 0368 ldr r3, [r0]  */
L_06a2:
    str r2, [r3, #4]                        /* @06A2 5a60 str r2, [r3, #4]  */
L_06a4:
    ldr r2, [r1, #24]                       /* @06A4 8a69 ldr r2, [r1, #24]  */
L_06a6:
    ldr r3, [r0]                            /* @06A6 0368 ldr r3, [r0]  */
L_06a8:
    str r2, [r3, #8]                        /* @06A8 9a60 str r2, [r3, #8]  */
L_06aa:
    ldr r1, [r1, #28]                       /* @06AA c969 ldr r1, [r1, #28]  */
L_06ac:
    ldr r0, [r0]                            /* @06AC 0068 ldr r0, [r0]  */
L_06ae:
    str r1, [r0, #12]                       /* @06AE c160 str r1, [r0, #12]  */
L_06b0:
    movs r0, #0                             /* @06B0 0020 movs r0, #0  */
L_06b2:
    bx lr                                   /* @06B2 7047 bx lr  */
L_06b4:
    movs r0, #4                             /* @06B4 0420 movs r0, #4  */
L_06b6:
    bx lr                                   /* @06B6 7047 bx lr  */
L_06b8:
    .byte 0x00,0x14,0x02,0x40 /* @06B8 literal */

/* sub_06BC */
L_06bc:
    lsls r1, r1, #27                        /* @06BC c906 lsls r1, r1, #27  */
L_06be:
    ldr r2, [pc, #84]                       /* @06BE 154a ldr r2, [pc, #84] literal 0x0714 -> 0x40002440 */
L_06c0:
    lsrs r1, r1, #27                        /* @06C0 c90e lsrs r1, r1, #27  */
L_06c2:
    cmp r0, #0                              /* @06C2 0028 cmp r0, #0  */
L_06c4:
    beq L_06d6                              /* @06C4 07d0 beq #14 -> 0x06D6 */
L_06c6:
    cmp r0, #1                              /* @06C6 0128 cmp r0, #1  */
L_06c8:
    beq L_06e2                              /* @06C8 0bd0 beq #22 -> 0x06E2 */
L_06ca:
    cmp r0, #2                              /* @06CA 0228 cmp r0, #2  */
L_06cc:
    beq L_06f2                              /* @06CC 11d0 beq #34 -> 0x06F2 */
L_06ce:
    cmp r0, #3                              /* @06CE 0328 cmp r0, #3  */
L_06d0:
    beq L_0702                              /* @06D0 17d0 beq #46 -> 0x0702 */
L_06d2:
    movs r0, #4                             /* @06D2 0420 movs r0, #4  */
L_06d4:
    bx lr                                   /* @06D4 7047 bx lr  */
L_06d6:
    ldr r0, [r2, #12]                       /* @06D6 d068 ldr r0, [r2, #12]  */
L_06d8:
    lsrs r0, r0, #5                         /* @06D8 4009 lsrs r0, r0, #5  */
L_06da:
    lsls r0, r0, #5                         /* @06DA 4001 lsls r0, r0, #5  */
L_06dc:
    orrs r0, r1                             /* @06DC 0843 orrs r0, r1  */
L_06de:
    str r0, [r2, #12]                       /* @06DE d060 str r0, [r2, #12]  */
L_06e0:
    b L_0710                                /* @06E0 16e0 b #44 -> 0x0710 */
L_06e2:
    ldr r0, [r2, #12]                       /* @06E2 d068 ldr r0, [r2, #12]  */
L_06e4:
    movs r3, #31                            /* @06E4 1f23 movs r3, #31  */
L_06e6:
    lsls r3, r3, #5                         /* @06E6 5b01 lsls r3, r3, #5  */
L_06e8:
    bics r0, r3                             /* @06E8 9843 bics r0, r3  */
L_06ea:
    lsls r1, r1, #5                         /* @06EA 4901 lsls r1, r1, #5  */
L_06ec:
    orrs r0, r1                             /* @06EC 0843 orrs r0, r1  */
L_06ee:
    str r0, [r2, #12]                       /* @06EE d060 str r0, [r2, #12]  */
L_06f0:
    b L_0710                                /* @06F0 0ee0 b #28 -> 0x0710 */
L_06f2:
    ldr r0, [r2, #12]                       /* @06F2 d068 ldr r0, [r2, #12]  */
L_06f4:
    movs r3, #31                            /* @06F4 1f23 movs r3, #31  */
L_06f6:
    lsls r3, r3, #10                        /* @06F6 9b02 lsls r3, r3, #10  */
L_06f8:
    bics r0, r3                             /* @06F8 9843 bics r0, r3  */
L_06fa:
    lsls r1, r1, #10                        /* @06FA 8902 lsls r1, r1, #10  */
L_06fc:
    orrs r0, r1                             /* @06FC 0843 orrs r0, r1  */
L_06fe:
    str r0, [r2, #12]                       /* @06FE d060 str r0, [r2, #12]  */
L_0700:
    b L_0710                                /* @0700 06e0 b #12 -> 0x0710 */
L_0702:
    ldr r0, [r2, #12]                       /* @0702 d068 ldr r0, [r2, #12]  */
L_0704:
    movs r3, #31                            /* @0704 1f23 movs r3, #31  */
L_0706:
    lsls r3, r3, #15                        /* @0706 db03 lsls r3, r3, #15  */
L_0708:
    bics r0, r3                             /* @0708 9843 bics r0, r3  */
L_070a:
    lsls r1, r1, #15                        /* @070A c903 lsls r1, r1, #15  */
L_070c:
    orrs r0, r1                             /* @070C 0843 orrs r0, r1  */
L_070e:
    str r0, [r2, #12]                       /* @070E d060 str r0, [r2, #12]  */
L_0710:
    movs r0, #0                             /* @0710 0020 movs r0, #0  */
L_0712:
    bx lr                                   /* @0712 7047 bx lr  */
L_0714:
    .byte 0x40,0x24,0x00,0x40 /* @0714 literal */

/* adc_clear_flags */
L_0718:
    ldr r1, [pc, #8]                        /* @0718 0249 ldr r1, [pc, #8] literal 0x0724 -> 0x40002480 */
L_071a:
    ldr r2, [r1, #52]                       /* @071A 4a6b ldr r2, [r1, #52]  */
L_071c:
    bics r2, r0                             /* @071C 8243 bics r2, r0  */
L_071e:
    str r2, [r1, #52]                       /* @071E 4a63 str r2, [r1, #52]  */
L_0720:
    bx lr                                   /* @0720 7047 bx lr  */
    .byte 0x00,0x00 /* @0722 unclassified */
L_0724:
    .byte 0x80,0x24,0x00,0x40 /* @0724 literal */

/* sub_0728 */
L_0728:
    ldr r0, [pc, #8]                        /* @0728 0248 ldr r0, [pc, #8] literal 0x0734 -> 0x40002400 */
L_072a:
    ldr r1, [r0, #4]                        /* @072A 4168 ldr r1, [r0, #4]  */
L_072c:
    movs r2, #1                             /* @072C 0122 movs r2, #1  */
L_072e:
    orrs r1, r2                             /* @072E 1143 orrs r1, r2  */
L_0730:
    str r1, [r0, #4]                        /* @0730 4160 str r1, [r0, #4]  */
L_0732:
    bx lr                                   /* @0732 7047 bx lr  */
L_0734:
    .byte 0x00,0x24,0x00,0x40 /* @0734 literal */

/* adc_test_flags */
L_0738:
    ldr r1, [pc, #12]                       /* @0738 0349 ldr r1, [pc, #12] literal 0x0748 -> 0x40002480 */
L_073a:
    ldr r1, [r1, #48]                       /* @073A 096b ldr r1, [r1, #48]  */
L_073c:
    tst r1, r0                              /* @073C 0142 tst r1, r0  */
L_073e:
    beq L_0744                              /* @073E 01d0 beq #2 -> 0x0744 */
L_0740:
    movs r0, #1                             /* @0740 0120 movs r0, #1  */
L_0742:
    bx lr                                   /* @0742 7047 bx lr  */
L_0744:
    movs r0, #0                             /* @0744 0020 movs r0, #0  */
L_0746:
    bx lr                                   /* @0746 7047 bx lr  */
L_0748:
    .byte 0x80,0x24,0x00,0x40 /* @0748 literal */

/* adc_read_result */
L_074c:
    ldr r1, [pc, #4]                        /* @074C 0149 ldr r1, [pc, #4] literal 0x0754 -> 0x40002490 */
L_074e:
    lsls r0, r0, #2                         /* @074E 8000 lsls r0, r0, #2  */
L_0750:
    ldr r0, [r1, r0]                        /* @0750 0858 ldr r0, [r1, r0]  */
L_0752:
    bx lr                                   /* @0752 7047 bx lr  */
L_0754:
    .byte 0x90,0x24,0x00,0x40 /* @0754 literal */

/* sub_0758 */
L_0758:
    push {r4, r5, r6, lr}                   /* @0758 70b5 push {r4, r5, r6, lr}  */
L_075a:
    mov r4, r0                              /* @075A 0446 mov r4, r0  */
L_075c:
    cmp r4, #0                              /* @075C 002c cmp r4, #0  */
L_075e:
    beq L_07a6                              /* @075E 22d0 beq #68 -> 0x07A6 */
L_0760:
    ldr r5, [pc, #72]                       /* @0760 124d ldr r5, [pc, #72] literal 0x07AC -> 0x40002400 */
L_0762:
    movs r0, #1                             /* @0762 0120 movs r0, #1  */
L_0764:
    str r0, [r5, #4]                        /* @0764 6860 str r0, [r5, #4]  */
L_0766:
    bl L_2d98                               /* @0766 02f017fb bl #9774 -> delay_100us_candidate */
L_076a:
    ldrb r0, [r4, #1]                       /* @076A 6078 ldrb r0, [r4, #1]  */
L_076c:
    ldrh r1, [r4, #4]                       /* @076C a188 ldrh r1, [r4, #4]  */
L_076e:
    ldrh r2, [r4, #2]                       /* @076E 6288 ldrh r2, [r4, #2]  */
L_0770:
    orrs r0, r1                             /* @0770 0843 orrs r0, r1  */
L_0772:
    ldrh r1, [r4, #6]                       /* @0772 e188 ldrh r1, [r4, #6]  */
L_0774:
    orrs r1, r2                             /* @0774 1143 orrs r1, r2  */
L_0776:
    orrs r0, r1                             /* @0776 0843 orrs r0, r1  */
L_0778:
    ldrh r1, [r4, #8]                       /* @0778 2189 ldrh r1, [r4, #8]  */
L_077a:
    orrs r0, r1                             /* @077A 0843 orrs r0, r1  */
L_077c:
    ldr r1, [r5, #4]                        /* @077C 6968 ldr r1, [r5, #4]  */
L_077e:
    orrs r0, r1                             /* @077E 0843 orrs r0, r1  */
L_0780:
    str r0, [r5, #4]                        /* @0780 6860 str r0, [r5, #4]  */
L_0782:
    ldr r0, [r5, #8]                        /* @0782 a868 ldr r0, [r5, #8]  */
L_0784:
    asrs r1, r5, #20                        /* @0784 2915 asrs r1, r5, #20  */
L_0786:
    bics r0, r1                             /* @0786 8843 bics r0, r1  */
L_0788:
    ldrb r1, [r4]                           /* @0788 2178 ldrb r1, [r4]  */
L_078a:
    lsls r1, r1, #31                        /* @078A c907 lsls r1, r1, #31  */
L_078c:
    lsrs r1, r1, #21                        /* @078C 490d lsrs r1, r1, #21  */
L_078e:
    orrs r0, r1                             /* @078E 0843 orrs r0, r1  */
L_0790:
    str r0, [r5, #8]                        /* @0790 a860 str r0, [r5, #8]  */
L_0792:
    ldr r0, [r5, #8]                        /* @0792 a868 ldr r0, [r5, #8]  */
L_0794:
    movs r1, #4                             /* @0794 0421 movs r1, #4  */
L_0796:
    bics r0, r1                             /* @0796 8843 bics r0, r1  */
L_0798:
    ldrb r1, [r4, #10]                      /* @0798 a17a ldrb r1, [r4, #10]  */
L_079a:
    lsls r1, r1, #31                        /* @079A c907 lsls r1, r1, #31  */
L_079c:
    lsrs r1, r1, #29                        /* @079C 490f lsrs r1, r1, #29  */
L_079e:
    orrs r0, r1                             /* @079E 0843 orrs r0, r1  */
L_07a0:
    str r0, [r5, #8]                        /* @07A0 a860 str r0, [r5, #8]  */
L_07a2:
    movs r0, #0                             /* @07A2 0020 movs r0, #0  */
L_07a4:
    pop {r4, r5, r6, pc}                    /* @07A4 70bd pop {r4, r5, r6, pc}  */
L_07a6:
    movs r0, #4                             /* @07A6 0420 movs r0, #4  */
L_07a8:
    pop {r4, r5, r6, pc}                    /* @07A8 70bd pop {r4, r5, r6, pc}  */
    .byte 0x00,0x00 /* @07AA unclassified */
L_07ac:
    .byte 0x00,0x24,0x00,0x40 /* @07AC literal */

/* adc_start */
L_07b0:
    ldr r1, [pc, #4]                        /* @07B0 0149 ldr r1, [pc, #4] literal 0x07B8 -> 0x400024C0 */
L_07b2:
    movs r0, #1                             /* @07B2 0120 movs r0, #1  */
L_07b4:
    str r0, [r1, #8]                        /* @07B4 8860 str r0, [r1, #8]  */
L_07b6:
    bx lr                                   /* @07B6 7047 bx lr  */
L_07b8:
    .byte 0xc0,0x24,0x00,0x40 /* @07B8 literal */

/* adc_stop */
L_07bc:
    ldr r1, [pc, #4]                        /* @07BC 0149 ldr r1, [pc, #4] literal 0x07C4 -> 0x400024C0 */
L_07be:
    movs r0, #0                             /* @07BE 0020 movs r0, #0  */
L_07c0:
    str r0, [r1, #8]                        /* @07C0 8860 str r0, [r1, #8]  */
L_07c2:
    bx lr                                   /* @07C2 7047 bx lr  */
L_07c4:
    .byte 0xc0,0x24,0x00,0x40 /* @07C4 literal */

/* sub_07C8 */
L_07c8:
    ldr r2, [pc, #20]                       /* @07C8 054a ldr r2, [pc, #20] literal 0x07E0 -> 0x40002480 */
L_07ca:
    cmp r1, #1                              /* @07CA 0129 cmp r1, #1  */
L_07cc:
    beq L_07d6                              /* @07CC 03d0 beq #6 -> 0x07D6 */
L_07ce:
    ldr r1, [r2, #60]                       /* @07CE d16b ldr r1, [r2, #60]  */
L_07d0:
    bics r1, r0                             /* @07D0 8143 bics r1, r0  */
L_07d2:
    str r1, [r2, #60]                       /* @07D2 d163 str r1, [r2, #60]  */
L_07d4:
    bx lr                                   /* @07D4 7047 bx lr  */
L_07d6:
    ldr r1, [r2, #60]                       /* @07D6 d16b ldr r1, [r2, #60]  */
L_07d8:
    orrs r1, r0                             /* @07D8 0143 orrs r1, r0  */
L_07da:
    str r1, [r2, #60]                       /* @07DA d163 str r1, [r2, #60]  */
L_07dc:
    bx lr                                   /* @07DC 7047 bx lr  */
    .byte 0x00,0x00 /* @07DE unclassified */
L_07e0:
    .byte 0x80,0x24,0x00,0x40 /* @07E0 literal */

/* sub_07E4 */
L_07e4:
    cmp r0, #0                              /* @07E4 0028 cmp r0, #0  */
L_07e6:
    beq L_07ee                              /* @07E6 02d0 beq #4 -> 0x07EE */
L_07e8:
    ldrb r1, [r0]                           /* @07E8 0178 ldrb r1, [r0]  */
L_07ea:
    cmp r1, #4                              /* @07EA 0429 cmp r1, #4  */
L_07ec:
    bls L_07f2                              /* @07EC 01d9 bls #2 -> 0x07F2 */
L_07ee:
    movs r0, #4                             /* @07EE 0420 movs r0, #4  */
L_07f0:
    bx lr                                   /* @07F0 7047 bx lr  */
L_07f2:
    ldr r1, [pc, #44]                       /* @07F2 0b49 ldr r1, [pc, #44] literal 0x0820 -> 0x40002400 */
L_07f4:
    ldr r2, [r1, #8]                        /* @07F4 8a68 ldr r2, [r1, #8]  */
L_07f6:
    asrs r3, r1, #21                        /* @07F6 4b15 asrs r3, r1, #21  */
L_07f8:
    bics r2, r3                             /* @07F8 9a43 bics r2, r3  */
L_07fa:
    ldrb r3, [r0, #1]                       /* @07FA 4378 ldrb r3, [r0, #1]  */
L_07fc:
    lsls r3, r3, #31                        /* @07FC db07 lsls r3, r3, #31  */
L_07fe:
    lsrs r3, r3, #22                        /* @07FE 9b0d lsrs r3, r3, #22  */
L_0800:
    orrs r2, r3                             /* @0800 1a43 orrs r2, r3  */
L_0802:
    str r2, [r1, #8]                        /* @0802 8a60 str r2, [r1, #8]  */
L_0804:
    ldr r1, [pc, #24]                       /* @0804 0649 ldr r1, [pc, #24] literal 0x0820 -> 0x40002400 */
L_0806:
    adds r1, #64                            /* @0806 4031 adds r1, #64  */
L_0808:
    ldr r2, [r1, #12]                       /* @0808 ca68 ldr r2, [r1, #12]  */
L_080a:
    ldrb r0, [r0]                           /* @080A 0078 ldrb r0, [r0]  */
L_080c:
    movs r3, #3                             /* @080C 0323 movs r3, #3  */
L_080e:
    lsls r3, r3, #20                        /* @080E 1b05 lsls r3, r3, #20  */
L_0810:
    subs r0, r0, #1                         /* @0810 401e subs r0, r0, #1  */
L_0812:
    lsls r0, r0, #30                        /* @0812 8007 lsls r0, r0, #30  */
L_0814:
    bics r2, r3                             /* @0814 9a43 bics r2, r3  */
L_0816:
    lsrs r0, r0, #10                        /* @0816 800a lsrs r0, r0, #10  */
L_0818:
    orrs r2, r0                             /* @0818 0243 orrs r2, r0  */
L_081a:
    str r2, [r1, #12]                       /* @081A ca60 str r2, [r1, #12]  */
L_081c:
    movs r0, #0                             /* @081C 0020 movs r0, #0  */
L_081e:
    bx lr                                   /* @081E 7047 bx lr  */
L_0820:
    .byte 0x00,0x24,0x00,0x40 /* @0820 literal */

/* sub_0824 */
L_0824:
    push {r4, lr}                           /* @0824 10b5 push {r4, lr}  */
L_0826:
    cmp r1, #0                              /* @0826 0029 cmp r1, #0  */
L_0828:
    beq L_0834                              /* @0828 04d0 beq #8 -> 0x0834 */
L_082a:
    ldrb r2, [r1]                           /* @082A 0a78 ldrb r2, [r1]  */
L_082c:
    cmp r2, #5                              /* @082C 052a cmp r2, #5  */
L_082e:
    bls L_0838                              /* @082E 03d9 bls #6 -> 0x0838 */
L_0830:
    movs r0, #4                             /* @0830 0420 movs r0, #4  */
L_0832:
    pop {r4, pc}                            /* @0832 10bd pop {r4, pc}  */
L_0834:
    movs r0, #4                             /* @0834 0420 movs r0, #4  */
L_0836:
    pop {r4, pc}                            /* @0836 10bd pop {r4, pc}  */
L_0838:
    ldr r3, [r0, #80]                       /* @0838 036d ldr r3, [r0, #80]  */
L_083a:
    movs r4, #14                            /* @083A 0e24 movs r4, #14  */
L_083c:
    lsls r2, r2, #29                        /* @083C 5207 lsls r2, r2, #29  */
L_083e:
    bics r3, r4                             /* @083E a343 bics r3, r4  */
L_0840:
    lsrs r2, r2, #28                        /* @0840 120f lsrs r2, r2, #28  */
L_0842:
    orrs r3, r2                             /* @0842 1343 orrs r3, r2  */
L_0844:
    str r3, [r0, #80]                       /* @0844 0365 str r3, [r0, #80]  */
L_0846:
    ldr r2, [r0, #80]                       /* @0846 026d ldr r2, [r0, #80]  */
L_0848:
    movs r3, #255                           /* @0848 ff23 movs r3, #255  */
L_084a:
    adds r3, #1                             /* @084A 0133 adds r3, #1  */
L_084c:
    bics r2, r3                             /* @084C 9a43 bics r2, r3  */
L_084e:
    ldrb r3, [r1, #1]                       /* @084E 4b78 ldrb r3, [r1, #1]  */
L_0850:
    lsls r3, r3, #31                        /* @0850 db07 lsls r3, r3, #31  */
L_0852:
    lsrs r3, r3, #23                        /* @0852 db0d lsrs r3, r3, #23  */
L_0854:
    orrs r2, r3                             /* @0854 1a43 orrs r2, r3  */
L_0856:
    str r2, [r0, #80]                       /* @0856 0265 str r2, [r0, #80]  */
L_0858:
    ldr r2, [r0, #80]                       /* @0858 026d ldr r2, [r0, #80]  */
L_085a:
    ldrb r1, [r1, #2]                       /* @085A 8978 ldrb r1, [r1, #2]  */
L_085c:
    movs r3, #112                           /* @085C 7023 movs r3, #112  */
L_085e:
    lsls r1, r1, #29                        /* @085E 4907 lsls r1, r1, #29  */
L_0860:
    bics r2, r3                             /* @0860 9a43 bics r2, r3  */
L_0862:
    lsrs r1, r1, #25                        /* @0862 490e lsrs r1, r1, #25  */
L_0864:
    orrs r2, r1                             /* @0864 0a43 orrs r2, r1  */
L_0866:
    str r2, [r0, #80]                       /* @0866 0265 str r2, [r0, #80]  */
L_0868:
    movs r0, #0                             /* @0868 0020 movs r0, #0  */
L_086a:
    pop {r4, pc}                            /* @086A 10bd pop {r4, pc}  */

/* sub_086C */
L_086c:
    ldr r2, [r0]                            /* @086C 0268 ldr r2, [r0]  */
L_086e:
    lsrs r2, r2, #16                        /* @086E 120c lsrs r2, r2, #16  */
L_0870:
    lsls r2, r2, #16                        /* @0870 1204 lsls r2, r2, #16  */
L_0872:
    orrs r2, r1                             /* @0872 0a43 orrs r2, r1  */
L_0874:
    str r2, [r0]                            /* @0874 0260 str r2, [r0]  */
L_0876:
    movs r0, #0                             /* @0876 0020 movs r0, #0  */
L_0878:
    bx lr                                   /* @0878 7047 bx lr  */

/* sub_087A */
L_087a:
    str r1, [r0, #4]                        /* @087A 4160 str r1, [r0, #4]  */
L_087c:
    movs r0, #0                             /* @087C 0020 movs r0, #0  */
L_087e:
    bx lr                                   /* @087E 7047 bx lr  */

/* sub_0880 */
L_0880:
    ldr r1, [r0, #80]                       /* @0880 016d ldr r1, [r0, #80]  */
L_0882:
    movs r2, #1                             /* @0882 0122 movs r2, #1  */
L_0884:
    orrs r1, r2                             /* @0884 1143 orrs r1, r2  */
L_0886:
    str r1, [r0, #80]                       /* @0886 0165 str r1, [r0, #80]  */
L_0888:
    movs r0, #0                             /* @0888 0020 movs r0, #0  */
L_088a:
    bx lr                                   /* @088A 7047 bx lr  */

/* sub_088C */
L_088c:
    ldr r1, [r0, #80]                       /* @088C 016d ldr r1, [r0, #80]  */
L_088e:
    lsrs r1, r1, #1                         /* @088E 4908 lsrs r1, r1, #1  */
L_0890:
    lsls r1, r1, #1                         /* @0890 4900 lsls r1, r1, #1  */
L_0892:
    str r1, [r0, #80]                       /* @0892 0165 str r1, [r0, #80]  */
L_0894:
    movs r0, #0                             /* @0894 0020 movs r0, #0  */
L_0896:
    bx lr                                   /* @0896 7047 bx lr  */

/* sub_0898 */
L_0898:
    push {r4, lr}                           /* @0898 10b5 push {r4, lr}  */
L_089a:
    ldr r0, [pc, #16]                       /* @089A 0448 ldr r0, [pc, #16] literal 0x08AC -> 0x40002400 */
L_089c:
    ldr r1, [r0]                            /* @089C 0168 ldr r1, [r0]  */
L_089e:
    movs r2, #1                             /* @089E 0122 movs r2, #1  */
L_08a0:
    orrs r1, r2                             /* @08A0 1143 orrs r1, r2  */
L_08a2:
    str r1, [r0]                            /* @08A2 0160 str r1, [r0]  */
L_08a4:
    movs r0, #1                             /* @08A4 0120 movs r0, #1  */
L_08a6:
    bl L_2d98                               /* @08A6 02f077fa bl #9454 -> delay_100us_candidate */
L_08aa:
    pop {r4, pc}                            /* @08AA 10bd pop {r4, pc}  */
L_08ac:
    .byte 0x00,0x24,0x00,0x40 /* @08AC literal */

/* sub_08B0 */
L_08b0:
    ldr r1, [pc, #8]                        /* @08B0 0249 ldr r1, [pc, #8] literal 0x08BC -> 0x40000C00 */
L_08b2:
    lsls r0, r0, #8                         /* @08B2 0002 lsls r0, r0, #8  */
L_08b4:
    adds r1, r0, r1                         /* @08B4 4118 adds r1, r0, r1  */
L_08b6:
    movs r0, #0                             /* @08B6 0020 movs r0, #0  */
L_08b8:
    str r0, [r1, #20]                       /* @08B8 4861 str r0, [r1, #20]  */
L_08ba:
    bx lr                                   /* @08BA 7047 bx lr  */
L_08bc:
    .byte 0x00,0x0c,0x00,0x40 /* @08BC literal */

/* sub_08C0 */
L_08c0:
    mov r2, r0                              /* @08C0 0246 mov r2, r0  */
L_08c2:
    movs r0, #0                             /* @08C2 0020 movs r0, #0  */
L_08c4:
    ldr r3, [pc, #12]                       /* @08C4 034b ldr r3, [pc, #12] literal 0x08D4 -> 0x40000C00 */
L_08c6:
    lsls r2, r2, #8                         /* @08C6 1202 lsls r2, r2, #8  */
L_08c8:
    adds r2, r2, r3                         /* @08C8 d218 adds r2, r2, r3  */
L_08ca:
    movs r3, #1                             /* @08CA 0123 movs r3, #1  */
L_08cc:
    lsls r3, r1                             /* @08CC 8b40 lsls r3, r1  */
L_08ce:
    mvns r1, r3                             /* @08CE d943 mvns r1, r3  */
L_08d0:
    str r1, [r2, #20]                       /* @08D0 5161 str r1, [r2, #20]  */
L_08d2:
    bx lr                                   /* @08D2 7047 bx lr  */
L_08d4:
    .byte 0x00,0x0c,0x00,0x40 /* @08D4 literal */

/* sub_08D8 */
L_08d8:
    ldr r2, [pc, #12]                       /* @08D8 034a ldr r2, [pc, #12] literal 0x08E8 -> 0x40000C00 */
L_08da:
    lsls r0, r0, #8                         /* @08DA 0002 lsls r0, r0, #8  */
L_08dc:
    adds r0, r0, r2                         /* @08DC 8018 adds r0, r0, r2  */
L_08de:
    ldr r0, [r0, #16]                       /* @08DE 0069 ldr r0, [r0, #16]  */
L_08e0:
    lsrs r0, r1                             /* @08E0 c840 lsrs r0, r1  */
L_08e2:
    lsls r0, r0, #31                        /* @08E2 c007 lsls r0, r0, #31  */
L_08e4:
    lsrs r0, r0, #31                        /* @08E4 c00f lsrs r0, r0, #31  */
L_08e6:
    bx lr                                   /* @08E6 7047 bx lr  */
L_08e8:
    .byte 0x00,0x0c,0x00,0x40 /* @08E8 literal */

/* sub_08EC */
L_08ec:
    mov r2, r0                              /* @08EC 0246 mov r2, r0  */
L_08ee:
    movs r0, #0                             /* @08EE 0020 movs r0, #0  */
L_08f0:
    ldr r3, [pc, #16]                       /* @08F0 044b ldr r3, [pc, #16] literal 0x0904 -> 0x40000C00 */
L_08f2:
    lsls r2, r2, #8                         /* @08F2 1202 lsls r2, r2, #8  */
L_08f4:
    adds r2, r2, r3                         /* @08F4 d218 adds r2, r2, r3  */
L_08f6:
    ldr r3, [r2]                            /* @08F6 1368 ldr r3, [r2]  */
L_08f8:
    lsrs r3, r3, #16                        /* @08F8 1b0c lsrs r3, r3, #16  */
L_08fa:
    lsls r3, r3, #16                        /* @08FA 1b04 lsls r3, r3, #16  */
L_08fc:
    orrs r3, r1                             /* @08FC 0b43 orrs r3, r1  */
L_08fe:
    str r3, [r2]                            /* @08FE 1360 str r3, [r2]  */
L_0900:
    bx lr                                   /* @0900 7047 bx lr  */
    .byte 0x00,0x00 /* @0902 unclassified */
L_0904:
    .byte 0x00,0x0c,0x00,0x40 /* @0904 literal */

/* sub_0908 */
L_0908:
    mov r2, r0                              /* @0908 0246 mov r2, r0  */
L_090a:
    movs r0, #0                             /* @090A 0020 movs r0, #0  */
L_090c:
    ldr r3, [pc, #16]                       /* @090C 044b ldr r3, [pc, #16] literal 0x0920 -> 0x40000C00 */
L_090e:
    lsls r2, r2, #8                         /* @090E 1202 lsls r2, r2, #8  */
L_0910:
    adds r2, r2, r3                         /* @0910 d218 adds r2, r2, r3  */
L_0912:
    ldr r3, [r2, #4]                        /* @0912 5368 ldr r3, [r2, #4]  */
L_0914:
    lsrs r3, r3, #16                        /* @0914 1b0c lsrs r3, r3, #16  */
L_0916:
    lsls r3, r3, #16                        /* @0916 1b04 lsls r3, r3, #16  */
L_0918:
    orrs r3, r1                             /* @0918 0b43 orrs r3, r1  */
L_091a:
    str r3, [r2, #4]                        /* @091A 5360 str r3, [r2, #4]  */
L_091c:
    bx lr                                   /* @091C 7047 bx lr  */
    .byte 0x00,0x00 /* @091E unclassified */
L_0920:
    .byte 0x00,0x0c,0x00,0x40 /* @0920 literal */

/* sub_0924 */
L_0924:
    mov r1, r0                              /* @0924 0146 mov r1, r0  */
L_0926:
    movs r0, #0                             /* @0926 0020 movs r0, #0  */
L_0928:
    ldr r2, [pc, #12]                       /* @0928 034a ldr r2, [pc, #12] literal 0x0938 -> 0x40000C00 */
L_092a:
    lsls r1, r1, #8                         /* @092A 0902 lsls r1, r1, #8  */
L_092c:
    adds r1, r1, r2                         /* @092C 8918 adds r1, r1, r2  */
L_092e:
    ldr r2, [r1, #12]                       /* @092E ca68 ldr r2, [r1, #12]  */
L_0930:
    movs r3, #1                             /* @0930 0123 movs r3, #1  */
L_0932:
    orrs r2, r3                             /* @0932 1a43 orrs r2, r3  */
L_0934:
    str r2, [r1, #12]                       /* @0934 ca60 str r2, [r1, #12]  */
L_0936:
    bx lr                                   /* @0936 7047 bx lr  */
L_0938:
    .byte 0x00,0x0c,0x00,0x40 /* @0938 literal */

/* sub_093C */
L_093c:
    mov r1, r0                              /* @093C 0146 mov r1, r0  */
L_093e:
    movs r0, #0                             /* @093E 0020 movs r0, #0  */
L_0940:
    ldr r2, [pc, #12]                       /* @0940 034a ldr r2, [pc, #12] literal 0x0950 -> 0x40000C00 */
L_0942:
    lsls r1, r1, #8                         /* @0942 0902 lsls r1, r1, #8  */
L_0944:
    adds r1, r1, r2                         /* @0944 8918 adds r1, r1, r2  */
L_0946:
    ldr r2, [r1, #12]                       /* @0946 ca68 ldr r2, [r1, #12]  */
L_0948:
    lsrs r2, r2, #1                         /* @0948 5208 lsrs r2, r2, #1  */
L_094a:
    lsls r2, r2, #1                         /* @094A 5200 lsls r2, r2, #1  */
L_094c:
    str r2, [r1, #12]                       /* @094C ca60 str r2, [r1, #12]  */
L_094e:
    bx lr                                   /* @094E 7047 bx lr  */
L_0950:
    .byte 0x00,0x0c,0x00,0x40 /* @0950 literal */

/* sub_0954 */
L_0954:
    push {r4, lr}                           /* @0954 10b5 push {r4, lr}  */
L_0956:
    mov r3, r0                              /* @0956 0346 mov r3, r0  */
L_0958:
    movs r0, #0                             /* @0958 0020 movs r0, #0  */
L_095a:
    ldr r4, [pc, #32]                       /* @095A 084c ldr r4, [pc, #32] literal 0x097C -> 0x40000C00 */
L_095c:
    lsls r3, r3, #8                         /* @095C 1b02 lsls r3, r3, #8  */
L_095e:
    adds r3, r3, r4                         /* @095E 1b19 adds r3, r3, r4  */
L_0960:
    ldr r4, [r3]                            /* @0960 1c68 ldr r4, [r3]  */
L_0962:
    lsrs r4, r4, #16                        /* @0962 240c lsrs r4, r4, #16  */
L_0964:
    lsls r4, r4, #16                        /* @0964 2404 lsls r4, r4, #16  */
L_0966:
    orrs r4, r1                             /* @0966 0c43 orrs r4, r1  */
L_0968:
    str r4, [r3]                            /* @0968 1c60 str r4, [r3]  */
L_096a:
    ldr r1, [r3, #12]                       /* @096A d968 ldr r1, [r3, #12]  */
L_096c:
    movs r4, #128                           /* @096C 8024 movs r4, #128  */
L_096e:
    lsls r2, r2, #31                        /* @096E d207 lsls r2, r2, #31  */
L_0970:
    bics r1, r4                             /* @0970 a143 bics r1, r4  */
L_0972:
    lsrs r2, r2, #24                        /* @0972 120e lsrs r2, r2, #24  */
L_0974:
    orrs r1, r2                             /* @0974 1143 orrs r1, r2  */
L_0976:
    str r1, [r3, #12]                       /* @0976 d960 str r1, [r3, #12]  */
L_0978:
    pop {r4, pc}                            /* @0978 10bd pop {r4, pc}  */
    .byte 0x00,0x00 /* @097A unclassified */
L_097c:
    .byte 0x00,0x0c,0x00,0x40 /* @097C literal */

/* sub_0980 */
L_0980:
    push {r4, lr}                           /* @0980 10b5 push {r4, lr}  */
L_0982:
    mov r3, r0                              /* @0982 0346 mov r3, r0  */
L_0984:
    movs r0, #0                             /* @0984 0020 movs r0, #0  */
L_0986:
    ldr r4, [pc, #40]                       /* @0986 0a4c ldr r4, [pc, #40] literal 0x09B0 -> 0x40000C00 */
L_0988:
    lsls r3, r3, #8                         /* @0988 1b02 lsls r3, r3, #8  */
L_098a:
    adds r3, r3, r4                         /* @098A 1b19 adds r3, r3, r4  */
L_098c:
    cmp r1, #0                              /* @098C 0029 cmp r1, #0  */
L_098e:
    beq L_0998                              /* @098E 03d0 beq #6 -> 0x0998 */
L_0990:
    cmp r1, #1                              /* @0990 0129 cmp r1, #1  */
L_0992:
    beq L_09a4                              /* @0992 07d0 beq #14 -> 0x09A4 */
L_0994:
    movs r0, #1                             /* @0994 0120 movs r0, #1  */
L_0996:
    pop {r4, pc}                            /* @0996 10bd pop {r4, pc}  */
L_0998:
    ldr r1, [r3, #60]                       /* @0998 d96b ldr r1, [r3, #60]  */
L_099a:
    lsrs r1, r1, #16                        /* @099A 090c lsrs r1, r1, #16  */
L_099c:
    lsls r1, r1, #16                        /* @099C 0904 lsls r1, r1, #16  */
L_099e:
    orrs r1, r2                             /* @099E 1143 orrs r1, r2  */
L_09a0:
    str r1, [r3, #60]                       /* @09A0 d963 str r1, [r3, #60]  */
L_09a2:
    pop {r4, pc}                            /* @09A2 10bd pop {r4, pc}  */
L_09a4:
    ldr r1, [r3, #64]                       /* @09A4 196c ldr r1, [r3, #64]  */
L_09a6:
    lsrs r1, r1, #16                        /* @09A6 090c lsrs r1, r1, #16  */
L_09a8:
    lsls r1, r1, #16                        /* @09A8 0904 lsls r1, r1, #16  */
L_09aa:
    orrs r1, r2                             /* @09AA 1143 orrs r1, r2  */
L_09ac:
    str r1, [r3, #64]                       /* @09AC 1964 str r1, [r3, #64]  */
L_09ae:
    pop {r4, pc}                            /* @09AE 10bd pop {r4, pc}  */
L_09b0:
    .byte 0x00,0x0c,0x00,0x40 /* @09B0 literal */

/* sub_09B4 */
L_09b4:
    mov r2, r0                              /* @09B4 0246 mov r2, r0  */
L_09b6:
    movs r0, #0                             /* @09B6 0020 movs r0, #0  */
L_09b8:
    ldr r3, [pc, #16]                       /* @09B8 044b ldr r3, [pc, #16] literal 0x09CC -> 0x40000C00 */
L_09ba:
    lsls r2, r2, #8                         /* @09BA 1202 lsls r2, r2, #8  */
L_09bc:
    adds r2, r2, r3                         /* @09BC d218 adds r2, r2, r3  */
L_09be:
    ldr r3, [r2, #4]                        /* @09BE 5368 ldr r3, [r2, #4]  */
L_09c0:
    lsrs r3, r3, #16                        /* @09C0 1b0c lsrs r3, r3, #16  */
L_09c2:
    lsls r3, r3, #16                        /* @09C2 1b04 lsls r3, r3, #16  */
L_09c4:
    orrs r3, r1                             /* @09C4 0b43 orrs r3, r1  */
L_09c6:
    str r3, [r2, #4]                        /* @09C6 5360 str r3, [r2, #4]  */
L_09c8:
    bx lr                                   /* @09C8 7047 bx lr  */
    .byte 0x00,0x00 /* @09CA unclassified */
L_09cc:
    .byte 0x00,0x0c,0x00,0x40 /* @09CC literal */

/* sub_09D0 */
L_09d0:
    push {r4, r5, lr}                       /* @09D0 30b5 push {r4, r5, lr}  */
L_09d2:
    mov r3, r0                              /* @09D2 0346 mov r3, r0  */
L_09d4:
    movs r0, #0                             /* @09D4 0020 movs r0, #0  */
L_09d6:
    ldr r4, [pc, #36]                       /* @09D6 094c ldr r4, [pc, #36] literal 0x09FC -> 0x40000C00 */
L_09d8:
    lsls r3, r3, #8                         /* @09D8 1b02 lsls r3, r3, #8  */
L_09da:
    adds r3, r3, r4                         /* @09DA 1b19 adds r3, r3, r4  */
L_09dc:
    ldr r4, [r3, #48]                       /* @09DC 1c6b ldr r4, [r3, #48]  */
L_09de:
    movs r5, #1                             /* @09DE 0125 movs r5, #1  */
L_09e0:
    lsls r5, r5, #12                        /* @09E0 2d03 lsls r5, r5, #12  */
L_09e2:
    lsls r1, r1, #31                        /* @09E2 c907 lsls r1, r1, #31  */
L_09e4:
    bics r4, r5                             /* @09E4 ac43 bics r4, r5  */
L_09e6:
    lsrs r1, r1, #19                        /* @09E6 c90c lsrs r1, r1, #19  */
L_09e8:
    orrs r4, r1                             /* @09E8 0c43 orrs r4, r1  */
L_09ea:
    str r4, [r3, #48]                       /* @09EA 1c63 str r4, [r3, #48]  */
L_09ec:
    ldr r1, [r3, #48]                       /* @09EC 196b ldr r1, [r3, #48]  */
L_09ee:
    asrs r4, r5, #1                         /* @09EE 6c10 asrs r4, r5, #1  */
L_09f0:
    lsls r2, r2, #31                        /* @09F0 d207 lsls r2, r2, #31  */
L_09f2:
    bics r1, r4                             /* @09F2 a143 bics r1, r4  */
L_09f4:
    lsrs r2, r2, #20                        /* @09F4 120d lsrs r2, r2, #20  */
L_09f6:
    orrs r1, r2                             /* @09F6 1143 orrs r1, r2  */
L_09f8:
    str r1, [r3, #48]                       /* @09F8 1963 str r1, [r3, #48]  */
L_09fa:
    pop {r4, r5, pc}                        /* @09FA 30bd pop {r4, r5, pc}  */
L_09fc:
    .byte 0x00,0x0c,0x00,0x40 /* @09FC literal */

/* sub_0A00 */
L_0a00:
    push {r4, lr}                           /* @0A00 10b5 push {r4, lr}  */
L_0a02:
    mov r2, r0                              /* @0A02 0246 mov r2, r0  */
L_0a04:
    movs r0, #0                             /* @0A04 0020 movs r0, #0  */
L_0a06:
    ldr r3, [pc, #72]                       /* @0A06 124b ldr r3, [pc, #72] literal 0x0A50 -> 0x40000C00 */
L_0a08:
    lsls r2, r2, #8                         /* @0A08 1202 lsls r2, r2, #8  */
L_0a0a:
    adds r2, r2, r3                         /* @0A0A d218 adds r2, r2, r3  */
L_0a0c:
    ldr r3, [r2, #24]                       /* @0A0C 9369 ldr r3, [r2, #24]  */
L_0a0e:
    movs r4, #16                            /* @0A0E 1024 movs r4, #16  */
L_0a10:
    bics r3, r4                             /* @0A10 a343 bics r3, r4  */
L_0a12:
    ldrb r4, [r1]                           /* @0A12 0c78 ldrb r4, [r1]  */
L_0a14:
    lsls r4, r4, #31                        /* @0A14 e407 lsls r4, r4, #31  */
L_0a16:
    lsrs r4, r4, #27                        /* @0A16 e40e lsrs r4, r4, #27  */
L_0a18:
    orrs r3, r4                             /* @0A18 2343 orrs r3, r4  */
L_0a1a:
    str r3, [r2, #24]                       /* @0A1A 9361 str r3, [r2, #24]  */
L_0a1c:
    ldr r3, [r2, #24]                       /* @0A1C 9369 ldr r3, [r2, #24]  */
L_0a1e:
    ldrb r4, [r1, #1]                       /* @0A1E 4c78 ldrb r4, [r1, #1]  */
L_0a20:
    lsrs r3, r3, #3                         /* @0A20 db08 lsrs r3, r3, #3  */
L_0a22:
    lsls r3, r3, #3                         /* @0A22 db00 lsls r3, r3, #3  */
L_0a24:
    lsls r4, r4, #29                        /* @0A24 6407 lsls r4, r4, #29  */
L_0a26:
    lsrs r4, r4, #29                        /* @0A26 640f lsrs r4, r4, #29  */
L_0a28:
    orrs r3, r4                             /* @0A28 2343 orrs r3, r4  */
L_0a2a:
    str r3, [r2, #24]                       /* @0A2A 9361 str r3, [r2, #24]  */
L_0a2c:
    ldr r3, [r2, #24]                       /* @0A2C 9369 ldr r3, [r2, #24]  */
L_0a2e:
    movs r4, #7                             /* @0A2E 0724 movs r4, #7  */
L_0a30:
    lsls r4, r4, #8                         /* @0A30 2402 lsls r4, r4, #8  */
L_0a32:
    bics r3, r4                             /* @0A32 a343 bics r3, r4  */
L_0a34:
    ldrb r4, [r1, #2]                       /* @0A34 8c78 ldrb r4, [r1, #2]  */
L_0a36:
    lsls r4, r4, #29                        /* @0A36 6407 lsls r4, r4, #29  */
L_0a38:
    lsrs r4, r4, #21                        /* @0A38 640d lsrs r4, r4, #21  */
L_0a3a:
    orrs r3, r4                             /* @0A3A 2343 orrs r3, r4  */
L_0a3c:
    str r3, [r2, #24]                       /* @0A3C 9361 str r3, [r2, #24]  */
L_0a3e:
    ldr r3, [r2, #24]                       /* @0A3E 9369 ldr r3, [r2, #24]  */
L_0a40:
    ldrb r1, [r1, #3]                       /* @0A40 c978 ldrb r1, [r1, #3]  */
L_0a42:
    movs r4, #224                           /* @0A42 e024 movs r4, #224  */
L_0a44:
    lsls r1, r1, #29                        /* @0A44 4907 lsls r1, r1, #29  */
L_0a46:
    bics r3, r4                             /* @0A46 a343 bics r3, r4  */
L_0a48:
    lsrs r1, r1, #24                        /* @0A48 090e lsrs r1, r1, #24  */
L_0a4a:
    orrs r3, r1                             /* @0A4A 0b43 orrs r3, r1  */
L_0a4c:
    str r3, [r2, #24]                       /* @0A4C 9361 str r3, [r2, #24]  */
L_0a4e:
    pop {r4, pc}                            /* @0A4E 10bd pop {r4, pc}  */
L_0a50:
    .byte 0x00,0x0c,0x00,0x40 /* @0A50 literal */

/* sub_0A54 */
L_0a54:
    push {r4, r5, lr}                       /* @0A54 30b5 push {r4, r5, lr}  */
L_0a56:
    mov r2, r0                              /* @0A56 0246 mov r2, r0  */
L_0a58:
    movs r0, #0                             /* @0A58 0020 movs r0, #0  */
L_0a5a:
    ldr r3, [pc, #152]                      /* @0A5A 264b ldr r3, [pc, #152] literal 0x0AF4 -> 0x40000C00 */
L_0a5c:
    lsls r2, r2, #8                         /* @0A5C 1202 lsls r2, r2, #8  */
L_0a5e:
    adds r2, r2, r3                         /* @0A5E d218 adds r2, r2, r3  */
L_0a60:
    ldr r3, [r2, #36]                       /* @0A60 536a ldr r3, [r2, #36]  */
L_0a62:
    movs r4, #16                            /* @0A62 1024 movs r4, #16  */
L_0a64:
    bics r3, r4                             /* @0A64 a343 bics r3, r4  */
L_0a66:
    str r3, [r2, #36]                       /* @0A66 5362 str r3, [r2, #36]  */
L_0a68:
    ldr r3, [r2, #28]                       /* @0A68 d369 ldr r3, [r2, #28]  */
L_0a6a:
    ldrb r4, [r1, #1]                       /* @0A6A 4c78 ldrb r4, [r1, #1]  */
L_0a6c:
    lsrs r3, r3, #3                         /* @0A6C db08 lsrs r3, r3, #3  */
L_0a6e:
    lsls r3, r3, #3                         /* @0A6E db00 lsls r3, r3, #3  */
L_0a70:
    lsls r4, r4, #29                        /* @0A70 6407 lsls r4, r4, #29  */
L_0a72:
    lsrs r4, r4, #29                        /* @0A72 640f lsrs r4, r4, #29  */
L_0a74:
    orrs r3, r4                             /* @0A74 2343 orrs r3, r4  */
L_0a76:
    str r3, [r2, #28]                       /* @0A76 d361 str r3, [r2, #28]  */
L_0a78:
    ldr r3, [r2, #28]                       /* @0A78 d369 ldr r3, [r2, #28]  */
L_0a7a:
    movs r4, #8                             /* @0A7A 0824 movs r4, #8  */
L_0a7c:
    bics r3, r4                             /* @0A7C a343 bics r3, r4  */
L_0a7e:
    ldrb r4, [r1, #2]                       /* @0A7E 8c78 ldrb r4, [r1, #2]  */
L_0a80:
    lsls r4, r4, #31                        /* @0A80 e407 lsls r4, r4, #31  */
L_0a82:
    lsrs r4, r4, #28                        /* @0A82 240f lsrs r4, r4, #28  */
L_0a84:
    orrs r3, r4                             /* @0A84 2343 orrs r3, r4  */
L_0a86:
    str r3, [r2, #28]                       /* @0A86 d361 str r3, [r2, #28]  */
L_0a88:
    ldr r3, [r2, #36]                       /* @0A88 536a ldr r3, [r2, #36]  */
L_0a8a:
    movs r4, #64                            /* @0A8A 4024 movs r4, #64  */
L_0a8c:
    bics r3, r4                             /* @0A8C a343 bics r3, r4  */
L_0a8e:
    ldrb r4, [r1, #3]                       /* @0A8E cc78 ldrb r4, [r1, #3]  */
L_0a90:
    lsls r4, r4, #31                        /* @0A90 e407 lsls r4, r4, #31  */
L_0a92:
    lsrs r4, r4, #25                        /* @0A92 640e lsrs r4, r4, #25  */
L_0a94:
    orrs r3, r4                             /* @0A94 2343 orrs r3, r4  */
L_0a96:
    str r3, [r2, #36]                       /* @0A96 5362 str r3, [r2, #36]  */
L_0a98:
    ldr r3, [r2, #12]                       /* @0A98 d368 ldr r3, [r2, #12]  */
L_0a9a:
    movs r4, #3                             /* @0A9A 0324 movs r4, #3  */
L_0a9c:
    lsls r4, r4, #21                        /* @0A9C 6405 lsls r4, r4, #21  */
L_0a9e:
    bics r3, r4                             /* @0A9E a343 bics r3, r4  */
L_0aa0:
    ldrb r4, [r1, #4]                       /* @0AA0 0c79 ldrb r4, [r1, #4]  */
L_0aa2:
    lsls r4, r4, #30                        /* @0AA2 a407 lsls r4, r4, #30  */
L_0aa4:
    lsrs r4, r4, #9                         /* @0AA4 640a lsrs r4, r4, #9  */
L_0aa6:
    orrs r3, r4                             /* @0AA6 2343 orrs r3, r4  */
L_0aa8:
    str r3, [r2, #12]                       /* @0AA8 d360 str r3, [r2, #12]  */
L_0aaa:
    ldr r3, [r2, #36]                       /* @0AAA 536a ldr r3, [r2, #36]  */
L_0aac:
    movs r4, #32                            /* @0AAC 2024 movs r4, #32  */
L_0aae:
    bics r3, r4                             /* @0AAE a343 bics r3, r4  */
L_0ab0:
    str r3, [r2, #36]                       /* @0AB0 5362 str r3, [r2, #36]  */
L_0ab2:
    ldr r3, [r2, #28]                       /* @0AB2 d369 ldr r3, [r2, #28]  */
L_0ab4:
    movs r4, #112                           /* @0AB4 7024 movs r4, #112  */
L_0ab6:
    bics r3, r4                             /* @0AB6 a343 bics r3, r4  */
L_0ab8:
    ldrb r4, [r1, #6]                       /* @0AB8 8c79 ldrb r4, [r1, #6]  */
L_0aba:
    lsls r4, r4, #29                        /* @0ABA 6407 lsls r4, r4, #29  */
L_0abc:
    lsrs r4, r4, #25                        /* @0ABC 640e lsrs r4, r4, #25  */
L_0abe:
    orrs r3, r4                             /* @0ABE 2343 orrs r3, r4  */
L_0ac0:
    str r3, [r2, #28]                       /* @0AC0 d361 str r3, [r2, #28]  */
L_0ac2:
    ldr r3, [r2, #28]                       /* @0AC2 d369 ldr r3, [r2, #28]  */
L_0ac4:
    ldrb r5, [r1, #7]                       /* @0AC4 cd79 ldrb r5, [r1, #7]  */
L_0ac6:
    movs r4, #128                           /* @0AC6 8024 movs r4, #128  */
L_0ac8:
    lsls r5, r5, #31                        /* @0AC8 ed07 lsls r5, r5, #31  */
L_0aca:
    bics r3, r4                             /* @0ACA a343 bics r3, r4  */
L_0acc:
    lsrs r5, r5, #24                        /* @0ACC 2d0e lsrs r5, r5, #24  */
L_0ace:
    orrs r3, r5                             /* @0ACE 2b43 orrs r3, r5  */
L_0ad0:
    str r3, [r2, #28]                       /* @0AD0 d361 str r3, [r2, #28]  */
L_0ad2:
    ldr r3, [r2, #36]                       /* @0AD2 536a ldr r3, [r2, #36]  */
L_0ad4:
    bics r3, r4                             /* @0AD4 a343 bics r3, r4  */
L_0ad6:
    ldrb r4, [r1, #8]                       /* @0AD6 0c7a ldrb r4, [r1, #8]  */
L_0ad8:
    lsls r4, r4, #31                        /* @0AD8 e407 lsls r4, r4, #31  */
L_0ada:
    lsrs r4, r4, #24                        /* @0ADA 240e lsrs r4, r4, #24  */
L_0adc:
    orrs r3, r4                             /* @0ADC 2343 orrs r3, r4  */
L_0ade:
    str r3, [r2, #36]                       /* @0ADE 5362 str r3, [r2, #36]  */
L_0ae0:
    ldr r3, [r2, #36]                       /* @0AE0 536a ldr r3, [r2, #36]  */
L_0ae2:
    movs r4, #3                             /* @0AE2 0324 movs r4, #3  */
L_0ae4:
    ldrb r1, [r1, #9]                       /* @0AE4 497a ldrb r1, [r1, #9]  */
L_0ae6:
    lsls r4, r4, #12                        /* @0AE6 2403 lsls r4, r4, #12  */
L_0ae8:
    lsls r1, r1, #30                        /* @0AE8 8907 lsls r1, r1, #30  */
L_0aea:
    bics r3, r4                             /* @0AEA a343 bics r3, r4  */
L_0aec:
    lsrs r1, r1, #18                        /* @0AEC 890c lsrs r1, r1, #18  */
L_0aee:
    orrs r3, r1                             /* @0AEE 0b43 orrs r3, r1  */
L_0af0:
    str r3, [r2, #36]                       /* @0AF0 5362 str r3, [r2, #36]  */
L_0af2:
    pop {r4, r5, pc}                        /* @0AF2 30bd pop {r4, r5, pc}  */
L_0af4:
    .byte 0x00,0x0c,0x00,0x40 /* @0AF4 literal */

/* sub_0AF8 */
L_0af8:
    mov r2, r0                              /* @0AF8 0246 mov r2, r0  */
L_0afa:
    movs r0, #0                             /* @0AFA 0020 movs r0, #0  */
L_0afc:
    ldr r3, [pc, #16]                       /* @0AFC 044b ldr r3, [pc, #16] literal 0x0B10 -> 0x40000C00 */
L_0afe:
    lsls r2, r2, #8                         /* @0AFE 1202 lsls r2, r2, #8  */
L_0b00:
    adds r2, r2, r3                         /* @0B00 d218 adds r2, r2, r3  */
L_0b02:
    ldr r3, [r2, #52]                       /* @0B02 536b ldr r3, [r2, #52]  */
L_0b04:
    lsrs r3, r3, #8                         /* @0B04 1b0a lsrs r3, r3, #8  */
L_0b06:
    lsls r3, r3, #8                         /* @0B06 1b02 lsls r3, r3, #8  */
L_0b08:
    orrs r3, r1                             /* @0B08 0b43 orrs r3, r1  */
L_0b0a:
    str r3, [r2, #52]                       /* @0B0A 5363 str r3, [r2, #52]  */
L_0b0c:
    bx lr                                   /* @0B0C 7047 bx lr  */
    .byte 0x00,0x00 /* @0B0E unclassified */
L_0b10:
    .byte 0x00,0x0c,0x00,0x40 /* @0B10 literal */

/* sub_0B14 */
L_0b14:
    push {r4, lr}                           /* @0B14 10b5 push {r4, lr}  */
L_0b16:
    mov r2, r0                              /* @0B16 0246 mov r2, r0  */
L_0b18:
    movs r0, #0                             /* @0B18 0020 movs r0, #0  */
L_0b1a:
    ldr r3, [pc, #72]                       /* @0B1A 124b ldr r3, [pc, #72] literal 0x0B64 -> 0x40000C00 */
L_0b1c:
    lsls r2, r2, #8                         /* @0B1C 1202 lsls r2, r2, #8  */
L_0b1e:
    adds r2, r2, r3                         /* @0B1E d218 adds r2, r2, r3  */
L_0b20:
    ldr r3, [r2, #32]                       /* @0B20 136a ldr r3, [r2, #32]  */
L_0b22:
    movs r4, #128                           /* @0B22 8024 movs r4, #128  */
L_0b24:
    bics r3, r4                             /* @0B24 a343 bics r3, r4  */
L_0b26:
    ldrb r4, [r1]                           /* @0B26 0c78 ldrb r4, [r1]  */
L_0b28:
    lsls r4, r4, #31                        /* @0B28 e407 lsls r4, r4, #31  */
L_0b2a:
    lsrs r4, r4, #24                        /* @0B2A 240e lsrs r4, r4, #24  */
L_0b2c:
    orrs r3, r4                             /* @0B2C 2343 orrs r3, r4  */
L_0b2e:
    str r3, [r2, #32]                       /* @0B2E 1362 str r3, [r2, #32]  */
L_0b30:
    ldr r3, [r2, #32]                       /* @0B30 136a ldr r3, [r2, #32]  */
L_0b32:
    ldrb r4, [r1, #1]                       /* @0B32 4c78 ldrb r4, [r1, #1]  */
L_0b34:
    lsrs r3, r3, #1                         /* @0B34 5b08 lsrs r3, r3, #1  */
L_0b36:
    lsls r3, r3, #1                         /* @0B36 5b00 lsls r3, r3, #1  */
L_0b38:
    lsls r4, r4, #31                        /* @0B38 e407 lsls r4, r4, #31  */
L_0b3a:
    lsrs r4, r4, #31                        /* @0B3A e40f lsrs r4, r4, #31  */
L_0b3c:
    orrs r3, r4                             /* @0B3C 2343 orrs r3, r4  */
L_0b3e:
    str r3, [r2, #32]                       /* @0B3E 1362 str r3, [r2, #32]  */
L_0b40:
    ldr r3, [r2, #32]                       /* @0B40 136a ldr r3, [r2, #32]  */
L_0b42:
    movs r4, #2                             /* @0B42 0224 movs r4, #2  */
L_0b44:
    bics r3, r4                             /* @0B44 a343 bics r3, r4  */
L_0b46:
    ldrb r4, [r1, #2]                       /* @0B46 8c78 ldrb r4, [r1, #2]  */
L_0b48:
    lsls r4, r4, #31                        /* @0B48 e407 lsls r4, r4, #31  */
L_0b4a:
    lsrs r4, r4, #30                        /* @0B4A a40f lsrs r4, r4, #30  */
L_0b4c:
    orrs r3, r4                             /* @0B4C 2343 orrs r3, r4  */
L_0b4e:
    str r3, [r2, #32]                       /* @0B4E 1362 str r3, [r2, #32]  */
L_0b50:
    ldr r3, [r2, #32]                       /* @0B50 136a ldr r3, [r2, #32]  */
L_0b52:
    ldrb r1, [r1, #3]                       /* @0B52 c978 ldrb r1, [r1, #3]  */
L_0b54:
    movs r4, #16                            /* @0B54 1024 movs r4, #16  */
L_0b56:
    lsls r1, r1, #31                        /* @0B56 c907 lsls r1, r1, #31  */
L_0b58:
    bics r3, r4                             /* @0B58 a343 bics r3, r4  */
L_0b5a:
    lsrs r1, r1, #27                        /* @0B5A c90e lsrs r1, r1, #27  */
L_0b5c:
    orrs r3, r1                             /* @0B5C 0b43 orrs r3, r1  */
L_0b5e:
    str r3, [r2, #32]                       /* @0B5E 1362 str r3, [r2, #32]  */
L_0b60:
    pop {r4, pc}                            /* @0B60 10bd pop {r4, pc}  */
    .byte 0x00,0x00 /* @0B62 unclassified */
L_0b64:
    .byte 0x00,0x0c,0x00,0x40 /* @0B64 literal */
    .byte 0x01,0x46,0x00,0x20,0x04,0x4a,0x09,0x02,0x89,0x18,0xca,0x68,0x01,0x23,0x9b,0x02 /* @0B68 unclassified */
    .byte 0x9a,0x43,0xca,0x60,0x70,0x47,0x00,0x00,0x00,0x0c,0x00,0x40 /* @0B78 unclassified */

/* sub_0B84 */
L_0b84:
    mov r1, r0                              /* @0B84 0146 mov r1, r0  */
L_0b86:
    movs r0, #0                             /* @0B86 0020 movs r0, #0  */
L_0b88:
    ldr r2, [pc, #16]                       /* @0B88 044a ldr r2, [pc, #16] literal 0x0B9C -> 0x40000C00 */
L_0b8a:
    lsls r1, r1, #8                         /* @0B8A 0902 lsls r1, r1, #8  */
L_0b8c:
    adds r1, r1, r2                         /* @0B8C 8918 adds r1, r1, r2  */
L_0b8e:
    ldr r2, [r1, #12]                       /* @0B8E ca68 ldr r2, [r1, #12]  */
L_0b90:
    movs r3, #1                             /* @0B90 0123 movs r3, #1  */
L_0b92:
    lsls r3, r3, #10                        /* @0B92 9b02 lsls r3, r3, #10  */
L_0b94:
    orrs r2, r3                             /* @0B94 1a43 orrs r2, r3  */
L_0b96:
    str r2, [r1, #12]                       /* @0B96 ca60 str r2, [r1, #12]  */
L_0b98:
    bx lr                                   /* @0B98 7047 bx lr  */
    .byte 0x00,0x00 /* @0B9A unclassified */
L_0b9c:
    .byte 0x00,0x0c,0x00,0x40 /* @0B9C literal */

/* sub_0BA0 */
L_0ba0:
    push {r4, lr}                           /* @0BA0 10b5 push {r4, lr}  */
L_0ba2:
    mov r2, r0                              /* @0BA2 0246 mov r2, r0  */
L_0ba4:
    movs r0, #0                             /* @0BA4 0020 movs r0, #0  */
L_0ba6:
    ldr r3, [pc, #124]                      /* @0BA6 1f4b ldr r3, [pc, #124] literal 0x0C24 -> 0x40000C00 */
L_0ba8:
    lsls r2, r2, #8                         /* @0BA8 1202 lsls r2, r2, #8  */
L_0baa:
    adds r2, r2, r3                         /* @0BAA d218 adds r2, r2, r3  */
L_0bac:
    ldr r3, [r2, #12]                       /* @0BAC d368 ldr r3, [r2, #12]  */
L_0bae:
    movs r4, #3                             /* @0BAE 0324 movs r4, #3  */
L_0bb0:
    lsls r4, r4, #12                        /* @0BB0 2403 lsls r4, r4, #12  */
L_0bb2:
    bics r3, r4                             /* @0BB2 a343 bics r3, r4  */
L_0bb4:
    ldrb r4, [r1]                           /* @0BB4 0c78 ldrb r4, [r1]  */
L_0bb6:
    lsls r4, r4, #30                        /* @0BB6 a407 lsls r4, r4, #30  */
L_0bb8:
    lsrs r4, r4, #18                        /* @0BB8 a40c lsrs r4, r4, #18  */
L_0bba:
    orrs r3, r4                             /* @0BBA 2343 orrs r3, r4  */
L_0bbc:
    str r3, [r2, #12]                       /* @0BBC d360 str r3, [r2, #12]  */
L_0bbe:
    ldr r3, [r2, #12]                       /* @0BBE d368 ldr r3, [r2, #12]  */
L_0bc0:
    movs r4, #1                             /* @0BC0 0124 movs r4, #1  */
L_0bc2:
    lsls r4, r4, #9                         /* @0BC2 6402 lsls r4, r4, #9  */
L_0bc4:
    bics r3, r4                             /* @0BC4 a343 bics r3, r4  */
L_0bc6:
    ldrb r4, [r1, #1]                       /* @0BC6 4c78 ldrb r4, [r1, #1]  */
L_0bc8:
    lsls r4, r4, #31                        /* @0BC8 e407 lsls r4, r4, #31  */
L_0bca:
    lsrs r4, r4, #22                        /* @0BCA a40d lsrs r4, r4, #22  */
L_0bcc:
    orrs r3, r4                             /* @0BCC 2343 orrs r3, r4  */
L_0bce:
    str r3, [r2, #12]                       /* @0BCE d360 str r3, [r2, #12]  */
L_0bd0:
    ldr r3, [r2, #12]                       /* @0BD0 d368 ldr r3, [r2, #12]  */
L_0bd2:
    movs r4, #255                           /* @0BD2 ff24 movs r4, #255  */
L_0bd4:
    adds r4, #1                             /* @0BD4 0134 adds r4, #1  */
L_0bd6:
    bics r3, r4                             /* @0BD6 a343 bics r3, r4  */
L_0bd8:
    ldrb r4, [r1, #2]                       /* @0BD8 8c78 ldrb r4, [r1, #2]  */
L_0bda:
    lsls r4, r4, #31                        /* @0BDA e407 lsls r4, r4, #31  */
L_0bdc:
    lsrs r4, r4, #23                        /* @0BDC e40d lsrs r4, r4, #23  */
L_0bde:
    orrs r3, r4                             /* @0BDE 2343 orrs r3, r4  */
L_0be0:
    str r3, [r2, #12]                       /* @0BE0 d360 str r3, [r2, #12]  */
L_0be2:
    ldr r3, [r2, #12]                       /* @0BE2 d368 ldr r3, [r2, #12]  */
L_0be4:
    movs r4, #112                           /* @0BE4 7024 movs r4, #112  */
L_0be6:
    bics r3, r4                             /* @0BE6 a343 bics r3, r4  */
L_0be8:
    ldrb r4, [r1, #3]                       /* @0BE8 cc78 ldrb r4, [r1, #3]  */
L_0bea:
    lsls r4, r4, #29                        /* @0BEA 6407 lsls r4, r4, #29  */
L_0bec:
    lsrs r4, r4, #25                        /* @0BEC 640e lsrs r4, r4, #25  */
L_0bee:
    orrs r3, r4                             /* @0BEE 2343 orrs r3, r4  */
L_0bf0:
    str r3, [r2, #12]                       /* @0BF0 d360 str r3, [r2, #12]  */
L_0bf2:
    ldr r3, [r2, #12]                       /* @0BF2 d368 ldr r3, [r2, #12]  */
L_0bf4:
    movs r4, #8                             /* @0BF4 0824 movs r4, #8  */
L_0bf6:
    bics r3, r4                             /* @0BF6 a343 bics r3, r4  */
L_0bf8:
    ldrb r4, [r1, #4]                       /* @0BF8 0c79 ldrb r4, [r1, #4]  */
L_0bfa:
    lsls r4, r4, #31                        /* @0BFA e407 lsls r4, r4, #31  */
L_0bfc:
    lsrs r4, r4, #28                        /* @0BFC 240f lsrs r4, r4, #28  */
L_0bfe:
    orrs r3, r4                             /* @0BFE 2343 orrs r3, r4  */
L_0c00:
    str r3, [r2, #12]                       /* @0C00 d360 str r3, [r2, #12]  */
L_0c02:
    ldr r3, [r2, #12]                       /* @0C02 d368 ldr r3, [r2, #12]  */
L_0c04:
    movs r4, #4                             /* @0C04 0424 movs r4, #4  */
L_0c06:
    bics r3, r4                             /* @0C06 a343 bics r3, r4  */
L_0c08:
    ldrb r4, [r1, #5]                       /* @0C08 4c79 ldrb r4, [r1, #5]  */
L_0c0a:
    lsls r4, r4, #31                        /* @0C0A e407 lsls r4, r4, #31  */
L_0c0c:
    lsrs r4, r4, #29                        /* @0C0C 640f lsrs r4, r4, #29  */
L_0c0e:
    orrs r3, r4                             /* @0C0E 2343 orrs r3, r4  */
L_0c10:
    str r3, [r2, #12]                       /* @0C10 d360 str r3, [r2, #12]  */
L_0c12:
    ldr r3, [r2, #12]                       /* @0C12 d368 ldr r3, [r2, #12]  */
L_0c14:
    ldrb r1, [r1, #6]                       /* @0C14 8979 ldrb r1, [r1, #6]  */
L_0c16:
    movs r4, #2                             /* @0C16 0224 movs r4, #2  */
L_0c18:
    lsls r1, r1, #31                        /* @0C18 c907 lsls r1, r1, #31  */
L_0c1a:
    bics r3, r4                             /* @0C1A a343 bics r3, r4  */
L_0c1c:
    lsrs r1, r1, #30                        /* @0C1C 890f lsrs r1, r1, #30  */
L_0c1e:
    orrs r3, r1                             /* @0C1E 0b43 orrs r3, r1  */
L_0c20:
    str r3, [r2, #12]                       /* @0C20 d360 str r3, [r2, #12]  */
L_0c22:
    pop {r4, pc}                            /* @0C22 10bd pop {r4, pc}  */
L_0c24:
    .byte 0x00,0x0c,0x00,0x40 /* @0C24 literal */

/* sub_0C28 */
L_0c28:
    push {r4, lr}                           /* @0C28 10b5 push {r4, lr}  */
L_0c2a:
    mov r2, r0                              /* @0C2A 0246 mov r2, r0  */
L_0c2c:
    movs r0, #0                             /* @0C2C 0020 movs r0, #0  */
L_0c2e:
    ldr r3, [pc, #144]                      /* @0C2E 244b ldr r3, [pc, #144] literal 0x0CC0 -> 0x40000C00 */
L_0c30:
    lsls r2, r2, #8                         /* @0C30 1202 lsls r2, r2, #8  */
L_0c32:
    adds r2, r2, r3                         /* @0C32 d218 adds r2, r2, r3  */
L_0c34:
    ldr r3, [r2, #12]                       /* @0C34 d368 ldr r3, [r2, #12]  */
L_0c36:
    movs r4, #3                             /* @0C36 0324 movs r4, #3  */
L_0c38:
    lsls r4, r4, #12                        /* @0C38 2403 lsls r4, r4, #12  */
L_0c3a:
    bics r3, r4                             /* @0C3A a343 bics r3, r4  */
L_0c3c:
    ldrb r4, [r1]                           /* @0C3C 0c78 ldrb r4, [r1]  */
L_0c3e:
    lsls r4, r4, #30                        /* @0C3E a407 lsls r4, r4, #30  */
L_0c40:
    lsrs r4, r4, #18                        /* @0C40 a40c lsrs r4, r4, #18  */
L_0c42:
    orrs r3, r4                             /* @0C42 2343 orrs r3, r4  */
L_0c44:
    str r3, [r2, #12]                       /* @0C44 d360 str r3, [r2, #12]  */
L_0c46:
    ldr r3, [r2, #12]                       /* @0C46 d368 ldr r3, [r2, #12]  */
L_0c48:
    movs r4, #112                           /* @0C48 7024 movs r4, #112  */
L_0c4a:
    bics r3, r4                             /* @0C4A a343 bics r3, r4  */
L_0c4c:
    ldrb r4, [r1, #2]                       /* @0C4C 8c78 ldrb r4, [r1, #2]  */
L_0c4e:
    lsls r4, r4, #29                        /* @0C4E 6407 lsls r4, r4, #29  */
L_0c50:
    lsrs r4, r4, #25                        /* @0C50 640e lsrs r4, r4, #25  */
L_0c52:
    orrs r3, r4                             /* @0C52 2343 orrs r3, r4  */
L_0c54:
    str r3, [r2, #12]                       /* @0C54 d360 str r3, [r2, #12]  */
L_0c56:
    ldr r3, [r2, #12]                       /* @0C56 d368 ldr r3, [r2, #12]  */
L_0c58:
    movs r4, #4                             /* @0C58 0424 movs r4, #4  */
L_0c5a:
    bics r3, r4                             /* @0C5A a343 bics r3, r4  */
L_0c5c:
    ldrb r4, [r1, #3]                       /* @0C5C cc78 ldrb r4, [r1, #3]  */
L_0c5e:
    lsls r4, r4, #31                        /* @0C5E e407 lsls r4, r4, #31  */
L_0c60:
    lsrs r4, r4, #29                        /* @0C60 640f lsrs r4, r4, #29  */
L_0c62:
    orrs r3, r4                             /* @0C62 2343 orrs r3, r4  */
L_0c64:
    str r3, [r2, #12]                       /* @0C64 d360 str r3, [r2, #12]  */
L_0c66:
    ldr r3, [r2, #12]                       /* @0C66 d368 ldr r3, [r2, #12]  */
L_0c68:
    movs r4, #2                             /* @0C68 0224 movs r4, #2  */
L_0c6a:
    bics r3, r4                             /* @0C6A a343 bics r3, r4  */
L_0c6c:
    ldrb r4, [r1, #4]                       /* @0C6C 0c79 ldrb r4, [r1, #4]  */
L_0c6e:
    lsls r4, r4, #31                        /* @0C6E e407 lsls r4, r4, #31  */
L_0c70:
    lsrs r4, r4, #30                        /* @0C70 a40f lsrs r4, r4, #30  */
L_0c72:
    orrs r3, r4                             /* @0C72 2343 orrs r3, r4  */
L_0c74:
    str r3, [r2, #12]                       /* @0C74 d360 str r3, [r2, #12]  */
L_0c76:
    ldr r3, [r2, #12]                       /* @0C76 d368 ldr r3, [r2, #12]  */
L_0c78:
    movs r4, #8                             /* @0C78 0824 movs r4, #8  */
L_0c7a:
    bics r3, r4                             /* @0C7A a343 bics r3, r4  */
L_0c7c:
    ldrb r4, [r1, #5]                       /* @0C7C 4c79 ldrb r4, [r1, #5]  */
L_0c7e:
    lsls r4, r4, #31                        /* @0C7E e407 lsls r4, r4, #31  */
L_0c80:
    lsrs r4, r4, #28                        /* @0C80 240f lsrs r4, r4, #28  */
L_0c82:
    orrs r3, r4                             /* @0C82 2343 orrs r3, r4  */
L_0c84:
    str r3, [r2, #12]                       /* @0C84 d360 str r3, [r2, #12]  */
L_0c86:
    ldr r3, [r2, #12]                       /* @0C86 d368 ldr r3, [r2, #12]  */
L_0c88:
    movs r4, #1                             /* @0C88 0124 movs r4, #1  */
L_0c8a:
    lsls r4, r4, #14                        /* @0C8A a403 lsls r4, r4, #14  */
L_0c8c:
    bics r3, r4                             /* @0C8C a343 bics r3, r4  */
L_0c8e:
    ldrb r4, [r1, #6]                       /* @0C8E 8c79 ldrb r4, [r1, #6]  */
L_0c90:
    lsls r4, r4, #31                        /* @0C90 e407 lsls r4, r4, #31  */
L_0c92:
    lsrs r4, r4, #17                        /* @0C92 640c lsrs r4, r4, #17  */
L_0c94:
    orrs r3, r4                             /* @0C94 2343 orrs r3, r4  */
L_0c96:
    str r3, [r2, #12]                       /* @0C96 d360 str r3, [r2, #12]  */
L_0c98:
    ldr r3, [r2, #12]                       /* @0C98 d368 ldr r3, [r2, #12]  */
L_0c9a:
    movs r4, #1                             /* @0C9A 0124 movs r4, #1  */
L_0c9c:
    lsls r4, r4, #17                        /* @0C9C 6404 lsls r4, r4, #17  */
L_0c9e:
    bics r3, r4                             /* @0C9E a343 bics r3, r4  */
L_0ca0:
    ldrb r4, [r1, #7]                       /* @0CA0 cc79 ldrb r4, [r1, #7]  */
L_0ca2:
    lsls r4, r4, #31                        /* @0CA2 e407 lsls r4, r4, #31  */
L_0ca4:
    lsrs r4, r4, #14                        /* @0CA4 a40b lsrs r4, r4, #14  */
L_0ca6:
    orrs r3, r4                             /* @0CA6 2343 orrs r3, r4  */
L_0ca8:
    str r3, [r2, #12]                       /* @0CA8 d360 str r3, [r2, #12]  */
L_0caa:
    ldr r3, [r2, #12]                       /* @0CAA d368 ldr r3, [r2, #12]  */
L_0cac:
    movs r4, #1                             /* @0CAC 0124 movs r4, #1  */
L_0cae:
    ldrb r1, [r1, #1]                       /* @0CAE 4978 ldrb r1, [r1, #1]  */
L_0cb0:
    lsls r4, r4, #27                        /* @0CB0 e406 lsls r4, r4, #27  */
L_0cb2:
    lsls r1, r1, #31                        /* @0CB2 c907 lsls r1, r1, #31  */
L_0cb4:
    bics r3, r4                             /* @0CB4 a343 bics r3, r4  */
L_0cb6:
    lsrs r1, r1, #4                         /* @0CB6 0909 lsrs r1, r1, #4  */
L_0cb8:
    orrs r3, r1                             /* @0CB8 0b43 orrs r3, r1  */
L_0cba:
    str r3, [r2, #12]                       /* @0CBA d360 str r3, [r2, #12]  */
L_0cbc:
    pop {r4, pc}                            /* @0CBC 10bd pop {r4, pc}  */
    .byte 0x00,0x00 /* @0CBE unclassified */
L_0cc0:
    .byte 0x00,0x0c,0x00,0x40 /* @0CC0 literal */

/* sub_0CC4 */
L_0cc4:
    push {r4, lr}                           /* @0CC4 10b5 push {r4, lr}  */
L_0cc6:
    mov r8, r8                              /* @0CC6 c046 mov r8, r8  */
L_0cc8:
    mov r8, r8                              /* @0CC8 c046 mov r8, r8  */
L_0cca:
    pop {r4, pc}                            /* @0CCA 10bd pop {r4, pc}  */

/* sub_0CCC */
L_0ccc:
    push {r4, r5, r6, r7, lr}               /* @0CCC f0b5 push {r4, r5, r6, r7, lr}  */
L_0cce:
    sub sp, #508                            /* @0CCE ffb0 sub sp, #508  */
L_0cd0:
    sub sp, #8                              /* @0CD0 82b0 sub sp, #8  */
L_0cd2:
    movs r4, #0                             /* @0CD2 0024 movs r4, #0  */
L_0cd4:
    movs r5, #0                             /* @0CD4 0025 movs r5, #0  */
L_0cd6:
    movs r1, #1                             /* @0CD6 0121 movs r1, #1  */
L_0cd8:
    lsls r1, r1, #9                         /* @0CD8 4902 lsls r1, r1, #9  */
L_0cda:
    add r0, sp, #4                          /* @0CDA 01a8 add r0, sp, #4  */
L_0cdc:
    bl L_025c                               /* @0CDC fff7befa bl #-2692 -> sub_025C */
L_0ce0:
    movs r6, #255                           /* @0CE0 ff26 movs r6, #255  */
L_0ce2:
    adds r6, #1                             /* @0CE2 0136 adds r6, #1  */
L_0ce4:
    movs r7, #63                            /* @0CE4 3f27 movs r7, #63  */
L_0ce6:
    lsls r7, r7, #9                         /* @0CE6 7f02 lsls r7, r7, #9  */
L_0ce8:
    b L_0d0a                                /* @0CE8 0fe0 b #30 -> 0x0D0A */
L_0cea:
    uxth r0, r4                             /* @0CEA a0b2 uxth r0, r4  */
L_0cec:
    bl L_26e8                               /* @0CEC 01f0fcfc bl #6648 -> encoder_inverse_lookup */
L_0cf0:
    lsls r2, r5, #1                         /* @0CF0 6a00 lsls r2, r5, #1  */
L_0cf2:
    add r1, sp, #4                          /* @0CF2 01a9 add r1, sp, #4  */
L_0cf4:
    strh r0, [r1, r2]                       /* @0CF4 8852 strh r0, [r1, r2]  */
L_0cf6:
    adds r4, r4, #1                         /* @0CF6 641c adds r4, r4, #1  */
L_0cf8:
    adds r5, r5, #1                         /* @0CF8 6d1c adds r5, r5, #1  */
L_0cfa:
    mov r2, r6                              /* @0CFA 3246 mov r2, r6  */
L_0cfc:
    cmp r5, r2                              /* @0CFC 9542 cmp r5, r2  */
L_0cfe:
    bne L_0d0a                              /* @0CFE 04d1 bne #8 -> 0x0D0A */
L_0d00:
    movs r5, #0                             /* @0D00 0025 movs r5, #0  */
L_0d02:
    lsls r0, r4, #1                         /* @0D02 6000 lsls r0, r4, #1  */
L_0d04:
    adds r0, r0, r7                         /* @0D04 c019 adds r0, r0, r7  */
L_0d06:
    bl L_113c                               /* @0D06 00f019fa bl #1074 -> sub_113C */
L_0d0a:
    movs r0, #1                             /* @0D0A 0120 movs r0, #1  */
L_0d0c:
    lsls r0, r0, #14                        /* @0D0C 8003 lsls r0, r0, #14  */
L_0d0e:
    cmp r4, r0                              /* @0D0E 8442 cmp r4, r0  */
L_0d10:
    blo L_0cea                              /* @0D10 ebd3 blo #-42 -> 0x0CEA */
L_0d12:
    ldr r0, [pc, #28]                       /* @0D12 0748 ldr r0, [pc, #28] literal 0x0D30 -> 0x200008C8 */
L_0d14:
    movs r4, #1                             /* @0D14 0124 movs r4, #1  */
L_0d16:
    strh r4, [r0, #22]                      /* @0D16 c482 strh r4, [r0, #22]  */
L_0d18:
    mov r1, r0                              /* @0D18 0146 mov r1, r0  */
L_0d1a:
    movs r2, #1                             /* @0D1A 0122 movs r2, #1  */
L_0d1c:
    adds r1, #22                            /* @0D1C 1631 adds r1, #22  */
L_0d1e:
    ldr r0, [pc, #20]                       /* @0D1E 0548 ldr r0, [pc, #20] literal 0x0D34 -> 0x00007F96 */
L_0d20:
    bl L_1164                               /* @0D20 00f020fa bl #1088 -> sub_1164 */
L_0d24:
    ldr r0, [pc, #16]                       /* @0D24 0448 ldr r0, [pc, #16] literal 0x0D38 -> 0x2000000B */
L_0d26:
    strb r4, [r0]                           /* @0D26 0470 strb r4, [r0]  */
L_0d28:
    add sp, #508                            /* @0D28 7fb0 add sp, #508  */
L_0d2a:
    add sp, #8                              /* @0D2A 02b0 add sp, #8  */
L_0d2c:
    pop {r4, r5, r6, r7, pc}                /* @0D2C f0bd pop {r4, r5, r6, r7, pc}  */
    .byte 0x00,0x00 /* @0D2E unclassified */
L_0d30:
    .byte 0xc8,0x08,0x00,0x20 /* @0D30 literal */
L_0d34:
    .byte 0x96,0x7f,0x00,0x00 /* @0D34 literal */
L_0d38:
    .byte 0x0b,0x00,0x00,0x20 /* @0D38 literal */

/* sub_0D3C */
L_0d3c:
    movs r0, #1                             /* @0D3C 0120 movs r0, #1  */
L_0d3e:
    lsls r0, r0, #30                        /* @0D3E 8007 lsls r0, r0, #30  */
L_0d40:
    ldr r1, [r0, #20]                       /* @0D40 4169 ldr r1, [r0, #20]  */
L_0d42:
    lsrs r1, r1, #1                         /* @0D42 4908 lsrs r1, r1, #1  */
L_0d44:
    lsls r1, r1, #1                         /* @0D44 4900 lsls r1, r1, #1  */
L_0d46:
    str r1, [r0, #20]                       /* @0D46 4161 str r1, [r0, #20]  */
L_0d48:
    ldr r2, [r0, #20]                       /* @0D48 4269 ldr r2, [r0, #20]  */
L_0d4a:
    movs r1, #2                             /* @0D4A 0221 movs r1, #2  */
L_0d4c:
    bics r2, r1                             /* @0D4C 8a43 bics r2, r1  */
L_0d4e:
    str r2, [r0, #20]                       /* @0D4E 4261 str r2, [r0, #20]  */
L_0d50:
    ldr r2, [r0, #4]                        /* @0D50 4268 ldr r2, [r0, #4]  */
L_0d52:
    bics r2, r1                             /* @0D52 8a43 bics r2, r1  */
L_0d54:
    str r2, [r0, #4]                        /* @0D54 4260 str r2, [r0, #4]  */
L_0d56:
    ldr r1, [r0, #4]                        /* @0D56 4168 ldr r1, [r0, #4]  */
L_0d58:
    movs r2, #1                             /* @0D58 0122 movs r2, #1  */
L_0d5a:
    orrs r1, r2                             /* @0D5A 1143 orrs r1, r2  */
L_0d5c:
    str r1, [r0, #4]                        /* @0D5C 4160 str r1, [r0, #4]  */
L_0d5e:
    bx lr                                   /* @0D5E 7047 bx lr  */
    .byte 0x70,0x47,0x00,0x00,0x04,0x00,0x00,0x00,0x03,0x00,0x00,0x00 /* @0D60 unclassified */

/* sub_0D6C */
L_0d6c:
    lsls r1, r0, #2                         /* @0D6C 8100 lsls r1, r0, #2  */
L_0d6e:
    ldr r0, [pc, #16]                       /* @0D6E 0448 ldr r0, [pc, #16] literal 0x0D80 -> 0x40021000 */
L_0d70:
    adds r0, r1, r0                         /* @0D70 0818 adds r0, r1, r0  */
L_0d72:
    ldr r1, [r0, #20]                       /* @0D72 4169 ldr r1, [r0, #20]  */
L_0d74:
    movs r2, #7                             /* @0D74 0722 movs r2, #7  */
L_0d76:
    lsls r2, r2, #16                        /* @0D76 1204 lsls r2, r2, #16  */
L_0d78:
    bics r1, r2                             /* @0D78 9143 bics r1, r2  */
L_0d7a:
    str r1, [r0, #20]                       /* @0D7A 4161 str r1, [r0, #20]  */
L_0d7c:
    bx lr                                   /* @0D7C 7047 bx lr  */
    .byte 0x00,0x00 /* @0D7E unclassified */
L_0d80:
    .byte 0x00,0x10,0x02,0x40 /* @0D80 literal */

/* sub_0D84 */
L_0d84:
    ldr r0, [pc, #8]                        /* @0D84 0248 ldr r0, [pc, #8] literal 0x0D90 -> 0x40021000 */
L_0d86:
    ldr r1, [r0]                            /* @0D86 0168 ldr r1, [r0]  */
L_0d88:
    lsls r2, r0, #19                        /* @0D88 c204 lsls r2, r0, #19  */
L_0d8a:
    orrs r1, r2                             /* @0D8A 1143 orrs r1, r2  */
L_0d8c:
    str r1, [r0]                            /* @0D8C 0160 str r1, [r0]  */
L_0d8e:
    bx lr                                   /* @0D8E 7047 bx lr  */
L_0d90:
    .byte 0x00,0x10,0x02,0x40 /* @0D90 literal */

/* sub_0D94 */
L_0d94:
    lsls r1, r0, #2                         /* @0D94 8100 lsls r1, r0, #2  */
L_0d96:
    ldr r0, [pc, #16]                       /* @0D96 0448 ldr r0, [pc, #16] literal 0x0DA8 -> 0x40021000 */
L_0d98:
    adds r0, r1, r0                         /* @0D98 0818 adds r0, r1, r0  */
L_0d9a:
    ldr r1, [r0, #16]                       /* @0D9A 0169 ldr r1, [r0, #16]  */
L_0d9c:
    movs r2, #1                             /* @0D9C 0122 movs r2, #1  */
L_0d9e:
    lsls r2, r2, #31                        /* @0D9E d207 lsls r2, r2, #31  */
L_0da0:
    orrs r1, r2                             /* @0DA0 1143 orrs r1, r2  */
L_0da2:
    str r1, [r0, #16]                       /* @0DA2 0161 str r1, [r0, #16]  */
L_0da4:
    bx lr                                   /* @0DA4 7047 bx lr  */
    .byte 0x00,0x00 /* @0DA6 unclassified */
L_0da8:
    .byte 0x00,0x10,0x02,0x40 /* @0DA8 literal */

/* sub_0DAC */
L_0dac:
    push {r4, r5, lr}                       /* @0DAC 30b5 push {r4, r5, lr}  */
L_0dae:
    cmp r1, #0                              /* @0DAE 0029 cmp r1, #0  */
L_0db0:
    beq L_0e14                              /* @0DB0 30d0 beq #96 -> 0x0E14 */
L_0db2:
    ldr r3, [pc, #100]                      /* @0DB2 194b ldr r3, [pc, #100] literal 0x0E18 -> 0x40021000 */
L_0db4:
    movs r2, #0                             /* @0DB4 0022 movs r2, #0  */
L_0db6:
    lsls r0, r0, #2                         /* @0DB6 8000 lsls r0, r0, #2  */
L_0db8:
    adds r0, r0, r3                         /* @0DB8 c018 adds r0, r0, r3  */
L_0dba:
    str r2, [r0, #20]                       /* @0DBA 4261 str r2, [r0, #20]  */
L_0dbc:
    ldr r4, [r1]                            /* @0DBC 0c68 ldr r4, [r1]  */
L_0dbe:
    ldr r2, [r1, #8]                        /* @0DBE 8a68 ldr r2, [r1, #8]  */
L_0dc0:
    ldr r5, [r1, #16]                       /* @0DC0 0d69 ldr r5, [r1, #16]  */
L_0dc2:
    orrs r4, r2                             /* @0DC2 1443 orrs r4, r2  */
L_0dc4:
    ldr r2, [r1, #12]                       /* @0DC4 ca68 ldr r2, [r1, #12]  */
L_0dc6:
    orrs r2, r5                             /* @0DC6 2a43 orrs r2, r5  */
L_0dc8:
    orrs r4, r2                             /* @0DC8 1443 orrs r4, r2  */
L_0dca:
    ldr r2, [r1, #20]                       /* @0DCA 4a69 ldr r2, [r1, #20]  */
L_0dcc:
    orrs r4, r2                             /* @0DCC 1443 orrs r4, r2  */
L_0dce:
    ldr r2, [r1, #24]                       /* @0DCE 8a69 ldr r2, [r1, #24]  */
L_0dd0:
    orrs r4, r2                             /* @0DD0 1443 orrs r4, r2  */
L_0dd2:
    ldr r2, [r1, #28]                       /* @0DD2 ca69 ldr r2, [r1, #28]  */
L_0dd4:
    orrs r4, r2                             /* @0DD4 1443 orrs r4, r2  */
L_0dd6:
    mov r2, r1                              /* @0DD6 0a46 mov r2, r1  */
L_0dd8:
    adds r2, #32                            /* @0DD8 2032 adds r2, #32  */
L_0dda:
    ldrb r5, [r2, #8]                       /* @0DDA 157a ldrb r5, [r2, #8]  */
L_0ddc:
    orrs r4, r5                             /* @0DDC 2c43 orrs r4, r5  */
L_0dde:
    str r4, [r0, #20]                       /* @0DDE 4461 str r4, [r0, #20]  */
L_0de0:
    ldr r4, [r0, #16]                       /* @0DE0 0469 ldr r4, [r0, #16]  */
L_0de2:
    ldr r5, [pc, #56]                       /* @0DE2 0e4d ldr r5, [pc, #56] literal 0x0E1C -> 0xE0300000 */
L_0de4:
    ands r4, r5                             /* @0DE4 2c40 ands r4, r5  */
L_0de6:
    str r4, [r0, #16]                       /* @0DE6 0461 str r4, [r0, #16]  */
L_0de8:
    ldr r4, [r0, #16]                       /* @0DE8 0469 ldr r4, [r0, #16]  */
L_0dea:
    ldrh r5, [r1, #6]                       /* @0DEA cd88 ldrh r5, [r1, #6]  */
L_0dec:
    ldrb r2, [r2, #16]                      /* @0DEC 127c ldrb r2, [r2, #16]  */
L_0dee:
    subs r5, r5, #1                         /* @0DEE 6d1e subs r5, r5, #1  */
L_0df0:
    orrs r4, r5                             /* @0DF0 2c43 orrs r4, r5  */
L_0df2:
    ldrh r5, [r1, #4]                       /* @0DF2 8d88 ldrh r5, [r1, #4]  */
L_0df4:
    subs r5, r5, #1                         /* @0DF4 6d1e subs r5, r5, #1  */
L_0df6:
    lsls r5, r5, #16                        /* @0DF6 2d04 lsls r5, r5, #16  */
L_0df8:
    lsls r2, r2, #23                        /* @0DF8 d205 lsls r2, r2, #23  */
L_0dfa:
    orrs r5, r2                             /* @0DFA 1543 orrs r5, r2  */
L_0dfc:
    orrs r4, r5                             /* @0DFC 2c43 orrs r4, r5  */
L_0dfe:
    str r4, [r0, #16]                       /* @0DFE 0461 str r4, [r0, #16]  */
L_0e00:
    ldr r2, [r3]                            /* @0E00 1a68 ldr r2, [r3]  */
L_0e02:
    ldr r4, [r1, #44]                       /* @0E02 cc6a ldr r4, [r1, #44]  */
L_0e04:
    orrs r2, r4                             /* @0E04 2243 orrs r2, r4  */
L_0e06:
    str r2, [r3]                            /* @0E06 1a60 str r2, [r3]  */
L_0e08:
    ldr r2, [r1, #32]                       /* @0E08 0a6a ldr r2, [r1, #32]  */
L_0e0a:
    str r2, [r0, #24]                       /* @0E0A 8261 str r2, [r0, #24]  */
L_0e0c:
    ldr r1, [r1, #36]                       /* @0E0C 496a ldr r1, [r1, #36]  */
L_0e0e:
    str r1, [r0, #28]                       /* @0E0E c161 str r1, [r0, #28]  */
L_0e10:
    movs r0, #0                             /* @0E10 0020 movs r0, #0  */
L_0e12:
    pop {r4, r5, pc}                        /* @0E12 30bd pop {r4, r5, pc}  */
L_0e14:
    movs r0, #4                             /* @0E14 0420 movs r0, #4  */
L_0e16:
    pop {r4, r5, pc}                        /* @0E16 30bd pop {r4, r5, pc}  */
L_0e18:
    .byte 0x00,0x10,0x02,0x40 /* @0E18 literal */
L_0e1c:
    .byte 0x00,0x00,0x30,0xe0 /* @0E1C literal */

/* sub_0E20 */
L_0e20:
    push {r3, r4, r5, r6, r7, lr}           /* @0E20 f8b5 push {r3, r4, r5, r6, r7, lr}  */
L_0e22:
    bl L_2fe0                               /* @0E22 02f0ddf8 bl #8634 -> display_banner */
L_0e26:
    ldr r4, [pc, #312]                      /* @0E26 4e4c ldr r4, [pc, #312] literal 0x0F60 -> 0x20000648 */
L_0e28:
    movs r0, #2                             /* @0E28 0220 movs r0, #2  */
L_0e2a:
    strb r0, [r4]                           /* @0E2A 2070 strb r0, [r4]  */
L_0e2c:
    movs r6, #0                             /* @0E2C 0026 movs r6, #0  */
L_0e2e:
    ldr r7, [pc, #308]                      /* @0E2E 4d4f ldr r7, [pc, #308] literal 0x0F64 -> 0x20000628 */
L_0e30:
    ldr r5, [pc, #308]                      /* @0E30 4d4d ldr r5, [pc, #308] literal 0x0F68 -> 0x2000072C */
L_0e32:
    b L_0f3c                                /* @0E32 83e0 b #262 -> 0x0F3C */
L_0e34:
    bl L_1df0                               /* @0E34 00f0dcff bl #4024 -> sub_1DF0 */
L_0e38:
    strb r0, [r4]                           /* @0E38 2070 strb r0, [r4]  */
L_0e3a:
    ldrb r0, [r4]                           /* @0E3A 2078 ldrb r0, [r4]  */
L_0e3c:
    cmp r0, #4                              /* @0E3C 0428 cmp r0, #4  */
L_0e3e:
    bne L_0e58                              /* @0E3E 0bd1 bne #22 -> 0x0E58 */
L_0e40:
    adr r2, L_0f6c                          /* @0E40 4aa2 adr r2, #296 address 0x0F6C -> 0x00000F6C 'Error!' */
L_0e42:
    adr r1, L_0f74                          /* @0E42 4ca1 adr r1, #304 address 0x0F74 -> 0x00000F74 ' Line' */
L_0e44:
    adr r0, L_0f7c                          /* @0E44 4da0 adr r0, #308 address 0x0F7C -> 0x00000F7C 'Phase' */
L_0e46:
    bl L_4884                               /* @0E46 03f01dfd bl #14906 -> display_three_lines */
L_0e4a:
    b L_0e52                                /* @0E4A 02e0 b #4 -> 0x0E52 */
L_0e4c:
    bl L_1df0                               /* @0E4C 00f0d0ff bl #4000 -> sub_1DF0 */
L_0e50:
    strb r0, [r4]                           /* @0E50 2070 strb r0, [r4]  */
L_0e52:
    ldrb r0, [r4]                           /* @0E52 2078 ldrb r0, [r4]  */
L_0e54:
    cmp r0, #4                              /* @0E54 0428 cmp r0, #4  */
L_0e56:
    beq L_0e4c                              /* @0E56 f9d0 beq #-14 -> 0x0E4C */
L_0e58:
    ldrb r0, [r4]                           /* @0E58 2078 ldrb r0, [r4]  */
L_0e5a:
    cmp r0, #3                              /* @0E5A 0328 cmp r0, #3  */
L_0e5c:
    bne L_0ee2                              /* @0E5C 41d1 bne #130 -> 0x0EE2 */
L_0e5e:
    ldr r0, [pc, #292]                      /* @0E5E 4948 ldr r0, [pc, #292] literal 0x0F84 -> 0x20000620 */
L_0e60:
    ldrb r0, [r0]                           /* @0E60 0078 ldrb r0, [r0]  */
L_0e62:
    cmp r0, #1                              /* @0E62 0128 cmp r0, #1  */
L_0e64:
    beq L_0ea8                              /* @0E64 20d0 beq #64 -> 0x0EA8 */
L_0e66:
    cmp r0, #2                              /* @0E66 0228 cmp r0, #2  */
L_0e68:
    beq L_0eb4                              /* @0E68 24d0 beq #72 -> 0x0EB4 */
L_0e6a:
    cmp r0, #3                              /* @0E6A 0328 cmp r0, #3  */
L_0e6c:
    beq L_0ec0                              /* @0E6C 28d0 beq #80 -> 0x0EC0 */
L_0e6e:
    cmp r0, #4                              /* @0E6E 0428 cmp r0, #4  */
L_0e70:
    beq L_0ecc                              /* @0E70 2cd0 beq #88 -> 0x0ECC */
L_0e72:
    adr r2, L_0f88                          /* @0E72 45a2 adr r2, #276 address 0x0F88 -> 0x00000F88 'Enter..' */
L_0e74:
    adr r1, L_0f90                          /* @0E74 46a1 adr r1, #280 address 0x0F90 -> 0x00000F90 ' Error!' */
L_0e76:
    adr r0, L_0f98                          /* @0E76 48a0 adr r0, #288 address 0x0F98 -> 0x00000F98 'Encoder' */
L_0e78:
    bl L_4884                               /* @0E78 03f004fd bl #14856 -> display_three_lines */
L_0e7c:
    movs r1, #10                            /* @0E7C 0a21 movs r1, #10  */
L_0e7e:
    movs r0, #0                             /* @0E7E 0020 movs r0, #0  */
L_0e80:
    bl L_135c                               /* @0E80 00f06cfa bl #1240 -> sub_135C */
L_0e84:
    cmp r0, #1                              /* @0E84 0128 cmp r0, #1  */
L_0e86:
    beq L_0e7c                              /* @0E86 f9d0 beq #-14 -> 0x0E7C */
L_0e88:
    movs r0, #10                            /* @0E88 0a20 movs r0, #10  */
L_0e8a:
    bl L_2de8                               /* @0E8A 01f0adff bl #8026 -> delay_ms_candidate */
L_0e8e:
    movs r1, #10                            /* @0E8E 0a21 movs r1, #10  */
L_0e90:
    movs r0, #0                             /* @0E90 0020 movs r0, #0  */
L_0e92:
    bl L_135c                               /* @0E92 00f063fa bl #1222 -> sub_135C */
L_0e96:
    cmp r0, #0                              /* @0E96 0028 cmp r0, #0  */
L_0e98:
    beq L_0e8e                              /* @0E98 f9d0 beq #-14 -> 0x0E8E */
L_0e9a:
    str r6, [r7, #8]                        /* @0E9A be60 str r6, [r7, #8]  */
L_0e9c:
    ldr r0, [pc, #256]                      /* @0E9C 4048 ldr r0, [pc, #256] literal 0x0FA0 -> 0x20000560 */
L_0e9e:
    bl L_4574                               /* @0E9E 03f069fb bl #14034 -> sub_4574 */
L_0ea2:
    bl L_4568                               /* @0EA2 03f061fb bl #14018 -> sub_4568 */
L_0ea6:
    b L_0edc                                /* @0EA6 19e0 b #50 -> 0x0EDC */
L_0ea8:
    adr r2, L_0f88                          /* @0EA8 37a2 adr r2, #220 address 0x0F88 -> 0x00000F88 'Enter..' */
L_0eaa:
    adr r1, L_0fa4                          /* @0EAA 3ea1 adr r1, #248 address 0x0FA4 -> 0x00000FA4 ' Strong!' */
L_0eac:
    adr r0, L_0fb0                          /* @0EAC 40a0 adr r0, #256 address 0x0FB0 -> 0x00000FB0 'Magnet' */
L_0eae:
    bl L_4884                               /* @0EAE 03f0e9fc bl #14802 -> display_three_lines */
L_0eb2:
    b L_0e7c                                /* @0EB2 e3e7 b #-58 -> 0x0E7C */
L_0eb4:
    adr r2, L_0f88                          /* @0EB4 34a2 adr r2, #208 address 0x0F88 -> 0x00000F88 'Enter..' */
L_0eb6:
    adr r1, L_0fb8                          /* @0EB6 40a1 adr r1, #256 address 0x0FB8 -> 0x00000FB8 ' Weak!' */
L_0eb8:
    adr r0, L_0fb0                          /* @0EB8 3da0 adr r0, #244 address 0x0FB0 -> 0x00000FB0 'Magnet' */
L_0eba:
    bl L_4884                               /* @0EBA 03f0e3fc bl #14790 -> display_three_lines */
L_0ebe:
    b L_0e7c                                /* @0EBE dde7 b #-70 -> 0x0E7C */
L_0ec0:
    adr r2, L_0f88                          /* @0EC0 31a2 adr r2, #196 address 0x0F88 -> 0x00000F88 'Enter..' */
L_0ec2:
    adr r1, L_0fc0                          /* @0EC2 3fa1 adr r1, #252 address 0x0FC0 -> 0x00000FC0 ' Loss!' */
L_0ec4:
    adr r0, L_0fb0                          /* @0EC4 3aa0 adr r0, #232 address 0x0FB0 -> 0x00000FB0 'Magnet' */
L_0ec6:
    bl L_4884                               /* @0EC6 03f0ddfc bl #14778 -> display_three_lines */
L_0eca:
    b L_0e7c                                /* @0ECA d7e7 b #-82 -> 0x0E7C */
L_0ecc:
    adr r2, L_0f88                          /* @0ECC 2ea2 adr r2, #184 address 0x0F88 -> 0x00000F88 'Enter..' */
L_0ece:
    adr r1, L_0f90                          /* @0ECE 30a1 adr r1, #192 address 0x0F90 -> 0x00000F90 ' Error!' */
L_0ed0:
    adr r0, L_0f98                          /* @0ED0 31a0 adr r0, #196 address 0x0F98 -> 0x00000F98 'Encoder' */
L_0ed2:
    bl L_4884                               /* @0ED2 03f0d7fc bl #14766 -> display_three_lines */
L_0ed6:
    b L_0e7c                                /* @0ED6 d1e7 b #-94 -> 0x0E7C */
L_0ed8:
    bl L_1b28                               /* @0ED8 00f026fe bl #3148 -> foreground_ui_protection */
L_0edc:
    ldrb r0, [r5, #9]                       /* @0EDC 687a ldrb r0, [r5, #9]  */
L_0ede:
    cmp r0, #0                              /* @0EDE 0028 cmp r0, #0  */
L_0ee0:
    bne L_0ed8                              /* @0EE0 fad1 bne #-12 -> 0x0ED8 */
L_0ee2:
    ldrb r0, [r4]                           /* @0EE2 2078 ldrb r0, [r4]  */
L_0ee4:
    cmp r0, #1                              /* @0EE4 0128 cmp r0, #1  */
L_0ee6:
    bne L_0f00                              /* @0EE6 0bd1 bne #22 -> 0x0F00 */
L_0ee8:
    adr r2, L_0f6c                          /* @0EE8 20a2 adr r2, #128 address 0x0F6C -> 0x00000F6C 'Error!' */
L_0eea:
    adr r1, L_0fc8                          /* @0EEA 37a1 adr r1, #220 address 0x0FC8 -> 0x00000FC8 ' Type' */
L_0eec:
    adr r0, L_0fd0                          /* @0EEC 38a0 adr r0, #224 address 0x0FD0 -> 0x00000FD0 'Motor' */
L_0eee:
    bl L_4884                               /* @0EEE 03f0c9fc bl #14738 -> display_three_lines */
L_0ef2:
    b L_0efa                                /* @0EF2 02e0 b #4 -> 0x0EFA */
L_0ef4:
    bl L_1df0                               /* @0EF4 00f07cff bl #3832 -> sub_1DF0 */
L_0ef8:
    strb r0, [r4]                           /* @0EF8 2070 strb r0, [r4]  */
L_0efa:
    ldrb r0, [r4]                           /* @0EFA 2078 ldrb r0, [r4]  */
L_0efc:
    cmp r0, #1                              /* @0EFC 0128 cmp r0, #1  */
L_0efe:
    beq L_0ef4                              /* @0EFE f9d0 beq #-14 -> 0x0EF4 */
L_0f00:
    ldrb r0, [r4]                           /* @0F00 2078 ldrb r0, [r4]  */
L_0f02:
    cmp r0, #2                              /* @0F02 0228 cmp r0, #2  */
L_0f04:
    bne L_0f3c                              /* @0F04 1ad1 bne #52 -> 0x0F3C */
L_0f06:
    adr r2, L_0fac                          /* @0F06 29a2 adr r2, #164 address 0x0FAC -> 0x00000FAC */
L_0f08:
    adr r1, L_0fd8                          /* @0F08 33a1 adr r1, #204 address 0x0FD8 -> 0x00000FD8 'Not Cal' */
L_0f0a:
    adr r0, L_0fac                          /* @0F0A 28a0 adr r0, #160 address 0x0FAC -> 0x00000FAC */
L_0f0c:
    bl L_4884                               /* @0F0C 03f0bafc bl #14708 -> display_three_lines */
L_0f10:
    movs r0, #75                            /* @0F10 4b20 movs r0, #75  */
L_0f12:
    lsls r0, r0, #4                         /* @0F12 0001 lsls r0, r0, #4  */
L_0f14:
    bl L_2de8                               /* @0F14 01f068ff bl #7888 -> delay_ms_candidate */
L_0f18:
    str r6, [r7, #8]                        /* @0F18 be60 str r6, [r7, #8]  */
L_0f1a:
    ldr r0, [pc, #196]                      /* @0F1A 3148 ldr r0, [pc, #196] literal 0x0FE0 -> 0x200004FC */
L_0f1c:
    bl L_4574                               /* @0F1C 03f02afb bl #13908 -> sub_4574 */
L_0f20:
    bl L_4568                               /* @0F20 03f022fb bl #13892 -> sub_4568 */
L_0f24:
    bl L_342c                               /* @0F24 02f082fa bl #9476 -> sub_342C */
L_0f28:
    b L_0f2e                                /* @0F28 01e0 b #2 -> 0x0F2E */
L_0f2a:
    bl L_1b28                               /* @0F2A 00f0fdfd bl #3066 -> foreground_ui_protection */
L_0f2e:
    ldrb r0, [r5, #9]                       /* @0F2E 687a ldrb r0, [r5, #9]  */
L_0f30:
    cmp r0, #0                              /* @0F30 0028 cmp r0, #0  */
L_0f32:
    beq L_0f3c                              /* @0F32 03d0 beq #6 -> 0x0F3C */
L_0f34:
    bl L_267c                               /* @0F34 01f0a2fb bl #5956 -> load_calibration_knots */
L_0f38:
    cmp r0, #0                              /* @0F38 0028 cmp r0, #0  */
L_0f3a:
    beq L_0f2a                              /* @0F3A f6d0 beq #-20 -> 0x0F2A */
L_0f3c:
    ldrb r0, [r4]                           /* @0F3C 2078 ldrb r0, [r4]  */
L_0f3e:
    cmp r0, #0                              /* @0F3E 0028 cmp r0, #0  */
L_0f40:
    beq L_0f44                              /* @0F40 00d0 beq #0 -> 0x0F44 */
L_0f42:
    b L_0e34                                /* @0F42 77e7 b #-274 -> 0x0E34 */
L_0f44:
    bl L_5bd4                               /* @0F44 04f046fe bl #19596 -> sub_5BD4 */
L_0f48:
    bl L_5b84                               /* @0F48 04f01cfe bl #19512 -> sub_5B84 */
L_0f4c:
    movs r0, #1                             /* @0F4C 0120 movs r0, #1  */
L_0f4e:
    bl L_4dc8                               /* @0F4E 03f03bff bl #15990 -> origin_return_candidate */
L_0f52:
    bl L_3cc8                               /* @0F52 02f0b9fe bl #11634 -> sub_3CC8 */
L_0f56:
    ldr r0, [pc, #140]                      /* @0F56 2348 ldr r0, [pc, #140] literal 0x0FE4 -> 0x20000358 */
L_0f58:
    bl L_4574                               /* @0F58 03f00cfb bl #13848 -> sub_4574 */
L_0f5c:
    strb r6, [r7, #2]                       /* @0F5C be70 strb r6, [r7, #2]  */
L_0f5e:
    pop {r3, r4, r5, r6, r7, pc}            /* @0F5E f8bd pop {r3, r4, r5, r6, r7, pc}  */
L_0f60:
    .byte 0x48,0x06,0x00,0x20 /* @0F60 literal */
L_0f64:
    .byte 0x28,0x06,0x00,0x20 /* @0F64 literal */
L_0f68:
    .byte 0x2c,0x07,0x00,0x20 /* @0F68 literal */
L_0f6c:
    .byte 0x45,0x72,0x72,0x6f,0x72,0x21,0x00,0x00 /* @0F6C unclassified */
L_0f74:
    .byte 0x20,0x4c,0x69,0x6e,0x65,0x00,0x00,0x00 /* @0F74 unclassified */
L_0f7c:
    .byte 0x50,0x68,0x61,0x73,0x65,0x00,0x00,0x00 /* @0F7C unclassified */
L_0f84:
    .byte 0x20,0x06,0x00,0x20 /* @0F84 literal */
L_0f88:
    .byte 0x45,0x6e,0x74,0x65,0x72,0x2e,0x2e,0x00 /* @0F88 unclassified */
L_0f90:
    .byte 0x20,0x45,0x72,0x72,0x6f,0x72,0x21,0x00 /* @0F90 unclassified */
L_0f98:
    .byte 0x45,0x6e,0x63,0x6f,0x64,0x65,0x72,0x00 /* @0F98 unclassified */
L_0fa0:
    .byte 0x60,0x05,0x00,0x20 /* @0FA0 literal */
L_0fa4:
    .byte 0x20,0x53,0x74,0x72,0x6f,0x6e,0x67,0x21 /* @0FA4 unclassified */
L_0fac:
    .byte 0x00,0x00,0x00,0x00 /* @0FAC unclassified */
L_0fb0:
    .byte 0x4d,0x61,0x67,0x6e,0x65,0x74,0x00,0x00 /* @0FB0 unclassified */
L_0fb8:
    .byte 0x20,0x57,0x65,0x61,0x6b,0x21,0x00,0x00 /* @0FB8 unclassified */
L_0fc0:
    .byte 0x20,0x4c,0x6f,0x73,0x73,0x21,0x00,0x00 /* @0FC0 unclassified */
L_0fc8:
    .byte 0x20,0x54,0x79,0x70,0x65,0x00,0x00,0x00 /* @0FC8 unclassified */
L_0fd0:
    .byte 0x4d,0x6f,0x74,0x6f,0x72,0x00,0x00,0x00 /* @0FD0 unclassified */
L_0fd8:
    .byte 0x4e,0x6f,0x74,0x20,0x43,0x61,0x6c,0x00 /* @0FD8 unclassified */
L_0fe0:
    .byte 0xfc,0x04,0x00,0x20 /* @0FE0 literal */
L_0fe4:
    .byte 0x58,0x03,0x00,0x20 /* @0FE4 literal */
    .byte 0x00,0x00,0x00,0x00 /* @0FE8 unclassified */

/* sub_0FEC */
L_0fec:
    push {r4, r5, r6, lr}                   /* @0FEC 70b5 push {r4, r5, r6, lr}  */
L_0fee:
    lsls r4, r0, #27                        /* @0FEE c406 lsls r4, r0, #27  */
L_0ff0:
    lsrs r4, r4, #27                        /* @0FF0 e40e lsrs r4, r4, #27  */
L_0ff2:
    movs r3, #1                             /* @0FF2 0123 movs r3, #1  */
L_0ff4:
    lsls r3, r4                             /* @0FF4 a340 lsls r3, r4  */
L_0ff6:
    cmp r0, #0                              /* @0FF6 0028 cmp r0, #0  */
L_0ff8:
    blt L_0ffe                              /* @0FF8 01db blt #2 -> 0x0FFE */
L_0ffa:
    ldr r4, [pc, #96]                       /* @0FFA 184c ldr r4, [pc, #96] literal 0x105C -> 0xE000E280 */
L_0ffc:
    str r3, [r4]                            /* @0FFC 2360 str r3, [r4]  */
L_0ffe:
    lsls r4, r0, #30                        /* @0FFE 8407 lsls r4, r0, #30  */
L_1000:
    lsrs r4, r4, #27                        /* @1000 e40e lsrs r4, r4, #27  */
L_1002:
    movs r5, #255                           /* @1002 ff25 movs r5, #255  */
L_1004:
    lsls r5, r4                             /* @1004 a540 lsls r5, r4  */
L_1006:
    lsls r1, r1, #30                        /* @1006 8907 lsls r1, r1, #30  */
L_1008:
    lsrs r1, r1, #24                        /* @1008 090e lsrs r1, r1, #24  */
L_100a:
    lsls r1, r4                             /* @100A a140 lsls r1, r4  */
L_100c:
    cmp r0, #0                              /* @100C 0028 cmp r0, #0  */
L_100e:
    blt L_1022                              /* @100E 08db blt #16 -> 0x1022 */
L_1010:
    lsrs r6, r0, #2                         /* @1010 8608 lsrs r6, r0, #2  */
L_1012:
    ldr r4, [pc, #76]                       /* @1012 134c ldr r4, [pc, #76] literal 0x1060 -> 0xE000E400 */
L_1014:
    lsls r6, r6, #2                         /* @1014 b600 lsls r6, r6, #2  */
L_1016:
    adds r4, r6, r4                         /* @1016 3419 adds r4, r6, r4  */
L_1018:
    ldr r6, [r4]                            /* @1018 2668 ldr r6, [r4]  */
L_101a:
    bics r6, r5                             /* @101A ae43 bics r6, r5  */
L_101c:
    orrs r6, r1                             /* @101C 0e43 orrs r6, r1  */
L_101e:
    str r6, [r4]                            /* @101E 2660 str r6, [r4]  */
L_1020:
    b L_1038                                /* @1020 0ae0 b #20 -> 0x1038 */
L_1022:
    lsls r4, r0, #28                        /* @1022 0407 lsls r4, r0, #28  */
L_1024:
    lsrs r4, r4, #28                        /* @1024 240f lsrs r4, r4, #28  */
L_1026:
    subs r4, #8                             /* @1026 083c subs r4, #8  */
L_1028:
    lsrs r6, r4, #2                         /* @1028 a608 lsrs r6, r4, #2  */
L_102a:
    ldr r4, [pc, #56]                       /* @102A 0e4c ldr r4, [pc, #56] literal 0x1064 -> 0xE000ED00 */
L_102c:
    lsls r6, r6, #2                         /* @102C b600 lsls r6, r6, #2  */
L_102e:
    adds r4, r6, r4                         /* @102E 3419 adds r4, r6, r4  */
L_1030:
    ldr r6, [r4, #28]                       /* @1030 e669 ldr r6, [r4, #28]  */
L_1032:
    bics r6, r5                             /* @1032 ae43 bics r6, r5  */
L_1034:
    orrs r6, r1                             /* @1034 0e43 orrs r6, r1  */
L_1036:
    str r6, [r4, #28]                       /* @1036 e661 str r6, [r4, #28]  */
L_1038:
    cmp r2, #1                              /* @1038 012a cmp r2, #1  */
L_103a:
    beq L_104e                              /* @103A 08d0 beq #16 -> 0x104E */
L_103c:
    cmp r0, #0                              /* @103C 0028 cmp r0, #0  */
L_103e:
    blt L_104c                              /* @103E 05db blt #10 -> 0x104C */
L_1040:
    ldr r0, [pc, #36]                       /* @1040 0948 ldr r0, [pc, #36] literal 0x1068 -> 0xE000E180 */
L_1042:
    str r3, [r0]                            /* @1042 0360 str r3, [r0]  */
L_1044:
    dsb sy                                  /* @1044 bff34f8f dsb sy  */
L_1048:
    isb sy                                  /* @1048 bff36f8f isb sy  */
L_104c:
    pop {r4, r5, r6, pc}                    /* @104C 70bd pop {r4, r5, r6, pc}  */
L_104e:
    cmp r0, #0                              /* @104E 0028 cmp r0, #0  */
L_1050:
    blt L_104c                              /* @1050 fcdb blt #-8 -> 0x104C */
L_1052:
    ldr r0, [pc, #20]                       /* @1052 0548 ldr r0, [pc, #20] literal 0x1068 -> 0xE000E180 */
L_1054:
    subs r0, #128                           /* @1054 8038 subs r0, #128  */
L_1056:
    str r3, [r0]                            /* @1056 0360 str r3, [r0]  */
L_1058:
    pop {r4, r5, r6, pc}                    /* @1058 70bd pop {r4, r5, r6, pc}  */
    .byte 0x00,0x00 /* @105A unclassified */
L_105c:
    .byte 0x80,0xe2,0x00,0xe0 /* @105C literal */
L_1060:
    .byte 0x00,0xe4,0x00,0xe0 /* @1060 literal */
L_1064:
    .byte 0x00,0xed,0x00,0xe0 /* @1064 literal */
L_1068:
    .byte 0x80,0xe1,0x00,0xe0 /* @1068 literal */

/* sub_106C */
L_106c:
    push {r4, lr}                           /* @106C 10b5 push {r4, lr}  */
L_106e:
    mov r8, r8                              /* @106E c046 mov r8, r8  */
L_1070:
    mov r8, r8                              /* @1070 c046 mov r8, r8  */
L_1072:
    mov r8, r8                              /* @1072 c046 mov r8, r8  */
L_1074:
    mov r8, r8                              /* @1074 c046 mov r8, r8  */
L_1076:
    pop {r4, pc}                            /* @1076 10bd pop {r4, pc}  */

/* sub_1078 */
L_1078:
    push {r0, r1, r4, r5, r6, r7, lr}       /* @1078 f3b5 push {r0, r1, r4, r5, r6, r7, lr}  */
L_107a:
    sub sp, #44                             /* @107A 8bb0 sub sp, #44  */
L_107c:
    mov r4, r0                              /* @107C 0446 mov r4, r0  */
L_107e:
    movs r5, #0                             /* @107E 0025 movs r5, #0  */
L_1080:
    movs r7, #255                           /* @1080 ff27 movs r7, #255  */
L_1082:
    str r7, [sp, #40]                       /* @1082 0a97 str r7, [sp, #40]  */
L_1084:
    movs r0, #0                             /* @1084 0020 movs r0, #0  */
L_1086:
    str r0, [sp, #36]                       /* @1086 0990 str r0, [sp, #36]  */
L_1088:
    movs r1, #32                            /* @1088 2021 movs r1, #32  */
L_108a:
    add r0, sp, #4                          /* @108A 01a8 add r0, sp, #4  */
L_108c:
    bl L_025c                               /* @108C fff7e6f8 bl #-3636 -> sub_025C */
L_1090:
    ldr r2, [pc, #124]                      /* @1090 1f4a ldr r2, [pc, #124] literal 0x1110 -> 0x40020000 */
L_1092:
    cmp r4, #1                              /* @1092 012c cmp r4, #1  */
L_1094:
    beq L_10b0                              /* @1094 0cd0 beq #24 -> 0x10B0 */
L_1096:
    cmp r4, #2                              /* @1096 022c cmp r4, #2  */
L_1098:
    beq L_10b0                              /* @1098 0ad0 beq #20 -> 0x10B0 */
L_109a:
    cmp r4, #4                              /* @109A 042c cmp r4, #4  */
L_109c:
    beq L_10b0                              /* @109C 08d0 beq #16 -> 0x10B0 */
L_109e:
    cmp r4, #6                              /* @109E 062c cmp r4, #6  */
L_10a0:
    beq L_10b0                              /* @10A0 06d0 beq #12 -> 0x10B0 */
L_10a2:
    cmp r4, #8                              /* @10A2 082c cmp r4, #8  */
L_10a4:
    beq L_10b0                              /* @10A4 04d0 beq #8 -> 0x10B0 */
L_10a6:
    cmp r4, #12                             /* @10A6 0c2c cmp r4, #12  */
L_10a8:
    beq L_10b0                              /* @10A8 02d0 beq #4 -> 0x10B0 */
L_10aa:
    movs r0, #4                             /* @10AA 0420 movs r0, #4  */
L_10ac:
    add sp, #52                             /* @10AC 0db0 add sp, #52  */
L_10ae:
    pop {r4, r5, r6, r7, pc}                /* @10AE f0bd pop {r4, r5, r6, r7, pc}  */
L_10b0:
    ldr r0, [r2, #32]                       /* @10B0 106a ldr r0, [r2, #32]  */
L_10b2:
    mov r6, r2                              /* @10B2 1646 mov r6, r2  */
L_10b4:
    movs r1, #1                             /* @10B4 0121 movs r1, #1  */
L_10b6:
    lsls r1, r1, #9                         /* @10B6 4902 lsls r1, r1, #9  */
L_10b8:
    bics r0, r1                             /* @10B8 8843 bics r0, r1  */
L_10ba:
    ldr r1, [sp, #48]                       /* @10BA 0c99 ldr r1, [sp, #48]  */
L_10bc:
    lsls r1, r1, #31                        /* @10BC c907 lsls r1, r1, #31  */
L_10be:
    lsrs r1, r1, #22                        /* @10BE 890d lsrs r1, r1, #22  */
L_10c0:
    orrs r0, r1                             /* @10C0 0843 orrs r0, r1  */
L_10c2:
    str r0, [r2, #32]                       /* @10C2 1062 str r0, [r2, #32]  */
L_10c4:
    add r3, sp, #4                          /* @10C4 01ab add r3, sp, #4  */
L_10c6:
    ldr r1, [pc, #76]                       /* @10C6 1349 ldr r1, [pc, #76] literal 0x1114 -> 0x000021A4 */
L_10c8:
    lsls r0, r5, #2                         /* @10C8 a800 lsls r0, r5, #2  */
L_10ca:
    ldr r1, [r1, r0]                        /* @10CA 0958 ldr r1, [r1, r0]  */
L_10cc:
    muls r1, r4, r1                         /* @10CC 6143 muls r1, r4, r1  */
L_10ce:
    str r1, [r3, r0]                        /* @10CE 1950 str r1, [r3, r0]  */
L_10d0:
    adds r5, r5, #1                         /* @10D0 6d1c adds r5, r5, #1  */
L_10d2:
    cmp r5, #8                              /* @10D2 082d cmp r5, #8  */
L_10d4:
    blo L_10c6                              /* @10D4 f7d3 blo #-18 -> 0x10C6 */
L_10d6:
    cmp r4, #12                             /* @10D6 0c2c cmp r4, #12  */
L_10d8:
    bne L_10dc                              /* @10D8 00d1 bne #0 -> 0x10DC */
L_10da:
    str r7, [sp, #8]                        /* @10DA 0297 str r7, [sp, #8]  */
L_10dc:
    movs r1, #0                             /* @10DC 0021 movs r1, #0  */
L_10de:
    mov r5, r3                              /* @10DE 1d46 mov r5, r3  */
L_10e0:
    movs r0, #255                           /* @10E0 ff20 movs r0, #255  */
L_10e2:
    str r0, [sp, #40]                       /* @10E2 0a90 str r0, [sp, #40]  */
L_10e4:
    lsls r0, r1, #2                         /* @10E4 8800 lsls r0, r1, #2  */
L_10e6:
    b L_10fa                                /* @10E6 08e0 b #16 -> 0x10FA */
L_10e8:
    ldr r4, [sp, #40]                       /* @10E8 0a9c ldr r4, [sp, #40]  */
L_10ea:
    subs r7, r4, #1                         /* @10EA 671e subs r7, r4, #1  */
L_10ec:
    str r7, [sp, #40]                       /* @10EC 0a97 str r7, [sp, #40]  */
L_10ee:
    blo L_110c                              /* @10EE 0dd3 blo #26 -> 0x110C */
L_10f0:
    ldr r4, [pc, #36]                       /* @10F0 094c ldr r4, [pc, #36] literal 0x1118 -> 0x00005A5A */
L_10f2:
    str r4, [r6, #44]                       /* @10F2 f462 str r4, [r6, #44]  */
L_10f4:
    ldr r4, [pc, #36]                       /* @10F4 094c ldr r4, [pc, #36] literal 0x111C -> 0x0000A5A5 */
L_10f6:
    str r4, [r6, #44]                       /* @10F6 f462 str r4, [r6, #44]  */
L_10f8:
    str r3, [r2, r0]                        /* @10F8 1350 str r3, [r2, r0]  */
L_10fa:
    ldr r4, [r2, r0]                        /* @10FA 1458 ldr r4, [r2, r0]  */
L_10fc:
    ldr r3, [r5, r0]                        /* @10FC 2b58 ldr r3, [r5, r0]  */
L_10fe:
    cmp r4, r3                              /* @10FE 9c42 cmp r4, r3  */
L_1100:
    bne L_10e8                              /* @1100 f2d1 bne #-28 -> 0x10E8 */
L_1102:
    adds r1, r1, #1                         /* @1102 491c adds r1, r1, #1  */
L_1104:
    cmp r1, #8                              /* @1104 0829 cmp r1, #8  */
L_1106:
    blo L_10e0                              /* @1106 ebd3 blo #-42 -> 0x10E0 */
L_1108:
    ldr r0, [sp, #36]                       /* @1108 0998 ldr r0, [sp, #36]  */
L_110a:
    b L_10ac                                /* @110A cfe7 b #-98 -> 0x10AC */
L_110c:
    movs r0, #7                             /* @110C 0720 movs r0, #7  */
L_110e:
    b L_10ac                                /* @110E cde7 b #-102 -> 0x10AC */
L_1110:
    .byte 0x00,0x00,0x02,0x40 /* @1110 literal */
L_1114:
    .byte 0xa4,0x21,0x00,0x00 /* @1114 literal */
L_1118:
    .byte 0x5a,0x5a,0x00,0x00 /* @1118 literal */
L_111c:
    .byte 0xa5,0xa5,0x00,0x00 /* @111C literal */

/* sub_1120 */
L_1120:
    ldr r0, [pc, #16]                       /* @1120 0448 ldr r0, [pc, #16] literal 0x1134 -> 0x40020000 */
L_1122:
    ldr r1, [pc, #12]                       /* @1122 0349 ldr r1, [pc, #12] literal 0x1130 -> 0x00005A5A */
L_1124:
    str r1, [r0, #44]                       /* @1124 c162 str r1, [r0, #44]  */
L_1126:
    ldr r1, [pc, #16]                       /* @1126 0449 ldr r1, [pc, #16] literal 0x1138 -> 0x0000A5A5 */
L_1128:
    str r1, [r0, #44]                       /* @1128 c162 str r1, [r0, #44]  */
L_112a:
    movs r1, #0                             /* @112A 0021 movs r1, #0  */
L_112c:
    str r1, [r0, #48]                       /* @112C 0163 str r1, [r0, #48]  */
L_112e:
    bx lr                                   /* @112E 7047 bx lr  */
L_1130:
    .byte 0x5a,0x5a,0x00,0x00 /* @1130 literal */
L_1134:
    .byte 0x00,0x00,0x02,0x40 /* @1134 literal */
L_1138:
    .byte 0xa5,0xa5,0x00,0x00 /* @1138 literal */

/* sub_113C */
L_113c:
    push {r3, r4, r5, r6, r7, lr}           /* @113C f8b5 push {r3, r4, r5, r6, r7, lr}  */
L_113e:
    mov r4, r1                              /* @113E 0c46 mov r4, r1  */
L_1140:
    mov r7, r2                              /* @1140 1746 mov r7, r2  */
L_1142:
    mov r6, r0                              /* @1142 0646 mov r6, r0  */
L_1144:
    bl L_1188                               /* @1144 00f020f8 bl #64 -> sub_1188 */
L_1148:
    movs r5, #0                             /* @1148 0025 movs r5, #0  */
L_114a:
    b L_115c                                /* @114A 07e0 b #14 -> 0x115C */
L_114c:
    ldrh r1, [r4]                           /* @114C 2188 ldrh r1, [r4]  */
L_114e:
    mov r0, r6                              /* @114E 3046 mov r0, r6  */
L_1150:
    bl L_1260                               /* @1150 00f086f8 bl #268 -> sub_1260 */
L_1154:
    adds r6, r6, #2                         /* @1154 b61c adds r6, r6, #2  */
L_1156:
    adds r4, r4, #2                         /* @1156 a41c adds r4, r4, #2  */
L_1158:
    adds r5, r5, #1                         /* @1158 6d1c adds r5, r5, #1  */
L_115a:
    uxth r5, r5                             /* @115A adb2 uxth r5, r5  */
L_115c:
    cmp r5, r7                              /* @115C bd42 cmp r5, r7  */
L_115e:
    blo L_114c                              /* @115E f5d3 blo #-22 -> 0x114C */
L_1160:
    pop {r3, r4, r5, r6, r7, pc}            /* @1160 f8bd pop {r3, r4, r5, r6, r7, pc}  */
    .byte 0x00,0x00 /* @1162 unclassified */

/* sub_1164 */
L_1164:
    push {r3, r4, r5, r6, r7, lr}           /* @1164 f8b5 push {r3, r4, r5, r6, r7, lr}  */
L_1166:
    mov r6, r0                              /* @1166 0646 mov r6, r0  */
L_1168:
    mov r5, r1                              /* @1168 0d46 mov r5, r1  */
L_116a:
    mov r7, r2                              /* @116A 1746 mov r7, r2  */
L_116c:
    movs r4, #0                             /* @116C 0024 movs r4, #0  */
L_116e:
    b L_1180                                /* @116E 07e0 b #14 -> 0x1180 */
L_1170:
    ldrh r1, [r5]                           /* @1170 2988 ldrh r1, [r5]  */
L_1172:
    mov r0, r6                              /* @1172 3046 mov r0, r6  */
L_1174:
    bl L_1260                               /* @1174 00f074f8 bl #232 -> sub_1260 */
L_1178:
    adds r6, r6, #2                         /* @1178 b61c adds r6, r6, #2  */
L_117a:
    adds r5, r5, #2                         /* @117A ad1c adds r5, r5, #2  */
L_117c:
    adds r4, r4, #1                         /* @117C 641c adds r4, r4, #1  */
L_117e:
    uxth r4, r4                             /* @117E a4b2 uxth r4, r4  */
L_1180:
    cmp r4, r7                              /* @1180 bc42 cmp r4, r7  */
L_1182:
    blo L_1170                              /* @1182 f5d3 blo #-22 -> 0x1170 */
L_1184:
    pop {r3, r4, r5, r6, r7, pc}            /* @1184 f8bd pop {r3, r4, r5, r6, r7, pc}  */
    .byte 0x00,0x00 /* @1186 unclassified */

/* sub_1188 */
L_1188:
    push {r3, r4, r5, r6, r7, lr}           /* @1188 f8b5 push {r3, r4, r5, r6, r7, lr}  */
L_118a:
    mov r3, r0                              /* @118A 0346 mov r3, r0  */
L_118c:
    movs r6, #0                             /* @118C 0026 movs r6, #0  */
L_118e:
    movs r5, #255                           /* @118E ff25 movs r5, #255  */
L_1190:
    str r5, [sp]                            /* @1190 0095 str r5, [sp]  */
L_1192:
    ldr r0, [pc, #112]                      /* @1192 1c48 ldr r0, [pc, #112] literal 0x1204 -> 0x0000FFFF */
L_1194:
    cmp r3, r0                              /* @1194 8342 cmp r3, r0  */
L_1196:
    bls L_119c                              /* @1196 01d9 bls #2 -> 0x119C */
L_1198:
    movs r0, #4                             /* @1198 0420 movs r0, #4  */
L_119a:
    pop {r3, r4, r5, r6, r7, pc}            /* @119A f8bd pop {r3, r4, r5, r6, r7, pc}  */
L_119c:
    ldr r4, [pc, #104]                      /* @119C 1a4c ldr r4, [pc, #104] literal 0x1208 -> 0x40020000 */
L_119e:
    b L_11ac                                /* @119E 05e0 b #10 -> 0x11AC */
L_11a0:
    ldr r0, [sp]                            /* @11A0 0098 ldr r0, [sp]  */
L_11a2:
    subs r1, r0, #1                         /* @11A2 411e subs r1, r0, #1  */
L_11a4:
    str r1, [sp]                            /* @11A4 0091 str r1, [sp]  */
L_11a6:
    bhs L_11ac                              /* @11A6 01d2 bhs #2 -> 0x11AC */
L_11a8:
    movs r0, #9                             /* @11A8 0920 movs r0, #9  */
L_11aa:
    pop {r3, r4, r5, r6, r7, pc}            /* @11AA f8bd pop {r3, r4, r5, r6, r7, pc}  */
L_11ac:
    ldr r0, [r4, #32]                       /* @11AC 206a ldr r0, [r4, #32]  */
L_11ae:
    lsls r0, r0, #27                        /* @11AE c006 lsls r0, r0, #27  */
L_11b0:
    bmi L_11a0                              /* @11B0 f6d4 bmi #-20 -> 0x11A0 */
L_11b2:
    bl L_1214                               /* @11B2 00f02ff8 bl #94 -> sub_1214 */
L_11b6:
    str r5, [sp]                            /* @11B6 0095 str r5, [sp]  */
L_11b8:
    ldr r0, [pc, #80]                       /* @11B8 1448 ldr r0, [pc, #80] literal 0x120C -> 0x00005A5A */
L_11ba:
    ldr r7, [pc, #84]                       /* @11BA 154f ldr r7, [pc, #84] literal 0x1210 -> 0x0000A5A5 */
L_11bc:
    b L_11d4                                /* @11BC 0ae0 b #20 -> 0x11D4 */
L_11be:
    ldr r1, [sp]                            /* @11BE 0099 ldr r1, [sp]  */
L_11c0:
    subs r2, r1, #1                         /* @11C0 4a1e subs r2, r1, #1  */
L_11c2:
    str r2, [sp]                            /* @11C2 0092 str r2, [sp]  */
L_11c4:
    blo L_11e6                              /* @11C4 0fd3 blo #30 -> 0x11E6 */
L_11c6:
    str r0, [r4, #44]                       /* @11C6 e062 str r0, [r4, #44]  */
L_11c8:
    str r7, [r4, #44]                       /* @11C8 e762 str r7, [r4, #44]  */
L_11ca:
    ldr r1, [r4, #32]                       /* @11CA 216a ldr r1, [r4, #32]  */
L_11cc:
    lsrs r1, r1, #2                         /* @11CC 8908 lsrs r1, r1, #2  */
L_11ce:
    lsls r1, r1, #2                         /* @11CE 8900 lsls r1, r1, #2  */
L_11d0:
    adds r1, r1, #2                         /* @11D0 891c adds r1, r1, #2  */
L_11d2:
    str r1, [r4, #32]                       /* @11D2 2162 str r1, [r4, #32]  */
L_11d4:
    ldr r1, [r4, #32]                       /* @11D4 216a ldr r1, [r4, #32]  */
L_11d6:
    lsls r1, r1, #30                        /* @11D6 8907 lsls r1, r1, #30  */
L_11d8:
    lsrs r1, r1, #30                        /* @11D8 890f lsrs r1, r1, #30  */
L_11da:
    cmp r1, #2                              /* @11DA 0229 cmp r1, #2  */
L_11dc:
    bne L_11be                              /* @11DC efd1 bne #-34 -> 0x11BE */
L_11de:
    movs r0, #0                             /* @11DE 0020 movs r0, #0  */
L_11e0:
    strb r0, [r3]                           /* @11E0 1870 strb r0, [r3]  */
L_11e2:
    str r5, [sp]                            /* @11E2 0095 str r5, [sp]  */
L_11e4:
    b L_11f6                                /* @11E4 07e0 b #14 -> 0x11F6 */
L_11e6:
    movs r0, #9                             /* @11E6 0920 movs r0, #9  */
L_11e8:
    pop {r3, r4, r5, r6, r7, pc}            /* @11E8 f8bd pop {r3, r4, r5, r6, r7, pc}  */
L_11ea:
    ldr r0, [sp]                            /* @11EA 0098 ldr r0, [sp]  */
L_11ec:
    subs r1, r0, #1                         /* @11EC 411e subs r1, r0, #1  */
L_11ee:
    str r1, [sp]                            /* @11EE 0091 str r1, [sp]  */
L_11f0:
    bhs L_11f6                              /* @11F0 01d2 bhs #2 -> 0x11F6 */
L_11f2:
    movs r0, #9                             /* @11F2 0920 movs r0, #9  */
L_11f4:
    pop {r3, r4, r5, r6, r7, pc}            /* @11F4 f8bd pop {r3, r4, r5, r6, r7, pc}  */
L_11f6:
    ldr r0, [r4, #32]                       /* @11F6 206a ldr r0, [r4, #32]  */
L_11f8:
    lsls r0, r0, #27                        /* @11F8 c006 lsls r0, r0, #27  */
L_11fa:
    bmi L_11ea                              /* @11FA f6d4 bmi #-20 -> 0x11EA */
L_11fc:
    bl L_1120                               /* @11FC fff790ff bl #-224 -> sub_1120 */
L_1200:
    mov r0, r6                              /* @1200 3046 mov r0, r6  */
L_1202:
    pop {r3, r4, r5, r6, r7, pc}            /* @1202 f8bd pop {r3, r4, r5, r6, r7, pc}  */
L_1204:
    .byte 0xff,0xff,0x00,0x00 /* @1204 literal */
L_1208:
    .byte 0x00,0x00,0x02,0x40 /* @1208 literal */
L_120c:
    .byte 0x5a,0x5a,0x00,0x00 /* @120C literal */
L_1210:
    .byte 0xa5,0xa5,0x00,0x00 /* @1210 literal */

/* sub_1214 */
L_1214:
    ldr r0, [pc, #20]                       /* @1214 0548 ldr r0, [pc, #20] literal 0x122C -> 0x40020000 */
L_1216:
    ldr r1, [pc, #16]                       /* @1216 0449 ldr r1, [pc, #16] literal 0x1228 -> 0x00005A5A */
L_1218:
    str r1, [r0, #44]                       /* @1218 c162 str r1, [r0, #44]  */
L_121a:
    ldr r1, [pc, #20]                       /* @121A 0549 ldr r1, [pc, #20] literal 0x1230 -> 0x0000A5A5 */
L_121c:
    str r1, [r0, #44]                       /* @121C c162 str r1, [r0, #44]  */
L_121e:
    movs r1, #0                             /* @121E 0021 movs r1, #0  */
L_1220:
    mvns r1, r1                             /* @1220 c943 mvns r1, r1  */
L_1222:
    str r1, [r0, #48]                       /* @1222 0163 str r1, [r0, #48]  */
L_1224:
    bx lr                                   /* @1224 7047 bx lr  */
    .byte 0x00,0x00 /* @1226 unclassified */
L_1228:
    .byte 0x5a,0x5a,0x00,0x00 /* @1228 literal */
L_122c:
    .byte 0x00,0x00,0x02,0x40 /* @122C literal */
L_1230:
    .byte 0xa5,0xa5,0x00,0x00 /* @1230 literal */

/* sub_1234 */
L_1234:
    push {r4, lr}                           /* @1234 10b5 push {r4, lr}  */
L_1236:
    mov r2, r0                              /* @1236 0246 mov r2, r0  */
L_1238:
    movs r0, #0                             /* @1238 0020 movs r0, #0  */
L_123a:
    ldr r1, [pc, #28]                       /* @123A 0749 ldr r1, [pc, #28] literal 0x1258 -> 0x40020000 */
L_123c:
    ldr r3, [pc, #20]                       /* @123C 054b ldr r3, [pc, #20] literal 0x1254 -> 0x00005A5A */
L_123e:
    str r3, [r1, #44]                       /* @123E cb62 str r3, [r1, #44]  */
L_1240:
    ldr r3, [pc, #24]                       /* @1240 064b ldr r3, [pc, #24] literal 0x125C -> 0x0000A5A5 */
L_1242:
    str r3, [r1, #44]                       /* @1242 cb62 str r3, [r1, #44]  */
L_1244:
    ldr r3, [r1, #32]                       /* @1244 0b6a ldr r3, [r1, #32]  */
L_1246:
    movs r4, #12                            /* @1246 0c24 movs r4, #12  */
L_1248:
    lsls r2, r2, #30                        /* @1248 9207 lsls r2, r2, #30  */
L_124a:
    bics r3, r4                             /* @124A a343 bics r3, r4  */
L_124c:
    lsrs r2, r2, #28                        /* @124C 120f lsrs r2, r2, #28  */
L_124e:
    orrs r3, r2                             /* @124E 1343 orrs r3, r2  */
L_1250:
    str r3, [r1, #32]                       /* @1250 0b62 str r3, [r1, #32]  */
L_1252:
    pop {r4, pc}                            /* @1252 10bd pop {r4, pc}  */
L_1254:
    .byte 0x5a,0x5a,0x00,0x00 /* @1254 literal */
L_1258:
    .byte 0x00,0x00,0x02,0x40 /* @1258 literal */
L_125c:
    .byte 0xa5,0xa5,0x00,0x00 /* @125C literal */

/* sub_1260 */
L_1260:
    push {r2, r3, r4, r5, r6, r7, lr}       /* @1260 fcb5 push {r2, r3, r4, r5, r6, r7, lr}  */
L_1262:
    mov r5, r0                              /* @1262 0546 mov r5, r0  */
L_1264:
    mov r7, r1                              /* @1264 0f46 mov r7, r1  */
L_1266:
    movs r0, #0                             /* @1266 0020 movs r0, #0  */
L_1268:
    str r0, [sp, #4]                        /* @1268 0190 str r0, [sp, #4]  */
L_126a:
    movs r6, #255                           /* @126A ff26 movs r6, #255  */
L_126c:
    str r6, [sp]                            /* @126C 0096 str r6, [sp]  */
L_126e:
    ldr r0, [pc, #112]                      /* @126E 1c48 ldr r0, [pc, #112] literal 0x12E0 -> 0x0000FFFF */
L_1270:
    cmp r5, r0                              /* @1270 8542 cmp r5, r0  */
L_1272:
    bls L_1278                              /* @1272 01d9 bls #2 -> 0x1278 */
L_1274:
    movs r0, #4                             /* @1274 0420 movs r0, #4  */
L_1276:
    pop {r2, r3, r4, r5, r6, r7, pc}        /* @1276 fcbd pop {r2, r3, r4, r5, r6, r7, pc}  */
L_1278:
    ldr r2, [pc, #104]                      /* @1278 1a4a ldr r2, [pc, #104] literal 0x12E4 -> 0x40020000 */
L_127a:
    b L_1288                                /* @127A 05e0 b #10 -> 0x1288 */
L_127c:
    ldr r0, [sp]                            /* @127C 0098 ldr r0, [sp]  */
L_127e:
    subs r0, r0, #1                         /* @127E 401e subs r0, r0, #1  */
L_1280:
    str r0, [sp]                            /* @1280 0090 str r0, [sp]  */
L_1282:
    bhs L_1288                              /* @1282 01d2 bhs #2 -> 0x1288 */
L_1284:
    movs r0, #9                             /* @1284 0920 movs r0, #9  */
L_1286:
    pop {r2, r3, r4, r5, r6, r7, pc}        /* @1286 fcbd pop {r2, r3, r4, r5, r6, r7, pc}  */
L_1288:
    ldr r0, [r2, #32]                       /* @1288 106a ldr r0, [r2, #32]  */
L_128a:
    lsls r0, r0, #27                        /* @128A c006 lsls r0, r0, #27  */
L_128c:
    bmi L_127c                              /* @128C f6d4 bmi #-20 -> 0x127C */
L_128e:
    str r6, [sp]                            /* @128E 0096 str r6, [sp]  */
L_1290:
    ldr r0, [pc, #84]                       /* @1290 1548 ldr r0, [pc, #84] literal 0x12E8 -> 0x00005A5A */
L_1292:
    ldr r1, [pc, #88]                       /* @1292 1649 ldr r1, [pc, #88] literal 0x12EC -> 0x0000A5A5 */
L_1294:
    b L_12ac                                /* @1294 0ae0 b #20 -> 0x12AC */
L_1296:
    ldr r3, [sp]                            /* @1296 009b ldr r3, [sp]  */
L_1298:
    subs r4, r3, #1                         /* @1298 5c1e subs r4, r3, #1  */
L_129a:
    str r4, [sp]                            /* @129A 0094 str r4, [sp]  */
L_129c:
    blo L_12c0                              /* @129C 10d3 blo #32 -> 0x12C0 */
L_129e:
    str r0, [r2, #44]                       /* @129E d062 str r0, [r2, #44]  */
L_12a0:
    str r1, [r2, #44]                       /* @12A0 d162 str r1, [r2, #44]  */
L_12a2:
    ldr r3, [r2, #32]                       /* @12A2 136a ldr r3, [r2, #32]  */
L_12a4:
    lsrs r3, r3, #2                         /* @12A4 9b08 lsrs r3, r3, #2  */
L_12a6:
    lsls r3, r3, #2                         /* @12A6 9b00 lsls r3, r3, #2  */
L_12a8:
    adds r3, r3, #1                         /* @12A8 5b1c adds r3, r3, #1  */
L_12aa:
    str r3, [r2, #32]                       /* @12AA 1362 str r3, [r2, #32]  */
L_12ac:
    ldr r3, [r2, #32]                       /* @12AC 136a ldr r3, [r2, #32]  */
L_12ae:
    lsls r3, r3, #30                        /* @12AE 9b07 lsls r3, r3, #30  */
L_12b0:
    lsrs r3, r3, #30                        /* @12B0 9b0f lsrs r3, r3, #30  */
L_12b2:
    cmp r3, #1                              /* @12B2 012b cmp r3, #1  */
L_12b4:
    bne L_1296                              /* @12B4 efd1 bne #-34 -> 0x1296 */
L_12b6:
    bl L_1214                               /* @12B6 fff7adff bl #-166 -> sub_1214 */
L_12ba:
    strh r7, [r5]                           /* @12BA 2f80 strh r7, [r5]  */
L_12bc:
    str r6, [sp]                            /* @12BC 0096 str r6, [sp]  */
L_12be:
    b L_12d0                                /* @12BE 07e0 b #14 -> 0x12D0 */
L_12c0:
    movs r0, #9                             /* @12C0 0920 movs r0, #9  */
L_12c2:
    pop {r2, r3, r4, r5, r6, r7, pc}        /* @12C2 fcbd pop {r2, r3, r4, r5, r6, r7, pc}  */
L_12c4:
    ldr r0, [sp]                            /* @12C4 0098 ldr r0, [sp]  */
L_12c6:
    subs r1, r0, #1                         /* @12C6 411e subs r1, r0, #1  */
L_12c8:
    str r1, [sp]                            /* @12C8 0091 str r1, [sp]  */
L_12ca:
    bhs L_12d0                              /* @12CA 01d2 bhs #2 -> 0x12D0 */
L_12cc:
    movs r0, #9                             /* @12CC 0920 movs r0, #9  */
L_12ce:
    pop {r2, r3, r4, r5, r6, r7, pc}        /* @12CE fcbd pop {r2, r3, r4, r5, r6, r7, pc}  */
L_12d0:
    ldr r0, [r2, #32]                       /* @12D0 106a ldr r0, [r2, #32]  */
L_12d2:
    lsls r0, r0, #27                        /* @12D2 c006 lsls r0, r0, #27  */
L_12d4:
    bmi L_12c4                              /* @12D4 f6d4 bmi #-20 -> 0x12C4 */
L_12d6:
    bl L_1120                               /* @12D6 fff723ff bl #-442 -> sub_1120 */
L_12da:
    ldr r0, [sp, #4]                        /* @12DA 0198 ldr r0, [sp, #4]  */
L_12dc:
    pop {r2, r3, r4, r5, r6, r7, pc}        /* @12DC fcbd pop {r2, r3, r4, r5, r6, r7, pc}  */
    .byte 0x00,0x00 /* @12DE unclassified */
L_12e0:
    .byte 0xff,0xff,0x00,0x00 /* @12E0 literal */
L_12e4:
    .byte 0x00,0x00,0x02,0x40 /* @12E4 literal */
L_12e8:
    .byte 0x5a,0x5a,0x00,0x00 /* @12E8 literal */
L_12ec:
    .byte 0xa5,0xa5,0x00,0x00 /* @12EC literal */

/* sub_12F0 */
L_12f0:
    ldrh r0, [r0]                           /* @12F0 0088 ldrh r0, [r0]  */
L_12f2:
    bx lr                                   /* @12F2 7047 bx lr  */

/* sub_12F4 */
L_12f4:
    ldr r0, [r0]                            /* @12F4 0068 ldr r0, [r0]  */
L_12f6:
    bx lr                                   /* @12F6 7047 bx lr  */

/* sub_12F8 */
L_12f8:
    ldr r0, [r0]                            /* @12F8 0068 ldr r0, [r0]  */
L_12fa:
    lsrs r0, r1                             /* @12FA c840 lsrs r0, r1  */
L_12fc:
    lsls r0, r0, #31                        /* @12FC c007 lsls r0, r0, #31  */
L_12fe:
    lsrs r0, r0, #31                        /* @12FE c00f lsrs r0, r0, #31  */
L_1300:
    bx lr                                   /* @1300 7047 bx lr  */
    .byte 0x00,0x00 /* @1302 unclassified */

/* sub_1304 */
L_1304:
    push {r4, lr}                           /* @1304 10b5 push {r4, lr}  */
L_1306:
    ldr r3, [pc, #12]                       /* @1306 034b ldr r3, [pc, #12] literal 0x1314 -> 0x40020E10 */
L_1308:
    movs r2, #0                             /* @1308 0022 movs r2, #0  */
L_130a:
    adds r0, r0, r3                         /* @130A c018 adds r0, r0, r3  */
L_130c:
    bl L_16ac                               /* @130C 00f0cef9 bl #924 -> sub_16AC */
L_1310:
    movs r0, #0                             /* @1310 0020 movs r0, #0  */
L_1312:
    pop {r4, pc}                            /* @1312 10bd pop {r4, pc}  */
L_1314:
    .byte 0x10,0x0e,0x02,0x40 /* @1314 literal */

/* sub_1318 */
L_1318:
    push {r4, lr}                           /* @1318 10b5 push {r4, lr}  */
L_131a:
    ldr r3, [pc, #12]                       /* @131A 034b ldr r3, [pc, #12] literal 0x1328 -> 0x40020D14 */
L_131c:
    movs r2, #1                             /* @131C 0122 movs r2, #1  */
L_131e:
    adds r0, r0, r3                         /* @131E c018 adds r0, r0, r3  */
L_1320:
    bl L_16ac                               /* @1320 00f0c4f9 bl #904 -> sub_16AC */
L_1324:
    movs r0, #0                             /* @1324 0020 movs r0, #0  */
L_1326:
    pop {r4, pc}                            /* @1326 10bd pop {r4, pc}  */
L_1328:
    .byte 0x14,0x0d,0x02,0x40 /* @1328 literal */

/* sub_132C */
L_132c:
    push {r4, lr}                           /* @132C 10b5 push {r4, lr}  */
L_132e:
    lsls r2, r2, #2                         /* @132E 9200 lsls r2, r2, #2  */
L_1330:
    adds r0, r2, r0                         /* @1330 1018 adds r0, r2, r0  */
L_1332:
    ldr r2, [pc, #12]                       /* @1332 034a ldr r2, [pc, #12] literal 0x1340 -> 0x40020D30 */
L_1334:
    adds r0, r0, r2                         /* @1334 8018 adds r0, r0, r2  */
L_1336:
    movs r2, #0                             /* @1336 0022 movs r2, #0  */
L_1338:
    bl L_16ac                               /* @1338 00f0b8f9 bl #880 -> sub_16AC */
L_133c:
    movs r0, #0                             /* @133C 0020 movs r0, #0  */
L_133e:
    pop {r4, pc}                            /* @133E 10bd pop {r4, pc}  */
L_1340:
    .byte 0x30,0x0d,0x02,0x40 /* @1340 literal */

/* sub_1344 */
L_1344:
    push {r4, lr}                           /* @1344 10b5 push {r4, lr}  */
L_1346:
    lsls r2, r2, #2                         /* @1346 9200 lsls r2, r2, #2  */
L_1348:
    adds r0, r2, r0                         /* @1348 1018 adds r0, r2, r0  */
L_134a:
    ldr r2, [pc, #12]                       /* @134A 034a ldr r2, [pc, #12] literal 0x1358 -> 0x40020D30 */
L_134c:
    adds r0, r0, r2                         /* @134C 8018 adds r0, r0, r2  */
L_134e:
    movs r2, #1                             /* @134E 0122 movs r2, #1  */
L_1350:
    bl L_16ac                               /* @1350 00f0acf9 bl #856 -> sub_16AC */
L_1354:
    movs r0, #0                             /* @1354 0020 movs r0, #0  */
L_1356:
    pop {r4, pc}                            /* @1356 10bd pop {r4, pc}  */
L_1358:
    .byte 0x30,0x0d,0x02,0x40 /* @1358 literal */

/* sub_135C */
L_135c:
    push {r4, lr}                           /* @135C 10b5 push {r4, lr}  */
L_135e:
    ldr r2, [pc, #8]                        /* @135E 024a ldr r2, [pc, #8] literal 0x1368 -> 0x40020D04 */
L_1360:
    adds r0, r0, r2                         /* @1360 8018 adds r0, r0, r2  */
L_1362:
    bl L_12f8                               /* @1362 fff7c9ff bl #-110 -> sub_12F8 */
L_1366:
    pop {r4, pc}                            /* @1366 10bd pop {r4, pc}  */
L_1368:
    .byte 0x04,0x0d,0x02,0x40 /* @1368 literal */

/* sub_136C */
L_136c:
    push {r4, lr}                           /* @136C 10b5 push {r4, lr}  */
L_136e:
    ldr r2, [pc, #8]                        /* @136E 024a ldr r2, [pc, #8] literal 0x1378 -> 0x40020E00 */
L_1370:
    adds r0, r0, r2                         /* @1370 8018 adds r0, r0, r2  */
L_1372:
    bl L_12f8                               /* @1372 fff7c1ff bl #-126 -> sub_12F8 */
L_1376:
    pop {r4, pc}                            /* @1376 10bd pop {r4, pc}  */
L_1378:
    .byte 0x00,0x0e,0x02,0x40 /* @1378 literal */

/* sub_137C */
L_137c:
    push {r4, r5, r6, lr}                   /* @137C 70b5 push {r4, r5, r6, lr}  */
L_137e:
    mov r4, r0                              /* @137E 0446 mov r4, r0  */
L_1380:
    mov r6, r1                              /* @1380 0e46 mov r6, r1  */
L_1382:
    mov r5, r2                              /* @1382 1546 mov r5, r2  */
L_1384:
    ldr r0, [pc, #116]                      /* @1384 1d48 ldr r0, [pc, #116] literal 0x13FC -> 0x40020D0C */
L_1386:
    movs r2, #0                             /* @1386 0022 movs r2, #0  */
L_1388:
    mov r1, r6                              /* @1388 3146 mov r1, r6  */
L_138a:
    adds r0, r4, r0                         /* @138A 2018 adds r0, r4, r0  */
L_138c:
    bl L_16ac                               /* @138C 00f08ef9 bl #796 -> sub_16AC */
L_1390:
    lsls r1, r6, #2                         /* @1390 b100 lsls r1, r6, #2  */
L_1392:
    ldr r2, [pc, #108]                      /* @1392 1b4a ldr r2, [pc, #108] literal 0x1400 -> 0x40020C00 */
L_1394:
    movs r0, #0                             /* @1394 0020 movs r0, #0  */
L_1396:
    adds r1, r1, r4                         /* @1396 0919 adds r1, r1, r4  */
L_1398:
    adds r1, r1, r2                         /* @1398 8918 adds r1, r1, r2  */
L_139a:
    str r0, [r1]                            /* @139A 0860 str r0, [r1]  */
L_139c:
    ldr r0, [pc, #92]                       /* @139C 1748 ldr r0, [pc, #92] literal 0x13FC -> 0x40020D0C */
L_139e:
    ldrb r2, [r5]                           /* @139E 2a78 ldrb r2, [r5]  */
L_13a0:
    subs r0, #12                            /* @13A0 0c38 subs r0, #12  */
L_13a2:
    mov r1, r6                              /* @13A2 3146 mov r1, r6  */
L_13a4:
    adds r0, r4, r0                         /* @13A4 2018 adds r0, r4, r0  */
L_13a6:
    bl L_16ac                               /* @13A6 00f081f9 bl #770 -> sub_16AC */
L_13aa:
    ldr r0, [pc, #80]                       /* @13AA 1448 ldr r0, [pc, #80] literal 0x13FC -> 0x40020D0C */
L_13ac:
    ldrb r2, [r5, #1]                       /* @13AC 6a78 ldrb r2, [r5, #1]  */
L_13ae:
    adds r0, #16                            /* @13AE 1030 adds r0, #16  */
L_13b0:
    mov r1, r6                              /* @13B0 3146 mov r1, r6  */
L_13b2:
    adds r0, r4, r0                         /* @13B2 2018 adds r0, r4, r0  */
L_13b4:
    bl L_16ac                               /* @13B4 00f07af9 bl #756 -> sub_16AC */
L_13b8:
    ldr r0, [pc, #64]                       /* @13B8 1048 ldr r0, [pc, #64] literal 0x13FC -> 0x40020D0C */
L_13ba:
    ldrb r2, [r5, #2]                       /* @13BA aa78 ldrb r2, [r5, #2]  */
L_13bc:
    adds r0, #20                            /* @13BC 1430 adds r0, #20  */
L_13be:
    mov r1, r6                              /* @13BE 3146 mov r1, r6  */
L_13c0:
    adds r0, r4, r0                         /* @13C0 2018 adds r0, r4, r0  */
L_13c2:
    bl L_16ac                               /* @13C2 00f073f9 bl #742 -> sub_16AC */
L_13c6:
    ldr r0, [pc, #52]                       /* @13C6 0d48 ldr r0, [pc, #52] literal 0x13FC -> 0x40020D0C */
L_13c8:
    ldrb r2, [r5, #3]                       /* @13C8 ea78 ldrb r2, [r5, #3]  */
L_13ca:
    adds r0, #24                            /* @13CA 1830 adds r0, #24  */
L_13cc:
    mov r1, r6                              /* @13CC 3146 mov r1, r6  */
L_13ce:
    adds r0, r4, r0                         /* @13CE 2018 adds r0, r4, r0  */
L_13d0:
    bl L_16ac                               /* @13D0 00f06cf9 bl #728 -> sub_16AC */
L_13d4:
    ldr r0, [pc, #36]                       /* @13D4 0948 ldr r0, [pc, #36] literal 0x13FC -> 0x40020D0C */
L_13d6:
    ldrb r2, [r5, #4]                       /* @13D6 2a79 ldrb r2, [r5, #4]  */
L_13d8:
    adds r0, #32                            /* @13D8 2030 adds r0, #32  */
L_13da:
    mov r1, r6                              /* @13DA 3146 mov r1, r6  */
L_13dc:
    adds r0, r4, r0                         /* @13DC 2018 adds r0, r4, r0  */
L_13de:
    bl L_16ac                               /* @13DE 00f065f9 bl #714 -> sub_16AC */
L_13e2:
    ldr r1, [pc, #32]                       /* @13E2 0849 ldr r1, [pc, #32] literal 0x1404 -> 0x40020F00 */
L_13e4:
    ldr r0, [r1, #8]                        /* @13E4 8868 ldr r0, [r1, #8]  */
L_13e6:
    movs r2, #1                             /* @13E6 0122 movs r2, #1  */
L_13e8:
    lsls r2, r2, #15                        /* @13E8 d203 lsls r2, r2, #15  */
L_13ea:
    bics r0, r2                             /* @13EA 9043 bics r0, r2  */
L_13ec:
    ldrb r2, [r5, #5]                       /* @13EC 6a79 ldrb r2, [r5, #5]  */
L_13ee:
    lsls r2, r2, #31                        /* @13EE d207 lsls r2, r2, #31  */
L_13f0:
    lsrs r2, r2, #16                        /* @13F0 120c lsrs r2, r2, #16  */
L_13f2:
    orrs r0, r2                             /* @13F2 1043 orrs r0, r2  */
L_13f4:
    str r0, [r1, #8]                        /* @13F4 8860 str r0, [r1, #8]  */
L_13f6:
    movs r0, #0                             /* @13F6 0020 movs r0, #0  */
L_13f8:
    pop {r4, r5, r6, pc}                    /* @13F8 70bd pop {r4, r5, r6, pc}  */
    .byte 0x00,0x00 /* @13FA unclassified */
L_13fc:
    .byte 0x0c,0x0d,0x02,0x40 /* @13FC literal */
L_1400:
    .byte 0x00,0x0c,0x02,0x40 /* @1400 literal */
L_1404:
    .byte 0x00,0x0f,0x02,0x40 /* @1404 literal */

/* sub_1408 */
L_1408:
    lsls r1, r1, #2                         /* @1408 8900 lsls r1, r1, #2  */
L_140a:
    adds r0, r1, r0                         /* @140A 0818 adds r0, r1, r0  */
L_140c:
    ldr r1, [pc, #8]                        /* @140C 0249 ldr r1, [pc, #8] literal 0x1418 -> 0x40020C00 */
L_140e:
    adds r0, r0, r1                         /* @140E 4018 adds r0, r0, r1  */
L_1410:
    str r2, [r0]                            /* @1410 0260 str r2, [r0]  */
L_1412:
    movs r0, #0                             /* @1412 0020 movs r0, #0  */
L_1414:
    bx lr                                   /* @1414 7047 bx lr  */
    .byte 0x00,0x00 /* @1416 unclassified */
L_1418:
    .byte 0x00,0x0c,0x02,0x40 /* @1418 literal */

/* sub_141C */
L_141c:
    push {r4, lr}                           /* @141C 10b5 push {r4, lr}  */
L_141e:
    ldr r3, [pc, #12]                       /* @141E 034b ldr r3, [pc, #12] literal 0x142C -> 0x40020D0C */
L_1420:
    movs r2, #1                             /* @1420 0122 movs r2, #1  */
L_1422:
    adds r0, r0, r3                         /* @1422 c018 adds r0, r0, r3  */
L_1424:
    bl L_16ac                               /* @1424 00f042f9 bl #644 -> sub_16AC */
L_1428:
    movs r0, #0                             /* @1428 0020 movs r0, #0  */
L_142a:
    pop {r4, pc}                            /* @142A 10bd pop {r4, pc}  */
L_142c:
    .byte 0x0c,0x0d,0x02,0x40 /* @142C literal */

/* sub_1430 */
L_1430:
    push {r4, lr}                           /* @1430 10b5 push {r4, lr}  */
L_1432:
    ldr r3, [pc, #12]                       /* @1432 034b ldr r3, [pc, #12] literal 0x1440 -> 0x40020D10 */
L_1434:
    movs r2, #1                             /* @1434 0122 movs r2, #1  */
L_1436:
    adds r0, r0, r3                         /* @1436 c018 adds r0, r0, r3  */
L_1438:
    bl L_16ac                               /* @1438 00f038f9 bl #624 -> sub_16AC */
L_143c:
    movs r0, #0                             /* @143C 0020 movs r0, #0  */
L_143e:
    pop {r4, pc}                            /* @143E 10bd pop {r4, pc}  */
L_1440:
    .byte 0x10,0x0d,0x02,0x40 /* @1440 literal */

/* hardfault_handler */
L_1444:
    push {r3, lr}                           /* @1444 08b5 push {r3, lr}  */
L_1446:
    movs r0, #0                             /* @1446 0020 movs r0, #0  */
L_1448:
    str r0, [sp]                            /* @1448 0090 str r0, [sp]  */
L_144a:
    ldr r0, [sp]                            /* @144A 0098 ldr r0, [sp]  */
L_144c:
    cmp r0, #0                              /* @144C 0028 cmp r0, #0  */
L_144e:
    beq L_144a                              /* @144E fcd0 beq #-8 -> 0x144A */
L_1450:
    pop {r3, pc}                            /* @1450 08bd pop {r3, pc}  */
    .byte 0x00,0x00 /* @1452 unclassified */

/* sub_1454 */
L_1454:
    ldr r0, [pc, #8]                        /* @1454 0248 ldr r0, [pc, #8] literal 0x1460 -> 0x40021800 */
L_1456:
    ldr r0, [r0, #20]                       /* @1456 4069 ldr r0, [r0, #20]  */
L_1458:
    lsls r0, r0, #31                        /* @1458 c007 lsls r0, r0, #31  */
L_145a:
    lsrs r0, r0, #31                        /* @145A c00f lsrs r0, r0, #31  */
L_145c:
    bx lr                                   /* @145C 7047 bx lr  */
    .byte 0x00,0x00 /* @145E unclassified */
L_1460:
    .byte 0x00,0x18,0x02,0x40 /* @1460 literal */

/* sub_1464 */
L_1464:
    ldr r0, [pc, #8]                        /* @1464 0248 ldr r0, [pc, #8] literal 0x1470 -> 0x40021800 */
L_1466:
    ldr r0, [r0, #20]                       /* @1466 4069 ldr r0, [r0, #20]  */
L_1468:
    lsls r0, r0, #30                        /* @1468 8007 lsls r0, r0, #30  */
L_146a:
    lsrs r0, r0, #31                        /* @146A c00f lsrs r0, r0, #31  */
L_146c:
    bx lr                                   /* @146C 7047 bx lr  */
    .byte 0x00,0x00 /* @146E unclassified */
L_1470:
    .byte 0x00,0x18,0x02,0x40 /* @1470 literal */

/* sub_1474 */
L_1474:
    push {r4, lr}                           /* @1474 10b5 push {r4, lr}  */
L_1476:
    ldr r3, [pc, #52]                       /* @1476 0d4b ldr r3, [pc, #52] literal 0x14AC -> 0x40021800 */
L_1478:
    ldr r4, [r3, #16]                       /* @1478 1c69 ldr r4, [r3, #16]  */
L_147a:
    lsrs r4, r4, #1                         /* @147A 6408 lsrs r4, r4, #1  */
L_147c:
    lsls r4, r4, #1                         /* @147C 6400 lsls r4, r4, #1  */
L_147e:
    str r4, [r3, #16]                       /* @147E 1c61 str r4, [r3, #16]  */
L_1480:
    cmp r2, #0                              /* @1480 002a cmp r2, #0  */
L_1482:
    beq L_14a4                              /* @1482 0fd0 beq #30 -> 0x14A4 */
L_1484:
    str r0, [r3]                            /* @1484 1860 str r0, [r3]  */
L_1486:
    str r1, [r3, #4]                        /* @1486 5960 str r1, [r3, #4]  */
L_1488:
    bl L_1464                               /* @1488 fff7ecff bl #-40 -> sub_1464 */
L_148c:
    cmp r0, #1                              /* @148C 0128 cmp r0, #1  */
L_148e:
    beq L_14a8                              /* @148E 0bd0 beq #22 -> 0x14A8 */
L_1490:
    bl L_1454                               /* @1490 fff7e0ff bl #-64 -> sub_1454 */
L_1494:
    cmp r0, #1                              /* @1494 0128 cmp r0, #1  */
L_1496:
    bne L_1490                              /* @1496 fbd1 bne #-10 -> 0x1490 */
L_1498:
    ldr r0, [r3, #8]                        /* @1498 9868 ldr r0, [r3, #8]  */
L_149a:
    str r0, [r2]                            /* @149A 1060 str r0, [r2]  */
L_149c:
    ldr r0, [r3, #12]                       /* @149C d868 ldr r0, [r3, #12]  */
L_149e:
    str r0, [r2, #4]                        /* @149E 5060 str r0, [r2, #4]  */
L_14a0:
    movs r0, #0                             /* @14A0 0020 movs r0, #0  */
L_14a2:
    pop {r4, pc}                            /* @14A2 10bd pop {r4, pc}  */
L_14a4:
    movs r0, #4                             /* @14A4 0420 movs r0, #4  */
L_14a6:
    pop {r4, pc}                            /* @14A6 10bd pop {r4, pc}  */
L_14a8:
    movs r0, #4                             /* @14A8 0420 movs r0, #4  */
L_14aa:
    pop {r4, pc}                            /* @14AA 10bd pop {r4, pc}  */
L_14ac:
    .byte 0x00,0x18,0x02,0x40 /* @14AC literal */

/* sub_14B0 */
L_14b0:
    push {r4, lr}                           /* @14B0 10b5 push {r4, lr}  */
L_14b2:
    mov r8, r8                              /* @14B2 c046 mov r8, r8  */
L_14b4:
    mov r8, r8                              /* @14B4 c046 mov r8, r8  */
L_14b6:
    pop {r4, pc}                            /* @14B6 10bd pop {r4, pc}  */

/* sub_14B8 */
L_14b8:
    push {r4, r5, r6, r7, lr}               /* @14B8 f0b5 push {r4, r5, r6, r7, lr}  */
L_14ba:
    mov lr, r0                              /* @14BA 8646 mov lr, r0  */
L_14bc:
    mov r12, r1                             /* @14BC 8c46 mov r12, r1  */
L_14be:
    movs r4, #0                             /* @14BE 0024 movs r4, #0  */
L_14c0:
    movs r3, #0                             /* @14C0 0023 movs r3, #0  */
L_14c2:
    ldr r5, [pc, #160]                      /* @14C2 284d ldr r5, [pc, #160] literal 0x1564 -> 0x40000400 */
L_14c4:
    ldr r0, [r5, #8]                        /* @14C4 a868 ldr r0, [r5, #8]  */
L_14c6:
    movs r7, #32                            /* @14C6 2027 movs r7, #32  */
L_14c8:
    orrs r0, r7                             /* @14C8 3843 orrs r0, r7  */
L_14ca:
    str r0, [r5, #8]                        /* @14CA a860 str r0, [r5, #8]  */
L_14cc:
    adds r6, r2, #1                         /* @14CC 561c adds r6, r2, #1  */
L_14ce:
    ldr r0, [pc, #152]                      /* @14CE 2648 ldr r0, [pc, #152] literal 0x1568 -> 0x00001770 */
L_14d0:
    b L_14e4                                /* @14D0 08e0 b #16 -> 0x14E4 */
L_14d2:
    adds r4, r4, #1                         /* @14D2 641c adds r4, r4, #1  */
L_14d4:
    uxth r4, r4                             /* @14D4 a4b2 uxth r4, r4  */
L_14d6:
    cmp r4, r0                              /* @14D6 8442 cmp r4, r0  */
L_14d8:
    bls L_14e4                              /* @14D8 04d9 bls #8 -> 0x14E4 */
L_14da:
    ldr r3, [pc, #144]                      /* @14DA 244b ldr r3, [pc, #144] literal 0x156C -> 0x200005DC */
L_14dc:
    movs r1, #10                            /* @14DC 0a21 movs r1, #10  */
L_14de:
    strh r1, [r3, #2]                       /* @14DE 5980 strh r1, [r3, #2]  */
L_14e0:
    uxtb r3, r6                             /* @14E0 f3b2 uxtb r3, r6  */
L_14e2:
    b L_14ea                                /* @14E2 02e0 b #4 -> 0x14EA */
L_14e4:
    ldr r1, [r5, #8]                        /* @14E4 a968 ldr r1, [r5, #8]  */
L_14e6:
    lsls r1, r1, #28                        /* @14E6 0907 lsls r1, r1, #28  */
L_14e8:
    bpl L_14d2                              /* @14E8 f3d5 bpl #-26 -> 0x14D2 */
L_14ea:
    movs r4, #0                             /* @14EA 0024 movs r4, #0  */
L_14ec:
    ldr r0, [r5, #20]                       /* @14EC 6869 ldr r0, [r5, #20]  */
L_14ee:
    cmp r0, #40                             /* @14EE 2828 cmp r0, #40  */
L_14f0:
    beq L_153a                              /* @14F0 23d0 beq #70 -> 0x153A */
L_14f2:
    bgt L_1502                              /* @14F2 06dc bgt #12 -> 0x1502 */
L_14f4:
    cmp r0, #8                              /* @14F4 0828 cmp r0, #8  */
L_14f6:
    beq L_1528                              /* @14F6 17d0 beq #46 -> 0x1528 */
L_14f8:
    cmp r0, #24                             /* @14F8 1828 cmp r0, #24  */
L_14fa:
    beq L_1534                              /* @14FA 1bd0 beq #54 -> 0x1534 */
L_14fc:
    cmp r0, #32                             /* @14FC 2028 cmp r0, #32  */
L_14fe:
    bne L_150a                              /* @14FE 04d1 bne #8 -> 0x150A */
L_1500:
    b L_1548                                /* @1500 22e0 b #68 -> 0x1548 */
L_1502:
    cmp r0, #48                             /* @1502 3028 cmp r0, #48  */
L_1504:
    beq L_1550                              /* @1504 24d0 beq #72 -> 0x1550 */
L_1506:
    cmp r0, #56                             /* @1506 3828 cmp r0, #56  */
L_1508:
    beq L_1548                              /* @1508 1ed0 beq #60 -> 0x1548 */
L_150a:
    ldr r0, [pc, #96]                       /* @150A 1848 ldr r0, [pc, #96] literal 0x156C -> 0x200005DC */
L_150c:
    movs r3, #10                            /* @150C 0a23 movs r3, #10  */
L_150e:
    strh r3, [r0, #2]                       /* @150E 4380 strh r3, [r0, #2]  */
L_1510:
    uxtb r3, r6                             /* @1510 f3b2 uxtb r3, r6  */
L_1512:
    cmp r3, r2                              /* @1512 9342 cmp r3, r2  */
L_1514:
    bls L_155a                              /* @1514 21d9 bls #66 -> 0x155A */
L_1516:
    ldr r1, [r5, #8]                        /* @1516 a968 ldr r1, [r5, #8]  */
L_1518:
    movs r0, #16                            /* @1518 1020 movs r0, #16  */
L_151a:
    orrs r1, r0                             /* @151A 0143 orrs r1, r0  */
L_151c:
    str r1, [r5, #8]                        /* @151C a960 str r1, [r5, #8]  */
L_151e:
    ldr r1, [r5, #8]                        /* @151E a968 ldr r1, [r5, #8]  */
L_1520:
    movs r0, #8                             /* @1520 0820 movs r0, #8  */
L_1522:
    bics r1, r0                             /* @1522 8143 bics r1, r0  */
L_1524:
    str r1, [r5, #8]                        /* @1524 a960 str r1, [r5, #8]  */
L_1526:
    pop {r4, r5, r6, r7, pc}                /* @1526 f0bd pop {r4, r5, r6, r7, pc}  */
L_1528:
    ldr r0, [r5, #8]                        /* @1528 a868 ldr r0, [r5, #8]  */
L_152a:
    bics r0, r7                             /* @152A b843 bics r0, r7  */
L_152c:
    str r0, [r5, #8]                        /* @152C a860 str r0, [r5, #8]  */
L_152e:
    movs r0, #120                           /* @152E 7820 movs r0, #120  */
L_1530:
    str r0, [r5, #12]                       /* @1530 e860 str r0, [r5, #12]  */
L_1532:
    b L_1512                                /* @1532 eee7 b #-36 -> 0x1512 */
L_1534:
    mov r0, lr                              /* @1534 7046 mov r0, lr  */
L_1536:
    str r0, [r5, #12]                       /* @1536 e860 str r0, [r5, #12]  */
L_1538:
    b L_1512                                /* @1538 ebe7 b #-42 -> 0x1512 */
L_153a:
    mov r0, r3                              /* @153A 1846 mov r0, r3  */
L_153c:
    mov r1, r12                             /* @153C 6146 mov r1, r12  */
L_153e:
    adds r3, r3, #1                         /* @153E 5b1c adds r3, r3, #1  */
L_1540:
    ldrb r0, [r1, r0]                       /* @1540 085c ldrb r0, [r1, r0]  */
L_1542:
    uxtb r3, r3                             /* @1542 dbb2 uxtb r3, r3  */
L_1544:
    str r0, [r5, #12]                       /* @1544 e860 str r0, [r5, #12]  */
L_1546:
    b L_1512                                /* @1546 e4e7 b #-56 -> 0x1512 */
L_1548:
    ldr r0, [r5, #8]                        /* @1548 a868 ldr r0, [r5, #8]  */
L_154a:
    orrs r0, r7                             /* @154A 3843 orrs r0, r7  */
L_154c:
    str r0, [r5, #8]                        /* @154C a860 str r0, [r5, #8]  */
L_154e:
    b L_1512                                /* @154E e0e7 b #-64 -> 0x1512 */
L_1550:
    ldr r0, [r5, #8]                        /* @1550 a868 ldr r0, [r5, #8]  */
L_1552:
    movs r1, #16                            /* @1552 1021 movs r1, #16  */
L_1554:
    orrs r0, r1                             /* @1554 0843 orrs r0, r1  */
L_1556:
    str r0, [r5, #8]                        /* @1556 a860 str r0, [r5, #8]  */
L_1558:
    b L_1512                                /* @1558 dbe7 b #-74 -> 0x1512 */
L_155a:
    ldr r0, [r5, #8]                        /* @155A a868 ldr r0, [r5, #8]  */
L_155c:
    movs r1, #8                             /* @155C 0821 movs r1, #8  */
L_155e:
    bics r0, r1                             /* @155E 8843 bics r0, r1  */
L_1560:
    str r0, [r5, #8]                        /* @1560 a860 str r0, [r5, #8]  */
L_1562:
    b L_14ce                                /* @1562 b4e7 b #-152 -> 0x14CE */
L_1564:
    .byte 0x00,0x04,0x00,0x40 /* @1564 literal */
L_1568:
    .byte 0x70,0x17,0x00,0x00 /* @1568 literal */
L_156c:
    .byte 0xdc,0x05,0x00,0x20 /* @156C literal */

/* sub_1570 */
L_1570:
    push {r4, lr}                           /* @1570 10b5 push {r4, lr}  */
L_1572:
    mov r8, r8                              /* @1572 c046 mov r8, r8  */
L_1574:
    mov r8, r8                              /* @1574 c046 mov r8, r8  */
L_1576:
    pop {r4, pc}                            /* @1576 10bd pop {r4, pc}  */

/* sub_1578 */
L_1578:
    push {r4, r5, r6, lr}                   /* @1578 70b5 push {r4, r5, r6, lr}  */
L_157a:
    mov r4, r0                              /* @157A 0446 mov r4, r0  */
L_157c:
    movs r0, #1                             /* @157C 0120 movs r0, #1  */
L_157e:
    ldr r2, [pc, #104]                      /* @157E 1a4a ldr r2, [pc, #104] literal 0x15E8 -> 0x40002000 */
L_1580:
    ldr r3, [r2, #40]                       /* @1580 936a ldr r3, [r2, #40]  */
L_1582:
    movs r5, #16                            /* @1582 1025 movs r5, #16  */
L_1584:
    bics r3, r5                             /* @1584 ab43 bics r3, r5  */
L_1586:
    str r3, [r2, #40]                       /* @1586 9362 str r3, [r2, #40]  */
L_1588:
    ldr r3, [r2, #40]                       /* @1588 936a ldr r3, [r2, #40]  */
L_158a:
    orrs r3, r5                             /* @158A 2b43 orrs r3, r5  */
L_158c:
    str r3, [r2, #40]                       /* @158C 9362 str r3, [r2, #40]  */
L_158e:
    movs r2, #0                             /* @158E 0022 movs r2, #0  */
L_1590:
    str r2, [r4, #8]                        /* @1590 a260 str r2, [r4, #8]  */
L_1592:
    ldrb r3, [r1, #8]                       /* @1592 0b7a ldrb r3, [r1, #8]  */
L_1594:
    str r3, [r4, #8]                        /* @1594 a360 str r3, [r4, #8]  */
L_1596:
    ldr r3, [r1, #4]                        /* @1596 4b68 ldr r3, [r1, #4]  */
L_1598:
    ldr r5, [r1]                            /* @1598 0d68 ldr r5, [r1]  */
L_159a:
    lsls r3, r3, #4                         /* @159A 1b01 lsls r3, r3, #4  */
L_159c:
    cmp r3, r5                              /* @159C ab42 cmp r3, r5  */
L_159e:
    bls L_15a2                              /* @159E 00d9 bls #0 -> 0x15A2 */
L_15a0:
    pop {r4, r5, r6, pc}                    /* @15A0 70bd pop {r4, r5, r6, pc}  */
L_15a2:
    ldrb r3, [r1, #8]                       /* @15A2 0b7a ldrb r3, [r1, #8]  */
L_15a4:
    cmp r3, #64                             /* @15A4 402b cmp r3, #64  */
L_15a6:
    beq L_15ba                              /* @15A6 08d0 beq #16 -> 0x15BA */
L_15a8:
    str r2, [r4]                            /* @15A8 2260 str r2, [r4]  */
L_15aa:
    ldrb r2, [r1, #9]                       /* @15AA 4a7a ldrb r2, [r1, #9]  */
L_15ac:
    ldrb r3, [r1, #10]                      /* @15AC 8b7a ldrb r3, [r1, #10]  */
L_15ae:
    lsls r2, r2, #1                         /* @15AE 5200 lsls r2, r2, #1  */
L_15b0:
    orrs r2, r3                             /* @15B0 1a43 orrs r2, r3  */
L_15b2:
    uxtb r2, r2                             /* @15B2 d2b2 uxtb r2, r2  */
L_15b4:
    strb r2, [r1, #9]                       /* @15B4 4a72 strb r2, [r1, #9]  */
L_15b6:
    str r2, [r4, #16]                       /* @15B6 2261 str r2, [r4, #16]  */
L_15b8:
    pop {r4, r5, r6, pc}                    /* @15B8 70bd pop {r4, r5, r6, pc}  */
L_15ba:
    movs r0, #1                             /* @15BA 0120 movs r0, #1  */
L_15bc:
    str r0, [r4]                            /* @15BC 2060 str r0, [r4]  */
L_15be:
    ldr r0, [r1, #4]                        /* @15BE 4868 ldr r0, [r1, #4]  */
L_15c0:
    ldr r2, [r1]                            /* @15C0 0a68 ldr r2, [r1]  */
L_15c2:
    mov r1, r0                              /* @15C2 0146 mov r1, r0  */
L_15c4:
    mov r0, r2                              /* @15C4 1046 mov r0, r2  */
L_15c6:
    bl L_0110                               /* @15C6 fef7a3fd bl #-5306 -> udivmod32 */
L_15ca:
    lsrs r0, r0, #3                         /* @15CA c008 lsrs r0, r0, #3  */
L_15cc:
    subs r0, r0, #1                         /* @15CC 401e subs r0, r0, #1  */
L_15ce:
    uxtb r5, r0                             /* @15CE c5b2 uxtb r5, r0  */
L_15d0:
    cmp r5, #9                              /* @15D0 092d cmp r5, #9  */
L_15d2:
    bhs L_15dc                              /* @15D2 03d2 bhs #6 -> 0x15DC */
L_15d4:
    movs r1, #0                             /* @15D4 0021 movs r1, #0  */
L_15d6:
    mov r0, r4                              /* @15D6 2046 mov r0, r4  */
L_15d8:
    bl L_160e                               /* @15D8 00f019f8 bl #50 -> sub_160E */
L_15dc:
    mov r1, r5                              /* @15DC 2946 mov r1, r5  */
L_15de:
    mov r0, r4                              /* @15DE 2046 mov r0, r4  */
L_15e0:
    bl L_1608                               /* @15E0 00f012f8 bl #36 -> sub_1608 */
L_15e4:
    pop {r4, r5, r6, pc}                    /* @15E4 70bd pop {r4, r5, r6, pc}  */
    .byte 0x00,0x00 /* @15E6 unclassified */
L_15e8:
    .byte 0x00,0x20,0x00,0x40 /* @15E8 literal */

/* switch_u8_dispatch */
L_15ec:
    push {r4, r5}                           /* @15EC 30b4 push {r4, r5}  */
L_15ee:
    mov r4, lr                              /* @15EE 7446 mov r4, lr  */
L_15f0:
    subs r4, r4, #1                         /* @15F0 641e subs r4, r4, #1  */
L_15f2:
    ldrb r5, [r4]                           /* @15F2 2578 ldrb r5, [r4]  */
L_15f4:
    adds r4, r4, #1                         /* @15F4 641c adds r4, r4, #1  */
L_15f6:
    cmp r3, r5                              /* @15F6 ab42 cmp r3, r5  */
L_15f8:
    bhs L_15fc                              /* @15F8 00d2 bhs #0 -> 0x15FC */
L_15fa:
    mov r5, r3                              /* @15FA 1d46 mov r5, r3  */
L_15fc:
    ldrb r3, [r4, r5]                       /* @15FC 635d ldrb r3, [r4, r5]  */
L_15fe:
    lsls r3, r3, #1                         /* @15FE 5b00 lsls r3, r3, #1  */
L_1600:
    adds r3, r4, r3                         /* @1600 e318 adds r3, r4, r3  */
L_1602:
    pop {r4, r5}                            /* @1602 30bc pop {r4, r5}  */
L_1604:
    bx r3                                   /* @1604 1847 bx r3  */
    .byte 0x00,0x00 /* @1606 unclassified */

/* sub_1608 */
L_1608:
    str r1, [r0, #4]                        /* @1608 4160 str r1, [r0, #4]  */
L_160a:
    movs r0, #0                             /* @160A 0020 movs r0, #0  */
L_160c:
    bx lr                                   /* @160C 7047 bx lr  */

/* sub_160E */
L_160e:
    push {r4, lr}                           /* @160E 10b5 push {r4, lr}  */
L_1610:
    movs r2, #1                             /* @1610 0122 movs r2, #1  */
L_1612:
    adds r0, #8                             /* @1612 0830 adds r0, #8  */
L_1614:
    bl L_16ac                               /* @1614 00f04af8 bl #148 -> sub_16AC */
L_1618:
    movs r0, #0                             /* @1618 0020 movs r0, #0  */
L_161a:
    pop {r4, pc}                            /* @161A 10bd pop {r4, pc}  */

/* sub_161C */
L_161c:
    push {r4, lr}                           /* @161C 10b5 push {r4, lr}  */
L_161e:
    ldr r0, [pc, #44]                       /* @161E 0b48 ldr r0, [pc, #44] literal 0x164C -> 0x20000012 */
L_1620:
    movs r4, #0                             /* @1620 0024 movs r4, #0  */
L_1622:
    strb r4, [r0, #2]                       /* @1622 8470 strb r4, [r0, #2]  */
L_1624:
    movs r0, #0                             /* @1624 0020 movs r0, #0  */
L_1626:
    bl L_2ec4                               /* @1626 01f04dfc bl #6298 -> menu_goto_0 */
L_162a:
    ldr r1, [pc, #36]                       /* @162A 0949 ldr r1, [pc, #36] literal 0x1650 -> 0x200006A8 */
L_162c:
    cmp r0, #1                              /* @162C 0128 cmp r0, #1  */
L_162e:
    beq L_1646                              /* @162E 0ad0 beq #20 -> 0x1646 */
L_1630:
    strb r4, [r1, #13]                      /* @1630 4c73 strb r4, [r1, #13]  */
L_1632:
    movs r0, #2                             /* @1632 0220 movs r0, #2  */
L_1634:
    strb r0, [r1, #2]                       /* @1634 8870 strb r0, [r1, #2]  */
L_1636:
    ldr r0, [pc, #28]                       /* @1636 0748 ldr r0, [pc, #28] literal 0x1654 -> 0x2000072C */
L_1638:
    ldrh r0, [r0, #20]                      /* @1638 808a ldrh r0, [r0, #20]  */
L_163a:
    ldrb r2, [r1, #13]                      /* @163A 4a7b ldrb r2, [r1, #13]  */
L_163c:
    adds r0, r0, r2                         /* @163C 8018 adds r0, r0, r2  */
L_163e:
    strb r0, [r1, #12]                      /* @163E 0873 strb r0, [r1, #12]  */
L_1640:
    bl L_2334                               /* @1640 00f078fe bl #3312 -> uart_start_reply */
L_1644:
    pop {r4, pc}                            /* @1644 10bd pop {r4, pc}  */
L_1646:
    movs r0, #1                             /* @1646 0120 movs r0, #1  */
L_1648:
    strb r0, [r1, #13]                      /* @1648 4873 strb r0, [r1, #13]  */
L_164a:
    b L_1632                                /* @164A f2e7 b #-28 -> 0x1632 */
L_164c:
    .byte 0x12,0x00,0x00,0x20 /* @164C literal */
L_1650:
    .byte 0xa8,0x06,0x00,0x20 /* @1650 literal */
L_1654:
    .byte 0x2c,0x07,0x00,0x20 /* @1654 literal */
    .byte 0x00,0x00,0x00,0x00 /* @1658 unclassified */

/* sub_165C */
L_165c:
    push {r4, lr}                           /* @165C 10b5 push {r4, lr}  */
L_165e:
    movs r1, #7                             /* @165E 0721 movs r1, #7  */
L_1660:
    movs r0, #64                            /* @1660 4020 movs r0, #64  */
L_1662:
    bl L_136c                               /* @1662 fff783fe bl #-762 -> sub_136C */
L_1666:
    cmp r0, #1                              /* @1666 0128 cmp r0, #1  */
L_1668:
    bne L_1684                              /* @1668 0cd1 bne #24 -> 0x1684 */
L_166a:
    ldr r0, [pc, #28]                       /* @166A 0748 ldr r0, [pc, #28] literal 0x1688 -> 0x2000072C */
L_166c:
    ldrb r1, [r0, #8]                       /* @166C 017a ldrb r1, [r0, #8]  */
L_166e:
    cmp r1, #2                              /* @166E 0229 cmp r1, #2  */
L_1670:
    beq L_167c                              /* @1670 04d0 beq #8 -> 0x167C */
L_1672:
    ldrb r0, [r0, #9]                       /* @1672 407a ldrb r0, [r0, #9]  */
L_1674:
    cmp r0, #2                              /* @1674 0228 cmp r0, #2  */
L_1676:
    beq L_167c                              /* @1676 01d0 beq #2 -> 0x167C */
L_1678:
    bl L_2090                               /* @1678 00f00afd bl #2580 -> sub_2090 */
L_167c:
    movs r1, #7                             /* @167C 0721 movs r1, #7  */
L_167e:
    movs r0, #64                            /* @167E 4020 movs r0, #64  */
L_1680:
    bl L_1304                               /* @1680 fff740fe bl #-896 -> sub_1304 */
L_1684:
    pop {r4, pc}                            /* @1684 10bd pop {r4, pc}  */
    .byte 0x00,0x00 /* @1686 unclassified */
L_1688:
    .byte 0x2c,0x07,0x00,0x20 /* @1688 literal */
    .byte 0x02,0xe0,0x08,0xc8,0x12,0x1f,0x08,0xc1,0x00,0x2a,0xfa,0xd1,0x70,0x47 /* @168C unclassified */

/* scatter_zero_words */
L_169a:
    movs r0, #0                             /* @169A 0020 movs r0, #0  */
L_169c:
    b L_16a2                                /* @169C 01e0 b #2 -> 0x16A2 */
L_169e:
    stmia r1!, {r0}                         /* @169E 01c1 stm r1!, {r0}  */
L_16a0:
    subs r2, r2, #4                         /* @16A0 121f subs r2, r2, #4  */
L_16a2:
    cmp r2, #0                              /* @16A2 002a cmp r2, #0  */
L_16a4:
    bne L_169e                              /* @16A4 fbd1 bne #-10 -> 0x169E */
L_16a6:
    bx lr                                   /* @16A6 7047 bx lr  */
    .byte 0x00,0x00,0x00,0x00 /* @16A8 unclassified */

/* sub_16AC */
L_16ac:
    movs r3, #1                             /* @16AC 0123 movs r3, #1  */
L_16ae:
    lsls r3, r1                             /* @16AE 8b40 lsls r3, r1  */
L_16b0:
    cmp r2, #1                              /* @16B0 012a cmp r2, #1  */
L_16b2:
    beq L_16bc                              /* @16B2 03d0 beq #6 -> 0x16BC */
L_16b4:
    ldr r1, [r0]                            /* @16B4 0168 ldr r1, [r0]  */
L_16b6:
    bics r1, r3                             /* @16B6 9943 bics r1, r3  */
L_16b8:
    str r1, [r0]                            /* @16B8 0160 str r1, [r0]  */
L_16ba:
    bx lr                                   /* @16BA 7047 bx lr  */
L_16bc:
    ldr r1, [r0]                            /* @16BC 0168 ldr r1, [r0]  */
L_16be:
    orrs r1, r3                             /* @16BE 1943 orrs r1, r3  */
L_16c0:
    str r1, [r0]                            /* @16C0 0160 str r1, [r0]  */
L_16c2:
    bx lr                                   /* @16C2 7047 bx lr  */

/* sub_16C4 */
L_16c4:
    push {r4, lr}                           /* @16C4 10b5 push {r4, lr}  */
L_16c6:
    movs r4, #0                             /* @16C6 0024 movs r4, #0  */
L_16c8:
    str r4, [r0]                            /* @16C8 0460 str r4, [r0]  */
L_16ca:
    ldrb r2, [r1]                           /* @16CA 0a78 ldrb r2, [r1]  */
L_16cc:
    ldrb r3, [r1, #1]                       /* @16CC 4b78 ldrb r3, [r1, #1]  */
L_16ce:
    orrs r2, r3                             /* @16CE 1a43 orrs r2, r3  */
L_16d0:
    ldrb r3, [r1, #2]                       /* @16D0 8b78 ldrb r3, [r1, #2]  */
L_16d2:
    ldrb r1, [r1, #3]                       /* @16D2 c978 ldrb r1, [r1, #3]  */
L_16d4:
    orrs r3, r1                             /* @16D4 0b43 orrs r3, r1  */
L_16d6:
    orrs r2, r3                             /* @16D6 1a43 orrs r2, r3  */
L_16d8:
    movs r1, #64                            /* @16D8 4021 movs r1, #64  */
L_16da:
    orrs r2, r1                             /* @16DA 0a43 orrs r2, r1  */
L_16dc:
    str r2, [r0]                            /* @16DC 0260 str r2, [r0]  */
L_16de:
    str r4, [r0, #8]                        /* @16DE 8460 str r4, [r0, #8]  */
L_16e0:
    movs r0, #0                             /* @16E0 0020 movs r0, #0  */
L_16e2:
    pop {r4, pc}                            /* @16E2 10bd pop {r4, pc}  */

/* systick_handler */
L_16e4:
    push {r4, lr}                           /* @16E4 10b5 push {r4, lr}  */
L_16e6:
    mov r8, r8                              /* @16E6 c046 mov r8, r8  */
L_16e8:
    mov r8, r8                              /* @16E8 c046 mov r8, r8  */
L_16ea:
    pop {r4, pc}                            /* @16EA 10bd pop {r4, pc}  */

/* sub_16EC */
L_16ec:
    push {r0, r1, r4, r5, r6, r7, lr}       /* @16EC f3b5 push {r0, r1, r4, r5, r6, r7, lr}  */
L_16ee:
    sub sp, #4                              /* @16EE 81b0 sub sp, #4  */
L_16f0:
    mov r4, r1                              /* @16F0 0c46 mov r4, r1  */
L_16f2:
    movs r6, #0                             /* @16F2 0026 movs r6, #0  */
L_16f4:
    ldr r5, [pc, #216]                      /* @16F4 364d ldr r5, [pc, #216] literal 0x17D0 -> 0x40002000 */
L_16f6:
    ldr r7, [r5]                            /* @16F6 2f68 ldr r7, [r5]  */
L_16f8:
    bl L_231c                               /* @16F8 00f010fe bl #3104 -> sub_231C */
L_16fc:
    cmp r4, #0                              /* @16FC 002c cmp r4, #0  */
L_16fe:
    beq L_1702                              /* @16FE 00d0 beq #0 -> 0x1702 */
L_1700:
    movs r4, #1                             /* @1700 0124 movs r4, #1  */
L_1702:
    ldr r0, [sp, #4]                        /* @1702 0198 ldr r0, [sp, #4]  */
L_1704:
    movs r3, r0                             /* @1704 0300 movs r3, r0  */
L_1706:
    bl L_15ec                               /* @1706 fff771ff bl #-286 -> switch_u8_dispatch ; inline switch destinations 0x1712,0x172C,0x1750,0x176A,0x1792,0x17C6 */
    .byte 0x05,0x04,0x11,0x23,0x30,0x44,0x5e /* @170A switch_table */
    .byte 0x00 /* @1711 unclassified */
L_1712:
    ldr r0, [r5]                            /* @1712 2868 ldr r0, [r5]  */
L_1714:
    lsls r1, r4, #31                        /* @1714 e107 lsls r1, r4, #31  */
L_1716:
    lsrs r0, r0, #1                         /* @1716 4008 lsrs r0, r0, #1  */
L_1718:
    lsls r0, r0, #1                         /* @1718 4000 lsls r0, r0, #1  */
L_171a:
    lsrs r1, r1, #31                        /* @171A c90f lsrs r1, r1, #31  */
L_171c:
    orrs r0, r1                             /* @171C 0843 orrs r0, r1  */
L_171e:
    str r0, [r5]                            /* @171E 2860 str r0, [r5]  */
L_1720:
    cmp r4, #0                              /* @1720 002c cmp r4, #0  */
L_1722:
    beq L_17c8                              /* @1722 51d0 beq #162 -> 0x17C8 */
L_1724:
    ldr r0, [r5, #12]                       /* @1724 e868 ldr r0, [r5, #12]  */
L_1726:
    lsls r0, r0, #20                        /* @1726 0005 lsls r0, r0, #20  */
L_1728:
    bpl L_1720                              /* @1728 fad5 bpl #-12 -> 0x1720 */
L_172a:
    b L_17c8                                /* @172A 4de0 b #154 -> 0x17C8 */
L_172c:
    ldr r0, [pc, #164]                      /* @172C 2948 ldr r0, [pc, #164] literal 0x17D4 -> 0x40020DC0 */
L_172e:
    ldr r1, [r0, #12]                       /* @172E c168 ldr r1, [r0, #12]  */
L_1730:
    movs r2, #3                             /* @1730 0322 movs r2, #3  */
L_1732:
    orrs r1, r2                             /* @1732 1143 orrs r1, r2  */
L_1734:
    str r1, [r0, #12]                       /* @1734 c160 str r1, [r0, #12]  */
L_1736:
    ldr r0, [r5]                            /* @1736 2868 ldr r0, [r5]  */
L_1738:
    movs r1, #2                             /* @1738 0221 movs r1, #2  */
L_173a:
    bics r0, r1                             /* @173A 8843 bics r0, r1  */
L_173c:
    lsls r1, r4, #31                        /* @173C e107 lsls r1, r4, #31  */
L_173e:
    lsrs r1, r1, #30                        /* @173E 890f lsrs r1, r1, #30  */
L_1740:
    orrs r0, r1                             /* @1740 0843 orrs r0, r1  */
L_1742:
    str r0, [r5]                            /* @1742 2860 str r0, [r5]  */
L_1744:
    cmp r4, #0                              /* @1744 002c cmp r4, #0  */
L_1746:
    beq L_17c8                              /* @1746 3fd0 beq #126 -> 0x17C8 */
L_1748:
    ldr r0, [r5, #16]                       /* @1748 2869 ldr r0, [r5, #16]  */
L_174a:
    lsls r0, r0, #25                        /* @174A 4006 lsls r0, r0, #25  */
L_174c:
    bpl L_1744                              /* @174C fad5 bpl #-12 -> 0x1744 */
L_174e:
    b L_17c8                                /* @174E 3be0 b #118 -> 0x17C8 */
L_1750:
    ldr r0, [r5]                            /* @1750 2868 ldr r0, [r5]  */
L_1752:
    movs r1, #4                             /* @1752 0421 movs r1, #4  */
L_1754:
    bics r0, r1                             /* @1754 8843 bics r0, r1  */
L_1756:
    lsls r1, r4, #31                        /* @1756 e107 lsls r1, r4, #31  */
L_1758:
    lsrs r1, r1, #29                        /* @1758 490f lsrs r1, r1, #29  */
L_175a:
    orrs r0, r1                             /* @175A 0843 orrs r0, r1  */
L_175c:
    str r0, [r5]                            /* @175C 2860 str r0, [r5]  */
L_175e:
    cmp r4, #0                              /* @175E 002c cmp r4, #0  */
L_1760:
    beq L_17c8                              /* @1760 32d0 beq #100 -> 0x17C8 */
L_1762:
    ldr r0, [r5, #20]                       /* @1762 6869 ldr r0, [r5, #20]  */
L_1764:
    lsls r0, r0, #19                        /* @1764 c004 lsls r0, r0, #19  */
L_1766:
    bpl L_175e                              /* @1766 fad5 bpl #-12 -> 0x175E */
L_1768:
    b L_17c8                                /* @1768 2ee0 b #92 -> 0x17C8 */
L_176a:
    ldr r0, [pc, #104]                      /* @176A 1a48 ldr r0, [pc, #104] literal 0x17D4 -> 0x40020DC0 */
L_176c:
    subs r0, #64                            /* @176C 4038 subs r0, #64  */
L_176e:
    ldr r1, [r0, #12]                       /* @176E c168 ldr r1, [r0, #12]  */
L_1770:
    movs r2, #3                             /* @1770 0322 movs r2, #3  */
L_1772:
    lsls r2, r2, #14                        /* @1772 9203 lsls r2, r2, #14  */
L_1774:
    orrs r1, r2                             /* @1774 1143 orrs r1, r2  */
L_1776:
    str r1, [r0, #12]                       /* @1776 c160 str r1, [r0, #12]  */
L_1778:
    ldr r0, [r5]                            /* @1778 2868 ldr r0, [r5]  */
L_177a:
    movs r1, #8                             /* @177A 0821 movs r1, #8  */
L_177c:
    bics r0, r1                             /* @177C 8843 bics r0, r1  */
L_177e:
    lsls r1, r4, #31                        /* @177E e107 lsls r1, r4, #31  */
L_1780:
    lsrs r1, r1, #28                        /* @1780 090f lsrs r1, r1, #28  */
L_1782:
    orrs r0, r1                             /* @1782 0843 orrs r0, r1  */
L_1784:
    str r0, [r5]                            /* @1784 2860 str r0, [r5]  */
L_1786:
    cmp r4, #0                              /* @1786 002c cmp r4, #0  */
L_1788:
    beq L_17c8                              /* @1788 1ed0 beq #60 -> 0x17C8 */
L_178a:
    ldr r0, [r5, #24]                       /* @178A a869 ldr r0, [r5, #24]  */
L_178c:
    lsls r0, r0, #25                        /* @178C 4006 lsls r0, r0, #25  */
L_178e:
    bpl L_1786                              /* @178E fad5 bpl #-12 -> 0x1786 */
L_1790:
    b L_17c8                                /* @1790 1ae0 b #52 -> 0x17C8 */
L_1792:
    ldr r0, [r5, #32]                       /* @1792 286a ldr r0, [r5, #32]  */
L_1794:
    movs r1, #1                             /* @1794 0121 movs r1, #1  */
L_1796:
    lsls r1, r1, #16                        /* @1796 0904 lsls r1, r1, #16  */
L_1798:
    orrs r0, r1                             /* @1798 0843 orrs r0, r1  */
L_179a:
    str r0, [r5, #32]                       /* @179A 2862 str r0, [r5, #32]  */
L_179c:
    ldr r2, [pc, #56]                       /* @179C 0e4a ldr r2, [pc, #56] literal 0x17D8 -> 0x40002400 */
L_179e:
    ldr r0, [r2]                            /* @179E 1068 ldr r0, [r2]  */
L_17a0:
    movs r1, #1                             /* @17A0 0121 movs r1, #1  */
L_17a2:
    orrs r0, r1                             /* @17A2 0843 orrs r0, r1  */
L_17a4:
    str r0, [r2]                            /* @17A4 1060 str r0, [r2]  */
L_17a6:
    movs r0, #1                             /* @17A6 0120 movs r0, #1  */
L_17a8:
    bl L_2d98                               /* @17A8 01f0f6fa bl #5612 -> delay_100us_candidate */
L_17ac:
    ldr r0, [r5]                            /* @17AC 2868 ldr r0, [r5]  */
L_17ae:
    movs r1, #16                            /* @17AE 1021 movs r1, #16  */
L_17b0:
    bics r0, r1                             /* @17B0 8843 bics r0, r1  */
L_17b2:
    lsls r1, r4, #31                        /* @17B2 e107 lsls r1, r4, #31  */
L_17b4:
    lsrs r1, r1, #27                        /* @17B4 c90e lsrs r1, r1, #27  */
L_17b6:
    orrs r0, r1                             /* @17B6 0843 orrs r0, r1  */
L_17b8:
    str r0, [r5]                            /* @17B8 2860 str r0, [r5]  */
L_17ba:
    cmp r4, #0                              /* @17BA 002c cmp r4, #0  */
L_17bc:
    beq L_17c8                              /* @17BC 04d0 beq #8 -> 0x17C8 */
L_17be:
    ldr r0, [r5, #60]                       /* @17BE e86b ldr r0, [r5, #60]  */
L_17c0:
    lsls r0, r0, #13                        /* @17C0 4003 lsls r0, r0, #13  */
L_17c2:
    bpl L_17ba                              /* @17C2 fad5 bpl #-12 -> 0x17BA */
L_17c4:
    b L_17c8                                /* @17C4 00e0 b #0 -> 0x17C8 */
L_17c6:
    movs r6, #4                             /* @17C6 0426 movs r6, #4  */
L_17c8:
    str r7, [r5]                            /* @17C8 2f60 str r7, [r5]  */
L_17ca:
    mov r0, r6                              /* @17CA 3046 mov r0, r6  */
L_17cc:
    pop {r1, r2, r3, r4, r5, r6, r7, pc}    /* @17CC febd pop {r1, r2, r3, r4, r5, r6, r7, pc}  */
    .byte 0x00,0x00 /* @17CE unclassified */
L_17d0:
    .byte 0x00,0x20,0x00,0x40 /* @17D0 literal */
L_17d4:
    .byte 0xc0,0x0d,0x02,0x40 /* @17D4 literal */
L_17d8:
    .byte 0x00,0x24,0x00,0x40 /* @17D8 literal */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @17DC unclassified */

/* sub_17E8 */
L_17e8:
    push {r4, lr}                           /* @17E8 10b5 push {r4, lr}  */
L_17ea:
    sub sp, #40                             /* @17EA 8ab0 sub sp, #40  */
L_17ec:
    movs r4, #0                             /* @17EC 0024 movs r4, #0  */
L_17ee:
    movs r2, #20                            /* @17EE 1422 movs r2, #20  */
L_17f0:
    ldr r1, [pc, #288]                      /* @17F0 4849 ldr r1, [pc, #288] literal 0x1914 -> 0x000021C8 */
L_17f2:
    add r0, sp, #20                         /* @17F2 05a8 add r0, sp, #20  */
L_17f4:
    bl L_022a                               /* @17F4 fef719fd bl #-5582 -> sub_022A */
L_17f8:
    ldr r0, [pc, #280]                      /* @17F8 4648 ldr r0, [pc, #280] literal 0x1914 -> 0x000021C8 */
L_17fa:
    subs r0, r0, #4                         /* @17FA 001f subs r0, r0, #4  */
L_17fc:
    ldr r0, [r0]                            /* @17FC 0068 ldr r0, [r0]  */
L_17fe:
    str r0, [sp, #16]                       /* @17FE 0490 str r0, [sp, #16]  */
L_1800:
    movs r0, #0                             /* @1800 0020 movs r0, #0  */
L_1802:
    str r0, [sp, #4]                        /* @1802 0190 str r0, [sp, #4]  */
L_1804:
    str r0, [sp, #8]                        /* @1804 0290 str r0, [sp, #8]  */
L_1806:
    str r0, [sp, #12]                       /* @1806 0390 str r0, [sp, #12]  */
L_1808:
    ldr r0, [pc, #268]                      /* @1808 4348 ldr r0, [pc, #268] literal 0x1918 -> 0x00100C00 */
L_180a:
    ldrh r1, [r0]                           /* @180A 0188 ldrh r1, [r0]  */
L_180c:
    mov r2, sp                              /* @180C 6a46 mov r2, sp  */
L_180e:
    strh r1, [r2, #12]                      /* @180E 9181 strh r1, [r2, #12]  */
L_1810:
    ldrh r1, [r0, #2]                       /* @1810 4188 ldrh r1, [r0, #2]  */
L_1812:
    strh r1, [r2, #10]                      /* @1812 5181 strh r1, [r2, #10]  */
L_1814:
    ldrh r1, [r0, #4]                       /* @1814 8188 ldrh r1, [r0, #4]  */
L_1816:
    strh r1, [r2, #8]                       /* @1816 1181 strh r1, [r2, #8]  */
L_1818:
    ldrh r0, [r0, #6]                       /* @1818 c088 ldrh r0, [r0, #6]  */
L_181a:
    strh r0, [r2, #6]                       /* @181A d080 strh r0, [r2, #6]  */
L_181c:
    ldr r0, [pc, #248]                      /* @181C 3e48 ldr r0, [pc, #248] literal 0x1918 -> 0x00100C00 */
L_181e:
    adds r0, #32                            /* @181E 2030 adds r0, #32  */
L_1820:
    ldrh r0, [r0]                           /* @1820 0088 ldrh r0, [r0]  */
L_1822:
    strh r0, [r2, #4]                       /* @1822 9080 strh r0, [r2, #4]  */
L_1824:
    ldr r1, [pc, #244]                      /* @1824 3d49 ldr r1, [pc, #244] literal 0x191C -> 0x40002000 */
L_1826:
    ldr r0, [r1]                            /* @1826 0868 ldr r0, [r1]  */
L_1828:
    lsls r0, r0, #24                        /* @1828 0006 lsls r0, r0, #24  */
L_182a:
    lsrs r2, r0, #29                        /* @182A 420f lsrs r2, r0, #29  */
L_182c:
    ldr r0, [pc, #240]                      /* @182C 3c48 ldr r0, [pc, #240] literal 0x1920 -> 0x01E84800 */
L_182e:
    movs r3, r2                             /* @182E 1300 movs r3, r2  */
L_1830:
    bl L_15ec                               /* @1830 fff7dcfe bl #-584 -> switch_u8_dispatch ; inline switch destinations 0x183C,0x1888,0x188C,0x18A6,0x18AC,0x1906 */
    .byte 0x05,0x04,0x2a,0x2c,0x39,0x3c,0x69 /* @1834 switch_table */
    .byte 0x00 /* @183B unclassified */
L_183c:
    ldr r0, [r1, #12]                       /* @183C c868 ldr r0, [r1, #12]  */
L_183e:
    mov r2, sp                              /* @183E 6a46 mov r2, sp  */
L_1840:
    lsls r0, r0, #21                        /* @1840 4005 lsls r0, r0, #21  */
L_1842:
    ldrh r2, [r2, #12]                      /* @1842 9289 ldrh r2, [r2, #12]  */
L_1844:
    lsrs r0, r0, #21                        /* @1844 400d lsrs r0, r0, #21  */
L_1846:
    cmp r0, r2                              /* @1846 9042 cmp r0, r2  */
L_1848:
    bne L_184e                              /* @1848 01d1 bne #2 -> 0x184E */
L_184a:
    ldr r4, [sp, #36]                       /* @184A 099c ldr r4, [sp, #36]  */
L_184c:
    b L_1906                                /* @184C 5be0 b #182 -> 0x1906 */
L_184e:
    ldr r0, [r1, #12]                       /* @184E c868 ldr r0, [r1, #12]  */
L_1850:
    mov r2, sp                              /* @1850 6a46 mov r2, sp  */
L_1852:
    lsls r0, r0, #21                        /* @1852 4005 lsls r0, r0, #21  */
L_1854:
    ldrh r2, [r2, #10]                      /* @1854 5289 ldrh r2, [r2, #10]  */
L_1856:
    lsrs r0, r0, #21                        /* @1856 400d lsrs r0, r0, #21  */
L_1858:
    cmp r0, r2                              /* @1858 9042 cmp r0, r2  */
L_185a:
    bne L_1860                              /* @185A 01d1 bne #2 -> 0x1860 */
L_185c:
    ldr r4, [sp, #32]                       /* @185C 089c ldr r4, [sp, #32]  */
L_185e:
    b L_1906                                /* @185E 52e0 b #164 -> 0x1906 */
L_1860:
    ldr r0, [r1, #12]                       /* @1860 c868 ldr r0, [r1, #12]  */
L_1862:
    mov r2, sp                              /* @1862 6a46 mov r2, sp  */
L_1864:
    lsls r0, r0, #21                        /* @1864 4005 lsls r0, r0, #21  */
L_1866:
    ldrh r2, [r2, #8]                       /* @1866 1289 ldrh r2, [r2, #8]  */
L_1868:
    lsrs r0, r0, #21                        /* @1868 400d lsrs r0, r0, #21  */
L_186a:
    cmp r0, r2                              /* @186A 9042 cmp r0, r2  */
L_186c:
    bne L_1872                              /* @186C 01d1 bne #2 -> 0x1872 */
L_186e:
    ldr r4, [sp, #28]                       /* @186E 079c ldr r4, [sp, #28]  */
L_1870:
    b L_1906                                /* @1870 49e0 b #146 -> 0x1906 */
L_1872:
    ldr r0, [r1, #12]                       /* @1872 c868 ldr r0, [r1, #12]  */
L_1874:
    mov r2, sp                              /* @1874 6a46 mov r2, sp  */
L_1876:
    lsls r0, r0, #21                        /* @1876 4005 lsls r0, r0, #21  */
L_1878:
    ldrh r2, [r2, #6]                       /* @1878 d288 ldrh r2, [r2, #6]  */
L_187a:
    lsrs r0, r0, #21                        /* @187A 400d lsrs r0, r0, #21  */
L_187c:
    cmp r0, r2                              /* @187C 9042 cmp r0, r2  */
L_187e:
    bne L_1884                              /* @187E 01d1 bne #2 -> 0x1884 */
L_1880:
    ldr r4, [sp, #24]                       /* @1880 069c ldr r4, [sp, #24]  */
L_1882:
    b L_1906                                /* @1882 40e0 b #128 -> 0x1906 */
L_1884:
    ldr r4, [sp, #20]                       /* @1884 059c ldr r4, [sp, #20]  */
L_1886:
    b L_1906                                /* @1886 3ee0 b #124 -> 0x1906 */
L_1888:
    mov r4, r0                              /* @1888 0446 mov r4, r0  */
L_188a:
    b L_1906                                /* @188A 3ce0 b #120 -> 0x1906 */
L_188c:
    mov r0, sp                              /* @188C 6846 mov r0, sp  */
L_188e:
    ldrh r0, [r0, #4]                       /* @188E 8088 ldrh r0, [r0, #4]  */
L_1890:
    ldr r2, [r1, #20]                       /* @1890 4a69 ldr r2, [r1, #20]  */
L_1892:
    lsls r2, r2, #22                        /* @1892 9205 lsls r2, r2, #22  */
L_1894:
    lsrs r2, r2, #22                        /* @1894 920d lsrs r2, r2, #22  */
L_1896:
    cmp r0, r2                              /* @1896 9042 cmp r0, r2  */
L_1898:
    bne L_18a0                              /* @1898 02d1 bne #4 -> 0x18A0 */
L_189a:
    mov r0, sp                              /* @189A 6846 mov r0, sp  */
L_189c:
    ldrh r4, [r0, #18]                      /* @189C 448a ldrh r4, [r0, #18]  */
L_189e:
    b L_1906                                /* @189E 32e0 b #100 -> 0x1906 */
L_18a0:
    mov r0, sp                              /* @18A0 6846 mov r0, sp  */
L_18a2:
    ldrh r4, [r0, #16]                      /* @18A2 048a ldrh r4, [r0, #16]  */
L_18a4:
    b L_1906                                /* @18A4 2fe0 b #94 -> 0x1906 */
L_18a6:
    movs r4, #1                             /* @18A6 0124 movs r4, #1  */
L_18a8:
    lsls r4, r4, #15                        /* @18A8 e403 lsls r4, r4, #15  */
L_18aa:
    b L_1906                                /* @18AA 2ce0 b #88 -> 0x1906 */
L_18ac:
    ldr r2, [r1, #60]                       /* @18AC ca6b ldr r2, [r1, #60]  */
L_18ae:
    mvns r2, r2                             /* @18AE d243 mvns r2, r2  */
L_18b0:
    lsls r2, r2, #30                        /* @18B0 9207 lsls r2, r2, #30  */
L_18b2:
    bne L_18fe                              /* @18B2 24d1 bne #72 -> 0x18FE */
L_18b4:
    mov r0, sp                              /* @18B4 6846 mov r0, sp  */
L_18b6:
    ldrh r0, [r0, #12]                      /* @18B6 8089 ldrh r0, [r0, #12]  */
L_18b8:
    ldr r2, [r1, #12]                       /* @18B8 ca68 ldr r2, [r1, #12]  */
L_18ba:
    lsls r2, r2, #21                        /* @18BA 5205 lsls r2, r2, #21  */
L_18bc:
    lsrs r2, r2, #21                        /* @18BC 520d lsrs r2, r2, #21  */
L_18be:
    cmp r0, r2                              /* @18BE 9042 cmp r0, r2  */
L_18c0:
    bne L_18c6                              /* @18C0 01d1 bne #2 -> 0x18C6 */
L_18c2:
    ldr r0, [sp, #36]                       /* @18C2 0998 ldr r0, [sp, #36]  */
L_18c4:
    b L_18fe                                /* @18C4 1be0 b #54 -> 0x18FE */
L_18c6:
    mov r0, sp                              /* @18C6 6846 mov r0, sp  */
L_18c8:
    ldrh r0, [r0, #10]                      /* @18C8 4089 ldrh r0, [r0, #10]  */
L_18ca:
    ldr r2, [r1, #12]                       /* @18CA ca68 ldr r2, [r1, #12]  */
L_18cc:
    lsls r2, r2, #21                        /* @18CC 5205 lsls r2, r2, #21  */
L_18ce:
    lsrs r2, r2, #21                        /* @18CE 520d lsrs r2, r2, #21  */
L_18d0:
    cmp r0, r2                              /* @18D0 9042 cmp r0, r2  */
L_18d2:
    bne L_18d8                              /* @18D2 01d1 bne #2 -> 0x18D8 */
L_18d4:
    ldr r0, [sp, #32]                       /* @18D4 0898 ldr r0, [sp, #32]  */
L_18d6:
    b L_18fe                                /* @18D6 12e0 b #36 -> 0x18FE */
L_18d8:
    mov r0, sp                              /* @18D8 6846 mov r0, sp  */
L_18da:
    ldrh r0, [r0, #8]                       /* @18DA 0089 ldrh r0, [r0, #8]  */
L_18dc:
    ldr r2, [r1, #12]                       /* @18DC ca68 ldr r2, [r1, #12]  */
L_18de:
    lsls r2, r2, #21                        /* @18DE 5205 lsls r2, r2, #21  */
L_18e0:
    lsrs r2, r2, #21                        /* @18E0 520d lsrs r2, r2, #21  */
L_18e2:
    cmp r0, r2                              /* @18E2 9042 cmp r0, r2  */
L_18e4:
    bne L_18ea                              /* @18E4 01d1 bne #2 -> 0x18EA */
L_18e6:
    ldr r0, [sp, #28]                       /* @18E6 0798 ldr r0, [sp, #28]  */
L_18e8:
    b L_18fe                                /* @18E8 09e0 b #18 -> 0x18FE */
L_18ea:
    mov r0, sp                              /* @18EA 6846 mov r0, sp  */
L_18ec:
    ldrh r0, [r0, #6]                       /* @18EC c088 ldrh r0, [r0, #6]  */
L_18ee:
    ldr r2, [r1, #12]                       /* @18EE ca68 ldr r2, [r1, #12]  */
L_18f0:
    lsls r2, r2, #21                        /* @18F0 5205 lsls r2, r2, #21  */
L_18f2:
    lsrs r2, r2, #21                        /* @18F2 520d lsrs r2, r2, #21  */
L_18f4:
    cmp r0, r2                              /* @18F4 9042 cmp r0, r2  */
L_18f6:
    bne L_18fc                              /* @18F6 01d1 bne #2 -> 0x18FC */
L_18f8:
    ldr r0, [sp, #24]                       /* @18F8 0698 ldr r0, [sp, #24]  */
L_18fa:
    b L_18fe                                /* @18FA 00e0 b #0 -> 0x18FE */
L_18fc:
    ldr r0, [sp, #20]                       /* @18FC 0598 ldr r0, [sp, #20]  */
L_18fe:
    ldr r2, [r1, #60]                       /* @18FE ca6b ldr r2, [r1, #60]  */
L_1900:
    lsls r2, r2, #23                        /* @1900 d205 lsls r2, r2, #23  */
L_1902:
    lsrs r4, r2, #28                        /* @1902 140f lsrs r4, r2, #28  */
L_1904:
    muls r4, r0, r4                         /* @1904 4443 muls r4, r0, r4  */
L_1906:
    ldr r0, [r1]                            /* @1906 0868 ldr r0, [r1]  */
L_1908:
    lsls r0, r0, #21                        /* @1908 4005 lsls r0, r0, #21  */
L_190a:
    lsrs r1, r0, #29                        /* @190A 410f lsrs r1, r0, #29  */
L_190c:
    mov r0, r4                              /* @190C 2046 mov r0, r4  */
L_190e:
    lsrs r0, r1                             /* @190E c840 lsrs r0, r1  */
L_1910:
    add sp, #40                             /* @1910 0ab0 add sp, #40  */
L_1912:
    pop {r4, pc}                            /* @1912 10bd pop {r4, pc}  */
L_1914:
    .byte 0xc8,0x21,0x00,0x00 /* @1914 literal */
L_1918:
    .byte 0x00,0x0c,0x10,0x00 /* @1918 literal */
L_191c:
    .byte 0x00,0x20,0x00,0x40 /* @191C literal */
L_1920:
    .byte 0x00,0x48,0xe8,0x01 /* @1920 literal */

/* sub_1924 */
L_1924:
    push {r4, lr}                           /* @1924 10b5 push {r4, lr}  */
L_1926:
    bl L_17e8                               /* @1926 fff75fff bl #-322 -> sub_17E8 */
L_192a:
    ldr r1, [pc, #12]                       /* @192A 0349 ldr r1, [pc, #12] literal 0x1938 -> 0x40002000 */
L_192c:
    ldr r1, [r1]                            /* @192C 0968 ldr r1, [r1]  */
L_192e:
    lsls r1, r1, #19                        /* @192E c904 lsls r1, r1, #19  */
L_1930:
    lsrs r1, r1, #30                        /* @1930 890f lsrs r1, r1, #30  */
L_1932:
    lsrs r0, r1                             /* @1932 c840 lsrs r0, r1  */
L_1934:
    pop {r4, pc}                            /* @1934 10bd pop {r4, pc}  */
    .byte 0x00,0x00 /* @1936 unclassified */
L_1938:
    .byte 0x00,0x20,0x00,0x40 /* @1938 literal */

/* sub_193C */
L_193c:
    push {r1, r2, r3, r4, lr}               /* @193C 1eb5 push {r1, r2, r3, r4, lr}  */
L_193e:
    movs r2, #0                             /* @193E 0022 movs r2, #0  */
L_1940:
    movs r1, #0                             /* @1940 0021 movs r1, #0  */
L_1942:
    str r1, [sp]                            /* @1942 0091 str r1, [sp]  */
L_1944:
    str r1, [sp, #4]                        /* @1944 0191 str r1, [sp, #4]  */
L_1946:
    str r1, [sp, #8]                        /* @1946 0291 str r1, [sp, #8]  */
L_1948:
    ldr r1, [pc, #196]                      /* @1948 3149 ldr r1, [pc, #196] literal 0x1A10 -> 0x00100C00 */
L_194a:
    ldrh r3, [r1]                           /* @194A 0b88 ldrh r3, [r1]  */
L_194c:
    mov r4, sp                              /* @194C 6c46 mov r4, sp  */
L_194e:
    strh r3, [r4, #8]                       /* @194E 2381 strh r3, [r4, #8]  */
L_1950:
    ldrh r3, [r1, #2]                       /* @1950 4b88 ldrh r3, [r1, #2]  */
L_1952:
    strh r3, [r4, #6]                       /* @1952 e380 strh r3, [r4, #6]  */
L_1954:
    ldrh r3, [r1, #4]                       /* @1954 8b88 ldrh r3, [r1, #4]  */
L_1956:
    strh r3, [r4, #4]                       /* @1956 a380 strh r3, [r4, #4]  */
L_1958:
    ldrh r1, [r1, #6]                       /* @1958 c988 ldrh r1, [r1, #6]  */
L_195a:
    strh r1, [r4, #2]                       /* @195A 6180 strh r1, [r4, #2]  */
L_195c:
    ldrb r3, [r0, #2]                       /* @195C 8378 ldrb r3, [r0, #2]  */
L_195e:
    ldr r1, [pc, #180]                      /* @195E 2d49 ldr r1, [pc, #180] literal 0x1A14 -> 0x40002000 */
L_1960:
    cmp r3, #3                              /* @1960 032b cmp r3, #3  */
L_1962:
    beq L_1974                              /* @1962 07d0 beq #14 -> 0x1974 */
L_1964:
    ldrb r3, [r0, #3]                       /* @1964 c378 ldrb r3, [r0, #3]  */
L_1966:
    ldr r4, [pc, #176]                      /* @1966 2c4c ldr r4, [pc, #176] literal 0x1A18 -> 0x01E84800 */
L_1968:
    muls r3, r4, r3                         /* @1968 6343 muls r3, r4, r3  */
L_196a:
    ldr r4, [pc, #176]                      /* @196A 2c4c ldr r4, [pc, #176] literal 0x1A1C -> 0x02DC6C00 */
L_196c:
    cmp r3, r4                              /* @196C a342 cmp r3, r4  */
L_196e:
    bls L_19c8                              /* @196E 2bd9 bls #86 -> 0x19C8 */
L_1970:
    movs r0, #6                             /* @1970 0620 movs r0, #6  */
L_1972:
    pop {r1, r2, r3, r4, pc}                /* @1972 1ebd pop {r1, r2, r3, r4, pc}  */
L_1974:
    mov r3, sp                              /* @1974 6b46 mov r3, sp  */
L_1976:
    ldrh r3, [r3, #8]                       /* @1976 1b89 ldrh r3, [r3, #8]  */
L_1978:
    ldr r4, [r1, #12]                       /* @1978 cc68 ldr r4, [r1, #12]  */
L_197a:
    lsls r4, r4, #21                        /* @197A 6405 lsls r4, r4, #21  */
L_197c:
    lsrs r4, r4, #21                        /* @197C 640d lsrs r4, r4, #21  */
L_197e:
    cmp r3, r4                              /* @197E a342 cmp r3, r4  */
L_1980:
    bne L_1988                              /* @1980 02d1 bne #4 -> 0x1988 */
L_1982:
    ldrb r3, [r0, #3]                       /* @1982 c378 ldrb r3, [r0, #3]  */
L_1984:
    cmp r3, #2                              /* @1984 022b cmp r3, #2  */
L_1986:
    bhi L_19c4                              /* @1986 1dd8 bhi #58 -> 0x19C4 */
L_1988:
    mov r3, sp                              /* @1988 6b46 mov r3, sp  */
L_198a:
    ldrh r3, [r3, #6]                       /* @198A db88 ldrh r3, [r3, #6]  */
L_198c:
    ldr r4, [r1, #12]                       /* @198C cc68 ldr r4, [r1, #12]  */
L_198e:
    lsls r4, r4, #21                        /* @198E 6405 lsls r4, r4, #21  */
L_1990:
    lsrs r4, r4, #21                        /* @1990 640d lsrs r4, r4, #21  */
L_1992:
    cmp r3, r4                              /* @1992 a342 cmp r3, r4  */
L_1994:
    bne L_199c                              /* @1994 02d1 bne #4 -> 0x199C */
L_1996:
    ldrb r3, [r0, #3]                       /* @1996 c378 ldrb r3, [r0, #3]  */
L_1998:
    cmp r3, #2                              /* @1998 022b cmp r3, #2  */
L_199a:
    bhi L_19c4                              /* @199A 13d8 bhi #38 -> 0x19C4 */
L_199c:
    mov r3, sp                              /* @199C 6b46 mov r3, sp  */
L_199e:
    ldrh r3, [r3, #4]                       /* @199E 9b88 ldrh r3, [r3, #4]  */
L_19a0:
    ldr r4, [r1, #12]                       /* @19A0 cc68 ldr r4, [r1, #12]  */
L_19a2:
    lsls r4, r4, #21                        /* @19A2 6405 lsls r4, r4, #21  */
L_19a4:
    lsrs r4, r4, #21                        /* @19A4 640d lsrs r4, r4, #21  */
L_19a6:
    cmp r3, r4                              /* @19A6 a342 cmp r3, r4  */
L_19a8:
    bne L_19b0                              /* @19A8 02d1 bne #4 -> 0x19B0 */
L_19aa:
    ldrb r3, [r0, #3]                       /* @19AA c378 ldrb r3, [r0, #3]  */
L_19ac:
    cmp r3, #3                              /* @19AC 032b cmp r3, #3  */
L_19ae:
    bhi L_19c4                              /* @19AE 09d8 bhi #18 -> 0x19C4 */
L_19b0:
    mov r3, sp                              /* @19B0 6b46 mov r3, sp  */
L_19b2:
    ldrh r3, [r3, #2]                       /* @19B2 5b88 ldrh r3, [r3, #2]  */
L_19b4:
    ldr r4, [r1, #12]                       /* @19B4 cc68 ldr r4, [r1, #12]  */
L_19b6:
    lsls r4, r4, #21                        /* @19B6 6405 lsls r4, r4, #21  */
L_19b8:
    lsrs r4, r4, #21                        /* @19B8 640d lsrs r4, r4, #21  */
L_19ba:
    cmp r3, r4                              /* @19BA a342 cmp r3, r4  */
L_19bc:
    bne L_19c8                              /* @19BC 04d1 bne #8 -> 0x19C8 */
L_19be:
    ldrb r3, [r0, #3]                       /* @19BE c378 ldrb r3, [r0, #3]  */
L_19c0:
    cmp r3, #6                              /* @19C0 062b cmp r3, #6  */
L_19c2:
    bls L_19c8                              /* @19C2 01d9 bls #2 -> 0x19C8 */
L_19c4:
    movs r0, #6                             /* @19C4 0620 movs r0, #6  */
L_19c6:
    pop {r1, r2, r3, r4, pc}                /* @19C6 1ebd pop {r1, r2, r3, r4, pc}  */
L_19c8:
    ldr r3, [r1, #60]                       /* @19C8 cb6b ldr r3, [r1, #60]  */
L_19ca:
    movs r4, #3                             /* @19CA 0324 movs r4, #3  */
L_19cc:
    lsls r4, r4, #13                        /* @19CC 6403 lsls r4, r4, #13  */
L_19ce:
    bics r3, r4                             /* @19CE a343 bics r3, r4  */
L_19d0:
    ldrb r4, [r0]                           /* @19D0 0478 ldrb r4, [r0]  */
L_19d2:
    lsls r4, r4, #30                        /* @19D2 a407 lsls r4, r4, #30  */
L_19d4:
    lsrs r4, r4, #17                        /* @19D4 640c lsrs r4, r4, #17  */
L_19d6:
    orrs r3, r4                             /* @19D6 2343 orrs r3, r4  */
L_19d8:
    str r3, [r1, #60]                       /* @19D8 cb63 str r3, [r1, #60]  */
L_19da:
    ldr r3, [r1, #60]                       /* @19DA cb6b ldr r3, [r1, #60]  */
L_19dc:
    movs r4, #28                            /* @19DC 1c24 movs r4, #28  */
L_19de:
    bics r3, r4                             /* @19DE a343 bics r3, r4  */
L_19e0:
    ldrb r4, [r0, #1]                       /* @19E0 4478 ldrb r4, [r0, #1]  */
L_19e2:
    lsls r4, r4, #29                        /* @19E2 6407 lsls r4, r4, #29  */
L_19e4:
    lsrs r4, r4, #27                        /* @19E4 e40e lsrs r4, r4, #27  */
L_19e6:
    orrs r3, r4                             /* @19E6 2343 orrs r3, r4  */
L_19e8:
    str r3, [r1, #60]                       /* @19E8 cb63 str r3, [r1, #60]  */
L_19ea:
    ldr r3, [r1, #60]                       /* @19EA cb6b ldr r3, [r1, #60]  */
L_19ec:
    movs r4, #255                           /* @19EC ff24 movs r4, #255  */
L_19ee:
    adds r4, #225                           /* @19EE e134 adds r4, #225  */
L_19f0:
    bics r3, r4                             /* @19F0 a343 bics r3, r4  */
L_19f2:
    ldrb r4, [r0, #3]                       /* @19F2 c478 ldrb r4, [r0, #3]  */
L_19f4:
    lsls r4, r4, #28                        /* @19F4 2407 lsls r4, r4, #28  */
L_19f6:
    lsrs r4, r4, #23                        /* @19F6 e40d lsrs r4, r4, #23  */
L_19f8:
    orrs r3, r4                             /* @19F8 2343 orrs r3, r4  */
L_19fa:
    str r3, [r1, #60]                       /* @19FA cb63 str r3, [r1, #60]  */
L_19fc:
    ldr r3, [r1, #60]                       /* @19FC cb6b ldr r3, [r1, #60]  */
L_19fe:
    ldrb r0, [r0, #2]                       /* @19FE 8078 ldrb r0, [r0, #2]  */
L_1a00:
    lsrs r3, r3, #2                         /* @1A00 9b08 lsrs r3, r3, #2  */
L_1a02:
    lsls r3, r3, #2                         /* @1A02 9b00 lsls r3, r3, #2  */
L_1a04:
    lsls r0, r0, #30                        /* @1A04 8007 lsls r0, r0, #30  */
L_1a06:
    lsrs r0, r0, #30                        /* @1A06 800f lsrs r0, r0, #30  */
L_1a08:
    orrs r3, r0                             /* @1A08 0343 orrs r3, r0  */
L_1a0a:
    str r3, [r1, #60]                       /* @1A0A cb63 str r3, [r1, #60]  */
L_1a0c:
    mov r0, r2                              /* @1A0C 1046 mov r0, r2  */
L_1a0e:
    pop {r1, r2, r3, r4, pc}                /* @1A0E 1ebd pop {r1, r2, r3, r4, pc}  */
L_1a10:
    .byte 0x00,0x0c,0x10,0x00 /* @1A10 literal */
L_1a14:
    .byte 0x00,0x20,0x00,0x40 /* @1A14 literal */
L_1a18:
    .byte 0x00,0x48,0xe8,0x01 /* @1A18 literal */
L_1a1c:
    .byte 0x00,0x6c,0xdc,0x02 /* @1A1C literal */

/* sub_1A20 */
L_1a20:
    push {r4, lr}                           /* @1A20 10b5 push {r4, lr}  */
L_1a22:
    mov r2, r0                              /* @1A22 0246 mov r2, r0  */
L_1a24:
    movs r0, #0                             /* @1A24 0020 movs r0, #0  */
L_1a26:
    ldr r3, [pc, #20]                       /* @1A26 054b ldr r3, [pc, #20] literal 0x1A3C -> 0x40002000 */
L_1a28:
    ldr r1, [r3, #60]                       /* @1A28 d96b ldr r1, [r3, #60]  */
L_1a2a:
    movs r4, #7                             /* @1A2A 0724 movs r4, #7  */
L_1a2c:
    lsls r4, r4, #15                        /* @1A2C e403 lsls r4, r4, #15  */
L_1a2e:
    lsls r2, r2, #29                        /* @1A2E 5207 lsls r2, r2, #29  */
L_1a30:
    bics r1, r4                             /* @1A30 a143 bics r1, r4  */
L_1a32:
    lsrs r2, r2, #14                        /* @1A32 920b lsrs r2, r2, #14  */
L_1a34:
    orrs r1, r2                             /* @1A34 1143 orrs r1, r2  */
L_1a36:
    str r1, [r3, #60]                       /* @1A36 d963 str r1, [r3, #60]  */
L_1a38:
    pop {r4, pc}                            /* @1A38 10bd pop {r4, pc}  */
    .byte 0x00,0x00 /* @1A3A unclassified */
L_1a3c:
    .byte 0x00,0x20,0x00,0x40 /* @1A3C literal */

/* sub_1A40 */
L_1a40:
    push {r4, lr}                           /* @1A40 10b5 push {r4, lr}  */
L_1a42:
    mov r2, r1                              /* @1A42 0a46 mov r2, r1  */
L_1a44:
    mov r1, r0                              /* @1A44 0146 mov r1, r0  */
L_1a46:
    ldr r0, [pc, #8]                        /* @1A46 0248 ldr r0, [pc, #8] literal 0x1A50 -> 0x40002020 */
L_1a48:
    bl L_16ac                               /* @1A48 fff730fe bl #-928 -> sub_16AC */
L_1a4c:
    movs r0, #0                             /* @1A4C 0020 movs r0, #0  */
L_1a4e:
    pop {r4, pc}                            /* @1A4E 10bd pop {r4, pc}  */
L_1a50:
    .byte 0x20,0x20,0x00,0x40 /* @1A50 literal */

/* sub_1A54 */
L_1a54:
    push {r4, lr}                           /* @1A54 10b5 push {r4, lr}  */
L_1a56:
    mov r2, r0                              /* @1A56 0246 mov r2, r0  */
L_1a58:
    movs r4, #0                             /* @1A58 0024 movs r4, #0  */
L_1a5a:
    bl L_231c                               /* @1A5A 00f05ffc bl #2238 -> sub_231C */
L_1a5e:
    ldr r1, [pc, #24]                       /* @1A5E 0649 ldr r1, [pc, #24] literal 0x1A78 -> 0x40002000 */
L_1a60:
    ldr r0, [r1]                            /* @1A60 0868 ldr r0, [r1]  */
L_1a62:
    movs r3, #224                           /* @1A62 e023 movs r3, #224  */
L_1a64:
    lsls r2, r2, #29                        /* @1A64 5207 lsls r2, r2, #29  */
L_1a66:
    bics r0, r3                             /* @1A66 9843 bics r0, r3  */
L_1a68:
    lsrs r2, r2, #24                        /* @1A68 120e lsrs r2, r2, #24  */
L_1a6a:
    orrs r0, r2                             /* @1A6A 1043 orrs r0, r2  */
L_1a6c:
    str r0, [r1]                            /* @1A6C 0860 str r0, [r1]  */
L_1a6e:
    bl L_1a7c                               /* @1A6E 00f005f8 bl #10 -> sub_1A7C */
L_1a72:
    mov r0, r4                              /* @1A72 2046 mov r0, r4  */
L_1a74:
    pop {r4, pc}                            /* @1A74 10bd pop {r4, pc}  */
    .byte 0x00,0x00 /* @1A76 unclassified */
L_1a78:
    .byte 0x00,0x20,0x00,0x40 /* @1A78 literal */

/* sub_1A7C */
L_1a7c:
    push {r4, lr}                           /* @1A7C 10b5 push {r4, lr}  */
L_1a7e:
    bl L_17e8                               /* @1A7E fff7b3fe bl #-666 -> sub_17E8 */
L_1a82:
    ldr r1, [pc, #4]                        /* @1A82 0149 ldr r1, [pc, #4] literal 0x1A88 -> 0x20000000 */
L_1a84:
    str r0, [r1]                            /* @1A84 0860 str r0, [r1]  */
L_1a86:
    pop {r4, pc}                            /* @1A86 10bd pop {r4, pc}  */
L_1a88:
    .byte 0x00,0x00,0x00,0x20 /* @1A88 literal */

/* system_init */
L_1a8c:
    push {r3, r4, r5, lr}                   /* @1A8C 38b5 push {r3, r4, r5, lr}  */
L_1a8e:
    ldr r0, [pc, #136]                      /* @1A8E 2248 ldr r0, [pc, #136] literal 0x1B18 -> 0x00100C20 */
L_1a90:
    ldrh r0, [r0, #2]                       /* @1A90 4088 ldrh r0, [r0, #2]  */
L_1a92:
    ldr r4, [pc, #136]                      /* @1A92 224c ldr r4, [pc, #136] literal 0x1B1C -> 0x40002000 */
L_1a94:
    lsls r0, r0, #22                        /* @1A94 8005 lsls r0, r0, #22  */
L_1a96:
    ldr r1, [r4, #20]                       /* @1A96 6169 ldr r1, [r4, #20]  */
L_1a98:
    lsrs r0, r0, #22                        /* @1A98 800d lsrs r0, r0, #22  */
L_1a9a:
    lsrs r1, r1, #10                        /* @1A9A 890a lsrs r1, r1, #10  */
L_1a9c:
    lsls r1, r1, #10                        /* @1A9C 8902 lsls r1, r1, #10  */
L_1a9e:
    orrs r1, r0                             /* @1A9E 0143 orrs r1, r0  */
L_1aa0:
    str r1, [r4, #20]                       /* @1AA0 6161 str r1, [r4, #20]  */
L_1aa2:
    ldr r5, [pc, #116]                      /* @1AA2 1d4d ldr r5, [pc, #116] literal 0x1B18 -> 0x00100C20 */
L_1aa4:
    subs r5, #32                            /* @1AA4 203d subs r5, #32  */
L_1aa6:
    ldrh r0, [r5, #8]                       /* @1AA6 2889 ldrh r0, [r5, #8]  */
L_1aa8:
    ldr r1, [r4, #12]                       /* @1AA8 e168 ldr r1, [r4, #12]  */
L_1aaa:
    lsls r0, r0, #21                        /* @1AAA 4005 lsls r0, r0, #21  */
L_1aac:
    lsrs r0, r0, #21                        /* @1AAC 400d lsrs r0, r0, #21  */
L_1aae:
    lsrs r1, r1, #11                        /* @1AAE c90a lsrs r1, r1, #11  */
L_1ab0:
    lsls r1, r1, #11                        /* @1AB0 c902 lsls r1, r1, #11  */
L_1ab2:
    orrs r1, r0                             /* @1AB2 0143 orrs r1, r0  */
L_1ab4:
    str r1, [r4, #12]                       /* @1AB4 e160 str r1, [r4, #12]  */
L_1ab6:
    bl L_2280                               /* @1AB6 00f0e3fb bl #1990 -> sub_2280 */
L_1aba:
    ldr r3, [pc, #100]                      /* @1ABA 194b ldr r3, [pc, #100] literal 0x1B20 -> 0x00005A5A */
L_1abc:
    str r3, [r4, #8]                        /* @1ABC a360 str r3, [r4, #8]  */
L_1abe:
    ldr r2, [pc, #100]                      /* @1ABE 194a ldr r2, [pc, #100] literal 0x1B24 -> 0x0000A5A5 */
L_1ac0:
    str r2, [r4, #8]                        /* @1AC0 a260 str r2, [r4, #8]  */
L_1ac2:
    ldr r1, [r4]                            /* @1AC2 2168 ldr r1, [r4]  */
L_1ac4:
    movs r0, #7                             /* @1AC4 0720 movs r0, #7  */
L_1ac6:
    lsls r0, r0, #8                         /* @1AC6 0002 lsls r0, r0, #8  */
L_1ac8:
    orrs r1, r0                             /* @1AC8 0143 orrs r1, r0  */
L_1aca:
    str r1, [r4]                            /* @1ACA 2160 str r1, [r4]  */
L_1acc:
    ldrh r1, [r5, #8]                       /* @1ACC 2989 ldrh r1, [r5, #8]  */
L_1ace:
    str r1, [r4, #12]                       /* @1ACE e160 str r1, [r4, #12]  */
L_1ad0:
    ldrh r1, [r5, #6]                       /* @1AD0 e988 ldrh r1, [r5, #6]  */
L_1ad2:
    str r1, [r4, #12]                       /* @1AD2 e160 str r1, [r4, #12]  */
L_1ad4:
    ldrh r1, [r5, #4]                       /* @1AD4 a988 ldrh r1, [r5, #4]  */
L_1ad6:
    str r1, [r4, #12]                       /* @1AD6 e160 str r1, [r4, #12]  */
L_1ad8:
    ldrh r1, [r5]                           /* @1AD8 2988 ldrh r1, [r5]  */
L_1ada:
    str r1, [r4, #12]                       /* @1ADA e160 str r1, [r4, #12]  */
L_1adc:
    str r3, [r4, #8]                        /* @1ADC a360 str r3, [r4, #8]  */
L_1ade:
    str r2, [r4, #8]                        /* @1ADE a260 str r2, [r4, #8]  */
L_1ae0:
    ldr r1, [r4]                            /* @1AE0 2168 ldr r1, [r4]  */
L_1ae2:
    bics r1, r0                             /* @1AE2 8143 bics r1, r0  */
L_1ae4:
    str r1, [r4]                            /* @1AE4 2160 str r1, [r4]  */
L_1ae6:
    movs r0, #1                             /* @1AE6 0120 movs r0, #1  */
L_1ae8:
    bl L_1234                               /* @1AE8 fff7a4fb bl #-2232 -> sub_1234 */
L_1aec:
    movs r0, #3                             /* @1AEC 0320 movs r0, #3  */
L_1aee:
    mov r2, sp                              /* @1AEE 6a46 mov r2, sp  */
L_1af0:
    strb r0, [r2]                           /* @1AF0 1070 strb r0, [r2]  */
L_1af2:
    movs r1, #4                             /* @1AF2 0421 movs r1, #4  */
L_1af4:
    strb r1, [r2, #1]                       /* @1AF4 5170 strb r1, [r2, #1]  */
L_1af6:
    strb r0, [r2, #2]                       /* @1AF6 9070 strb r0, [r2, #2]  */
L_1af8:
    movs r0, #2                             /* @1AF8 0220 movs r0, #2  */
L_1afa:
    strb r0, [r2, #3]                       /* @1AFA d070 strb r0, [r2, #3]  */
L_1afc:
    mov r0, sp                              /* @1AFC 6846 mov r0, sp  */
L_1afe:
    bl L_193c                               /* @1AFE fff71dff bl #-454 -> sub_193C */
L_1b02:
    movs r0, #7                             /* @1B02 0720 movs r0, #7  */
L_1b04:
    bl L_1a20                               /* @1B04 fff78cff bl #-232 -> sub_1A20 */
L_1b08:
    movs r1, #1                             /* @1B08 0121 movs r1, #1  */
L_1b0a:
    movs r0, #4                             /* @1B0A 0420 movs r0, #4  */
L_1b0c:
    bl L_16ec                               /* @1B0C fff7eefd bl #-1060 -> sub_16EC */
L_1b10:
    movs r0, #4                             /* @1B10 0420 movs r0, #4  */
L_1b12:
    bl L_1a54                               /* @1B12 fff79fff bl #-194 -> sub_1A54 */
L_1b16:
    pop {r3, r4, r5, pc}                    /* @1B16 38bd pop {r3, r4, r5, pc}  */
L_1b18:
    .byte 0x20,0x0c,0x10,0x00 /* @1B18 literal */
L_1b1c:
    .byte 0x00,0x20,0x00,0x40 /* @1B1C literal */
L_1b20:
    .byte 0x5a,0x5a,0x00,0x00 /* @1B20 literal */
L_1b24:
    .byte 0xa5,0xa5,0x00,0x00 /* @1B24 literal */

/* foreground_ui_protection */
L_1b28:
    push {r3, r4, r5, r6, r7, lr}           /* @1B28 f8b5 push {r3, r4, r5, r6, r7, lr}  */
L_1b2a:
    ldr r0, [pc, #372]                      /* @1B2A 5d48 ldr r0, [pc, #372] literal 0x1CA0 -> 0x2000072C */
L_1b2c:
    ldrb r0, [r0, #11]                      /* @1B2C c07a ldrb r0, [r0, #11]  */
L_1b2e:
    ldr r7, [pc, #372]                      /* @1B2E 5d4f ldr r7, [pc, #372] literal 0x1CA4 -> 0x20000648 */
L_1b30:
    movs r4, #0                             /* @1B30 0024 movs r4, #0  */
L_1b32:
    ldr r5, [pc, #372]                      /* @1B32 5d4d ldr r5, [pc, #372] literal 0x1CA8 -> 0x200005DC */
L_1b34:
    cmp r0, #1                              /* @1B34 0128 cmp r0, #1  */
L_1b36:
    bne L_1ba4                              /* @1B36 35d1 bne #106 -> 0x1BA4 */
L_1b38:
    ldrb r0, [r7, #1]                       /* @1B38 7878 ldrb r0, [r7, #1]  */
L_1b3a:
    cmp r0, #0                              /* @1B3A 0028 cmp r0, #0  */
L_1b3c:
    beq L_1ba4                              /* @1B3C 32d0 beq #100 -> 0x1BA4 */
L_1b3e:
    movs r0, #1                             /* @1B3E 0120 movs r0, #1  */
L_1b40:
    bl L_3f28                               /* @1B40 02f0f2f9 bl #9188 -> sub_3F28 */
L_1b44:
    ldr r0, [pc, #360]                      /* @1B44 5a48 ldr r0, [pc, #360] literal 0x1CB0 -> 0x20000628 */
L_1b46:
    ldr r1, [pc, #356]                      /* @1B46 5949 ldr r1, [pc, #356] literal 0x1CAC -> 0x2000059C */
L_1b48:
    str r1, [r0, #4]                        /* @1B48 4160 str r1, [r0, #4]  */
L_1b4a:
    strh r4, [r5, #4]                       /* @1B4A ac80 strh r4, [r5, #4]  */
L_1b4c:
    ldrb r0, [r5]                           /* @1B4C 2878 ldrb r0, [r5]  */
L_1b4e:
    cmp r0, #0                              /* @1B4E 0028 cmp r0, #0  */
L_1b50:
    beq L_1b5a                              /* @1B50 03d0 beq #6 -> 0x1B5A */
L_1b52:
    movs r0, #175                           /* @1B52 af20 movs r0, #175  */
L_1b54:
    bl L_4d84                               /* @1B54 03f016f9 bl #12844 -> sub_4D84 */
L_1b58:
    strb r4, [r5]                           /* @1B58 2c70 strb r4, [r5]  */
L_1b5a:
    adr r2, L_1cb4                          /* @1B5A 56a2 adr r2, #344 address 0x1CB4 -> 0x00001CB4 'Enter..' */
L_1b5c:
    adr r1, L_1cbc                          /* @1B5C 57a1 adr r1, #348 address 0x1CBC -> 0x00001CBC ' Protect!' */
L_1b5e:
    adr r0, L_1cc8                          /* @1B5E 5aa0 adr r0, #360 address 0x1CC8 -> 0x00001CC8 'Wrong' */
L_1b60:
    bl L_4884                               /* @1B60 02f090fe bl #11552 -> display_three_lines */
L_1b64:
    ldr r6, [pc, #360]                      /* @1B64 5a4e ldr r6, [pc, #360] literal 0x1CD0 -> 0x200005D8 */
L_1b66:
    b L_1b72                                /* @1B66 04e0 b #8 -> 0x1B72 */
L_1b68:
    bl L_27e8                               /* @1B68 00f03efe bl #3196 -> update_multiturn_angle */
L_1b6c:
    ldrb r0, [r6]                           /* @1B6C 3078 ldrb r0, [r6]  */
L_1b6e:
    cmp r0, #1                              /* @1B6E 0128 cmp r0, #1  */
L_1b70:
    beq L_1be6                              /* @1B70 39d0 beq #114 -> 0x1BE6 */
L_1b72:
    movs r1, #10                            /* @1B72 0a21 movs r1, #10  */
L_1b74:
    movs r0, #0                             /* @1B74 0020 movs r0, #0  */
L_1b76:
    bl L_135c                               /* @1B76 fff7f1fb bl #-2078 -> sub_135C */
L_1b7a:
    cmp r0, #1                              /* @1B7A 0128 cmp r0, #1  */
L_1b7c:
    beq L_1b68                              /* @1B7C f4d0 beq #-24 -> 0x1B68 */
L_1b7e:
    strb r4, [r7, #1]                       /* @1B7E 7c70 strb r4, [r7, #1]  */
L_1b80:
    ldr r0, [pc, #336]                      /* @1B80 5448 ldr r0, [pc, #336] literal 0x1CD4 -> 0x20000604 */
L_1b82:
    strb r4, [r0, #3]                       /* @1B82 c470 strb r4, [r0, #3]  */
L_1b84:
    bl L_20e4                               /* @1B84 00f0aefa bl #1372 -> sub_20E4 */
L_1b88:
    bl L_3e54                               /* @1B88 02f064f9 bl #8904 -> sub_3E54 */
L_1b8c:
    bl L_2090                               /* @1B8C 00f080fa bl #1280 -> sub_2090 */
L_1b90:
    str r4, [r7, #36]                       /* @1B90 7c62 str r4, [r7, #36]  */
L_1b92:
    ldr r0, [pc, #324]                      /* @1B92 5148 ldr r0, [pc, #324] literal 0x1CD8 -> 0x20000358 */
L_1b94:
    bl L_4574                               /* @1B94 02f0eefc bl #10716 -> sub_4574 */
L_1b98:
    ldr r0, [pc, #276]                      /* @1B98 4548 ldr r0, [pc, #276] literal 0x1CB0 -> 0x20000628 */
L_1b9a:
    strb r4, [r0, #2]                       /* @1B9A 8470 strb r4, [r0, #2]  */
L_1b9c:
    strb r4, [r5, #1]                       /* @1B9C 6c70 strb r4, [r5, #1]  */
L_1b9e:
    movs r0, #32                            /* @1B9E 2020 movs r0, #32  */
L_1ba0:
    bl L_2de8                               /* @1BA0 01f022f9 bl #4676 -> delay_ms_candidate */
L_1ba4:
    ldrb r0, [r5]                           /* @1BA4 2878 ldrb r0, [r5]  */
L_1ba6:
    cmp r0, #0                              /* @1BA6 0028 cmp r0, #0  */
L_1ba8:
    beq L_1bb0                              /* @1BA8 02d0 beq #4 -> 0x1BB0 */
L_1baa:
    ldrb r0, [r7, #1]                       /* @1BAA 7878 ldrb r0, [r7, #1]  */
L_1bac:
    cmp r0, #0                              /* @1BAC 0028 cmp r0, #0  */
L_1bae:
    beq L_1c6c                              /* @1BAE 5dd0 beq #186 -> 0x1C6C */
L_1bb0:
    ldrh r0, [r5, #2]                       /* @1BB0 6888 ldrh r0, [r5, #2]  */
L_1bb2:
    cmp r0, #10                             /* @1BB2 0a28 cmp r0, #10  */
L_1bb4:
    blo L_1bc0                              /* @1BB4 04d3 blo #8 -> 0x1BC0 */
L_1bb6:
    bl L_2fe0                               /* @1BB6 01f013fa bl #5158 -> display_banner */
L_1bba:
    strh r4, [r5, #2]                       /* @1BBA 6c80 strh r4, [r5, #2]  */
L_1bbc:
    strb r4, [r5]                           /* @1BBC 2c70 strb r4, [r5]  */
L_1bbe:
    strh r4, [r5, #4]                       /* @1BBE ac80 strh r4, [r5, #4]  */
L_1bc0:
    ldr r6, [pc, #236]                      /* @1BC0 3b4e ldr r6, [pc, #236] literal 0x1CB0 -> 0x20000628 */
L_1bc2:
    ldrb r0, [r6, #2]                       /* @1BC2 b078 ldrb r0, [r6, #2]  */
L_1bc4:
    ldr r1, [r6, #4]                        /* @1BC4 7168 ldr r1, [r6, #4]  */
L_1bc6:
    rsbs r0, r0, #0                         /* @1BC6 4042 rsbs r0, r0, #0  */
L_1bc8:
    tst r0, r1                              /* @1BC8 0842 tst r0, r1  */
L_1bca:
    bne L_1bd2                              /* @1BCA 02d1 bne #4 -> 0x1BD2 */
L_1bcc:
    bl L_3220                               /* @1BCC 01f028fb bl #5712 -> sub_3220 */
L_1bd0:
    b L_1bd6                                /* @1BD0 01e0 b #2 -> 0x1BD6 */
L_1bd2:
    bl L_311c                               /* @1BD2 01f0a3fa bl #5446 -> sub_311C */
L_1bd6:
    movs r1, #4                             /* @1BD6 0421 movs r1, #4  */
L_1bd8:
    movs r0, #64                            /* @1BD8 4020 movs r0, #64  */
L_1bda:
    bl L_135c                               /* @1BDA fff7bffb bl #-2178 -> sub_135C */
L_1bde:
    movs r7, #1                             /* @1BDE 0127 movs r7, #1  */
L_1be0:
    cmp r0, #0                              /* @1BE0 0028 cmp r0, #0  */
L_1be2:
    beq L_1bea                              /* @1BE2 02d0 beq #4 -> 0x1BEA */
L_1be4:
    b L_1c0a                                /* @1BE4 11e0 b #34 -> 0x1C0A */
L_1be6:
    strb r4, [r6]                           /* @1BE6 3470 strb r4, [r6]  */
L_1be8:
    b L_1b7e                                /* @1BE8 c9e7 b #-110 -> 0x1B7E */
L_1bea:
    ldrb r0, [r5, #1]                       /* @1BEA 6878 ldrb r0, [r5, #1]  */
L_1bec:
    lsls r1, r0, #29                        /* @1BEC 4107 lsls r1, r0, #29  */
L_1bee:
    bmi L_1c0a                              /* @1BEE 0cd4 bmi #24 -> 0x1C0A */
L_1bf0:
    movs r1, #4                             /* @1BF0 0421 movs r1, #4  */
L_1bf2:
    orrs r0, r1                             /* @1BF2 0843 orrs r0, r1  */
L_1bf4:
    strb r0, [r5, #1]                       /* @1BF4 6870 strb r0, [r5, #1]  */
L_1bf6:
    strh r4, [r5, #4]                       /* @1BF6 ac80 strh r4, [r5, #4]  */
L_1bf8:
    ldrb r0, [r6, #2]                       /* @1BF8 b078 ldrb r0, [r6, #2]  */
L_1bfa:
    eors r0, r7                             /* @1BFA 7840 eors r0, r7  */
L_1bfc:
    strb r0, [r6, #2]                       /* @1BFC b070 strb r0, [r6, #2]  */
L_1bfe:
    ldrb r0, [r6, #2]                       /* @1BFE b078 ldrb r0, [r6, #2]  */
L_1c00:
    cmp r0, #0                              /* @1C00 0028 cmp r0, #0  */
L_1c02:
    bne L_1c0a                              /* @1C02 02d1 bne #4 -> 0x1C0A */
L_1c04:
    str r4, [r6, #8]                        /* @1C04 b460 str r4, [r6, #8]  */
L_1c06:
    strb r4, [r6, #1]                       /* @1C06 7470 strb r4, [r6, #1]  */
L_1c08:
    strb r4, [r6]                           /* @1C08 3470 strb r4, [r6]  */
L_1c0a:
    movs r1, #4                             /* @1C0A 0421 movs r1, #4  */
L_1c0c:
    movs r0, #64                            /* @1C0C 4020 movs r0, #64  */
L_1c0e:
    bl L_135c                               /* @1C0E fff7a5fb bl #-2230 -> sub_135C */
L_1c12:
    cmp r0, #1                              /* @1C12 0128 cmp r0, #1  */
L_1c14:
    bne L_1c1e                              /* @1C14 03d1 bne #6 -> 0x1C1E */
L_1c16:
    ldrb r0, [r5, #1]                       /* @1C16 6878 ldrb r0, [r5, #1]  */
L_1c18:
    movs r1, #251                           /* @1C18 fb21 movs r1, #251  */
L_1c1a:
    ands r0, r1                             /* @1C1A 0840 ands r0, r1  */
L_1c1c:
    strb r0, [r5, #1]                       /* @1C1C 6870 strb r0, [r5, #1]  */
L_1c1e:
    ldr r0, [pc, #128]                      /* @1C1E 2048 ldr r0, [pc, #128] literal 0x1CA0 -> 0x2000072C */
L_1c20:
    ldrb r1, [r0, #9]                       /* @1C20 417a ldrb r1, [r0, #9]  */
L_1c22:
    cmp r1, #2                              /* @1C22 0229 cmp r1, #2  */
L_1c24:
    beq L_1c4a                              /* @1C24 11d0 beq #34 -> 0x1C4A */
L_1c26:
    ldrb r0, [r0, #10]                      /* @1C26 807a ldrb r0, [r0, #10]  */
L_1c28:
    cmp r0, #1                              /* @1C28 0128 cmp r0, #1  */
L_1c2a:
    bne L_1c4c                              /* @1C2A 0fd1 bne #30 -> 0x1C4C */
L_1c2c:
    ldrh r0, [r5, #4]                       /* @1C2C a888 ldrh r0, [r5, #4]  */
L_1c2e:
    cmp r0, #0                              /* @1C2E 0028 cmp r0, #0  */
L_1c30:
    bne L_1c4c                              /* @1C30 0cd1 bne #24 -> 0x1C4C */
L_1c32:
    ldr r1, [pc, #164]                      /* @1C32 2949 ldr r1, [pc, #164] literal 0x1CD8 -> 0x20000358 */
L_1c34:
    ldr r0, [r6, #4]                        /* @1C34 7068 ldr r0, [r6, #4]  */
L_1c36:
    cmp r0, r1                              /* @1C36 8842 cmp r0, r1  */
L_1c38:
    bne L_1c4c                              /* @1C38 08d1 bne #16 -> 0x1C4C */
L_1c3a:
    strh r7, [r5, #4]                       /* @1C3A af80 strh r7, [r5, #4]  */
L_1c3c:
    ldr r1, [pc, #156]                      /* @1C3C 2749 ldr r1, [pc, #156] literal 0x1CDC -> 0x0000FE89 */
L_1c3e:
    movs r0, #2                             /* @1C3E 0220 movs r0, #2  */
L_1c40:
    bl L_0908                               /* @1C40 fef762fe bl #-4924 -> sub_0908 */
L_1c44:
    movs r0, #2                             /* @1C44 0220 movs r0, #2  */
L_1c46:
    bl L_0924                               /* @1C46 fef76dfe bl #-4902 -> sub_0924 */
L_1c4a:
    pop {r3, r4, r5, r6, r7, pc}            /* @1C4A f8bd pop {r3, r4, r5, r6, r7, pc}  */
L_1c4c:
    ldrh r0, [r5, #4]                       /* @1C4C a888 ldrh r0, [r5, #4]  */
L_1c4e:
    movs r1, #1                             /* @1C4E 0121 movs r1, #1  */
L_1c50:
    lsls r1, r1, #12                        /* @1C50 0903 lsls r1, r1, #12  */
L_1c52:
    cmp r0, r1                              /* @1C52 8842 cmp r0, r1  */
L_1c54:
    blo L_1c4a                              /* @1C54 f9d3 blo #-14 -> 0x1C4A */
L_1c56:
    movs r0, #2                             /* @1C56 0220 movs r0, #2  */
L_1c58:
    bl L_093c                               /* @1C58 fef770fe bl #-4896 -> sub_093C */
L_1c5c:
    strb r7, [r5]                           /* @1C5C 2f70 strb r7, [r5]  */
L_1c5e:
    movs r0, #174                           /* @1C5E ae20 movs r0, #174  */
L_1c60:
    bl L_4d84                               /* @1C60 03f090f8 bl #12576 -> sub_4D84 */
L_1c64:
    strh r4, [r5, #4]                       /* @1C64 ac80 strh r4, [r5, #4]  */
L_1c66:
    movs r0, #7                             /* @1C66 0720 movs r0, #7  */
L_1c68:
    strb r0, [r5, #1]                       /* @1C68 6870 strb r0, [r5, #1]  */
L_1c6a:
    pop {r3, r4, r5, r6, r7, pc}            /* @1C6A f8bd pop {r3, r4, r5, r6, r7, pc}  */
L_1c6c:
    movs r1, #4                             /* @1C6C 0421 movs r1, #4  */
L_1c6e:
    movs r0, #64                            /* @1C6E 4020 movs r0, #64  */
L_1c70:
    bl L_135c                               /* @1C70 fff774fb bl #-2328 -> sub_135C */
L_1c74:
    cmp r0, #0                              /* @1C74 0028 cmp r0, #0  */
L_1c76:
    beq L_1c90                              /* @1C76 0bd0 beq #22 -> 0x1C90 */
L_1c78:
    movs r1, #9                             /* @1C78 0921 movs r1, #9  */
L_1c7a:
    movs r0, #0                             /* @1C7A 0020 movs r0, #0  */
L_1c7c:
    bl L_135c                               /* @1C7C fff76efb bl #-2340 -> sub_135C */
L_1c80:
    cmp r0, #0                              /* @1C80 0028 cmp r0, #0  */
L_1c82:
    beq L_1c90                              /* @1C82 05d0 beq #10 -> 0x1C90 */
L_1c84:
    movs r1, #10                            /* @1C84 0a21 movs r1, #10  */
L_1c86:
    movs r0, #0                             /* @1C86 0020 movs r0, #0  */
L_1c88:
    bl L_135c                               /* @1C88 fff768fb bl #-2352 -> sub_135C */
L_1c8c:
    cmp r0, #0                              /* @1C8C 0028 cmp r0, #0  */
L_1c8e:
    bne L_1c6a                              /* @1C8E ecd1 bne #-40 -> 0x1C6A */
L_1c90:
    movs r0, #7                             /* @1C90 0720 movs r0, #7  */
L_1c92:
    strb r0, [r5, #1]                       /* @1C92 6870 strb r0, [r5, #1]  */
L_1c94:
    strb r4, [r5]                           /* @1C94 2c70 strb r4, [r5]  */
L_1c96:
    strh r4, [r5, #4]                       /* @1C96 ac80 strh r4, [r5, #4]  */
L_1c98:
    movs r0, #175                           /* @1C98 af20 movs r0, #175  */
L_1c9a:
    bl L_4d84                               /* @1C9A 03f073f8 bl #12518 -> sub_4D84 */
L_1c9e:
    pop {r3, r4, r5, r6, r7, pc}            /* @1C9E f8bd pop {r3, r4, r5, r6, r7, pc}  */
L_1ca0:
    .byte 0x2c,0x07,0x00,0x20 /* @1CA0 literal */
L_1ca4:
    .byte 0x48,0x06,0x00,0x20 /* @1CA4 literal */
L_1ca8:
    .byte 0xdc,0x05,0x00,0x20 /* @1CA8 literal */
L_1cac:
    .byte 0x9c,0x05,0x00,0x20 /* @1CAC literal */
L_1cb0:
    .byte 0x28,0x06,0x00,0x20 /* @1CB0 literal */
L_1cb4:
    .byte 0x45,0x6e,0x74,0x65,0x72,0x2e,0x2e,0x00 /* @1CB4 unclassified */
L_1cbc:
    .byte 0x20,0x50,0x72,0x6f,0x74,0x65,0x63,0x74,0x21,0x00,0x00,0x00 /* @1CBC unclassified */
L_1cc8:
    .byte 0x57,0x72,0x6f,0x6e,0x67,0x00,0x00,0x00 /* @1CC8 unclassified */
L_1cd0:
    .byte 0xd8,0x05,0x00,0x20 /* @1CD0 literal */
L_1cd4:
    .byte 0x04,0x06,0x00,0x20 /* @1CD4 literal */
L_1cd8:
    .byte 0x58,0x03,0x00,0x20 /* @1CD8 literal */
L_1cdc:
    .byte 0x89,0xfe,0x00,0x00 /* @1CDC literal */

/* sub_1CE0 */
L_1ce0:
    push {r0, r1, r4, r5, r6, r7, lr}       /* @1CE0 f3b5 push {r0, r1, r4, r5, r6, r7, lr}  */
L_1ce2:
    sub sp, #12                             /* @1CE2 83b0 sub sp, #12  */
L_1ce4:
    ldr r4, [pc, #244]                      /* @1CE4 3d4c ldr r4, [pc, #244] literal 0x1DDC -> 0x200006C0 */
L_1ce6:
    ldr r0, [r4, #4]                        /* @1CE6 6068 ldr r0, [r4, #4]  */
L_1ce8:
    uxth r1, r0                             /* @1CE8 81b2 uxth r1, r0  */
L_1cea:
    ldr r0, [r4, #4]                        /* @1CEA 6068 ldr r0, [r4, #4]  */
L_1cec:
    movs r5, #75                            /* @1CEC 4b25 movs r5, #75  */
L_1cee:
    asrs r0, r0, #1                         /* @1CEE 4010 asrs r0, r0, #1  */
L_1cf0:
    lsls r5, r5, #14                        /* @1CF0 ad03 lsls r5, r5, #14  */
L_1cf2:
    adds r0, r0, r5                         /* @1CF2 4019 adds r0, r0, r5  */
L_1cf4:
    add r2, sp, #4                          /* @1CF4 01aa add r2, sp, #4  */
L_1cf6:
    bl L_1474                               /* @1CF6 fff7bdfb bl #-2182 -> sub_1474 */
L_1cfa:
    cmp r0, #0                              /* @1CFA 0028 cmp r0, #0  */
L_1cfc:
    beq L_1d0e                              /* @1CFC 07d0 beq #14 -> 0x1D0E */
L_1cfe:
    ldr r1, [r4, #4]                        /* @1CFE 6168 ldr r1, [r4, #4]  */
L_1d00:
    ldr r0, [r4, #4]                        /* @1D00 6068 ldr r0, [r4, #4]  */
L_1d02:
    asrs r0, r0, #1                         /* @1D02 4010 asrs r0, r0, #1  */
L_1d04:
    adds r0, r0, r5                         /* @1D04 4019 adds r0, r0, r5  */
L_1d06:
    bl L_0110                               /* @1D06 fef703fa bl #-7162 -> udivmod32 */
L_1d0a:
    mov r5, r0                              /* @1D0A 0546 mov r5, r0  */
L_1d0c:
    b L_1d10                                /* @1D0C 00e0 b #0 -> 0x1D10 */
L_1d0e:
    ldr r5, [sp, #4]                        /* @1D0E 019d ldr r5, [sp, #4]  */
L_1d10:
    ldr r0, [sp, #12]                       /* @1D10 0398 ldr r0, [sp, #12]  */
L_1d12:
    bl L_373a                               /* @1D12 01f012fd bl #6692 -> abs32_candidate */
L_1d16:
    muls r0, r5, r0                         /* @1D16 6843 muls r0, r5, r0  */
L_1d18:
    adds r0, #255                           /* @1D18 ff30 adds r0, #255  */
L_1d1a:
    adds r0, #255                           /* @1D1A ff30 adds r0, #255  */
L_1d1c:
    adds r0, #2                             /* @1D1C 0230 adds r0, #2  */
L_1d1e:
    lsls r0, r0, #6                         /* @1D1E 8001 lsls r0, r0, #6  */
L_1d20:
    lsrs r0, r0, #16                        /* @1D20 000c lsrs r0, r0, #16  */
L_1d22:
    ldr r7, [pc, #188]                      /* @1D22 2f4f ldr r7, [pc, #188] literal 0x1DE0 -> 0x2000072C */
L_1d24:
    ldrh r1, [r7, #26]                      /* @1D24 798b ldrh r1, [r7, #26]  */
L_1d26:
    cmp r1, r0                              /* @1D26 8142 cmp r1, r0  */
L_1d28:
    bhs L_1d2c                              /* @1D28 00d2 bhs #0 -> 0x1D2C */
L_1d2a:
    ldrh r0, [r7, #26]                      /* @1D2A 788b ldrh r0, [r7, #26]  */
L_1d2c:
    movs r6, #75                            /* @1D2C 4b26 movs r6, #75  */
L_1d2e:
    lsls r6, r6, #4                         /* @1D2E 3601 lsls r6, r6, #4  */
L_1d30:
    subs r0, r6, r0                         /* @1D30 301a subs r0, r6, r0  */
L_1d32:
    uxth r4, r0                             /* @1D32 84b2 uxth r4, r0  */
L_1d34:
    ldr r0, [sp, #16]                       /* @1D34 0498 ldr r0, [sp, #16]  */
L_1d36:
    bl L_373a                               /* @1D36 01f000fd bl #6656 -> abs32_candidate */
L_1d3a:
    muls r0, r5, r0                         /* @1D3A 6843 muls r0, r5, r0  */
L_1d3c:
    adds r0, #255                           /* @1D3C ff30 adds r0, #255  */
L_1d3e:
    adds r0, #255                           /* @1D3E ff30 adds r0, #255  */
L_1d40:
    adds r0, #2                             /* @1D40 0230 adds r0, #2  */
L_1d42:
    lsls r0, r0, #6                         /* @1D42 8001 lsls r0, r0, #6  */
L_1d44:
    lsrs r0, r0, #16                        /* @1D44 000c lsrs r0, r0, #16  */
L_1d46:
    ldrh r1, [r7, #26]                      /* @1D46 798b ldrh r1, [r7, #26]  */
L_1d48:
    cmp r1, r0                              /* @1D48 8142 cmp r1, r0  */
L_1d4a:
    bhs L_1d4e                              /* @1D4A 00d2 bhs #0 -> 0x1D4E */
L_1d4c:
    ldrh r0, [r7, #26]                      /* @1D4C 788b ldrh r0, [r7, #26]  */
L_1d4e:
    subs r0, r6, r0                         /* @1D4E 301a subs r0, r6, r0  */
L_1d50:
    uxth r0, r0                             /* @1D50 80b2 uxth r0, r0  */
L_1d52:
    ldr r1, [pc, #144]                      /* @1D52 2449 ldr r1, [pc, #144] literal 0x1DE4 -> 0x2000000C */
L_1d54:
    ldr r1, [r1]                            /* @1D54 0968 ldr r1, [r1]  */
L_1d56:
    cmp r1, #1                              /* @1D56 0129 cmp r1, #1  */
L_1d58:
    bne L_1d6a                              /* @1D58 07d1 bne #14 -> 0x1D6A */
L_1d5a:
    movs r1, #75                            /* @1D5A 4b21 movs r1, #75  */
L_1d5c:
    lsls r1, r1, #3                         /* @1D5C c900 lsls r1, r1, #3  */
L_1d5e:
    cmp r4, r1                              /* @1D5E 8c42 cmp r4, r1  */
L_1d60:
    bhs L_1d64                              /* @1D60 00d2 bhs #0 -> 0x1D64 */
L_1d62:
    mov r4, r1                              /* @1D62 0c46 mov r4, r1  */
L_1d64:
    cmp r0, r1                              /* @1D64 8842 cmp r0, r1  */
L_1d66:
    bhs L_1d6a                              /* @1D66 00d2 bhs #0 -> 0x1D6A */
L_1d68:
    mov r0, r1                              /* @1D68 0846 mov r0, r1  */
L_1d6a:
    ldr r3, [sp, #12]                       /* @1D6A 039b ldr r3, [sp, #12]  */
L_1d6c:
    ldr r2, [pc, #120]                      /* @1D6C 1e4a ldr r2, [pc, #120] literal 0x1DE8 -> 0x40000C40 */
L_1d6e:
    ldr r1, [pc, #120]                      /* @1D6E 1e49 ldr r1, [pc, #120] literal 0x1DE8 -> 0x40000C40 */
L_1d70:
    subs r2, #64                            /* @1D70 403a subs r2, #64  */
L_1d72:
    cmp r3, #0                              /* @1D72 002b cmp r3, #0  */
L_1d74:
    blt L_1d8c                              /* @1D74 0adb blt #20 -> 0x1D8C */
L_1d76:
    ldr r3, [r1]                            /* @1D76 0b68 ldr r3, [r1]  */
L_1d78:
    lsrs r3, r3, #16                        /* @1D78 1b0c lsrs r3, r3, #16  */
L_1d7a:
    lsls r3, r3, #16                        /* @1D7A 1b04 lsls r3, r3, #16  */
L_1d7c:
    adds r3, r3, r6                         /* @1D7C 9b19 adds r3, r3, r6  */
L_1d7e:
    str r3, [r1]                            /* @1D7E 0b60 str r3, [r1]  */
L_1d80:
    ldr r1, [r2, #60]                       /* @1D80 d16b ldr r1, [r2, #60]  */
L_1d82:
    lsrs r1, r1, #16                        /* @1D82 090c lsrs r1, r1, #16  */
L_1d84:
    lsls r1, r1, #16                        /* @1D84 0904 lsls r1, r1, #16  */
L_1d86:
    orrs r1, r4                             /* @1D86 2143 orrs r1, r4  */
L_1d88:
    str r1, [r2, #60]                       /* @1D88 d163 str r1, [r2, #60]  */
L_1d8a:
    b L_1da0                                /* @1D8A 09e0 b #18 -> 0x1DA0 */
L_1d8c:
    ldr r3, [r2, #60]                       /* @1D8C d36b ldr r3, [r2, #60]  */
L_1d8e:
    lsrs r3, r3, #16                        /* @1D8E 1b0c lsrs r3, r3, #16  */
L_1d90:
    lsls r3, r3, #16                        /* @1D90 1b04 lsls r3, r3, #16  */
L_1d92:
    adds r3, r3, r6                         /* @1D92 9b19 adds r3, r3, r6  */
L_1d94:
    str r3, [r2, #60]                       /* @1D94 d363 str r3, [r2, #60]  */
L_1d96:
    ldr r2, [r1]                            /* @1D96 0a68 ldr r2, [r1]  */
L_1d98:
    lsrs r2, r2, #16                        /* @1D98 120c lsrs r2, r2, #16  */
L_1d9a:
    lsls r2, r2, #16                        /* @1D9A 1204 lsls r2, r2, #16  */
L_1d9c:
    orrs r2, r4                             /* @1D9C 2243 orrs r2, r4  */
L_1d9e:
    str r2, [r1]                            /* @1D9E 0a60 str r2, [r1]  */
L_1da0:
    ldr r3, [sp, #16]                       /* @1DA0 049b ldr r3, [sp, #16]  */
L_1da2:
    ldr r1, [pc, #68]                       /* @1DA2 1149 ldr r1, [pc, #68] literal 0x1DE8 -> 0x40000C40 */
L_1da4:
    ldr r2, [pc, #68]                       /* @1DA4 114a ldr r2, [pc, #68] literal 0x1DEC -> 0x40000D40 */
L_1da6:
    adds r1, #192                           /* @1DA6 c031 adds r1, #192  */
L_1da8:
    cmp r3, #0                              /* @1DA8 002b cmp r3, #0  */
L_1daa:
    blt L_1dc4                              /* @1DAA 0bdb blt #22 -> 0x1DC4 */
L_1dac:
    ldr r3, [r1, #60]                       /* @1DAC cb6b ldr r3, [r1, #60]  */
L_1dae:
    lsrs r3, r3, #16                        /* @1DAE 1b0c lsrs r3, r3, #16  */
L_1db0:
    lsls r3, r3, #16                        /* @1DB0 1b04 lsls r3, r3, #16  */
L_1db2:
    adds r3, r3, r6                         /* @1DB2 9b19 adds r3, r3, r6  */
L_1db4:
    str r3, [r1, #60]                       /* @1DB4 cb63 str r3, [r1, #60]  */
L_1db6:
    ldr r1, [r2]                            /* @1DB6 1168 ldr r1, [r2]  */
L_1db8:
    lsrs r1, r1, #16                        /* @1DB8 090c lsrs r1, r1, #16  */
L_1dba:
    lsls r1, r1, #16                        /* @1DBA 0904 lsls r1, r1, #16  */
L_1dbc:
    orrs r1, r0                             /* @1DBC 0143 orrs r1, r0  */
L_1dbe:
    str r1, [r2]                            /* @1DBE 1160 str r1, [r2]  */
L_1dc0:
    add sp, #20                             /* @1DC0 05b0 add sp, #20  */
L_1dc2:
    pop {r4, r5, r6, r7, pc}                /* @1DC2 f0bd pop {r4, r5, r6, r7, pc}  */
L_1dc4:
    ldr r3, [r2]                            /* @1DC4 1368 ldr r3, [r2]  */
L_1dc6:
    lsrs r3, r3, #16                        /* @1DC6 1b0c lsrs r3, r3, #16  */
L_1dc8:
    lsls r3, r3, #16                        /* @1DC8 1b04 lsls r3, r3, #16  */
L_1dca:
    adds r3, r3, r6                         /* @1DCA 9b19 adds r3, r3, r6  */
L_1dcc:
    str r3, [r2]                            /* @1DCC 1360 str r3, [r2]  */
L_1dce:
    ldr r2, [r1, #60]                       /* @1DCE ca6b ldr r2, [r1, #60]  */
L_1dd0:
    lsrs r2, r2, #16                        /* @1DD0 120c lsrs r2, r2, #16  */
L_1dd2:
    lsls r2, r2, #16                        /* @1DD2 1204 lsls r2, r2, #16  */
L_1dd4:
    orrs r2, r0                             /* @1DD4 0243 orrs r2, r0  */
L_1dd6:
    str r2, [r1, #60]                       /* @1DD6 ca63 str r2, [r1, #60]  */
L_1dd8:
    b L_1dc0                                /* @1DD8 f2e7 b #-28 -> 0x1DC0 */
    .byte 0x00,0x00 /* @1DDA unclassified */
L_1ddc:
    .byte 0xc0,0x06,0x00,0x20 /* @1DDC literal */
L_1de0:
    .byte 0x2c,0x07,0x00,0x20 /* @1DE0 literal */
L_1de4:
    .byte 0x0c,0x00,0x00,0x20 /* @1DE4 literal */
L_1de8:
    .byte 0x40,0x0c,0x00,0x40 /* @1DE8 literal */
L_1dec:
    .byte 0x40,0x0d,0x00,0x40 /* @1DEC literal */

/* sub_1DF0 */
L_1df0:
    push {r3, r4, r5, r6, r7, lr}           /* @1DF0 f8b5 push {r3, r4, r5, r6, r7, lr}  */
L_1df2:
    ldr r5, [pc, #336]                      /* @1DF2 544d ldr r5, [pc, #336] literal 0x1F44 -> 0x20000648 */
L_1df4:
    ldrh r0, [r5, #8]                       /* @1DF4 2889 ldrh r0, [r5, #8]  */
L_1df6:
    cmp r0, #1                              /* @1DF6 0128 cmp r0, #1  */
L_1df8:
    beq L_1e02                              /* @1DF8 03d0 beq #6 -> 0x1E02 */
L_1dfa:
    bl L_5368                               /* @1DFA 03f0b5fa bl #13674 -> sub_5368 */
L_1dfe:
    bl L_2464                               /* @1DFE 00f031fb bl #1634 -> check_current_offsets */
L_1e02:
    ldrb r0, [r5]                           /* @1E02 2878 ldrb r0, [r5]  */
L_1e04:
    ldr r6, [pc, #320]                      /* @1E04 504e ldr r6, [pc, #320] literal 0x1F48 -> 0x20000004 */
L_1e06:
    cmp r0, #1                              /* @1E06 0128 cmp r0, #1  */
L_1e08:
    beq L_1e1e                              /* @1E08 09d0 beq #18 -> 0x1E1E */
L_1e0a:
    ldr r0, [r6]                            /* @1E0A 3068 ldr r0, [r6]  */
L_1e0c:
    ldrh r0, [r0, #54]                      /* @1E0C c08e ldrh r0, [r0, #54]  */
L_1e0e:
    cmp r0, #1                              /* @1E0E 0128 cmp r0, #1  */
L_1e10:
    beq L_1e1a                              /* @1E10 03d0 beq #6 -> 0x1E1A */
L_1e12:
    bl L_560c                               /* @1E12 03f0fbfb bl #14326 -> sub_560C */
L_1e16:
    cmp r0, #0                              /* @1E16 0028 cmp r0, #0  */
L_1e18:
    beq L_1e32                              /* @1E18 0bd0 beq #22 -> 0x1E32 */
L_1e1a:
    bl L_3a8c                               /* @1E1A 01f037fe bl #7278 -> sub_3A8C */
L_1e1e:
    ldr r4, [pc, #300]                      /* @1E1E 4b4c ldr r4, [pc, #300] literal 0x1F4C -> 0x2000072C */
L_1e20:
    ldrb r0, [r4, #9]                       /* @1E20 607a ldrb r0, [r4, #9]  */
L_1e22:
    cmp r0, #0                              /* @1E22 0028 cmp r0, #0  */
L_1e24:
    beq L_1e36                              /* @1E24 07d0 beq #14 -> 0x1E36 */
L_1e26:
    bl L_5030                               /* @1E26 03f003f9 bl #12806 -> sub_5030 */
L_1e2a:
    cmp r0, #0                              /* @1E2A 0028 cmp r0, #0  */
L_1e2c:
    beq L_1e36                              /* @1E2C 03d0 beq #6 -> 0x1E36 */
L_1e2e:
    movs r0, #3                             /* @1E2E 0320 movs r0, #3  */
L_1e30:
    pop {r3, r4, r5, r6, r7, pc}            /* @1E30 f8bd pop {r3, r4, r5, r6, r7, pc}  */
L_1e32:
    movs r0, #4                             /* @1E32 0420 movs r0, #4  */
L_1e34:
    pop {r3, r4, r5, r6, r7, pc}            /* @1E34 f8bd pop {r3, r4, r5, r6, r7, pc}  */
L_1e36:
    ldr r0, [r6]                            /* @1E36 3068 ldr r0, [r6]  */
L_1e38:
    ldr r6, [pc, #276]                      /* @1E38 454e ldr r6, [pc, #276] literal 0x1F50 -> 0x200005E4 */
L_1e3a:
    ldrh r0, [r0, #54]                      /* @1E3A c08e ldrh r0, [r0, #54]  */
L_1e3c:
    cmp r0, #1                              /* @1E3C 0128 cmp r0, #1  */
L_1e3e:
    beq L_1e94                              /* @1E3E 29d0 beq #82 -> 0x1E94 */
L_1e40:
    ldrb r0, [r4, #9]                       /* @1E40 607a ldrb r0, [r4, #9]  */
L_1e42:
    cmp r0, #0                              /* @1E42 0028 cmp r0, #0  */
L_1e44:
    beq L_1e8c                              /* @1E44 22d0 beq #68 -> 0x1E8C */
L_1e46:
    bl L_59c8                               /* @1E46 03f0bffd bl #15230 -> sub_59C8 */
L_1e4a:
    cmp r0, #0                              /* @1E4A 0028 cmp r0, #0  */
L_1e4c:
    bge L_1e50                              /* @1E4C 00da bge #0 -> 0x1E50 */
L_1e4e:
    rsbs r0, r0, #0                         /* @1E4E 4042 rsbs r0, r0, #0  */
L_1e50:
    sxth r0, r0                             /* @1E50 00b2 sxth r0, r0  */
L_1e52:
    cmp r0, #23                             /* @1E52 1728 cmp r0, #23  */
L_1e54:
    bge L_1e5a                              /* @1E54 01da bge #2 -> 0x1E5A */
L_1e56:
    movs r0, #1                             /* @1E56 0120 movs r0, #1  */
L_1e58:
    pop {r3, r4, r5, r6, r7, pc}            /* @1E58 f8bd pop {r3, r4, r5, r6, r7, pc}  */
L_1e5a:
    movs r7, #200                           /* @1E5A c827 movs r7, #200  */
L_1e5c:
    cmp r0, #60                             /* @1E5C 3c28 cmp r0, #60  */
L_1e5e:
    bge L_1e84                              /* @1E5E 11da bge #34 -> 0x1E84 */
L_1e60:
    bl L_59c8                               /* @1E60 03f0b2fd bl #15204 -> sub_59C8 */
L_1e64:
    cmp r0, #0                              /* @1E64 0028 cmp r0, #0  */
L_1e66:
    bge L_1e6a                              /* @1E66 00da bge #0 -> 0x1E6A */
L_1e68:
    rsbs r0, r0, #0                         /* @1E68 4042 rsbs r0, r0, #0  */
L_1e6a:
    sxth r0, r0                             /* @1E6A 00b2 sxth r0, r0  */
L_1e6c:
    cmp r0, #23                             /* @1E6C 1728 cmp r0, #23  */
L_1e6e:
    bge L_1e74                              /* @1E6E 01da bge #2 -> 0x1E74 */
L_1e70:
    movs r0, #1                             /* @1E70 0120 movs r0, #1  */
L_1e72:
    pop {r3, r4, r5, r6, r7, pc}            /* @1E72 f8bd pop {r3, r4, r5, r6, r7, pc}  */
L_1e74:
    cmp r0, #60                             /* @1E74 3c28 cmp r0, #60  */
L_1e76:
    bge L_1e80                              /* @1E76 03da bge #6 -> 0x1E80 */
L_1e78:
    movs r0, #255                           /* @1E78 ff20 movs r0, #255  */
L_1e7a:
    adds r0, #145                           /* @1E7A 9130 adds r0, #145  */
L_1e7c:
    strh r0, [r6, #4]                       /* @1E7C b080 strh r0, [r6, #4]  */
L_1e7e:
    b L_1e86                                /* @1E7E 02e0 b #4 -> 0x1E86 */
L_1e80:
    strh r7, [r6, #4]                       /* @1E80 b780 strh r7, [r6, #4]  */
L_1e82:
    b L_1e86                                /* @1E82 00e0 b #0 -> 0x1E86 */
L_1e84:
    strh r7, [r6, #4]                       /* @1E84 b780 strh r7, [r6, #4]  */
L_1e86:
    ldrh r0, [r6, #4]                       /* @1E86 b088 ldrh r0, [r6, #4]  */
L_1e88:
    lsrs r0, r0, #3                         /* @1E88 c008 lsrs r0, r0, #3  */
L_1e8a:
    strh r0, [r6, #4]                       /* @1E8A b080 strh r0, [r6, #4]  */
L_1e8c:
    bl L_52cc                               /* @1E8C 03f01efa bl #13372 -> sub_52CC */
L_1e90:
    bl L_470c                               /* @1E90 02f03cfc bl #10360 -> sub_470C */
L_1e94:
    ldrb r0, [r4, #9]                       /* @1E94 607a ldrb r0, [r4, #9]  */
L_1e96:
    cmp r0, #0                              /* @1E96 0028 cmp r0, #0  */
L_1e98:
    beq L_1ea2                              /* @1E98 03d0 beq #6 -> 0x1EA2 */
L_1e9a:
    bl L_267c                               /* @1E9A 00f0effb bl #2014 -> load_calibration_knots */
L_1e9e:
    cmp r0, #0                              /* @1E9E 0028 cmp r0, #0  */
L_1ea0:
    beq L_1ef0                              /* @1EA0 26d0 beq #76 -> 0x1EF0 */
L_1ea2:
    ldrh r1, [r6, #4]                       /* @1EA2 b188 ldrh r1, [r6, #4]  */
L_1ea4:
    movs r7, #1                             /* @1EA4 0127 movs r7, #1  */
L_1ea6:
    lsls r7, r7, #13                        /* @1EA6 7f03 lsls r7, r7, #13  */
L_1ea8:
    mov r0, r7                              /* @1EA8 3846 mov r0, r7  */
L_1eaa:
    bl L_0110                               /* @1EAA fef731f9 bl #-7582 -> udivmod32 */
L_1eae:
    str r0, [r5, #16]                       /* @1EAE 2861 str r0, [r5, #16]  */
L_1eb0:
    ldr r0, [r5, #16]                       /* @1EB0 2869 ldr r0, [r5, #16]  */
L_1eb2:
    rsbs r0, r0, #0                         /* @1EB2 4042 rsbs r0, r0, #0  */
L_1eb4:
    str r0, [r5, #20]                       /* @1EB4 6861 str r0, [r5, #20]  */
L_1eb6:
    ldrh r1, [r4]                           /* @1EB6 2188 ldrh r1, [r4]  */
L_1eb8:
    ldrh r0, [r4]                           /* @1EB8 2088 ldrh r0, [r4]  */
L_1eba:
    lsrs r0, r0, #1                         /* @1EBA 4008 lsrs r0, r0, #1  */
L_1ebc:
    adds r0, r0, r7                         /* @1EBC c019 adds r0, r0, r7  */
L_1ebe:
    bl L_0110                               /* @1EBE fef727f9 bl #-7602 -> udivmod32 */
L_1ec2:
    str r0, [r5, #12]                       /* @1EC2 e860 str r0, [r5, #12]  */
L_1ec4:
    ldrh r1, [r6, #4]                       /* @1EC4 b188 ldrh r1, [r6, #4]  */
L_1ec6:
    ldrh r0, [r4]                           /* @1EC6 2088 ldrh r0, [r4]  */
L_1ec8:
    muls r1, r0, r1                         /* @1EC8 4143 muls r1, r0, r1  */
L_1eca:
    ldrh r0, [r6, #4]                       /* @1ECA b088 ldrh r0, [r6, #4]  */
L_1ecc:
    ldrh r2, [r4]                           /* @1ECC 2288 ldrh r2, [r4]  */
L_1ece:
    muls r0, r2, r0                         /* @1ECE 5043 muls r0, r2, r0  */
L_1ed0:
    asrs r0, r0, #1                         /* @1ED0 4010 asrs r0, r0, #1  */
L_1ed2:
    lsls r2, r7, #8                         /* @1ED2 3a02 lsls r2, r7, #8  */
L_1ed4:
    adds r0, r0, r2                         /* @1ED4 8018 adds r0, r0, r2  */
L_1ed6:
    bl L_0110                               /* @1ED6 fef71bf9 bl #-7626 -> udivmod32 */
L_1eda:
    str r0, [r5, #24]                       /* @1EDA a861 str r0, [r5, #24]  */
L_1edc:
    ldrh r0, [r4, #2]                       /* @1EDC 6088 ldrh r0, [r4, #2]  */
L_1ede:
    ldr r1, [pc, #116]                      /* @1EDE 1d49 ldr r1, [pc, #116] literal 0x1F54 -> 0x20000145 */
L_1ee0:
    bl L_3052                               /* @1EE0 01f0b7f8 bl #4462 -> sub_3052 */
L_1ee4:
    bl L_20e4                               /* @1EE4 00f0fef8 bl #508 -> sub_20E4 */
L_1ee8:
    ldrb r0, [r4, #9]                       /* @1EE8 607a ldrb r0, [r4, #9]  */
L_1eea:
    cmp r0, #2                              /* @1EEA 0228 cmp r0, #2  */
L_1eec:
    beq L_1ef4                              /* @1EEC 02d0 beq #4 -> 0x1EF4 */
L_1eee:
    b L_1f40                                /* @1EEE 27e0 b #78 -> 0x1F40 */
L_1ef0:
    movs r0, #2                             /* @1EF0 0220 movs r0, #2  */
L_1ef2:
    pop {r3, r4, r5, r6, r7, pc}            /* @1EF2 f8bd pop {r3, r4, r5, r6, r7, pc}  */
L_1ef4:
    ldrh r0, [r4, #22]                      /* @1EF4 e08a ldrh r0, [r4, #22]  */
L_1ef6:
    ldr r1, [pc, #96]                       /* @1EF6 1849 ldr r1, [pc, #96] literal 0x1F58 -> 0x0000FFFF */
L_1ef8:
    cmp r0, r1                              /* @1EF8 8842 cmp r0, r1  */
L_1efa:
    beq L_1f40                              /* @1EFA 21d0 beq #66 -> 0x1F40 */
L_1efc:
    ldr r1, [pc, #92]                       /* @1EFC 1749 ldr r1, [pc, #92] literal 0x1F5C -> 0x2000061C */
L_1efe:
    movs r0, #0                             /* @1EFE 0020 movs r0, #0  */
L_1f00:
    str r0, [r1]                            /* @1F00 0860 str r0, [r1]  */
L_1f02:
    ldr r1, [pc, #92]                       /* @1F02 1749 ldr r1, [pc, #92] literal 0x1F60 -> 0x20000013 */
L_1f04:
    movs r0, #1                             /* @1F04 0120 movs r0, #1  */
L_1f06:
    strb r0, [r1]                           /* @1F06 0870 strb r0, [r1]  */
L_1f08:
    ldr r0, [pc, #88]                       /* @1F08 1648 ldr r0, [pc, #88] literal 0x1F64 -> 0x20000610 */
L_1f0a:
    movs r1, #0                             /* @1F0A 0021 movs r1, #0  */
L_1f0c:
    str r1, [r0]                            /* @1F0C 0160 str r1, [r0]  */
L_1f0e:
    str r1, [r0, #4]                        /* @1F0E 4160 str r1, [r0, #4]  */
L_1f10:
    ldrh r0, [r4, #22]                      /* @1F10 e08a ldrh r0, [r4, #22]  */
L_1f12:
    ldr r1, [pc, #84]                       /* @1F12 1549 ldr r1, [pc, #84] literal 0x1F68 -> 0x20000608 */
L_1f14:
    lsrs r0, r0, #15                        /* @1F14 c00b lsrs r0, r0, #15  */
L_1f16:
    strb r0, [r1]                           /* @1F16 0870 strb r0, [r1]  */
L_1f18:
    ldrh r0, [r4, #22]                      /* @1F18 e08a ldrh r0, [r4, #22]  */
L_1f1a:
    ldr r6, [pc, #80]                       /* @1F1A 144e ldr r6, [pc, #80] literal 0x1F6C -> 0x20000618 */
L_1f1c:
    lsls r0, r0, #17                        /* @1F1C 4004 lsls r0, r0, #17  */
L_1f1e:
    lsrs r0, r0, #17                        /* @1F1E 400c lsrs r0, r0, #17  */
L_1f20:
    strh r0, [r6]                           /* @1F20 3080 strh r0, [r6]  */
L_1f22:
    ldrh r1, [r4]                           /* @1F22 2188 ldrh r1, [r4]  */
L_1f24:
    ldrh r0, [r6]                           /* @1F24 3088 ldrh r0, [r6]  */
L_1f26:
    bl L_0110                               /* @1F26 fef7f3f8 bl #-7706 -> udivmod32 */
L_1f2a:
    strh r0, [r6, #2]                       /* @1F2A 7080 strh r0, [r6, #2]  */
L_1f2c:
    ldr r1, [pc, #64]                       /* @1F2C 1049 ldr r1, [pc, #64] literal 0x1F70 -> 0x20000010 */
L_1f2e:
    ldrh r0, [r5, #2]                       /* @1F2E 6888 ldrh r0, [r5, #2]  */
L_1f30:
    strh r0, [r1]                           /* @1F30 0880 strh r0, [r1]  */
L_1f32:
    ldr r1, [pc, #64]                       /* @1F32 1049 ldr r1, [pc, #64] literal 0x1F74 -> 0x0000FE89 */
L_1f34:
    movs r0, #2                             /* @1F34 0220 movs r0, #2  */
L_1f36:
    bl L_0908                               /* @1F36 fef7e7fc bl #-5682 -> sub_0908 */
L_1f3a:
    movs r0, #2                             /* @1F3A 0220 movs r0, #2  */
L_1f3c:
    bl L_0924                               /* @1F3C fef7f2fc bl #-5660 -> sub_0924 */
L_1f40:
    movs r0, #0                             /* @1F40 0020 movs r0, #0  */
L_1f42:
    pop {r3, r4, r5, r6, r7, pc}            /* @1F42 f8bd pop {r3, r4, r5, r6, r7, pc}  */
L_1f44:
    .byte 0x48,0x06,0x00,0x20 /* @1F44 literal */
L_1f48:
    .byte 0x04,0x00,0x00,0x20 /* @1F48 literal */
L_1f4c:
    .byte 0x2c,0x07,0x00,0x20 /* @1F4C literal */
L_1f50:
    .byte 0xe4,0x05,0x00,0x20 /* @1F50 literal */
L_1f54:
    .byte 0x45,0x01,0x00,0x20 /* @1F54 literal */
L_1f58:
    .byte 0xff,0xff,0x00,0x00 /* @1F58 literal */
L_1f5c:
    .byte 0x1c,0x06,0x00,0x20 /* @1F5C literal */
L_1f60:
    .byte 0x13,0x00,0x00,0x20 /* @1F60 literal */
L_1f64:
    .byte 0x10,0x06,0x00,0x20 /* @1F64 literal */
L_1f68:
    .byte 0x08,0x06,0x00,0x20 /* @1F68 literal */
L_1f6c:
    .byte 0x18,0x06,0x00,0x20 /* @1F6C literal */
L_1f70:
    .byte 0x10,0x00,0x00,0x20 /* @1F70 literal */
L_1f74:
    .byte 0x89,0xfe,0x00,0x00 /* @1F74 literal */

/* sub_1F78 */
L_1f78:
    push {r4, lr}                           /* @1F78 10b5 push {r4, lr}  */
L_1f7a:
    ldr r0, [pc, #44]                       /* @1F7A 0b48 ldr r0, [pc, #44] literal 0x1FA8 -> 0x20000012 */
L_1f7c:
    movs r4, #0                             /* @1F7C 0024 movs r4, #0  */
L_1f7e:
    strb r4, [r0, #3]                       /* @1F7E c470 strb r4, [r0, #3]  */
L_1f80:
    movs r0, #0                             /* @1F80 0020 movs r0, #0  */
L_1f82:
    bl L_44a8                               /* @1F82 02f091fa bl #9506 -> menu_set_0 */
L_1f86:
    ldr r1, [pc, #36]                       /* @1F86 0949 ldr r1, [pc, #36] literal 0x1FAC -> 0x200006A8 */
L_1f88:
    cmp r0, #1                              /* @1F88 0128 cmp r0, #1  */
L_1f8a:
    beq L_1fa2                              /* @1F8A 0ad0 beq #20 -> 0x1FA2 */
L_1f8c:
    strb r4, [r1, #13]                      /* @1F8C 4c73 strb r4, [r1, #13]  */
L_1f8e:
    movs r0, #2                             /* @1F8E 0220 movs r0, #2  */
L_1f90:
    strb r0, [r1, #2]                       /* @1F90 8870 strb r0, [r1, #2]  */
L_1f92:
    ldr r0, [pc, #28]                       /* @1F92 0748 ldr r0, [pc, #28] literal 0x1FB0 -> 0x2000072C */
L_1f94:
    ldrh r0, [r0, #20]                      /* @1F94 808a ldrh r0, [r0, #20]  */
L_1f96:
    ldrb r2, [r1, #13]                      /* @1F96 4a7b ldrb r2, [r1, #13]  */
L_1f98:
    adds r0, r0, r2                         /* @1F98 8018 adds r0, r0, r2  */
L_1f9a:
    strb r0, [r1, #12]                      /* @1F9A 0873 strb r0, [r1, #12]  */
L_1f9c:
    bl L_2334                               /* @1F9C 00f0caf9 bl #916 -> uart_start_reply */
L_1fa0:
    pop {r4, pc}                            /* @1FA0 10bd pop {r4, pc}  */
L_1fa2:
    movs r0, #1                             /* @1FA2 0120 movs r0, #1  */
L_1fa4:
    strb r0, [r1, #13]                      /* @1FA4 4873 strb r0, [r1, #13]  */
L_1fa6:
    b L_1f8e                                /* @1FA6 f2e7 b #-28 -> 0x1F8E */
L_1fa8:
    .byte 0x12,0x00,0x00,0x20 /* @1FA8 literal */
L_1fac:
    .byte 0xa8,0x06,0x00,0x20 /* @1FAC literal */
L_1fb0:
    .byte 0x2c,0x07,0x00,0x20 /* @1FB0 literal */
    .byte 0x00,0x00,0x00,0x00 /* @1FB4 unclassified */

/* sub_1FB8 */
L_1fb8:
    mov r1, r0                              /* @1FB8 0146 mov r1, r0  */
L_1fba:
    movs r0, #0                             /* @1FBA 0020 movs r0, #0  */
L_1fbc:
    movs r2, #1                             /* @1FBC 0122 movs r2, #1  */
L_1fbe:
    lsls r2, r1                             /* @1FBE 8a40 lsls r2, r1  */
L_1fc0:
    mvns r1, r2                             /* @1FC0 d143 mvns r1, r2  */
L_1fc2:
    ldr r2, [pc, #4]                        /* @1FC2 014a ldr r2, [pc, #4] literal 0x1FC8 -> 0x40005800 */
L_1fc4:
    str r1, [r2, #20]                       /* @1FC4 5161 str r1, [r2, #20]  */
L_1fc6:
    bx lr                                   /* @1FC6 7047 bx lr  */
L_1fc8:
    .byte 0x00,0x58,0x00,0x40 /* @1FC8 literal */

/* sub_1FCC */
L_1fcc:
    mov r1, r0                              /* @1FCC 0146 mov r1, r0  */
L_1fce:
    movs r0, #0                             /* @1FCE 0020 movs r0, #0  */
L_1fd0:
    ldr r3, [pc, #12]                       /* @1FD0 034b ldr r3, [pc, #12] literal 0x1FE0 -> 0x40005800 */
L_1fd2:
    ldr r2, [r3]                            /* @1FD2 1a68 ldr r2, [r3]  */
L_1fd4:
    lsrs r2, r2, #16                        /* @1FD4 120c lsrs r2, r2, #16  */
L_1fd6:
    lsls r2, r2, #16                        /* @1FD6 1204 lsls r2, r2, #16  */
L_1fd8:
    orrs r2, r1                             /* @1FD8 0a43 orrs r2, r1  */
L_1fda:
    str r2, [r3]                            /* @1FDA 1a60 str r2, [r3]  */
L_1fdc:
    bx lr                                   /* @1FDC 7047 bx lr  */
    .byte 0x00,0x00 /* @1FDE unclassified */
L_1fe0:
    .byte 0x00,0x58,0x00,0x40 /* @1FE0 literal */

/* sub_1FE4 */
L_1fe4:
    mov r1, r0                              /* @1FE4 0146 mov r1, r0  */
L_1fe6:
    movs r0, #0                             /* @1FE6 0020 movs r0, #0  */
L_1fe8:
    ldr r3, [pc, #12]                       /* @1FE8 034b ldr r3, [pc, #12] literal 0x1FF8 -> 0x40005800 */
L_1fea:
    ldr r2, [r3, #4]                        /* @1FEA 5a68 ldr r2, [r3, #4]  */
L_1fec:
    lsrs r2, r2, #16                        /* @1FEC 120c lsrs r2, r2, #16  */
L_1fee:
    lsls r2, r2, #16                        /* @1FEE 1204 lsls r2, r2, #16  */
L_1ff0:
    orrs r2, r1                             /* @1FF0 0a43 orrs r2, r1  */
L_1ff2:
    str r2, [r3, #4]                        /* @1FF2 5a60 str r2, [r3, #4]  */
L_1ff4:
    bx lr                                   /* @1FF4 7047 bx lr  */
    .byte 0x00,0x00 /* @1FF6 unclassified */
L_1ff8:
    .byte 0x00,0x58,0x00,0x40 /* @1FF8 literal */

/* sub_1FFC */
L_1ffc:
    movs r0, #0                             /* @1FFC 0020 movs r0, #0  */
L_1ffe:
    ldr r1, [pc, #12]                       /* @1FFE 0349 ldr r1, [pc, #12] literal 0x200C -> 0x40005800 */
L_2000:
    ldr r2, [r1, #12]                       /* @2000 ca68 ldr r2, [r1, #12]  */
L_2002:
    movs r3, #1                             /* @2002 0123 movs r3, #1  */
L_2004:
    orrs r2, r3                             /* @2004 1a43 orrs r2, r3  */
L_2006:
    str r2, [r1, #12]                       /* @2006 ca60 str r2, [r1, #12]  */
L_2008:
    bx lr                                   /* @2008 7047 bx lr  */
    .byte 0x00,0x00 /* @200A unclassified */
L_200c:
    .byte 0x00,0x58,0x00,0x40 /* @200C literal */

/* sub_2010 */
L_2010:
    push {r4, lr}                           /* @2010 10b5 push {r4, lr}  */
L_2012:
    movs r2, #0                             /* @2012 0022 movs r2, #0  */
L_2014:
    ldr r1, [pc, #116]                      /* @2014 1d49 ldr r1, [pc, #116] literal 0x208C -> 0x40005800 */
L_2016:
    ldr r3, [r1, #12]                       /* @2016 cb68 ldr r3, [r1, #12]  */
L_2018:
    movs r4, #3                             /* @2018 0324 movs r4, #3  */
L_201a:
    lsls r4, r4, #12                        /* @201A 2403 lsls r4, r4, #12  */
L_201c:
    bics r3, r4                             /* @201C a343 bics r3, r4  */
L_201e:
    ldrb r4, [r0]                           /* @201E 0478 ldrb r4, [r0]  */
L_2020:
    lsls r4, r4, #30                        /* @2020 a407 lsls r4, r4, #30  */
L_2022:
    lsrs r4, r4, #18                        /* @2022 a40c lsrs r4, r4, #18  */
L_2024:
    orrs r3, r4                             /* @2024 2343 orrs r3, r4  */
L_2026:
    str r3, [r1, #12]                       /* @2026 cb60 str r3, [r1, #12]  */
L_2028:
    ldr r3, [r1, #12]                       /* @2028 cb68 ldr r3, [r1, #12]  */
L_202a:
    asrs r4, r1, #21                        /* @202A 4c15 asrs r4, r1, #21  */
L_202c:
    bics r3, r4                             /* @202C a343 bics r3, r4  */
L_202e:
    ldrb r4, [r0, #1]                       /* @202E 4478 ldrb r4, [r0, #1]  */
L_2030:
    lsls r4, r4, #31                        /* @2030 e407 lsls r4, r4, #31  */
L_2032:
    lsrs r4, r4, #22                        /* @2032 a40d lsrs r4, r4, #22  */
L_2034:
    orrs r3, r4                             /* @2034 2343 orrs r3, r4  */
L_2036:
    str r3, [r1, #12]                       /* @2036 cb60 str r3, [r1, #12]  */
L_2038:
    ldr r3, [r1, #12]                       /* @2038 cb68 ldr r3, [r1, #12]  */
L_203a:
    asrs r4, r1, #22                        /* @203A 8c15 asrs r4, r1, #22  */
L_203c:
    bics r3, r4                             /* @203C a343 bics r3, r4  */
L_203e:
    ldrb r4, [r0, #2]                       /* @203E 8478 ldrb r4, [r0, #2]  */
L_2040:
    lsls r4, r4, #31                        /* @2040 e407 lsls r4, r4, #31  */
L_2042:
    lsrs r4, r4, #23                        /* @2042 e40d lsrs r4, r4, #23  */
L_2044:
    orrs r3, r4                             /* @2044 2343 orrs r3, r4  */
L_2046:
    str r3, [r1, #12]                       /* @2046 cb60 str r3, [r1, #12]  */
L_2048:
    ldr r3, [r1, #12]                       /* @2048 cb68 ldr r3, [r1, #12]  */
L_204a:
    movs r4, #112                           /* @204A 7024 movs r4, #112  */
L_204c:
    bics r3, r4                             /* @204C a343 bics r3, r4  */
L_204e:
    ldrb r4, [r0, #3]                       /* @204E c478 ldrb r4, [r0, #3]  */
L_2050:
    lsls r4, r4, #29                        /* @2050 6407 lsls r4, r4, #29  */
L_2052:
    lsrs r4, r4, #25                        /* @2052 640e lsrs r4, r4, #25  */
L_2054:
    orrs r3, r4                             /* @2054 2343 orrs r3, r4  */
L_2056:
    str r3, [r1, #12]                       /* @2056 cb60 str r3, [r1, #12]  */
L_2058:
    ldr r3, [r1, #12]                       /* @2058 cb68 ldr r3, [r1, #12]  */
L_205a:
    movs r4, #8                             /* @205A 0824 movs r4, #8  */
L_205c:
    bics r3, r4                             /* @205C a343 bics r3, r4  */
L_205e:
    ldrb r4, [r0, #4]                       /* @205E 0479 ldrb r4, [r0, #4]  */
L_2060:
    lsls r4, r4, #31                        /* @2060 e407 lsls r4, r4, #31  */
L_2062:
    lsrs r4, r4, #28                        /* @2062 240f lsrs r4, r4, #28  */
L_2064:
    orrs r3, r4                             /* @2064 2343 orrs r3, r4  */
L_2066:
    str r3, [r1, #12]                       /* @2066 cb60 str r3, [r1, #12]  */
L_2068:
    ldr r3, [r1, #12]                       /* @2068 cb68 ldr r3, [r1, #12]  */
L_206a:
    movs r4, #4                             /* @206A 0424 movs r4, #4  */
L_206c:
    bics r3, r4                             /* @206C a343 bics r3, r4  */
L_206e:
    ldrb r4, [r0, #5]                       /* @206E 4479 ldrb r4, [r0, #5]  */
L_2070:
    lsls r4, r4, #31                        /* @2070 e407 lsls r4, r4, #31  */
L_2072:
    lsrs r4, r4, #29                        /* @2072 640f lsrs r4, r4, #29  */
L_2074:
    orrs r3, r4                             /* @2074 2343 orrs r3, r4  */
L_2076:
    str r3, [r1, #12]                       /* @2076 cb60 str r3, [r1, #12]  */
L_2078:
    ldr r3, [r1, #12]                       /* @2078 cb68 ldr r3, [r1, #12]  */
L_207a:
    ldrb r0, [r0, #6]                       /* @207A 8079 ldrb r0, [r0, #6]  */
L_207c:
    movs r4, #2                             /* @207C 0224 movs r4, #2  */
L_207e:
    lsls r0, r0, #31                        /* @207E c007 lsls r0, r0, #31  */
L_2080:
    bics r3, r4                             /* @2080 a343 bics r3, r4  */
L_2082:
    lsrs r0, r0, #30                        /* @2082 800f lsrs r0, r0, #30  */
L_2084:
    orrs r3, r0                             /* @2084 0343 orrs r3, r0  */
L_2086:
    str r3, [r1, #12]                       /* @2086 cb60 str r3, [r1, #12]  */
L_2088:
    mov r0, r2                              /* @2088 1046 mov r0, r2  */
L_208a:
    pop {r4, pc}                            /* @208A 10bd pop {r4, pc}  */
L_208c:
    .byte 0x00,0x58,0x00,0x40 /* @208C literal */

/* sub_2090 */
L_2090:
    push {r4, r5, r6, lr}                   /* @2090 70b5 push {r4, r5, r6, lr}  */
L_2092:
    bl L_3e80                               /* @2092 01f0f5fe bl #7658 -> sub_3E80 */
L_2096:
    ldr r5, [pc, #68]                       /* @2096 114d ldr r5, [pc, #68] literal 0x20DC -> 0x40003000 */
L_2098:
    ldr r4, [pc, #68]                       /* @2098 114c ldr r4, [pc, #68] literal 0x20E0 -> 0x200005DC */
L_209a:
    cmp r0, #0                              /* @209A 0028 cmp r0, #0  */
L_209c:
    beq L_20ba                              /* @209C 0dd0 beq #26 -> 0x20BA */
L_209e:
    bl L_20e4                               /* @209E 00f021f8 bl #66 -> sub_20E4 */
L_20a2:
    bl L_34a4                               /* @20A2 01f0fff9 bl #5118 -> sub_34A4 */
L_20a6:
    bl L_3b9c                               /* @20A6 01f079fd bl #6898 -> sub_3B9C */
L_20aa:
    mov r0, r5                              /* @20AA 2846 mov r0, r5  */
L_20ac:
    bl L_0880                               /* @20AC fef7e8fb bl #-6192 -> sub_0880 */
L_20b0:
    bl L_5918                               /* @20B0 03f032fc bl #14436 -> sub_5918 */
L_20b4:
    movs r0, #1                             /* @20B4 0120 movs r0, #1  */
L_20b6:
    strb r0, [r4, #6]                       /* @20B6 a071 strb r0, [r4, #6]  */
L_20b8:
    pop {r4, r5, r6, pc}                    /* @20B8 70bd pop {r4, r5, r6, pc}  */
L_20ba:
    mov r0, r5                              /* @20BA 2846 mov r0, r5  */
L_20bc:
    bl L_088c                               /* @20BC fef7e6fb bl #-6196 -> sub_088C */
L_20c0:
    bl L_588c                               /* @20C0 03f0e4fb bl #14280 -> sub_588C */
L_20c4:
    bl L_3b70                               /* @20C4 01f054fd bl #6824 -> sub_3B70 */
L_20c8:
    bl L_3488                               /* @20C8 01f0def9 bl #5052 -> sub_3488 */
L_20cc:
    bl L_342c                               /* @20CC 01f0aef9 bl #4956 -> sub_342C */
L_20d0:
    movs r0, #0                             /* @20D0 0020 movs r0, #0  */
L_20d2:
    bl L_3f28                               /* @20D2 01f029ff bl #7762 -> sub_3F28 */
L_20d6:
    movs r0, #0                             /* @20D6 0020 movs r0, #0  */
L_20d8:
    strb r0, [r4, #6]                       /* @20D8 a071 strb r0, [r4, #6]  */
L_20da:
    pop {r4, r5, r6, pc}                    /* @20DA 70bd pop {r4, r5, r6, pc}  */
L_20dc:
    .byte 0x00,0x30,0x00,0x40 /* @20DC literal */
L_20e0:
    .byte 0xdc,0x05,0x00,0x20 /* @20E0 literal */

/* sub_20E4 */
L_20e4:
    push {r3, r4, r5, r6, r7, lr}           /* @20E4 f8b5 push {r3, r4, r5, r6, r7, lr}  */
L_20e6:
    bl L_3cac                               /* @20E6 01f0e1fd bl #7106 -> sub_3CAC */
L_20ea:
    ldr r4, [pc, #148]                      /* @20EA 254c ldr r4, [pc, #148] literal 0x2180 -> 0x20000680 */
L_20ec:
    movs r1, #0                             /* @20EC 0021 movs r1, #0  */
L_20ee:
    str r1, [r4]                            /* @20EE 2160 str r1, [r4]  */
L_20f0:
    str r1, [r4, #4]                        /* @20F0 6160 str r1, [r4, #4]  */
L_20f2:
    str r1, [r4, #16]                       /* @20F2 2161 str r1, [r4, #16]  */
L_20f4:
    str r1, [r4, #20]                       /* @20F4 6161 str r1, [r4, #20]  */
L_20f6:
    ldr r5, [pc, #140]                      /* @20F6 234d ldr r5, [pc, #140] literal 0x2184 -> 0x20000648 */
L_20f8:
    movs r7, #0                             /* @20F8 0027 movs r7, #0  */
L_20fa:
    str r7, [r5, #48]                       /* @20FA 2f63 str r7, [r5, #48]  */
L_20fc:
    str r7, [r5, #44]                       /* @20FC ef62 str r7, [r5, #44]  */
L_20fe:
    strh r7, [r5, #2]                       /* @20FE 6f80 strh r7, [r5, #2]  */
L_2100:
    ldr r0, [pc, #132]                      /* @2100 2148 ldr r0, [pc, #132] literal 0x2188 -> 0x40003000 */
L_2102:
    ldr r1, [r0]                            /* @2102 0168 ldr r1, [r0]  */
L_2104:
    lsrs r1, r1, #16                        /* @2104 090c lsrs r1, r1, #16  */
L_2106:
    lsls r1, r1, #16                        /* @2106 0904 lsls r1, r1, #16  */
L_2108:
    str r1, [r0]                            /* @2108 0160 str r1, [r0]  */
L_210a:
    ldr r0, [pc, #128]                      /* @210A 2048 ldr r0, [pc, #128] literal 0x218C -> 0x20000610 */
L_210c:
    movs r2, #0                             /* @210C 0022 movs r2, #0  */
L_210e:
    str r2, [r0]                            /* @210E 0260 str r2, [r0]  */
L_2110:
    str r2, [r0, #4]                        /* @2110 4260 str r2, [r0, #4]  */
L_2112:
    ldr r0, [pc, #124]                      /* @2112 1f48 ldr r0, [pc, #124] literal 0x2190 -> 0x2000061C */
L_2114:
    str r7, [r0]                            /* @2114 0760 str r7, [r0]  */
L_2116:
    ldr r0, [pc, #124]                      /* @2116 1f48 ldr r0, [pc, #124] literal 0x2194 -> 0x2000072C */
L_2118:
    ldrb r0, [r0, #9]                       /* @2118 407a ldrb r0, [r0, #9]  */
L_211a:
    ldr r6, [pc, #124]                      /* @211A 1f4e ldr r6, [pc, #124] literal 0x2198 -> 0x200005E4 */
L_211c:
    cmp r0, #0                              /* @211C 0028 cmp r0, #0  */
L_211e:
    beq L_215c                              /* @211E 1dd0 beq #58 -> 0x215C */
L_2120:
    ldr r0, [pc, #120]                      /* @2120 1e48 ldr r0, [pc, #120] literal 0x219C -> 0x200008C8 */
L_2122:
    ldrh r0, [r0, #22]                      /* @2122 c08a ldrh r0, [r0, #22]  */
L_2124:
    cmp r0, #1                              /* @2124 0128 cmp r0, #1  */
L_2126:
    bne L_215c                              /* @2126 19d1 bne #50 -> 0x215C */
L_2128:
    movs r0, #3                             /* @2128 0320 movs r0, #3  */
L_212a:
    bl L_5a38                               /* @212A 03f085fc bl #14602 -> sub_5A38 */
L_212e:
    bl L_26cc                               /* @212E 00f0cdfa bl #1434 -> encoder_lut_read */
L_2132:
    strh r0, [r5, #4]                       /* @2132 a880 strh r0, [r5, #4]  */
L_2134:
    ldrh r0, [r5, #4]                       /* @2134 a888 ldrh r0, [r5, #4]  */
L_2136:
    str r0, [r4, #16]                       /* @2136 2061 str r0, [r4, #16]  */
L_2138:
    str r7, [r4, #20]                       /* @2138 6761 str r7, [r4, #20]  */
L_213a:
    ldrh r0, [r5, #4]                       /* @213A a888 ldrh r0, [r5, #4]  */
L_213c:
    strh r0, [r5, #6]                       /* @213C e880 strh r0, [r5, #6]  */
L_213e:
    ldrh r0, [r5, #4]                       /* @213E a888 ldrh r0, [r5, #4]  */
L_2140:
    ldrh r1, [r6, #4]                       /* @2140 b188 ldrh r1, [r6, #4]  */
L_2142:
    muls r0, r1, r0                         /* @2142 4843 muls r0, r1, r0  */
L_2144:
    ldr r1, [r5, #12]                       /* @2144 e968 ldr r1, [r5, #12]  */
L_2146:
    asrs r1, r1, #1                         /* @2146 4910 asrs r1, r1, #1  */
L_2148:
    adds r0, r0, r1                         /* @2148 4018 adds r0, r0, r1  */
L_214a:
    ldr r1, [r5, #12]                       /* @214A e968 ldr r1, [r5, #12]  */
L_214c:
    bl L_013c                               /* @214C fdf7f6ff bl #-8212 -> sdivmod32 */
L_2150:
    asrs r1, r0, #31                        /* @2150 c117 asrs r1, r0, #31  */
L_2152:
    str r0, [r4]                            /* @2152 2060 str r0, [r4]  */
L_2154:
    str r1, [r4, #4]                        /* @2154 6160 str r1, [r4, #4]  */
L_2156:
    ldr r1, [pc, #72]                       /* @2156 1249 ldr r1, [pc, #72] literal 0x21A0 -> 0x200006C0 */
L_2158:
    movs r0, #1                             /* @2158 0120 movs r0, #1  */
L_215a:
    strb r0, [r1]                           /* @215A 0870 strb r0, [r1]  */
L_215c:
    ldr r0, [r4]                            /* @215C 2068 ldr r0, [r4]  */
L_215e:
    ldr r1, [r4, #4]                        /* @215E 6168 ldr r1, [r4, #4]  */
L_2160:
    ldr r2, [r5, #12]                       /* @2160 ea68 ldr r2, [r5, #12]  */
L_2162:
    asrs r3, r2, #31                        /* @2162 d317 asrs r3, r2, #31  */
L_2164:
    bl L_01b0                               /* @2164 fef724f8 bl #-8120 -> sub_01B0 */
L_2168:
    ldrh r2, [r6, #4]                       /* @2168 b288 ldrh r2, [r6, #4]  */
L_216a:
    bl L_3744                               /* @216A 01f0ebfa bl #5590 -> sub_3744 */
L_216e:
    str r0, [r4, #32]                       /* @216E 2062 str r0, [r4, #32]  */
L_2170:
    str r1, [r4, #36]                       /* @2170 6162 str r1, [r4, #36]  */
L_2172:
    ldr r0, [r4]                            /* @2172 2068 ldr r0, [r4]  */
L_2174:
    ldr r1, [r4, #4]                        /* @2174 6168 ldr r1, [r4, #4]  */
L_2176:
    str r0, [r5, #28]                       /* @2176 e861 str r0, [r5, #28]  */
L_2178:
    ldr r0, [r4, #16]                       /* @2178 2069 ldr r0, [r4, #16]  */
L_217a:
    ldr r1, [r4, #20]                       /* @217A 6169 ldr r1, [r4, #20]  */
L_217c:
    str r0, [r5, #32]                       /* @217C 2862 str r0, [r5, #32]  */
L_217e:
    pop {r3, r4, r5, r6, r7, pc}            /* @217E f8bd pop {r3, r4, r5, r6, r7, pc}  */
L_2180:
    .byte 0x80,0x06,0x00,0x20 /* @2180 literal */
L_2184:
    .byte 0x48,0x06,0x00,0x20 /* @2184 literal */
L_2188:
    .byte 0x00,0x30,0x00,0x40 /* @2188 literal */
L_218c:
    .byte 0x10,0x06,0x00,0x20 /* @218C literal */
L_2190:
    .byte 0x1c,0x06,0x00,0x20 /* @2190 literal */
L_2194:
    .byte 0x2c,0x07,0x00,0x20 /* @2194 literal */
L_2198:
    .byte 0xe4,0x05,0x00,0x20 /* @2198 literal */
L_219c:
    .byte 0xc8,0x08,0x00,0x20 /* @219C literal */
L_21a0:
    .byte 0xc0,0x06,0x00,0x20 /* @21A0 literal */
    .byte 0x20,0x00,0x00,0x00,0x17,0x00,0x00,0x00,0x1b,0x00,0x00,0x00,0x50,0x46,0x00,0x00 /* @21A4 unclassified */
    .byte 0xe0,0x22,0x02,0x00,0x18,0x00,0x00,0x00,0xf0,0x00,0x00,0x00,0xe8,0x03,0x00,0x00 /* @21B4 unclassified */
    .byte 0x00,0x80,0x00,0x96,0x00,0x09,0x3d,0x00,0x00,0x12,0x7a,0x00,0x00,0x24,0xf4,0x00 /* @21C4 unclassified */
    .byte 0x40,0x86,0x51,0x01,0x00,0x36,0x6e,0x01,0x00,0x00,0x00,0x00 /* @21D4 unclassified */

/* sub_21E0 */
L_21e0:
    push {r4, lr}                           /* @21E0 10b5 push {r4, lr}  */
L_21e2:
    movs r2, #1                             /* @21E2 0122 movs r2, #1  */
L_21e4:
    adds r0, r0, #4                         /* @21E4 001d adds r0, r0, #4  */
L_21e6:
    bl L_16ac                               /* @21E6 fff761fa bl #-2878 -> sub_16AC */
L_21ea:
    movs r0, #0                             /* @21EA 0020 movs r0, #0  */
L_21ec:
    pop {r4, pc}                            /* @21EC 10bd pop {r4, pc}  */
    .byte 0x00,0x00 /* @21EE unclassified */

/* sub_21F0 */
L_21f0:
    push {r2, r3, r4, lr}                   /* @21F0 1cb5 push {r2, r3, r4, lr}  */
L_21f2:
    mov r4, r0                              /* @21F2 0446 mov r4, r0  */
L_21f4:
    ldr r0, [pc, #84]                       /* @21F4 1548 ldr r0, [pc, #84] literal 0x224C -> 0x00000D64 */
L_21f6:
    ldmia r0, {r0, r2}                      /* @21F6 05c8 ldm r0, {r0, r2}  */
L_21f8:
    str r2, [sp, #4]                        /* @21F8 0192 str r2, [sp, #4]  */
L_21fa:
    str r0, [sp]                            /* @21FA 0090 str r0, [sp]  */
L_21fc:
    cmp r1, #0                              /* @21FC 0029 cmp r1, #0  */
L_21fe:
    beq L_2220                              /* @21FE 0fd0 beq #30 -> 0x2220 */
L_2200:
    movs r0, #0                             /* @2200 0020 movs r0, #0  */
L_2202:
    str r0, [r4, #4]                        /* @2202 6060 str r0, [r4, #4]  */
L_2204:
    ldrh r0, [r1, #2]                       /* @2204 4888 ldrh r0, [r1, #2]  */
L_2206:
    ldrb r2, [r1, #1]                       /* @2206 4a78 ldrb r2, [r1, #1]  */
L_2208:
    ldrb r3, [r1]                           /* @2208 0b78 ldrb r3, [r1]  */
L_220a:
    orrs r0, r2                             /* @220A 1043 orrs r0, r2  */
L_220c:
    ldrh r2, [r1, #4]                       /* @220C 8a88 ldrh r2, [r1, #4]  */
L_220e:
    orrs r2, r3                             /* @220E 1a43 orrs r2, r3  */
L_2210:
    orrs r0, r2                             /* @2210 1043 orrs r0, r2  */
L_2212:
    str r0, [r4, #4]                        /* @2212 6060 str r0, [r4, #4]  */
L_2214:
    ldrb r0, [r1]                           /* @2214 0878 ldrb r0, [r1]  */
L_2216:
    cmp r0, #64                             /* @2216 4028 cmp r0, #64  */
L_2218:
    beq L_2224                              /* @2218 04d0 beq #8 -> 0x2224 */
L_221a:
    cmp r0, #192                            /* @221A c028 cmp r0, #192  */
L_221c:
    beq L_2224                              /* @221C 02d0 beq #4 -> 0x2224 */
L_221e:
    b L_2246                                /* @221E 12e0 b #36 -> 0x2246 */
L_2220:
    movs r0, #4                             /* @2220 0420 movs r0, #4  */
L_2222:
    pop {r2, r3, r4, pc}                    /* @2222 1cbd pop {r2, r3, r4, pc}  */
L_2224:
    ldrh r0, [r1, #4]                       /* @2224 8888 ldrh r0, [r1, #4]  */
L_2226:
    mov r2, sp                              /* @2226 6a46 mov r2, sp  */
L_2228:
    lsrs r0, r0, #9                         /* @2228 400a lsrs r0, r0, #9  */
L_222a:
    lsls r0, r0, #2                         /* @222A 8000 lsls r0, r0, #2  */
L_222c:
    ldrb r2, [r2, r0]                       /* @222C 125c ldrb r2, [r2, r0]  */
L_222e:
    ldr r0, [r1, #12]                       /* @222E c868 ldr r0, [r1, #12]  */
L_2230:
    lsls r0, r2                             /* @2230 9040 lsls r0, r2  */
L_2232:
    ldr r2, [r1, #8]                        /* @2232 8a68 ldr r2, [r1, #8]  */
L_2234:
    mov r1, r0                              /* @2234 0146 mov r1, r0  */
L_2236:
    mov r0, r2                              /* @2236 1046 mov r0, r2  */
L_2238:
    bl L_0110                               /* @2238 fdf76aff bl #-8492 -> udivmod32 */
L_223c:
    str r0, [r4, #24]                       /* @223C a061 str r0, [r4, #24]  */
L_223e:
    movs r1, #4                             /* @223E 0421 movs r1, #4  */
L_2240:
    mov r0, r4                              /* @2240 2046 mov r0, r4  */
L_2242:
    bl L_21e0                               /* @2242 fff7cdff bl #-102 -> sub_21E0 */
L_2246:
    movs r0, #0                             /* @2246 0020 movs r0, #0  */
L_2248:
    pop {r2, r3, r4, pc}                    /* @2248 1cbd pop {r2, r3, r4, pc}  */
    .byte 0x00,0x00 /* @224A unclassified */
L_224c:
    .byte 0x64,0x0d,0x00,0x00 /* @224C literal */
    .byte 0x14,0x78,0x00,0x00,0x00,0x00,0x00,0x20,0x28,0x06,0x00,0x00,0x5a,0x03,0x00,0x00 /* @2250 scatter_records */
    .byte 0x44,0x7a,0x00,0x00,0x28,0x06,0x00,0x20,0x2c,0x04,0x00,0x00,0x9a,0x16,0x00,0x00 /* @2260 scatter_records */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @2270 unclassified */

/* sub_2280 */
L_2280:
    push {r4, r5, r6, r7, lr}               /* @2280 f0b5 push {r4, r5, r6, r7, lr}  */
L_2282:
    ldr r4, [pc, #128]                      /* @2282 204c ldr r4, [pc, #128] literal 0x2304 -> 0x40002000 */
L_2284:
    ldr r5, [r4, #32]                       /* @2284 256a ldr r5, [r4, #32]  */
L_2286:
    ldr r0, [r4, #32]                       /* @2286 206a ldr r0, [r4, #32]  */
L_2288:
    lsls r1, r4, #15                        /* @2288 e103 lsls r1, r4, #15  */
L_228a:
    orrs r0, r1                             /* @228A 0843 orrs r0, r1  */
L_228c:
    str r0, [r4, #32]                       /* @228C 2062 str r0, [r4, #32]  */
L_228e:
    ldr r0, [pc, #120]                      /* @228E 1e48 ldr r0, [pc, #120] literal 0x2308 -> 0x40020D00 */
L_2290:
    ldr r1, [r0, #12]                       /* @2290 c168 ldr r1, [r0, #12]  */
L_2292:
    ldr r2, [pc, #120]                      /* @2292 1e4a ldr r2, [pc, #120] literal 0x230C -> 0x0000FFF4 */
L_2294:
    ands r1, r2                             /* @2294 1140 ands r1, r2  */
L_2296:
    str r1, [r0, #12]                       /* @2296 c160 str r1, [r0, #12]  */
L_2298:
    ldr r1, [pc, #108]                      /* @2298 1b49 ldr r1, [pc, #108] literal 0x2308 -> 0x40020D00 */
L_229a:
    adds r1, #64                            /* @229A 4031 adds r1, #64  */
L_229c:
    ldr r2, [r1, #12]                       /* @229C ca68 ldr r2, [r1, #12]  */
L_229e:
    ldr r3, [pc, #112]                      /* @229E 1c4b ldr r3, [pc, #112] literal 0x2310 -> 0x000008FB */
L_22a0:
    ands r2, r3                             /* @22A0 1a40 ands r2, r3  */
L_22a2:
    str r2, [r1, #12]                       /* @22A2 ca60 str r2, [r1, #12]  */
L_22a4:
    ldr r2, [pc, #96]                       /* @22A4 184a ldr r2, [pc, #96] literal 0x2308 -> 0x40020D00 */
L_22a6:
    adds r2, #128                           /* @22A6 8032 adds r2, #128  */
L_22a8:
    ldr r3, [r2, #12]                       /* @22A8 d368 ldr r3, [r2, #12]  */
L_22aa:
    movs r6, #3                             /* @22AA 0326 movs r6, #3  */
L_22ac:
    lsls r6, r6, #14                        /* @22AC b603 lsls r6, r6, #14  */
L_22ae:
    ands r3, r6                             /* @22AE 3340 ands r3, r6  */
L_22b0:
    str r3, [r2, #12]                       /* @22B0 d360 str r3, [r2, #12]  */
L_22b2:
    ldr r3, [pc, #84]                       /* @22B2 154b ldr r3, [pc, #84] literal 0x2308 -> 0x40020D00 */
L_22b4:
    adds r3, #192                           /* @22B4 c033 adds r3, #192  */
L_22b6:
    ldr r6, [r3, #12]                       /* @22B6 de68 ldr r6, [r3, #12]  */
L_22b8:
    ldr r7, [pc, #80]                       /* @22B8 144f ldr r7, [pc, #80] literal 0x230C -> 0x0000FFF4 */
L_22ba:
    subs r7, #233                           /* @22BA e93f subs r7, #233  */
L_22bc:
    ands r6, r7                             /* @22BC 3e40 ands r6, r7  */
L_22be:
    str r6, [r3, #12]                       /* @22BE de60 str r6, [r3, #12]  */
L_22c0:
    ldr r6, [r0]                            /* @22C0 0668 ldr r6, [r0]  */
L_22c2:
    movs r7, #11                            /* @22C2 0b27 movs r7, #11  */
L_22c4:
    orrs r6, r7                             /* @22C4 3e43 orrs r6, r7  */
L_22c6:
    str r6, [r0]                            /* @22C6 0660 str r6, [r0]  */
L_22c8:
    ldr r6, [r1]                            /* @22C8 0e68 ldr r6, [r1]  */
L_22ca:
    ldr r7, [pc, #72]                       /* @22CA 124f ldr r7, [pc, #72] literal 0x2314 -> 0x0000F704 */
L_22cc:
    orrs r6, r7                             /* @22CC 3e43 orrs r6, r7  */
L_22ce:
    str r6, [r1]                            /* @22CE 0e60 str r6, [r1]  */
L_22d0:
    ldr r6, [r2]                            /* @22D0 1668 ldr r6, [r2]  */
L_22d2:
    ldr r7, [pc, #68]                       /* @22D2 114f ldr r7, [pc, #68] literal 0x2318 -> 0x00003FFF */
L_22d4:
    orrs r6, r7                             /* @22D4 3e43 orrs r6, r7  */
L_22d6:
    str r6, [r2]                            /* @22D6 1660 str r6, [r2]  */
L_22d8:
    ldr r6, [r3]                            /* @22D8 1e68 ldr r6, [r3]  */
L_22da:
    movs r7, #244                           /* @22DA f427 movs r7, #244  */
L_22dc:
    orrs r6, r7                             /* @22DC 3e43 orrs r6, r7  */
L_22de:
    str r6, [r3]                            /* @22DE 1e60 str r6, [r3]  */
L_22e0:
    ldr r6, [r0, #32]                       /* @22E0 066a ldr r6, [r0, #32]  */
L_22e2:
    movs r7, #11                            /* @22E2 0b27 movs r7, #11  */
L_22e4:
    orrs r6, r7                             /* @22E4 3e43 orrs r6, r7  */
L_22e6:
    str r6, [r0, #32]                       /* @22E6 0662 str r6, [r0, #32]  */
L_22e8:
    ldr r0, [r1, #32]                       /* @22E8 086a ldr r0, [r1, #32]  */
L_22ea:
    ldr r6, [pc, #40]                       /* @22EA 0a4e ldr r6, [pc, #40] literal 0x2314 -> 0x0000F704 */
L_22ec:
    orrs r0, r6                             /* @22EC 3043 orrs r0, r6  */
L_22ee:
    str r0, [r1, #32]                       /* @22EE 0862 str r0, [r1, #32]  */
L_22f0:
    ldr r0, [r2, #32]                       /* @22F0 106a ldr r0, [r2, #32]  */
L_22f2:
    ldr r1, [pc, #36]                       /* @22F2 0949 ldr r1, [pc, #36] literal 0x2318 -> 0x00003FFF */
L_22f4:
    orrs r0, r1                             /* @22F4 0843 orrs r0, r1  */
L_22f6:
    str r0, [r2, #32]                       /* @22F6 1062 str r0, [r2, #32]  */
L_22f8:
    ldr r0, [r3, #32]                       /* @22F8 186a ldr r0, [r3, #32]  */
L_22fa:
    movs r1, #244                           /* @22FA f421 movs r1, #244  */
L_22fc:
    orrs r0, r1                             /* @22FC 0843 orrs r0, r1  */
L_22fe:
    str r0, [r3, #32]                       /* @22FE 1862 str r0, [r3, #32]  */
L_2300:
    str r5, [r4, #32]                       /* @2300 2562 str r5, [r4, #32]  */
L_2302:
    pop {r4, r5, r6, r7, pc}                /* @2302 f0bd pop {r4, r5, r6, r7, pc}  */
L_2304:
    .byte 0x00,0x20,0x00,0x40 /* @2304 literal */
L_2308:
    .byte 0x00,0x0d,0x02,0x40 /* @2308 literal */
L_230c:
    .byte 0xf4,0xff,0x00,0x00 /* @230C literal */
L_2310:
    .byte 0xfb,0x08,0x00,0x00 /* @2310 literal */
L_2314:
    .byte 0x04,0xf7,0x00,0x00 /* @2314 literal */
L_2318:
    .byte 0xff,0x3f,0x00,0x00 /* @2318 literal */

/* sub_231C */
L_231c:
    ldr r0, [pc, #12]                       /* @231C 0348 ldr r0, [pc, #12] literal 0x232C -> 0x40002000 */
L_231e:
    ldr r1, [pc, #8]                        /* @231E 0249 ldr r1, [pc, #8] literal 0x2328 -> 0x00005A5A */
L_2320:
    str r1, [r0, #8]                        /* @2320 8160 str r1, [r0, #8]  */
L_2322:
    ldr r1, [pc, #12]                       /* @2322 0349 ldr r1, [pc, #12] literal 0x2330 -> 0x0000A5A5 */
L_2324:
    str r1, [r0, #8]                        /* @2324 8160 str r1, [r0, #8]  */
L_2326:
    bx lr                                   /* @2326 7047 bx lr  */
L_2328:
    .byte 0x5a,0x5a,0x00,0x00 /* @2328 literal */
L_232c:
    .byte 0x00,0x20,0x00,0x40 /* @232C literal */
L_2330:
    .byte 0xa5,0xa5,0x00,0x00 /* @2330 literal */

/* uart_start_reply */
L_2334:
    ldr r0, [pc, #72]                       /* @2334 1248 ldr r0, [pc, #72] literal 0x2380 -> 0x40020D00 */
L_2336:
    ldr r1, [r0]                            /* @2336 0168 ldr r1, [r0]  */
L_2338:
    movs r2, #1                             /* @2338 0122 movs r2, #1  */
L_233a:
    lsls r2, r2, #14                        /* @233A 9203 lsls r2, r2, #14  */
L_233c:
    bics r1, r2                             /* @233C 9143 bics r1, r2  */
L_233e:
    str r1, [r0]                            /* @233E 0160 str r1, [r0]  */
L_2340:
    ldr r0, [pc, #64]                       /* @2340 1048 ldr r0, [pc, #64] literal 0x2384 -> 0x40020C00 */
L_2342:
    ldr r1, [r0, #56]                       /* @2342 816b ldr r1, [r0, #56]  */
L_2344:
    lsrs r1, r1, #3                         /* @2344 c908 lsrs r1, r1, #3  */
L_2346:
    lsls r1, r1, #3                         /* @2346 c900 lsls r1, r1, #3  */
L_2348:
    adds r1, r1, #2                         /* @2348 891c adds r1, r1, #2  */
L_234a:
    str r1, [r0, #56]                       /* @234A 8163 str r1, [r0, #56]  */
L_234c:
    lsls r0, r2, #16                        /* @234C 1004 lsls r0, r2, #16  */
L_234e:
    ldr r1, [r0, #4]                        /* @234E 4168 ldr r1, [r0, #4]  */
L_2350:
    lsrs r1, r1, #1                         /* @2350 4908 lsrs r1, r1, #1  */
L_2352:
    lsls r1, r1, #1                         /* @2352 4900 lsls r1, r1, #1  */
L_2354:
    str r1, [r0, #4]                        /* @2354 4160 str r1, [r0, #4]  */
L_2356:
    ldr r2, [r0, #4]                        /* @2356 4268 ldr r2, [r0, #4]  */
L_2358:
    movs r1, #2                             /* @2358 0221 movs r1, #2  */
L_235a:
    orrs r2, r1                             /* @235A 0a43 orrs r2, r1  */
L_235c:
    str r2, [r0, #4]                        /* @235C 4260 str r2, [r0, #4]  */
L_235e:
    ldr r2, [r0, #20]                       /* @235E 4269 ldr r2, [r0, #20]  */
L_2360:
    lsrs r2, r2, #1                         /* @2360 5208 lsrs r2, r2, #1  */
L_2362:
    lsls r2, r2, #1                         /* @2362 5200 lsls r2, r2, #1  */
L_2364:
    str r2, [r0, #20]                       /* @2364 4261 str r2, [r0, #20]  */
L_2366:
    ldr r2, [r0, #20]                       /* @2366 4269 ldr r2, [r0, #20]  */
L_2368:
    bics r2, r1                             /* @2368 8a43 bics r2, r1  */
L_236a:
    str r2, [r0, #20]                       /* @236A 4261 str r2, [r0, #20]  */
L_236c:
    ldr r1, [r0]                            /* @236C 0168 ldr r1, [r0]  */
L_236e:
    ldr r2, [pc, #24]                       /* @236E 064a ldr r2, [pc, #24] literal 0x2388 -> 0x2000072C */
L_2370:
    lsrs r1, r1, #8                         /* @2370 090a lsrs r1, r1, #8  */
L_2372:
    ldrh r2, [r2, #20]                      /* @2372 928a ldrh r2, [r2, #20]  */
L_2374:
    lsls r1, r1, #8                         /* @2374 0902 lsls r1, r1, #8  */
L_2376:
    uxtb r2, r2                             /* @2376 d2b2 uxtb r2, r2  */
L_2378:
    orrs r1, r2                             /* @2378 1143 orrs r1, r2  */
L_237a:
    str r1, [r0]                            /* @237A 0160 str r1, [r0]  */
L_237c:
    bx lr                                   /* @237C 7047 bx lr  */
    .byte 0x00,0x00 /* @237E unclassified */
L_2380:
    .byte 0x00,0x0d,0x02,0x40 /* @2380 literal */
L_2384:
    .byte 0x00,0x0c,0x02,0x40 /* @2384 literal */
L_2388:
    .byte 0x2c,0x07,0x00,0x20 /* @2388 literal */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @238C unclassified */

/* sub_239C */
L_239c:
    push {r3, r4, r5, lr}                   /* @239C 38b5 push {r3, r4, r5, lr}  */
L_239e:
    bl L_3b70                               /* @239E 01f0e7fb bl #6094 -> sub_3B70 */
L_23a2:
    bl L_25f4                               /* @23A2 00f027f9 bl #590 -> sub_25F4 */
L_23a6:
    ldr r4, [pc, #96]                       /* @23A6 184c ldr r4, [pc, #96] literal 0x2408 -> 0x40000C00 */
L_23a8:
    ldr r0, [r4, #60]                       /* @23A8 e06b ldr r0, [r4, #60]  */
L_23aa:
    movs r2, #75                            /* @23AA 4b22 movs r2, #75  */
L_23ac:
    lsrs r0, r0, #16                        /* @23AC 000c lsrs r0, r0, #16  */
L_23ae:
    lsls r0, r0, #16                        /* @23AE 0004 lsls r0, r0, #16  */
L_23b0:
    lsls r2, r2, #4                         /* @23B0 1201 lsls r2, r2, #4  */
L_23b2:
    adds r0, r0, r2                         /* @23B2 8018 adds r0, r0, r2  */
L_23b4:
    str r0, [r4, #60]                       /* @23B4 e063 str r0, [r4, #60]  */
L_23b6:
    ldr r0, [pc, #80]                       /* @23B6 1448 ldr r0, [pc, #80] literal 0x2408 -> 0x40000C00 */
L_23b8:
    adds r0, #64                            /* @23B8 4030 adds r0, #64  */
L_23ba:
    ldr r1, [r0]                            /* @23BA 0168 ldr r1, [r0]  */
L_23bc:
    lsrs r3, r1, #16                        /* @23BC 0b0c lsrs r3, r1, #16  */
L_23be:
    movs r1, #225                           /* @23BE e121 movs r1, #225  */
L_23c0:
    lsls r3, r3, #16                        /* @23C0 1b04 lsls r3, r3, #16  */
L_23c2:
    lsls r1, r1, #2                         /* @23C2 8900 lsls r1, r1, #2  */
L_23c4:
    adds r3, r3, r1                         /* @23C4 5b18 adds r3, r3, r1  */
L_23c6:
    str r3, [r0]                            /* @23C6 0360 str r3, [r0]  */
L_23c8:
    ldr r0, [pc, #64]                       /* @23C8 1048 ldr r0, [pc, #64] literal 0x240C -> 0x40000D00 */
L_23ca:
    ldr r3, [r0, #60]                       /* @23CA c36b ldr r3, [r0, #60]  */
L_23cc:
    lsrs r3, r3, #16                        /* @23CC 1b0c lsrs r3, r3, #16  */
L_23ce:
    lsls r3, r3, #16                        /* @23CE 1b04 lsls r3, r3, #16  */
L_23d0:
    adds r1, r3, r1                         /* @23D0 5918 adds r1, r3, r1  */
L_23d2:
    str r1, [r0, #60]                       /* @23D2 c163 str r1, [r0, #60]  */
L_23d4:
    ldr r0, [pc, #52]                       /* @23D4 0d48 ldr r0, [pc, #52] literal 0x240C -> 0x40000D00 */
L_23d6:
    adds r0, #64                            /* @23D6 4030 adds r0, #64  */
L_23d8:
    ldr r1, [r0]                            /* @23D8 0168 ldr r1, [r0]  */
L_23da:
    lsrs r1, r1, #16                        /* @23DA 090c lsrs r1, r1, #16  */
L_23dc:
    lsls r1, r1, #16                        /* @23DC 0904 lsls r1, r1, #16  */
L_23de:
    adds r1, r1, r2                         /* @23DE 8918 adds r1, r1, r2  */
L_23e0:
    str r1, [r0]                            /* @23E0 0160 str r1, [r0]  */
L_23e2:
    movs r1, #2                             /* @23E2 0221 movs r1, #2  */
L_23e4:
    mov r0, sp                              /* @23E4 6846 mov r0, sp  */
L_23e6:
    bl L_2d84                               /* @23E6 00f0cdfc bl #2458 -> zero_bytes */
L_23ea:
    movs r0, #0                             /* @23EA 0020 movs r0, #0  */
L_23ec:
    mov r1, sp                              /* @23EC 6946 mov r1, sp  */
L_23ee:
    strb r0, [r1, #1]                       /* @23EE 4870 strb r0, [r1, #1]  */
L_23f0:
    movs r0, #3                             /* @23F0 0320 movs r0, #3  */
L_23f2:
    strb r0, [r1]                           /* @23F2 0870 strb r0, [r1]  */
L_23f4:
    mov r0, sp                              /* @23F4 6846 mov r0, sp  */
L_23f6:
    bl L_07e4                               /* @23F6 fef7f5f9 bl #-7190 -> sub_07E4 */
L_23fa:
    bl L_2448                               /* @23FA 00f025f8 bl #74 -> sub_2448 */
L_23fe:
    ldr r0, [r4, #12]                       /* @23FE e068 ldr r0, [r4, #12]  */
L_2400:
    movs r1, #1                             /* @2400 0121 movs r1, #1  */
L_2402:
    orrs r0, r1                             /* @2402 0843 orrs r0, r1  */
L_2404:
    str r0, [r4, #12]                       /* @2404 e060 str r0, [r4, #12]  */
L_2406:
    pop {r3, r4, r5, pc}                    /* @2406 38bd pop {r3, r4, r5, pc}  */
L_2408:
    .byte 0x00,0x0c,0x00,0x40 /* @2408 literal */
L_240c:
    .byte 0x00,0x0d,0x00,0x40 /* @240C literal */

/* sub_2410 */
L_2410:
    push {r4, lr}                           /* @2410 10b5 push {r4, lr}  */
L_2412:
    ldr r0, [pc, #44]                       /* @2412 0b48 ldr r0, [pc, #44] literal 0x2440 -> 0x40002400 */
L_2414:
    ldr r1, [r0, #4]                        /* @2414 4168 ldr r1, [r0, #4]  */
L_2416:
    asrs r2, r0, #15                        /* @2416 c213 asrs r2, r0, #15  */
L_2418:
    bics r1, r2                             /* @2418 9143 bics r1, r2  */
L_241a:
    str r1, [r0, #4]                        /* @241A 4160 str r1, [r0, #4]  */
L_241c:
    movs r2, #0                             /* @241C 0022 movs r2, #0  */
L_241e:
    mov r1, r2                              /* @241E 1146 mov r1, r2  */
L_2420:
    movs r0, #24                            /* @2420 1820 movs r0, #24  */
L_2422:
    bl L_0fec                               /* @2422 fef7e3fd bl #-5178 -> sub_0FEC */
L_2426:
    ldr r0, [pc, #28]                       /* @2426 0748 ldr r0, [pc, #28] literal 0x2444 -> 0x40021000 */
L_2428:
    ldr r1, [r0, #20]                       /* @2428 4169 ldr r1, [r0, #20]  */
L_242a:
    movs r2, #7                             /* @242A 0722 movs r2, #7  */
L_242c:
    lsls r2, r2, #16                        /* @242C 1204 lsls r2, r2, #16  */
L_242e:
    bics r1, r2                             /* @242E 9143 bics r1, r2  */
L_2430:
    str r1, [r0, #20]                       /* @2430 4161 str r1, [r0, #20]  */
L_2432:
    ldr r0, [pc, #12]                       /* @2432 0348 ldr r0, [pc, #12] literal 0x2440 -> 0x40002400 */
L_2434:
    adds r0, #128                           /* @2434 8030 adds r0, #128  */
L_2436:
    ldr r1, [r0, #52]                       /* @2436 416b ldr r1, [r0, #52]  */
L_2438:
    movs r2, #32                            /* @2438 2022 movs r2, #32  */
L_243a:
    bics r1, r2                             /* @243A 9143 bics r1, r2  */
L_243c:
    str r1, [r0, #52]                       /* @243C 4163 str r1, [r0, #52]  */
L_243e:
    pop {r4, pc}                            /* @243E 10bd pop {r4, pc}  */
L_2440:
    .byte 0x00,0x24,0x00,0x40 /* @2440 literal */
L_2444:
    .byte 0x00,0x10,0x02,0x40 /* @2444 literal */

/* sub_2448 */
L_2448:
    push {r4, lr}                           /* @2448 10b5 push {r4, lr}  */
L_244a:
    movs r2, #1                             /* @244A 0122 movs r2, #1  */
L_244c:
    movs r1, #0                             /* @244C 0021 movs r1, #0  */
L_244e:
    movs r0, #24                            /* @244E 1820 movs r0, #24  */
L_2450:
    bl L_0fec                               /* @2450 fef7ccfd bl #-5224 -> sub_0FEC */
L_2454:
    ldr r0, [pc, #8]                        /* @2454 0248 ldr r0, [pc, #8] literal 0x2460 -> 0x40002400 */
L_2456:
    ldr r1, [r0, #4]                        /* @2456 4168 ldr r1, [r0, #4]  */
L_2458:
    asrs r2, r0, #15                        /* @2458 c213 asrs r2, r0, #15  */
L_245a:
    orrs r1, r2                             /* @245A 1143 orrs r1, r2  */
L_245c:
    str r1, [r0, #4]                        /* @245C 4160 str r1, [r0, #4]  */
L_245e:
    pop {r4, pc}                            /* @245E 10bd pop {r4, pc}  */
L_2460:
    .byte 0x00,0x24,0x00,0x40 /* @2460 literal */

/* check_current_offsets */
L_2464:
    push {r1, r2, r3, r4, r5, r6, r7, lr}   /* @2464 feb5 push {r1, r2, r3, r4, r5, r6, r7, lr}  */
L_2466:
    movs r5, #0                             /* @2466 0025 movs r5, #0  */
L_2468:
    bl L_07b0                               /* @2468 fef7a2f9 bl #-7356 -> adc_start */
L_246c:
    movs r0, #5                             /* @246C 0520 movs r0, #5  */
L_246e:
    bl L_2de8                               /* @246E 00f0bbfc bl #2422 -> delay_ms_candidate */
L_2472:
    movs r0, #32                            /* @2472 2020 movs r0, #32  */
L_2474:
    bl L_0738                               /* @2474 fef760f9 bl #-7488 -> adc_test_flags */
L_2478:
    cmp r0, #0                              /* @2478 0028 cmp r0, #0  */
L_247a:
    beq L_2472                              /* @247A fad0 beq #-12 -> 0x2472 */
L_247c:
    movs r0, #2                             /* @247C 0220 movs r0, #2  */
L_247e:
    bl L_074c                               /* @247E fef765f9 bl #-7478 -> adc_read_result */
L_2482:
    mov r4, r0                              /* @2482 0446 mov r4, r0  */
L_2484:
    movs r0, #1                             /* @2484 0120 movs r0, #1  */
L_2486:
    bl L_074c                               /* @2486 fef761f9 bl #-7486 -> adc_read_result */
L_248a:
    cmp r5, #0                              /* @248A 002d cmp r5, #0  */
L_248c:
    beq L_2496                              /* @248C 03d0 beq #6 -> 0x2496 */
L_248e:
    cmp r4, r7                              /* @248E bc42 cmp r4, r7  */
L_2490:
    bge L_24a0                              /* @2490 06da bge #12 -> 0x24A0 */
L_2492:
    mov r7, r4                              /* @2492 2746 mov r7, r4  */
L_2494:
    b L_24a8                                /* @2494 08e0 b #16 -> 0x24A8 */
L_2496:
    mov r7, r4                              /* @2496 2746 mov r7, r4  */
L_2498:
    mov r6, r0                              /* @2498 0646 mov r6, r0  */
L_249a:
    str r4, [sp, #8]                        /* @249A 0294 str r4, [sp, #8]  */
L_249c:
    str r0, [sp, #4]                        /* @249C 0190 str r0, [sp, #4]  */
L_249e:
    b L_24b8                                /* @249E 0be0 b #22 -> 0x24B8 */
L_24a0:
    ldr r1, [sp, #8]                        /* @24A0 0299 ldr r1, [sp, #8]  */
L_24a2:
    cmp r4, r1                              /* @24A2 8c42 cmp r4, r1  */
L_24a4:
    ble L_24a8                              /* @24A4 00dd ble #0 -> 0x24A8 */
L_24a6:
    str r4, [sp, #8]                        /* @24A6 0294 str r4, [sp, #8]  */
L_24a8:
    cmp r0, r6                              /* @24A8 b042 cmp r0, r6  */
L_24aa:
    bge L_24b0                              /* @24AA 01da bge #2 -> 0x24B0 */
L_24ac:
    mov r6, r0                              /* @24AC 0646 mov r6, r0  */
L_24ae:
    b L_24b8                                /* @24AE 03e0 b #6 -> 0x24B8 */
L_24b0:
    ldr r1, [sp, #4]                        /* @24B0 0199 ldr r1, [sp, #4]  */
L_24b2:
    cmp r0, r1                              /* @24B2 8842 cmp r0, r1  */
L_24b4:
    ble L_24b8                              /* @24B4 00dd ble #0 -> 0x24B8 */
L_24b6:
    str r0, [sp, #4]                        /* @24B6 0190 str r0, [sp, #4]  */
L_24b8:
    ldr r1, [pc, #136]                      /* @24B8 2249 ldr r1, [pc, #136] literal 0x2544 -> 0x200006E0 */
L_24ba:
    ldr r2, [r1, #4]                        /* @24BA 4a68 ldr r2, [r1, #4]  */
L_24bc:
    adds r2, r2, r4                         /* @24BC 1219 adds r2, r2, r4  */
L_24be:
    str r2, [r1, #4]                        /* @24BE 4a60 str r2, [r1, #4]  */
L_24c0:
    ldr r2, [r1, #8]                        /* @24C0 8a68 ldr r2, [r1, #8]  */
L_24c2:
    adds r0, r2, r0                         /* @24C2 1018 adds r0, r2, r0  */
L_24c4:
    str r0, [r1, #8]                        /* @24C4 8860 str r0, [r1, #8]  */
L_24c6:
    movs r0, #32                            /* @24C6 2020 movs r0, #32  */
L_24c8:
    bl L_0718                               /* @24C8 fef726f9 bl #-7604 -> adc_clear_flags */
L_24cc:
    adds r5, r5, #1                         /* @24CC 6d1c adds r5, r5, #1  */
L_24ce:
    uxth r5, r5                             /* @24CE adb2 uxth r5, r5  */
L_24d0:
    cmp r5, #18                             /* @24D0 122d cmp r5, #18  */
L_24d2:
    blo L_2468                              /* @24D2 c9d3 blo #-110 -> 0x2468 */
L_24d4:
    bl L_07bc                               /* @24D4 fef772f9 bl #-7452 -> adc_stop */
L_24d8:
    ldr r0, [pc, #104]                      /* @24D8 1a48 ldr r0, [pc, #104] literal 0x2544 -> 0x200006E0 */
L_24da:
    ldr r1, [r0, #4]                        /* @24DA 4168 ldr r1, [r0, #4]  */
L_24dc:
    subs r2, r1, r7                         /* @24DC ca1b subs r2, r1, r7  */
L_24de:
    ldr r1, [sp, #8]                        /* @24DE 0299 ldr r1, [sp, #8]  */
L_24e0:
    subs r1, r2, r1                         /* @24E0 511a subs r1, r2, r1  */
L_24e2:
    adds r1, #8                             /* @24E2 0831 adds r1, #8  */
L_24e4:
    asrs r1, r1, #4                         /* @24E4 0911 asrs r1, r1, #4  */
L_24e6:
    str r1, [r0, #4]                        /* @24E6 4160 str r1, [r0, #4]  */
L_24e8:
    ldr r1, [r0, #8]                        /* @24E8 8168 ldr r1, [r0, #8]  */
L_24ea:
    subs r2, r1, r6                         /* @24EA 8a1b subs r2, r1, r6  */
L_24ec:
    ldr r1, [sp, #4]                        /* @24EC 0199 ldr r1, [sp, #4]  */
L_24ee:
    subs r1, r2, r1                         /* @24EE 511a subs r1, r2, r1  */
L_24f0:
    adds r1, #8                             /* @24F0 0831 adds r1, #8  */
L_24f2:
    asrs r1, r1, #4                         /* @24F2 0911 asrs r1, r1, #4  */
L_24f4:
    str r1, [r0, #8]                        /* @24F4 8160 str r1, [r0, #8]  */
L_24f6:
    ldr r2, [r0, #4]                        /* @24F6 4268 ldr r2, [r0, #4]  */
L_24f8:
    ldr r1, [pc, #76]                       /* @24F8 1349 ldr r1, [pc, #76] literal 0x2548 -> 0x00000745 */
L_24fa:
    cmp r2, r1                              /* @24FA 8a42 cmp r2, r1  */
L_24fc:
    blt L_2512                              /* @24FC 09db blt #18 -> 0x2512 */
L_24fe:
    ldr r3, [r0, #4]                        /* @24FE 4368 ldr r3, [r0, #4]  */
L_2500:
    ldr r2, [pc, #72]                       /* @2500 124a ldr r2, [pc, #72] literal 0x254C -> 0x000008BA */
L_2502:
    cmp r3, r2                              /* @2502 9342 cmp r3, r2  */
L_2504:
    bgt L_2512                              /* @2504 05dc bgt #10 -> 0x2512 */
L_2506:
    ldr r3, [r0, #8]                        /* @2506 8368 ldr r3, [r0, #8]  */
L_2508:
    cmp r3, r1                              /* @2508 8b42 cmp r3, r1  */
L_250a:
    blt L_2512                              /* @250A 02db blt #4 -> 0x2512 */
L_250c:
    ldr r0, [r0, #8]                        /* @250C 8068 ldr r0, [r0, #8]  */
L_250e:
    cmp r0, r2                              /* @250E 9042 cmp r0, r2  */
L_2510:
    ble L_251e                              /* @2510 05dd ble #10 -> 0x251E */
L_2512:
    adr r2, L_2550                          /* @2512 0fa2 adr r2, #60 address 0x2550 -> 0x00002550 'Error!' */
L_2514:
    adr r1, L_2558                          /* @2514 10a1 adr r1, #64 address 0x2558 -> 0x00002558 ' Current' */
L_2516:
    adr r0, L_2564                          /* @2516 13a0 adr r0, #76 address 0x2564 -> 0x00002564 'Offset' */
L_2518:
    bl L_4884                               /* @2518 02f0b4f9 bl #9064 -> display_three_lines */
L_251c:
    b L_251c                                /* @251C fee7 b #-4 -> 0x251C */
L_251e:
    movs r0, #1                             /* @251E 0120 movs r0, #1  */
L_2520:
    mov r1, sp                              /* @2520 6946 mov r1, sp  */
L_2522:
    strb r0, [r1]                           /* @2522 0870 strb r0, [r1]  */
L_2524:
    strb r0, [r1, #1]                       /* @2524 4870 strb r0, [r1, #1]  */
L_2526:
    movs r0, #0                             /* @2526 0020 movs r0, #0  */
L_2528:
    bl L_0b14                               /* @2528 fef7f4fa bl #-6680 -> sub_0B14 */
L_252c:
    movs r0, #0                             /* @252C 0020 movs r0, #0  */
L_252e:
    bl L_08b0                               /* @252E fef7bff9 bl #-7298 -> sub_08B0 */
L_2532:
    movs r1, #0                             /* @2532 0021 movs r1, #0  */
L_2534:
    mov r0, r1                              /* @2534 0846 mov r0, r1  */
L_2536:
    bl L_08c0                               /* @2536 fef7c3f9 bl #-7290 -> sub_08C0 */
L_253a:
    movs r1, #1                             /* @253A 0121 movs r1, #1  */
L_253c:
    mov r0, r1                              /* @253C 0846 mov r0, r1  */
L_253e:
    bl L_07c8                               /* @253E fef743f9 bl #-7546 -> sub_07C8 */
L_2542:
    pop {r1, r2, r3, r4, r5, r6, r7, pc}    /* @2542 febd pop {r1, r2, r3, r4, r5, r6, r7, pc}  */
L_2544:
    .byte 0xe0,0x06,0x00,0x20 /* @2544 literal */
L_2548:
    .byte 0x45,0x07,0x00,0x00 /* @2548 literal */
L_254c:
    .byte 0xba,0x08,0x00,0x00 /* @254C literal */
L_2550:
    .byte 0x45,0x72,0x72,0x6f,0x72,0x21,0x00,0x00 /* @2550 unclassified */
L_2558:
    .byte 0x20,0x43,0x75,0x72,0x72,0x65,0x6e,0x74,0x00,0x00,0x00,0x00 /* @2558 unclassified */
L_2564:
    .byte 0x4f,0x66,0x66,0x73,0x65,0x74,0x00,0x00 /* @2564 unclassified */

/* sub_256C */
L_256c:
    push {r0, r1, r2, r3, r4, lr}           /* @256C 1fb5 push {r0, r1, r2, r3, r4, lr}  */
L_256e:
    movs r1, #12                            /* @256E 0c21 movs r1, #12  */
L_2570:
    add r0, sp, #4                          /* @2570 01a8 add r0, sp, #4  */
L_2572:
    bl L_2d84                               /* @2572 00f007fc bl #2062 -> zero_bytes */
L_2576:
    movs r1, #2                             /* @2576 0221 movs r1, #2  */
L_2578:
    mov r0, sp                              /* @2578 6846 mov r0, sp  */
L_257a:
    bl L_2d84                               /* @257A 00f003fc bl #2054 -> zero_bytes */
L_257e:
    movs r1, #0                             /* @257E 0021 movs r1, #0  */
L_2580:
    movs r0, #64                            /* @2580 4020 movs r0, #64  */
L_2582:
    bl L_141c                               /* @2582 fef74bff bl #-4458 -> sub_141C */
L_2586:
    movs r1, #1                             /* @2586 0121 movs r1, #1  */
L_2588:
    movs r0, #64                            /* @2588 4020 movs r0, #64  */
L_258a:
    bl L_141c                               /* @258A fef747ff bl #-4466 -> sub_141C */
L_258e:
    movs r1, #11                            /* @258E 0b21 movs r1, #11  */
L_2590:
    movs r0, #64                            /* @2590 4020 movs r0, #64  */
L_2592:
    bl L_141c                               /* @2592 fef743ff bl #-4474 -> sub_141C */
L_2596:
    bl L_0728                               /* @2596 fef7c7f8 bl #-7794 -> sub_0728 */
L_259a:
    bl L_0898                               /* @259A fef77df9 bl #-7430 -> sub_0898 */
L_259e:
    movs r0, #1                             /* @259E 0120 movs r0, #1  */
L_25a0:
    bl L_2de8                               /* @25A0 00f022fc bl #2116 -> delay_ms_candidate */
L_25a4:
    movs r0, #1                             /* @25A4 0120 movs r0, #1  */
L_25a6:
    mov r1, sp                              /* @25A6 6946 mov r1, sp  */
L_25a8:
    strb r0, [r1, #4]                       /* @25A8 0871 strb r0, [r1, #4]  */
L_25aa:
    movs r0, #8                             /* @25AA 0820 movs r0, #8  */
L_25ac:
    strb r0, [r1, #5]                       /* @25AC 4871 strb r0, [r1, #5]  */
L_25ae:
    lsls r0, r0, #10                        /* @25AE 8002 lsls r0, r0, #10  */
L_25b0:
    strh r0, [r1, #6]                       /* @25B0 c880 strh r0, [r1, #6]  */
L_25b2:
    movs r0, #3                             /* @25B2 0320 movs r0, #3  */
L_25b4:
    lsls r0, r0, #9                         /* @25B4 4002 lsls r0, r0, #9  */
L_25b6:
    strh r0, [r1, #8]                       /* @25B6 0881 strh r0, [r1, #8]  */
L_25b8:
    movs r4, #0                             /* @25B8 0024 movs r4, #0  */
L_25ba:
    strh r4, [r1, #10]                      /* @25BA 4c81 strh r4, [r1, #10]  */
L_25bc:
    strh r4, [r1, #12]                      /* @25BC 8c81 strh r4, [r1, #12]  */
L_25be:
    strb r4, [r1, #14]                      /* @25BE 8c73 strb r4, [r1, #14]  */
L_25c0:
    add r0, sp, #4                          /* @25C0 01a8 add r0, sp, #4  */
L_25c2:
    bl L_0758                               /* @25C2 fef7c9f8 bl #-7790 -> sub_0758 */
L_25c6:
    mov r1, sp                              /* @25C6 6946 mov r1, sp  */
L_25c8:
    strb r4, [r1, #1]                       /* @25C8 4c70 strb r4, [r1, #1]  */
L_25ca:
    movs r0, #3                             /* @25CA 0320 movs r0, #3  */
L_25cc:
    strb r0, [r1]                           /* @25CC 0870 strb r0, [r1]  */
L_25ce:
    mov r0, sp                              /* @25CE 6846 mov r0, sp  */
L_25d0:
    bl L_07e4                               /* @25D0 fef708f9 bl #-7664 -> sub_07E4 */
L_25d4:
    movs r1, #8                             /* @25D4 0821 movs r1, #8  */
L_25d6:
    movs r0, #0                             /* @25D6 0020 movs r0, #0  */
L_25d8:
    bl L_06bc                               /* @25D8 fef770f8 bl #-7968 -> sub_06BC */
L_25dc:
    movs r1, #9                             /* @25DC 0921 movs r1, #9  */
L_25de:
    movs r0, #1                             /* @25DE 0120 movs r0, #1  */
L_25e0:
    bl L_06bc                               /* @25E0 fef76cf8 bl #-7976 -> sub_06BC */
L_25e4:
    movs r1, #18                            /* @25E4 1221 movs r1, #18  */
L_25e6:
    movs r0, #2                             /* @25E6 0220 movs r0, #2  */
L_25e8:
    bl L_06bc                               /* @25E8 fef768f8 bl #-7984 -> sub_06BC */
L_25ec:
    movs r0, #32                            /* @25EC 2020 movs r0, #32  */
L_25ee:
    bl L_0718                               /* @25EE fef793f8 bl #-7898 -> adc_clear_flags */
L_25f2:
    pop {r0, r1, r2, r3, r4, pc}            /* @25F2 1fbd pop {r0, r1, r2, r3, r4, pc}  */

/* sub_25F4 */
L_25f4:
    ldr r0, [pc, #12]                       /* @25F4 0348 ldr r0, [pc, #12] literal 0x2604 -> 0x200006E0 */
L_25f6:
    movs r1, #0                             /* @25F6 0021 movs r1, #0  */
L_25f8:
    strb r1, [r0]                           /* @25F8 0170 strb r1, [r0]  */
L_25fa:
    str r1, [r0, #12]                       /* @25FA c160 str r1, [r0, #12]  */
L_25fc:
    str r1, [r0, #16]                       /* @25FC 0161 str r1, [r0, #16]  */
L_25fe:
    strh r1, [r0, #2]                       /* @25FE 4180 strh r1, [r0, #2]  */
L_2600:
    bx lr                                   /* @2600 7047 bx lr  */
    .byte 0x00,0x00 /* @2602 unclassified */
L_2604:
    .byte 0xe0,0x06,0x00,0x20 /* @2604 literal */

/* board_init */
L_2608:
    push {r4, lr}                           /* @2608 10b5 push {r4, lr}  */
L_260a:
    bl L_286c                               /* @260A 00f02ff9 bl #606 -> sub_286C */
L_260e:
    bl L_3f84                               /* @260E 01f0b9fc bl #6514 -> sub_3F84 */
L_2612:
    bl L_2634                               /* @2612 00f00ff8 bl #30 -> sub_2634 */
L_2616:
    bl L_4c8c                               /* @2616 02f039fb bl #9842 -> sub_4C8C */
L_261a:
    bl L_505c                               /* @261A 02f01ffd bl #10814 -> sub_505C */
L_261e:
    bl L_35a8                               /* @261E 00f0c3ff bl #3974 -> sub_35A8 */
L_2622:
    bl L_256c                               /* @2622 fff7a3ff bl #-186 -> sub_256C */
L_2626:
    movs r1, #0                             /* @2626 0021 movs r1, #0  */
L_2628:
    movs r0, #12                            /* @2628 0c20 movs r0, #12  */
L_262a:
    bl L_1078                               /* @262A fef725fd bl #-5558 -> sub_1078 */
L_262e:
    pop {r4, pc}                            /* @262E 10bd pop {r4, pc}  */
    .byte 0x00,0x00,0x00,0x00 /* @2630 unclassified */

/* sub_2634 */
L_2634:
    push {r2, r3, r4, lr}                   /* @2634 1cb5 push {r2, r3, r4, lr}  */
L_2636:
    movs r0, #1                             /* @2636 0120 movs r0, #1  */
L_2638:
    mov r2, sp                              /* @2638 6a46 mov r2, sp  */
L_263a:
    strb r0, [r2]                           /* @263A 1070 strb r0, [r2]  */
L_263c:
    strb r0, [r2, #1]                       /* @263C 5070 strb r0, [r2, #1]  */
L_263e:
    strb r0, [r2, #2]                       /* @263E 9070 strb r0, [r2, #2]  */
L_2640:
    movs r1, #0                             /* @2640 0021 movs r1, #0  */
L_2642:
    strb r1, [r2, #3]                       /* @2642 d170 strb r1, [r2, #3]  */
L_2644:
    strb r1, [r2, #4]                       /* @2644 1171 strb r1, [r2, #4]  */
L_2646:
    strb r0, [r2, #5]                       /* @2646 5071 strb r0, [r2, #5]  */
L_2648:
    movs r1, #9                             /* @2648 0921 movs r1, #9  */
L_264a:
    movs r0, #0                             /* @264A 0020 movs r0, #0  */
L_264c:
    bl L_137c                               /* @264C fef796fe bl #-4820 -> sub_137C */
L_2650:
    mov r2, sp                              /* @2650 6a46 mov r2, sp  */
L_2652:
    movs r1, #10                            /* @2652 0a21 movs r1, #10  */
L_2654:
    movs r0, #0                             /* @2654 0020 movs r0, #0  */
L_2656:
    bl L_137c                               /* @2656 fef791fe bl #-4830 -> sub_137C */
L_265a:
    mov r2, sp                              /* @265A 6a46 mov r2, sp  */
L_265c:
    movs r1, #4                             /* @265C 0421 movs r1, #4  */
L_265e:
    movs r0, #64                            /* @265E 4020 movs r0, #64  */
L_2660:
    bl L_137c                               /* @2660 fef78cfe bl #-4840 -> sub_137C */
L_2664:
    movs r1, #9                             /* @2664 0921 movs r1, #9  */
L_2666:
    movs r0, #0                             /* @2666 0020 movs r0, #0  */
L_2668:
    bl L_135c                               /* @2668 fef778fe bl #-4880 -> sub_135C */
L_266c:
    cmp r0, #0                              /* @266C 0028 cmp r0, #0  */
L_266e:
    bne L_2678                              /* @266E 03d1 bne #6 -> 0x2678 */
L_2670:
    movs r0, #1                             /* @2670 0120 movs r0, #1  */
L_2672:
    bl L_3f28                               /* @2672 01f059fc bl #6322 -> sub_3F28 */
L_2676:
    b L_2676                                /* @2676 fee7 b #-4 -> 0x2676 */
L_2678:
    pop {r2, r3, r4, pc}                    /* @2678 1cbd pop {r2, r3, r4, pc}  */
    .byte 0x00,0x00 /* @267A unclassified */

/* load_calibration_knots */
L_267c:
    push {r4, r5, lr}                       /* @267C 30b5 push {r4, r5, lr}  */
L_267e:
    movs r0, #0                             /* @267E 0020 movs r0, #0  */
L_2680:
    movs r1, #255                           /* @2680 ff21 movs r1, #255  */
L_2682:
    lsls r1, r1, #7                         /* @2682 c901 lsls r1, r1, #7  */
L_2684:
    ldrh r2, [r1, #20]                      /* @2684 8a8a ldrh r2, [r1, #20]  */
L_2686:
    cmp r2, #1                              /* @2686 012a cmp r2, #1  */
L_2688:
    bne L_2690                              /* @2688 02d1 bne #4 -> 0x2690 */
L_268a:
    ldrh r2, [r1, #22]                      /* @268A ca8a ldrh r2, [r1, #22]  */
L_268c:
    cmp r2, #1                              /* @268C 012a cmp r2, #1  */
L_268e:
    beq L_2694                              /* @268E 01d0 beq #2 -> 0x2694 */
L_2690:
    movs r0, #0                             /* @2690 0020 movs r0, #0  */
L_2692:
    pop {r4, r5, pc}                        /* @2692 30bd pop {r4, r5, pc}  */
L_2694:
    ldr r2, [pc, #40]                       /* @2694 0a4a ldr r2, [pc, #40] literal 0x26C0 -> 0x200008C8 */
L_2696:
    ldrh r3, [r1, #16]                      /* @2696 0b8a ldrh r3, [r1, #16]  */
L_2698:
    strh r3, [r2, #16]                      /* @2698 1382 strh r3, [r2, #16]  */
L_269a:
    ldrh r1, [r1, #18]                      /* @269A 498a ldrh r1, [r1, #18]  */
L_269c:
    strh r1, [r2, #18]                      /* @269C 5182 strh r1, [r2, #18]  */
L_269e:
    ldr r4, [pc, #36]                       /* @269E 094c ldr r4, [pc, #36] literal 0x26C4 -> 0x20000748 */
L_26a0:
    movs r3, #63                            /* @26A0 3f23 movs r3, #63  */
L_26a2:
    lsls r3, r3, #9                         /* @26A2 5b02 lsls r3, r3, #9  */
L_26a4:
    lsls r1, r0, #1                         /* @26A4 4100 lsls r1, r0, #1  */
L_26a6:
    adds r5, r1, r3                         /* @26A6 cd18 adds r5, r1, r3  */
L_26a8:
    ldrh r5, [r5]                           /* @26A8 2d88 ldrh r5, [r5]  */
L_26aa:
    strh r5, [r4, r1]                       /* @26AA 6552 strh r5, [r4, r1]  */
L_26ac:
    adds r0, r0, #1                         /* @26AC 401c adds r0, r0, #1  */
L_26ae:
    uxth r0, r0                             /* @26AE 80b2 uxth r0, r0  */
L_26b0:
    cmp r0, #200                            /* @26B0 c828 cmp r0, #200  */
L_26b2:
    blo L_26a4                              /* @26B2 f7d3 blo #-18 -> 0x26A4 */
L_26b4:
    movs r0, #1                             /* @26B4 0120 movs r0, #1  */
L_26b6:
    strh r0, [r2, #20]                      /* @26B6 9082 strh r0, [r2, #20]  */
L_26b8:
    strh r0, [r2, #22]                      /* @26B8 d082 strh r0, [r2, #22]  */
L_26ba:
    ldr r1, [pc, #12]                       /* @26BA 0349 ldr r1, [pc, #12] literal 0x26C8 -> 0x2000000B */
L_26bc:
    strb r0, [r1]                           /* @26BC 0870 strb r0, [r1]  */
L_26be:
    pop {r4, r5, pc}                        /* @26BE 30bd pop {r4, r5, pc}  */
L_26c0:
    .byte 0xc8,0x08,0x00,0x20 /* @26C0 literal */
L_26c4:
    .byte 0x48,0x07,0x00,0x20 /* @26C4 literal */
L_26c8:
    .byte 0x0b,0x00,0x00,0x20 /* @26C8 literal */

/* encoder_lut_read */
L_26cc:
    ldr r1, [pc, #20]                       /* @26CC 0549 ldr r1, [pc, #20] literal 0x26E4 -> 0x2000000B */
L_26ce:
    ldrb r1, [r1]                           /* @26CE 0978 ldrb r1, [r1]  */
L_26d0:
    cmp r1, #0                              /* @26D0 0029 cmp r1, #0  */
L_26d2:
    beq L_26e0                              /* @26D2 05d0 beq #10 -> 0x26E0 */
L_26d4:
    movs r1, #1                             /* @26D4 0121 movs r1, #1  */
L_26d6:
    lsls r0, r0, #1                         /* @26D6 4000 lsls r0, r0, #1  */
L_26d8:
    lsls r1, r1, #15                        /* @26D8 c903 lsls r1, r1, #15  */
L_26da:
    adds r0, r0, r1                         /* @26DA 4018 adds r0, r0, r1  */
L_26dc:
    ldrh r0, [r0]                           /* @26DC 0088 ldrh r0, [r0]  */
L_26de:
    bx lr                                   /* @26DE 7047 bx lr  */
L_26e0:
    movs r0, #0                             /* @26E0 0020 movs r0, #0  */
L_26e2:
    bx lr                                   /* @26E2 7047 bx lr  */
L_26e4:
    .byte 0x0b,0x00,0x00,0x20 /* @26E4 literal */

/* encoder_inverse_lookup */
L_26e8:
    push {r1, r2, r3, r4, r5, r6, r7, lr}   /* @26E8 feb5 push {r1, r2, r3, r4, r5, r6, r7, lr}  */
L_26ea:
    mov r7, r0                              /* @26EA 0746 mov r7, r0  */
L_26ec:
    movs r5, #0                             /* @26EC 0025 movs r5, #0  */
L_26ee:
    ldr r0, [pc, #140]                      /* @26EE 2348 ldr r0, [pc, #140] literal 0x277C -> 0x200008C8 */
L_26f0:
    ldrh r0, [r0, #18]                      /* @26F0 408a ldrh r0, [r0, #18]  */
L_26f2:
    cmp r0, r7                              /* @26F2 b842 cmp r0, r7  */
L_26f4:
    ble L_276a                              /* @26F4 39dd ble #114 -> 0x276A */
L_26f6:
    movs r0, #1                             /* @26F6 0120 movs r0, #1  */
L_26f8:
    lsls r0, r0, #14                        /* @26F8 8003 lsls r0, r0, #14  */
L_26fa:
    adds r7, r7, r0                         /* @26FA 3f18 adds r7, r7, r0  */
L_26fc:
    b L_276a                                /* @26FC 35e0 b #106 -> 0x276A */
L_26fe:
    ldr r0, [pc, #128]                      /* @26FE 2048 ldr r0, [pc, #128] literal 0x2780 -> 0x20000748 */
L_2700:
    lsls r1, r5, #1                         /* @2700 6900 lsls r1, r5, #1  */
L_2702:
    ldrh r6, [r0, r1]                       /* @2702 465a ldrh r6, [r0, r1]  */
L_2704:
    cmp r5, #199                            /* @2704 c72d cmp r5, #199  */
L_2706:
    beq L_2710                              /* @2706 03d0 beq #6 -> 0x2710 */
L_2708:
    adds r1, r5, #1                         /* @2708 691c adds r1, r5, #1  */
L_270a:
    lsls r1, r1, #1                         /* @270A 4900 lsls r1, r1, #1  */
L_270c:
    ldrh r4, [r0, r1]                       /* @270C 445a ldrh r4, [r0, r1]  */
L_270e:
    b L_2712                                /* @270E 00e0 b #0 -> 0x2712 */
L_2710:
    ldrh r4, [r0]                           /* @2710 0488 ldrh r4, [r0]  */
L_2712:
    subs r0, r6, r4                         /* @2712 301b subs r0, r6, r4  */
L_2714:
    bl L_373a                               /* @2714 01f011f8 bl #4130 -> abs32_candidate */
L_2718:
    movs r1, #1                             /* @2718 0121 movs r1, #1  */
L_271a:
    lsls r1, r1, #13                        /* @271A 4903 lsls r1, r1, #13  */
L_271c:
    cmp r0, r1                              /* @271C 8842 cmp r0, r1  */
L_271e:
    ble L_2730                              /* @271E 07dd ble #14 -> 0x2730 */
L_2720:
    cmp r6, r4                              /* @2720 a642 cmp r6, r4  */
L_2722:
    bge L_272a                              /* @2722 02da bge #4 -> 0x272A */
L_2724:
    lsls r0, r1, #1                         /* @2724 4800 lsls r0, r1, #1  */
L_2726:
    adds r6, r6, r0                         /* @2726 3618 adds r6, r6, r0  */
L_2728:
    b L_2730                                /* @2728 02e0 b #4 -> 0x2730 */
L_272a:
    movs r0, #1                             /* @272A 0120 movs r0, #1  */
L_272c:
    lsls r0, r0, #14                        /* @272C 8003 lsls r0, r0, #14  */
L_272e:
    adds r4, r4, r0                         /* @272E 2418 adds r4, r4, r0  */
L_2730:
    cmp r7, r6                              /* @2730 b742 cmp r7, r6  */
L_2732:
    blt L_2738                              /* @2732 01db blt #2 -> 0x2738 */
L_2734:
    cmp r7, r4                              /* @2734 a742 cmp r7, r4  */
L_2736:
    ble L_2740                              /* @2736 03dd ble #6 -> 0x2740 */
L_2738:
    cmp r7, r4                              /* @2738 a742 cmp r7, r4  */
L_273a:
    blt L_2768                              /* @273A 15db blt #42 -> 0x2768 */
L_273c:
    cmp r7, r6                              /* @273C b742 cmp r7, r6  */
L_273e:
    bgt L_2768                              /* @273E 13dc bgt #38 -> 0x2768 */
L_2740:
    lsls r0, r5, #13                        /* @2740 6803 lsls r0, r5, #13  */
L_2742:
    adds r0, #12                            /* @2742 0c30 adds r0, #12  */
L_2744:
    movs r1, #25                            /* @2744 1921 movs r1, #25  */
L_2746:
    bl L_0110                               /* @2746 fdf7e3fc bl #-9786 -> udivmod32 */
L_274a:
    str r0, [sp, #4]                        /* @274A 0190 str r0, [sp, #4]  */
L_274c:
    adds r5, r5, #1                         /* @274C 6d1c adds r5, r5, #1  */
L_274e:
    lsls r0, r5, #13                        /* @274E 6803 lsls r0, r5, #13  */
L_2750:
    adds r0, #12                            /* @2750 0c30 adds r0, #12  */
L_2752:
    movs r1, #25                            /* @2752 1921 movs r1, #25  */
L_2754:
    bl L_0110                               /* @2754 fdf7dcfc bl #-9800 -> udivmod32 */
L_2758:
    mov r3, r0                              /* @2758 0346 mov r3, r0  */
L_275a:
    mov r2, r4                              /* @275A 2246 mov r2, r4  */
L_275c:
    str r7, [sp]                            /* @275C 0097 str r7, [sp]  */
L_275e:
    mov r0, r6                              /* @275E 3046 mov r0, r6  */
L_2760:
    ldr r1, [sp, #4]                        /* @2760 0199 ldr r1, [sp, #4]  */
L_2762:
    bl L_3ecc                               /* @2762 01f0b3fb bl #5990 -> sub_3ECC */
L_2766:
    pop {r1, r2, r3, r4, r5, r6, r7, pc}    /* @2766 febd pop {r1, r2, r3, r4, r5, r6, r7, pc}  */
L_2768:
    adds r5, r5, #1                         /* @2768 6d1c adds r5, r5, #1  */
L_276a:
    cmp r5, #200                            /* @276A c82d cmp r5, #200  */
L_276c:
    blt L_26fe                              /* @276C c7db blt #-114 -> 0x26FE */
L_276e:
    adr r2, L_2784                          /* @276E 05a2 adr r2, #20 address 0x2784 -> 0x00002784 'Error!' */
L_2770:
    adr r1, L_278c                          /* @2770 06a1 adr r1, #24 address 0x278C -> 0x0000278C 'Lookup' */
L_2772:
    adr r0, L_2794                          /* @2772 08a0 adr r0, #32 address 0x2794 -> 0x00002794 'Reverse' */
L_2774:
    bl L_4884                               /* @2774 02f086f8 bl #8460 -> display_three_lines */
L_2778:
    b L_2778                                /* @2778 fee7 b #-4 -> 0x2778 */
    .byte 0x00,0x00 /* @277A unclassified */
L_277c:
    .byte 0xc8,0x08,0x00,0x20 /* @277C literal */
L_2780:
    .byte 0x48,0x07,0x00,0x20 /* @2780 literal */
L_2784:
    .byte 0x45,0x72,0x72,0x6f,0x72,0x21,0x00,0x00 /* @2784 unclassified */
L_278c:
    .byte 0x4c,0x6f,0x6f,0x6b,0x75,0x70,0x00,0x00 /* @278C unclassified */
L_2794:
    .byte 0x52,0x65,0x76,0x65,0x72,0x73,0x65,0x00 /* @2794 unclassified */

/* sub_279C */
L_279c:
    push {r4, r5, r6, lr}                   /* @279C 70b5 push {r4, r5, r6, lr}  */
L_279e:
    ldr r3, [pc, #64]                       /* @279E 104b ldr r3, [pc, #64] literal 0x27E0 -> 0x20000748 */
L_27a0:
    ldrh r1, [r3]                           /* @27A0 1988 ldrh r1, [r3]  */
L_27a2:
    ldr r0, [pc, #64]                       /* @27A2 1048 ldr r0, [pc, #64] literal 0x27E4 -> 0x200008C8 */
L_27a4:
    strh r1, [r0, #16]                      /* @27A4 0182 strh r1, [r0, #16]  */
L_27a6:
    ldrh r1, [r3]                           /* @27A6 1988 ldrh r1, [r3]  */
L_27a8:
    strh r1, [r0, #18]                      /* @27A8 4182 strh r1, [r0, #18]  */
L_27aa:
    movs r1, #1                             /* @27AA 0121 movs r1, #1  */
L_27ac:
    lsls r2, r1, #1                         /* @27AC 4a00 lsls r2, r1, #1  */
L_27ae:
    ldrh r4, [r3, r2]                       /* @27AE 9c5a ldrh r4, [r3, r2]  */
L_27b0:
    ldrh r5, [r0, #18]                      /* @27B0 458a ldrh r5, [r0, #18]  */
L_27b2:
    cmp r4, r5                              /* @27B2 ac42 cmp r4, r5  */
L_27b4:
    bhs L_27bc                              /* @27B4 02d2 bhs #4 -> 0x27BC */
L_27b6:
    ldrh r2, [r3, r2]                       /* @27B6 9a5a ldrh r2, [r3, r2]  */
L_27b8:
    strh r2, [r0, #18]                      /* @27B8 4282 strh r2, [r0, #18]  */
L_27ba:
    b L_27c8                                /* @27BA 05e0 b #10 -> 0x27C8 */
L_27bc:
    ldrh r4, [r3, r2]                       /* @27BC 9c5a ldrh r4, [r3, r2]  */
L_27be:
    ldrh r5, [r0, #16]                      /* @27BE 058a ldrh r5, [r0, #16]  */
L_27c0:
    cmp r4, r5                              /* @27C0 ac42 cmp r4, r5  */
L_27c2:
    bls L_27c8                              /* @27C2 01d9 bls #2 -> 0x27C8 */
L_27c4:
    ldrh r2, [r3, r2]                       /* @27C4 9a5a ldrh r2, [r3, r2]  */
L_27c6:
    strh r2, [r0, #16]                      /* @27C6 0282 strh r2, [r0, #16]  */
L_27c8:
    adds r1, r1, #1                         /* @27C8 491c adds r1, r1, #1  */
L_27ca:
    uxth r1, r1                             /* @27CA 89b2 uxth r1, r1  */
L_27cc:
    cmp r1, #200                            /* @27CC c829 cmp r1, #200  */
L_27ce:
    blo L_27ac                              /* @27CE edd3 blo #-38 -> 0x27AC */
L_27d0:
    movs r1, #1                             /* @27D0 0121 movs r1, #1  */
L_27d2:
    strh r1, [r0, #20]                      /* @27D2 8182 strh r1, [r0, #20]  */
L_27d4:
    ldr r0, [pc, #8]                        /* @27D4 0248 ldr r0, [pc, #8] literal 0x27E0 -> 0x20000748 */
L_27d6:
    bl L_46e0                               /* @27D6 01f083ff bl #7942 -> sub_46E0 */
L_27da:
    bl L_0ccc                               /* @27DA fef777fa bl #-6930 -> sub_0CCC */
L_27de:
    pop {r4, r5, r6, pc}                    /* @27DE 70bd pop {r4, r5, r6, pc}  */
L_27e0:
    .byte 0x48,0x07,0x00,0x20 /* @27E0 literal */
L_27e4:
    .byte 0xc8,0x08,0x00,0x20 /* @27E4 literal */

/* update_multiturn_angle */
L_27e8:
    push {r4, r5, r6, lr}                   /* @27E8 70b5 push {r4, r5, r6, lr}  */
L_27ea:
    ldr r0, [pc, #96]                       /* @27EA 1848 ldr r0, [pc, #96] literal 0x284C -> 0x200005DC */
L_27ec:
    ldrb r0, [r0, #6]                       /* @27EC 8079 ldrb r0, [r0, #6]  */
L_27ee:
    cmp r0, #0                              /* @27EE 0028 cmp r0, #0  */
L_27f0:
    beq L_27f8                              /* @27F0 02d0 beq #4 -> 0x27F8 */
L_27f2:
    ldr r0, [pc, #92]                       /* @27F2 1748 ldr r0, [pc, #92] literal 0x2850 -> 0x20000648 */
L_27f4:
    ldrh r0, [r0, #4]                       /* @27F4 8088 ldrh r0, [r0, #4]  */
L_27f6:
    b L_2800                                /* @27F6 03e0 b #6 -> 0x2800 */
L_27f8:
    bl L_51dc                               /* @27F8 02f0f0fc bl #10720 -> sub_51DC */
L_27fc:
    bl L_26cc                               /* @27FC fff766ff bl #-308 -> encoder_lut_read */
L_2800:
    ldr r2, [pc, #80]                       /* @2800 144a ldr r2, [pc, #80] literal 0x2854 -> 0x20000638 */
L_2802:
    ldr r1, [r2]                            /* @2802 1168 ldr r1, [r2]  */
L_2804:
    ldr r3, [r2, #4]                        /* @2804 5368 ldr r3, [r2, #4]  */
L_2806:
    uxth r1, r1                             /* @2806 89b2 uxth r1, r1  */
L_2808:
    subs r1, r0, r1                         /* @2808 411a subs r1, r0, r1  */
L_280a:
    movs r3, #1                             /* @280A 0123 movs r3, #1  */
L_280c:
    lsls r3, r3, #15                        /* @280C db03 lsls r3, r3, #15  */
L_280e:
    cmp r1, r3                              /* @280E 9942 cmp r1, r3  */
L_2810:
    ble L_2822                              /* @2810 07dd ble #14 -> 0x2822 */
L_2812:
    ldr r4, [r2]                            /* @2812 1468 ldr r4, [r2]  */
L_2814:
    ldr r3, [r2, #4]                        /* @2814 5368 ldr r3, [r2, #4]  */
L_2816:
    ldr r6, [pc, #64]                       /* @2816 104e ldr r6, [pc, #64] literal 0x2858 -> 0xFFFF0000 */
L_2818:
    asrs r5, r6, #16                        /* @2818 3514 asrs r5, r6, #16  */
L_281a:
    adds r4, r4, r6                         /* @281A a419 adds r4, r4, r6  */
L_281c:
    adcs r3, r5                             /* @281C 6b41 adcs r3, r5  */
L_281e:
    str r4, [r2]                            /* @281E 1460 str r4, [r2]  */
L_2820:
    str r3, [r2, #4]                        /* @2820 5360 str r3, [r2, #4]  */
L_2822:
    movs r3, #1                             /* @2822 0123 movs r3, #1  */
L_2824:
    lsls r3, r3, #15                        /* @2824 db03 lsls r3, r3, #15  */
L_2826:
    cmn r1, r3                              /* @2826 d942 cmn r1, r3  */
L_2828:
    bge L_283c                              /* @2828 08da bge #16 -> 0x283C */
L_282a:
    ldr r3, [r2]                            /* @282A 1368 ldr r3, [r2]  */
L_282c:
    ldr r1, [r2, #4]                        /* @282C 5168 ldr r1, [r2, #4]  */
L_282e:
    movs r5, #1                             /* @282E 0125 movs r5, #1  */
L_2830:
    lsls r5, r5, #16                        /* @2830 2d04 lsls r5, r5, #16  */
L_2832:
    adds r3, r3, r5                         /* @2832 5b19 adds r3, r3, r5  */
L_2834:
    movs r4, #0                             /* @2834 0024 movs r4, #0  */
L_2836:
    adcs r1, r4                             /* @2836 6141 adcs r1, r4  */
L_2838:
    str r3, [r2]                            /* @2838 1360 str r3, [r2]  */
L_283a:
    str r1, [r2, #4]                        /* @283A 5160 str r1, [r2, #4]  */
L_283c:
    ldr r3, [r2]                            /* @283C 1368 ldr r3, [r2]  */
L_283e:
    ldr r1, [r2, #4]                        /* @283E 5168 ldr r1, [r2, #4]  */
L_2840:
    lsrs r3, r3, #16                        /* @2840 1b0c lsrs r3, r3, #16  */
L_2842:
    lsls r3, r3, #16                        /* @2842 1b04 lsls r3, r3, #16  */
L_2844:
    orrs r3, r0                             /* @2844 0343 orrs r3, r0  */
L_2846:
    str r3, [r2]                            /* @2846 1360 str r3, [r2]  */
L_2848:
    str r1, [r2, #4]                        /* @2848 5160 str r1, [r2, #4]  */
L_284a:
    pop {r4, r5, r6, pc}                    /* @284A 70bd pop {r4, r5, r6, pc}  */
L_284c:
    .byte 0xdc,0x05,0x00,0x20 /* @284C literal */
L_2850:
    .byte 0x48,0x06,0x00,0x20 /* @2850 literal */
L_2854:
    .byte 0x38,0x06,0x00,0x20 /* @2854 literal */
L_2858:
    .byte 0x00,0x00,0xff,0xff /* @2858 literal */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @285C unclassified */

/* sub_286C */
L_286c:
    push {r4, lr}                           /* @286C 10b5 push {r4, lr}  */
L_286e:
    movs r1, #1                             /* @286E 0121 movs r1, #1  */
L_2870:
    movs r0, #30                            /* @2870 1e20 movs r0, #30  */
L_2872:
    bl L_1a40                               /* @2872 fff7e5f8 bl #-3638 -> sub_1A40 */
L_2876:
    movs r1, #1                             /* @2876 0121 movs r1, #1  */
L_2878:
    movs r0, #28                            /* @2878 1c20 movs r0, #28  */
L_287a:
    bl L_1a40                               /* @287A fff7e1f8 bl #-3646 -> sub_1A40 */
L_287e:
    movs r1, #1                             /* @287E 0121 movs r1, #1  */
L_2880:
    movs r0, #6                             /* @2880 0620 movs r0, #6  */
L_2882:
    bl L_1a40                               /* @2882 fff7ddf8 bl #-3654 -> sub_1A40 */
L_2886:
    movs r1, #1                             /* @2886 0121 movs r1, #1  */
L_2888:
    movs r0, #4                             /* @2888 0420 movs r0, #4  */
L_288a:
    bl L_1a40                               /* @288A fff7d9f8 bl #-3662 -> sub_1A40 */
L_288e:
    movs r1, #1                             /* @288E 0121 movs r1, #1  */
L_2890:
    movs r0, #0                             /* @2890 0020 movs r0, #0  */
L_2892:
    bl L_1a40                               /* @2892 fff7d5f8 bl #-3670 -> sub_1A40 */
L_2896:
    movs r1, #1                             /* @2896 0121 movs r1, #1  */
L_2898:
    movs r0, #8                             /* @2898 0820 movs r0, #8  */
L_289a:
    bl L_1a40                               /* @289A fff7d1f8 bl #-3678 -> sub_1A40 */
L_289e:
    movs r1, #1                             /* @289E 0121 movs r1, #1  */
L_28a0:
    movs r0, #10                            /* @28A0 0a20 movs r0, #10  */
L_28a2:
    bl L_1a40                               /* @28A2 fff7cdf8 bl #-3686 -> sub_1A40 */
L_28a6:
    movs r1, #1                             /* @28A6 0121 movs r1, #1  */
L_28a8:
    movs r0, #11                            /* @28A8 0b20 movs r0, #11  */
L_28aa:
    bl L_1a40                               /* @28AA fff7c9f8 bl #-3694 -> sub_1A40 */
L_28ae:
    movs r1, #1                             /* @28AE 0121 movs r1, #1  */
L_28b0:
    movs r0, #16                            /* @28B0 1020 movs r0, #16  */
L_28b2:
    bl L_1a40                               /* @28B2 fff7c5f8 bl #-3702 -> sub_1A40 */
L_28b6:
    movs r1, #1                             /* @28B6 0121 movs r1, #1  */
L_28b8:
    movs r0, #29                            /* @28B8 1d20 movs r0, #29  */
L_28ba:
    bl L_1a40                               /* @28BA fff7c1f8 bl #-3710 -> sub_1A40 */
L_28be:
    movs r1, #1                             /* @28BE 0121 movs r1, #1  */
L_28c0:
    movs r0, #27                            /* @28C0 1b20 movs r0, #27  */
L_28c2:
    bl L_1a40                               /* @28C2 fff7bdf8 bl #-3718 -> sub_1A40 */
L_28c6:
    pop {r4, pc}                            /* @28C6 10bd pop {r4, pc}  */

/* sub_28C8 */
L_28c8:
    push {r3, r4, r5, r6, r7, lr}           /* @28C8 f8b5 push {r3, r4, r5, r6, r7, lr}  */
L_28ca:
    movs r1, #0                             /* @28CA 0021 movs r1, #0  */
L_28cc:
    movs r0, #2                             /* @28CC 0220 movs r0, #2  */
L_28ce:
    bl L_08d8                               /* @28CE fef703f8 bl #-8186 -> sub_08D8 */
L_28d2:
    cmp r0, #1                              /* @28D2 0128 cmp r0, #1  */
L_28d4:
    bne L_2948                              /* @28D4 38d1 bne #112 -> 0x2948 */
L_28d6:
    ldr r5, [pc, #304]                      /* @28D6 4c4d ldr r5, [pc, #304] literal 0x2A08 -> 0x20000013 */
L_28d8:
    ldrb r0, [r5]                           /* @28D8 2878 ldrb r0, [r5]  */
L_28da:
    ldr r6, [pc, #304]                      /* @28DA 4c4e ldr r6, [pc, #304] literal 0x2A0C -> 0x2000072C */
L_28dc:
    movs r4, #0                             /* @28DC 0024 movs r4, #0  */
L_28de:
    cmp r0, #2                              /* @28DE 0228 cmp r0, #2  */
L_28e0:
    bne L_291e                              /* @28E0 1dd1 bne #58 -> 0x291E */
L_28e2:
    ldr r0, [pc, #300]                      /* @28E2 4b48 ldr r0, [pc, #300] literal 0x2A10 -> 0x20000680 */
L_28e4:
    ldr r1, [r0]                            /* @28E4 0168 ldr r1, [r0]  */
L_28e6:
    ldr r2, [r0, #4]                        /* @28E6 4268 ldr r2, [r0, #4]  */
L_28e8:
    ldr r0, [pc, #296]                      /* @28E8 4a48 ldr r0, [pc, #296] literal 0x2A14 -> 0x20000610 */
L_28ea:
    ldr r3, [r0]                            /* @28EA 0368 ldr r3, [r0]  */
L_28ec:
    ldr r0, [r0, #4]                        /* @28EC 4068 ldr r0, [r0, #4]  */
L_28ee:
    eors r1, r3                             /* @28EE 5940 eors r1, r3  */
L_28f0:
    eors r2, r0                             /* @28F0 4240 eors r2, r0  */
L_28f2:
    orrs r1, r2                             /* @28F2 1143 orrs r1, r2  */
L_28f4:
    bne L_291e                              /* @28F4 13d1 bne #38 -> 0x291E */
L_28f6:
    ldr r7, [pc, #288]                      /* @28F6 484f ldr r7, [pc, #288] literal 0x2A18 -> 0x20000018 */
L_28f8:
    ldr r0, [r7]                            /* @28F8 3868 ldr r0, [r7]  */
L_28fa:
    adds r0, r0, #1                         /* @28FA 401c adds r0, r0, #1  */
L_28fc:
    str r0, [r7]                            /* @28FC 3860 str r0, [r7]  */
L_28fe:
    ldr r0, [r7]                            /* @28FE 3868 ldr r0, [r7]  */
L_2900:
    cmp r0, #25                             /* @2900 1928 cmp r0, #25  */
L_2902:
    bne L_2916                              /* @2902 08d1 bne #16 -> 0x2916 */
L_2904:
    ldr r0, [pc, #276]                      /* @2904 4548 ldr r0, [pc, #276] literal 0x2A1C -> 0x200006A8 */
L_2906:
    movs r2, #2                             /* @2906 0222 movs r2, #2  */
L_2908:
    strb r2, [r0, #13]                      /* @2908 4273 strb r2, [r0, #13]  */
L_290a:
    ldrh r1, [r6, #20]                      /* @290A b18a ldrh r1, [r6, #20]  */
L_290c:
    adds r1, r1, #2                         /* @290C 891c adds r1, r1, #2  */
L_290e:
    strb r1, [r0, #12]                      /* @290E 0173 strb r1, [r0, #12]  */
L_2910:
    strb r2, [r0, #2]                       /* @2910 8270 strb r2, [r0, #2]  */
L_2912:
    bl L_2334                               /* @2912 fff70ffd bl #-1506 -> uart_start_reply */
L_2916:
    ldr r0, [r7]                            /* @2916 3868 ldr r0, [r7]  */
L_2918:
    cmp r0, #50                             /* @2918 3228 cmp r0, #50  */
L_291a:
    blo L_291e                              /* @291A 00d3 blo #0 -> 0x291E */
L_291c:
    strb r4, [r5]                           /* @291C 2c70 strb r4, [r5]  */
L_291e:
    ldrb r0, [r6, #9]                       /* @291E 707a ldrb r0, [r6, #9]  */
L_2920:
    cmp r0, #2                              /* @2920 0228 cmp r0, #2  */
L_2922:
    beq L_294e                              /* @2922 14d0 beq #40 -> 0x294E */
L_2924:
    ldrb r1, [r6, #10]                      /* @2924 b17a ldrb r1, [r6, #10]  */
L_2926:
    ldr r0, [pc, #248]                      /* @2926 3e48 ldr r0, [pc, #248] literal 0x2A20 -> 0x200005DC */
L_2928:
    cmp r1, #1                              /* @2928 0129 cmp r1, #1  */
L_292a:
    bne L_294a                              /* @292A 0ed1 bne #28 -> 0x294A */
L_292c:
    ldr r1, [pc, #244]                      /* @292C 3d49 ldr r1, [pc, #244] literal 0x2A24 -> 0x20000628 */
L_292e:
    ldr r2, [pc, #248]                      /* @292E 3e4a ldr r2, [pc, #248] literal 0x2A28 -> 0x20000358 */
L_2930:
    ldr r1, [r1, #4]                        /* @2930 4968 ldr r1, [r1, #4]  */
L_2932:
    cmp r1, r2                              /* @2932 9142 cmp r1, r2  */
L_2934:
    bne L_294a                              /* @2934 09d1 bne #18 -> 0x294A */
L_2936:
    ldrh r1, [r0, #4]                       /* @2936 8188 ldrh r1, [r0, #4]  */
L_2938:
    adds r1, r1, #1                         /* @2938 491c adds r1, r1, #1  */
L_293a:
    strh r1, [r0, #4]                       /* @293A 8180 strh r1, [r0, #4]  */
L_293c:
    ldrh r2, [r0, #4]                       /* @293C 8288 ldrh r2, [r0, #4]  */
L_293e:
    movs r1, #1                             /* @293E 0121 movs r1, #1  */
L_2940:
    lsls r1, r1, #12                        /* @2940 0903 lsls r1, r1, #12  */
L_2942:
    cmp r2, r1                              /* @2942 8a42 cmp r2, r1  */
L_2944:
    bls L_29fe                              /* @2944 5bd9 bls #182 -> 0x29FE */
L_2946:
    strh r1, [r0, #4]                       /* @2946 8180 strh r1, [r0, #4]  */
L_2948:
    b L_29fe                                /* @2948 59e0 b #178 -> 0x29FE */
L_294a:
    strh r4, [r0, #4]                       /* @294A 8480 strh r4, [r0, #4]  */
L_294c:
    b L_29fe                                /* @294C 57e0 b #174 -> 0x29FE */
L_294e:
    ldrb r1, [r5]                           /* @294E 2978 ldrb r1, [r5]  */
L_2950:
    ldr r0, [pc, #216]                      /* @2950 3648 ldr r0, [pc, #216] literal 0x2A2C -> 0x20000010 */
L_2952:
    ldr r6, [pc, #220]                      /* @2952 374e ldr r6, [pc, #220] literal 0x2A30 -> 0x20000618 */
L_2954:
    cmp r1, #1                              /* @2954 0129 cmp r1, #1  */
L_2956:
    beq L_2960                              /* @2956 03d0 beq #6 -> 0x2960 */
L_2958:
    ldrb r1, [r5]                           /* @2958 2978 ldrb r1, [r5]  */
L_295a:
    cmp r1, #2                              /* @295A 0229 cmp r1, #2  */
L_295c:
    beq L_297c                              /* @295C 0ed0 beq #28 -> 0x297C */
L_295e:
    b L_29fe                                /* @295E 4ee0 b #156 -> 0x29FE */
L_2960:
    ldr r1, [pc, #208]                      /* @2960 3449 ldr r1, [pc, #208] literal 0x2A34 -> 0x20000608 */
L_2962:
    ldrb r1, [r1]                           /* @2962 0978 ldrb r1, [r1]  */
L_2964:
    cmp r1, #0                              /* @2964 0029 cmp r1, #0  */
L_2966:
    beq L_2972                              /* @2966 04d0 beq #8 -> 0x2972 */
L_2968:
    ldrh r1, [r6]                           /* @2968 3188 ldrh r1, [r6]  */
L_296a:
    ldrh r2, [r0]                           /* @296A 0288 ldrh r2, [r0]  */
L_296c:
    adds r1, r1, r2                         /* @296C 8918 adds r1, r1, r2  */
L_296e:
    strh r1, [r0]                           /* @296E 0180 strh r1, [r0]  */
L_2970:
    b L_29fe                                /* @2970 45e0 b #138 -> 0x29FE */
L_2972:
    ldrh r1, [r6]                           /* @2972 3188 ldrh r1, [r6]  */
L_2974:
    ldrh r2, [r0]                           /* @2974 0288 ldrh r2, [r0]  */
L_2976:
    subs r1, r2, r1                         /* @2976 511a subs r1, r2, r1  */
L_2978:
    strh r1, [r0]                           /* @2978 0180 strh r1, [r0]  */
L_297a:
    b L_29fe                                /* @297A 40e0 b #128 -> 0x29FE */
L_297c:
    ldr r1, [pc, #144]                      /* @297C 2449 ldr r1, [pc, #144] literal 0x2A10 -> 0x20000680 */
L_297e:
    ldr r2, [r1]                            /* @297E 0a68 ldr r2, [r1]  */
L_2980:
    ldr r3, [r1, #4]                        /* @2980 4b68 ldr r3, [r1, #4]  */
L_2982:
    ldr r1, [pc, #144]                      /* @2982 2449 ldr r1, [pc, #144] literal 0x2A14 -> 0x20000610 */
L_2984:
    ldr r5, [r1]                            /* @2984 0d68 ldr r5, [r1]  */
L_2986:
    ldr r7, [r1, #4]                        /* @2986 4f68 ldr r7, [r1, #4]  */
L_2988:
    eors r2, r5                             /* @2988 6a40 eors r2, r5  */
L_298a:
    eors r3, r7                             /* @298A 7b40 eors r3, r7  */
L_298c:
    orrs r2, r3                             /* @298C 1a43 orrs r2, r3  */
L_298e:
    beq L_29fe                              /* @298E 36d0 beq #108 -> 0x29FE */
L_2990:
    ldr r2, [pc, #132]                      /* @2990 214a ldr r2, [pc, #132] literal 0x2A18 -> 0x20000018 */
L_2992:
    str r4, [r2]                            /* @2992 1460 str r4, [r2]  */
L_2994:
    ldr r2, [pc, #156]                      /* @2994 274a ldr r2, [pc, #156] literal 0x2A34 -> 0x20000608 */
L_2996:
    ldrb r2, [r2]                           /* @2996 1278 ldrb r2, [r2]  */
L_2998:
    cmp r2, #0                              /* @2998 002a cmp r2, #0  */
L_299a:
    beq L_29ce                              /* @299A 18d0 beq #48 -> 0x29CE */
L_299c:
    ldr r7, [pc, #112]                      /* @299C 1c4f ldr r7, [pc, #112] literal 0x2A10 -> 0x20000680 */
L_299e:
    ldr r5, [r7]                            /* @299E 3d68 ldr r5, [r7]  */
L_29a0:
    ldr r2, [r7, #4]                        /* @29A0 7a68 ldr r2, [r7, #4]  */
L_29a2:
    ldrh r3, [r6]                           /* @29A2 3388 ldrh r3, [r6]  */
L_29a4:
    adds r5, r5, r3                         /* @29A4 ed18 adds r5, r5, r3  */
L_29a6:
    adcs r2, r4                             /* @29A6 6241 adcs r2, r4  */
L_29a8:
    ldr r4, [r1]                            /* @29A8 0c68 ldr r4, [r1]  */
L_29aa:
    ldr r3, [r1, #4]                        /* @29AA 4b68 ldr r3, [r1, #4]  */
L_29ac:
    subs r4, r4, r5                         /* @29AC 641b subs r4, r4, r5  */
L_29ae:
    sbcs r3, r2                             /* @29AE 9341 sbcs r3, r2  */
L_29b0:
    blt L_29bc                              /* @29B0 04db blt #8 -> 0x29BC */
L_29b2:
    ldrh r1, [r6]                           /* @29B2 3188 ldrh r1, [r6]  */
L_29b4:
    ldrh r2, [r0]                           /* @29B4 0288 ldrh r2, [r0]  */
L_29b6:
    adds r1, r1, r2                         /* @29B6 8918 adds r1, r1, r2  */
L_29b8:
    strh r1, [r0]                           /* @29B8 0180 strh r1, [r0]  */
L_29ba:
    b L_29fe                                /* @29BA 20e0 b #64 -> 0x29FE */
L_29bc:
    ldr r2, [r7]                            /* @29BC 3a68 ldr r2, [r7]  */
L_29be:
    ldr r3, [r7, #4]                        /* @29BE 7b68 ldr r3, [r7, #4]  */
L_29c0:
    ldr r3, [r1]                            /* @29C0 0b68 ldr r3, [r1]  */
L_29c2:
    ldr r1, [r1, #4]                        /* @29C2 4968 ldr r1, [r1, #4]  */
L_29c4:
    ldrh r1, [r0]                           /* @29C4 0188 ldrh r1, [r0]  */
L_29c6:
    subs r2, r3, r2                         /* @29C6 9a1a subs r2, r3, r2  */
L_29c8:
    adds r1, r2, r1                         /* @29C8 5118 adds r1, r2, r1  */
L_29ca:
    strh r1, [r0]                           /* @29CA 0180 strh r1, [r0]  */
L_29cc:
    b L_29fe                                /* @29CC 17e0 b #46 -> 0x29FE */
L_29ce:
    ldr r7, [pc, #64]                       /* @29CE 104f ldr r7, [pc, #64] literal 0x2A10 -> 0x20000680 */
L_29d0:
    ldr r5, [r7]                            /* @29D0 3d68 ldr r5, [r7]  */
L_29d2:
    ldr r2, [r7, #4]                        /* @29D2 7a68 ldr r2, [r7, #4]  */
L_29d4:
    ldrh r3, [r6]                           /* @29D4 3388 ldrh r3, [r6]  */
L_29d6:
    subs r3, r5, r3                         /* @29D6 eb1a subs r3, r5, r3  */
L_29d8:
    sbcs r2, r4                             /* @29D8 a241 sbcs r2, r4  */
L_29da:
    ldr r4, [r1]                            /* @29DA 0c68 ldr r4, [r1]  */
L_29dc:
    ldr r5, [r1, #4]                        /* @29DC 4d68 ldr r5, [r1, #4]  */
L_29de:
    subs r3, r3, r4                         /* @29DE 1b1b subs r3, r3, r4  */
L_29e0:
    sbcs r2, r5                             /* @29E0 aa41 sbcs r2, r5  */
L_29e2:
    blt L_29ee                              /* @29E2 04db blt #8 -> 0x29EE */
L_29e4:
    ldrh r1, [r6]                           /* @29E4 3188 ldrh r1, [r6]  */
L_29e6:
    ldrh r2, [r0]                           /* @29E6 0288 ldrh r2, [r0]  */
L_29e8:
    subs r1, r2, r1                         /* @29E8 511a subs r1, r2, r1  */
L_29ea:
    strh r1, [r0]                           /* @29EA 0180 strh r1, [r0]  */
L_29ec:
    b L_29fe                                /* @29EC 07e0 b #14 -> 0x29FE */
L_29ee:
    ldr r3, [r7]                            /* @29EE 3b68 ldr r3, [r7]  */
L_29f0:
    ldr r2, [r7, #4]                        /* @29F0 7a68 ldr r2, [r7, #4]  */
L_29f2:
    ldr r2, [r1]                            /* @29F2 0a68 ldr r2, [r1]  */
L_29f4:
    ldr r1, [r1, #4]                        /* @29F4 4968 ldr r1, [r1, #4]  */
L_29f6:
    ldrh r1, [r0]                           /* @29F6 0188 ldrh r1, [r0]  */
L_29f8:
    subs r2, r3, r2                         /* @29F8 9a1a subs r2, r3, r2  */
L_29fa:
    subs r1, r1, r2                         /* @29FA 891a subs r1, r1, r2  */
L_29fc:
    strh r1, [r0]                           /* @29FC 0180 strh r1, [r0]  */
L_29fe:
    movs r1, #0                             /* @29FE 0021 movs r1, #0  */
L_2a00:
    movs r0, #2                             /* @2A00 0220 movs r0, #2  */
L_2a02:
    bl L_08c0                               /* @2A02 fdf75dff bl #-8518 -> sub_08C0 */
L_2a06:
    pop {r3, r4, r5, r6, r7, pc}            /* @2A06 f8bd pop {r3, r4, r5, r6, r7, pc}  */
L_2a08:
    .byte 0x13,0x00,0x00,0x20 /* @2A08 literal */
L_2a0c:
    .byte 0x2c,0x07,0x00,0x20 /* @2A0C literal */
L_2a10:
    .byte 0x80,0x06,0x00,0x20 /* @2A10 literal */
L_2a14:
    .byte 0x10,0x06,0x00,0x20 /* @2A14 literal */
L_2a18:
    .byte 0x18,0x00,0x00,0x20 /* @2A18 literal */
L_2a1c:
    .byte 0xa8,0x06,0x00,0x20 /* @2A1C literal */
L_2a20:
    .byte 0xdc,0x05,0x00,0x20 /* @2A20 literal */
L_2a24:
    .byte 0x28,0x06,0x00,0x20 /* @2A24 literal */
L_2a28:
    .byte 0x58,0x03,0x00,0x20 /* @2A28 literal */
L_2a2c:
    .byte 0x10,0x00,0x00,0x20 /* @2A2C literal */
L_2a30:
    .byte 0x18,0x06,0x00,0x20 /* @2A30 literal */
L_2a34:
    .byte 0x08,0x06,0x00,0x20 /* @2A34 literal */

/* uart0_irq */
L_2a38:
    push {r3, r4, r5, r6, r7, lr}           /* @2A38 f8b5 push {r3, r4, r5, r6, r7, lr}  */
L_2a3a:
    movs r6, #1                             /* @2A3A 0126 movs r6, #1  */
L_2a3c:
    lsls r6, r6, #30                        /* @2A3C b607 lsls r6, r6, #30  */
L_2a3e:
    ldr r0, [r6, #16]                       /* @2A3E 3069 ldr r0, [r6, #16]  */
L_2a40:
    ldr r5, [pc, #468]                      /* @2A40 754d ldr r5, [pc, #468] literal 0x2C18 -> 0x200006A8 */
L_2a42:
    lsls r0, r0, #30                        /* @2A42 8007 lsls r0, r0, #30  */
L_2a44:
    bpl L_2a8e                              /* @2A44 23d5 bpl #70 -> 0x2A8E */
L_2a46:
    ldrb r0, [r5, #2]                       /* @2A46 a878 ldrb r0, [r5, #2]  */
L_2a48:
    cmp r0, #0                              /* @2A48 0028 cmp r0, #0  */
L_2a4a:
    beq L_2a66                              /* @2A4A 0cd0 beq #24 -> 0x2A66 */
L_2a4c:
    ldrb r0, [r5, #2]                       /* @2A4C a878 ldrb r0, [r5, #2]  */
L_2a4e:
    subs r0, r0, #1                         /* @2A4E 401e subs r0, r0, #1  */
L_2a50:
    strb r0, [r5, #2]                       /* @2A50 a870 strb r0, [r5, #2]  */
L_2a52:
    ldrb r1, [r5, #2]                       /* @2A52 a978 ldrb r1, [r5, #2]  */
L_2a54:
    mov r0, r5                              /* @2A54 2846 mov r0, r5  */
L_2a56:
    adds r0, #12                            /* @2A56 0c30 adds r0, #12  */
L_2a58:
    ldrb r0, [r1, r0]                       /* @2A58 085c ldrb r0, [r1, r0]  */
L_2a5a:
    ldr r1, [r6]                            /* @2A5A 3168 ldr r1, [r6]  */
L_2a5c:
    lsrs r1, r1, #8                         /* @2A5C 090a lsrs r1, r1, #8  */
L_2a5e:
    lsls r1, r1, #8                         /* @2A5E 0902 lsls r1, r1, #8  */
L_2a60:
    orrs r1, r0                             /* @2A60 0143 orrs r1, r0  */
L_2a62:
    str r1, [r6]                            /* @2A62 3160 str r1, [r6]  */
L_2a64:
    b L_2a84                                /* @2A64 0ee0 b #28 -> 0x2A84 */
L_2a66:
    ldr r0, [pc, #436]                      /* @2A66 6d48 ldr r0, [pc, #436] literal 0x2C1C -> 0x40020C00 */
L_2a68:
    ldr r1, [r0, #56]                       /* @2A68 816b ldr r1, [r0, #56]  */
L_2a6a:
    lsrs r1, r1, #3                         /* @2A6A c908 lsrs r1, r1, #3  */
L_2a6c:
    lsls r1, r1, #3                         /* @2A6C c900 lsls r1, r1, #3  */
L_2a6e:
    str r1, [r0, #56]                       /* @2A6E 8163 str r1, [r0, #56]  */
L_2a70:
    ldr r0, [pc, #428]                      /* @2A70 6b48 ldr r0, [pc, #428] literal 0x2C20 -> 0x40020D00 */
L_2a72:
    ldr r1, [r0]                            /* @2A72 0168 ldr r1, [r0]  */
L_2a74:
    movs r2, #1                             /* @2A74 0122 movs r2, #1  */
L_2a76:
    lsls r2, r2, #14                        /* @2A76 9203 lsls r2, r2, #14  */
L_2a78:
    orrs r1, r2                             /* @2A78 1143 orrs r1, r2  */
L_2a7a:
    str r1, [r0]                            /* @2A7A 0160 str r1, [r0]  */
L_2a7c:
    bl L_0d3c                               /* @2A7C fef75ef9 bl #-7492 -> sub_0D3C */
L_2a80:
    bl L_5ce8                               /* @2A80 03f032f9 bl #12900 -> sub_5CE8 */
L_2a84:
    ldr r0, [r6, #20]                       /* @2A84 7069 ldr r0, [r6, #20]  */
L_2a86:
    movs r1, #2                             /* @2A86 0221 movs r1, #2  */
L_2a88:
    bics r0, r1                             /* @2A88 8843 bics r0, r1  */
L_2a8a:
    str r0, [r6, #20]                       /* @2A8A 7061 str r0, [r6, #20]  */
L_2a8c:
    pop {r3, r4, r5, r6, r7, pc}            /* @2A8C f8bd pop {r3, r4, r5, r6, r7, pc}  */
L_2a8e:
    ldr r0, [r6, #16]                       /* @2A8E 3069 ldr r0, [r6, #16]  */
L_2a90:
    lsls r0, r0, #31                        /* @2A90 c007 lsls r0, r0, #31  */
L_2a92:
    beq L_2a8c                              /* @2A92 fbd0 beq #-10 -> 0x2A8C */
L_2a94:
    ldr r0, [r6]                            /* @2A94 3068 ldr r0, [r6]  */
L_2a96:
    str r0, [sp]                            /* @2A96 0090 str r0, [sp]  */
L_2a98:
    ldrb r0, [r5, #1]                       /* @2A98 6878 ldrb r0, [r5, #1]  */
L_2a9a:
    cmp r0, #0                              /* @2A9A 0028 cmp r0, #0  */
L_2a9c:
    bne L_2b7e                              /* @2A9C 6fd1 bne #222 -> 0x2B7E */
L_2a9e:
    ldr r1, [pc, #388]                      /* @2A9E 6149 ldr r1, [pc, #388] literal 0x2C24 -> 0x2000072C */
L_2aa0:
    ldrh r2, [r1, #20]                      /* @2AA0 8a8a ldrh r2, [r1, #20]  */
L_2aa2:
    mov r0, sp                              /* @2AA2 6846 mov r0, sp  */
L_2aa4:
    ldrb r3, [r0]                           /* @2AA4 0378 ldrb r3, [r0]  */
L_2aa6:
    ldr r0, [pc, #368]                      /* @2AA6 5c48 ldr r0, [pc, #368] literal 0x2C18 -> 0x200006A8 */
L_2aa8:
    movs r4, #0                             /* @2AA8 0024 movs r4, #0  */
L_2aaa:
    adds r0, r0, #3                         /* @2AAA c01c adds r0, r0, #3  */
L_2aac:
    cmp r2, r3                              /* @2AAC 9a42 cmp r2, r3  */
L_2aae:
    bne L_2af2                              /* @2AAE 20d1 bne #64 -> 0x2AF2 */
L_2ab0:
    ldrb r2, [r5]                           /* @2AB0 2a78 ldrb r2, [r5]  */
L_2ab2:
    cmp r2, #2                              /* @2AB2 022a cmp r2, #2  */
L_2ab4:
    bhs L_2aba                              /* @2AB4 01d2 bhs #2 -> 0x2ABA */
L_2ab6:
    strb r4, [r5]                           /* @2AB6 2c70 strb r4, [r5]  */
L_2ab8:
    b L_2ae2                                /* @2AB8 13e0 b #38 -> 0x2AE2 */
L_2aba:
    ldrh r1, [r1, #20]                      /* @2ABA 898a ldrh r1, [r1, #20]  */
L_2abc:
    ldrb r2, [r5, #3]                       /* @2ABC ea78 ldrb r2, [r5, #3]  */
L_2abe:
    cmp r1, r2                              /* @2ABE 9142 cmp r1, r2  */
L_2ac0:
    bne L_2ae0                              /* @2AC0 0ed1 bne #28 -> 0x2AE0 */
L_2ac2:
    ldrb r1, [r5, #4]                       /* @2AC2 2979 ldrb r1, [r5, #4]  */
L_2ac4:
    cmp r1, #132                            /* @2AC4 8429 cmp r1, #132  */
L_2ac6:
    beq L_2ae2                              /* @2AC6 0cd0 beq #24 -> 0x2AE2 */
L_2ac8:
    ldrb r1, [r5, #4]                       /* @2AC8 2979 ldrb r1, [r5, #4]  */
L_2aca:
    lsrs r1, r1, #4                         /* @2ACA 0909 lsrs r1, r1, #4  */
L_2acc:
    cmp r1, #10                             /* @2ACC 0a29 cmp r1, #10  */
L_2ace:
    beq L_2ae2                              /* @2ACE 08d0 beq #16 -> 0x2AE2 */
L_2ad0:
    ldrb r1, [r5, #4]                       /* @2AD0 2979 ldrb r1, [r5, #4]  */
L_2ad2:
    cmp r1, #246                            /* @2AD2 f629 cmp r1, #246  */
L_2ad4:
    beq L_2ae2                              /* @2AD4 05d0 beq #10 -> 0x2AE2 */
L_2ad6:
    ldrb r1, [r5, #4]                       /* @2AD6 2979 ldrb r1, [r5, #4]  */
L_2ad8:
    cmp r1, #253                            /* @2AD8 fd29 cmp r1, #253  */
L_2ada:
    beq L_2ae2                              /* @2ADA 02d0 beq #4 -> 0x2AE2 */
L_2adc:
    strb r4, [r5]                           /* @2ADC 2c70 strb r4, [r5]  */
L_2ade:
    b L_2ae2                                /* @2ADE 00e0 b #0 -> 0x2AE2 */
L_2ae0:
    strb r4, [r5]                           /* @2AE0 2c70 strb r4, [r5]  */
L_2ae2:
    mov r1, sp                              /* @2AE2 6946 mov r1, sp  */
L_2ae4:
    ldrb r1, [r1]                           /* @2AE4 0978 ldrb r1, [r1]  */
L_2ae6:
    ldrb r2, [r5]                           /* @2AE6 2a78 ldrb r2, [r5]  */
L_2ae8:
    strb r1, [r2, r0]                       /* @2AE8 1154 strb r1, [r2, r0]  */
L_2aea:
    ldrb r0, [r5]                           /* @2AEA 2878 ldrb r0, [r5]  */
L_2aec:
    adds r0, r0, #1                         /* @2AEC 401c adds r0, r0, #1  */
L_2aee:
    strb r0, [r5]                           /* @2AEE 2870 strb r0, [r5]  */
L_2af0:
    b L_2b0c                                /* @2AF0 0ce0 b #24 -> 0x2B0C */
L_2af2:
    ldrh r1, [r1, #20]                      /* @2AF2 898a ldrh r1, [r1, #20]  */
L_2af4:
    ldrb r2, [r5, #3]                       /* @2AF4 ea78 ldrb r2, [r5, #3]  */
L_2af6:
    cmp r1, r2                              /* @2AF6 9142 cmp r1, r2  */
L_2af8:
    bne L_2b0a                              /* @2AF8 07d1 bne #14 -> 0x2B0A */
L_2afa:
    mov r1, sp                              /* @2AFA 6946 mov r1, sp  */
L_2afc:
    ldrb r1, [r1]                           /* @2AFC 0978 ldrb r1, [r1]  */
L_2afe:
    ldrb r2, [r5]                           /* @2AFE 2a78 ldrb r2, [r5]  */
L_2b00:
    strb r1, [r2, r0]                       /* @2B00 1154 strb r1, [r2, r0]  */
L_2b02:
    ldrb r0, [r5]                           /* @2B02 2878 ldrb r0, [r5]  */
L_2b04:
    adds r0, r0, #1                         /* @2B04 401c adds r0, r0, #1  */
L_2b06:
    strb r0, [r5]                           /* @2B06 2870 strb r0, [r5]  */
L_2b08:
    b L_2b0c                                /* @2B08 00e0 b #0 -> 0x2B0C */
L_2b0a:
    strb r4, [r5]                           /* @2B0A 2c70 strb r4, [r5]  */
L_2b0c:
    ldrb r1, [r5]                           /* @2B0C 2978 ldrb r1, [r5]  */
L_2b0e:
    movs r0, #1                             /* @2B0E 0120 movs r0, #1  */
L_2b10:
    movs r3, r1                             /* @2B10 0b00 movs r3, r1  */
L_2b12:
    bl L_15ec                               /* @2B12 fef76bfd bl #-5418 -> switch_u8_dispatch ; inline switch destinations 0x2BEC,0x2BEC,0x2BEC,0x2B22,0x2B46,0x2B84,0x2BB4,0x2BB4,0x2BBE,0x2BE8 */
    .byte 0x09,0x6b,0x6b,0x6b,0x06,0x18,0x37,0x4f,0x4f,0x54,0x69 /* @2B16 switch_table */
    .byte 0x00 /* @2B21 unclassified */
L_2b22:
    ldrb r1, [r5, #4]                       /* @2B22 2979 ldrb r1, [r5, #4]  */
L_2b24:
    lsrs r1, r1, #4                         /* @2B24 0909 lsrs r1, r1, #4  */
L_2b26:
    cmp r1, #3                              /* @2B26 0329 cmp r1, #3  */
L_2b28:
    beq L_2b30                              /* @2B28 02d0 beq #4 -> 0x2B30 */
L_2b2a:
    ldrb r1, [r5, #4]                       /* @2B2A 2979 ldrb r1, [r5, #4]  */
L_2b2c:
    cmp r1, #247                            /* @2B2C f729 cmp r1, #247  */
L_2b2e:
    bne L_2bec                              /* @2B2E 5dd1 bne #186 -> 0x2BEC */
L_2b30:
    ldrb r1, [r5, #3]                       /* @2B30 e978 ldrb r1, [r5, #3]  */
L_2b32:
    ldrb r2, [r5, #4]                       /* @2B32 2a79 ldrb r2, [r5, #4]  */
L_2b34:
    adds r1, r1, r2                         /* @2B34 8918 adds r1, r1, r2  */
L_2b36:
    ldrb r2, [r5, #5]                       /* @2B36 6a79 ldrb r2, [r5, #5]  */
L_2b38:
    uxtb r1, r1                             /* @2B38 c9b2 uxtb r1, r1  */
L_2b3a:
    cmp r1, r2                              /* @2B3A 9142 cmp r1, r2  */
L_2b3c:
    bne L_2b42                              /* @2B3C 01d1 bne #2 -> 0x2B42 */
L_2b3e:
    strb r0, [r5, #1]                       /* @2B3E 6870 strb r0, [r5, #1]  */
L_2b40:
    b L_2bec                                /* @2B40 54e0 b #168 -> 0x2BEC */
L_2b42:
    strb r4, [r5]                           /* @2B42 2c70 strb r4, [r5]  */
L_2b44:
    b L_2bec                                /* @2B44 52e0 b #164 -> 0x2BEC */
L_2b46:
    ldrb r1, [r5, #4]                       /* @2B46 2979 ldrb r1, [r5, #4]  */
L_2b48:
    lsrs r1, r1, #4                         /* @2B48 0909 lsrs r1, r1, #4  */
L_2b4a:
    cmp r1, #8                              /* @2B4A 0829 cmp r1, #8  */
L_2b4c:
    beq L_2b68                              /* @2B4C 0cd0 beq #24 -> 0x2B68 */
L_2b4e:
    ldrb r1, [r5, #4]                       /* @2B4E 2979 ldrb r1, [r5, #4]  */
L_2b50:
    lsrs r1, r1, #4                         /* @2B50 0909 lsrs r1, r1, #4  */
L_2b52:
    cmp r1, #9                              /* @2B52 0929 cmp r1, #9  */
L_2b54:
    beq L_2b68                              /* @2B54 08d0 beq #16 -> 0x2B68 */
L_2b56:
    ldrb r1, [r5, #4]                       /* @2B56 2979 ldrb r1, [r5, #4]  */
L_2b58:
    cmp r1, #243                            /* @2B58 f329 cmp r1, #243  */
L_2b5a:
    beq L_2b68                              /* @2B5A 05d0 beq #10 -> 0x2B68 */
L_2b5c:
    ldrb r1, [r5, #4]                       /* @2B5C 2979 ldrb r1, [r5, #4]  */
L_2b5e:
    cmp r1, #246                            /* @2B5E f629 cmp r1, #246  */
L_2b60:
    beq L_2b68                              /* @2B60 02d0 beq #4 -> 0x2B68 */
L_2b62:
    ldrb r1, [r5, #4]                       /* @2B62 2979 ldrb r1, [r5, #4]  */
L_2b64:
    cmp r1, #255                            /* @2B64 ff29 cmp r1, #255  */
L_2b66:
    bne L_2bec                              /* @2B66 41d1 bne #130 -> 0x2BEC */
L_2b68:
    ldrb r1, [r5, #3]                       /* @2B68 e978 ldrb r1, [r5, #3]  */
L_2b6a:
    ldrb r2, [r5, #4]                       /* @2B6A 2a79 ldrb r2, [r5, #4]  */
L_2b6c:
    adds r1, r1, r2                         /* @2B6C 8918 adds r1, r1, r2  */
L_2b6e:
    ldrb r2, [r5, #5]                       /* @2B6E 6a79 ldrb r2, [r5, #5]  */
L_2b70:
    adds r1, r1, r2                         /* @2B70 8918 adds r1, r1, r2  */
L_2b72:
    ldrb r2, [r5, #6]                       /* @2B72 aa79 ldrb r2, [r5, #6]  */
L_2b74:
    uxtb r1, r1                             /* @2B74 c9b2 uxtb r1, r1  */
L_2b76:
    cmp r1, r2                              /* @2B76 9142 cmp r1, r2  */
L_2b78:
    bne L_2b80                              /* @2B78 02d1 bne #4 -> 0x2B80 */
L_2b7a:
    strb r0, [r5, #1]                       /* @2B7A 6870 strb r0, [r5, #1]  */
L_2b7c:
    b L_2bec                                /* @2B7C 36e0 b #108 -> 0x2BEC */
L_2b7e:
    b L_2c0c                                /* @2B7E 45e0 b #138 -> 0x2C0C */
L_2b80:
    strb r4, [r5]                           /* @2B80 2c70 strb r4, [r5]  */
L_2b82:
    b L_2bec                                /* @2B82 33e0 b #102 -> 0x2BEC */
L_2b84:
    ldrb r1, [r5, #4]                       /* @2B84 2979 ldrb r1, [r5, #4]  */
L_2b86:
    lsrs r1, r1, #4                         /* @2B86 0909 lsrs r1, r1, #4  */
L_2b88:
    cmp r1, #10                             /* @2B88 0a29 cmp r1, #10  */
L_2b8a:
    beq L_2b96                              /* @2B8A 04d0 beq #8 -> 0x2B96 */
L_2b8c:
    ldrb r0, [r5, #4]                       /* @2B8C 2879 ldrb r0, [r5, #4]  */
L_2b8e:
    cmp r0, #253                            /* @2B8E fd28 cmp r0, #253  */
L_2b90:
    beq L_2bec                              /* @2B90 2cd0 beq #88 -> 0x2BEC */
L_2b92:
    strb r4, [r5]                           /* @2B92 2c70 strb r4, [r5]  */
L_2b94:
    b L_2bec                                /* @2B94 2ae0 b #84 -> 0x2BEC */
L_2b96:
    ldrb r1, [r5, #3]                       /* @2B96 e978 ldrb r1, [r5, #3]  */
L_2b98:
    ldrb r2, [r5, #4]                       /* @2B98 2a79 ldrb r2, [r5, #4]  */
L_2b9a:
    adds r1, r1, r2                         /* @2B9A 8918 adds r1, r1, r2  */
L_2b9c:
    ldrb r2, [r5, #5]                       /* @2B9C 6a79 ldrb r2, [r5, #5]  */
L_2b9e:
    ldrb r3, [r5, #6]                       /* @2B9E ab79 ldrb r3, [r5, #6]  */
L_2ba0:
    adds r2, r2, r3                         /* @2BA0 d218 adds r2, r2, r3  */
L_2ba2:
    adds r1, r1, r2                         /* @2BA2 8918 adds r1, r1, r2  */
L_2ba4:
    ldrb r2, [r5, #7]                       /* @2BA4 ea79 ldrb r2, [r5, #7]  */
L_2ba6:
    uxtb r1, r1                             /* @2BA6 c9b2 uxtb r1, r1  */
L_2ba8:
    cmp r1, r2                              /* @2BA8 9142 cmp r1, r2  */
L_2baa:
    bne L_2bb0                              /* @2BAA 01d1 bne #2 -> 0x2BB0 */
L_2bac:
    strb r0, [r5, #1]                       /* @2BAC 6870 strb r0, [r5, #1]  */
L_2bae:
    b L_2bec                                /* @2BAE 1de0 b #58 -> 0x2BEC */
L_2bb0:
    strb r4, [r5]                           /* @2BB0 2c70 strb r4, [r5]  */
L_2bb2:
    b L_2bec                                /* @2BB2 1be0 b #54 -> 0x2BEC */
L_2bb4:
    ldrb r0, [r5, #4]                       /* @2BB4 2879 ldrb r0, [r5, #4]  */
L_2bb6:
    cmp r0, #253                            /* @2BB6 fd28 cmp r0, #253  */
L_2bb8:
    beq L_2bec                              /* @2BB8 18d0 beq #48 -> 0x2BEC */
L_2bba:
    strb r4, [r5]                           /* @2BBA 2c70 strb r4, [r5]  */
L_2bbc:
    b L_2bec                                /* @2BBC 16e0 b #44 -> 0x2BEC */
L_2bbe:
    ldrb r1, [r5, #3]                       /* @2BBE e978 ldrb r1, [r5, #3]  */
L_2bc0:
    ldrb r2, [r5, #4]                       /* @2BC0 2a79 ldrb r2, [r5, #4]  */
L_2bc2:
    adds r1, r1, r2                         /* @2BC2 8918 adds r1, r1, r2  */
L_2bc4:
    ldrb r2, [r5, #5]                       /* @2BC4 6a79 ldrb r2, [r5, #5]  */
L_2bc6:
    ldrb r3, [r5, #6]                       /* @2BC6 ab79 ldrb r3, [r5, #6]  */
L_2bc8:
    adds r2, r2, r3                         /* @2BC8 d218 adds r2, r2, r3  */
L_2bca:
    adds r1, r1, r2                         /* @2BCA 8918 adds r1, r1, r2  */
L_2bcc:
    ldrb r2, [r5, #7]                       /* @2BCC ea79 ldrb r2, [r5, #7]  */
L_2bce:
    adds r1, r1, r2                         /* @2BCE 8918 adds r1, r1, r2  */
L_2bd0:
    ldrb r2, [r5, #8]                       /* @2BD0 2a7a ldrb r2, [r5, #8]  */
L_2bd2:
    adds r1, r1, r2                         /* @2BD2 8918 adds r1, r1, r2  */
L_2bd4:
    ldrb r2, [r5, #9]                       /* @2BD4 6a7a ldrb r2, [r5, #9]  */
L_2bd6:
    adds r1, r1, r2                         /* @2BD6 8918 adds r1, r1, r2  */
L_2bd8:
    ldrb r2, [r5, #10]                      /* @2BD8 aa7a ldrb r2, [r5, #10]  */
L_2bda:
    uxtb r1, r1                             /* @2BDA c9b2 uxtb r1, r1  */
L_2bdc:
    cmp r1, r2                              /* @2BDC 9142 cmp r1, r2  */
L_2bde:
    bne L_2be4                              /* @2BDE 01d1 bne #2 -> 0x2BE4 */
L_2be0:
    strb r0, [r5, #1]                       /* @2BE0 6870 strb r0, [r5, #1]  */
L_2be2:
    b L_2bec                                /* @2BE2 03e0 b #6 -> 0x2BEC */
L_2be4:
    strb r4, [r5]                           /* @2BE4 2c70 strb r4, [r5]  */
L_2be6:
    b L_2bec                                /* @2BE6 01e0 b #2 -> 0x2BEC */
L_2be8:
    bl L_5ce8                               /* @2BE8 03f07ef8 bl #12540 -> sub_5CE8 */
L_2bec:
    ldrb r0, [r5, #1]                       /* @2BEC 6878 ldrb r0, [r5, #1]  */
L_2bee:
    cmp r0, #0                              /* @2BEE 0028 cmp r0, #0  */
L_2bf0:
    beq L_2c0c                              /* @2BF0 0cd0 beq #24 -> 0x2C0C */
L_2bf2:
    ldr r0, [pc, #52]                       /* @2BF2 0d48 ldr r0, [pc, #52] literal 0x2C28 -> 0x20000012 */
L_2bf4:
    ldrb r1, [r0, #1]                       /* @2BF4 4178 ldrb r1, [r0, #1]  */
L_2bf6:
    cmp r1, #2                              /* @2BF6 0229 cmp r1, #2  */
L_2bf8:
    bne L_2c08                              /* @2BF8 06d1 bne #12 -> 0x2C08 */
L_2bfa:
    ldrb r1, [r5, #4]                       /* @2BFA 2979 ldrb r1, [r5, #4]  */
L_2bfc:
    cmp r1, #253                            /* @2BFC fd29 cmp r1, #253  */
L_2bfe:
    beq L_2c06                              /* @2BFE 02d0 beq #4 -> 0x2C06 */
L_2c00:
    ldrb r1, [r5, #4]                       /* @2C00 2979 ldrb r1, [r5, #4]  */
L_2c02:
    cmp r1, #246                            /* @2C02 f629 cmp r1, #246  */
L_2c04:
    bne L_2c08                              /* @2C04 00d1 bne #0 -> 0x2C08 */
L_2c06:
    strb r4, [r0, #1]                       /* @2C06 4470 strb r4, [r0, #1]  */
L_2c08:
    bl L_70dc                               /* @2C08 04f068fa bl #17616 -> uart_dispatch */
L_2c0c:
    ldr r0, [r6, #20]                       /* @2C0C 7069 ldr r0, [r6, #20]  */
L_2c0e:
    lsrs r0, r0, #1                         /* @2C0E 4008 lsrs r0, r0, #1  */
L_2c10:
    lsls r0, r0, #1                         /* @2C10 4000 lsls r0, r0, #1  */
L_2c12:
    str r0, [r6, #20]                       /* @2C12 7061 str r0, [r6, #20]  */
L_2c14:
    pop {r3, r4, r5, r6, r7, pc}            /* @2C14 f8bd pop {r3, r4, r5, r6, r7, pc}  */
    .byte 0x00,0x00 /* @2C16 unclassified */
L_2c18:
    .byte 0xa8,0x06,0x00,0x20 /* @2C18 literal */
L_2c1c:
    .byte 0x00,0x0c,0x02,0x40 /* @2C1C literal */
L_2c20:
    .byte 0x00,0x0d,0x02,0x40 /* @2C20 literal */
L_2c24:
    .byte 0x2c,0x07,0x00,0x20 /* @2C24 literal */
L_2c28:
    .byte 0x12,0x00,0x00,0x20 /* @2C28 literal */

/* device_identity_check */
L_2c2c:
    push {r4, r5, lr}                       /* @2C2C 30b5 push {r4, r5, lr}  */
L_2c2e:
    sub sp, #44                             /* @2C2E 8bb0 sub sp, #44  */
L_2c30:
    ldr r0, [pc, #124]                      /* @2C30 1f48 ldr r0, [pc, #124] literal 0x2CB0 -> 0x00100E78 */
L_2c32:
    bl L_12f4                               /* @2C32 fef75ffb bl #-6466 -> sub_12F4 */
L_2c36:
    str r0, [sp, #28]                       /* @2C36 0790 str r0, [sp, #28]  */
L_2c38:
    ldr r0, [pc, #116]                      /* @2C38 1d48 ldr r0, [pc, #116] literal 0x2CB0 -> 0x00100E78 */
L_2c3a:
    adds r0, r0, #4                         /* @2C3A 001d adds r0, r0, #4  */
L_2c3c:
    bl L_12f4                               /* @2C3C fef75afb bl #-6476 -> sub_12F4 */
L_2c40:
    str r0, [sp, #32]                       /* @2C40 0890 str r0, [sp, #32]  */
L_2c42:
    ldr r0, [pc, #108]                      /* @2C42 1b48 ldr r0, [pc, #108] literal 0x2CB0 -> 0x00100E78 */
L_2c44:
    adds r0, #12                            /* @2C44 0c30 adds r0, #12  */
L_2c46:
    bl L_12f4                               /* @2C46 fef755fb bl #-6486 -> sub_12F4 */
L_2c4a:
    str r0, [sp, #36]                       /* @2C4A 0990 str r0, [sp, #36]  */
L_2c4c:
    ldr r0, [pc, #96]                       /* @2C4C 1848 ldr r0, [pc, #96] literal 0x2CB0 -> 0x00100E78 */
L_2c4e:
    adds r0, #16                            /* @2C4E 1030 adds r0, #16  */
L_2c50:
    bl L_12f4                               /* @2C50 fef750fb bl #-6496 -> sub_12F4 */
L_2c54:
    str r0, [sp, #40]                       /* @2C54 0a90 str r0, [sp, #40]  */
L_2c56:
    add r0, sp, #28                         /* @2C56 07a8 add r0, sp, #28  */
L_2c58:
    str r0, [sp, #4]                        /* @2C58 0190 str r0, [sp, #4]  */
L_2c5a:
    str r0, [sp, #8]                        /* @2C5A 0290 str r0, [sp, #8]  */
L_2c5c:
    add r0, sp, #12                         /* @2C5C 03a8 add r0, sp, #12  */
L_2c5e:
    str r0, [sp]                            /* @2C5E 0090 str r0, [sp]  */
L_2c60:
    mov r0, sp                              /* @2C60 6846 mov r0, sp  */
L_2c62:
    bl L_064c                               /* @2C62 fdf7f3fc bl #-9754 -> sub_064C */
L_2c66:
    ldr r5, [pc, #76]                       /* @2C66 134d ldr r5, [pc, #76] literal 0x2CB4 -> 0x00007FFC */
L_2c68:
    mov r0, r5                              /* @2C68 2846 mov r0, r5  */
L_2c6a:
    bl L_12f4                               /* @2C6A fef743fb bl #-6522 -> sub_12F4 */
L_2c6e:
    ldr r1, [pc, #72]                       /* @2C6E 1249 ldr r1, [pc, #72] literal 0x2CB8 -> 0x0000CC82 */
L_2c70:
    uxth r0, r0                             /* @2C70 80b2 uxth r0, r0  */
L_2c72:
    ldr r4, [pc, #72]                       /* @2C72 124c ldr r4, [pc, #72] literal 0x2CBC -> 0x00007BFC */
L_2c74:
    cmp r0, r1                              /* @2C74 8842 cmp r0, r1  */
L_2c76:
    bne L_2c9a                              /* @2C76 10d1 bne #32 -> 0x2C9A */
L_2c78:
    movs r0, #75                            /* @2C78 4b20 movs r0, #75  */
L_2c7a:
    lsls r0, r0, #3                         /* @2C7A c000 lsls r0, r0, #3  */
L_2c7c:
    bl L_2de8                               /* @2C7C 00f0b4f8 bl #360 -> delay_ms_candidate */
L_2c80:
    mov r0, r5                              /* @2C80 2846 mov r0, r5  */
L_2c82:
    bl L_1188                               /* @2C82 fef781fa bl #-6910 -> sub_1188 */
L_2c86:
    ldr r0, [sp, #12]                       /* @2C86 0398 ldr r0, [sp, #12]  */
L_2c88:
    uxth r1, r0                             /* @2C88 81b2 uxth r1, r0  */
L_2c8a:
    mov r0, r4                              /* @2C8A 2046 mov r0, r4  */
L_2c8c:
    bl L_1260                               /* @2C8C fef7e8fa bl #-6704 -> sub_1260 */
L_2c90:
    ldr r0, [sp, #12]                       /* @2C90 0398 ldr r0, [sp, #12]  */
L_2c92:
    lsrs r1, r0, #16                        /* @2C92 010c lsrs r1, r0, #16  */
L_2c94:
    adds r0, r4, #2                         /* @2C94 a01c adds r0, r4, #2  */
L_2c96:
    bl L_1260                               /* @2C96 fef7e3fa bl #-6714 -> sub_1260 */
L_2c9a:
    mov r0, r4                              /* @2C9A 2046 mov r0, r4  */
L_2c9c:
    bl L_12f4                               /* @2C9C fef72afb bl #-6572 -> sub_12F4 */
L_2ca0:
    ldr r1, [sp, #12]                       /* @2CA0 0399 ldr r1, [sp, #12]  */
L_2ca2:
    cmp r0, r1                              /* @2CA2 8842 cmp r0, r1  */
L_2ca4:
    beq L_2cac                              /* @2CA4 02d0 beq #4 -> 0x2CAC */
L_2ca6:
    movs r0, #0                             /* @2CA6 0020 movs r0, #0  */
L_2ca8:
    add sp, #44                             /* @2CA8 0bb0 add sp, #44  */
L_2caa:
    pop {r4, r5, pc}                        /* @2CAA 30bd pop {r4, r5, pc}  */
L_2cac:
    movs r0, #1                             /* @2CAC 0120 movs r0, #1  */
L_2cae:
    b L_2ca8                                /* @2CAE fbe7 b #-10 -> 0x2CA8 */
L_2cb0:
    .byte 0x78,0x0e,0x10,0x00 /* @2CB0 literal */
L_2cb4:
    .byte 0xfc,0x7f,0x00,0x00 /* @2CB4 literal */
L_2cb8:
    .byte 0x82,0xcc,0x00,0x00 /* @2CB8 literal */
L_2cbc:
    .byte 0xfc,0x7b,0x00,0x00 /* @2CBC literal */

/* main */
L_2cc0:
    bl L_1a8c                               /* @2CC0 fef7e4fe bl #-4664 -> system_init */
L_2cc4:
    ldr r6, [pc, #92]                       /* @2CC4 174e ldr r6, [pc, #92] literal 0x2D24 -> 0x2000000C */
L_2cc6:
    movs r0, #1                             /* @2CC6 0120 movs r0, #1  */
L_2cc8:
    str r0, [r6]                            /* @2CC8 3060 str r0, [r6]  */
L_2cca:
    bl L_2608                               /* @2CCA fff79dfc bl #-1734 -> board_init */
L_2cce:
    bl L_2c2c                               /* @2CCE fff7adff bl #-166 -> device_identity_check */
L_2cd2:
    mov r4, r0                              /* @2CD2 0446 mov r4, r0  */
L_2cd4:
    bl L_5ca0                               /* @2CD4 02f0e4ff bl #12232 -> sub_5CA0 */
L_2cd8:
    cmp r4, #0                              /* @2CD8 002c cmp r4, #0  */
L_2cda:
    beq L_2d14                              /* @2CDA 1bd0 beq #54 -> 0x2D14 */
L_2cdc:
    bl L_0e20                               /* @2CDC fef7a0f8 bl #-7872 -> sub_0E20 */
L_2ce0:
    movs r5, #0                             /* @2CE0 0025 movs r5, #0  */
L_2ce2:
    str r5, [r6]                            /* @2CE2 3560 str r5, [r6]  */
L_2ce4:
    ldr r4, [pc, #64]                       /* @2CE4 104c ldr r4, [pc, #64] literal 0x2D28 -> 0x20000012 */
L_2ce6:
    ldr r6, [pc, #68]                       /* @2CE6 114e ldr r6, [pc, #68] literal 0x2D2C -> 0x20000014 */
L_2ce8:
    ldr r7, [pc, #68]                       /* @2CE8 114f ldr r7, [pc, #68] literal 0x2D30 -> 0x20000015 */
L_2cea:
    bl L_1b28                               /* @2CEA fef71dff bl #-4550 -> foreground_ui_protection */
L_2cee:
    bl L_27e8                               /* @2CEE fff77bfd bl #-1290 -> update_multiturn_angle */
L_2cf2:
    ldrb r0, [r4]                           /* @2CF2 2078 ldrb r0, [r4]  */
L_2cf4:
    cmp r0, #0                              /* @2CF4 0028 cmp r0, #0  */
L_2cf6:
    beq L_2cfe                              /* @2CF6 02d0 beq #4 -> 0x2CFE */
L_2cf8:
    strb r5, [r4]                           /* @2CF8 2570 strb r5, [r4]  */
L_2cfa:
    bl L_2090                               /* @2CFA fff7c9f9 bl #-3182 -> sub_2090 */
L_2cfe:
    ldrb r0, [r6]                           /* @2CFE 3078 ldrb r0, [r6]  */
L_2d00:
    cmp r0, #0                              /* @2D00 0028 cmp r0, #0  */
L_2d02:
    beq L_2d08                              /* @2D02 01d0 beq #2 -> 0x2D08 */
L_2d04:
    bl L_161c                               /* @2D04 fef78afc bl #-5868 -> sub_161C */
L_2d08:
    ldrb r0, [r7]                           /* @2D08 3878 ldrb r0, [r7]  */
L_2d0a:
    cmp r0, #0                              /* @2D0A 0028 cmp r0, #0  */
L_2d0c:
    beq L_2cea                              /* @2D0C edd0 beq #-38 -> 0x2CEA */
L_2d0e:
    bl L_1f78                               /* @2D0E fff733f9 bl #-3482 -> sub_1F78 */
L_2d12:
    b L_2cea                                /* @2D12 eae7 b #-44 -> 0x2CEA */
L_2d14:
    movs r4, #125                           /* @2D14 7d24 movs r4, #125  */
L_2d16:
    lsls r4, r4, #3                         /* @2D16 e400 lsls r4, r4, #3  */
L_2d18:
    mov r1, r4                              /* @2D18 2146 mov r1, r4  */
L_2d1a:
    movs r0, #255                           /* @2D1A ff20 movs r0, #255  */
L_2d1c:
    bl L_3f48                               /* @2D1C 01f014f9 bl #4648 -> sub_3F48 */
L_2d20:
    b L_2d18                                /* @2D20 fae7 b #-12 -> 0x2D18 */
    .byte 0x00,0x00 /* @2D22 unclassified */
L_2d24:
    .byte 0x0c,0x00,0x00,0x20 /* @2D24 literal */
L_2d28:
    .byte 0x12,0x00,0x00,0x20 /* @2D28 literal */
L_2d2c:
    .byte 0x14,0x00,0x00,0x20 /* @2D2C literal */
L_2d30:
    .byte 0x15,0x00,0x00,0x20 /* @2D30 literal */

/* menu_mode */
L_2d34:
    push {r4, r5, r6, lr}                   /* @2D34 70b5 push {r4, r5, r6, lr}  */
L_2d36:
    mov r4, r0                              /* @2D36 0446 mov r4, r0  */
L_2d38:
    ldr r6, [pc, #48]                       /* @2D38 0c4e ldr r6, [pc, #48] literal 0x2D6C -> 0x2000072C */
L_2d3a:
    cmp r4, #255                            /* @2D3A ff2c cmp r4, #255  */
L_2d3c:
    beq L_2d68                              /* @2D3C 14d0 beq #40 -> 0x2D68 */
L_2d3e:
    ldrb r0, [r6, #9]                       /* @2D3E 707a ldrb r0, [r6, #9]  */
L_2d40:
    movs r5, #1                             /* @2D40 0125 movs r5, #1  */
L_2d42:
    cmp r0, r4                              /* @2D42 a042 cmp r0, r4  */
L_2d44:
    beq L_2d56                              /* @2D44 07d0 beq #14 -> 0x2D56 */
L_2d46:
    bl L_52cc                               /* @2D46 02f0c1fa bl #9602 -> sub_52CC */
L_2d4a:
    ldr r0, [pc, #36]                       /* @2D4A 0948 ldr r0, [pc, #36] literal 0x2D70 -> 0x200005D9 */
L_2d4c:
    strb r5, [r0]                           /* @2D4C 0570 strb r5, [r0]  */
L_2d4e:
    ldr r0, [pc, #36]                       /* @2D4E 0948 ldr r0, [pc, #36] literal 0x2D74 -> 0x200006F4 */
L_2d50:
    strb r4, [r0, #9]                       /* @2D50 4472 strb r4, [r0, #9]  */
L_2d52:
    bl L_470c                               /* @2D52 01f0dbfc bl #6582 -> sub_470C */
L_2d56:
    ldrb r0, [r6, #9]                       /* @2D56 707a ldrb r0, [r6, #9]  */
L_2d58:
    cmp r0, #2                              /* @2D58 0228 cmp r0, #2  */
L_2d5a:
    bne L_2d64                              /* @2D5A 03d1 bne #6 -> 0x2D64 */
L_2d5c:
    ldr r0, [pc, #24]                       /* @2D5C 0648 ldr r0, [pc, #24] literal 0x2D78 -> 0x20000604 */
L_2d5e:
    strb r5, [r0, #3]                       /* @2D5E c570 strb r5, [r0, #3]  */
L_2d60:
    bl L_2090                               /* @2D60 fff796f9 bl #-3284 -> sub_2090 */
L_2d64:
    mov r0, r4                              /* @2D64 2046 mov r0, r4  */
L_2d66:
    pop {r4, r5, r6, pc}                    /* @2D66 70bd pop {r4, r5, r6, pc}  */
L_2d68:
    ldrb r0, [r6, #9]                       /* @2D68 707a ldrb r0, [r6, #9]  */
L_2d6a:
    pop {r4, r5, r6, pc}                    /* @2D6A 70bd pop {r4, r5, r6, pc}  */
L_2d6c:
    .byte 0x2c,0x07,0x00,0x20 /* @2D6C literal */
L_2d70:
    .byte 0xd9,0x05,0x00,0x20 /* @2D70 literal */
L_2d74:
    .byte 0xf4,0x06,0x00,0x20 /* @2D74 literal */
L_2d78:
    .byte 0x04,0x06,0x00,0x20 /* @2D78 literal */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @2D7C unclassified */

/* zero_bytes */
L_2d84:
    cmp r0, #0                              /* @2D84 0028 cmp r0, #0  */
L_2d86:
    beq L_2d96                              /* @2D86 06d0 beq #12 -> 0x2D96 */
L_2d88:
    movs r2, #0                             /* @2D88 0022 movs r2, #0  */
L_2d8a:
    b L_2d90                                /* @2D8A 01e0 b #2 -> 0x2D90 */
L_2d8c:
    strb r2, [r0]                           /* @2D8C 0270 strb r2, [r0]  */
L_2d8e:
    adds r0, r0, #1                         /* @2D8E 401c adds r0, r0, #1  */
L_2d90:
    subs r1, r1, #1                         /* @2D90 491e subs r1, r1, #1  */
L_2d92:
    adds r3, r1, #1                         /* @2D92 4b1c adds r3, r1, #1  */
L_2d94:
    bne L_2d8c                              /* @2D94 fad1 bne #-12 -> 0x2D8C */
L_2d96:
    bx lr                                   /* @2D96 7047 bx lr  */

/* delay_100us_candidate */
L_2d98:
    push {r3, r4, r5, r6, r7, lr}           /* @2D98 f8b5 push {r3, r4, r5, r6, r7, lr}  */
L_2d9a:
    mov r4, r0                              /* @2D9A 0446 mov r4, r0  */
L_2d9c:
    ldr r5, [pc, #60]                       /* @2D9C 0f4d ldr r5, [pc, #60] literal 0x2DDC -> 0xE000E000 */
L_2d9e:
    ldr r0, [pc, #56]                       /* @2D9E 0e48 ldr r0, [pc, #56] literal 0x2DD8 -> 0x00FFFFFF */
L_2da0:
    str r0, [r5, #20]                       /* @2DA0 6861 str r0, [r5, #20]  */
L_2da2:
    movs r6, #0                             /* @2DA2 0026 movs r6, #0  */
L_2da4:
    str r6, [r5, #24]                       /* @2DA4 ae61 str r6, [r5, #24]  */
L_2da6:
    movs r0, #5                             /* @2DA6 0520 movs r0, #5  */
L_2da8:
    str r0, [r5, #16]                       /* @2DA8 2861 str r0, [r5, #16]  */
L_2daa:
    ldr r0, [pc, #52]                       /* @2DAA 0d48 ldr r0, [pc, #52] literal 0x2DE0 -> 0x20000000 */
L_2dac:
    movs r7, #1                             /* @2DAC 0127 movs r7, #1  */
L_2dae:
    ldr r0, [r0]                            /* @2DAE 0068 ldr r0, [r0]  */
L_2db0:
    lsls r7, r7, #24                        /* @2DB0 3f06 lsls r7, r7, #24  */
L_2db2:
    str r0, [sp]                            /* @2DB2 0090 str r0, [sp]  */
L_2db4:
    b L_2dc8                                /* @2DB4 08e0 b #16 -> 0x2DC8 */
L_2db6:
    str r6, [r5, #24]                       /* @2DB6 ae61 str r6, [r5, #24]  */
L_2db8:
    ldr r1, [pc, #40]                       /* @2DB8 0a49 ldr r1, [pc, #40] literal 0x2DE4 -> 0x00002710 */
L_2dba:
    ldr r0, [sp]                            /* @2DBA 0098 ldr r0, [sp]  */
L_2dbc:
    bl L_0110                               /* @2DBC fdf7a8f9 bl #-11440 -> udivmod32 */
L_2dc0:
    subs r0, r7, r0                         /* @2DC0 381a subs r0, r7, r0  */
L_2dc2:
    ldr r1, [r5, #24]                       /* @2DC2 a969 ldr r1, [r5, #24]  */
L_2dc4:
    cmp r1, r0                              /* @2DC4 8142 cmp r1, r0  */
L_2dc6:
    bhi L_2dc2                              /* @2DC6 fcd8 bhi #-8 -> 0x2DC2 */
L_2dc8:
    subs r4, r4, #1                         /* @2DC8 641e subs r4, r4, #1  */
L_2dca:
    adds r0, r4, #1                         /* @2DCA 601c adds r0, r4, #1  */
L_2dcc:
    bne L_2db6                              /* @2DCC f3d1 bne #-26 -> 0x2DB6 */
L_2dce:
    ldr r0, [r5, #16]                       /* @2DCE 2869 ldr r0, [r5, #16]  */
L_2dd0:
    lsrs r0, r0, #1                         /* @2DD0 4008 lsrs r0, r0, #1  */
L_2dd2:
    lsls r0, r0, #1                         /* @2DD2 4000 lsls r0, r0, #1  */
L_2dd4:
    str r0, [r5, #16]                       /* @2DD4 2861 str r0, [r5, #16]  */
L_2dd6:
    pop {r3, r4, r5, r6, r7, pc}            /* @2DD6 f8bd pop {r3, r4, r5, r6, r7, pc}  */
L_2dd8:
    .byte 0xff,0xff,0xff,0x00 /* @2DD8 literal */
L_2ddc:
    .byte 0x00,0xe0,0x00,0xe0 /* @2DDC literal */
L_2de0:
    .byte 0x00,0x00,0x00,0x20 /* @2DE0 literal */
L_2de4:
    .byte 0x10,0x27,0x00,0x00 /* @2DE4 literal */

/* delay_ms_candidate */
L_2de8:
    push {r3, r4, r5, r6, r7, lr}           /* @2DE8 f8b5 push {r3, r4, r5, r6, r7, lr}  */
L_2dea:
    mov r4, r0                              /* @2DEA 0446 mov r4, r0  */
L_2dec:
    ldr r5, [pc, #64]                       /* @2DEC 104d ldr r5, [pc, #64] literal 0x2E30 -> 0xE000E000 */
L_2dee:
    ldr r0, [pc, #60]                       /* @2DEE 0f48 ldr r0, [pc, #60] literal 0x2E2C -> 0x00FFFFFF */
L_2df0:
    str r0, [r5, #20]                       /* @2DF0 6861 str r0, [r5, #20]  */
L_2df2:
    movs r6, #0                             /* @2DF2 0026 movs r6, #0  */
L_2df4:
    str r6, [r5, #24]                       /* @2DF4 ae61 str r6, [r5, #24]  */
L_2df6:
    movs r0, #5                             /* @2DF6 0520 movs r0, #5  */
L_2df8:
    str r0, [r5, #16]                       /* @2DF8 2861 str r0, [r5, #16]  */
L_2dfa:
    ldr r0, [pc, #56]                       /* @2DFA 0e48 ldr r0, [pc, #56] literal 0x2E34 -> 0x20000000 */
L_2dfc:
    movs r7, #1                             /* @2DFC 0127 movs r7, #1  */
L_2dfe:
    ldr r0, [r0]                            /* @2DFE 0068 ldr r0, [r0]  */
L_2e00:
    lsls r7, r7, #24                        /* @2E00 3f06 lsls r7, r7, #24  */
L_2e02:
    str r0, [sp]                            /* @2E02 0090 str r0, [sp]  */
L_2e04:
    b L_2e1a                                /* @2E04 09e0 b #18 -> 0x2E1A */
L_2e06:
    str r6, [r5, #24]                       /* @2E06 ae61 str r6, [r5, #24]  */
L_2e08:
    movs r1, #125                           /* @2E08 7d21 movs r1, #125  */
L_2e0a:
    lsls r1, r1, #3                         /* @2E0A c900 lsls r1, r1, #3  */
L_2e0c:
    ldr r0, [sp]                            /* @2E0C 0098 ldr r0, [sp]  */
L_2e0e:
    bl L_0110                               /* @2E0E fdf77ff9 bl #-11522 -> udivmod32 */
L_2e12:
    subs r0, r7, r0                         /* @2E12 381a subs r0, r7, r0  */
L_2e14:
    ldr r1, [r5, #24]                       /* @2E14 a969 ldr r1, [r5, #24]  */
L_2e16:
    cmp r1, r0                              /* @2E16 8142 cmp r1, r0  */
L_2e18:
    bhi L_2e14                              /* @2E18 fcd8 bhi #-8 -> 0x2E14 */
L_2e1a:
    subs r4, r4, #1                         /* @2E1A 641e subs r4, r4, #1  */
L_2e1c:
    adds r0, r4, #1                         /* @2E1C 601c adds r0, r4, #1  */
L_2e1e:
    bne L_2e06                              /* @2E1E f2d1 bne #-28 -> 0x2E06 */
L_2e20:
    ldr r0, [r5, #16]                       /* @2E20 2869 ldr r0, [r5, #16]  */
L_2e22:
    lsrs r0, r0, #1                         /* @2E22 4008 lsrs r0, r0, #1  */
L_2e24:
    lsls r0, r0, #1                         /* @2E24 4000 lsls r0, r0, #1  */
L_2e26:
    str r0, [r5, #16]                       /* @2E26 2861 str r0, [r5, #16]  */
L_2e28:
    pop {r3, r4, r5, r6, r7, pc}            /* @2E28 f8bd pop {r3, r4, r5, r6, r7, pc}  */
    .byte 0x00,0x00 /* @2E2A unclassified */
L_2e2c:
    .byte 0xff,0xff,0xff,0x00 /* @2E2C literal */
L_2e30:
    .byte 0x00,0xe0,0x00,0xe0 /* @2E30 literal */
L_2e34:
    .byte 0x00,0x00,0x00,0x20 /* @2E34 literal */

/* calibration_request */
L_2e38:
    push {r3, r4, r5, r6, r7, lr}           /* @2E38 f8b5 push {r3, r4, r5, r6, r7, lr}  */
L_2e3a:
    movs r5, #0                             /* @2E3A 0025 movs r5, #0  */
L_2e3c:
    ldr r4, [pc, #84]                       /* @2E3C 154c ldr r4, [pc, #84] literal 0x2E94 -> 0x2000072C */
L_2e3e:
    ldrb r0, [r4, #9]                       /* @2E3E 607a ldrb r0, [r4, #9]  */
L_2e40:
    cmp r0, #0                              /* @2E40 0028 cmp r0, #0  */
L_2e42:
    beq L_2e7c                              /* @2E42 1bd0 beq #54 -> 0x2E7C */
L_2e44:
    ldrh r6, [r4]                           /* @2E44 2688 ldrh r6, [r4]  */
L_2e46:
    ldr r0, [pc, #80]                       /* @2E46 1448 ldr r0, [pc, #80] literal 0x2E98 -> 0x200005F0 */
L_2e48:
    ldrh r7, [r0]                           /* @2E48 0788 ldrh r7, [r0]  */
L_2e4a:
    movs r1, #16                            /* @2E4A 1021 movs r1, #16  */
L_2e4c:
    strh r1, [r4]                           /* @2E4C 2180 strh r1, [r4]  */
L_2e4e:
    movs r1, #255                           /* @2E4E ff21 movs r1, #255  */
L_2e50:
    adds r1, #31                            /* @2E50 1f31 adds r1, #31  */
L_2e52:
    strh r1, [r0]                           /* @2E52 0180 strh r1, [r0]  */
L_2e54:
    ldrb r0, [r4, #9]                       /* @2E54 607a ldrb r0, [r4, #9]  */
L_2e56:
    cmp r0, #2                              /* @2E56 0228 cmp r0, #2  */
L_2e58:
    bne L_2e6c                              /* @2E58 08d1 bne #16 -> 0x2E6C */
L_2e5a:
    bl L_3a18                               /* @2E5A 00f0ddfd bl #3002 -> sub_3A18 */
L_2e5e:
    bl L_5874                               /* @2E5E 02f009fd bl #10770 -> sub_5874 */
L_2e62:
    adr r2, L_2e9c                          /* @2E62 0ea2 adr r2, #56 address 0x2E9C -> 0x00002E9C */
L_2e64:
    adr r1, L_2ea0                          /* @2E64 0ea1 adr r1, #56 address 0x2EA0 -> 0x00002EA0 'Cal...' */
L_2e66:
    adr r0, L_2e9c                          /* @2E66 0da0 adr r0, #52 address 0x2E9C -> 0x00002E9C */
L_2e68:
    bl L_4884                               /* @2E68 01f00cfd bl #6680 -> display_three_lines */
L_2e6c:
    movs r0, #0                             /* @2E6C 0020 movs r0, #0  */
L_2e6e:
    bl L_54cc                               /* @2E6E 02f02dfb bl #9818 -> calibration_work_candidate */
L_2e72:
    mov r5, r0                              /* @2E72 0546 mov r5, r0  */
L_2e74:
    strh r6, [r4]                           /* @2E74 2680 strh r6, [r4]  */
L_2e76:
    ldr r0, [pc, #32]                       /* @2E76 0848 ldr r0, [pc, #32] literal 0x2E98 -> 0x200005F0 */
L_2e78:
    strh r7, [r0]                           /* @2E78 0780 strh r7, [r0]  */
L_2e7a:
    b L_2e8e                                /* @2E7A 08e0 b #16 -> 0x2E8E */
L_2e7c:
    adr r2, L_2ea8                          /* @2E7C 0aa2 adr r2, #40 address 0x2EA8 -> 0x00002EA8 'Mode' */
L_2e7e:
    adr r1, L_2eb0                          /* @2E7E 0ca1 adr r1, #48 address 0x2EB0 -> 0x00002EB0 ' CR_OPEN' */
L_2e80:
    adr r0, L_2ebc                          /* @2E80 0ea0 adr r0, #56 address 0x2EBC -> 0x00002EBC 'It is' */
L_2e82:
    bl L_4884                               /* @2E82 01f0fffc bl #6654 -> display_three_lines */
L_2e86:
    movs r0, #75                            /* @2E86 4b20 movs r0, #75  */
L_2e88:
    lsls r0, r0, #4                         /* @2E88 0001 lsls r0, r0, #4  */
L_2e8a:
    bl L_2de8                               /* @2E8A fff7adff bl #-166 -> delay_ms_candidate */
L_2e8e:
    mov r0, r5                              /* @2E8E 2846 mov r0, r5  */
L_2e90:
    pop {r3, r4, r5, r6, r7, pc}            /* @2E90 f8bd pop {r3, r4, r5, r6, r7, pc}  */
    .byte 0x00,0x00 /* @2E92 unclassified */
L_2e94:
    .byte 0x2c,0x07,0x00,0x20 /* @2E94 literal */
L_2e98:
    .byte 0xf0,0x05,0x00,0x20 /* @2E98 literal */
L_2e9c:
    .byte 0x00,0x00,0x00,0x00 /* @2E9C unclassified */
L_2ea0:
    .byte 0x43,0x61,0x6c,0x2e,0x2e,0x2e,0x00,0x00 /* @2EA0 unclassified */
L_2ea8:
    .byte 0x4d,0x6f,0x64,0x65,0x00,0x00,0x00,0x00 /* @2EA8 unclassified */
L_2eb0:
    .byte 0x20,0x43,0x52,0x5f,0x4f,0x50,0x45,0x4e,0x00,0x00,0x00,0x00 /* @2EB0 unclassified */
L_2ebc:
    .byte 0x49,0x74,0x20,0x69,0x73,0x00,0x00,0x00 /* @2EBC unclassified */

/* menu_goto_0 */
L_2ec4:
    push {r3, r4, r5, r6, r7, lr}           /* @2EC4 f8b5 push {r3, r4, r5, r6, r7, lr}  */
L_2ec6:
    movs r6, #0                             /* @2EC6 0026 movs r6, #0  */
L_2ec8:
    ldr r4, [pc, #144]                      /* @2EC8 244c ldr r4, [pc, #144] literal 0x2F5C -> 0x2000072C */
L_2eca:
    ldrb r0, [r4, #9]                       /* @2ECA 607a ldrb r0, [r4, #9]  */
L_2ecc:
    movs r5, #75                            /* @2ECC 4b25 movs r5, #75  */
L_2ece:
    lsls r5, r5, #4                         /* @2ECE 2d01 lsls r5, r5, #4  */
L_2ed0:
    cmp r0, #0                              /* @2ED0 0028 cmp r0, #0  */
L_2ed2:
    beq L_2f48                              /* @2ED2 39d0 beq #114 -> 0x2F48 */
L_2ed4:
    ldr r1, [pc, #136]                      /* @2ED4 2249 ldr r1, [pc, #136] literal 0x2F60 -> 0x200005F6 */
L_2ed6:
    ldrb r0, [r1]                           /* @2ED6 0878 ldrb r0, [r1]  */
L_2ed8:
    cmp r0, #0                              /* @2ED8 0028 cmp r0, #0  */
L_2eda:
    beq L_2ef6                              /* @2EDA 0cd0 beq #24 -> 0x2EF6 */
L_2edc:
    ldrh r0, [r1, #2]                       /* @2EDC 4888 ldrh r0, [r1, #2]  */
L_2ede:
    ldr r1, [pc, #132]                      /* @2EDE 2149 ldr r1, [pc, #132] literal 0x2F64 -> 0x0000FFFF */
L_2ee0:
    cmp r0, r1                              /* @2EE0 8842 cmp r0, r1  */
L_2ee2:
    bne L_2f08                              /* @2EE2 11d1 bne #34 -> 0x2F08 */
L_2ee4:
    adr r2, L_2f68                          /* @2EE4 20a2 adr r2, #128 address 0x2F68 -> 0x00002F68 ' Set' */
L_2ee6:
    adr r1, L_2f70                          /* @2EE6 22a1 adr r1, #136 address 0x2F70 -> 0x00002F70 ' not' */
L_2ee8:
    adr r0, L_2f78                          /* @2EE8 23a0 adr r0, #140 address 0x2F78 -> 0x00002F78 'Origin' */
L_2eea:
    bl L_4884                               /* @2EEA 01f0cbfc bl #6550 -> display_three_lines */
L_2eee:
    mov r0, r5                              /* @2EEE 2846 mov r0, r5  */
L_2ef0:
    bl L_2de8                               /* @2EF0 fff77aff bl #-268 -> delay_ms_candidate */
L_2ef4:
    b L_2f58                                /* @2EF4 30e0 b #96 -> 0x2F58 */
L_2ef6:
    adr r2, L_2f80                          /* @2EF6 22a2 adr r2, #136 address 0x2F80 -> 0x00002F80 'Disable' */
L_2ef8:
    adr r1, L_2f88                          /* @2EF8 23a1 adr r1, #140 address 0x2F88 -> 0x00002F88 ' is' */
L_2efa:
    adr r0, L_2f8c                          /* @2EFA 24a0 adr r0, #144 address 0x2F8C -> 0x00002F8C '0_Mode' */
L_2efc:
    bl L_4884                               /* @2EFC 01f0c2fc bl #6532 -> display_three_lines */
L_2f00:
    mov r0, r5                              /* @2F00 2846 mov r0, r5  */
L_2f02:
    bl L_2de8                               /* @2F02 fff771ff bl #-286 -> delay_ms_candidate */
L_2f06:
    b L_2f58                                /* @2F06 27e0 b #78 -> 0x2F58 */
L_2f08:
    bl L_3e80                               /* @2F08 00f0baff bl #3956 -> sub_3E80 */
L_2f0c:
    cmp r0, #0                              /* @2F0C 0028 cmp r0, #0  */
L_2f0e:
    beq L_2f1a                              /* @2F0E 04d0 beq #8 -> 0x2F1A */
L_2f10:
    movs r0, #0                             /* @2F10 0020 movs r0, #0  */
L_2f12:
    bl L_4dc8                               /* @2F12 01f059ff bl #7858 -> origin_return_candidate */
L_2f16:
    mov r6, r0                              /* @2F16 0646 mov r6, r0  */
L_2f18:
    b L_2f58                                /* @2F18 1ee0 b #60 -> 0x2F58 */
L_2f1a:
    bl L_20e4                               /* @2F1A fff7e3f8 bl #-3642 -> sub_20E4 */
L_2f1e:
    ldrb r5, [r4, #8]                       /* @2F1E 257a ldrb r5, [r4, #8]  */
L_2f20:
    movs r0, #2                             /* @2F20 0220 movs r0, #2  */
L_2f22:
    strb r0, [r4, #8]                       /* @2F22 2072 strb r0, [r4, #8]  */
L_2f24:
    ldr r7, [pc, #108]                      /* @2F24 1b4f ldr r7, [pc, #108] literal 0x2F94 -> 0x20000604 */
L_2f26:
    movs r0, #1                             /* @2F26 0120 movs r0, #1  */
L_2f28:
    strb r0, [r7, #3]                       /* @2F28 f870 strb r0, [r7, #3]  */
L_2f2a:
    bl L_2090                               /* @2F2A fff7b1f8 bl #-3742 -> sub_2090 */
L_2f2e:
    movs r0, #10                            /* @2F2E 0a20 movs r0, #10  */
L_2f30:
    bl L_2de8                               /* @2F30 fff75aff bl #-332 -> delay_ms_candidate */
L_2f34:
    movs r0, #0                             /* @2F34 0020 movs r0, #0  */
L_2f36:
    bl L_4dc8                               /* @2F36 01f047ff bl #7822 -> origin_return_candidate */
L_2f3a:
    mov r6, r0                              /* @2F3A 0646 mov r6, r0  */
L_2f3c:
    movs r0, #0                             /* @2F3C 0020 movs r0, #0  */
L_2f3e:
    strb r0, [r7, #3]                       /* @2F3E f870 strb r0, [r7, #3]  */
L_2f40:
    strb r5, [r4, #8]                       /* @2F40 2572 strb r5, [r4, #8]  */
L_2f42:
    bl L_2090                               /* @2F42 fff7a5f8 bl #-3766 -> sub_2090 */
L_2f46:
    b L_2f58                                /* @2F46 07e0 b #14 -> 0x2F58 */
L_2f48:
    adr r2, L_2f98                          /* @2F48 13a2 adr r2, #76 address 0x2F98 -> 0x00002F98 'Mode' */
L_2f4a:
    adr r1, L_2fa0                          /* @2F4A 15a1 adr r1, #84 address 0x2FA0 -> 0x00002FA0 ' CR_OPEN' */
L_2f4c:
    adr r0, L_2fac                          /* @2F4C 17a0 adr r0, #92 address 0x2FAC -> 0x00002FAC 'It is' */
L_2f4e:
    bl L_4884                               /* @2F4E 01f099fc bl #6450 -> display_three_lines */
L_2f52:
    mov r0, r5                              /* @2F52 2846 mov r0, r5  */
L_2f54:
    bl L_2de8                               /* @2F54 fff748ff bl #-368 -> delay_ms_candidate */
L_2f58:
    mov r0, r6                              /* @2F58 3046 mov r0, r6  */
L_2f5a:
    pop {r3, r4, r5, r6, r7, pc}            /* @2F5A f8bd pop {r3, r4, r5, r6, r7, pc}  */
L_2f5c:
    .byte 0x2c,0x07,0x00,0x20 /* @2F5C literal */
L_2f60:
    .byte 0xf6,0x05,0x00,0x20 /* @2F60 literal */
L_2f64:
    .byte 0xff,0xff,0x00,0x00 /* @2F64 literal */
L_2f68:
    .byte 0x20,0x53,0x65,0x74,0x00,0x00,0x00,0x00 /* @2F68 unclassified */
L_2f70:
    .byte 0x20,0x6e,0x6f,0x74,0x00,0x00,0x00,0x00 /* @2F70 unclassified */
L_2f78:
    .byte 0x4f,0x72,0x69,0x67,0x69,0x6e,0x00,0x00 /* @2F78 unclassified */
L_2f80:
    .byte 0x44,0x69,0x73,0x61,0x62,0x6c,0x65,0x00 /* @2F80 unclassified */
L_2f88:
    .byte 0x20,0x69,0x73,0x00 /* @2F88 unclassified */
L_2f8c:
    .byte 0x30,0x5f,0x4d,0x6f,0x64,0x65,0x00,0x00 /* @2F8C unclassified */
L_2f94:
    .byte 0x04,0x06,0x00,0x20 /* @2F94 literal */
L_2f98:
    .byte 0x4d,0x6f,0x64,0x65,0x00,0x00,0x00,0x00 /* @2F98 unclassified */
L_2fa0:
    .byte 0x20,0x43,0x52,0x5f,0x4f,0x50,0x45,0x4e,0x00,0x00,0x00,0x00 /* @2FA0 unclassified */
L_2fac:
    .byte 0x49,0x74,0x20,0x69,0x73,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @2FAC unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @2FBC unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @2FCC unclassified */
    .byte 0x00,0x00,0x00,0x00 /* @2FDC unclassified */

/* display_banner */
L_2fe0:
    push {r4, lr}                           /* @2FE0 10b5 push {r4, lr}  */
L_2fe2:
    bl L_492c                               /* @2FE2 01f0a3fc bl #6470 -> sub_492C */
L_2fe6:
    adr r2, L_2ffc                          /* @2FE6 05a2 adr r2, #20 address 0x2FFC -> 0x00002FFC 'V1.1.3' */
L_2fe8:
    adr r1, L_3004                          /* @2FE8 06a1 adr r1, #24 address 0x3004 -> 0x00003004 'SERVO42C' */
L_2fea:
    adr r0, L_3010                          /* @2FEA 09a0 adr r0, #36 address 0x3010 -> 0x00003010 'MKS' */
L_2fec:
    bl L_4884                               /* @2FEC 01f04afc bl #6292 -> display_three_lines */
L_2ff0:
    movs r0, #125                           /* @2FF0 7d20 movs r0, #125  */
L_2ff2:
    lsls r0, r0, #3                         /* @2FF2 c000 lsls r0, r0, #3  */
L_2ff4:
    bl L_2de8                               /* @2FF4 fff7f8fe bl #-528 -> delay_ms_candidate */
L_2ff8:
    pop {r4, pc}                            /* @2FF8 10bd pop {r4, pc}  */
    .byte 0x00,0x00 /* @2FFA unclassified */
L_2ffc:
    .ascii "V1.1.3\0\0" /* @2FFC firmware version: keep six visible characters and two NULs */
L_3004:
    .byte 0x53,0x45,0x52,0x56,0x4f,0x34,0x32,0x43,0x00,0x00,0x00,0x00 /* @3004 unclassified */
L_3010:
    .byte 0x4d,0x4b,0x53,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @3010 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @3020 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @3030 unclassified */

/* sub_3040 */
L_3040:
    push {r4, lr}                           /* @3040 10b5 push {r4, lr}  */
L_3042:
    movs r1, #0                             /* @3042 0021 movs r1, #0  */
L_3044:
    movs r0, #2                             /* @3044 0220 movs r0, #2  */
L_3046:
    bl L_08c0                               /* @3046 fdf73bfc bl #-10122 -> sub_08C0 */
L_304a:
    movs r0, #2                             /* @304A 0220 movs r0, #2  */
L_304c:
    bl L_0b84                               /* @304C fdf79afd bl #-9420 -> sub_0B84 */
L_3050:
    pop {r4, pc}                            /* @3050 10bd pop {r4, pc}  */

/* sub_3052 */
L_3052:
    push {r4, r5, r6, lr}                   /* @3052 70b5 push {r4, r5, r6, lr}  */
L_3054:
    mov r5, r0                              /* @3054 0546 mov r5, r0  */
L_3056:
    mov r4, r1                              /* @3056 0c46 mov r4, r1  */
L_3058:
    movs r0, #0                             /* @3058 0020 movs r0, #0  */
L_305a:
    cmp r5, #0                              /* @305A 002d cmp r5, #0  */
L_305c:
    beq L_30ea                              /* @305C 45d0 beq #138 -> 0x30EA */
L_305e:
    cmp r5, #10                             /* @305E 0a2d cmp r5, #10  */
L_3060:
    bhs L_306a                              /* @3060 03d2 bhs #6 -> 0x306A */
L_3062:
    adds r5, #48                            /* @3062 3035 adds r5, #48  */
L_3064:
    strb r5, [r4]                           /* @3064 2570 strb r5, [r4]  */
L_3066:
    movs r0, #1                             /* @3066 0120 movs r0, #1  */
L_3068:
    b L_30ea                                /* @3068 3fe0 b #126 -> 0x30EA */
L_306a:
    cmp r5, #100                            /* @306A 642d cmp r5, #100  */
L_306c:
    bhs L_3082                              /* @306C 09d2 bhs #18 -> 0x3082 */
L_306e:
    movs r1, #10                            /* @306E 0a21 movs r1, #10  */
L_3070:
    mov r0, r5                              /* @3070 2846 mov r0, r5  */
L_3072:
    bl L_0110                               /* @3072 fdf74df8 bl #-12134 -> udivmod32 */
L_3076:
    adds r0, #48                            /* @3076 3030 adds r0, #48  */
L_3078:
    strb r0, [r4]                           /* @3078 2070 strb r0, [r4]  */
L_307a:
    adds r1, #48                            /* @307A 3031 adds r1, #48  */
L_307c:
    strb r1, [r4, #1]                       /* @307C 6170 strb r1, [r4, #1]  */
L_307e:
    movs r0, #2                             /* @307E 0220 movs r0, #2  */
L_3080:
    b L_30ea                                /* @3080 33e0 b #102 -> 0x30EA */
L_3082:
    movs r1, #125                           /* @3082 7d21 movs r1, #125  */
L_3084:
    lsls r1, r1, #3                         /* @3084 c900 lsls r1, r1, #3  */
L_3086:
    cmp r5, r1                              /* @3086 8d42 cmp r5, r1  */
L_3088:
    bhs L_30b2                              /* @3088 13d2 bhs #38 -> 0x30B2 */
L_308a:
    movs r1, #100                           /* @308A 6421 movs r1, #100  */
L_308c:
    mov r0, r5                              /* @308C 2846 mov r0, r5  */
L_308e:
    bl L_0110                               /* @308E fdf73ff8 bl #-12162 -> udivmod32 */
L_3092:
    adds r0, #48                            /* @3092 3030 adds r0, #48  */
L_3094:
    strb r0, [r4]                           /* @3094 2070 strb r0, [r4]  */
L_3096:
    mov r0, r1                              /* @3096 0846 mov r0, r1  */
L_3098:
    movs r1, #10                            /* @3098 0a21 movs r1, #10  */
L_309a:
    bl L_0110                               /* @309A fdf739f8 bl #-12174 -> udivmod32 */
L_309e:
    adds r0, #48                            /* @309E 3030 adds r0, #48  */
L_30a0:
    strb r0, [r4, #1]                       /* @30A0 6070 strb r0, [r4, #1]  */
L_30a2:
    movs r1, #10                            /* @30A2 0a21 movs r1, #10  */
L_30a4:
    mov r0, r5                              /* @30A4 2846 mov r0, r5  */
L_30a6:
    bl L_0110                               /* @30A6 fdf733f8 bl #-12186 -> udivmod32 */
L_30aa:
    adds r1, #48                            /* @30AA 3031 adds r1, #48  */
L_30ac:
    strb r1, [r4, #2]                       /* @30AC a170 strb r1, [r4, #2]  */
L_30ae:
    movs r0, #3                             /* @30AE 0320 movs r0, #3  */
L_30b0:
    b L_30ea                                /* @30B0 1be0 b #54 -> 0x30EA */
L_30b2:
    mov r0, r5                              /* @30B2 2846 mov r0, r5  */
L_30b4:
    bl L_0110                               /* @30B4 fdf72cf8 bl #-12200 -> udivmod32 */
L_30b8:
    adds r0, #48                            /* @30B8 3030 adds r0, #48  */
L_30ba:
    strb r0, [r4]                           /* @30BA 2070 strb r0, [r4]  */
L_30bc:
    mov r0, r1                              /* @30BC 0846 mov r0, r1  */
L_30be:
    movs r1, #100                           /* @30BE 6421 movs r1, #100  */
L_30c0:
    bl L_0110                               /* @30C0 fdf726f8 bl #-12212 -> udivmod32 */
L_30c4:
    adds r0, #48                            /* @30C4 3030 adds r0, #48  */
L_30c6:
    strb r0, [r4, #1]                       /* @30C6 6070 strb r0, [r4, #1]  */
L_30c8:
    movs r1, #100                           /* @30C8 6421 movs r1, #100  */
L_30ca:
    mov r0, r5                              /* @30CA 2846 mov r0, r5  */
L_30cc:
    bl L_0110                               /* @30CC fdf720f8 bl #-12224 -> udivmod32 */
L_30d0:
    mov r0, r1                              /* @30D0 0846 mov r0, r1  */
L_30d2:
    movs r1, #10                            /* @30D2 0a21 movs r1, #10  */
L_30d4:
    bl L_0110                               /* @30D4 fdf71cf8 bl #-12232 -> udivmod32 */
L_30d8:
    adds r0, #48                            /* @30D8 3030 adds r0, #48  */
L_30da:
    strb r0, [r4, #2]                       /* @30DA a070 strb r0, [r4, #2]  */
L_30dc:
    movs r1, #10                            /* @30DC 0a21 movs r1, #10  */
L_30de:
    mov r0, r5                              /* @30DE 2846 mov r0, r5  */
L_30e0:
    bl L_0110                               /* @30E0 fdf716f8 bl #-12244 -> udivmod32 */
L_30e4:
    adds r1, #48                            /* @30E4 3031 adds r1, #48  */
L_30e6:
    strb r1, [r4, #3]                       /* @30E6 e170 strb r1, [r4, #3]  */
L_30e8:
    movs r0, #4                             /* @30E8 0420 movs r0, #4  */
L_30ea:
    movs r1, #0                             /* @30EA 0021 movs r1, #0  */
L_30ec:
    b L_30f4                                /* @30EC 02e0 b #4 -> 0x30F4 */
L_30ee:
    strb r1, [r4, r0]                       /* @30EE 2154 strb r1, [r4, r0]  */
L_30f0:
    adds r0, r0, #1                         /* @30F0 401c adds r0, r0, #1  */
L_30f2:
    uxth r0, r0                             /* @30F2 80b2 uxth r0, r0  */
L_30f4:
    cmp r0, #9                              /* @30F4 0928 cmp r0, #9  */
L_30f6:
    blo L_30ee                              /* @30F6 fad3 blo #-12 -> 0x30EE */
L_30f8:
    pop {r4, r5, r6, pc}                    /* @30F8 70bd pop {r4, r5, r6, pc}  */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @30FA unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @310A unclassified */
    .byte 0x00,0x00 /* @311A unclassified */

/* sub_311C */
L_311c:
    push {r4, r5, r6, lr}                   /* @311C 70b5 push {r4, r5, r6, lr}  */
L_311e:
    bl L_4584                               /* @311E 01f031fa bl #5218 -> sub_4584 */
L_3122:
    movs r1, #9                             /* @3122 0921 movs r1, #9  */
L_3124:
    movs r0, #0                             /* @3124 0020 movs r0, #0  */
L_3126:
    bl L_135c                               /* @3126 fef719f9 bl #-7630 -> sub_135C */
L_312a:
    movs r6, #0                             /* @312A 0026 movs r6, #0  */
L_312c:
    ldr r5, [pc, #204]                      /* @312C 334d ldr r5, [pc, #204] literal 0x31FC -> 0x20000628 */
L_312e:
    ldr r4, [pc, #208]                      /* @312E 344c ldr r4, [pc, #208] literal 0x3200 -> 0x200005DC */
L_3130:
    cmp r0, #0                              /* @3130 0028 cmp r0, #0  */
L_3132:
    bne L_3174                              /* @3132 1fd1 bne #62 -> 0x3174 */
L_3134:
    ldrb r0, [r4, #1]                       /* @3134 6078 ldrb r0, [r4, #1]  */
L_3136:
    lsls r1, r0, #31                        /* @3136 c107 lsls r1, r0, #31  */
L_3138:
    bne L_3174                              /* @3138 1cd1 bne #56 -> 0x3174 */
L_313a:
    movs r1, #1                             /* @313A 0121 movs r1, #1  */
L_313c:
    orrs r0, r1                             /* @313C 0843 orrs r0, r1  */
L_313e:
    strb r0, [r4, #1]                       /* @313E 6070 strb r0, [r4, #1]  */
L_3140:
    strh r1, [r4, #4]                       /* @3140 a180 strh r1, [r4, #4]  */
L_3142:
    ldr r0, [r5, #8]                        /* @3142 a868 ldr r0, [r5, #8]  */
L_3144:
    cmp r0, #0                              /* @3144 0028 cmp r0, #0  */
L_3146:
    beq L_315e                              /* @3146 0ad0 beq #20 -> 0x315E */
L_3148:
    ldrb r1, [r5, #1]                       /* @3148 6978 ldrb r1, [r5, #1]  */
L_314a:
    adds r1, r1, #1                         /* @314A 491c adds r1, r1, #1  */
L_314c:
    uxtb r1, r1                             /* @314C c9b2 uxtb r1, r1  */
L_314e:
    strb r1, [r5, #1]                       /* @314E 6970 strb r1, [r5, #1]  */
L_3150:
    lsls r2, r1, #3                         /* @3150 ca00 lsls r2, r1, #3  */
L_3152:
    adds r1, r1, r2                         /* @3152 8918 adds r1, r1, r2  */
L_3154:
    ldrb r0, [r0, r1]                       /* @3154 405c ldrb r0, [r0, r1]  */
L_3156:
    cmp r0, #0                              /* @3156 0028 cmp r0, #0  */
L_3158:
    bne L_3174                              /* @3158 0cd1 bne #24 -> 0x3174 */
L_315a:
    strb r6, [r5, #1]                       /* @315A 6e70 strb r6, [r5, #1]  */
L_315c:
    b L_3174                                /* @315C 0ae0 b #20 -> 0x3174 */
L_315e:
    ldrb r0, [r5]                           /* @315E 2878 ldrb r0, [r5]  */
L_3160:
    adds r0, r0, #1                         /* @3160 401c adds r0, r0, #1  */
L_3162:
    uxtb r0, r0                             /* @3162 c0b2 uxtb r0, r0  */
L_3164:
    strb r0, [r5]                           /* @3164 2870 strb r0, [r5]  */
L_3166:
    movs r2, #20                            /* @3166 1422 movs r2, #20  */
L_3168:
    ldr r1, [r5, #4]                        /* @3168 6968 ldr r1, [r5, #4]  */
L_316a:
    muls r0, r2, r0                         /* @316A 5043 muls r0, r2, r0  */
L_316c:
    ldrb r0, [r1, r0]                       /* @316C 085c ldrb r0, [r1, r0]  */
L_316e:
    cmp r0, #0                              /* @316E 0028 cmp r0, #0  */
L_3170:
    bne L_3174                              /* @3170 00d1 bne #0 -> 0x3174 */
L_3172:
    strb r6, [r5]                           /* @3172 2e70 strb r6, [r5]  */
L_3174:
    movs r1, #10                            /* @3174 0a21 movs r1, #10  */
L_3176:
    movs r0, #0                             /* @3176 0020 movs r0, #0  */
L_3178:
    bl L_135c                               /* @3178 fef7f0f8 bl #-7712 -> sub_135C */
L_317c:
    cmp r0, #0                              /* @317C 0028 cmp r0, #0  */
L_317e:
    bne L_31d0                              /* @317E 27d1 bne #78 -> 0x31D0 */
L_3180:
    ldrb r0, [r4, #1]                       /* @3180 6078 ldrb r0, [r4, #1]  */
L_3182:
    lsls r1, r0, #30                        /* @3182 8107 lsls r1, r0, #30  */
L_3184:
    bmi L_31d0                              /* @3184 24d4 bmi #72 -> 0x31D0 */
L_3186:
    movs r1, #2                             /* @3186 0221 movs r1, #2  */
L_3188:
    orrs r0, r1                             /* @3188 0843 orrs r0, r1  */
L_318a:
    strb r0, [r4, #1]                       /* @318A 6070 strb r0, [r4, #1]  */
L_318c:
    strh r6, [r4, #4]                       /* @318C a680 strh r6, [r4, #4]  */
L_318e:
    ldr r0, [r5, #8]                        /* @318E a868 ldr r0, [r5, #8]  */
L_3190:
    cmp r0, #0                              /* @3190 0028 cmp r0, #0  */
L_3192:
    beq L_31a8                              /* @3192 09d0 beq #18 -> 0x31A8 */
L_3194:
    ldrb r0, [r5]                           /* @3194 2878 ldrb r0, [r5]  */
L_3196:
    movs r2, #20                            /* @3196 1422 movs r2, #20  */
L_3198:
    muls r0, r2, r0                         /* @3198 5043 muls r0, r2, r0  */
L_319a:
    ldr r1, [r5, #4]                        /* @319A 6968 ldr r1, [r5, #4]  */
L_319c:
    adds r0, #12                            /* @319C 0c30 adds r0, #12  */
L_319e:
    ldr r1, [r1, r0]                        /* @319E 0958 ldr r1, [r1, r0]  */
L_31a0:
    ldrb r0, [r5, #1]                       /* @31A0 6878 ldrb r0, [r5, #1]  */
L_31a2:
    blx r1                                  /* @31A2 8847 blx r1  */
L_31a4:
    str r6, [r5, #8]                        /* @31A4 ae60 str r6, [r5, #8]  */
L_31a6:
    b L_31d0                                /* @31A6 13e0 b #38 -> 0x31D0 */
L_31a8:
    ldrb r0, [r5]                           /* @31A8 2878 ldrb r0, [r5]  */
L_31aa:
    movs r2, #20                            /* @31AA 1422 movs r2, #20  */
L_31ac:
    muls r0, r2, r0                         /* @31AC 5043 muls r0, r2, r0  */
L_31ae:
    mov r2, r0                              /* @31AE 0246 mov r2, r0  */
L_31b0:
    ldr r1, [r5, #4]                        /* @31B0 6968 ldr r1, [r5, #4]  */
L_31b2:
    adds r2, #16                            /* @31B2 1032 adds r2, #16  */
L_31b4:
    ldr r2, [r1, r2]                        /* @31B4 8a58 ldr r2, [r1, r2]  */
L_31b6:
    cmp r2, #0                              /* @31B6 002a cmp r2, #0  */
L_31b8:
    beq L_31c8                              /* @31B8 06d0 beq #12 -> 0x31C8 */
L_31ba:
    str r2, [r5, #8]                        /* @31BA aa60 str r2, [r5, #8]  */
L_31bc:
    adds r0, #12                            /* @31BC 0c30 adds r0, #12  */
L_31be:
    ldr r1, [r1, r0]                        /* @31BE 0958 ldr r1, [r1, r0]  */
L_31c0:
    movs r0, #255                           /* @31C0 ff20 movs r0, #255  */
L_31c2:
    blx r1                                  /* @31C2 8847 blx r1  */
L_31c4:
    strb r0, [r5, #1]                       /* @31C4 6870 strb r0, [r5, #1]  */
L_31c6:
    b L_31d0                                /* @31C6 03e0 b #6 -> 0x31D0 */
L_31c8:
    adds r0, #12                            /* @31C8 0c30 adds r0, #12  */
L_31ca:
    ldr r1, [r1, r0]                        /* @31CA 0958 ldr r1, [r1, r0]  */
L_31cc:
    movs r0, #255                           /* @31CC ff20 movs r0, #255  */
L_31ce:
    blx r1                                  /* @31CE 8847 blx r1  */
L_31d0:
    movs r1, #9                             /* @31D0 0921 movs r1, #9  */
L_31d2:
    movs r0, #0                             /* @31D2 0020 movs r0, #0  */
L_31d4:
    bl L_135c                               /* @31D4 fef7c2f8 bl #-7804 -> sub_135C */
L_31d8:
    cmp r0, #1                              /* @31D8 0128 cmp r0, #1  */
L_31da:
    bne L_31e4                              /* @31DA 03d1 bne #6 -> 0x31E4 */
L_31dc:
    ldrb r0, [r4, #1]                       /* @31DC 6078 ldrb r0, [r4, #1]  */
L_31de:
    lsrs r0, r0, #1                         /* @31DE 4008 lsrs r0, r0, #1  */
L_31e0:
    lsls r0, r0, #1                         /* @31E0 4000 lsls r0, r0, #1  */
L_31e2:
    strb r0, [r4, #1]                       /* @31E2 6070 strb r0, [r4, #1]  */
L_31e4:
    movs r1, #10                            /* @31E4 0a21 movs r1, #10  */
L_31e6:
    movs r0, #0                             /* @31E6 0020 movs r0, #0  */
L_31e8:
    bl L_135c                               /* @31E8 fef7b8f8 bl #-7824 -> sub_135C */
L_31ec:
    cmp r0, #1                              /* @31EC 0128 cmp r0, #1  */
L_31ee:
    bne L_31f8                              /* @31EE 03d1 bne #6 -> 0x31F8 */
L_31f0:
    ldrb r0, [r4, #1]                       /* @31F0 6078 ldrb r0, [r4, #1]  */
L_31f2:
    movs r1, #253                           /* @31F2 fd21 movs r1, #253  */
L_31f4:
    ands r0, r1                             /* @31F4 0840 ands r0, r1  */
L_31f6:
    strb r0, [r4, #1]                       /* @31F6 6070 strb r0, [r4, #1]  */
L_31f8:
    pop {r4, r5, r6, pc}                    /* @31F8 70bd pop {r4, r5, r6, pc}  */
    .byte 0x00,0x00 /* @31FA unclassified */
L_31fc:
    .byte 0x28,0x06,0x00,0x20 /* @31FC literal */
L_3200:
    .byte 0xdc,0x05,0x00,0x20 /* @3200 literal */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @3204 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @3214 unclassified */

/* sub_3220 */
L_3220:
    push {r4, r5, r6, r7, lr}               /* @3220 f0b5 push {r4, r5, r6, r7, lr}  */
L_3222:
    sub sp, #44                             /* @3222 8bb0 sub sp, #44  */
L_3224:
    movs r1, #32                            /* @3224 2021 movs r1, #32  */
L_3226:
    add r0, sp, #8                          /* @3226 02a8 add r0, sp, #8  */
L_3228:
    bl L_025c                               /* @3228 fdf718f8 bl #-12240 -> sub_025C */
L_322c:
    ldr r0, [pc, #448]                      /* @322C 7048 ldr r0, [pc, #448] literal 0x33F0 -> 0x20000680 */
L_322e:
    ldr r1, [r0, #32]                       /* @322E 016a ldr r1, [r0, #32]  */
L_3230:
    ldr r5, [r0, #36]                       /* @3230 456a ldr r5, [r0, #36]  */
L_3232:
    ldr r7, [pc, #448]                      /* @3232 704f ldr r7, [pc, #448] literal 0x33F4 -> 0x20000648 */
L_3234:
    ldr r0, [r7, #32]                       /* @3234 386a ldr r0, [r7, #32]  */
L_3236:
    asrs r2, r0, #31                        /* @3236 c217 asrs r2, r0, #31  */
L_3238:
    subs r4, r1, r0                         /* @3238 0c1a subs r4, r1, r0  */
L_323a:
    sbcs r5, r2                             /* @323A 9541 sbcs r5, r2  */
L_323c:
    cmp r5, #0                              /* @323C 002d cmp r5, #0  */
L_323e:
    bge L_324e                              /* @323E 06da bge #12 -> 0x324E */
L_3240:
    movs r0, #45                            /* @3240 2d20 movs r0, #45  */
L_3242:
    mov r1, sp                              /* @3242 6946 mov r1, sp  */
L_3244:
    strb r0, [r1, #8]                       /* @3244 0872 strb r0, [r1, #8]  */
L_3246:
    mov r0, r5                              /* @3246 2846 mov r0, r5  */
L_3248:
    movs r5, #0                             /* @3248 0025 movs r5, #0  */
L_324a:
    rsbs r4, r4, #0                         /* @324A 6442 rsbs r4, r4, #0  */
L_324c:
    sbcs r5, r0                             /* @324C 8541 sbcs r5, r0  */
L_324e:
    movs r2, #225                           /* @324E e122 movs r2, #225  */
L_3250:
    movs r3, #0                             /* @3250 0023 movs r3, #0  */
L_3252:
    mov r0, r4                              /* @3252 2046 mov r0, r4  */
L_3254:
    mov r1, r5                              /* @3254 2946 mov r1, r5  */
L_3256:
    bl L_01b0                               /* @3256 fcf7abff bl #-12458 -> sub_01B0 */
L_325a:
    lsls r2, r1, #20                        /* @325A 0a05 lsls r2, r1, #20  */
L_325c:
    lsrs r4, r0, #12                        /* @325C 040b lsrs r4, r0, #12  */
L_325e:
    orrs r4, r2                             /* @325E 1443 orrs r4, r2  */
L_3260:
    asrs r5, r1, #12                        /* @3260 0d13 asrs r5, r1, #12  */
L_3262:
    movs r6, #0                             /* @3262 0026 movs r6, #0  */
L_3264:
    ldr r1, [pc, #400]                      /* @3264 6449 ldr r1, [pc, #400] literal 0x33F8 -> 0x0000270F */
L_3266:
    movs r0, #0                             /* @3266 0020 movs r0, #0  */
L_3268:
    subs r1, r1, r4                         /* @3268 091b subs r1, r1, r4  */
L_326a:
    sbcs r0, r5                             /* @326A a841 sbcs r0, r5  */
L_326c:
    bge L_32aa                              /* @326C 1dda bge #58 -> 0x32AA */
L_326e:
    movs r6, #1                             /* @326E 0126 movs r6, #1  */
L_3270:
    movs r2, #125                           /* @3270 7d22 movs r2, #125  */
L_3272:
    lsls r2, r2, #3                         /* @3272 d200 lsls r2, r2, #3  */
L_3274:
    movs r3, #0                             /* @3274 0023 movs r3, #0  */
L_3276:
    mov r0, r4                              /* @3276 2046 mov r0, r4  */
L_3278:
    mov r1, r5                              /* @3278 2946 mov r1, r5  */
L_327a:
    bl L_0164                               /* @327A fcf773ff bl #-12570 -> signed_divmod64_wrapper */
L_327e:
    mov r4, r0                              /* @327E 0446 mov r4, r0  */
L_3280:
    mov r5, r1                              /* @3280 0d46 mov r5, r1  */
L_3282:
    ldr r1, [pc, #372]                      /* @3282 5d49 ldr r1, [pc, #372] literal 0x33F8 -> 0x0000270F */
L_3284:
    movs r0, #0                             /* @3284 0020 movs r0, #0  */
L_3286:
    subs r1, r1, r4                         /* @3286 091b subs r1, r1, r4  */
L_3288:
    sbcs r0, r5                             /* @3288 a841 sbcs r0, r5  */
L_328a:
    bge L_32aa                              /* @328A 0eda bge #28 -> 0x32AA */
L_328c:
    mov r0, sp                              /* @328C 6846 mov r0, sp  */
L_328e:
    ldrb r0, [r0, #8]                       /* @328E 007a ldrb r0, [r0, #8]  */
L_3290:
    cmp r0, #45                             /* @3290 2d28 cmp r0, #45  */
L_3292:
    beq L_329e                              /* @3292 04d0 beq #8 -> 0x329E */
L_3294:
    ldr r1, [pc, #356]                      /* @3294 5949 ldr r1, [pc, #356] literal 0x33FC -> 0x00005DE8 */
L_3296:
    add r0, sp, #8                          /* @3296 02a8 add r0, sp, #8  */
L_3298:
    bl L_0280                               /* @3298 fcf7f2ff bl #-12316 -> sub_0280 */
L_329c:
    b L_32a8                                /* @329C 04e0 b #8 -> 0x32A8 */
L_329e:
    ldr r1, [pc, #348]                      /* @329E 5749 ldr r1, [pc, #348] literal 0x33FC -> 0x00005DE8 */
L_32a0:
    add r0, sp, #8                          /* @32A0 02a8 add r0, sp, #8  */
L_32a2:
    adds r1, #9                             /* @32A2 0931 adds r1, #9  */
L_32a4:
    bl L_0280                               /* @32A4 fcf7ecff bl #-12328 -> sub_0280 */
L_32a8:
    movs r6, #255                           /* @32A8 ff26 movs r6, #255  */
L_32aa:
    cmp r6, #2                              /* @32AA 022e cmp r6, #2  */
L_32ac:
    bhs L_32cc                              /* @32AC 0ed2 bhs #28 -> 0x32CC */
L_32ae:
    movs r2, #10                            /* @32AE 0a22 movs r2, #10  */
L_32b0:
    movs r3, #0                             /* @32B0 0023 movs r3, #0  */
L_32b2:
    mov r0, r4                              /* @32B2 2046 mov r0, r4  */
L_32b4:
    mov r1, r5                              /* @32B4 2946 mov r1, r5  */
L_32b6:
    bl L_0164                               /* @32B6 fcf755ff bl #-12630 -> signed_divmod64_wrapper */
L_32ba:
    movs r1, #10                            /* @32BA 0a21 movs r1, #10  */
L_32bc:
    muls r1, r0, r1                         /* @32BC 4143 muls r1, r0, r1  */
L_32be:
    subs r1, r4, r1                         /* @32BE 611a subs r1, r4, r1  */
L_32c0:
    add r2, sp, #8                          /* @32C0 02aa add r2, sp, #8  */
L_32c2:
    str r2, [sp]                            /* @32C2 0092 str r2, [sp]  */
L_32c4:
    movs r3, #0                             /* @32C4 0023 movs r3, #0  */
L_32c6:
    mov r2, r6                              /* @32C6 3246 mov r2, r6  */
L_32c8:
    bl L_5d08                               /* @32C8 02f01efd bl #10812 -> sub_5D08 */
L_32cc:
    ldr r0, [r7, #36]                       /* @32CC 786a ldr r0, [r7, #36]  */
L_32ce:
    cmp r0, #0                              /* @32CE 0028 cmp r0, #0  */
L_32d0:
    bge L_32d4                              /* @32D0 00da bge #0 -> 0x32D4 */
L_32d2:
    rsbs r0, r0, #0                         /* @32D2 4042 rsbs r0, r0, #0  */
L_32d4:
    ldr r1, [pc, #296]                      /* @32D4 4a49 ldr r1, [pc, #296] literal 0x3400 -> 0x00000465 */
L_32d6:
    muls r0, r1, r0                         /* @32D6 4843 muls r0, r1, r0  */
L_32d8:
    asrs r4, r0, #11                        /* @32D8 c412 asrs r4, r0, #11  */
L_32da:
    asrs r5, r4, #31                        /* @32DA e517 asrs r5, r4, #31  */
L_32dc:
    movs r6, #3                             /* @32DC 0326 movs r6, #3  */
L_32de:
    add r0, sp, #16                         /* @32DE 04a8 add r0, sp, #16  */
L_32e0:
    ldr r2, [pc, #288]                      /* @32E0 484a ldr r2, [pc, #288] literal 0x3404 -> 0x0001869F */
L_32e2:
    adds r0, #2                             /* @32E2 0230 adds r0, #2  */
L_32e4:
    str r0, [sp, #40]                       /* @32E4 0a90 str r0, [sp, #40]  */
L_32e6:
    movs r1, #0                             /* @32E6 0021 movs r1, #0  */
L_32e8:
    subs r0, r2, r4                         /* @32E8 101b subs r0, r2, r4  */
L_32ea:
    sbcs r1, r5                             /* @32EA a941 sbcs r1, r5  */
L_32ec:
    bge L_331a                              /* @32EC 15da bge #42 -> 0x331A */
L_32ee:
    movs r6, #1                             /* @32EE 0126 movs r6, #1  */
L_32f0:
    movs r2, #125                           /* @32F0 7d22 movs r2, #125  */
L_32f2:
    lsls r2, r2, #3                         /* @32F2 d200 lsls r2, r2, #3  */
L_32f4:
    movs r3, #0                             /* @32F4 0023 movs r3, #0  */
L_32f6:
    mov r0, r4                              /* @32F6 2046 mov r0, r4  */
L_32f8:
    mov r1, r5                              /* @32F8 2946 mov r1, r5  */
L_32fa:
    bl L_0164                               /* @32FA fcf733ff bl #-12698 -> signed_divmod64_wrapper */
L_32fe:
    mov r4, r0                              /* @32FE 0446 mov r4, r0  */
L_3300:
    mov r5, r1                              /* @3300 0d46 mov r5, r1  */
L_3302:
    ldr r1, [pc, #244]                      /* @3302 3d49 ldr r1, [pc, #244] literal 0x33F8 -> 0x0000270F */
L_3304:
    movs r0, #0                             /* @3304 0020 movs r0, #0  */
L_3306:
    subs r1, r1, r4                         /* @3306 091b subs r1, r1, r4  */
L_3308:
    sbcs r0, r5                             /* @3308 a841 sbcs r0, r5  */
L_330a:
    bge L_3326                              /* @330A 0cda bge #24 -> 0x3326 */
L_330c:
    ldr r1, [pc, #236]                      /* @330C 3b49 ldr r1, [pc, #236] literal 0x33FC -> 0x00005DE8 */
L_330e:
    ldr r0, [sp, #40]                       /* @330E 0a98 ldr r0, [sp, #40]  */
L_3310:
    adds r1, #18                            /* @3310 1231 adds r1, #18  */
L_3312:
    bl L_0280                               /* @3312 fcf7b5ff bl #-12438 -> sub_0280 */
L_3316:
    movs r6, #255                           /* @3316 ff26 movs r6, #255  */
L_3318:
    b L_3326                                /* @3318 05e0 b #10 -> 0x3326 */
L_331a:
    ldr r1, [pc, #220]                      /* @331A 3749 ldr r1, [pc, #220] literal 0x33F8 -> 0x0000270F */
L_331c:
    movs r0, #0                             /* @331C 0020 movs r0, #0  */
L_331e:
    subs r1, r1, r4                         /* @331E 091b subs r1, r1, r4  */
L_3320:
    sbcs r0, r5                             /* @3320 a841 sbcs r0, r5  */
L_3322:
    bge L_3326                              /* @3322 00da bge #0 -> 0x3326 */
L_3324:
    movs r6, #2                             /* @3324 0226 movs r6, #2  */
L_3326:
    cmp r6, #4                              /* @3326 042e cmp r6, #4  */
L_3328:
    bhs L_3348                              /* @3328 0ed2 bhs #28 -> 0x3348 */
L_332a:
    movs r2, #100                           /* @332A 6422 movs r2, #100  */
L_332c:
    movs r3, #0                             /* @332C 0023 movs r3, #0  */
L_332e:
    mov r0, r4                              /* @332E 2046 mov r0, r4  */
L_3330:
    mov r1, r5                              /* @3330 2946 mov r1, r5  */
L_3332:
    bl L_0164                               /* @3332 fcf717ff bl #-12754 -> signed_divmod64_wrapper */
L_3336:
    movs r1, #100                           /* @3336 6421 movs r1, #100  */
L_3338:
    muls r1, r0, r1                         /* @3338 4143 muls r1, r0, r1  */
L_333a:
    subs r1, r4, r1                         /* @333A 611a subs r1, r4, r1  */
L_333c:
    ldr r2, [sp, #40]                       /* @333C 0a9a ldr r2, [sp, #40]  */
L_333e:
    str r2, [sp]                            /* @333E 0092 str r2, [sp]  */
L_3340:
    movs r3, #1                             /* @3340 0123 movs r3, #1  */
L_3342:
    mov r2, r6                              /* @3342 3246 mov r2, r6  */
L_3344:
    bl L_5d08                               /* @3344 02f0e0fc bl #10688 -> sub_5D08 */
L_3348:
    movs r0, #0                             /* @3348 0020 movs r0, #0  */
L_334a:
    str r0, [sp, #4]                        /* @334A 0190 str r0, [sp, #4]  */
L_334c:
    ldr r0, [pc, #160]                      /* @334C 2848 ldr r0, [pc, #160] literal 0x33F0 -> 0x20000680 */
L_334e:
    ldr r1, [r0]                            /* @334E 0168 ldr r1, [r0]  */
L_3350:
    ldr r5, [r0, #4]                        /* @3350 4568 ldr r5, [r0, #4]  */
L_3352:
    ldr r0, [r7, #28]                       /* @3352 f869 ldr r0, [r7, #28]  */
L_3354:
    asrs r2, r0, #31                        /* @3354 c217 asrs r2, r0, #31  */
L_3356:
    subs r4, r1, r0                         /* @3356 0c1a subs r4, r1, r0  */
L_3358:
    sbcs r5, r2                             /* @3358 9541 sbcs r5, r2  */
L_335a:
    cmp r5, #0                              /* @335A 002d cmp r5, #0  */
L_335c:
    bge L_336c                              /* @335C 06da bge #12 -> 0x336C */
L_335e:
    movs r0, #45                            /* @335E 2d20 movs r0, #45  */
L_3360:
    mov r1, sp                              /* @3360 6946 mov r1, sp  */
L_3362:
    strb r0, [r1, #28]                      /* @3362 0877 strb r0, [r1, #28]  */
L_3364:
    mov r0, r5                              /* @3364 2846 mov r0, r5  */
L_3366:
    movs r5, #0                             /* @3366 0025 movs r5, #0  */
L_3368:
    rsbs r4, r4, #0                         /* @3368 6442 rsbs r4, r4, #0  */
L_336a:
    sbcs r5, r0                             /* @336A 8541 sbcs r5, r0  */
L_336c:
    movs r6, #4                             /* @336C 0426 movs r6, #4  */
L_336e:
    ldr r1, [pc, #144]                      /* @336E 2449 ldr r1, [pc, #144] literal 0x3400 -> 0x00000465 */
L_3370:
    movs r0, #0                             /* @3370 0020 movs r0, #0  */
L_3372:
    subs r1, #126                           /* @3372 7e39 subs r1, #126  */
L_3374:
    subs r1, r1, r4                         /* @3374 091b subs r1, r1, r4  */
L_3376:
    add r7, sp, #28                         /* @3376 07af add r7, sp, #28  */
L_3378:
    sbcs r0, r5                             /* @3378 a841 sbcs r0, r5  */
L_337a:
    bge L_33d0                              /* @337A 29da bge #82 -> 0x33D0 */
L_337c:
    movs r6, #1                             /* @337C 0126 movs r6, #1  */
L_337e:
    movs r2, #100                           /* @337E 6422 movs r2, #100  */
L_3380:
    movs r3, #0                             /* @3380 0023 movs r3, #0  */
L_3382:
    mov r0, r4                              /* @3382 2046 mov r0, r4  */
L_3384:
    mov r1, r5                              /* @3384 2946 mov r1, r5  */
L_3386:
    bl L_0164                               /* @3386 fcf7edfe bl #-12838 -> signed_divmod64_wrapper */
L_338a:
    movs r2, #10                            /* @338A 0a22 movs r2, #10  */
L_338c:
    movs r3, #0                             /* @338C 0023 movs r3, #0  */
L_338e:
    bl L_0164                               /* @338E fcf7e9fe bl #-12846 -> signed_divmod64_wrapper */
L_3392:
    str r2, [sp, #4]                        /* @3392 0192 str r2, [sp, #4]  */
L_3394:
    movs r2, #125                           /* @3394 7d22 movs r2, #125  */
L_3396:
    lsls r2, r2, #3                         /* @3396 d200 lsls r2, r2, #3  */
L_3398:
    movs r3, #0                             /* @3398 0023 movs r3, #0  */
L_339a:
    mov r0, r4                              /* @339A 2046 mov r0, r4  */
L_339c:
    mov r1, r5                              /* @339C 2946 mov r1, r5  */
L_339e:
    bl L_0164                               /* @339E fcf7e1fe bl #-12862 -> signed_divmod64_wrapper */
L_33a2:
    mov r4, r0                              /* @33A2 0446 mov r4, r0  */
L_33a4:
    mov r5, r1                              /* @33A4 0d46 mov r5, r1  */
L_33a6:
    movs r1, #99                            /* @33A6 6321 movs r1, #99  */
L_33a8:
    movs r0, #0                             /* @33A8 0020 movs r0, #0  */
L_33aa:
    subs r1, r1, r4                         /* @33AA 091b subs r1, r1, r4  */
L_33ac:
    sbcs r0, r5                             /* @33AC a841 sbcs r0, r5  */
L_33ae:
    bge L_33d0                              /* @33AE 0fda bge #30 -> 0x33D0 */
L_33b0:
    mov r0, sp                              /* @33B0 6846 mov r0, sp  */
L_33b2:
    ldrb r0, [r0, #28]                      /* @33B2 007f ldrb r0, [r0, #28]  */
L_33b4:
    cmp r0, #45                             /* @33B4 2d28 cmp r0, #45  */
L_33b6:
    beq L_33c4                              /* @33B6 05d0 beq #10 -> 0x33C4 */
L_33b8:
    ldr r1, [pc, #64]                       /* @33B8 1049 ldr r1, [pc, #64] literal 0x33FC -> 0x00005DE8 */
L_33ba:
    mov r0, r7                              /* @33BA 3846 mov r0, r7  */
L_33bc:
    adds r1, #27                            /* @33BC 1b31 adds r1, #27  */
L_33be:
    bl L_0280                               /* @33BE fcf75fff bl #-12610 -> sub_0280 */
L_33c2:
    b L_33ce                                /* @33C2 04e0 b #8 -> 0x33CE */
L_33c4:
    ldr r1, [pc, #52]                       /* @33C4 0d49 ldr r1, [pc, #52] literal 0x33FC -> 0x00005DE8 */
L_33c6:
    mov r0, r7                              /* @33C6 3846 mov r0, r7  */
L_33c8:
    adds r1, #36                            /* @33C8 2431 adds r1, #36  */
L_33ca:
    bl L_0280                               /* @33CA fcf759ff bl #-12622 -> sub_0280 */
L_33ce:
    movs r6, #255                           /* @33CE ff26 movs r6, #255  */
L_33d0:
    cmp r6, #5                              /* @33D0 052e cmp r6, #5  */
L_33d2:
    bhs L_33e2                              /* @33D2 06d2 bhs #12 -> 0x33E2 */
L_33d4:
    movs r3, #2                             /* @33D4 0223 movs r3, #2  */
L_33d6:
    mov r2, r6                              /* @33D6 3246 mov r2, r6  */
L_33d8:
    str r7, [sp]                            /* @33D8 0097 str r7, [sp]  */
L_33da:
    mov r0, r4                              /* @33DA 2046 mov r0, r4  */
L_33dc:
    ldr r1, [sp, #4]                        /* @33DC 0199 ldr r1, [sp, #4]  */
L_33de:
    bl L_5d08                               /* @33DE 02f093fc bl #10534 -> sub_5D08 */
L_33e2:
    mov r2, r7                              /* @33E2 3a46 mov r2, r7  */
L_33e4:
    add r0, sp, #8                          /* @33E4 02a8 add r0, sp, #8  */
L_33e6:
    ldr r1, [sp, #40]                       /* @33E6 0a99 ldr r1, [sp, #40]  */
L_33e8:
    bl L_4884                               /* @33E8 01f04cfa bl #5272 -> display_three_lines */
L_33ec:
    add sp, #44                             /* @33EC 0bb0 add sp, #44  */
L_33ee:
    pop {r4, r5, r6, r7, pc}                /* @33EE f0bd pop {r4, r5, r6, r7, pc}  */
L_33f0:
    .byte 0x80,0x06,0x00,0x20 /* @33F0 literal */
L_33f4:
    .byte 0x48,0x06,0x00,0x20 /* @33F4 literal */
L_33f8:
    .byte 0x0f,0x27,0x00,0x00 /* @33F8 literal */
L_33fc:
    .byte 0xe8,0x5d,0x00,0x00 /* @33FC literal */
L_3400:
    .byte 0x65,0x04,0x00,0x00 /* @3400 literal */
L_3404:
    .byte 0x9f,0x86,0x01,0x00 /* @3404 literal */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @3408 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @3418 unclassified */
    .byte 0x00,0x00,0x00,0x00 /* @3428 unclassified */

/* sub_342C */
L_342c:
    ldr r1, [pc, #56]                       /* @342C 0e49 ldr r1, [pc, #56] literal 0x3468 -> 0x40000C00 */
L_342e:
    ldr r0, [r1, #60]                       /* @342E c86b ldr r0, [r1, #60]  */
L_3430:
    lsrs r2, r0, #16                        /* @3430 020c lsrs r2, r0, #16  */
L_3432:
    movs r0, #75                            /* @3432 4b20 movs r0, #75  */
L_3434:
    lsls r2, r2, #16                        /* @3434 1204 lsls r2, r2, #16  */
L_3436:
    lsls r0, r0, #4                         /* @3436 0001 lsls r0, r0, #4  */
L_3438:
    adds r2, r2, r0                         /* @3438 1218 adds r2, r2, r0  */
L_343a:
    str r2, [r1, #60]                       /* @343A ca63 str r2, [r1, #60]  */
L_343c:
    ldr r1, [pc, #40]                       /* @343C 0a49 ldr r1, [pc, #40] literal 0x3468 -> 0x40000C00 */
L_343e:
    adds r1, #64                            /* @343E 4031 adds r1, #64  */
L_3440:
    ldr r2, [r1]                            /* @3440 0a68 ldr r2, [r1]  */
L_3442:
    lsrs r2, r2, #16                        /* @3442 120c lsrs r2, r2, #16  */
L_3444:
    lsls r2, r2, #16                        /* @3444 1204 lsls r2, r2, #16  */
L_3446:
    adds r2, r2, r0                         /* @3446 1218 adds r2, r2, r0  */
L_3448:
    str r2, [r1]                            /* @3448 0a60 str r2, [r1]  */
L_344a:
    ldr r1, [pc, #32]                       /* @344A 0849 ldr r1, [pc, #32] literal 0x346C -> 0x40000D00 */
L_344c:
    ldr r2, [r1, #60]                       /* @344C ca6b ldr r2, [r1, #60]  */
L_344e:
    lsrs r2, r2, #16                        /* @344E 120c lsrs r2, r2, #16  */
L_3450:
    lsls r2, r2, #16                        /* @3450 1204 lsls r2, r2, #16  */
L_3452:
    adds r2, r2, r0                         /* @3452 1218 adds r2, r2, r0  */
L_3454:
    str r2, [r1, #60]                       /* @3454 ca63 str r2, [r1, #60]  */
L_3456:
    ldr r1, [pc, #20]                       /* @3456 0549 ldr r1, [pc, #20] literal 0x346C -> 0x40000D00 */
L_3458:
    adds r1, #64                            /* @3458 4031 adds r1, #64  */
L_345a:
    ldr r2, [r1]                            /* @345A 0a68 ldr r2, [r1]  */
L_345c:
    lsrs r2, r2, #16                        /* @345C 120c lsrs r2, r2, #16  */
L_345e:
    lsls r2, r2, #16                        /* @345E 1204 lsls r2, r2, #16  */
L_3460:
    adds r0, r2, r0                         /* @3460 1018 adds r0, r2, r0  */
L_3462:
    str r0, [r1]                            /* @3462 0860 str r0, [r1]  */
L_3464:
    bx lr                                   /* @3464 7047 bx lr  */
    .byte 0x00,0x00 /* @3466 unclassified */
L_3468:
    .byte 0x00,0x0c,0x00,0x40 /* @3468 literal */
L_346c:
    .byte 0x00,0x0d,0x00,0x40 /* @346C literal */

/* sub_3470 */
L_3470:
    lsls r2, r0, #22                        /* @3470 8205 lsls r2, r0, #22  */
L_3472:
    ldr r0, [pc, #8]                        /* @3472 0248 ldr r0, [pc, #8] literal 0x347C -> 0x200006C0 */
L_3474:
    lsrs r2, r2, #22                        /* @3474 920d lsrs r2, r2, #22  */
L_3476:
    strh r2, [r0, #2]                       /* @3476 4280 strh r2, [r0, #2]  */
L_3478:
    str r1, [r0, #24]                       /* @3478 8161 str r1, [r0, #24]  */
L_347a:
    bx lr                                   /* @347A 7047 bx lr  */
L_347c:
    .byte 0xc0,0x06,0x00,0x20 /* @347C literal */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @3480 unclassified */

/* sub_3488 */
L_3488:
    ldr r1, [pc, #16]                       /* @3488 0449 ldr r1, [pc, #16] literal 0x349C -> 0x40000C00 */
L_348a:
    ldr r2, [r1, #48]                       /* @348A 0a6b ldr r2, [r1, #48]  */
L_348c:
    asrs r0, r1, #18                        /* @348C 8814 asrs r0, r1, #18  */
L_348e:
    bics r2, r0                             /* @348E 8243 bics r2, r0  */
L_3490:
    str r2, [r1, #48]                       /* @3490 0a63 str r2, [r1, #48]  */
L_3492:
    ldr r1, [pc, #12]                       /* @3492 0349 ldr r1, [pc, #12] literal 0x34A0 -> 0x40000D00 */
L_3494:
    ldr r2, [r1, #48]                       /* @3494 0a6b ldr r2, [r1, #48]  */
L_3496:
    bics r2, r0                             /* @3496 8243 bics r2, r0  */
L_3498:
    str r2, [r1, #48]                       /* @3498 0a63 str r2, [r1, #48]  */
L_349a:
    bx lr                                   /* @349A 7047 bx lr  */
L_349c:
    .byte 0x00,0x0c,0x00,0x40 /* @349C literal */
L_34a0:
    .byte 0x00,0x0d,0x00,0x40 /* @34A0 literal */

/* sub_34A4 */
L_34a4:
    ldr r0, [pc, #16]                       /* @34A4 0448 ldr r0, [pc, #16] literal 0x34B8 -> 0x40000C00 */
L_34a6:
    ldr r2, [r0, #48]                       /* @34A6 026b ldr r2, [r0, #48]  */
L_34a8:
    asrs r1, r0, #18                        /* @34A8 8114 asrs r1, r0, #18  */
L_34aa:
    orrs r2, r1                             /* @34AA 0a43 orrs r2, r1  */
L_34ac:
    str r2, [r0, #48]                       /* @34AC 0263 str r2, [r0, #48]  */
L_34ae:
    ldr r0, [pc, #12]                       /* @34AE 0348 ldr r0, [pc, #12] literal 0x34BC -> 0x40000D00 */
L_34b0:
    ldr r2, [r0, #48]                       /* @34B0 026b ldr r2, [r0, #48]  */
L_34b2:
    orrs r2, r1                             /* @34B2 0a43 orrs r2, r1  */
L_34b4:
    str r2, [r0, #48]                       /* @34B4 0263 str r2, [r0, #48]  */
L_34b6:
    bx lr                                   /* @34B6 7047 bx lr  */
L_34b8:
    .byte 0x00,0x0c,0x00,0x40 /* @34B8 literal */
L_34bc:
    .byte 0x00,0x0d,0x00,0x40 /* @34BC literal */

/* sub_34C0 */
L_34c0:
    push {r4, r5, r6, lr}                   /* @34C0 70b5 push {r4, r5, r6, lr}  */
L_34c2:
    ldr r4, [pc, #100]                      /* @34C2 194c ldr r4, [pc, #100] literal 0x3528 -> 0x20000600 */
L_34c4:
    ldr r0, [r4]                            /* @34C4 2068 ldr r0, [r4]  */
L_34c6:
    subs r0, #128                           /* @34C6 8038 subs r0, #128  */
L_34c8:
    str r0, [r4]                            /* @34C8 2060 str r0, [r4]  */
L_34ca:
    ldr r0, [r4]                            /* @34CA 2068 ldr r0, [r4]  */
L_34cc:
    ldr r5, [pc, #92]                       /* @34CC 174d ldr r5, [pc, #92] literal 0x352C -> 0x00000BB8 */
L_34ce:
    uxth r0, r0                             /* @34CE 80b2 uxth r0, r0  */
L_34d0:
    mov r1, r5                              /* @34D0 2946 mov r1, r5  */
L_34d2:
    bl L_3470                               /* @34D2 fff7cdff bl #-102 -> sub_3470 */
L_34d6:
    movs r0, #50                            /* @34D6 3220 movs r0, #50  */
L_34d8:
    bl L_2de8                               /* @34D8 fff786fc bl #-1780 -> delay_ms_candidate */
L_34dc:
    ldr r0, [r4]                            /* @34DC 2068 ldr r0, [r4]  */
L_34de:
    subs r0, #128                           /* @34DE 8038 subs r0, #128  */
L_34e0:
    str r0, [r4]                            /* @34E0 2060 str r0, [r4]  */
L_34e2:
    ldr r0, [r4]                            /* @34E2 2068 ldr r0, [r4]  */
L_34e4:
    mov r1, r5                              /* @34E4 2946 mov r1, r5  */
L_34e6:
    uxth r0, r0                             /* @34E6 80b2 uxth r0, r0  */
L_34e8:
    bl L_3470                               /* @34E8 fff7c2ff bl #-124 -> sub_3470 */
L_34ec:
    movs r0, #50                            /* @34EC 3220 movs r0, #50  */
L_34ee:
    bl L_2de8                               /* @34EE fff77bfc bl #-1802 -> delay_ms_candidate */
L_34f2:
    ldr r0, [pc, #60]                       /* @34F2 0f48 ldr r0, [pc, #60] literal 0x3530 -> 0x200005E4 */
L_34f4:
    ldrh r0, [r0, #4]                       /* @34F4 8088 ldrh r0, [r0, #4]  */
L_34f6:
    cmp r0, #50                             /* @34F6 3228 cmp r0, #50  */
L_34f8:
    bne L_3526                              /* @34F8 15d1 bne #42 -> 0x3526 */
L_34fa:
    ldr r0, [r4]                            /* @34FA 2068 ldr r0, [r4]  */
L_34fc:
    subs r0, #128                           /* @34FC 8038 subs r0, #128  */
L_34fe:
    str r0, [r4]                            /* @34FE 2060 str r0, [r4]  */
L_3500:
    ldr r0, [r4]                            /* @3500 2068 ldr r0, [r4]  */
L_3502:
    mov r1, r5                              /* @3502 2946 mov r1, r5  */
L_3504:
    uxth r0, r0                             /* @3504 80b2 uxth r0, r0  */
L_3506:
    bl L_3470                               /* @3506 fff7b3ff bl #-154 -> sub_3470 */
L_350a:
    movs r0, #50                            /* @350A 3220 movs r0, #50  */
L_350c:
    bl L_2de8                               /* @350C fff76cfc bl #-1832 -> delay_ms_candidate */
L_3510:
    ldr r0, [r4]                            /* @3510 2068 ldr r0, [r4]  */
L_3512:
    subs r0, #128                           /* @3512 8038 subs r0, #128  */
L_3514:
    str r0, [r4]                            /* @3514 2060 str r0, [r4]  */
L_3516:
    ldr r0, [r4]                            /* @3516 2068 ldr r0, [r4]  */
L_3518:
    mov r1, r5                              /* @3518 2946 mov r1, r5  */
L_351a:
    uxth r0, r0                             /* @351A 80b2 uxth r0, r0  */
L_351c:
    bl L_3470                               /* @351C fff7a8ff bl #-176 -> sub_3470 */
L_3520:
    movs r0, #50                            /* @3520 3220 movs r0, #50  */
L_3522:
    bl L_2de8                               /* @3522 fff761fc bl #-1854 -> delay_ms_candidate */
L_3526:
    pop {r4, r5, r6, pc}                    /* @3526 70bd pop {r4, r5, r6, pc}  */
L_3528:
    .byte 0x00,0x06,0x00,0x20 /* @3528 literal */
L_352c:
    .byte 0xb8,0x0b,0x00,0x00 /* @352C literal */
L_3530:
    .byte 0xe4,0x05,0x00,0x20 /* @3530 literal */

/* sub_3534 */
L_3534:
    push {r4, r5, r6, lr}                   /* @3534 70b5 push {r4, r5, r6, lr}  */
L_3536:
    ldr r4, [pc, #100]                      /* @3536 194c ldr r4, [pc, #100] literal 0x359C -> 0x20000600 */
L_3538:
    ldr r0, [r4]                            /* @3538 2068 ldr r0, [r4]  */
L_353a:
    adds r0, #128                           /* @353A 8030 adds r0, #128  */
L_353c:
    str r0, [r4]                            /* @353C 2060 str r0, [r4]  */
L_353e:
    ldr r0, [r4]                            /* @353E 2068 ldr r0, [r4]  */
L_3540:
    ldr r5, [pc, #92]                       /* @3540 174d ldr r5, [pc, #92] literal 0x35A0 -> 0x00000BB8 */
L_3542:
    uxth r0, r0                             /* @3542 80b2 uxth r0, r0  */
L_3544:
    mov r1, r5                              /* @3544 2946 mov r1, r5  */
L_3546:
    bl L_3470                               /* @3546 fff793ff bl #-218 -> sub_3470 */
L_354a:
    movs r0, #50                            /* @354A 3220 movs r0, #50  */
L_354c:
    bl L_2de8                               /* @354C fff74cfc bl #-1896 -> delay_ms_candidate */
L_3550:
    ldr r0, [r4]                            /* @3550 2068 ldr r0, [r4]  */
L_3552:
    adds r0, #128                           /* @3552 8030 adds r0, #128  */
L_3554:
    str r0, [r4]                            /* @3554 2060 str r0, [r4]  */
L_3556:
    ldr r0, [r4]                            /* @3556 2068 ldr r0, [r4]  */
L_3558:
    mov r1, r5                              /* @3558 2946 mov r1, r5  */
L_355a:
    uxth r0, r0                             /* @355A 80b2 uxth r0, r0  */
L_355c:
    bl L_3470                               /* @355C fff788ff bl #-240 -> sub_3470 */
L_3560:
    movs r0, #50                            /* @3560 3220 movs r0, #50  */
L_3562:
    bl L_2de8                               /* @3562 fff741fc bl #-1918 -> delay_ms_candidate */
L_3566:
    ldr r0, [pc, #60]                       /* @3566 0f48 ldr r0, [pc, #60] literal 0x35A4 -> 0x200005E4 */
L_3568:
    ldrh r0, [r0, #4]                       /* @3568 8088 ldrh r0, [r0, #4]  */
L_356a:
    cmp r0, #50                             /* @356A 3228 cmp r0, #50  */
L_356c:
    bne L_359a                              /* @356C 15d1 bne #42 -> 0x359A */
L_356e:
    ldr r0, [r4]                            /* @356E 2068 ldr r0, [r4]  */
L_3570:
    adds r0, #128                           /* @3570 8030 adds r0, #128  */
L_3572:
    str r0, [r4]                            /* @3572 2060 str r0, [r4]  */
L_3574:
    ldr r0, [r4]                            /* @3574 2068 ldr r0, [r4]  */
L_3576:
    mov r1, r5                              /* @3576 2946 mov r1, r5  */
L_3578:
    uxth r0, r0                             /* @3578 80b2 uxth r0, r0  */
L_357a:
    bl L_3470                               /* @357A fff779ff bl #-270 -> sub_3470 */
L_357e:
    movs r0, #50                            /* @357E 3220 movs r0, #50  */
L_3580:
    bl L_2de8                               /* @3580 fff732fc bl #-1948 -> delay_ms_candidate */
L_3584:
    ldr r0, [r4]                            /* @3584 2068 ldr r0, [r4]  */
L_3586:
    adds r0, #128                           /* @3586 8030 adds r0, #128  */
L_3588:
    str r0, [r4]                            /* @3588 2060 str r0, [r4]  */
L_358a:
    ldr r0, [r4]                            /* @358A 2068 ldr r0, [r4]  */
L_358c:
    mov r1, r5                              /* @358C 2946 mov r1, r5  */
L_358e:
    uxth r0, r0                             /* @358E 80b2 uxth r0, r0  */
L_3590:
    bl L_3470                               /* @3590 fff76eff bl #-292 -> sub_3470 */
L_3594:
    movs r0, #50                            /* @3594 3220 movs r0, #50  */
L_3596:
    bl L_2de8                               /* @3596 fff727fc bl #-1970 -> delay_ms_candidate */
L_359a:
    pop {r4, r5, r6, pc}                    /* @359A 70bd pop {r4, r5, r6, pc}  */
L_359c:
    .byte 0x00,0x06,0x00,0x20 /* @359C literal */
L_35a0:
    .byte 0xb8,0x0b,0x00,0x00 /* @35A0 literal */
L_35a4:
    .byte 0xe4,0x05,0x00,0x20 /* @35A4 literal */

/* sub_35A8 */
L_35a8:
    push {r4, r5, r6, r7, lr}               /* @35A8 f0b5 push {r4, r5, r6, r7, lr}  */
L_35aa:
    sub sp, #44                             /* @35AA 8bb0 sub sp, #44  */
L_35ac:
    movs r4, #0                             /* @35AC 0024 movs r4, #0  */
L_35ae:
    add r0, sp, #32                         /* @35AE 08a8 add r0, sp, #32  */
L_35b0:
    strb r4, [r0, #4]                       /* @35B0 0471 strb r4, [r0, #4]  */
L_35b2:
    strb r4, [r0, #8]                       /* @35B2 0472 strb r4, [r0, #8]  */
L_35b4:
    movs r5, #1                             /* @35B4 0125 movs r5, #1  */
L_35b6:
    strb r5, [r0, #9]                       /* @35B6 4572 strb r5, [r0, #9]  */
L_35b8:
    movs r1, #15                            /* @35B8 0f21 movs r1, #15  */
L_35ba:
    movs r0, #0                             /* @35BA 0020 movs r0, #0  */
L_35bc:
    bl L_1318                               /* @35BC fdf7acfe bl #-8872 -> sub_1318 */
L_35c0:
    add r2, sp, #36                         /* @35C0 09aa add r2, sp, #36  */
L_35c2:
    movs r1, #15                            /* @35C2 0f21 movs r1, #15  */
L_35c4:
    movs r0, #0                             /* @35C4 0020 movs r0, #0  */
L_35c6:
    bl L_137c                               /* @35C6 fdf7d9fe bl #-8782 -> sub_137C */
L_35ca:
    movs r2, #5                             /* @35CA 0522 movs r2, #5  */
L_35cc:
    movs r1, #15                            /* @35CC 0f21 movs r1, #15  */
L_35ce:
    movs r0, #0                             /* @35CE 0020 movs r0, #0  */
L_35d0:
    bl L_1408                               /* @35D0 fdf71aff bl #-8652 -> sub_1408 */
L_35d4:
    movs r1, #3                             /* @35D4 0321 movs r1, #3  */
L_35d6:
    movs r0, #64                            /* @35D6 4020 movs r0, #64  */
L_35d8:
    bl L_1318                               /* @35D8 fdf79efe bl #-8900 -> sub_1318 */
L_35dc:
    add r2, sp, #36                         /* @35DC 09aa add r2, sp, #36  */
L_35de:
    movs r1, #3                             /* @35DE 0321 movs r1, #3  */
L_35e0:
    movs r0, #64                            /* @35E0 4020 movs r0, #64  */
L_35e2:
    bl L_137c                               /* @35E2 fdf7cbfe bl #-8810 -> sub_137C */
L_35e6:
    movs r2, #2                             /* @35E6 0222 movs r2, #2  */
L_35e8:
    movs r1, #3                             /* @35E8 0321 movs r1, #3  */
L_35ea:
    movs r0, #64                            /* @35EA 4020 movs r0, #64  */
L_35ec:
    bl L_1408                               /* @35EC fdf70cff bl #-8680 -> sub_1408 */
L_35f0:
    movs r1, #6                             /* @35F0 0621 movs r1, #6  */
L_35f2:
    movs r0, #64                            /* @35F2 4020 movs r0, #64  */
L_35f4:
    bl L_1318                               /* @35F4 fdf790fe bl #-8928 -> sub_1318 */
L_35f8:
    add r2, sp, #36                         /* @35F8 09aa add r2, sp, #36  */
L_35fa:
    movs r1, #6                             /* @35FA 0621 movs r1, #6  */
L_35fc:
    movs r0, #64                            /* @35FC 4020 movs r0, #64  */
L_35fe:
    bl L_137c                               /* @35FE fdf7bdfe bl #-8838 -> sub_137C */
L_3602:
    movs r2, #3                             /* @3602 0322 movs r2, #3  */
L_3604:
    movs r1, #6                             /* @3604 0621 movs r1, #6  */
L_3606:
    movs r0, #64                            /* @3606 4020 movs r0, #64  */
L_3608:
    bl L_1408                               /* @3608 fdf7fefe bl #-8708 -> sub_1408 */
L_360c:
    movs r1, #2                             /* @360C 0221 movs r1, #2  */
L_360e:
    movs r0, #0                             /* @360E 0020 movs r0, #0  */
L_3610:
    bl L_1318                               /* @3610 fdf782fe bl #-8956 -> sub_1318 */
L_3614:
    add r2, sp, #36                         /* @3614 09aa add r2, sp, #36  */
L_3616:
    movs r1, #2                             /* @3616 0221 movs r1, #2  */
L_3618:
    movs r0, #0                             /* @3618 0020 movs r0, #0  */
L_361a:
    bl L_137c                               /* @361A fdf7affe bl #-8866 -> sub_137C */
L_361e:
    movs r2, #4                             /* @361E 0422 movs r2, #4  */
L_3620:
    movs r1, #2                             /* @3620 0221 movs r1, #2  */
L_3622:
    movs r0, #0                             /* @3622 0020 movs r0, #0  */
L_3624:
    bl L_1408                               /* @3624 fdf7f0fe bl #-8736 -> sub_1408 */
L_3628:
    movs r1, #20                            /* @3628 1421 movs r1, #20  */
L_362a:
    add r0, sp, #16                         /* @362A 04a8 add r0, sp, #16  */
L_362c:
    bl L_2d84                               /* @362C fff7aafb bl #-2220 -> zero_bytes */
L_3630:
    movs r1, #10                            /* @3630 0a21 movs r1, #10  */
L_3632:
    add r0, sp, #4                          /* @3632 01a8 add r0, sp, #4  */
L_3634:
    bl L_2d84                               /* @3634 fff7a6fb bl #-2228 -> zero_bytes */
L_3638:
    movs r1, #3                             /* @3638 0321 movs r1, #3  */
L_363a:
    mov r0, sp                              /* @363A 6846 mov r0, sp  */
L_363c:
    strb r1, [r0, #16]                      /* @363C 0174 strb r1, [r0, #16]  */
L_363e:
    strb r4, [r0, #19]                      /* @363E c474 strb r4, [r0, #19]  */
L_3640:
    strb r4, [r0, #18]                      /* @3640 8474 strb r4, [r0, #18]  */
L_3642:
    strb r4, [r0, #17]                      /* @3642 4474 strb r4, [r0, #17]  */
L_3644:
    strb r4, [r0, #20]                      /* @3644 0475 strb r4, [r0, #20]  */
L_3646:
    strb r5, [r0, #21]                      /* @3646 4575 strb r5, [r0, #21]  */
L_3648:
    strb r4, [r0, #22]                      /* @3648 8475 strb r4, [r0, #22]  */
L_364a:
    strb r4, [r0, #23]                      /* @364A c475 strb r4, [r0, #23]  */
L_364c:
    add r1, sp, #16                         /* @364C 04a9 add r1, sp, #16  */
L_364e:
    movs r0, #0                             /* @364E 0020 movs r0, #0  */
L_3650:
    bl L_0c28                               /* @3650 fdf7eafa bl #-10796 -> sub_0C28 */
L_3654:
    movs r7, #75                            /* @3654 4b27 movs r7, #75  */
L_3656:
    lsls r7, r7, #4                         /* @3656 3f01 lsls r7, r7, #4  */
L_3658:
    movs r2, #1                             /* @3658 0122 movs r2, #1  */
L_365a:
    mov r1, r7                              /* @365A 3946 mov r1, r7  */
L_365c:
    movs r0, #0                             /* @365C 0020 movs r0, #0  */
L_365e:
    bl L_0954                               /* @365E fdf779f9 bl #-11534 -> sub_0954 */
L_3662:
    movs r1, #0                             /* @3662 0021 movs r1, #0  */
L_3664:
    mov r2, r7                              /* @3664 3a46 mov r2, r7  */
L_3666:
    mov r0, r1                              /* @3666 0846 mov r0, r1  */
L_3668:
    bl L_0980                               /* @3668 fdf78af9 bl #-11500 -> sub_0980 */
L_366c:
    mov r2, r7                              /* @366C 3a46 mov r2, r7  */
L_366e:
    movs r1, #1                             /* @366E 0121 movs r1, #1  */
L_3670:
    movs r0, #0                             /* @3670 0020 movs r0, #0  */
L_3672:
    bl L_0980                               /* @3672 fdf785f9 bl #-11510 -> sub_0980 */
L_3676:
    movs r6, #7                             /* @3676 0726 movs r6, #7  */
L_3678:
    mov r0, sp                              /* @3678 6846 mov r0, sp  */
L_367a:
    strb r6, [r0, #5]                       /* @367A 4671 strb r6, [r0, #5]  */
L_367c:
    strb r4, [r0, #6]                       /* @367C 8471 strb r4, [r0, #6]  */
L_367e:
    strb r5, [r0, #7]                       /* @367E c571 strb r5, [r0, #7]  */
L_3680:
    strb r4, [r0, #8]                       /* @3680 0472 strb r4, [r0, #8]  */
L_3682:
    strb r6, [r0, #10]                      /* @3682 8672 strb r6, [r0, #10]  */
L_3684:
    strb r4, [r0, #11]                      /* @3684 c472 strb r4, [r0, #11]  */
L_3686:
    strb r5, [r0, #12]                      /* @3686 0573 strb r5, [r0, #12]  */
L_3688:
    strb r4, [r0, #13]                      /* @3688 4473 strb r4, [r0, #13]  */
L_368a:
    add r1, sp, #4                          /* @368A 01a9 add r1, sp, #4  */
L_368c:
    movs r0, #0                             /* @368C 0020 movs r0, #0  */
L_368e:
    bl L_0a54                               /* @368E fdf7e1f9 bl #-11326 -> sub_0A54 */
L_3692:
    mov r1, r5                              /* @3692 2946 mov r1, r5  */
L_3694:
    movs r0, #0                             /* @3694 0020 movs r0, #0  */
L_3696:
    bl L_0af8                               /* @3696 fdf72ffa bl #-11170 -> sub_0AF8 */
L_369a:
    mov r1, r4                              /* @369A 2146 mov r1, r4  */
L_369c:
    movs r0, #0                             /* @369C 0020 movs r0, #0  */
L_369e:
    bl L_09b4                               /* @369E fdf789f9 bl #-11502 -> sub_09B4 */
L_36a2:
    movs r2, #0                             /* @36A2 0022 movs r2, #0  */
L_36a4:
    movs r1, #1                             /* @36A4 0121 movs r1, #1  */
L_36a6:
    mov r0, r2                              /* @36A6 1046 mov r0, r2  */
L_36a8:
    bl L_09d0                               /* @36A8 fdf792f9 bl #-11484 -> sub_09D0 */
L_36ac:
    add r1, sp, #16                         /* @36AC 04a9 add r1, sp, #16  */
L_36ae:
    movs r0, #1                             /* @36AE 0120 movs r0, #1  */
L_36b0:
    bl L_0c28                               /* @36B0 fdf7bafa bl #-10892 -> sub_0C28 */
L_36b4:
    movs r2, #1                             /* @36B4 0122 movs r2, #1  */
L_36b6:
    mov r1, r7                              /* @36B6 3946 mov r1, r7  */
L_36b8:
    mov r0, r2                              /* @36B8 1046 mov r0, r2  */
L_36ba:
    bl L_0954                               /* @36BA fdf74bf9 bl #-11626 -> sub_0954 */
L_36be:
    mov r2, r7                              /* @36BE 3a46 mov r2, r7  */
L_36c0:
    movs r1, #0                             /* @36C0 0021 movs r1, #0  */
L_36c2:
    movs r0, #1                             /* @36C2 0120 movs r0, #1  */
L_36c4:
    bl L_0980                               /* @36C4 fdf75cf9 bl #-11592 -> sub_0980 */
L_36c8:
    movs r1, #1                             /* @36C8 0121 movs r1, #1  */
L_36ca:
    mov r2, r7                              /* @36CA 3a46 mov r2, r7  */
L_36cc:
    mov r0, r1                              /* @36CC 0846 mov r0, r1  */
L_36ce:
    bl L_0980                               /* @36CE fdf757f9 bl #-11602 -> sub_0980 */
L_36d2:
    mov r0, sp                              /* @36D2 6846 mov r0, sp  */
L_36d4:
    strb r6, [r0, #5]                       /* @36D4 4671 strb r6, [r0, #5]  */
L_36d6:
    strb r4, [r0, #6]                       /* @36D6 8471 strb r4, [r0, #6]  */
L_36d8:
    strb r5, [r0, #7]                       /* @36D8 c571 strb r5, [r0, #7]  */
L_36da:
    strb r4, [r0, #8]                       /* @36DA 0472 strb r4, [r0, #8]  */
L_36dc:
    strb r6, [r0, #10]                      /* @36DC 8672 strb r6, [r0, #10]  */
L_36de:
    strb r4, [r0, #11]                      /* @36DE c472 strb r4, [r0, #11]  */
L_36e0:
    strb r5, [r0, #12]                      /* @36E0 0573 strb r5, [r0, #12]  */
L_36e2:
    strb r4, [r0, #13]                      /* @36E2 4473 strb r4, [r0, #13]  */
L_36e4:
    add r1, sp, #4                          /* @36E4 01a9 add r1, sp, #4  */
L_36e6:
    movs r0, #1                             /* @36E6 0120 movs r0, #1  */
L_36e8:
    bl L_0a54                               /* @36E8 fdf7b4f9 bl #-11416 -> sub_0A54 */
L_36ec:
    mov r1, r5                              /* @36EC 2946 mov r1, r5  */
L_36ee:
    movs r0, #1                             /* @36EE 0120 movs r0, #1  */
L_36f0:
    bl L_0af8                               /* @36F0 fdf702fa bl #-11260 -> sub_0AF8 */
L_36f4:
    mov r1, r4                              /* @36F4 2146 mov r1, r4  */
L_36f6:
    movs r0, #1                             /* @36F6 0120 movs r0, #1  */
L_36f8:
    bl L_09b4                               /* @36F8 fdf75cf9 bl #-11592 -> sub_09B4 */
L_36fc:
    movs r1, #1                             /* @36FC 0121 movs r1, #1  */
L_36fe:
    movs r2, #0                             /* @36FE 0022 movs r2, #0  */
L_3700:
    mov r0, r1                              /* @3700 0846 mov r0, r1  */
L_3702:
    bl L_09d0                               /* @3702 fdf765f9 bl #-11574 -> sub_09D0 */
L_3706:
    movs r1, #4                             /* @3706 0421 movs r1, #4  */
L_3708:
    mov r0, sp                              /* @3708 6846 mov r0, sp  */
L_370a:
    bl L_2d84                               /* @370A fff73bfb bl #-2442 -> zero_bytes */
L_370e:
    mov r0, sp                              /* @370E 6846 mov r0, sp  */
L_3710:
    strb r5, [r0]                           /* @3710 0570 strb r5, [r0]  */
L_3712:
    strb r5, [r0, #1]                       /* @3712 4570 strb r5, [r0, #1]  */
L_3714:
    mov r1, sp                              /* @3714 6946 mov r1, sp  */
L_3716:
    movs r0, #0                             /* @3716 0020 movs r0, #0  */
L_3718:
    bl L_0a00                               /* @3718 fdf772f9 bl #-11548 -> sub_0A00 */
L_371c:
    movs r1, #4                             /* @371C 0421 movs r1, #4  */
L_371e:
    mov r0, sp                              /* @371E 6846 mov r0, sp  */
L_3720:
    bl L_2d84                               /* @3720 fff730fb bl #-2464 -> zero_bytes */
L_3724:
    mov r0, sp                              /* @3724 6846 mov r0, sp  */
L_3726:
    strb r4, [r0]                           /* @3726 0470 strb r4, [r0]  */
L_3728:
    movs r1, #2                             /* @3728 0221 movs r1, #2  */
L_372a:
    strb r1, [r0, #2]                       /* @372A 8170 strb r1, [r0, #2]  */
L_372c:
    strb r5, [r0, #3]                       /* @372C c570 strb r5, [r0, #3]  */
L_372e:
    mov r1, sp                              /* @372E 6946 mov r1, sp  */
L_3730:
    movs r0, #1                             /* @3730 0120 movs r0, #1  */
L_3732:
    bl L_0a00                               /* @3732 fdf765f9 bl #-11574 -> sub_0A00 */
L_3736:
    add sp, #44                             /* @3736 0bb0 add sp, #44  */
L_3738:
    pop {r4, r5, r6, r7, pc}                /* @3738 f0bd pop {r4, r5, r6, r7, pc}  */

/* abs32_candidate */
L_373a:
    asrs r1, r0, #31                        /* @373A c117 asrs r1, r0, #31  */
L_373c:
    eors r0, r1                             /* @373C 4840 eors r0, r1  */
L_373e:
    subs r0, r0, r1                         /* @373E 401a subs r0, r0, r1  */
L_3740:
    bx lr                                   /* @3740 7047 bx lr  */
    .byte 0x00,0x00 /* @3742 unclassified */

/* sub_3744 */
L_3744:
    push {r4, r5, lr}                       /* @3744 30b5 push {r4, r5, lr}  */
L_3746:
    sub sp, #28                             /* @3746 87b0 sub sp, #28  */
L_3748:
    movs r5, #0                             /* @3748 0025 movs r5, #0  */
L_374a:
    str r5, [sp, #20]                       /* @374A 0595 str r5, [sp, #20]  */
L_374c:
    str r1, [sp, #12]                       /* @374C 0391 str r1, [sp, #12]  */
L_374e:
    str r0, [sp, #8]                        /* @374E 0290 str r0, [sp, #8]  */
L_3750:
    cmp r1, #0                              /* @3750 0029 cmp r1, #0  */
L_3752:
    bge L_3766                              /* @3752 08da bge #16 -> 0x3766 */
L_3754:
    ldr r1, [sp, #8]                        /* @3754 0299 ldr r1, [sp, #8]  */
L_3756:
    ldr r3, [sp, #12]                       /* @3756 039b ldr r3, [sp, #12]  */
L_3758:
    rsbs r1, r1, #0                         /* @3758 4942 rsbs r1, r1, #0  */
L_375a:
    movs r0, #0                             /* @375A 0020 movs r0, #0  */
L_375c:
    sbcs r0, r3                             /* @375C 9841 sbcs r0, r3  */
L_375e:
    str r1, [sp, #8]                        /* @375E 0291 str r1, [sp, #8]  */
L_3760:
    str r0, [sp, #12]                       /* @3760 0390 str r0, [sp, #12]  */
L_3762:
    movs r0, #1                             /* @3762 0120 movs r0, #1  */
L_3764:
    str r0, [sp, #20]                       /* @3764 0590 str r0, [sp, #20]  */
L_3766:
    ldr r0, [sp, #8]                        /* @3766 0298 ldr r0, [sp, #8]  */
L_3768:
    lsrs r1, r2, #1                         /* @3768 5108 lsrs r1, r2, #1  */
L_376a:
    adds r0, r1, r0                         /* @376A 0818 adds r0, r1, r0  */
L_376c:
    mov r1, r2                              /* @376C 1146 mov r1, r2  */
L_376e:
    bl L_0110                               /* @376E fcf7cffc bl #-13922 -> udivmod32 */
L_3772:
    str r5, [sp, #4]                        /* @3772 0195 str r5, [sp, #4]  */
L_3774:
    str r0, [sp]                            /* @3774 0090 str r0, [sp]  */
L_3776:
    ldr r0, [sp, #12]                       /* @3776 0398 ldr r0, [sp, #12]  */
L_3778:
    str r0, [sp, #16]                       /* @3778 0490 str r0, [sp, #16]  */
L_377a:
    ldr r0, [sp, #16]                       /* @377A 0498 ldr r0, [sp, #16]  */
L_377c:
    cmp r0, #0                              /* @377C 0028 cmp r0, #0  */
L_377e:
    beq L_37c2                              /* @377E 20d0 beq #64 -> 0x37C2 */
L_3780:
    ldr r0, [pc, #92]                       /* @3780 1748 ldr r0, [pc, #92] literal 0x37E0 -> 0x200005E4 */
L_3782:
    ldrh r0, [r0, #4]                       /* @3782 8088 ldrh r0, [r0, #4]  */
L_3784:
    cmp r0, #50                             /* @3784 3228 cmp r0, #50  */
L_3786:
    beq L_3790                              /* @3786 03d0 beq #6 -> 0x3790 */
L_3788:
    ldr r2, [pc, #88]                       /* @3788 164a ldr r2, [pc, #88] literal 0x37E4 -> 0x0A3D70A4 */
L_378a:
    movs r3, #0                             /* @378A 0023 movs r3, #0  */
L_378c:
    movs r4, #4                             /* @378C 0424 movs r4, #4  */
L_378e:
    b L_3796                                /* @378E 02e0 b #4 -> 0x3796 */
L_3790:
    ldr r2, [pc, #84]                       /* @3790 154a ldr r2, [pc, #84] literal 0x37E8 -> 0x051EB852 */
L_3792:
    movs r3, #0                             /* @3792 0023 movs r3, #0  */
L_3794:
    movs r4, #3                             /* @3794 0324 movs r4, #3  */
L_3796:
    mov r1, r5                              /* @3796 2946 mov r1, r5  */
L_3798:
    ldr r0, [sp, #12]                       /* @3798 0398 ldr r0, [sp, #12]  */
L_379a:
    bl L_01b0                               /* @379A fcf709fd bl #-13806 -> sub_01B0 */
L_379e:
    ldr r2, [sp]                            /* @379E 009a ldr r2, [sp]  */
L_37a0:
    ldr r3, [sp, #4]                        /* @37A0 019b ldr r3, [sp, #4]  */
L_37a2:
    adds r0, r0, r2                         /* @37A2 8018 adds r0, r0, r2  */
L_37a4:
    adcs r1, r3                             /* @37A4 5941 adcs r1, r3  */
L_37a6:
    str r1, [sp, #4]                        /* @37A6 0191 str r1, [sp, #4]  */
L_37a8:
    str r0, [sp]                            /* @37A8 0090 str r0, [sp]  */
L_37aa:
    ldr r0, [sp, #16]                       /* @37AA 0498 ldr r0, [sp, #16]  */
L_37ac:
    movs r1, #100                           /* @37AC 6421 movs r1, #100  */
L_37ae:
    lsls r0, r4                             /* @37AE a040 lsls r0, r4  */
L_37b0:
    adds r0, #50                            /* @37B0 3230 adds r0, #50  */
L_37b2:
    bl L_0110                               /* @37B2 fcf7adfc bl #-13990 -> udivmod32 */
L_37b6:
    ldr r2, [sp]                            /* @37B6 009a ldr r2, [sp]  */
L_37b8:
    ldr r1, [sp, #4]                        /* @37B8 0199 ldr r1, [sp, #4]  */
L_37ba:
    subs r0, r2, r0                         /* @37BA 101a subs r0, r2, r0  */
L_37bc:
    sbcs r1, r5                             /* @37BC a941 sbcs r1, r5  */
L_37be:
    str r1, [sp, #4]                        /* @37BE 0191 str r1, [sp, #4]  */
L_37c0:
    str r0, [sp]                            /* @37C0 0090 str r0, [sp]  */
L_37c2:
    ldr r0, [sp, #20]                       /* @37C2 0598 ldr r0, [sp, #20]  */
L_37c4:
    cmp r0, #1                              /* @37C4 0128 cmp r0, #1  */
L_37c6:
    bne L_37d6                              /* @37C6 06d1 bne #12 -> 0x37D6 */
L_37c8:
    ldr r1, [sp]                            /* @37C8 0099 ldr r1, [sp]  */
L_37ca:
    ldr r2, [sp, #4]                        /* @37CA 019a ldr r2, [sp, #4]  */
L_37cc:
    rsbs r1, r1, #0                         /* @37CC 4942 rsbs r1, r1, #0  */
L_37ce:
    movs r0, #0                             /* @37CE 0020 movs r0, #0  */
L_37d0:
    sbcs r0, r2                             /* @37D0 9041 sbcs r0, r2  */
L_37d2:
    str r1, [sp]                            /* @37D2 0091 str r1, [sp]  */
L_37d4:
    str r0, [sp, #4]                        /* @37D4 0190 str r0, [sp, #4]  */
L_37d6:
    ldr r1, [sp, #4]                        /* @37D6 0199 ldr r1, [sp, #4]  */
L_37d8:
    ldr r0, [sp]                            /* @37D8 0098 ldr r0, [sp]  */
L_37da:
    add sp, #28                             /* @37DA 07b0 add sp, #28  */
L_37dc:
    pop {r4, r5, pc}                        /* @37DC 30bd pop {r4, r5, pc}  */
    .byte 0x00,0x00 /* @37DE unclassified */
L_37e0:
    .byte 0xe4,0x05,0x00,0x20 /* @37E0 literal */
L_37e4:
    .byte 0xa4,0x70,0x3d,0x0a /* @37E4 literal */
L_37e8:
    .byte 0x52,0xb8,0x1e,0x05 /* @37E8 literal */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @37EC unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @37FC unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @380C unclassified */

/* sub_3818 */
L_3818:
    push {r0, r1, r2, r4, r5, r6, lr}       /* @3818 77b5 push {r0, r1, r2, r4, r5, r6, lr}  */
L_381a:
    sub sp, #12                             /* @381A 83b0 sub sp, #12  */
L_381c:
    mov r4, r0                              /* @381C 0446 mov r4, r0  */
L_381e:
    ldr r0, [sp, #16]                       /* @381E 0498 ldr r0, [sp, #16]  */
L_3820:
    ldr r5, [pc, #104]                      /* @3820 1a4d ldr r5, [pc, #104] literal 0x388C -> 0x200006C0 */
L_3822:
    ldr r1, [r5, #16]                       /* @3822 2969 ldr r1, [r5, #16]  */
L_3824:
    muls r0, r1, r0                         /* @3824 4843 muls r0, r1, r0  */
L_3826:
    ldr r1, [sp, #20]                       /* @3826 0599 ldr r1, [sp, #20]  */
L_3828:
    ldr r2, [r5, #20]                       /* @3828 6a69 ldr r2, [r5, #20]  */
L_382a:
    muls r1, r2, r1                         /* @382A 5143 muls r1, r2, r1  */
L_382c:
    subs r0, r0, r1                         /* @382C 401a subs r0, r0, r1  */
L_382e:
    str r0, [sp, #4]                        /* @382E 0190 str r0, [sp, #4]  */
L_3830:
    ldr r0, [sp, #4]                        /* @3830 0198 ldr r0, [sp, #4]  */
L_3832:
    movs r6, #1                             /* @3832 0126 movs r6, #1  */
L_3834:
    lsls r6, r6, #13                        /* @3834 7603 lsls r6, r6, #13  */
L_3836:
    cmp r0, #0                              /* @3836 0028 cmp r0, #0  */
L_3838:
    beq L_3852                              /* @3838 0bd0 beq #22 -> 0x3852 */
L_383a:
    ldr r0, [sp, #4]                        /* @383A 0198 ldr r0, [sp, #4]  */
L_383c:
    bl L_373a                               /* @383C fff77dff bl #-262 -> abs32_candidate */
L_3840:
    adds r0, r0, r6                         /* @3840 8019 adds r0, r0, r6  */
L_3842:
    asrs r1, r0, #14                        /* @3842 8113 asrs r1, r0, #14  */
L_3844:
    ldr r0, [sp, #4]                        /* @3844 0198 ldr r0, [sp, #4]  */
L_3846:
    cmp r0, #0                              /* @3846 0028 cmp r0, #0  */
L_3848:
    ble L_384e                              /* @3848 01dd ble #2 -> 0x384E */
L_384a:
    str r1, [sp, #4]                        /* @384A 0191 str r1, [sp, #4]  */
L_384c:
    b L_3852                                /* @384C 01e0 b #2 -> 0x3852 */
L_384e:
    rsbs r0, r1, #0                         /* @384E 4842 rsbs r0, r1, #0  */
L_3850:
    str r0, [sp, #4]                        /* @3850 0190 str r0, [sp, #4]  */
L_3852:
    ldr r0, [sp, #16]                       /* @3852 0498 ldr r0, [sp, #16]  */
L_3854:
    ldr r1, [r5, #20]                       /* @3854 6969 ldr r1, [r5, #20]  */
L_3856:
    muls r0, r1, r0                         /* @3856 4843 muls r0, r1, r0  */
L_3858:
    ldr r1, [sp, #20]                       /* @3858 0599 ldr r1, [sp, #20]  */
L_385a:
    ldr r2, [r5, #16]                       /* @385A 2a69 ldr r2, [r5, #16]  */
L_385c:
    muls r1, r2, r1                         /* @385C 5143 muls r1, r2, r1  */
L_385e:
    adds r0, r0, r1                         /* @385E 4018 adds r0, r0, r1  */
L_3860:
    str r0, [sp, #8]                        /* @3860 0290 str r0, [sp, #8]  */
L_3862:
    ldr r0, [sp, #8]                        /* @3862 0298 ldr r0, [sp, #8]  */
L_3864:
    cmp r0, #0                              /* @3864 0028 cmp r0, #0  */
L_3866:
    beq L_3880                              /* @3866 0bd0 beq #22 -> 0x3880 */
L_3868:
    ldr r0, [sp, #8]                        /* @3868 0298 ldr r0, [sp, #8]  */
L_386a:
    bl L_373a                               /* @386A fff766ff bl #-308 -> abs32_candidate */
L_386e:
    adds r0, r0, r6                         /* @386E 8019 adds r0, r0, r6  */
L_3870:
    asrs r1, r0, #14                        /* @3870 8113 asrs r1, r0, #14  */
L_3872:
    ldr r0, [sp, #8]                        /* @3872 0298 ldr r0, [sp, #8]  */
L_3874:
    cmp r0, #0                              /* @3874 0028 cmp r0, #0  */
L_3876:
    ble L_387c                              /* @3876 01dd ble #2 -> 0x387C */
L_3878:
    str r1, [sp, #8]                        /* @3878 0291 str r1, [sp, #8]  */
L_387a:
    b L_3880                                /* @387A 01e0 b #2 -> 0x3880 */
L_387c:
    rsbs r0, r1, #0                         /* @387C 4842 rsbs r0, r1, #0  */
L_387e:
    str r0, [sp, #8]                        /* @387E 0290 str r0, [sp, #8]  */
L_3880:
    ldr r0, [sp, #4]                        /* @3880 0198 ldr r0, [sp, #4]  */
L_3882:
    str r0, [r4]                            /* @3882 2060 str r0, [r4]  */
L_3884:
    ldr r0, [sp, #8]                        /* @3884 0298 ldr r0, [sp, #8]  */
L_3886:
    str r0, [r4, #4]                        /* @3886 6060 str r0, [r4, #4]  */
L_3888:
    add sp, #24                             /* @3888 06b0 add sp, #24  */
L_388a:
    pop {r4, r5, r6, pc}                    /* @388A 70bd pop {r4, r5, r6, pc}  */
L_388c:
    .byte 0xc0,0x06,0x00,0x20 /* @388C literal */

/* sub_3890 */
L_3890:
    push {r4, lr}                           /* @3890 10b5 push {r4, lr}  */
L_3892:
    sub sp, #48                             /* @3892 8cb0 sub sp, #48  */
L_3894:
    ldr r0, [pc, #152]                      /* @3894 2648 ldr r0, [pc, #152] literal 0x3930 -> 0x200008E0 */
L_3896:
    ldr r4, [pc, #156]                      /* @3896 274c ldr r4, [pc, #156] literal 0x3934 -> 0x200006C0 */
L_3898:
    ldr r0, [r0]                            /* @3898 0068 ldr r0, [r0]  */
L_389a:
    str r0, [r4, #4]                        /* @389A 6060 str r0, [r4, #4]  */
L_389c:
    ldr r0, [r4, #4]                        /* @389C 6068 ldr r0, [r4, #4]  */
L_389e:
    movs r1, #93                            /* @389E 5d21 movs r1, #93  */
L_38a0:
    lsls r1, r1, #3                         /* @38A0 c900 lsls r1, r1, #3  */
L_38a2:
    cmp r0, r1                              /* @38A2 8842 cmp r0, r1  */
L_38a4:
    ble L_3928                              /* @38A4 40dd ble #128 -> 0x3928 */
L_38a6:
    ldr r0, [r4, #4]                        /* @38A6 6068 ldr r0, [r4, #4]  */
L_38a8:
    movs r1, #243                           /* @38A8 f321 movs r1, #243  */
L_38aa:
    lsls r1, r1, #2                         /* @38AA 8900 lsls r1, r1, #2  */
L_38ac:
    muls r0, r1, r0                         /* @38AC 4843 muls r0, r1, r0  */
L_38ae:
    adds r0, #255                           /* @38AE ff30 adds r0, #255  */
L_38b0:
    adds r0, #255                           /* @38B0 ff30 adds r0, #255  */
L_38b2:
    adds r0, #2                             /* @38B2 0230 adds r0, #2  */
L_38b4:
    asrs r0, r0, #10                        /* @38B4 8012 asrs r0, r0, #10  */
L_38b6:
    str r0, [r4, #8]                        /* @38B6 a060 str r0, [r4, #8]  */
L_38b8:
    ldr r0, [r4, #8]                        /* @38B8 a068 ldr r0, [r4, #8]  */
L_38ba:
    rsbs r0, r0, #0                         /* @38BA 4042 rsbs r0, r0, #0  */
L_38bc:
    str r0, [r4, #12]                       /* @38BC e060 str r0, [r4, #12]  */
L_38be:
    add r0, sp, #8                          /* @38BE 02a8 add r0, sp, #8  */
L_38c0:
    bl L_3c38                               /* @38C0 00f0baf9 bl #884 -> sub_3C38 */
L_38c4:
    ldr r0, [sp, #8]                        /* @38C4 0298 ldr r0, [sp, #8]  */
L_38c6:
    str r0, [sp, #40]                       /* @38C6 0a90 str r0, [sp, #40]  */
L_38c8:
    ldr r0, [sp, #12]                       /* @38C8 0398 ldr r0, [sp, #12]  */
L_38ca:
    str r0, [sp, #44]                       /* @38CA 0b90 str r0, [sp, #44]  */
L_38cc:
    bl L_3bb0                               /* @38CC 00f070f9 bl #736 -> sub_3BB0 */
L_38d0:
    mov r3, r0                              /* @38D0 0346 mov r3, r0  */
L_38d2:
    ldr r0, [sp, #40]                       /* @38D2 0a98 ldr r0, [sp, #40]  */
L_38d4:
    str r0, [sp, #8]                        /* @38D4 0290 str r0, [sp, #8]  */
L_38d6:
    ldr r0, [sp, #44]                       /* @38D6 0b98 ldr r0, [sp, #44]  */
L_38d8:
    str r0, [sp, #12]                       /* @38D8 0390 str r0, [sp, #12]  */
L_38da:
    mov r2, r0                              /* @38DA 0246 mov r2, r0  */
L_38dc:
    mov r0, sp                              /* @38DC 6846 mov r0, sp  */
L_38de:
    ldr r1, [sp, #8]                        /* @38DE 0299 ldr r1, [sp, #8]  */
L_38e0:
    bl L_04bc                               /* @38E0 fcf7ecfd bl #-13352 -> sub_04BC */
L_38e4:
    ldr r0, [sp]                            /* @38E4 0098 ldr r0, [sp]  */
L_38e6:
    str r0, [sp, #16]                       /* @38E6 0490 str r0, [sp, #16]  */
L_38e8:
    ldr r0, [sp, #4]                        /* @38E8 0198 ldr r0, [sp, #4]  */
L_38ea:
    str r0, [sp, #20]                       /* @38EA 0590 str r0, [sp, #20]  */
L_38ec:
    ldr r1, [sp, #16]                       /* @38EC 0499 ldr r1, [sp, #16]  */
L_38ee:
    ldr r0, [r4, #24]                       /* @38EE a069 ldr r0, [r4, #24]  */
L_38f0:
    bl L_05a0                               /* @38F0 fcf756fe bl #-13140 -> sub_05A0 */
L_38f4:
    str r0, [sp, #32]                       /* @38F4 0890 str r0, [sp, #32]  */
L_38f6:
    ldr r1, [sp, #20]                       /* @38F6 0599 ldr r1, [sp, #20]  */
L_38f8:
    ldr r0, [r4, #28]                       /* @38F8 e069 ldr r0, [r4, #28]  */
L_38fa:
    bl L_0558                               /* @38FA fcf72dfe bl #-13222 -> sub_0558 */
L_38fe:
    str r0, [sp, #36]                       /* @38FE 0990 str r0, [sp, #36]  */
L_3900:
    ldr r0, [sp, #32]                       /* @3900 0898 ldr r0, [sp, #32]  */
L_3902:
    ldr r2, [sp, #36]                       /* @3902 099a ldr r2, [sp, #36]  */
L_3904:
    str r0, [sp, #8]                        /* @3904 0290 str r0, [sp, #8]  */
L_3906:
    mov r1, r0                              /* @3906 0146 mov r1, r0  */
L_3908:
    mov r0, sp                              /* @3908 6846 mov r0, sp  */
L_390a:
    str r2, [sp, #12]                       /* @390A 0392 str r2, [sp, #12]  */
L_390c:
    bl L_3818                               /* @390C fff784ff bl #-248 -> sub_3818 */
L_3910:
    ldr r0, [sp]                            /* @3910 0098 ldr r0, [sp]  */
L_3912:
    str r0, [sp, #24]                       /* @3912 0690 str r0, [sp, #24]  */
L_3914:
    ldr r0, [sp, #4]                        /* @3914 0198 ldr r0, [sp, #4]  */
L_3916:
    str r0, [sp, #28]                       /* @3916 0790 str r0, [sp, #28]  */
L_3918:
    ldr r0, [sp, #24]                       /* @3918 0698 ldr r0, [sp, #24]  */
L_391a:
    str r0, [sp, #8]                        /* @391A 0290 str r0, [sp, #8]  */
L_391c:
    ldr r1, [sp, #28]                       /* @391C 0799 ldr r1, [sp, #28]  */
L_391e:
    str r1, [sp, #12]                       /* @391E 0391 str r1, [sp, #12]  */
L_3920:
    bl L_1ce0                               /* @3920 fef7def9 bl #-7236 -> sub_1CE0 */
L_3924:
    add sp, #48                             /* @3924 0cb0 add sp, #48  */
L_3926:
    pop {r4, pc}                            /* @3926 10bd pop {r4, pc}  */
L_3928:
    bl L_342c                               /* @3928 fff780fd bl #-1280 -> sub_342C */
L_392c:
    b L_3924                                /* @392C fae7 b #-12 -> 0x3924 */
    .byte 0x00,0x00 /* @392E unclassified */
L_3930:
    .byte 0xe0,0x08,0x00,0x20 /* @3930 literal */
L_3934:
    .byte 0xc0,0x06,0x00,0x20 /* @3934 literal */

/* menu_restore */
L_3938:
    push {r4, lr}                           /* @3938 10b5 push {r4, lr}  */
L_393a:
    bl L_5874                               /* @393A 01f09bff bl #7990 -> sub_5874 */
L_393e:
    movs r0, #31                            /* @393E 1f20 movs r0, #31  */
L_3940:
    lsls r0, r0, #10                        /* @3940 8002 lsls r0, r0, #10  */
L_3942:
    bl L_1188                               /* @3942 fdf721fc bl #-10174 -> sub_1188 */
L_3946:
    bl L_5368                               /* @3946 01f00ffd bl #6686 -> sub_5368 */
L_394a:
    bl L_470c                               /* @394A 00f0dffe bl #3518 -> sub_470C */
L_394e:
    adr r2, L_3964                          /* @394E 05a2 adr r2, #20 address 0x3964 -> 0x00003964 */
L_3950:
    adr r1, L_3968                          /* @3950 05a1 adr r1, #20 address 0x3968 -> 0x00003968 '   ok' */
L_3952:
    adr r0, L_3970                          /* @3952 07a0 adr r0, #28 address 0x3970 -> 0x00003970 ' Reload' */
L_3954:
    bl L_4884                               /* @3954 00f096ff bl #3884 -> display_three_lines */
L_3958:
    movs r0, #75                            /* @3958 4b20 movs r0, #75  */
L_395a:
    lsls r0, r0, #4                         /* @395A 0001 lsls r0, r0, #4  */
L_395c:
    bl L_2de8                               /* @395C fff744fa bl #-2936 -> delay_ms_candidate */
L_3960:
    pop {r4, pc}                            /* @3960 10bd pop {r4, pc}  */
    .byte 0x00,0x00 /* @3962 unclassified */
L_3964:
    .byte 0x00,0x00,0x00,0x00 /* @3964 unclassified */
L_3968:
    .byte 0x20,0x20,0x20,0x6f,0x6b,0x00,0x00,0x00 /* @3968 unclassified */
L_3970:
    .byte 0x20,0x52,0x65,0x6c,0x6f,0x61,0x64,0x00 /* @3970 unclassified */

/* sub_3978 */
L_3978:
    ldr r2, [pc, #132]                      /* @3978 214a ldr r2, [pc, #132] literal 0x3A00 -> 0x20000004 */
L_397a:
    ldr r0, [r2]                            /* @397A 1068 ldr r0, [r2]  */
L_397c:
    ldr r1, [pc, #132]                      /* @397C 2149 ldr r1, [pc, #132] literal 0x3A04 -> 0x2000072C */
L_397e:
    ldrh r3, [r0]                           /* @397E 0388 ldrh r3, [r0]  */
L_3980:
    strh r3, [r1]                           /* @3980 0b80 strh r3, [r1]  */
L_3982:
    ldrh r3, [r0, #2]                       /* @3982 4388 ldrh r3, [r0, #2]  */
L_3984:
    strh r3, [r1, #2]                       /* @3984 4b80 strh r3, [r1, #2]  */
L_3986:
    ldrb r3, [r0, #4]                       /* @3986 0379 ldrb r3, [r0, #4]  */
L_3988:
    strb r3, [r1, #4]                       /* @3988 0b71 strb r3, [r1, #4]  */
L_398a:
    ldrh r3, [r0, #6]                       /* @398A c388 ldrh r3, [r0, #6]  */
L_398c:
    strh r3, [r1, #6]                       /* @398C cb80 strh r3, [r1, #6]  */
L_398e:
    ldrb r3, [r0, #8]                       /* @398E 037a ldrb r3, [r0, #8]  */
L_3990:
    strb r3, [r1, #8]                       /* @3990 0b72 strb r3, [r1, #8]  */
L_3992:
    ldrb r3, [r0, #9]                       /* @3992 437a ldrb r3, [r0, #9]  */
L_3994:
    strb r3, [r1, #9]                       /* @3994 4b72 strb r3, [r1, #9]  */
L_3996:
    ldrb r3, [r0, #10]                      /* @3996 837a ldrb r3, [r0, #10]  */
L_3998:
    strb r3, [r1, #10]                      /* @3998 8b72 strb r3, [r1, #10]  */
L_399a:
    ldrb r3, [r0, #11]                      /* @399A c37a ldrb r3, [r0, #11]  */
L_399c:
    strb r3, [r1, #11]                      /* @399C cb72 strb r3, [r1, #11]  */
L_399e:
    ldrb r3, [r0, #12]                      /* @399E 037b ldrb r3, [r0, #12]  */
L_39a0:
    strb r3, [r1, #12]                      /* @39A0 0b73 strb r3, [r1, #12]  */
L_39a2:
    ldr r3, [r0, #16]                       /* @39A2 0369 ldr r3, [r0, #16]  */
L_39a4:
    str r3, [r1, #16]                       /* @39A4 0b61 str r3, [r1, #16]  */
L_39a6:
    ldrh r3, [r0, #20]                      /* @39A6 838a ldrh r3, [r0, #20]  */
L_39a8:
    strh r3, [r1, #20]                      /* @39A8 8b82 strh r3, [r1, #20]  */
L_39aa:
    ldrh r3, [r0, #22]                      /* @39AA c38a ldrh r3, [r0, #22]  */
L_39ac:
    strh r3, [r1, #22]                      /* @39AC cb82 strh r3, [r1, #22]  */
L_39ae:
    ldrh r3, [r0, #24]                      /* @39AE 038b ldrh r3, [r0, #24]  */
L_39b0:
    strh r3, [r1, #24]                      /* @39B0 0b83 strh r3, [r1, #24]  */
L_39b2:
    ldrh r0, [r0, #26]                      /* @39B2 408b ldrh r0, [r0, #26]  */
L_39b4:
    strh r0, [r1, #26]                      /* @39B4 4883 strh r0, [r1, #26]  */
L_39b6:
    ldr r0, [r2]                            /* @39B6 1068 ldr r0, [r2]  */
L_39b8:
    ldr r1, [pc, #76]                       /* @39B8 1349 ldr r1, [pc, #76] literal 0x3A08 -> 0x200005E4 */
L_39ba:
    ldrh r3, [r0, #28]                      /* @39BA 838b ldrh r3, [r0, #28]  */
L_39bc:
    strh r3, [r1]                           /* @39BC 0b80 strh r3, [r1]  */
L_39be:
    ldrh r3, [r0, #30]                      /* @39BE c38b ldrh r3, [r0, #30]  */
L_39c0:
    strh r3, [r1, #2]                       /* @39C0 4b80 strh r3, [r1, #2]  */
L_39c2:
    ldrh r0, [r0, #32]                      /* @39C2 008c ldrh r0, [r0, #32]  */
L_39c4:
    strh r0, [r1, #4]                       /* @39C4 8880 strh r0, [r1, #4]  */
L_39c6:
    ldr r0, [r2]                            /* @39C6 1068 ldr r0, [r2]  */
L_39c8:
    ldr r1, [pc, #64]                       /* @39C8 1049 ldr r1, [pc, #64] literal 0x3A0C -> 0x200005F6 */
L_39ca:
    adds r0, #34                            /* @39CA 2230 adds r0, #34  */
L_39cc:
    ldrb r3, [r0]                           /* @39CC 0378 ldrb r3, [r0]  */
L_39ce:
    strb r3, [r1]                           /* @39CE 0b70 strb r3, [r1]  */
L_39d0:
    ldrh r3, [r0, #2]                       /* @39D0 4388 ldrh r3, [r0, #2]  */
L_39d2:
    strh r3, [r1, #2]                       /* @39D2 4b80 strh r3, [r1, #2]  */
L_39d4:
    ldrh r3, [r0, #4]                       /* @39D4 8388 ldrh r3, [r0, #4]  */
L_39d6:
    strh r3, [r1, #4]                       /* @39D6 8b80 strh r3, [r1, #4]  */
L_39d8:
    ldrb r0, [r0, #6]                       /* @39D8 8079 ldrb r0, [r0, #6]  */
L_39da:
    strb r0, [r1, #6]                       /* @39DA 8871 strb r0, [r1, #6]  */
L_39dc:
    ldr r0, [r2]                            /* @39DC 1068 ldr r0, [r2]  */
L_39de:
    ldr r1, [pc, #48]                       /* @39DE 0c49 ldr r1, [pc, #48] literal 0x3A10 -> 0x200005EA */
L_39e0:
    ldrh r3, [r0, #42]                      /* @39E0 438d ldrh r3, [r0, #42]  */
L_39e2:
    strh r3, [r1]                           /* @39E2 0b80 strh r3, [r1]  */
L_39e4:
    ldrh r3, [r0, #44]                      /* @39E4 838d ldrh r3, [r0, #44]  */
L_39e6:
    strh r3, [r1, #2]                       /* @39E6 4b80 strh r3, [r1, #2]  */
L_39e8:
    ldrh r0, [r0, #46]                      /* @39E8 c08d ldrh r0, [r0, #46]  */
L_39ea:
    strh r0, [r1, #4]                       /* @39EA 8880 strh r0, [r1, #4]  */
L_39ec:
    ldr r0, [r2]                            /* @39EC 1068 ldr r0, [r2]  */
L_39ee:
    ldr r1, [pc, #36]                       /* @39EE 0949 ldr r1, [pc, #36] literal 0x3A14 -> 0x200005F0 */
L_39f0:
    ldrh r2, [r0, #48]                      /* @39F0 028e ldrh r2, [r0, #48]  */
L_39f2:
    strh r2, [r1]                           /* @39F2 0a80 strh r2, [r1]  */
L_39f4:
    ldrh r2, [r0, #50]                      /* @39F4 428e ldrh r2, [r0, #50]  */
L_39f6:
    strh r2, [r1, #2]                       /* @39F6 4a80 strh r2, [r1, #2]  */
L_39f8:
    ldrh r0, [r0, #52]                      /* @39F8 808e ldrh r0, [r0, #52]  */
L_39fa:
    strh r0, [r1, #4]                       /* @39FA 8880 strh r0, [r1, #4]  */
L_39fc:
    bx lr                                   /* @39FC 7047 bx lr  */
    .byte 0x00,0x00 /* @39FE unclassified */
L_3a00:
    .byte 0x04,0x00,0x00,0x20 /* @3A00 literal */
L_3a04:
    .byte 0x2c,0x07,0x00,0x20 /* @3A04 literal */
L_3a08:
    .byte 0xe4,0x05,0x00,0x20 /* @3A08 literal */
L_3a0c:
    .byte 0xf6,0x05,0x00,0x20 /* @3A0C literal */
L_3a10:
    .byte 0xea,0x05,0x00,0x20 /* @3A10 literal */
L_3a14:
    .byte 0xf0,0x05,0x00,0x20 /* @3A14 literal */

/* sub_3A18 */
L_3a18:
    push {r4, lr}                           /* @3A18 10b5 push {r4, lr}  */
L_3a1a:
    ldr r1, [pc, #60]                       /* @3A1A 0f49 ldr r1, [pc, #60] literal 0x3A58 -> 0x40005800 */
L_3a1c:
    ldr r0, [r1, #12]                       /* @3A1C c868 ldr r0, [r1, #12]  */
L_3a1e:
    asrs r4, r1, #20                        /* @3A1E 0c15 asrs r4, r1, #20  */
L_3a20:
    bics r0, r4                             /* @3A20 a043 bics r0, r4  */
L_3a22:
    str r0, [r1, #12]                       /* @3A22 c860 str r0, [r1, #12]  */
L_3a24:
    ldr r0, [pc, #52]                       /* @3A24 0d48 ldr r0, [pc, #52] literal 0x3A5C -> 0x20000610 */
L_3a26:
    movs r3, #0                             /* @3A26 0023 movs r3, #0  */
L_3a28:
    str r3, [r0]                            /* @3A28 0360 str r3, [r0]  */
L_3a2a:
    str r3, [r0, #4]                        /* @3A2A 4360 str r3, [r0, #4]  */
L_3a2c:
    ldr r3, [pc, #48]                       /* @3A2C 0c4b ldr r3, [pc, #48] literal 0x3A60 -> 0x2000061C */
L_3a2e:
    movs r2, #0                             /* @3A2E 0022 movs r2, #0  */
L_3a30:
    str r2, [r3]                            /* @3A30 1a60 str r2, [r3]  */
L_3a32:
    ldr r3, [pc, #48]                       /* @3A32 0c4b ldr r3, [pc, #48] literal 0x3A64 -> 0x20000012 */
L_3a34:
    strb r2, [r3, #1]                       /* @3A34 5a70 strb r2, [r3, #1]  */
L_3a36:
    ldr r2, [r1, #12]                       /* @3A36 ca68 ldr r2, [r1, #12]  */
L_3a38:
    orrs r2, r4                             /* @3A38 2243 orrs r2, r4  */
L_3a3a:
    str r2, [r1, #12]                       /* @3A3A ca60 str r2, [r1, #12]  */
L_3a3c:
    ldr r1, [pc, #40]                       /* @3A3C 0a49 ldr r1, [pc, #40] literal 0x3A68 -> 0x20000648 */
L_3a3e:
    ldr r2, [pc, #44]                       /* @3A3E 0b4a ldr r2, [pc, #44] literal 0x3A6C -> 0x20000010 */
L_3a40:
    ldrh r1, [r1, #2]                       /* @3A40 4988 ldrh r1, [r1, #2]  */
L_3a42:
    strh r1, [r2]                           /* @3A42 1180 strh r1, [r2]  */
L_3a44:
    ldr r1, [pc, #40]                       /* @3A44 0a49 ldr r1, [pc, #40] literal 0x3A70 -> 0x20000680 */
L_3a46:
    ldr r2, [r1]                            /* @3A46 0a68 ldr r2, [r1]  */
L_3a48:
    ldr r1, [r1, #4]                        /* @3A48 4968 ldr r1, [r1, #4]  */
L_3a4a:
    str r2, [r0]                            /* @3A4A 0260 str r2, [r0]  */
L_3a4c:
    str r1, [r0, #4]                        /* @3A4C 4160 str r1, [r0, #4]  */
L_3a4e:
    movs r0, #2                             /* @3A4E 0220 movs r0, #2  */
L_3a50:
    bl L_093c                               /* @3A50 fcf774ff bl #-12568 -> sub_093C */
L_3a54:
    pop {r4, pc}                            /* @3A54 10bd pop {r4, pc}  */
    .byte 0x00,0x00 /* @3A56 unclassified */
L_3a58:
    .byte 0x00,0x58,0x00,0x40 /* @3A58 literal */
L_3a5c:
    .byte 0x10,0x06,0x00,0x20 /* @3A5C literal */
L_3a60:
    .byte 0x1c,0x06,0x00,0x20 /* @3A60 literal */
L_3a64:
    .byte 0x12,0x00,0x00,0x20 /* @3A64 literal */
L_3a68:
    .byte 0x48,0x06,0x00,0x20 /* @3A68 literal */
L_3a6c:
    .byte 0x10,0x00,0x00,0x20 /* @3A6C literal */
L_3a70:
    .byte 0x80,0x06,0x00,0x20 /* @3A70 literal */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @3A74 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @3A84 unclassified */

/* sub_3A8C */
L_3a8c:
    push {r4, r5, lr}                       /* @3A8C 30b5 push {r4, r5, lr}  */
L_3a8e:
    sub sp, #60                             /* @3A8E 8fb0 sub sp, #60  */
L_3a90:
    bl L_2410                               /* @3A90 fef7befc bl #-5764 -> sub_2410 */
L_3a94:
    movs r1, #2                             /* @3A94 0221 movs r1, #2  */
L_3a96:
    add r0, sp, #56                         /* @3A96 0ea8 add r0, sp, #56  */
L_3a98:
    bl L_2d84                               /* @3A98 fff774f9 bl #-3352 -> zero_bytes */
L_3a9c:
    movs r4, #1                             /* @3A9C 0124 movs r4, #1  */
L_3a9e:
    add r1, sp, #32                         /* @3A9E 08a9 add r1, sp, #32  */
L_3aa0:
    strb r4, [r1, #25]                      /* @3AA0 4c76 strb r4, [r1, #25]  */
L_3aa2:
    movs r5, #3                             /* @3AA2 0325 movs r5, #3  */
L_3aa4:
    strb r5, [r1, #24]                      /* @3AA4 0d76 strb r5, [r1, #24]  */
L_3aa6:
    add r0, sp, #56                         /* @3AA6 0ea8 add r0, sp, #56  */
L_3aa8:
    bl L_07e4                               /* @3AA8 fcf79cfe bl #-13000 -> sub_07E4 */
L_3aac:
    movs r1, #52                            /* @3AAC 3421 movs r1, #52  */
L_3aae:
    add r0, sp, #4                          /* @3AAE 01a8 add r0, sp, #4  */
L_3ab0:
    bl L_2d84                               /* @3AB0 fff768f9 bl #-3376 -> zero_bytes */
L_3ab4:
    movs r0, #0                             /* @3AB4 0020 movs r0, #0  */
L_3ab6:
    str r0, [sp, #4]                        /* @3AB6 0190 str r0, [sp, #4]  */
L_3ab8:
    mov r1, sp                              /* @3AB8 6946 mov r1, sp  */
L_3aba:
    strh r5, [r1, #8]                       /* @3ABA 0d81 strh r5, [r1, #8]  */
L_3abc:
    strh r4, [r1, #10]                      /* @3ABC 4c81 strh r4, [r1, #10]  */
L_3abe:
    lsls r1, r4, #27                        /* @3ABE e106 lsls r1, r4, #27  */
L_3ac0:
    str r1, [sp, #12]                       /* @3AC0 0391 str r1, [sp, #12]  */
L_3ac2:
    str r0, [sp, #16]                       /* @3AC2 0490 str r0, [sp, #16]  */
L_3ac4:
    str r0, [sp, #20]                       /* @3AC4 0590 str r0, [sp, #20]  */
L_3ac6:
    asrs r1, r1, #6                         /* @3AC6 8911 asrs r1, r1, #6  */
L_3ac8:
    str r1, [sp, #28]                       /* @3AC8 0791 str r1, [sp, #28]  */
L_3aca:
    lsls r1, r1, #1                         /* @3ACA 4900 lsls r1, r1, #1  */
L_3acc:
    str r1, [sp, #24]                       /* @3ACC 0691 str r1, [sp, #24]  */
L_3ace:
    lsls r1, r1, #1                         /* @3ACE 4900 lsls r1, r1, #1  */
L_3ad0:
    str r1, [sp, #32]                       /* @3AD0 0891 str r1, [sp, #32]  */
L_3ad2:
    ldr r1, [pc, #72]                       /* @3AD2 1249 ldr r1, [pc, #72] literal 0x3B1C -> 0x40002490 */
L_3ad4:
    str r1, [sp, #36]                       /* @3AD4 0991 str r1, [sp, #36]  */
L_3ad6:
    ldr r1, [pc, #72]                       /* @3AD6 1249 ldr r1, [pc, #72] literal 0x3B20 -> 0x200008E0 */
L_3ad8:
    str r1, [sp, #40]                       /* @3AD8 0a91 str r1, [sp, #40]  */
L_3ada:
    add r1, sp, #32                         /* @3ADA 08a9 add r1, sp, #32  */
L_3adc:
    strb r4, [r1, #12]                      /* @3ADC 0c73 strb r4, [r1, #12]  */
L_3ade:
    str r0, [sp, #48]                       /* @3ADE 0c90 str r0, [sp, #48]  */
L_3ae0:
    movs r0, #36                            /* @3AE0 2420 movs r0, #36  */
L_3ae2:
    strb r0, [r1, #20]                      /* @3AE2 0875 strb r0, [r1, #20]  */
L_3ae4:
    add r1, sp, #4                          /* @3AE4 01a9 add r1, sp, #4  */
L_3ae6:
    movs r0, #0                             /* @3AE6 0020 movs r0, #0  */
L_3ae8:
    bl L_0dac                               /* @3AE8 fdf760f9 bl #-11584 -> sub_0DAC */
L_3aec:
    movs r0, #0                             /* @3AEC 0020 movs r0, #0  */
L_3aee:
    bl L_0d6c                               /* @3AEE fdf73df9 bl #-11654 -> sub_0D6C */
L_3af2:
    movs r2, #1                             /* @3AF2 0122 movs r2, #1  */
L_3af4:
    movs r1, #0                             /* @3AF4 0021 movs r1, #0  */
L_3af6:
    movs r0, #4                             /* @3AF6 0420 movs r0, #4  */
L_3af8:
    bl L_0fec                               /* @3AF8 fdf778fa bl #-11024 -> sub_0FEC */
L_3afc:
    bl L_0d84                               /* @3AFC fdf742f9 bl #-11644 -> sub_0D84 */
L_3b00:
    movs r0, #0                             /* @3B00 0020 movs r0, #0  */
L_3b02:
    bl L_0d94                               /* @3B02 fdf747f9 bl #-11634 -> sub_0D94 */
L_3b06:
    bl L_3cac                               /* @3B06 00f0d1f8 bl #418 -> sub_3CAC */
L_3b0a:
    bl L_3b9c                               /* @3B0A 00f047f8 bl #142 -> sub_3B9C */
L_3b0e:
    ldr r0, [pc, #20]                       /* @3B0E 0548 ldr r0, [pc, #20] literal 0x3B24 -> 0x40000C00 */
L_3b10:
    ldr r1, [r0, #12]                       /* @3B10 c168 ldr r1, [r0, #12]  */
L_3b12:
    orrs r1, r4                             /* @3B12 2143 orrs r1, r4  */
L_3b14:
    str r1, [r0, #12]                       /* @3B14 c160 str r1, [r0, #12]  */
L_3b16:
    add sp, #60                             /* @3B16 0fb0 add sp, #60  */
L_3b18:
    pop {r4, r5, pc}                        /* @3B18 30bd pop {r4, r5, pc}  */
    .byte 0x00,0x00 /* @3B1A unclassified */
L_3b1c:
    .byte 0x90,0x24,0x00,0x40 /* @3B1C literal */
L_3b20:
    .byte 0xe0,0x08,0x00,0x20 /* @3B20 literal */
L_3b24:
    .byte 0x00,0x0c,0x00,0x40 /* @3B24 literal */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @3B28 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @3B38 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @3B48 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @3B58 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @3B68 unclassified */

/* sub_3B70 */
L_3b70:
    ldr r0, [pc, #32]                       /* @3B70 0848 ldr r0, [pc, #32] literal 0x3B94 -> 0x40021000 */
L_3b72:
    ldr r1, [r0, #20]                       /* @3B72 4169 ldr r1, [r0, #20]  */
L_3b74:
    movs r2, #1                             /* @3B74 0122 movs r2, #1  */
L_3b76:
    lsls r2, r2, #19                        /* @3B76 d204 lsls r2, r2, #19  */
L_3b78:
    bics r1, r2                             /* @3B78 9143 bics r1, r2  */
L_3b7a:
    str r1, [r0, #20]                       /* @3B7A 4161 str r1, [r0, #20]  */
L_3b7c:
    ldr r1, [r0, #20]                       /* @3B7C 4169 ldr r1, [r0, #20]  */
L_3b7e:
    movs r2, #7                             /* @3B7E 0722 movs r2, #7  */
L_3b80:
    lsls r2, r2, #16                        /* @3B80 1204 lsls r2, r2, #16  */
L_3b82:
    bics r1, r2                             /* @3B82 9143 bics r1, r2  */
L_3b84:
    str r1, [r0, #20]                       /* @3B84 4161 str r1, [r0, #20]  */
L_3b86:
    ldr r0, [pc, #16]                       /* @3B86 0448 ldr r0, [pc, #16] literal 0x3B98 -> 0x40002480 */
L_3b88:
    ldr r1, [r0, #52]                       /* @3B88 416b ldr r1, [r0, #52]  */
L_3b8a:
    movs r2, #32                            /* @3B8A 2022 movs r2, #32  */
L_3b8c:
    bics r1, r2                             /* @3B8C 9143 bics r1, r2  */
L_3b8e:
    str r1, [r0, #52]                       /* @3B8E 4163 str r1, [r0, #52]  */
L_3b90:
    bx lr                                   /* @3B90 7047 bx lr  */
    .byte 0x00,0x00 /* @3B92 unclassified */
L_3b94:
    .byte 0x00,0x10,0x02,0x40 /* @3B94 literal */
L_3b98:
    .byte 0x80,0x24,0x00,0x40 /* @3B98 literal */

/* sub_3B9C */
L_3b9c:
    ldr r0, [pc, #12]                       /* @3B9C 0348 ldr r0, [pc, #12] literal 0x3BAC -> 0x40021000 */
L_3b9e:
    ldr r1, [r0, #20]                       /* @3B9E 4169 ldr r1, [r0, #20]  */
L_3ba0:
    movs r2, #1                             /* @3BA0 0122 movs r2, #1  */
L_3ba2:
    lsls r2, r2, #19                        /* @3BA2 d204 lsls r2, r2, #19  */
L_3ba4:
    orrs r1, r2                             /* @3BA4 1143 orrs r1, r2  */
L_3ba6:
    str r1, [r0, #20]                       /* @3BA6 4161 str r1, [r0, #20]  */
L_3ba8:
    bx lr                                   /* @3BA8 7047 bx lr  */
    .byte 0x00,0x00 /* @3BAA unclassified */
L_3bac:
    .byte 0x00,0x10,0x02,0x40 /* @3BAC literal */

/* sub_3BB0 */
L_3bb0:
    push {r4, r5, r6, lr}                   /* @3BB0 70b5 push {r4, r5, r6, lr}  */
L_3bb2:
    ldr r6, [pc, #108]                      /* @3BB2 1b4e ldr r6, [pc, #108] literal 0x3C20 -> 0x200006C0 */
L_3bb4:
    ldrb r0, [r6]                           /* @3BB4 3078 ldrb r0, [r6]  */
L_3bb6:
    cmp r0, #0                              /* @3BB6 0028 cmp r0, #0  */
L_3bb8:
    beq L_3c18                              /* @3BB8 2ed0 beq #92 -> 0x3C18 */
L_3bba:
    bl L_51dc                               /* @3BBA 01f00ffb bl #5662 -> sub_51DC */
L_3bbe:
    bl L_26cc                               /* @3BBE fef785fd bl #-5366 -> encoder_lut_read */
L_3bc2:
    ldr r5, [pc, #96]                       /* @3BC2 184d ldr r5, [pc, #96] literal 0x3C24 -> 0x20000648 */
L_3bc4:
    strh r0, [r5, #4]                       /* @3BC4 a880 strh r0, [r5, #4]  */
L_3bc6:
    ldrh r0, [r5, #4]                       /* @3BC6 a888 ldrh r0, [r5, #4]  */
L_3bc8:
    ldrh r1, [r5, #6]                       /* @3BC8 e988 ldrh r1, [r5, #6]  */
L_3bca:
    subs r0, r0, r1                         /* @3BCA 401a subs r0, r0, r1  */
L_3bcc:
    sxth r4, r0                             /* @3BCC 04b2 sxth r4, r0  */
L_3bce:
    ldrh r0, [r5, #4]                       /* @3BCE a888 ldrh r0, [r5, #4]  */
L_3bd0:
    strh r0, [r5, #6]                       /* @3BD0 e880 strh r0, [r5, #6]  */
L_3bd2:
    ldr r0, [r6, #28]                       /* @3BD2 f069 ldr r0, [r6, #28]  */
L_3bd4:
    cmp r0, #0                              /* @3BD4 0028 cmp r0, #0  */
L_3bd6:
    beq L_3c02                              /* @3BD6 14d0 beq #40 -> 0x3C02 */
L_3bd8:
    ldr r0, [r6, #28]                       /* @3BD8 f069 ldr r0, [r6, #28]  */
L_3bda:
    bl L_373a                               /* @3BDA fff7aefd bl #-1188 -> abs32_candidate */
L_3bde:
    adds r0, #64                            /* @3BDE 4030 adds r0, #64  */
L_3be0:
    asrs r0, r0, #7                         /* @3BE0 c011 asrs r0, r0, #7  */
L_3be2:
    cmp r0, #49                             /* @3BE2 3128 cmp r0, #49  */
L_3be4:
    ble L_3be8                              /* @3BE4 00dd ble #0 -> 0x3BE8 */
L_3be6:
    movs r0, #49                            /* @3BE6 3120 movs r0, #49  */
L_3be8:
    ldr r1, [r6, #28]                       /* @3BE8 f169 ldr r1, [r6, #28]  */
L_3bea:
    cmp r1, #0                              /* @3BEA 0029 cmp r1, #0  */
L_3bec:
    ble L_3bf8                              /* @3BEC 04dd ble #8 -> 0x3BF8 */
L_3bee:
    ldrh r1, [r5, #4]                       /* @3BEE a988 ldrh r1, [r5, #4]  */
L_3bf0:
    adds r0, r4, r0                         /* @3BF0 2018 adds r0, r4, r0  */
L_3bf2:
    adds r0, r1, r0                         /* @3BF2 0818 adds r0, r1, r0  */
L_3bf4:
    uxth r0, r0                             /* @3BF4 80b2 uxth r0, r0  */
L_3bf6:
    b L_3c08                                /* @3BF6 07e0 b #14 -> 0x3C08 */
L_3bf8:
    ldrh r1, [r5, #4]                       /* @3BF8 a988 ldrh r1, [r5, #4]  */
L_3bfa:
    adds r1, r1, r4                         /* @3BFA 0919 adds r1, r1, r4  */
L_3bfc:
    subs r0, r1, r0                         /* @3BFC 081a subs r0, r1, r0  */
L_3bfe:
    uxth r0, r0                             /* @3BFE 80b2 uxth r0, r0  */
L_3c00:
    b L_3c08                                /* @3C00 02e0 b #4 -> 0x3C08 */
L_3c02:
    ldrh r0, [r5, #4]                       /* @3C02 a888 ldrh r0, [r5, #4]  */
L_3c04:
    adds r0, r0, r4                         /* @3C04 0019 adds r0, r0, r4  */
L_3c06:
    uxth r0, r0                             /* @3C06 80b2 uxth r0, r0  */
L_3c08:
    ldr r1, [pc, #28]                       /* @3C08 0749 ldr r1, [pc, #28] literal 0x3C28 -> 0x200005E4 */
L_3c0a:
    ldrh r1, [r1, #4]                       /* @3C0A 8988 ldrh r1, [r1, #4]  */
L_3c0c:
    muls r1, r0, r1                         /* @3C0C 4143 muls r1, r0, r1  */
L_3c0e:
    adds r1, #16                            /* @3C0E 1031 adds r1, #16  */
L_3c10:
    asrs r0, r1, #5                         /* @3C10 4811 asrs r0, r1, #5  */
L_3c12:
    lsls r0, r0, #22                        /* @3C12 8005 lsls r0, r0, #22  */
L_3c14:
    lsrs r0, r0, #22                        /* @3C14 800d lsrs r0, r0, #22  */
L_3c16:
    pop {r4, r5, r6, pc}                    /* @3C16 70bd pop {r4, r5, r6, pc}  */
L_3c18:
    ldrh r0, [r6, #2]                       /* @3C18 7088 ldrh r0, [r6, #2]  */
L_3c1a:
    lsls r0, r0, #22                        /* @3C1A 8005 lsls r0, r0, #22  */
L_3c1c:
    lsrs r0, r0, #22                        /* @3C1C 800d lsrs r0, r0, #22  */
L_3c1e:
    pop {r4, r5, r6, pc}                    /* @3C1E 70bd pop {r4, r5, r6, pc}  */
L_3c20:
    .byte 0xc0,0x06,0x00,0x20 /* @3C20 literal */
L_3c24:
    .byte 0x48,0x06,0x00,0x20 /* @3C24 literal */
L_3c28:
    .byte 0xe4,0x05,0x00,0x20 /* @3C28 literal */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @3C2C unclassified */

/* sub_3C38 */
L_3c38:
    push {r2, r3, r4, lr}                   /* @3C38 1cb5 push {r2, r3, r4, lr}  */
L_3c3a:
    ldr r1, [pc, #28]                       /* @3C3A 0749 ldr r1, [pc, #28] literal 0x3C58 -> 0x200006E0 */
L_3c3c:
    ldr r3, [r1, #4]                        /* @3C3C 4b68 ldr r3, [r1, #4]  */
L_3c3e:
    ldr r2, [pc, #28]                       /* @3C3E 074a ldr r2, [pc, #28] literal 0x3C5C -> 0x200008E0 */
L_3c40:
    ldr r4, [r2, #8]                        /* @3C40 9468 ldr r4, [r2, #8]  */
L_3c42:
    subs r3, r3, r4                         /* @3C42 1b1b subs r3, r3, r4  */
L_3c44:
    str r3, [sp]                            /* @3C44 0093 str r3, [sp]  */
L_3c46:
    ldr r2, [r2, #4]                        /* @3C46 5268 ldr r2, [r2, #4]  */
L_3c48:
    ldr r1, [r1, #8]                        /* @3C48 8968 ldr r1, [r1, #8]  */
L_3c4a:
    subs r1, r2, r1                         /* @3C4A 511a subs r1, r2, r1  */
L_3c4c:
    str r1, [sp, #4]                        /* @3C4C 0191 str r1, [sp, #4]  */
L_3c4e:
    ldr r1, [sp]                            /* @3C4E 0099 ldr r1, [sp]  */
L_3c50:
    str r1, [r0]                            /* @3C50 0160 str r1, [r0]  */
L_3c52:
    ldr r1, [sp, #4]                        /* @3C52 0199 ldr r1, [sp, #4]  */
L_3c54:
    str r1, [r0, #4]                        /* @3C54 4160 str r1, [r0, #4]  */
L_3c56:
    pop {r2, r3, r4, pc}                    /* @3C56 1cbd pop {r2, r3, r4, pc}  */
L_3c58:
    .byte 0xe0,0x06,0x00,0x20 /* @3C58 literal */
L_3c5c:
    .byte 0xe0,0x08,0x00,0x20 /* @3C5C literal */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @3C60 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @3C70 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @3C80 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @3C90 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @3CA0 unclassified */

/* sub_3CAC */
L_3cac:
    push {r4, lr}                           /* @3CAC 10b5 push {r4, lr}  */
L_3cae:
    ldr r0, [pc, #16]                       /* @3CAE 0448 ldr r0, [pc, #16] literal 0x3CC0 -> 0x200006C0 */
L_3cb0:
    movs r1, #0                             /* @3CB0 0021 movs r1, #0  */
L_3cb2:
    strb r1, [r0]                           /* @3CB2 0170 strb r1, [r0]  */
L_3cb4:
    strh r1, [r0, #2]                       /* @3CB4 4180 strh r1, [r0, #2]  */
L_3cb6:
    str r1, [r0, #24]                       /* @3CB6 8161 str r1, [r0, #24]  */
L_3cb8:
    str r1, [r0, #28]                       /* @3CB8 c161 str r1, [r0, #28]  */
L_3cba:
    bl L_342c                               /* @3CBA fff7b7fb bl #-2194 -> sub_342C */
L_3cbe:
    pop {r4, pc}                            /* @3CBE 10bd pop {r4, pc}  */
L_3cc0:
    .byte 0xc0,0x06,0x00,0x20 /* @3CC0 literal */
    .byte 0x00,0x00,0x00,0x00 /* @3CC4 unclassified */

/* sub_3CC8 */
L_3cc8:
    push {r1, r2, r3, r4, r5, r6, r7, lr}   /* @3CC8 feb5 push {r1, r2, r3, r4, r5, r6, r7, lr}  */
L_3cca:
    movs r5, #1                             /* @3CCA 0125 movs r5, #1  */
L_3ccc:
    mov r0, sp                              /* @3CCC 6846 mov r0, sp  */
L_3cce:
    strb r5, [r0, #4]                       /* @3CCE 0571 strb r5, [r0, #4]  */
L_3cd0:
    strb r5, [r0, #5]                       /* @3CD0 4571 strb r5, [r0, #5]  */
L_3cd2:
    strb r5, [r0, #6]                       /* @3CD2 8571 strb r5, [r0, #6]  */
L_3cd4:
    movs r4, #0                             /* @3CD4 0024 movs r4, #0  */
L_3cd6:
    strb r4, [r0, #7]                       /* @3CD6 c471 strb r4, [r0, #7]  */
L_3cd8:
    strb r4, [r0, #8]                       /* @3CD8 0472 strb r4, [r0, #8]  */
L_3cda:
    strb r4, [r0, #9]                       /* @3CDA 4472 strb r4, [r0, #9]  */
L_3cdc:
    add r2, sp, #4                          /* @3CDC 01aa add r2, sp, #4  */
L_3cde:
    movs r1, #7                             /* @3CDE 0721 movs r1, #7  */
L_3ce0:
    movs r0, #64                            /* @3CE0 4020 movs r0, #64  */
L_3ce2:
    bl L_137c                               /* @3CE2 fdf74bfb bl #-10602 -> sub_137C */
L_3ce6:
    movs r1, #7                             /* @3CE6 0721 movs r1, #7  */
L_3ce8:
    movs r0, #64                            /* @3CE8 4020 movs r0, #64  */
L_3cea:
    bl L_1304                               /* @3CEA fdf70bfb bl #-10730 -> sub_1304 */
L_3cee:
    movs r2, #2                             /* @3CEE 0222 movs r2, #2  */
L_3cf0:
    movs r1, #7                             /* @3CF0 0721 movs r1, #7  */
L_3cf2:
    movs r0, #64                            /* @3CF2 4020 movs r0, #64  */
L_3cf4:
    bl L_1344                               /* @3CF4 fdf726fb bl #-10676 -> sub_1344 */
L_3cf8:
    movs r2, #3                             /* @3CF8 0322 movs r2, #3  */
L_3cfa:
    movs r1, #7                             /* @3CFA 0721 movs r1, #7  */
L_3cfc:
    movs r0, #64                            /* @3CFC 4020 movs r0, #64  */
L_3cfe:
    bl L_1344                               /* @3CFE fdf721fb bl #-10686 -> sub_1344 */
L_3d02:
    movs r2, #1                             /* @3D02 0122 movs r2, #1  */
L_3d04:
    movs r1, #3                             /* @3D04 0321 movs r1, #3  */
L_3d06:
    mov r0, r2                              /* @3D06 1046 mov r0, r2  */
L_3d08:
    bl L_0fec                               /* @3D08 fdf770f9 bl #-11552 -> sub_0FEC */
L_3d0c:
    movs r1, #6                             /* @3D0C 0621 movs r1, #6  */
L_3d0e:
    add r0, sp, #4                          /* @3D0E 01a8 add r0, sp, #4  */
L_3d10:
    bl L_2d84                               /* @3D10 fff738f8 bl #-3984 -> zero_bytes */
L_3d14:
    mov r0, sp                              /* @3D14 6846 mov r0, sp  */
L_3d16:
    strb r5, [r0, #4]                       /* @3D16 0571 strb r5, [r0, #4]  */
L_3d18:
    strb r4, [r0, #5]                       /* @3D18 4471 strb r4, [r0, #5]  */
L_3d1a:
    strb r4, [r0, #6]                       /* @3D1A 8471 strb r4, [r0, #6]  */
L_3d1c:
    strb r4, [r0, #7]                       /* @3D1C c471 strb r4, [r0, #7]  */
L_3d1e:
    strb r4, [r0, #8]                       /* @3D1E 0472 strb r4, [r0, #8]  */
L_3d20:
    strb r5, [r0, #9]                       /* @3D20 4572 strb r5, [r0, #9]  */
L_3d22:
    add r2, sp, #4                          /* @3D22 01aa add r2, sp, #4  */
L_3d24:
    movs r1, #11                            /* @3D24 0b21 movs r1, #11  */
L_3d26:
    movs r0, #0                             /* @3D26 0020 movs r0, #0  */
L_3d28:
    bl L_137c                               /* @3D28 fdf728fb bl #-10672 -> sub_137C */
L_3d2c:
    movs r2, #7                             /* @3D2C 0722 movs r2, #7  */
L_3d2e:
    movs r1, #11                            /* @3D2E 0b21 movs r1, #11  */
L_3d30:
    movs r0, #0                             /* @3D30 0020 movs r0, #0  */
L_3d32:
    bl L_1408                               /* @3D32 fdf769fb bl #-10542 -> sub_1408 */
L_3d36:
    add r2, sp, #4                          /* @3D36 01aa add r2, sp, #4  */
L_3d38:
    movs r1, #8                             /* @3D38 0821 movs r1, #8  */
L_3d3a:
    movs r0, #0                             /* @3D3A 0020 movs r0, #0  */
L_3d3c:
    bl L_137c                               /* @3D3C fdf71efb bl #-10692 -> sub_137C */
L_3d40:
    movs r2, #6                             /* @3D40 0622 movs r2, #6  */
L_3d42:
    movs r1, #8                             /* @3D42 0821 movs r1, #8  */
L_3d44:
    movs r0, #0                             /* @3D44 0020 movs r0, #0  */
L_3d46:
    bl L_1408                               /* @3D46 fdf75ffb bl #-10562 -> sub_1408 */
L_3d4a:
    movs r1, #3                             /* @3D4A 0321 movs r1, #3  */
L_3d4c:
    mov r0, sp                              /* @3D4C 6846 mov r0, sp  */
L_3d4e:
    bl L_2d84                               /* @3D4E fff719f8 bl #-4046 -> zero_bytes */
L_3d52:
    mov r0, sp                              /* @3D52 6846 mov r0, sp  */
L_3d54:
    strb r4, [r0]                           /* @3D54 0470 strb r4, [r0]  */
L_3d56:
    strb r5, [r0, #1]                       /* @3D56 4570 strb r5, [r0, #1]  */
L_3d58:
    strb r4, [r0, #2]                       /* @3D58 8470 strb r4, [r0, #2]  */
L_3d5a:
    ldr r6, [pc, #104]                      /* @3D5A 1a4e ldr r6, [pc, #104] literal 0x3DC4 -> 0x40003000 */
L_3d5c:
    mov r1, sp                              /* @3D5C 6946 mov r1, sp  */
L_3d5e:
    mov r0, r6                              /* @3D5E 3046 mov r0, r6  */
L_3d60:
    bl L_0824                               /* @3D60 fcf760fd bl #-13632 -> sub_0824 */
L_3d64:
    ldr r1, [pc, #96]                       /* @3D64 1849 ldr r1, [pc, #96] literal 0x3DC8 -> 0x0000FFFF */
L_3d66:
    mov r0, r6                              /* @3D66 3046 mov r0, r6  */
L_3d68:
    bl L_087a                               /* @3D68 fcf787fd bl #-13554 -> sub_087A */
L_3d6c:
    ldr r0, [pc, #84]                       /* @3D6C 1548 ldr r0, [pc, #84] literal 0x3DC4 -> 0x40003000 */
L_3d6e:
    adds r0, #128                           /* @3D6E 8030 adds r0, #128  */
L_3d70:
    str r4, [r0, #8]                        /* @3D70 8460 str r4, [r0, #8]  */
L_3d72:
    str r4, [r0, #12]                       /* @3D72 c460 str r4, [r0, #12]  */
L_3d74:
    ldr r1, [pc, #84]                       /* @3D74 1549 ldr r1, [pc, #84] literal 0x3DCC -> 0x2000072C */
L_3d76:
    ldrb r3, [r1, #4]                       /* @3D76 0b79 ldrb r3, [r1, #4]  */
L_3d78:
    movs r1, #64                            /* @3D78 4021 movs r1, #64  */
L_3d7a:
    movs r2, #16                            /* @3D7A 1022 movs r2, #16  */
L_3d7c:
    cmp r3, #0                              /* @3D7C 002b cmp r3, #0  */
L_3d7e:
    beq L_3d8e                              /* @3D7E 06d0 beq #12 -> 0x3D8E */
L_3d80:
    ldr r3, [r0, #8]                        /* @3D80 8368 ldr r3, [r0, #8]  */
L_3d82:
    orrs r3, r1                             /* @3D82 0b43 orrs r3, r1  */
L_3d84:
    str r3, [r0, #8]                        /* @3D84 8360 str r3, [r0, #8]  */
L_3d86:
    ldr r1, [r0, #12]                       /* @3D86 c168 ldr r1, [r0, #12]  */
L_3d88:
    orrs r1, r2                             /* @3D88 1143 orrs r1, r2  */
L_3d8a:
    str r1, [r0, #12]                       /* @3D8A c160 str r1, [r0, #12]  */
L_3d8c:
    b L_3d9a                                /* @3D8C 05e0 b #10 -> 0x3D9A */
L_3d8e:
    ldr r3, [r0, #8]                        /* @3D8E 8368 ldr r3, [r0, #8]  */
L_3d90:
    orrs r3, r2                             /* @3D90 1343 orrs r3, r2  */
L_3d92:
    str r3, [r0, #8]                        /* @3D92 8360 str r3, [r0, #8]  */
L_3d94:
    ldr r2, [r0, #12]                       /* @3D94 c268 ldr r2, [r0, #12]  */
L_3d96:
    orrs r2, r1                             /* @3D96 0a43 orrs r2, r1  */
L_3d98:
    str r2, [r0, #12]                       /* @3D98 c260 str r2, [r0, #12]  */
L_3d9a:
    ldr r0, [pc, #40]                       /* @3D9A 0a48 ldr r0, [pc, #40] literal 0x3DC4 -> 0x40003000 */
L_3d9c:
    adds r0, #64                            /* @3D9C 4030 adds r0, #64  */
L_3d9e:
    str r4, [r0, #60]                       /* @3D9E c463 str r4, [r0, #60]  */
L_3da0:
    ldr r1, [r0, #40]                       /* @3DA0 816a ldr r1, [r0, #40]  */
L_3da2:
    movs r2, #3                             /* @3DA2 0322 movs r2, #3  */
L_3da4:
    orrs r1, r2                             /* @3DA4 1143 orrs r1, r2  */
L_3da6:
    str r1, [r0, #40]                       /* @3DA6 8162 str r1, [r0, #40]  */
L_3da8:
    ldr r1, [r0, #40]                       /* @3DA8 816a ldr r1, [r0, #40]  */
L_3daa:
    movs r2, #48                            /* @3DAA 3022 movs r2, #48  */
L_3dac:
    orrs r1, r2                             /* @3DAC 1143 orrs r1, r2  */
L_3dae:
    str r1, [r0, #40]                       /* @3DAE 8162 str r1, [r0, #40]  */
L_3db0:
    movs r1, #0                             /* @3DB0 0021 movs r1, #0  */
L_3db2:
    mov r0, r6                              /* @3DB2 3046 mov r0, r6  */
L_3db4:
    bl L_086c                               /* @3DB4 fcf75afd bl #-13644 -> sub_086C */
L_3db8:
    ldr r0, [pc, #20]                       /* @3DB8 0548 ldr r0, [pc, #20] literal 0x3DD0 -> 0x20000604 */
L_3dba:
    strb r5, [r0, #3]                       /* @3DBA c570 strb r5, [r0, #3]  */
L_3dbc:
    bl L_2090                               /* @3DBC fef768f9 bl #-7472 -> sub_2090 */
L_3dc0:
    pop {r1, r2, r3, r4, r5, r6, r7, pc}    /* @3DC0 febd pop {r1, r2, r3, r4, r5, r6, r7, pc}  */
    .byte 0x00,0x00 /* @3DC2 unclassified */
L_3dc4:
    .byte 0x00,0x30,0x00,0x40 /* @3DC4 literal */
L_3dc8:
    .byte 0xff,0xff,0x00,0x00 /* @3DC8 literal */
L_3dcc:
    .byte 0x2c,0x07,0x00,0x20 /* @3DCC literal */
L_3dd0:
    .byte 0x04,0x06,0x00,0x20 /* @3DD0 literal */

/* sub_3DD4 */
L_3dd4:
    push {r4, lr}                           /* @3DD4 10b5 push {r4, lr}  */
L_3dd6:
    ldr r0, [pc, #36]                       /* @3DD6 0948 ldr r0, [pc, #36] literal 0x3DFC -> 0x40003000 */
L_3dd8:
    bl L_088c                               /* @3DD8 fcf758fd bl #-13648 -> sub_088C */
L_3ddc:
    movs r2, #2                             /* @3DDC 0222 movs r2, #2  */
L_3dde:
    movs r1, #7                             /* @3DDE 0721 movs r1, #7  */
L_3de0:
    movs r0, #64                            /* @3DE0 4020 movs r0, #64  */
L_3de2:
    bl L_132c                               /* @3DE2 fdf7a3fa bl #-10938 -> sub_132C */
L_3de6:
    movs r2, #3                             /* @3DE6 0322 movs r2, #3  */
L_3de8:
    movs r1, #7                             /* @3DE8 0721 movs r1, #7  */
L_3dea:
    movs r0, #64                            /* @3DEA 4020 movs r0, #64  */
L_3dec:
    bl L_132c                               /* @3DEC fdf79efa bl #-10948 -> sub_132C */
L_3df0:
    movs r1, #7                             /* @3DF0 0721 movs r1, #7  */
L_3df2:
    movs r0, #64                            /* @3DF2 4020 movs r0, #64  */
L_3df4:
    bl L_1304                               /* @3DF4 fdf786fa bl #-10996 -> sub_1304 */
L_3df8:
    pop {r4, pc}                            /* @3DF8 10bd pop {r4, pc}  */
    .byte 0x00,0x00 /* @3DFA unclassified */
L_3dfc:
    .byte 0x00,0x30,0x00,0x40 /* @3DFC literal */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @3E00 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @3E10 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @3E20 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @3E30 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @3E40 unclassified */
    .byte 0x00,0x00,0x00,0x00 /* @3E50 unclassified */

/* sub_3E54 */
L_3e54:
    push {r4, lr}                           /* @3E54 10b5 push {r4, lr}  */
L_3e56:
    movs r1, #7                             /* @3E56 0721 movs r1, #7  */
L_3e58:
    movs r0, #64                            /* @3E58 4020 movs r0, #64  */
L_3e5a:
    bl L_1304                               /* @3E5A fdf753fa bl #-11098 -> sub_1304 */
L_3e5e:
    movs r2, #2                             /* @3E5E 0222 movs r2, #2  */
L_3e60:
    movs r1, #7                             /* @3E60 0721 movs r1, #7  */
L_3e62:
    movs r0, #64                            /* @3E62 4020 movs r0, #64  */
L_3e64:
    bl L_1344                               /* @3E64 fdf76efa bl #-11044 -> sub_1344 */
L_3e68:
    movs r2, #3                             /* @3E68 0322 movs r2, #3  */
L_3e6a:
    movs r1, #7                             /* @3E6A 0721 movs r1, #7  */
L_3e6c:
    movs r0, #64                            /* @3E6C 4020 movs r0, #64  */
L_3e6e:
    bl L_1344                               /* @3E6E fdf769fa bl #-11054 -> sub_1344 */
L_3e72:
    ldr r0, [pc, #8]                        /* @3E72 0248 ldr r0, [pc, #8] literal 0x3E7C -> 0x40003000 */
L_3e74:
    bl L_0880                               /* @3E74 fcf704fd bl #-13816 -> sub_0880 */
L_3e78:
    pop {r4, pc}                            /* @3E78 10bd pop {r4, pc}  */
    .byte 0x00,0x00 /* @3E7A unclassified */
L_3e7c:
    .byte 0x00,0x30,0x00,0x40 /* @3E7C literal */

/* sub_3E80 */
L_3e80:
    push {r4, lr}                           /* @3E80 10b5 push {r4, lr}  */
L_3e82:
    ldr r0, [pc, #60]                       /* @3E82 0f48 ldr r0, [pc, #60] literal 0x3EC0 -> 0x2000072C */
L_3e84:
    ldrb r1, [r0, #9]                       /* @3E84 417a ldrb r1, [r0, #9]  */
L_3e86:
    cmp r1, #2                              /* @3E86 0229 cmp r1, #2  */
L_3e88:
    beq L_3ea6                              /* @3E88 0dd0 beq #26 -> 0x3EA6 */
L_3e8a:
    ldrb r1, [r0, #8]                       /* @3E8A 017a ldrb r1, [r0, #8]  */
L_3e8c:
    cmp r1, #2                              /* @3E8C 0229 cmp r1, #2  */
L_3e8e:
    beq L_3eac                              /* @3E8E 0dd0 beq #26 -> 0x3EAC */
L_3e90:
    ldrb r0, [r0, #8]                       /* @3E90 007a ldrb r0, [r0, #8]  */
L_3e92:
    cmp r0, #1                              /* @3E92 0128 cmp r0, #1  */
L_3e94:
    beq L_3eb0                              /* @3E94 0cd0 beq #24 -> 0x3EB0 */
L_3e96:
    movs r1, #7                             /* @3E96 0721 movs r1, #7  */
L_3e98:
    movs r0, #64                            /* @3E98 4020 movs r0, #64  */
L_3e9a:
    bl L_135c                               /* @3E9A fdf75ffa bl #-11074 -> sub_135C */
L_3e9e:
    cmp r0, #0                              /* @3E9E 0028 cmp r0, #0  */
L_3ea0:
    beq L_3eba                              /* @3EA0 0bd0 beq #22 -> 0x3EBA */
L_3ea2:
    movs r0, #0                             /* @3EA2 0020 movs r0, #0  */
L_3ea4:
    pop {r4, pc}                            /* @3EA4 10bd pop {r4, pc}  */
L_3ea6:
    ldr r0, [pc, #28]                       /* @3EA6 0748 ldr r0, [pc, #28] literal 0x3EC4 -> 0x20000604 */
L_3ea8:
    ldrb r0, [r0, #3]                       /* @3EA8 c078 ldrb r0, [r0, #3]  */
L_3eaa:
    pop {r4, pc}                            /* @3EAA 10bd pop {r4, pc}  */
L_3eac:
    movs r0, #1                             /* @3EAC 0120 movs r0, #1  */
L_3eae:
    pop {r4, pc}                            /* @3EAE 10bd pop {r4, pc}  */
L_3eb0:
    movs r1, #7                             /* @3EB0 0721 movs r1, #7  */
L_3eb2:
    movs r0, #64                            /* @3EB2 4020 movs r0, #64  */
L_3eb4:
    bl L_135c                               /* @3EB4 fdf752fa bl #-11100 -> sub_135C */
L_3eb8:
    pop {r4, pc}                            /* @3EB8 10bd pop {r4, pc}  */
L_3eba:
    movs r0, #1                             /* @3EBA 0120 movs r0, #1  */
L_3ebc:
    pop {r4, pc}                            /* @3EBC 10bd pop {r4, pc}  */
    .byte 0x00,0x00 /* @3EBE unclassified */
L_3ec0:
    .byte 0x2c,0x07,0x00,0x20 /* @3EC0 literal */
L_3ec4:
    .byte 0x04,0x06,0x00,0x20 /* @3EC4 literal */
    .byte 0x00,0x00,0x00,0x00 /* @3EC8 unclassified */

/* sub_3ECC */
L_3ecc:
    push {r4, r5, r6, lr}                   /* @3ECC 70b5 push {r4, r5, r6, lr}  */
L_3ece:
    ldr r5, [sp, #16]                       /* @3ECE 049d ldr r5, [sp, #16]  */
L_3ed0:
    mov r4, r1                              /* @3ED0 0c46 mov r4, r1  */
L_3ed2:
    subs r1, r2, r0                         /* @3ED2 111a subs r1, r2, r0  */
L_3ed4:
    subs r2, r3, r4                         /* @3ED4 1a1b subs r2, r3, r4  */
L_3ed6:
    subs r0, r5, r0                         /* @3ED6 281a subs r0, r5, r0  */
L_3ed8:
    cmp r1, #0                              /* @3ED8 0029 cmp r1, #0  */
L_3eda:
    beq L_3f00                              /* @3EDA 11d0 beq #34 -> 0x3F00 */
L_3edc:
    muls r0, r2, r0                         /* @3EDC 5043 muls r0, r2, r0  */
L_3ede:
    asrs r2, r1, #1                         /* @3EDE 4a10 asrs r2, r1, #1  */
L_3ee0:
    adds r0, r0, r2                         /* @3EE0 8018 adds r0, r0, r2  */
L_3ee2:
    bl L_013c                               /* @3EE2 fcf72bf9 bl #-15786 -> sdivmod32 */
L_3ee6:
    adds r0, r0, r4                         /* @3EE6 0019 adds r0, r0, r4  */
L_3ee8:
    movs r1, #1                             /* @3EE8 0121 movs r1, #1  */
L_3eea:
    lsls r1, r1, #16                        /* @3EEA 0904 lsls r1, r1, #16  */
L_3eec:
    cmp r0, #0                              /* @3EEC 0028 cmp r0, #0  */
L_3eee:
    bge L_3ef4                              /* @3EEE 01da bge #2 -> 0x3EF4 */
L_3ef0:
    adds r0, r0, r1                         /* @3EF0 4018 adds r0, r0, r1  */
L_3ef2:
    b L_3efc                                /* @3EF2 03e0 b #6 -> 0x3EFC */
L_3ef4:
    ldr r2, [pc, #20]                       /* @3EF4 054a ldr r2, [pc, #20] literal 0x3F0C -> 0x0000FFFF */
L_3ef6:
    cmp r0, r2                              /* @3EF6 9042 cmp r0, r2  */
L_3ef8:
    ble L_3efc                              /* @3EF8 00dd ble #0 -> 0x3EFC */
L_3efa:
    subs r0, r0, r1                         /* @3EFA 401a subs r0, r0, r1  */
L_3efc:
    uxth r0, r0                             /* @3EFC 80b2 uxth r0, r0  */
L_3efe:
    pop {r4, r5, r6, pc}                    /* @3EFE 70bd pop {r4, r5, r6, pc}  */
L_3f00:
    adr r2, L_3f10                          /* @3F00 03a2 adr r2, #12 address 0x3F10 -> 0x00003F10 'Error!' */
L_3f02:
    adr r1, L_3f18                          /* @3F02 05a1 adr r1, #20 address 0x3F18 -> 0x00003F18 'Reboot!' */
L_3f04:
    adr r0, L_3f20                          /* @3F04 06a0 adr r0, #24 address 0x3F20 -> 0x00003F20 'dx=0?' */
L_3f06:
    bl L_4884                               /* @3F06 00f0bdfc bl #2426 -> display_three_lines */
L_3f0a:
    b L_3f0a                                /* @3F0A fee7 b #-4 -> 0x3F0A */
L_3f0c:
    .byte 0xff,0xff,0x00,0x00 /* @3F0C literal */
L_3f10:
    .byte 0x45,0x72,0x72,0x6f,0x72,0x21,0x00,0x00 /* @3F10 unclassified */
L_3f18:
    .byte 0x52,0x65,0x62,0x6f,0x6f,0x74,0x21,0x00 /* @3F18 unclassified */
L_3f20:
    .byte 0x64,0x78,0x3d,0x30,0x3f,0x00,0x00,0x00 /* @3F20 unclassified */

/* sub_3F28 */
L_3f28:
    movs r2, #1                             /* @3F28 0122 movs r2, #1  */
L_3f2a:
    ldr r1, [pc, #24]                       /* @3F2A 0649 ldr r1, [pc, #24] literal 0x3F44 -> 0x40020D80 */
L_3f2c:
    lsls r2, r2, #14                        /* @3F2C 9203 lsls r2, r2, #14  */
L_3f2e:
    cmp r0, #0                              /* @3F2E 0028 cmp r0, #0  */
L_3f30:
    beq L_3f3a                              /* @3F30 03d0 beq #6 -> 0x3F3A */
L_3f32:
    ldr r0, [r1, #8]                        /* @3F32 8868 ldr r0, [r1, #8]  */
L_3f34:
    orrs r0, r2                             /* @3F34 1043 orrs r0, r2  */
L_3f36:
    str r0, [r1, #8]                        /* @3F36 8860 str r0, [r1, #8]  */
L_3f38:
    bx lr                                   /* @3F38 7047 bx lr  */
L_3f3a:
    ldr r0, [r1, #8]                        /* @3F3A 8868 ldr r0, [r1, #8]  */
L_3f3c:
    bics r0, r2                             /* @3F3C 9043 bics r0, r2  */
L_3f3e:
    str r0, [r1, #8]                        /* @3F3E 8860 str r0, [r1, #8]  */
L_3f40:
    bx lr                                   /* @3F40 7047 bx lr  */
    .byte 0x00,0x00 /* @3F42 unclassified */
L_3f44:
    .byte 0x80,0x0d,0x02,0x40 /* @3F44 literal */

/* sub_3F48 */
L_3f48:
    push {r3, r4, r5, r6, r7, lr}           /* @3F48 f8b5 push {r3, r4, r5, r6, r7, lr}  */
L_3f4a:
    mov r4, r0                              /* @3F4A 0446 mov r4, r0  */
L_3f4c:
    mov r7, r1                              /* @3F4C 0f46 mov r7, r1  */
L_3f4e:
    movs r6, #1                             /* @3F4E 0126 movs r6, #1  */
L_3f50:
    ldr r5, [pc, #44]                       /* @3F50 0b4d ldr r5, [pc, #44] literal 0x3F80 -> 0x40020D80 */
L_3f52:
    lsls r6, r6, #14                        /* @3F52 b603 lsls r6, r6, #14  */
L_3f54:
    b L_3f78                                /* @3F54 10e0 b #32 -> 0x3F78 */
L_3f56:
    cmp r7, #0                              /* @3F56 002f cmp r7, #0  */
L_3f58:
    beq L_3f60                              /* @3F58 02d0 beq #4 -> 0x3F60 */
L_3f5a:
    mov r0, r7                              /* @3F5A 3846 mov r0, r7  */
L_3f5c:
    bl L_2de8                               /* @3F5C fef744ff bl #-4472 -> delay_ms_candidate */
L_3f60:
    ldr r0, [r5, #8]                        /* @3F60 a868 ldr r0, [r5, #8]  */
L_3f62:
    lsls r0, r0, #17                        /* @3F62 4004 lsls r0, r0, #17  */
L_3f64:
    bmi L_3f6e                              /* @3F64 03d4 bmi #6 -> 0x3F6E */
L_3f66:
    ldr r0, [r5, #8]                        /* @3F66 a868 ldr r0, [r5, #8]  */
L_3f68:
    orrs r0, r6                             /* @3F68 3043 orrs r0, r6  */
L_3f6a:
    str r0, [r5, #8]                        /* @3F6A a860 str r0, [r5, #8]  */
L_3f6c:
    b L_3f74                                /* @3F6C 02e0 b #4 -> 0x3F74 */
L_3f6e:
    ldr r0, [r5, #8]                        /* @3F6E a868 ldr r0, [r5, #8]  */
L_3f70:
    bics r0, r6                             /* @3F70 b043 bics r0, r6  */
L_3f72:
    str r0, [r5, #8]                        /* @3F72 a860 str r0, [r5, #8]  */
L_3f74:
    subs r4, r4, #1                         /* @3F74 641e subs r4, r4, #1  */
L_3f76:
    uxth r4, r4                             /* @3F76 a4b2 uxth r4, r4  */
L_3f78:
    cmp r4, #0                              /* @3F78 002c cmp r4, #0  */
L_3f7a:
    bne L_3f56                              /* @3F7A ecd1 bne #-40 -> 0x3F56 */
L_3f7c:
    pop {r3, r4, r5, r6, r7, pc}            /* @3F7C f8bd pop {r3, r4, r5, r6, r7, pc}  */
    .byte 0x00,0x00 /* @3F7E unclassified */
L_3f80:
    .byte 0x80,0x0d,0x02,0x40 /* @3F80 literal */

/* sub_3F84 */
L_3f84:
    push {r2, r3, r4, lr}                   /* @3F84 1cb5 push {r2, r3, r4, lr}  */
L_3f86:
    movs r0, #0                             /* @3F86 0020 movs r0, #0  */
L_3f88:
    mov r1, sp                              /* @3F88 6946 mov r1, sp  */
L_3f8a:
    strb r0, [r1]                           /* @3F8A 0870 strb r0, [r1]  */
L_3f8c:
    strb r0, [r1, #4]                       /* @3F8C 0871 strb r0, [r1, #4]  */
L_3f8e:
    movs r0, #1                             /* @3F8E 0120 movs r0, #1  */
L_3f90:
    strb r0, [r1, #5]                       /* @3F90 4871 strb r0, [r1, #5]  */
L_3f92:
    movs r1, #14                            /* @3F92 0e21 movs r1, #14  */
L_3f94:
    movs r0, #128                           /* @3F94 8020 movs r0, #128  */
L_3f96:
    bl L_1318                               /* @3F96 fdf7bff9 bl #-11394 -> sub_1318 */
L_3f9a:
    mov r2, sp                              /* @3F9A 6a46 mov r2, sp  */
L_3f9c:
    movs r1, #14                            /* @3F9C 0e21 movs r1, #14  */
L_3f9e:
    movs r0, #128                           /* @3F9E 8020 movs r0, #128  */
L_3fa0:
    bl L_137c                               /* @3FA0 fdf7ecf9 bl #-11304 -> sub_137C */
L_3fa4:
    pop {r2, r3, r4, pc}                    /* @3FA4 1cbd pop {r2, r3, r4, pc}  */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @3FA6 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @3FB6 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00 /* @3FC6 unclassified */

/* menu_acc */
L_3fcc:
    push {r4, lr}                           /* @3FCC 10b5 push {r4, lr}  */
L_3fce:
    mov r4, r0                              /* @3FCE 0446 mov r4, r0  */
L_3fd0:
    ldr r0, [pc, #104]                      /* @3FD0 1a48 ldr r0, [pc, #104] literal 0x403C -> 0x2000072C */
L_3fd2:
    cmp r4, #255                            /* @3FD2 ff2c cmp r4, #255  */
L_3fd4:
    beq L_4038                              /* @3FD4 30d0 beq #96 -> 0x4038 */
L_3fd6:
    ldrh r0, [r0, #24]                      /* @3FD6 008b ldrh r0, [r0, #24]  */
L_3fd8:
    cmp r0, r4                              /* @3FD8 a042 cmp r0, r4  */
L_3fda:
    beq L_4034                              /* @3FDA 2bd0 beq #86 -> 0x4034 */
L_3fdc:
    bl L_52cc                               /* @3FDC 01f076f9 bl #4844 -> sub_52CC */
L_3fe0:
    ldr r1, [pc, #92]                       /* @3FE0 1749 ldr r1, [pc, #92] literal 0x4040 -> 0x200006F4 */
L_3fe2:
    strh r4, [r1, #24]                      /* @3FE2 0c83 strh r4, [r1, #24]  */
L_3fe4:
    ldrh r2, [r1, #24]                      /* @3FE4 0a8b ldrh r2, [r1, #24]  */
L_3fe6:
    movs r0, #255                           /* @3FE6 ff20 movs r0, #255  */
L_3fe8:
    adds r0, #31                            /* @3FE8 1f30 adds r0, #31  */
L_3fea:
    movs r3, r2                             /* @3FEA 1300 movs r3, r2  */
L_3fec:
    bl L_15ec                               /* @3FEC fdf7fefa bl #-10756 -> switch_u8_dispatch ; inline switch destinations 0x3FFA,0x3FFE,0x4002,0x400A,0x4010,0x4018,0x4020,0x4028,0x402E */
    .byte 0x08,0x05,0x07,0x09,0x0d,0x10,0x14,0x18,0x1c,0x1f /* @3FF0 switch_table */
L_3ffa:
    strh r0, [r1, #48]                      /* @3FFA 0886 strh r0, [r1, #48]  */
L_3ffc:
    b L_4030                                /* @3FFC 18e0 b #48 -> 0x4030 */
L_3ffe:
    strh r0, [r1, #48]                      /* @3FFE 0886 strh r0, [r1, #48]  */
L_4000:
    b L_4030                                /* @4000 16e0 b #44 -> 0x4030 */
L_4002:
    movs r0, #255                           /* @4002 ff20 movs r0, #255  */
L_4004:
    adds r0, #157                           /* @4004 9d30 adds r0, #157  */
L_4006:
    strh r0, [r1, #48]                      /* @4006 0886 strh r0, [r1, #48]  */
L_4008:
    b L_4030                                /* @4008 12e0 b #36 -> 0x4030 */
L_400a:
    ldr r0, [pc, #56]                       /* @400A 0e48 ldr r0, [pc, #56] literal 0x4044 -> 0x0000021A */
L_400c:
    strh r0, [r1, #48]                      /* @400C 0886 strh r0, [r1, #48]  */
L_400e:
    b L_4030                                /* @400E 0fe0 b #30 -> 0x4030 */
L_4010:
    movs r0, #83                            /* @4010 5320 movs r0, #83  */
L_4012:
    lsls r0, r0, #3                         /* @4012 c000 lsls r0, r0, #3  */
L_4014:
    strh r0, [r1, #48]                      /* @4014 0886 strh r0, [r1, #48]  */
L_4016:
    b L_4030                                /* @4016 0be0 b #22 -> 0x4030 */
L_4018:
    ldr r0, [pc, #40]                       /* @4018 0a48 ldr r0, [pc, #40] literal 0x4044 -> 0x0000021A */
L_401a:
    adds r0, #252                           /* @401A fc30 adds r0, #252  */
L_401c:
    strh r0, [r1, #48]                      /* @401C 0886 strh r0, [r1, #48]  */
L_401e:
    b L_4030                                /* @401E 07e0 b #14 -> 0x4030 */
L_4020:
    movs r0, #229                           /* @4020 e520 movs r0, #229  */
L_4022:
    lsls r0, r0, #2                         /* @4022 8000 lsls r0, r0, #2  */
L_4024:
    strh r0, [r1, #48]                      /* @4024 0886 strh r0, [r1, #48]  */
L_4026:
    b L_4030                                /* @4026 03e0 b #6 -> 0x4030 */
L_4028:
    ldr r0, [pc, #28]                       /* @4028 0748 ldr r0, [pc, #28] literal 0x4048 -> 0x00000412 */
L_402a:
    strh r0, [r1, #48]                      /* @402A 0886 strh r0, [r1, #48]  */
L_402c:
    b L_4030                                /* @402C 00e0 b #0 -> 0x4030 */
L_402e:
    strh r0, [r1, #48]                      /* @402E 0886 strh r0, [r1, #48]  */
L_4030:
    bl L_470c                               /* @4030 00f06cfb bl #1752 -> sub_470C */
L_4034:
    mov r0, r4                              /* @4034 2046 mov r0, r4  */
L_4036:
    pop {r4, pc}                            /* @4036 10bd pop {r4, pc}  */
L_4038:
    ldrh r0, [r0, #24]                      /* @4038 008b ldrh r0, [r0, #24]  */
L_403a:
    pop {r4, pc}                            /* @403A 10bd pop {r4, pc}  */
L_403c:
    .byte 0x2c,0x07,0x00,0x20 /* @403C literal */
L_4040:
    .byte 0xf4,0x06,0x00,0x20 /* @4040 literal */
L_4044:
    .byte 0x1a,0x02,0x00,0x00 /* @4044 literal */
L_4048:
    .byte 0x12,0x04,0x00,0x00 /* @4048 literal */

/* menu_autosdd */
L_404c:
    push {r4, lr}                           /* @404C 10b5 push {r4, lr}  */
L_404e:
    mov r4, r0                              /* @404E 0446 mov r4, r0  */
L_4050:
    ldr r0, [pc, #40]                       /* @4050 0a48 ldr r0, [pc, #40] literal 0x407C -> 0x2000072C */
L_4052:
    cmp r4, #255                            /* @4052 ff2c cmp r4, #255  */
L_4054:
    beq L_406c                              /* @4054 0ad0 beq #20 -> 0x406C */
L_4056:
    ldrb r0, [r0, #10]                      /* @4056 807a ldrb r0, [r0, #10]  */
L_4058:
    cmp r0, r4                              /* @4058 a042 cmp r0, r4  */
L_405a:
    beq L_4068                              /* @405A 05d0 beq #10 -> 0x4068 */
L_405c:
    bl L_52cc                               /* @405C 01f036f9 bl #4716 -> sub_52CC */
L_4060:
    ldr r0, [pc, #28]                       /* @4060 0748 ldr r0, [pc, #28] literal 0x4080 -> 0x200006F4 */
L_4062:
    strb r4, [r0, #10]                      /* @4062 8472 strb r4, [r0, #10]  */
L_4064:
    bl L_470c                               /* @4064 00f052fb bl #1700 -> sub_470C */
L_4068:
    mov r0, r4                              /* @4068 2046 mov r0, r4  */
L_406a:
    pop {r4, pc}                            /* @406A 10bd pop {r4, pc}  */
L_406c:
    ldrb r0, [r0, #10]                      /* @406C 807a ldrb r0, [r0, #10]  */
L_406e:
    cmp r0, #0                              /* @406E 0028 cmp r0, #0  */
L_4070:
    beq L_4076                              /* @4070 01d0 beq #2 -> 0x4076 */
L_4072:
    movs r0, #1                             /* @4072 0120 movs r0, #1  */
L_4074:
    pop {r4, pc}                            /* @4074 10bd pop {r4, pc}  */
L_4076:
    movs r0, #0                             /* @4076 0020 movs r0, #0  */
L_4078:
    pop {r4, pc}                            /* @4078 10bd pop {r4, pc}  */
    .byte 0x00,0x00 /* @407A unclassified */
L_407c:
    .byte 0x2c,0x07,0x00,0x20 /* @407C literal */
L_4080:
    .byte 0xf4,0x06,0x00,0x20 /* @4080 literal */

/* menu_uartbaud */
L_4084:
    push {r3, r4, r5, r6, r7, lr}           /* @4084 f8b5 push {r3, r4, r5, r6, r7, lr}  */
L_4086:
    mov r5, r0                              /* @4086 0546 mov r5, r0  */
L_4088:
    movs r4, #75                            /* @4088 4b24 movs r4, #75  */
L_408a:
    lsls r4, r4, #9                         /* @408A 6402 lsls r4, r4, #9  */
L_408c:
    asrs r2, r4, #1                         /* @408C 6210 asrs r2, r4, #1  */
L_408e:
    movs r7, #225                           /* @408E e127 movs r7, #225  */
L_4090:
    ldr r6, [pc, #144]                      /* @4090 244e ldr r6, [pc, #144] literal 0x4124 -> 0x000061A8 */
L_4092:
    lsls r7, r7, #8                         /* @4092 3f02 lsls r7, r7, #8  */
L_4094:
    ldr r1, [pc, #144]                      /* @4094 2449 ldr r1, [pc, #144] literal 0x4128 -> 0x2000072C */
L_4096:
    asrs r0, r2, #1                         /* @4096 5010 asrs r0, r2, #1  */
L_4098:
    cmp r5, #255                            /* @4098 ff2d cmp r5, #255  */
L_409a:
    beq L_40dc                              /* @409A 1fd0 beq #62 -> 0x40DC */
L_409c:
    movs r3, r5                             /* @409C 2b00 movs r3, r5  */
L_409e:
    bl L_15ec                               /* @409E fdf7a5fa bl #-10934 -> switch_u8_dispatch ; inline switch destinations 0x40C0,0x40AC,0x40B0,0x40B4,0x40C0,0x40B8,0x40BC,0x40C0 */
    .byte 0x07,0x0f,0x05,0x07,0x09,0x0f,0x0b,0x0d,0x0f /* @40A2 switch_table */
    .byte 0x00 /* @40AB unclassified */
L_40ac:
    mov r4, r0                              /* @40AC 0446 mov r4, r0  */
L_40ae:
    b L_40c0                                /* @40AE 07e0 b #14 -> 0x40C0 */
L_40b0:
    mov r4, r2                              /* @40B0 1446 mov r4, r2  */
L_40b2:
    b L_40c0                                /* @40B2 05e0 b #10 -> 0x40C0 */
L_40b4:
    mov r4, r6                              /* @40B4 3446 mov r4, r6  */
L_40b6:
    b L_40c0                                /* @40B6 03e0 b #6 -> 0x40C0 */
L_40b8:
    mov r4, r7                              /* @40B8 3c46 mov r4, r7  */
L_40ba:
    b L_40c0                                /* @40BA 01e0 b #2 -> 0x40C0 */
L_40bc:
    movs r4, #225                           /* @40BC e124 movs r4, #225  */
L_40be:
    lsls r4, r4, #9                         /* @40BE 6402 lsls r4, r4, #9  */
L_40c0:
    ldr r0, [r1, #16]                       /* @40C0 0869 ldr r0, [r1, #16]  */
L_40c2:
    cmp r0, r4                              /* @40C2 a042 cmp r0, r4  */
L_40c4:
    beq L_40d8                              /* @40C4 08d0 beq #16 -> 0x40D8 */
L_40c6:
    bl L_52cc                               /* @40C6 01f001f9 bl #4610 -> sub_52CC */
L_40ca:
    ldr r1, [pc, #96]                       /* @40CA 1849 ldr r1, [pc, #96] literal 0x412C -> 0x200005D9 */
L_40cc:
    movs r0, #3                             /* @40CC 0320 movs r0, #3  */
L_40ce:
    strb r0, [r1]                           /* @40CE 0870 strb r0, [r1]  */
L_40d0:
    ldr r0, [pc, #92]                       /* @40D0 1748 ldr r0, [pc, #92] literal 0x4130 -> 0x200006F4 */
L_40d2:
    str r4, [r0, #16]                       /* @40D2 0461 str r4, [r0, #16]  */
L_40d4:
    bl L_470c                               /* @40D4 00f01afb bl #1588 -> sub_470C */
L_40d8:
    mov r0, r5                              /* @40D8 2846 mov r0, r5  */
L_40da:
    pop {r3, r4, r5, r6, r7, pc}            /* @40DA f8bd pop {r3, r4, r5, r6, r7, pc}  */
L_40dc:
    ldr r3, [r1, #16]                       /* @40DC 0b69 ldr r3, [r1, #16]  */
L_40de:
    subs r4, r3, r6                         /* @40DE 9c1b subs r4, r3, r6  */
L_40e0:
    cmp r3, r6                              /* @40E0 b342 cmp r3, r6  */
L_40e2:
    beq L_4116                              /* @40E2 18d0 beq #48 -> 0x4116 */
L_40e4:
    bgt L_40f6                              /* @40E4 07dc bgt #14 -> 0x40F6 */
L_40e6:
    cmp r3, #0                              /* @40E6 002b cmp r3, #0  */
L_40e8:
    beq L_410e                              /* @40E8 11d0 beq #34 -> 0x410E */
L_40ea:
    subs r2, r3, r0                         /* @40EA 1a1a subs r2, r3, r0  */
L_40ec:
    beq L_4112                              /* @40EC 11d0 beq #34 -> 0x4112 */
L_40ee:
    cmp r2, r0                              /* @40EE 8242 cmp r2, r0  */
L_40f0:
    bne L_4108                              /* @40F0 0ad1 bne #20 -> 0x4108 */
L_40f2:
    movs r0, #2                             /* @40F2 0220 movs r0, #2  */
L_40f4:
    pop {r3, r4, r5, r6, r7, pc}            /* @40F4 f8bd pop {r3, r4, r5, r6, r7, pc}  */
L_40f6:
    ldr r0, [pc, #60]                       /* @40F6 0f48 ldr r0, [pc, #60] literal 0x4134 -> 0xFFFFCBA8 */
L_40f8:
    adds r0, r4, r0                         /* @40F8 2018 adds r0, r4, r0  */
L_40fa:
    beq L_411a                              /* @40FA 0ed0 beq #28 -> 0x411A */
L_40fc:
    subs r0, r0, r2                         /* @40FC 801a subs r0, r0, r2  */
L_40fe:
    beq L_411e                              /* @40FE 0ed0 beq #28 -> 0x411E */
L_4100:
    cmp r0, r7                              /* @4100 b842 cmp r0, r7  */
L_4102:
    bne L_4108                              /* @4102 01d1 bne #2 -> 0x4108 */
L_4104:
    movs r0, #6                             /* @4104 0620 movs r0, #6  */
L_4106:
    pop {r3, r4, r5, r6, r7, pc}            /* @4106 f8bd pop {r3, r4, r5, r6, r7, pc}  */
L_4108:
    movs r0, #0                             /* @4108 0020 movs r0, #0  */
L_410a:
    str r0, [r1, #16]                       /* @410A 0861 str r0, [r1, #16]  */
L_410c:
    pop {r3, r4, r5, r6, r7, pc}            /* @410C f8bd pop {r3, r4, r5, r6, r7, pc}  */
L_410e:
    movs r0, #0                             /* @410E 0020 movs r0, #0  */
L_4110:
    pop {r3, r4, r5, r6, r7, pc}            /* @4110 f8bd pop {r3, r4, r5, r6, r7, pc}  */
L_4112:
    movs r0, #1                             /* @4112 0120 movs r0, #1  */
L_4114:
    pop {r3, r4, r5, r6, r7, pc}            /* @4114 f8bd pop {r3, r4, r5, r6, r7, pc}  */
L_4116:
    movs r0, #3                             /* @4116 0320 movs r0, #3  */
L_4118:
    pop {r3, r4, r5, r6, r7, pc}            /* @4118 f8bd pop {r3, r4, r5, r6, r7, pc}  */
L_411a:
    movs r0, #4                             /* @411A 0420 movs r0, #4  */
L_411c:
    pop {r3, r4, r5, r6, r7, pc}            /* @411C f8bd pop {r3, r4, r5, r6, r7, pc}  */
L_411e:
    movs r0, #5                             /* @411E 0520 movs r0, #5  */
L_4120:
    pop {r3, r4, r5, r6, r7, pc}            /* @4120 f8bd pop {r3, r4, r5, r6, r7, pc}  */
    .byte 0x00,0x00 /* @4122 unclassified */
L_4124:
    .byte 0xa8,0x61,0x00,0x00 /* @4124 literal */
L_4128:
    .byte 0x2c,0x07,0x00,0x20 /* @4128 literal */
L_412c:
    .byte 0xd9,0x05,0x00,0x20 /* @412C literal */
L_4130:
    .byte 0xf4,0x06,0x00,0x20 /* @4130 literal */
L_4134:
    .byte 0xa8,0xcb,0xff,0xff /* @4134 literal */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @4138 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @4148 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @4158 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @4168 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @4178 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @4188 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @4198 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @41A8 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @41B8 unclassified */

/* menu_ma */
L_41c8:
    push {r4, r5, r6, lr}                   /* @41C8 70b5 push {r4, r5, r6, lr}  */
L_41ca:
    mov r4, r0                              /* @41CA 0446 mov r4, r0  */
L_41cc:
    ldr r0, [pc, #48]                       /* @41CC 0c48 ldr r0, [pc, #48] literal 0x4200 -> 0x200005E4 */
L_41ce:
    cmp r4, #255                            /* @41CE ff2c cmp r4, #255  */
L_41d0:
    beq L_41f6                              /* @41D0 11d0 beq #34 -> 0x41F6 */
L_41d2:
    ldrh r1, [r0]                           /* @41D2 0188 ldrh r1, [r0]  */
L_41d4:
    mov r0, r4                              /* @41D4 2046 mov r0, r4  */
L_41d6:
    movs r2, #200                           /* @41D6 c822 movs r2, #200  */
L_41d8:
    muls r0, r2, r0                         /* @41D8 5043 muls r0, r2, r0  */
L_41da:
    mov r5, r0                              /* @41DA 0546 mov r5, r0  */
L_41dc:
    cmp r1, r0                              /* @41DC 8142 cmp r1, r0  */
L_41de:
    beq L_41f2                              /* @41DE 08d0 beq #16 -> 0x41F2 */
L_41e0:
    bl L_52cc                               /* @41E0 01f074f8 bl #4328 -> sub_52CC */
L_41e4:
    ldr r0, [pc, #28]                       /* @41E4 0748 ldr r0, [pc, #28] literal 0x4204 -> 0x200006F4 */
L_41e6:
    strh r5, [r0, #28]                      /* @41E6 8583 strh r5, [r0, #28]  */
L_41e8:
    movs r1, #100                           /* @41E8 6421 movs r1, #100  */
L_41ea:
    muls r1, r4, r1                         /* @41EA 6143 muls r1, r4, r1  */
L_41ec:
    strh r1, [r0, #30]                      /* @41EC c183 strh r1, [r0, #30]  */
L_41ee:
    bl L_470c                               /* @41EE 00f08dfa bl #1306 -> sub_470C */
L_41f2:
    mov r0, r4                              /* @41F2 2046 mov r0, r4  */
L_41f4:
    pop {r4, r5, r6, pc}                    /* @41F4 70bd pop {r4, r5, r6, pc}  */
L_41f6:
    ldrh r0, [r0]                           /* @41F6 0088 ldrh r0, [r0]  */
L_41f8:
    movs r1, #200                           /* @41F8 c821 movs r1, #200  */
L_41fa:
    bl L_0110                               /* @41FA fbf789ff bl #-16622 -> udivmod32 */
L_41fe:
    pop {r4, r5, r6, pc}                    /* @41FE 70bd pop {r4, r5, r6, pc}  */
L_4200:
    .byte 0xe4,0x05,0x00,0x20 /* @4200 literal */
L_4204:
    .byte 0xf4,0x06,0x00,0x20 /* @4204 literal */
    .byte 0x00,0x00,0x00,0x00 /* @4208 unclassified */

/* menu_dir */
L_420c:
    push {r4, lr}                           /* @420C 10b5 push {r4, lr}  */
L_420e:
    mov r4, r0                              /* @420E 0446 mov r4, r0  */
L_4210:
    ldr r0, [pc, #36]                       /* @4210 0948 ldr r0, [pc, #36] literal 0x4238 -> 0x2000072C */
L_4212:
    cmp r4, #255                            /* @4212 ff2c cmp r4, #255  */
L_4214:
    beq L_4232                              /* @4214 0dd0 beq #26 -> 0x4232 */
L_4216:
    ldrb r0, [r0, #4]                       /* @4216 0079 ldrb r0, [r0, #4]  */
L_4218:
    cmp r0, r4                              /* @4218 a042 cmp r0, r4  */
L_421a:
    beq L_422e                              /* @421A 08d0 beq #16 -> 0x422E */
L_421c:
    bl L_52cc                               /* @421C 01f056f8 bl #4268 -> sub_52CC */
L_4220:
    ldr r1, [pc, #24]                       /* @4220 0649 ldr r1, [pc, #24] literal 0x423C -> 0x200005D9 */
L_4222:
    movs r0, #2                             /* @4222 0220 movs r0, #2  */
L_4224:
    strb r0, [r1]                           /* @4224 0870 strb r0, [r1]  */
L_4226:
    ldr r0, [pc, #24]                       /* @4226 0648 ldr r0, [pc, #24] literal 0x4240 -> 0x200006F4 */
L_4228:
    strb r4, [r0, #4]                       /* @4228 0471 strb r4, [r0, #4]  */
L_422a:
    bl L_470c                               /* @422A 00f06ffa bl #1246 -> sub_470C */
L_422e:
    mov r0, r4                              /* @422E 2046 mov r0, r4  */
L_4230:
    pop {r4, pc}                            /* @4230 10bd pop {r4, pc}  */
L_4232:
    ldrb r0, [r0, #4]                       /* @4232 0079 ldrb r0, [r0, #4]  */
L_4234:
    pop {r4, pc}                            /* @4234 10bd pop {r4, pc}  */
    .byte 0x00,0x00 /* @4236 unclassified */
L_4238:
    .byte 0x2c,0x07,0x00,0x20 /* @4238 literal */
L_423c:
    .byte 0xd9,0x05,0x00,0x20 /* @423C literal */
L_4240:
    .byte 0xf4,0x06,0x00,0x20 /* @4240 literal */

/* menu_en */
L_4244:
    push {r4, lr}                           /* @4244 10b5 push {r4, lr}  */
L_4246:
    mov r4, r0                              /* @4246 0446 mov r4, r0  */
L_4248:
    ldr r0, [pc, #28]                       /* @4248 0748 ldr r0, [pc, #28] literal 0x4268 -> 0x2000072C */
L_424a:
    cmp r4, #255                            /* @424A ff2c cmp r4, #255  */
L_424c:
    beq L_4264                              /* @424C 0ad0 beq #20 -> 0x4264 */
L_424e:
    ldrb r0, [r0, #8]                       /* @424E 007a ldrb r0, [r0, #8]  */
L_4250:
    cmp r0, r4                              /* @4250 a042 cmp r0, r4  */
L_4252:
    beq L_4260                              /* @4252 05d0 beq #10 -> 0x4260 */
L_4254:
    bl L_52cc                               /* @4254 01f03af8 bl #4212 -> sub_52CC */
L_4258:
    ldr r0, [pc, #16]                       /* @4258 0448 ldr r0, [pc, #16] literal 0x426C -> 0x200006F4 */
L_425a:
    strb r4, [r0, #8]                       /* @425A 0472 strb r4, [r0, #8]  */
L_425c:
    bl L_470c                               /* @425C 00f056fa bl #1196 -> sub_470C */
L_4260:
    mov r0, r4                              /* @4260 2046 mov r0, r4  */
L_4262:
    pop {r4, pc}                            /* @4262 10bd pop {r4, pc}  */
L_4264:
    ldrb r0, [r0, #8]                       /* @4264 007a ldrb r0, [r0, #8]  */
L_4266:
    pop {r4, pc}                            /* @4266 10bd pop {r4, pc}  */
L_4268:
    .byte 0x2c,0x07,0x00,0x20 /* @4268 literal */
L_426c:
    .byte 0xf4,0x06,0x00,0x20 /* @426C literal */
    .byte 0x00,0x00,0x00,0x00 /* @4270 unclassified */

/* menu_exit */
L_4274:
    ldr r0, [pc, #8]                        /* @4274 0248 ldr r0, [pc, #8] literal 0x4280 -> 0x20000628 */
L_4276:
    movs r1, #0                             /* @4276 0021 movs r1, #0  */
L_4278:
    strb r1, [r0]                           /* @4278 0170 strb r1, [r0]  */
L_427a:
    strb r1, [r0, #2]                       /* @427A 8170 strb r1, [r0, #2]  */
L_427c:
    bx lr                                   /* @427C 7047 bx lr  */
    .byte 0x00,0x00 /* @427E unclassified */
L_4280:
    .byte 0x28,0x06,0x00,0x20 /* @4280 literal */
    .byte 0x00,0x00,0x00,0x00 /* @4284 unclassified */

/* menu_mplyer */
L_4288:
    push {r4, lr}                           /* @4288 10b5 push {r4, lr}  */
L_428a:
    mov r4, r0                              /* @428A 0446 mov r4, r0  */
L_428c:
    ldr r0, [pc, #40]                       /* @428C 0a48 ldr r0, [pc, #40] literal 0x42B8 -> 0x2000072C */
L_428e:
    cmp r4, #255                            /* @428E ff2c cmp r4, #255  */
L_4290:
    beq L_42a8                              /* @4290 0ad0 beq #20 -> 0x42A8 */
L_4292:
    ldrb r0, [r0, #12]                      /* @4292 007b ldrb r0, [r0, #12]  */
L_4294:
    cmp r0, r4                              /* @4294 a042 cmp r0, r4  */
L_4296:
    beq L_42a4                              /* @4296 05d0 beq #10 -> 0x42A4 */
L_4298:
    bl L_52cc                               /* @4298 01f018f8 bl #4144 -> sub_52CC */
L_429c:
    ldr r0, [pc, #28]                       /* @429C 0748 ldr r0, [pc, #28] literal 0x42BC -> 0x200006F4 */
L_429e:
    strb r4, [r0, #12]                      /* @429E 0473 strb r4, [r0, #12]  */
L_42a0:
    bl L_470c                               /* @42A0 00f034fa bl #1128 -> sub_470C */
L_42a4:
    mov r0, r4                              /* @42A4 2046 mov r0, r4  */
L_42a6:
    pop {r4, pc}                            /* @42A6 10bd pop {r4, pc}  */
L_42a8:
    ldrb r0, [r0, #12]                      /* @42A8 007b ldrb r0, [r0, #12]  */
L_42aa:
    cmp r0, #0                              /* @42AA 0028 cmp r0, #0  */
L_42ac:
    beq L_42b2                              /* @42AC 01d0 beq #2 -> 0x42B2 */
L_42ae:
    movs r0, #1                             /* @42AE 0120 movs r0, #1  */
L_42b0:
    pop {r4, pc}                            /* @42B0 10bd pop {r4, pc}  */
L_42b2:
    movs r0, #0                             /* @42B2 0020 movs r0, #0  */
L_42b4:
    pop {r4, pc}                            /* @42B4 10bd pop {r4, pc}  */
    .byte 0x00,0x00 /* @42B6 unclassified */
L_42b8:
    .byte 0x2c,0x07,0x00,0x20 /* @42B8 literal */
L_42bc:
    .byte 0xf4,0x06,0x00,0x20 /* @42BC literal */

/* menu_mstep */
L_42c0:
    push {r4, r5, r6, lr}                   /* @42C0 70b5 push {r4, r5, r6, lr}  */
L_42c2:
    mov r4, r0                              /* @42C2 0446 mov r4, r0  */
L_42c4:
    ldr r5, [pc, #144]                      /* @42C4 244d ldr r5, [pc, #144] literal 0x4358 -> 0x2000072C */
L_42c6:
    cmp r4, #255                            /* @42C6 ff2c cmp r4, #255  */
L_42c8:
    beq L_4304                              /* @42C8 1cd0 beq #56 -> 0x4304 */
L_42ca:
    ldr r6, [pc, #144]                      /* @42CA 244e ldr r6, [pc, #144] literal 0x435C -> 0x200006F4 */
L_42cc:
    cmp r4, #9                              /* @42CC 092c cmp r4, #9  */
L_42ce:
    beq L_42e6                              /* @42CE 0ad0 beq #20 -> 0x42E6 */
L_42d0:
    ldrh r0, [r5]                           /* @42D0 2888 ldrh r0, [r5]  */
L_42d2:
    movs r5, #1                             /* @42D2 0125 movs r5, #1  */
L_42d4:
    lsls r5, r4                             /* @42D4 a540 lsls r5, r4  */
L_42d6:
    cmp r0, r5                              /* @42D6 a842 cmp r0, r5  */
L_42d8:
    beq L_4300                              /* @42D8 12d0 beq #36 -> 0x4300 */
L_42da:
    bl L_52cc                               /* @42DA 00f0f7ff bl #4078 -> sub_52CC */
L_42de:
    strh r5, [r6]                           /* @42DE 3580 strh r5, [r6]  */
L_42e0:
    bl L_470c                               /* @42E0 00f014fa bl #1064 -> sub_470C */
L_42e4:
    b L_4300                                /* @42E4 0ce0 b #24 -> 0x4300 */
L_42e6:
    ldrh r0, [r5]                           /* @42E6 2888 ldrh r0, [r5]  */
L_42e8:
    ldrh r1, [r5, #2]                       /* @42E8 6988 ldrh r1, [r5, #2]  */
L_42ea:
    cmp r0, r1                              /* @42EA 8842 cmp r0, r1  */
L_42ec:
    beq L_4300                              /* @42EC 08d0 beq #16 -> 0x4300 */
L_42ee:
    ldrh r0, [r5, #2]                       /* @42EE 6888 ldrh r0, [r5, #2]  */
L_42f0:
    cmp r0, #0                              /* @42F0 0028 cmp r0, #0  */
L_42f2:
    beq L_4300                              /* @42F2 05d0 beq #10 -> 0x4300 */
L_42f4:
    bl L_52cc                               /* @42F4 00f0eaff bl #4052 -> sub_52CC */
L_42f8:
    ldrh r0, [r5, #2]                       /* @42F8 6888 ldrh r0, [r5, #2]  */
L_42fa:
    strh r0, [r6]                           /* @42FA 3080 strh r0, [r6]  */
L_42fc:
    bl L_470c                               /* @42FC 00f006fa bl #1036 -> sub_470C */
L_4300:
    mov r0, r4                              /* @4300 2046 mov r0, r4  */
L_4302:
    pop {r4, r5, r6, pc}                    /* @4302 70bd pop {r4, r5, r6, pc}  */
L_4304:
    ldrh r0, [r5]                           /* @4304 2888 ldrh r0, [r5]  */
L_4306:
    cmp r0, #16                             /* @4306 1028 cmp r0, #16  */
L_4308:
    beq L_4344                              /* @4308 1cd0 beq #56 -> 0x4344 */
L_430a:
    bgt L_431e                              /* @430A 08dc bgt #16 -> 0x431E */
L_430c:
    cmp r0, #1                              /* @430C 0128 cmp r0, #1  */
L_430e:
    beq L_4334                              /* @430E 11d0 beq #34 -> 0x4334 */
L_4310:
    cmp r0, #2                              /* @4310 0228 cmp r0, #2  */
L_4312:
    beq L_4338                              /* @4312 11d0 beq #34 -> 0x4338 */
L_4314:
    cmp r0, #4                              /* @4314 0428 cmp r0, #4  */
L_4316:
    beq L_433c                              /* @4316 11d0 beq #34 -> 0x433C */
L_4318:
    cmp r0, #8                              /* @4318 0828 cmp r0, #8  */
L_431a:
    bne L_4330                              /* @431A 09d1 bne #18 -> 0x4330 */
L_431c:
    b L_4340                                /* @431C 10e0 b #32 -> 0x4340 */
L_431e:
    cmp r0, #32                             /* @431E 2028 cmp r0, #32  */
L_4320:
    beq L_4348                              /* @4320 12d0 beq #36 -> 0x4348 */
L_4322:
    cmp r0, #64                             /* @4322 4028 cmp r0, #64  */
L_4324:
    beq L_434c                              /* @4324 12d0 beq #36 -> 0x434C */
L_4326:
    cmp r0, #128                            /* @4326 8028 cmp r0, #128  */
L_4328:
    beq L_4350                              /* @4328 12d0 beq #36 -> 0x4350 */
L_432a:
    subs r0, #255                           /* @432A ff38 subs r0, #255  */
L_432c:
    subs r0, #1                             /* @432C 0138 subs r0, #1  */
L_432e:
    beq L_4354                              /* @432E 11d0 beq #34 -> 0x4354 */
L_4330:
    movs r0, #9                             /* @4330 0920 movs r0, #9  */
L_4332:
    pop {r4, r5, r6, pc}                    /* @4332 70bd pop {r4, r5, r6, pc}  */
L_4334:
    movs r0, #0                             /* @4334 0020 movs r0, #0  */
L_4336:
    pop {r4, r5, r6, pc}                    /* @4336 70bd pop {r4, r5, r6, pc}  */
L_4338:
    movs r0, #1                             /* @4338 0120 movs r0, #1  */
L_433a:
    pop {r4, r5, r6, pc}                    /* @433A 70bd pop {r4, r5, r6, pc}  */
L_433c:
    movs r0, #2                             /* @433C 0220 movs r0, #2  */
L_433e:
    pop {r4, r5, r6, pc}                    /* @433E 70bd pop {r4, r5, r6, pc}  */
L_4340:
    movs r0, #3                             /* @4340 0320 movs r0, #3  */
L_4342:
    pop {r4, r5, r6, pc}                    /* @4342 70bd pop {r4, r5, r6, pc}  */
L_4344:
    movs r0, #4                             /* @4344 0420 movs r0, #4  */
L_4346:
    pop {r4, r5, r6, pc}                    /* @4346 70bd pop {r4, r5, r6, pc}  */
L_4348:
    movs r0, #5                             /* @4348 0520 movs r0, #5  */
L_434a:
    pop {r4, r5, r6, pc}                    /* @434A 70bd pop {r4, r5, r6, pc}  */
L_434c:
    movs r0, #6                             /* @434C 0620 movs r0, #6  */
L_434e:
    pop {r4, r5, r6, pc}                    /* @434E 70bd pop {r4, r5, r6, pc}  */
L_4350:
    movs r0, #7                             /* @4350 0720 movs r0, #7  */
L_4352:
    pop {r4, r5, r6, pc}                    /* @4352 70bd pop {r4, r5, r6, pc}  */
L_4354:
    movs r0, #8                             /* @4354 0820 movs r0, #8  */
L_4356:
    pop {r4, r5, r6, pc}                    /* @4356 70bd pop {r4, r5, r6, pc}  */
L_4358:
    .byte 0x2c,0x07,0x00,0x20 /* @4358 literal */
L_435c:
    .byte 0xf4,0x06,0x00,0x20 /* @435C literal */

/* menu_mottype */
L_4360:
    push {r4, r5, r6, lr}                   /* @4360 70b5 push {r4, r5, r6, lr}  */
L_4362:
    ldr r5, [pc, #48]                       /* @4362 0c4d ldr r5, [pc, #48] literal 0x4394 -> 0x200005E4 */
L_4364:
    cmp r0, #255                            /* @4364 ff28 cmp r0, #255  */
L_4366:
    beq L_4384                              /* @4366 0dd0 beq #26 -> 0x4384 */
L_4368:
    cmp r0, #0                              /* @4368 0028 cmp r0, #0  */
L_436a:
    beq L_4370                              /* @436A 01d0 beq #2 -> 0x4370 */
L_436c:
    movs r4, #25                            /* @436C 1924 movs r4, #25  */
L_436e:
    b L_4372                                /* @436E 00e0 b #0 -> 0x4372 */
L_4370:
    movs r4, #50                            /* @4370 3224 movs r4, #50  */
L_4372:
    ldrh r0, [r5, #4]                       /* @4372 a888 ldrh r0, [r5, #4]  */
L_4374:
    cmp r0, r4                              /* @4374 a042 cmp r0, r4  */
L_4376:
    beq L_4384                              /* @4376 05d0 beq #10 -> 0x4384 */
L_4378:
    bl L_52cc                               /* @4378 00f0a8ff bl #3920 -> sub_52CC */
L_437c:
    ldr r0, [pc, #24]                       /* @437C 0648 ldr r0, [pc, #24] literal 0x4398 -> 0x200006F4 */
L_437e:
    strh r4, [r0, #32]                      /* @437E 0484 strh r4, [r0, #32]  */
L_4380:
    bl L_470c                               /* @4380 00f0c4f9 bl #904 -> sub_470C */
L_4384:
    ldrh r0, [r5, #4]                       /* @4384 a888 ldrh r0, [r5, #4]  */
L_4386:
    cmp r0, #50                             /* @4386 3228 cmp r0, #50  */
L_4388:
    beq L_438e                              /* @4388 01d0 beq #2 -> 0x438E */
L_438a:
    movs r0, #1                             /* @438A 0120 movs r0, #1  */
L_438c:
    pop {r4, r5, r6, pc}                    /* @438C 70bd pop {r4, r5, r6, pc}  */
L_438e:
    movs r0, #0                             /* @438E 0020 movs r0, #0  */
L_4390:
    pop {r4, r5, r6, pc}                    /* @4390 70bd pop {r4, r5, r6, pc}  */
    .byte 0x00,0x00 /* @4392 unclassified */
L_4394:
    .byte 0xe4,0x05,0x00,0x20 /* @4394 literal */
L_4398:
    .byte 0xf4,0x06,0x00,0x20 /* @4398 literal */

/* menu_uartaddr */
L_439c:
    push {r4, r5, r6, lr}                   /* @439C 70b5 push {r4, r5, r6, lr}  */
L_439e:
    mov r5, r0                              /* @439E 0546 mov r5, r0  */
L_43a0:
    ldr r0, [pc, #36]                       /* @43A0 0948 ldr r0, [pc, #36] literal 0x43C8 -> 0x2000072C */
L_43a2:
    cmp r5, #255                            /* @43A2 ff2d cmp r5, #255  */
L_43a4:
    beq L_43c0                              /* @43A4 0cd0 beq #24 -> 0x43C0 */
L_43a6:
    ldrh r0, [r0, #20]                      /* @43A6 808a ldrh r0, [r0, #20]  */
L_43a8:
    mov r4, r5                              /* @43A8 2c46 mov r4, r5  */
L_43aa:
    adds r4, #224                           /* @43AA e034 adds r4, #224  */
L_43ac:
    cmp r0, r4                              /* @43AC a042 cmp r0, r4  */
L_43ae:
    beq L_43bc                              /* @43AE 05d0 beq #10 -> 0x43BC */
L_43b0:
    bl L_52cc                               /* @43B0 00f08cff bl #3864 -> sub_52CC */
L_43b4:
    ldr r0, [pc, #20]                       /* @43B4 0548 ldr r0, [pc, #20] literal 0x43CC -> 0x200006F4 */
L_43b6:
    strh r4, [r0, #20]                      /* @43B6 8482 strh r4, [r0, #20]  */
L_43b8:
    bl L_470c                               /* @43B8 00f0a8f9 bl #848 -> sub_470C */
L_43bc:
    mov r0, r5                              /* @43BC 2846 mov r0, r5  */
L_43be:
    pop {r4, r5, r6, pc}                    /* @43BE 70bd pop {r4, r5, r6, pc}  */
L_43c0:
    ldrh r0, [r0, #20]                      /* @43C0 808a ldrh r0, [r0, #20]  */
L_43c2:
    subs r0, #224                           /* @43C2 e038 subs r0, #224  */
L_43c4:
    pop {r4, r5, r6, pc}                    /* @43C4 70bd pop {r4, r5, r6, pc}  */
    .byte 0x00,0x00 /* @43C6 unclassified */
L_43c8:
    .byte 0x2c,0x07,0x00,0x20 /* @43C8 literal */
L_43cc:
    .byte 0xf4,0x06,0x00,0x20 /* @43CC literal */

/* menu_0_dir */
L_43d0:
    push {r4, lr}                           /* @43D0 10b5 push {r4, lr}  */
L_43d2:
    mov r4, r0                              /* @43D2 0446 mov r4, r0  */
L_43d4:
    ldr r0, [pc, #28]                       /* @43D4 0748 ldr r0, [pc, #28] literal 0x43F4 -> 0x200005F6 */
L_43d6:
    cmp r4, #255                            /* @43D6 ff2c cmp r4, #255  */
L_43d8:
    beq L_43f0                              /* @43D8 0ad0 beq #20 -> 0x43F0 */
L_43da:
    ldrb r0, [r0, #6]                       /* @43DA 8079 ldrb r0, [r0, #6]  */
L_43dc:
    cmp r0, r4                              /* @43DC a042 cmp r0, r4  */
L_43de:
    beq L_43ec                              /* @43DE 05d0 beq #10 -> 0x43EC */
L_43e0:
    bl L_52cc                               /* @43E0 00f074ff bl #3816 -> sub_52CC */
L_43e4:
    ldr r0, [pc, #16]                       /* @43E4 0448 ldr r0, [pc, #16] literal 0x43F8 -> 0x20000714 */
L_43e6:
    strb r4, [r0, #8]                       /* @43E6 0472 strb r4, [r0, #8]  */
L_43e8:
    bl L_470c                               /* @43E8 00f090f9 bl #800 -> sub_470C */
L_43ec:
    mov r0, r4                              /* @43EC 2046 mov r0, r4  */
L_43ee:
    pop {r4, pc}                            /* @43EE 10bd pop {r4, pc}  */
L_43f0:
    ldrb r0, [r0, #6]                       /* @43F0 8079 ldrb r0, [r0, #6]  */
L_43f2:
    pop {r4, pc}                            /* @43F2 10bd pop {r4, pc}  */
L_43f4:
    .byte 0xf6,0x05,0x00,0x20 /* @43F4 literal */
L_43f8:
    .byte 0x14,0x07,0x00,0x20 /* @43F8 literal */
    .byte 0x00,0x00,0x00,0x00 /* @43FC unclassified */

/* menu_0_speed */
L_4400:
    push {r4, lr}                           /* @4400 10b5 push {r4, lr}  */
L_4402:
    mov r4, r0                              /* @4402 0446 mov r4, r0  */
L_4404:
    ldr r0, [pc, #28]                       /* @4404 0748 ldr r0, [pc, #28] literal 0x4424 -> 0x200005F6 */
L_4406:
    cmp r4, #255                            /* @4406 ff2c cmp r4, #255  */
L_4408:
    beq L_4420                              /* @4408 0ad0 beq #20 -> 0x4420 */
L_440a:
    ldrh r0, [r0, #4]                       /* @440A 8088 ldrh r0, [r0, #4]  */
L_440c:
    cmp r0, r4                              /* @440C a042 cmp r0, r4  */
L_440e:
    beq L_441c                              /* @440E 05d0 beq #10 -> 0x441C */
L_4410:
    bl L_52cc                               /* @4410 00f05cff bl #3768 -> sub_52CC */
L_4414:
    ldr r0, [pc, #16]                       /* @4414 0448 ldr r0, [pc, #16] literal 0x4428 -> 0x200006F4 */
L_4416:
    strh r4, [r0, #38]                      /* @4416 c484 strh r4, [r0, #38]  */
L_4418:
    bl L_470c                               /* @4418 00f078f9 bl #752 -> sub_470C */
L_441c:
    mov r0, r4                              /* @441C 2046 mov r0, r4  */
L_441e:
    pop {r4, pc}                            /* @441E 10bd pop {r4, pc}  */
L_4420:
    ldrh r0, [r0, #4]                       /* @4420 8088 ldrh r0, [r0, #4]  */
L_4422:
    pop {r4, pc}                            /* @4422 10bd pop {r4, pc}  */
L_4424:
    .byte 0xf6,0x05,0x00,0x20 /* @4424 literal */
L_4428:
    .byte 0xf4,0x06,0x00,0x20 /* @4428 literal */

/* menu_0_mode */
L_442c:
    push {r4, lr}                           /* @442C 10b5 push {r4, lr}  */
L_442e:
    mov r4, r0                              /* @442E 0446 mov r4, r0  */
L_4430:
    ldr r0, [pc, #40]                       /* @4430 0a48 ldr r0, [pc, #40] literal 0x445C -> 0x200005F6 */
L_4432:
    cmp r4, #255                            /* @4432 ff2c cmp r4, #255  */
L_4434:
    beq L_4458                              /* @4434 10d0 beq #32 -> 0x4458 */
L_4436:
    ldrb r0, [r0]                           /* @4436 0078 ldrb r0, [r0]  */
L_4438:
    cmp r0, r4                              /* @4438 a042 cmp r0, r4  */
L_443a:
    beq L_4454                              /* @443A 0bd0 beq #22 -> 0x4454 */
L_443c:
    bl L_52cc                               /* @443C 00f046ff bl #3724 -> sub_52CC */
L_4440:
    cmp r4, #0                              /* @4440 002c cmp r4, #0  */
L_4442:
    bne L_444a                              /* @4442 02d1 bne #4 -> 0x444A */
L_4444:
    ldr r1, [pc, #24]                       /* @4444 0649 ldr r1, [pc, #24] literal 0x4460 -> 0x200006F4 */
L_4446:
    subs r0, r4, #1                         /* @4446 601e subs r0, r4, #1  */
L_4448:
    strh r0, [r1, #36]                      /* @4448 8884 strh r0, [r1, #36]  */
L_444a:
    ldr r0, [pc, #20]                       /* @444A 0548 ldr r0, [pc, #20] literal 0x4460 -> 0x200006F4 */
L_444c:
    adds r0, #32                            /* @444C 2030 adds r0, #32  */
L_444e:
    strb r4, [r0, #2]                       /* @444E 8470 strb r4, [r0, #2]  */
L_4450:
    bl L_470c                               /* @4450 00f05cf9 bl #696 -> sub_470C */
L_4454:
    mov r0, r4                              /* @4454 2046 mov r0, r4  */
L_4456:
    pop {r4, pc}                            /* @4456 10bd pop {r4, pc}  */
L_4458:
    ldrb r0, [r0]                           /* @4458 0078 ldrb r0, [r0]  */
L_445a:
    pop {r4, pc}                            /* @445A 10bd pop {r4, pc}  */
L_445c:
    .byte 0xf6,0x05,0x00,0x20 /* @445C literal */
L_4460:
    .byte 0xf4,0x06,0x00,0x20 /* @4460 literal */

/* menu_protect */
L_4464:
    push {r4, lr}                           /* @4464 10b5 push {r4, lr}  */
L_4466:
    mov r4, r0                              /* @4466 0446 mov r4, r0  */
L_4468:
    ldr r0, [pc, #44]                       /* @4468 0b48 ldr r0, [pc, #44] literal 0x4498 -> 0x2000072C */
L_446a:
    cmp r4, #255                            /* @446A ff2c cmp r4, #255  */
L_446c:
    beq L_448a                              /* @446C 0dd0 beq #26 -> 0x448A */
L_446e:
    ldrb r0, [r0, #11]                      /* @446E c07a ldrb r0, [r0, #11]  */
L_4470:
    cmp r0, r4                              /* @4470 a042 cmp r0, r4  */
L_4472:
    beq L_4486                              /* @4472 08d0 beq #16 -> 0x4486 */
L_4474:
    bl L_52cc                               /* @4474 00f02aff bl #3668 -> sub_52CC */
L_4478:
    ldr r1, [pc, #32]                       /* @4478 0849 ldr r1, [pc, #32] literal 0x449C -> 0x200005D9 */
L_447a:
    movs r0, #4                             /* @447A 0420 movs r0, #4  */
L_447c:
    strb r0, [r1]                           /* @447C 0870 strb r0, [r1]  */
L_447e:
    ldr r0, [pc, #32]                       /* @447E 0848 ldr r0, [pc, #32] literal 0x44A0 -> 0x200006F4 */
L_4480:
    strb r4, [r0, #11]                      /* @4480 c472 strb r4, [r0, #11]  */
L_4482:
    bl L_470c                               /* @4482 00f043f9 bl #646 -> sub_470C */
L_4486:
    mov r0, r4                              /* @4486 2046 mov r0, r4  */
L_4488:
    pop {r4, pc}                            /* @4488 10bd pop {r4, pc}  */
L_448a:
    ldrb r0, [r0, #11]                      /* @448A c07a ldrb r0, [r0, #11]  */
L_448c:
    cmp r0, #0                              /* @448C 0028 cmp r0, #0  */
L_448e:
    beq L_4494                              /* @448E 01d0 beq #2 -> 0x4494 */
L_4490:
    movs r0, #1                             /* @4490 0120 movs r0, #1  */
L_4492:
    pop {r4, pc}                            /* @4492 10bd pop {r4, pc}  */
L_4494:
    movs r0, #0                             /* @4494 0020 movs r0, #0  */
L_4496:
    pop {r4, pc}                            /* @4496 10bd pop {r4, pc}  */
L_4498:
    .byte 0x2c,0x07,0x00,0x20 /* @4498 literal */
L_449c:
    .byte 0xd9,0x05,0x00,0x20 /* @449C literal */
L_44a0:
    .byte 0xf4,0x06,0x00,0x20 /* @44A0 literal */
    .byte 0x00,0x00,0x00,0x00 /* @44A4 unclassified */

/* menu_set_0 */
L_44a8:
    push {r4, lr}                           /* @44A8 10b5 push {r4, lr}  */
L_44aa:
    ldr r0, [pc, #104]                      /* @44AA 1a48 ldr r0, [pc, #104] literal 0x4514 -> 0x2000072C */
L_44ac:
    ldrb r0, [r0, #9]                       /* @44AC 407a ldrb r0, [r0, #9]  */
L_44ae:
    movs r4, #75                            /* @44AE 4b24 movs r4, #75  */
L_44b0:
    lsls r4, r4, #4                         /* @44B0 2401 lsls r4, r4, #4  */
L_44b2:
    cmp r0, #0                              /* @44B2 0028 cmp r0, #0  */
L_44b4:
    beq L_44fe                              /* @44B4 23d0 beq #70 -> 0x44FE */
L_44b6:
    ldr r0, [pc, #96]                       /* @44B6 1848 ldr r0, [pc, #96] literal 0x4518 -> 0x200005F6 */
L_44b8:
    ldrb r0, [r0]                           /* @44B8 0078 ldrb r0, [r0]  */
L_44ba:
    cmp r0, #0                              /* @44BA 0028 cmp r0, #0  */
L_44bc:
    beq L_44ec                              /* @44BC 16d0 beq #44 -> 0x44EC */
L_44be:
    bl L_5874                               /* @44BE 01f0d9f9 bl #5042 -> sub_5874 */
L_44c2:
    bl L_52cc                               /* @44C2 00f003ff bl #3590 -> sub_52CC */
L_44c6:
    movs r0, #3                             /* @44C6 0320 movs r0, #3  */
L_44c8:
    bl L_5a38                               /* @44C8 01f0b6fa bl #5484 -> sub_5A38 */
L_44cc:
    bl L_26cc                               /* @44CC fef7fef8 bl #-7684 -> encoder_lut_read */
L_44d0:
    ldr r1, [pc, #72]                       /* @44D0 1249 ldr r1, [pc, #72] literal 0x451C -> 0x200006F4 */
L_44d2:
    strh r0, [r1, #36]                      /* @44D2 8884 strh r0, [r1, #36]  */
L_44d4:
    bl L_470c                               /* @44D4 00f01af9 bl #564 -> sub_470C */
L_44d8:
    adr r2, L_4520                          /* @44D8 11a2 adr r2, #68 address 0x4520 -> 0x00004520 'Done!' */
L_44da:
    adr r1, L_4528                          /* @44DA 13a1 adr r1, #76 address 0x4528 -> 0x00004528 ' Set' */
L_44dc:
    adr r0, L_4530                          /* @44DC 14a0 adr r0, #80 address 0x4530 -> 0x00004530 'Origin' */
L_44de:
    bl L_4884                               /* @44DE 00f0d1f9 bl #930 -> display_three_lines */
L_44e2:
    mov r0, r4                              /* @44E2 2046 mov r0, r4  */
L_44e4:
    bl L_2de8                               /* @44E4 fef780fc bl #-5888 -> delay_ms_candidate */
L_44e8:
    movs r0, #1                             /* @44E8 0120 movs r0, #1  */
L_44ea:
    pop {r4, pc}                            /* @44EA 10bd pop {r4, pc}  */
L_44ec:
    adr r2, L_4538                          /* @44EC 12a2 adr r2, #72 address 0x4538 -> 0x00004538 'Disable' */
L_44ee:
    adr r1, L_4540                          /* @44EE 14a1 adr r1, #80 address 0x4540 -> 0x00004540 ' is' */
L_44f0:
    adr r0, L_4544                          /* @44F0 14a0 adr r0, #80 address 0x4544 -> 0x00004544 '0_Mode' */
L_44f2:
    bl L_4884                               /* @44F2 00f0c7f9 bl #910 -> display_three_lines */
L_44f6:
    mov r0, r4                              /* @44F6 2046 mov r0, r4  */
L_44f8:
    bl L_2de8                               /* @44F8 fef776fc bl #-5908 -> delay_ms_candidate */
L_44fc:
    b L_450e                                /* @44FC 07e0 b #14 -> 0x450E */
L_44fe:
    adr r2, L_454c                          /* @44FE 13a2 adr r2, #76 address 0x454C -> 0x0000454C 'Mode' */
L_4500:
    adr r1, L_4554                          /* @4500 14a1 adr r1, #80 address 0x4554 -> 0x00004554 ' CR_OPEN' */
L_4502:
    adr r0, L_4560                          /* @4502 17a0 adr r0, #92 address 0x4560 -> 0x00004560 'It is' */
L_4504:
    bl L_4884                               /* @4504 00f0bef9 bl #892 -> display_three_lines */
L_4508:
    mov r0, r4                              /* @4508 2046 mov r0, r4  */
L_450a:
    bl L_2de8                               /* @450A fef76dfc bl #-5926 -> delay_ms_candidate */
L_450e:
    movs r0, #0                             /* @450E 0020 movs r0, #0  */
L_4510:
    pop {r4, pc}                            /* @4510 10bd pop {r4, pc}  */
    .byte 0x00,0x00 /* @4512 unclassified */
L_4514:
    .byte 0x2c,0x07,0x00,0x20 /* @4514 literal */
L_4518:
    .byte 0xf6,0x05,0x00,0x20 /* @4518 literal */
L_451c:
    .byte 0xf4,0x06,0x00,0x20 /* @451C literal */
L_4520:
    .byte 0x44,0x6f,0x6e,0x65,0x21,0x00,0x00,0x00 /* @4520 unclassified */
L_4528:
    .byte 0x20,0x53,0x65,0x74,0x00,0x00,0x00,0x00 /* @4528 unclassified */
L_4530:
    .byte 0x4f,0x72,0x69,0x67,0x69,0x6e,0x00,0x00 /* @4530 unclassified */
L_4538:
    .byte 0x44,0x69,0x73,0x61,0x62,0x6c,0x65,0x00 /* @4538 unclassified */
L_4540:
    .byte 0x20,0x69,0x73,0x00 /* @4540 unclassified */
L_4544:
    .byte 0x30,0x5f,0x4d,0x6f,0x64,0x65,0x00,0x00 /* @4544 unclassified */
L_454c:
    .byte 0x4d,0x6f,0x64,0x65,0x00,0x00,0x00,0x00 /* @454C unclassified */
L_4554:
    .byte 0x20,0x43,0x52,0x5f,0x4f,0x50,0x45,0x4e,0x00,0x00,0x00,0x00 /* @4554 unclassified */
L_4560:
    .byte 0x49,0x74,0x20,0x69,0x73,0x00,0x00,0x00 /* @4560 unclassified */

/* sub_4568 */
L_4568:
    ldr r1, [pc, #4]                        /* @4568 0149 ldr r1, [pc, #4] literal 0x4570 -> 0x20000628 */
L_456a:
    movs r0, #1                             /* @456A 0120 movs r0, #1  */
L_456c:
    strb r0, [r1, #2]                       /* @456C 8870 strb r0, [r1, #2]  */
L_456e:
    bx lr                                   /* @456E 7047 bx lr  */
L_4570:
    .byte 0x28,0x06,0x00,0x20 /* @4570 literal */

/* sub_4574 */
L_4574:
    ldr r1, [pc, #8]                        /* @4574 0249 ldr r1, [pc, #8] literal 0x4580 -> 0x20000628 */
L_4576:
    str r0, [r1, #4]                        /* @4576 4860 str r0, [r1, #4]  */
L_4578:
    movs r0, #0                             /* @4578 0020 movs r0, #0  */
L_457a:
    strb r0, [r1]                           /* @457A 0870 strb r0, [r1]  */
L_457c:
    bx lr                                   /* @457C 7047 bx lr  */
    .byte 0x00,0x00 /* @457E unclassified */
L_4580:
    .byte 0x28,0x06,0x00,0x20 /* @4580 literal */

/* sub_4584 */
L_4584:
    push {r4, r5, r6, r7, lr}               /* @4584 f0b5 push {r4, r5, r6, r7, lr}  */
L_4586:
    sub sp, #36                             /* @4586 89b0 sub sp, #36  */
L_4588:
    movs r1, #32                            /* @4588 2021 movs r1, #32  */
L_458a:
    add r0, sp, #4                          /* @458A 01a8 add r0, sp, #4  */
L_458c:
    bl L_025c                               /* @458C fbf766fe bl #-17204 -> sub_025C */
L_4590:
    movs r4, #0                             /* @4590 0024 movs r4, #0  */
L_4592:
    ldr r6, [pc, #200]                      /* @4592 324e ldr r6, [pc, #200] literal 0x465C -> 0x20000628 */
L_4594:
    movs r0, #32                            /* @4594 2020 movs r0, #32  */
L_4596:
    ldr r1, [r6, #8]                        /* @4596 b168 ldr r1, [r6, #8]  */
L_4598:
    add r7, sp, #4                          /* @4598 01af add r7, sp, #4  */
L_459a:
    cmp r1, #0                              /* @459A 0029 cmp r1, #0  */
L_459c:
    beq L_462c                              /* @459C 46d0 beq #140 -> 0x462C */
L_459e:
    ldrb r1, [r6, #1]                       /* @459E 7178 ldrb r1, [r6, #1]  */
L_45a0:
    cmp r1, #3                              /* @45A0 0329 cmp r1, #3  */
L_45a2:
    blo L_45c0                              /* @45A2 0dd3 blo #26 -> 0x45C0 */
L_45a4:
    b L_45e0                                /* @45A4 1ce0 b #56 -> 0x45E0 */
L_45a6:
    movs r1, #32                            /* @45A6 2021 movs r1, #32  */
L_45a8:
    mov r0, r4                              /* @45A8 2046 mov r0, r4  */
L_45aa:
    movs r2, #10                            /* @45AA 0a22 movs r2, #10  */
L_45ac:
    muls r0, r2, r0                         /* @45AC 5043 muls r0, r2, r0  */
L_45ae:
    strb r1, [r7, r0]                       /* @45AE 3954 strb r1, [r7, r0]  */
L_45b0:
    ldr r1, [r6, #8]                        /* @45B0 b168 ldr r1, [r6, #8]  */
L_45b2:
    adds r0, r0, r7                         /* @45B2 c019 adds r0, r0, r7  */
L_45b4:
    adds r1, r1, r5                         /* @45B4 4919 adds r1, r1, r5  */
L_45b6:
    adds r0, r0, #1                         /* @45B6 401c adds r0, r0, #1  */
L_45b8:
    bl L_0280                               /* @45B8 fbf762fe bl #-17212 -> sub_0280 */
L_45bc:
    adds r4, r4, #1                         /* @45BC 641c adds r4, r4, #1  */
L_45be:
    uxtb r4, r4                             /* @45BE e4b2 uxtb r4, r4  */
L_45c0:
    lsls r1, r4, #3                         /* @45C0 e100 lsls r1, r4, #3  */
L_45c2:
    ldr r0, [r6, #8]                        /* @45C2 b068 ldr r0, [r6, #8]  */
L_45c4:
    adds r5, r4, r1                         /* @45C4 6518 adds r5, r4, r1  */
L_45c6:
    adds r0, r0, r5                         /* @45C6 4019 adds r0, r0, r5  */
L_45c8:
    bl L_0272                               /* @45C8 fbf753fe bl #-17242 -> sub_0272 */
L_45cc:
    cmp r0, #0                              /* @45CC 0028 cmp r0, #0  */
L_45ce:
    beq L_45d4                              /* @45CE 01d0 beq #2 -> 0x45D4 */
L_45d0:
    cmp r4, #3                              /* @45D0 032c cmp r4, #3  */
L_45d2:
    blo L_45a6                              /* @45D2 e8d3 blo #-48 -> 0x45A6 */
L_45d4:
    ldrb r1, [r6, #1]                       /* @45D4 7178 ldrb r1, [r6, #1]  */
L_45d6:
    movs r0, #42                            /* @45D6 2a20 movs r0, #42  */
L_45d8:
    movs r2, #10                            /* @45D8 0a22 movs r2, #10  */
L_45da:
    muls r1, r2, r1                         /* @45DA 5143 muls r1, r2, r1  */
L_45dc:
    strb r0, [r7, r1]                       /* @45DC 7854 strb r0, [r7, r1]  */
L_45de:
    b L_464a                                /* @45DE 34e0 b #104 -> 0x464A */
L_45e0:
    mov r5, r0                              /* @45E0 0546 mov r5, r0  */
L_45e2:
    b L_4608                                /* @45E2 11e0 b #34 -> 0x4608 */
L_45e4:
    mov r0, r4                              /* @45E4 2046 mov r0, r4  */
L_45e6:
    movs r2, #10                            /* @45E6 0a22 movs r2, #10  */
L_45e8:
    muls r0, r2, r0                         /* @45E8 5043 muls r0, r2, r0  */
L_45ea:
    mov r2, r0                              /* @45EA 0246 mov r2, r0  */
L_45ec:
    strb r5, [r7, r0]                       /* @45EC 3d54 strb r5, [r7, r0]  */
L_45ee:
    ldrb r0, [r6, #1]                       /* @45EE 7078 ldrb r0, [r6, #1]  */
L_45f0:
    subs r3, r4, #2                         /* @45F0 a31e subs r3, r4, #2  */
L_45f2:
    adds r0, r0, r3                         /* @45F2 c018 adds r0, r0, r3  */
L_45f4:
    lsls r3, r0, #3                         /* @45F4 c300 lsls r3, r0, #3  */
L_45f6:
    ldr r1, [r6, #8]                        /* @45F6 b168 ldr r1, [r6, #8]  */
L_45f8:
    adds r0, r0, r3                         /* @45F8 c018 adds r0, r0, r3  */
L_45fa:
    adds r1, r1, r0                         /* @45FA 0918 adds r1, r1, r0  */
L_45fc:
    adds r0, r2, r7                         /* @45FC d019 adds r0, r2, r7  */
L_45fe:
    adds r0, r0, #1                         /* @45FE 401c adds r0, r0, #1  */
L_4600:
    bl L_0280                               /* @4600 fbf73efe bl #-17284 -> sub_0280 */
L_4604:
    adds r4, r4, #1                         /* @4604 641c adds r4, r4, #1  */
L_4606:
    uxtb r4, r4                             /* @4606 e4b2 uxtb r4, r4  */
L_4608:
    cmp r4, #3                              /* @4608 032c cmp r4, #3  */
L_460a:
    blo L_45e4                              /* @460A ebd3 blo #-42 -> 0x45E4 */
L_460c:
    movs r0, #42                            /* @460C 2a20 movs r0, #42  */
L_460e:
    mov r1, sp                              /* @460E 6946 mov r1, sp  */
L_4610:
    strb r0, [r1, #24]                      /* @4610 0876 strb r0, [r1, #24]  */
L_4612:
    b L_464a                                /* @4612 1ae0 b #52 -> 0x464A */
L_4614:
    movs r3, #32                            /* @4614 2023 movs r3, #32  */
L_4616:
    mov r0, r4                              /* @4616 2046 mov r0, r4  */
L_4618:
    movs r5, #10                            /* @4618 0a25 movs r5, #10  */
L_461a:
    muls r0, r5, r0                         /* @461A 6843 muls r0, r5, r0  */
L_461c:
    strb r3, [r7, r0]                       /* @461C 3b54 strb r3, [r7, r0]  */
L_461e:
    adds r0, r0, r7                         /* @461E c019 adds r0, r0, r7  */
L_4620:
    adds r1, r2, r1                         /* @4620 5118 adds r1, r2, r1  */
L_4622:
    adds r0, r0, #1                         /* @4622 401c adds r0, r0, #1  */
L_4624:
    bl L_0280                               /* @4624 fbf72cfe bl #-17320 -> sub_0280 */
L_4628:
    adds r4, r4, #1                         /* @4628 641c adds r4, r4, #1  */
L_462a:
    uxtb r4, r4                             /* @462A e4b2 uxtb r4, r4  */
L_462c:
    ldrb r0, [r6]                           /* @462C 3078 ldrb r0, [r6]  */
L_462e:
    ldr r2, [r6, #4]                        /* @462E 7268 ldr r2, [r6, #4]  */
L_4630:
    adds r1, r0, r4                         /* @4630 0119 adds r1, r0, r4  */
L_4632:
    movs r0, #20                            /* @4632 1420 movs r0, #20  */
L_4634:
    muls r1, r0, r1                         /* @4634 4143 muls r1, r0, r1  */
L_4636:
    mov r0, r1                              /* @4636 0846 mov r0, r1  */
L_4638:
    adds r0, #12                            /* @4638 0c30 adds r0, #12  */
L_463a:
    ldr r0, [r2, r0]                        /* @463A 1058 ldr r0, [r2, r0]  */
L_463c:
    cmp r0, #0                              /* @463C 0028 cmp r0, #0  */
L_463e:
    beq L_4644                              /* @463E 01d0 beq #2 -> 0x4644 */
L_4640:
    cmp r4, #3                              /* @4640 032c cmp r4, #3  */
L_4642:
    blo L_4614                              /* @4642 e7d3 blo #-50 -> 0x4614 */
L_4644:
    movs r0, #42                            /* @4644 2a20 movs r0, #42  */
L_4646:
    mov r1, sp                              /* @4646 6946 mov r1, sp  */
L_4648:
    strb r0, [r1, #4]                       /* @4648 0871 strb r0, [r1, #4]  */
L_464a:
    add r1, sp, #12                         /* @464A 03a9 add r1, sp, #12  */
L_464c:
    add r2, sp, #24                         /* @464C 06aa add r2, sp, #24  */
L_464e:
    adds r1, #2                             /* @464E 0231 adds r1, #2  */
L_4650:
    add r0, sp, #4                          /* @4650 01a8 add r0, sp, #4  */
L_4652:
    bl L_4884                               /* @4652 00f017f9 bl #558 -> display_three_lines */
L_4656:
    add sp, #36                             /* @4656 09b0 add sp, #36  */
L_4658:
    pop {r4, r5, r6, r7, pc}                /* @4658 f0bd pop {r4, r5, r6, r7, pc}  */
    .byte 0x00,0x00 /* @465A unclassified */
L_465c:
    .byte 0x28,0x06,0x00,0x20 /* @465C literal */

/* sub_4660 */
L_4660:
    push {r4, lr}                           /* @4660 10b5 push {r4, lr}  */
L_4662:
    movs r2, #31                            /* @4662 1f22 movs r2, #31  */
L_4664:
    ldr r3, [pc, #36]                       /* @4664 094b ldr r3, [pc, #36] literal 0x468C -> 0x20000004 */
L_4666:
    lsls r2, r2, #10                        /* @4666 9202 lsls r2, r2, #10  */
L_4668:
    str r2, [r3]                            /* @4668 1a60 str r2, [r3]  */
L_466a:
    movs r1, #7                             /* @466A 0721 movs r1, #7  */
L_466c:
    ldr r4, [pc, #32]                       /* @466C 084c ldr r4, [pc, #32] literal 0x4690 -> 0x0000FFFF */
L_466e:
    lsls r0, r1, #6                         /* @466E 8801 lsls r0, r1, #6  */
L_4670:
    adds r0, r0, r2                         /* @4670 8018 adds r0, r0, r2  */
L_4672:
    bl L_12f0                               /* @4672 fcf73dfe bl #-13190 -> sub_12F0 */
L_4676:
    cmp r0, r4                              /* @4676 a042 cmp r0, r4  */
L_4678:
    beq L_4682                              /* @4678 03d0 beq #6 -> 0x4682 */
L_467a:
    lsls r0, r1, #6                         /* @467A 8801 lsls r0, r1, #6  */
L_467c:
    adds r0, r0, r2                         /* @467C 8018 adds r0, r0, r2  */
L_467e:
    str r0, [r3]                            /* @467E 1860 str r0, [r3]  */
L_4680:
    pop {r4, pc}                            /* @4680 10bd pop {r4, pc}  */
L_4682:
    subs r1, r1, #1                         /* @4682 491e subs r1, r1, #1  */
L_4684:
    cmp r1, #0                              /* @4684 0029 cmp r1, #0  */
L_4686:
    bne L_466e                              /* @4686 f2d1 bne #-28 -> 0x466E */
L_4688:
    pop {r4, pc}                            /* @4688 10bd pop {r4, pc}  */
    .byte 0x00,0x00 /* @468A unclassified */
L_468c:
    .byte 0x04,0x00,0x00,0x20 /* @468C literal */
L_4690:
    .byte 0xff,0xff,0x00,0x00 /* @4690 literal */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @4694 unclassified */

/* sub_46A0 */
L_46a0:
    push {r4, lr}                           /* @46A0 10b5 push {r4, lr}  */
L_46a2:
    movs r1, #0                             /* @46A2 0021 movs r1, #0  */
L_46a4:
    ldr r3, [pc, #32]                       /* @46A4 084b ldr r3, [pc, #32] literal 0x46C8 -> 0x20000004 */
L_46a6:
    ldr r2, [pc, #36]                       /* @46A6 094a ldr r2, [pc, #36] literal 0x46CC -> 0x0000FFFF */
L_46a8:
    ldr r4, [r3]                            /* @46A8 1c68 ldr r4, [r3]  */
L_46aa:
    lsls r0, r1, #1                         /* @46AA 4800 lsls r0, r1, #1  */
L_46ac:
    adds r0, r0, r4                         /* @46AC 0019 adds r0, r0, r4  */
L_46ae:
    bl L_12f0                               /* @46AE fcf71ffe bl #-13250 -> sub_12F0 */
L_46b2:
    cmp r0, r2                              /* @46B2 9042 cmp r0, r2  */
L_46b4:
    beq L_46ba                              /* @46B4 01d0 beq #2 -> 0x46BA */
L_46b6:
    movs r0, #0                             /* @46B6 0020 movs r0, #0  */
L_46b8:
    pop {r4, pc}                            /* @46B8 10bd pop {r4, pc}  */
L_46ba:
    adds r1, r1, #1                         /* @46BA 491c adds r1, r1, #1  */
L_46bc:
    uxth r1, r1                             /* @46BC 89b2 uxth r1, r1  */
L_46be:
    cmp r1, #22                             /* @46BE 1629 cmp r1, #22  */
L_46c0:
    blo L_46a8                              /* @46C0 f2d3 blo #-28 -> 0x46A8 */
L_46c2:
    movs r0, #1                             /* @46C2 0120 movs r0, #1  */
L_46c4:
    pop {r4, pc}                            /* @46C4 10bd pop {r4, pc}  */
    .byte 0x00,0x00 /* @46C6 unclassified */
L_46c8:
    .byte 0x04,0x00,0x00,0x20 /* @46C8 literal */
L_46cc:
    .byte 0xff,0xff,0x00,0x00 /* @46CC literal */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @46D0 unclassified */

/* sub_46E0 */
L_46e0:
    push {r4, lr}                           /* @46E0 10b5 push {r4, lr}  */
L_46e2:
    mov r1, r0                              /* @46E2 0146 mov r1, r0  */
L_46e4:
    movs r0, #63                            /* @46E4 3f20 movs r0, #63  */
L_46e6:
    movs r2, #203                           /* @46E6 cb22 movs r2, #203  */
L_46e8:
    lsls r0, r0, #9                         /* @46E8 4002 lsls r0, r0, #9  */
L_46ea:
    bl L_113c                               /* @46EA fcf727fd bl #-13746 -> sub_113C */
L_46ee:
    pop {r4, pc}                            /* @46EE 10bd pop {r4, pc}  */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @46F0 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @4700 unclassified */

/* sub_470C */
L_470c:
    push {r3, r4, r5, r6, r7, lr}           /* @470C f8b5 push {r3, r4, r5, r6, r7, lr}  */
L_470e:
    ldr r1, [pc, #280]                      /* @470E 4649 ldr r1, [pc, #280] literal 0x4828 -> 0x200006F4 */
L_4710:
    movs r0, #1                             /* @4710 0120 movs r0, #1  */
L_4712:
    strh r0, [r1, #54]                      /* @4712 c886 strh r0, [r1, #54]  */
L_4714:
    ldr r4, [pc, #276]                      /* @4714 454c ldr r4, [pc, #276] literal 0x482C -> 0x20000004 */
L_4716:
    ldr r0, [r4]                            /* @4716 2068 ldr r0, [r4]  */
L_4718:
    movs r5, #31                            /* @4718 1f25 movs r5, #31  */
L_471a:
    ldrh r0, [r0, #54]                      /* @471A c08e ldrh r0, [r0, #54]  */
L_471c:
    lsls r5, r5, #10                        /* @471C ad02 lsls r5, r5, #10  */
L_471e:
    cmp r0, #1                              /* @471E 0128 cmp r0, #1  */
L_4720:
    bne L_4750                              /* @4720 16d1 bne #44 -> 0x4750 */
L_4722:
    ldr r0, [r4]                            /* @4722 2068 ldr r0, [r4]  */
L_4724:
    ldr r6, [pc, #264]                      /* @4724 424e ldr r6, [pc, #264] literal 0x4830 -> 0x00007DC0 */
L_4726:
    adds r0, #64                            /* @4726 4030 adds r0, #64  */
L_4728:
    cmp r0, r6                              /* @4728 b042 cmp r0, r6  */
L_472a:
    bhi L_4750                              /* @472A 11d8 bhi #34 -> 0x4750 */
L_472c:
    ldr r0, [r4]                            /* @472C 2068 ldr r0, [r4]  */
L_472e:
    adds r0, #64                            /* @472E 4030 adds r0, #64  */
L_4730:
    str r0, [r4]                            /* @4730 2060 str r0, [r4]  */
L_4732:
    b L_4742                                /* @4732 06e0 b #12 -> 0x4742 */
L_4734:
    ldr r0, [r4]                            /* @4734 2068 ldr r0, [r4]  */
L_4736:
    adds r0, #64                            /* @4736 4030 adds r0, #64  */
L_4738:
    cmp r0, r6                              /* @4738 b042 cmp r0, r6  */
L_473a:
    bhi L_474c                              /* @473A 07d8 bhi #14 -> 0x474C */
L_473c:
    ldr r0, [r4]                            /* @473C 2068 ldr r0, [r4]  */
L_473e:
    adds r0, #64                            /* @473E 4030 adds r0, #64  */
L_4740:
    str r0, [r4]                            /* @4740 2060 str r0, [r4]  */
L_4742:
    bl L_46a0                               /* @4742 fff7adff bl #-166 -> sub_46A0 */
L_4746:
    cmp r0, #0                              /* @4746 0028 cmp r0, #0  */
L_4748:
    beq L_4734                              /* @4748 f4d0 beq #-24 -> 0x4734 */
L_474a:
    b L_4752                                /* @474A 02e0 b #4 -> 0x4752 */
L_474c:
    str r5, [r4]                            /* @474C 2560 str r5, [r4]  */
L_474e:
    b L_4752                                /* @474E 00e0 b #0 -> 0x4752 */
L_4750:
    str r5, [r4]                            /* @4750 2560 str r5, [r4]  */
L_4752:
    bl L_5874                               /* @4752 01f08ff8 bl #4382 -> sub_5874 */
L_4756:
    ldr r0, [r4]                            /* @4756 2068 ldr r0, [r4]  */
L_4758:
    cmp r0, r5                              /* @4758 a842 cmp r0, r5  */
L_475a:
    bne L_4768                              /* @475A 05d1 bne #10 -> 0x4768 */
L_475c:
    movs r2, #28                            /* @475C 1c22 movs r2, #28  */
L_475e:
    ldr r1, [pc, #200]                      /* @475E 3249 ldr r1, [pc, #200] literal 0x4828 -> 0x200006F4 */
L_4760:
    ldr r0, [r4]                            /* @4760 2068 ldr r0, [r4]  */
L_4762:
    bl L_113c                               /* @4762 fcf7ebfc bl #-13866 -> sub_113C */
L_4766:
    b L_4772                                /* @4766 04e0 b #8 -> 0x4772 */
L_4768:
    movs r2, #28                            /* @4768 1c22 movs r2, #28  */
L_476a:
    ldr r1, [pc, #188]                      /* @476A 2f49 ldr r1, [pc, #188] literal 0x4828 -> 0x200006F4 */
L_476c:
    ldr r0, [r4]                            /* @476C 2068 ldr r0, [r4]  */
L_476e:
    bl L_1164                               /* @476E fcf7f9fc bl #-13838 -> sub_1164 */
L_4772:
    bl L_3978                               /* @4772 fff701f9 bl #-3582 -> sub_3978 */
L_4776:
    bl L_52cc                               /* @4776 00f0a9fd bl #2898 -> sub_52CC */
L_477a:
    ldr r6, [pc, #184]                      /* @477A 2e4e ldr r6, [pc, #184] literal 0x4834 -> 0x200005E4 */
L_477c:
    ldrh r1, [r6, #4]                       /* @477C b188 ldrh r1, [r6, #4]  */
L_477e:
    movs r7, #1                             /* @477E 0127 movs r7, #1  */
L_4780:
    lsls r7, r7, #13                        /* @4780 7f03 lsls r7, r7, #13  */
L_4782:
    mov r0, r7                              /* @4782 3846 mov r0, r7  */
L_4784:
    bl L_0110                               /* @4784 fbf7c4fc bl #-18040 -> udivmod32 */
L_4788:
    ldr r4, [pc, #172]                      /* @4788 2b4c ldr r4, [pc, #172] literal 0x4838 -> 0x20000648 */
L_478a:
    str r0, [r4, #16]                       /* @478A 2061 str r0, [r4, #16]  */
L_478c:
    ldr r0, [r4, #16]                       /* @478C 2069 ldr r0, [r4, #16]  */
L_478e:
    rsbs r0, r0, #0                         /* @478E 4042 rsbs r0, r0, #0  */
L_4790:
    str r0, [r4, #20]                       /* @4790 6061 str r0, [r4, #20]  */
L_4792:
    ldr r5, [pc, #168]                      /* @4792 2a4d ldr r5, [pc, #168] literal 0x483C -> 0x2000072C */
L_4794:
    ldrh r1, [r5]                           /* @4794 2988 ldrh r1, [r5]  */
L_4796:
    ldrh r0, [r5]                           /* @4796 2888 ldrh r0, [r5]  */
L_4798:
    lsrs r0, r0, #1                         /* @4798 4008 lsrs r0, r0, #1  */
L_479a:
    adds r0, r0, r7                         /* @479A c019 adds r0, r0, r7  */
L_479c:
    bl L_0110                               /* @479C fbf7b8fc bl #-18064 -> udivmod32 */
L_47a0:
    str r0, [r4, #12]                       /* @47A0 e060 str r0, [r4, #12]  */
L_47a2:
    ldrh r1, [r6, #4]                       /* @47A2 b188 ldrh r1, [r6, #4]  */
L_47a4:
    ldrh r0, [r5]                           /* @47A4 2888 ldrh r0, [r5]  */
L_47a6:
    muls r1, r0, r1                         /* @47A6 4143 muls r1, r0, r1  */
L_47a8:
    ldrh r0, [r6, #4]                       /* @47A8 b088 ldrh r0, [r6, #4]  */
L_47aa:
    ldrh r2, [r5]                           /* @47AA 2a88 ldrh r2, [r5]  */
L_47ac:
    muls r0, r2, r0                         /* @47AC 5043 muls r0, r2, r0  */
L_47ae:
    asrs r0, r0, #1                         /* @47AE 4010 asrs r0, r0, #1  */
L_47b0:
    lsls r2, r7, #8                         /* @47B0 3a02 lsls r2, r7, #8  */
L_47b2:
    adds r0, r0, r2                         /* @47B2 8018 adds r0, r0, r2  */
L_47b4:
    bl L_0110                               /* @47B4 fbf7acfc bl #-18088 -> udivmod32 */
L_47b8:
    str r0, [r4, #24]                       /* @47B8 a061 str r0, [r4, #24]  */
L_47ba:
    ldrh r0, [r5, #2]                       /* @47BA 6888 ldrh r0, [r5, #2]  */
L_47bc:
    ldr r1, [pc, #128]                      /* @47BC 2049 ldr r1, [pc, #128] literal 0x4840 -> 0x20000145 */
L_47be:
    bl L_3052                               /* @47BE fef748fc bl #-6000 -> sub_3052 */
L_47c2:
    ldr r6, [pc, #128]                      /* @47C2 204e ldr r6, [pc, #128] literal 0x4844 -> 0x200005D9 */
L_47c4:
    ldr r7, [pc, #128]                      /* @47C4 204f ldr r7, [pc, #128] literal 0x4848 -> 0x40005800 */
L_47c6:
    ldrb r0, [r6]                           /* @47C6 3078 ldrb r0, [r6]  */
L_47c8:
    cmp r0, #1                              /* @47C8 0128 cmp r0, #1  */
L_47ca:
    bne L_47de                              /* @47CA 08d1 bne #16 -> 0x47DE */
L_47cc:
    ldr r0, [r7, #12]                       /* @47CC f868 ldr r0, [r7, #12]  */
L_47ce:
    lsls r0, r0, #31                        /* @47CE c007 lsls r0, r0, #31  */
L_47d0:
    beq L_47de                              /* @47D0 05d0 beq #10 -> 0x47DE */
L_47d2:
    ldrb r0, [r5, #9]                       /* @47D2 687a ldrb r0, [r5, #9]  */
L_47d4:
    bl L_570c                               /* @47D4 00f09aff bl #3892 -> sub_570C */
L_47d8:
    strb r0, [r4]                           /* @47D8 2070 strb r0, [r4]  */
L_47da:
    movs r0, #0                             /* @47DA 0020 movs r0, #0  */
L_47dc:
    strb r0, [r4, #1]                       /* @47DC 6070 strb r0, [r4, #1]  */
L_47de:
    ldrb r0, [r6]                           /* @47DE 3078 ldrb r0, [r6]  */
L_47e0:
    cmp r0, #4                              /* @47E0 0428 cmp r0, #4  */
L_47e2:
    bne L_47ea                              /* @47E2 02d1 bne #4 -> 0x47EA */
L_47e4:
    ldr r2, [pc, #104]                      /* @47E4 1a4a ldr r2, [pc, #104] literal 0x4850 -> 0x20000628 */
L_47e6:
    ldr r1, [pc, #100]                      /* @47E6 1949 ldr r1, [pc, #100] literal 0x484C -> 0x20000358 */
L_47e8:
    str r1, [r2, #4]                        /* @47E8 5160 str r1, [r2, #4]  */
L_47ea:
    cmp r0, #2                              /* @47EA 0228 cmp r0, #2  */
L_47ec:
    bne L_47f4                              /* @47EC 02d1 bne #4 -> 0x47F4 */
L_47ee:
    ldrb r0, [r5, #4]                       /* @47EE 2879 ldrb r0, [r5, #4]  */
L_47f0:
    bl L_583c                               /* @47F0 01f024f8 bl #4168 -> sub_583C */
L_47f4:
    ldrb r0, [r5, #9]                       /* @47F4 687a ldrb r0, [r5, #9]  */
L_47f6:
    cmp r0, #2                              /* @47F6 0228 cmp r0, #2  */
L_47f8:
    bne L_4806                              /* @47F8 05d1 bne #10 -> 0x4806 */
L_47fa:
    ldrh r1, [r5]                           /* @47FA 2988 ldrh r1, [r5]  */
L_47fc:
    ldr r5, [pc, #84]                       /* @47FC 154d ldr r5, [pc, #84] literal 0x4854 -> 0x20000618 */
L_47fe:
    ldrh r0, [r5]                           /* @47FE 2888 ldrh r0, [r5]  */
L_4800:
    bl L_0110                               /* @4800 fbf786fc bl #-18164 -> udivmod32 */
L_4804:
    strh r0, [r5, #2]                       /* @4804 6880 strh r0, [r5, #2]  */
L_4806:
    ldrb r0, [r6]                           /* @4806 3078 ldrb r0, [r6]  */
L_4808:
    cmp r0, #3                              /* @4808 0328 cmp r0, #3  */
L_480a:
    bne L_4810                              /* @480A 01d1 bne #2 -> 0x4810 */
L_480c:
    bl L_5bd4                               /* @480C 01f0e2f9 bl #5060 -> sub_5BD4 */
L_4810:
    movs r0, #0                             /* @4810 0020 movs r0, #0  */
L_4812:
    strb r0, [r6]                           /* @4812 3070 strb r0, [r6]  */
L_4814:
    ldr r0, [r7, #12]                       /* @4814 f868 ldr r0, [r7, #12]  */
L_4816:
    lsls r0, r0, #31                        /* @4816 c007 lsls r0, r0, #31  */
L_4818:
    beq L_4824                              /* @4818 04d0 beq #8 -> 0x4824 */
L_481a:
    ldrb r0, [r4]                           /* @481A 2078 ldrb r0, [r4]  */
L_481c:
    cmp r0, #0                              /* @481C 0028 cmp r0, #0  */
L_481e:
    bne L_4824                              /* @481E 01d1 bne #2 -> 0x4824 */
L_4820:
    bl L_58a0                               /* @4820 01f03ef8 bl #4220 -> sub_58A0 */
L_4824:
    pop {r3, r4, r5, r6, r7, pc}            /* @4824 f8bd pop {r3, r4, r5, r6, r7, pc}  */
    .byte 0x00,0x00 /* @4826 unclassified */
L_4828:
    .byte 0xf4,0x06,0x00,0x20 /* @4828 literal */
L_482c:
    .byte 0x04,0x00,0x00,0x20 /* @482C literal */
L_4830:
    .byte 0xc0,0x7d,0x00,0x00 /* @4830 literal */
L_4834:
    .byte 0xe4,0x05,0x00,0x20 /* @4834 literal */
L_4838:
    .byte 0x48,0x06,0x00,0x20 /* @4838 literal */
L_483c:
    .byte 0x2c,0x07,0x00,0x20 /* @483C literal */
L_4840:
    .byte 0x45,0x01,0x00,0x20 /* @4840 literal */
L_4844:
    .byte 0xd9,0x05,0x00,0x20 /* @4844 literal */
L_4848:
    .byte 0x00,0x58,0x00,0x40 /* @4848 literal */
L_484c:
    .byte 0x58,0x03,0x00,0x20 /* @484C literal */
L_4850:
    .byte 0x28,0x06,0x00,0x20 /* @4850 literal */
L_4854:
    .byte 0x18,0x06,0x00,0x20 /* @4854 literal */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @4858 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @4868 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @4878 unclassified */

/* display_three_lines */
L_4884:
    push {r4, r5, r6, lr}                   /* @4884 70b5 push {r4, r5, r6, lr}  */
L_4886:
    mov r4, r0                              /* @4886 0446 mov r4, r0  */
L_4888:
    mov r5, r1                              /* @4888 0d46 mov r5, r1  */
L_488a:
    mov r6, r2                              /* @488A 1646 mov r6, r2  */
L_488c:
    mov r2, r6                              /* @488C 3246 mov r2, r6  */
L_488e:
    mov r1, r5                              /* @488E 2946 mov r1, r5  */
L_4890:
    mov r0, r4                              /* @4890 2046 mov r0, r4  */
L_4892:
    bl L_4a2c                               /* @4892 00f0cbf8 bl #406 -> sub_4A2C */
L_4896:
    mov r2, r6                              /* @4896 3246 mov r2, r6  */
L_4898:
    mov r1, r5                              /* @4898 2946 mov r1, r5  */
L_489a:
    mov r0, r4                              /* @489A 2046 mov r0, r4  */
L_489c:
    bl L_4b70                               /* @489C 00f068f9 bl #720 -> sub_4B70 */
L_48a0:
    movs r1, #0                             /* @48A0 0021 movs r1, #0  */
L_48a2:
    mov r0, r1                              /* @48A2 0846 mov r0, r1  */
L_48a4:
    bl L_4d64                               /* @48A4 00f05efa bl #1212 -> sub_4D64 */
L_48a8:
    ldr r5, [pc, #108]                      /* @48A8 1b4d ldr r5, [pc, #108] literal 0x4918 -> 0x20000622 */
L_48aa:
    ldr r1, [pc, #112]                      /* @48AA 1c49 ldr r1, [pc, #112] literal 0x491C -> 0x200008EC */
L_48ac:
    ldrh r2, [r5]                           /* @48AC 2a88 ldrh r2, [r5]  */
L_48ae:
    movs r0, #64                            /* @48AE 4020 movs r0, #64  */
L_48b0:
    bl L_14b8                               /* @48B0 fcf702fe bl #-13308 -> sub_14B8 */
L_48b4:
    ldrh r4, [r5]                           /* @48B4 2c88 ldrh r4, [r5]  */
L_48b6:
    ldrh r0, [r5, #2]                       /* @48B6 6888 ldrh r0, [r5, #2]  */
L_48b8:
    cmp r4, r0                              /* @48B8 8442 cmp r4, r0  */
L_48ba:
    bhi L_48be                              /* @48BA 00d8 bhi #0 -> 0x48BE */
L_48bc:
    mov r4, r0                              /* @48BC 0446 mov r4, r0  */
L_48be:
    movs r1, #1                             /* @48BE 0121 movs r1, #1  */
L_48c0:
    movs r0, #0                             /* @48C0 0020 movs r0, #0  */
L_48c2:
    bl L_4d64                               /* @48C2 00f04ffa bl #1182 -> sub_4D64 */
L_48c6:
    ldr r1, [pc, #84]                       /* @48C6 1549 ldr r1, [pc, #84] literal 0x491C -> 0x200008EC */
L_48c8:
    mov r2, r4                              /* @48C8 2246 mov r2, r4  */
L_48ca:
    adds r1, #72                            /* @48CA 4831 adds r1, #72  */
L_48cc:
    movs r0, #64                            /* @48CC 4020 movs r0, #64  */
L_48ce:
    bl L_14b8                               /* @48CE fcf7f3fd bl #-13338 -> sub_14B8 */
L_48d2:
    movs r1, #2                             /* @48D2 0221 movs r1, #2  */
L_48d4:
    movs r0, #0                             /* @48D4 0020 movs r0, #0  */
L_48d6:
    bl L_4d64                               /* @48D6 00f045fa bl #1162 -> sub_4D64 */
L_48da:
    ldr r1, [pc, #64]                       /* @48DA 1049 ldr r1, [pc, #64] literal 0x491C -> 0x200008EC */
L_48dc:
    ldrh r2, [r5, #2]                       /* @48DC 6a88 ldrh r2, [r5, #2]  */
L_48de:
    adds r1, #144                           /* @48DE 9031 adds r1, #144  */
L_48e0:
    movs r0, #64                            /* @48E0 4020 movs r0, #64  */
L_48e2:
    bl L_14b8                               /* @48E2 fcf7e9fd bl #-13358 -> sub_14B8 */
L_48e6:
    ldrh r4, [r5, #2]                       /* @48E6 6c88 ldrh r4, [r5, #2]  */
L_48e8:
    ldrh r0, [r5, #4]                       /* @48E8 a888 ldrh r0, [r5, #4]  */
L_48ea:
    cmp r4, r0                              /* @48EA 8442 cmp r4, r0  */
L_48ec:
    bhi L_48f0                              /* @48EC 00d8 bhi #0 -> 0x48F0 */
L_48ee:
    mov r4, r0                              /* @48EE 0446 mov r4, r0  */
L_48f0:
    movs r1, #3                             /* @48F0 0321 movs r1, #3  */
L_48f2:
    movs r0, #0                             /* @48F2 0020 movs r0, #0  */
L_48f4:
    bl L_4d64                               /* @48F4 00f036fa bl #1132 -> sub_4D64 */
L_48f8:
    ldr r1, [pc, #32]                       /* @48F8 0849 ldr r1, [pc, #32] literal 0x491C -> 0x200008EC */
L_48fa:
    mov r2, r4                              /* @48FA 2246 mov r2, r4  */
L_48fc:
    adds r1, #216                           /* @48FC d831 adds r1, #216  */
L_48fe:
    movs r0, #64                            /* @48FE 4020 movs r0, #64  */
L_4900:
    bl L_14b8                               /* @4900 fcf7dafd bl #-13388 -> sub_14B8 */
L_4904:
    movs r1, #4                             /* @4904 0421 movs r1, #4  */
L_4906:
    movs r0, #0                             /* @4906 0020 movs r0, #0  */
L_4908:
    bl L_4d64                               /* @4908 00f02cfa bl #1112 -> sub_4D64 */
L_490c:
    ldrh r2, [r5, #4]                       /* @490C aa88 ldrh r2, [r5, #4]  */
L_490e:
    ldr r1, [pc, #16]                       /* @490E 0449 ldr r1, [pc, #16] literal 0x4920 -> 0x20000A0C */
L_4910:
    movs r0, #64                            /* @4910 4020 movs r0, #64  */
L_4912:
    bl L_14b8                               /* @4912 fcf7d1fd bl #-13406 -> sub_14B8 */
L_4916:
    pop {r4, r5, r6, pc}                    /* @4916 70bd pop {r4, r5, r6, pc}  */
L_4918:
    .byte 0x22,0x06,0x00,0x20 /* @4918 literal */
L_491c:
    .byte 0xec,0x08,0x00,0x20 /* @491C literal */
L_4920:
    .byte 0x0c,0x0a,0x00,0x20 /* @4920 literal */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @4924 unclassified */

/* sub_492C */
L_492c:
    push {r4, lr}                           /* @492C 10b5 push {r4, lr}  */
L_492e:
    movs r1, #12                            /* @492E 0c21 movs r1, #12  */
L_4930:
    movs r0, #0                             /* @4930 0020 movs r0, #0  */
L_4932:
    bl L_1318                               /* @4932 fcf7f1fc bl #-13854 -> sub_1318 */
L_4936:
    movs r0, #100                           /* @4936 6420 movs r0, #100  */
L_4938:
    bl L_2de8                               /* @4938 fef756fa bl #-6996 -> delay_ms_candidate */
L_493c:
    movs r1, #12                            /* @493C 0c21 movs r1, #12  */
L_493e:
    movs r0, #0                             /* @493E 0020 movs r0, #0  */
L_4940:
    bl L_1430                               /* @4940 fcf776fd bl #-13588 -> sub_1430 */
L_4944:
    movs r0, #100                           /* @4944 6420 movs r0, #100  */
L_4946:
    bl L_2de8                               /* @4946 fef74ffa bl #-7010 -> delay_ms_candidate */
L_494a:
    movs r0, #174                           /* @494A ae20 movs r0, #174  */
L_494c:
    bl L_4d84                               /* @494C 00f01afa bl #1076 -> sub_4D84 */
L_4950:
    movs r0, #213                           /* @4950 d520 movs r0, #213  */
L_4952:
    bl L_4d84                               /* @4952 00f017fa bl #1070 -> sub_4D84 */
L_4956:
    movs r0, #240                           /* @4956 f020 movs r0, #240  */
L_4958:
    bl L_4d84                               /* @4958 00f014fa bl #1064 -> sub_4D84 */
L_495c:
    movs r0, #168                           /* @495C a820 movs r0, #168  */
L_495e:
    bl L_4d84                               /* @495E 00f011fa bl #1058 -> sub_4D84 */
L_4962:
    movs r0, #39                            /* @4962 2720 movs r0, #39  */
L_4964:
    bl L_4d84                               /* @4964 00f00efa bl #1052 -> sub_4D84 */
L_4968:
    movs r0, #211                           /* @4968 d320 movs r0, #211  */
L_496a:
    bl L_4d84                               /* @496A 00f00bfa bl #1046 -> sub_4D84 */
L_496e:
    movs r0, #0                             /* @496E 0020 movs r0, #0  */
L_4970:
    bl L_4d84                               /* @4970 00f008fa bl #1040 -> sub_4D84 */
L_4974:
    movs r0, #64                            /* @4974 4020 movs r0, #64  */
L_4976:
    bl L_4d84                               /* @4976 00f005fa bl #1034 -> sub_4D84 */
L_497a:
    movs r0, #141                           /* @497A 8d20 movs r0, #141  */
L_497c:
    bl L_4d84                               /* @497C 00f002fa bl #1028 -> sub_4D84 */
L_4980:
    movs r0, #20                            /* @4980 1420 movs r0, #20  */
L_4982:
    bl L_4d84                               /* @4982 00f0fff9 bl #1022 -> sub_4D84 */
L_4986:
    movs r0, #32                            /* @4986 2020 movs r0, #32  */
L_4988:
    bl L_4d84                               /* @4988 00f0fcf9 bl #1016 -> sub_4D84 */
L_498c:
    movs r0, #2                             /* @498C 0220 movs r0, #2  */
L_498e:
    bl L_4d84                               /* @498E 00f0f9f9 bl #1010 -> sub_4D84 */
L_4992:
    movs r0, #161                           /* @4992 a120 movs r0, #161  */
L_4994:
    bl L_4d84                               /* @4994 00f0f6f9 bl #1004 -> sub_4D84 */
L_4998:
    movs r0, #200                           /* @4998 c820 movs r0, #200  */
L_499a:
    bl L_4d84                               /* @499A 00f0f3f9 bl #998 -> sub_4D84 */
L_499e:
    movs r0, #218                           /* @499E da20 movs r0, #218  */
L_49a0:
    bl L_4d84                               /* @49A0 00f0f0f9 bl #992 -> sub_4D84 */
L_49a4:
    movs r0, #18                            /* @49A4 1220 movs r0, #18  */
L_49a6:
    bl L_4d84                               /* @49A6 00f0edf9 bl #986 -> sub_4D84 */
L_49aa:
    movs r0, #173                           /* @49AA ad20 movs r0, #173  */
L_49ac:
    bl L_4d84                               /* @49AC 00f0eaf9 bl #980 -> sub_4D84 */
L_49b0:
    movs r0, #48                            /* @49B0 3020 movs r0, #48  */
L_49b2:
    bl L_4d84                               /* @49B2 00f0e7f9 bl #974 -> sub_4D84 */
L_49b6:
    movs r0, #129                           /* @49B6 8120 movs r0, #129  */
L_49b8:
    bl L_4d84                               /* @49B8 00f0e4f9 bl #968 -> sub_4D84 */
L_49bc:
    movs r0, #64                            /* @49BC 4020 movs r0, #64  */
L_49be:
    bl L_4d84                               /* @49BE 00f0e1f9 bl #962 -> sub_4D84 */
L_49c2:
    movs r0, #217                           /* @49C2 d920 movs r0, #217  */
L_49c4:
    bl L_4d84                               /* @49C4 00f0def9 bl #956 -> sub_4D84 */
L_49c8:
    movs r0, #34                            /* @49C8 2220 movs r0, #34  */
L_49ca:
    bl L_4d84                               /* @49CA 00f0dbf9 bl #950 -> sub_4D84 */
L_49ce:
    movs r0, #219                           /* @49CE db20 movs r0, #219  */
L_49d0:
    bl L_4d84                               /* @49D0 00f0d8f9 bl #944 -> sub_4D84 */
L_49d4:
    movs r0, #32                            /* @49D4 2020 movs r0, #32  */
L_49d6:
    bl L_4d84                               /* @49D6 00f0d5f9 bl #938 -> sub_4D84 */
L_49da:
    movs r0, #164                           /* @49DA a420 movs r0, #164  */
L_49dc:
    bl L_4d84                               /* @49DC 00f0d2f9 bl #932 -> sub_4D84 */
L_49e0:
    movs r0, #166                           /* @49E0 a620 movs r0, #166  */
L_49e2:
    bl L_4d84                               /* @49E2 00f0cff9 bl #926 -> sub_4D84 */
L_49e6:
    movs r0, #12                            /* @49E6 0c20 movs r0, #12  */
L_49e8:
    bl L_4d84                               /* @49E8 00f0ccf9 bl #920 -> sub_4D84 */
L_49ec:
    movs r0, #17                            /* @49EC 1120 movs r0, #17  */
L_49ee:
    bl L_4d84                               /* @49EE 00f0c9f9 bl #914 -> sub_4D84 */
L_49f2:
    movs r0, #175                           /* @49F2 af20 movs r0, #175  */
L_49f4:
    bl L_4d84                               /* @49F4 00f0c6f9 bl #908 -> sub_4D84 */
L_49f8:
    bl L_4a00                               /* @49F8 00f002f8 bl #4 -> sub_4A00 */
L_49fc:
    pop {r4, pc}                            /* @49FC 10bd pop {r4, pc}  */
    .byte 0x00,0x00 /* @49FE unclassified */

/* sub_4A00 */
L_4a00:
    push {r4, r5, lr}                       /* @4A00 30b5 push {r4, r5, lr}  */
L_4a02:
    movs r4, #0                             /* @4A02 0024 movs r4, #0  */
L_4a04:
    ldr r5, [pc, #32]                       /* @4A04 084d ldr r5, [pc, #32] literal 0x4A28 -> 0x200008EC */
L_4a06:
    mov r1, r4                              /* @4A06 2146 mov r1, r4  */
L_4a08:
    movs r0, #0                             /* @4A08 0020 movs r0, #0  */
L_4a0a:
    bl L_4d64                               /* @4A0A 00f0abf9 bl #854 -> sub_4D64 */
L_4a0e:
    movs r0, #72                            /* @4A0E 4820 movs r0, #72  */
L_4a10:
    muls r0, r4, r0                         /* @4A10 6043 muls r0, r4, r0  */
L_4a12:
    adds r1, r0, r5                         /* @4A12 4119 adds r1, r0, r5  */
L_4a14:
    movs r2, #72                            /* @4A14 4822 movs r2, #72  */
L_4a16:
    movs r0, #64                            /* @4A16 4020 movs r0, #64  */
L_4a18:
    bl L_14b8                               /* @4A18 fcf74efd bl #-13668 -> sub_14B8 */
L_4a1c:
    adds r4, r4, #1                         /* @4A1C 641c adds r4, r4, #1  */
L_4a1e:
    uxtb r4, r4                             /* @4A1E e4b2 uxtb r4, r4  */
L_4a20:
    cmp r4, #5                              /* @4A20 052c cmp r4, #5  */
L_4a22:
    blo L_4a06                              /* @4A22 f0d3 blo #-32 -> 0x4A06 */
L_4a24:
    pop {r4, r5, pc}                        /* @4A24 30bd pop {r4, r5, pc}  */
    .byte 0x00,0x00 /* @4A26 unclassified */
L_4a28:
    .byte 0xec,0x08,0x00,0x20 /* @4A28 literal */

/* sub_4A2C */
L_4a2c:
    push {r3, r4, r5, r6, r7, lr}           /* @4A2C f8b5 push {r3, r4, r5, r6, r7, lr}  */
L_4a2e:
    mov r5, r1                              /* @4A2E 0d46 mov r5, r1  */
L_4a30:
    mov r4, r2                              /* @4A30 1446 mov r4, r2  */
L_4a32:
    bl L_0272                               /* @4A32 fbf71efc bl #-18372 -> sub_0272 */
L_4a36:
    uxtb r7, r0                             /* @4A36 c7b2 uxtb r7, r0  */
L_4a38:
    mov r0, r5                              /* @4A38 2846 mov r0, r5  */
L_4a3a:
    bl L_0272                               /* @4A3A fbf71afc bl #-18380 -> sub_0272 */
L_4a3e:
    uxtb r5, r0                             /* @4A3E c5b2 uxtb r5, r0  */
L_4a40:
    mov r0, r4                              /* @4A40 2046 mov r0, r4  */
L_4a42:
    bl L_0272                               /* @4A42 fbf716fc bl #-18388 -> sub_0272 */
L_4a46:
    uxtb r6, r0                             /* @4A46 c6b2 uxtb r6, r0  */
L_4a48:
    ldr r0, [pc, #268]                      /* @4A48 4348 ldr r0, [pc, #268] literal 0x4B58 -> 0x20000008 */
L_4a4a:
    ldrb r0, [r0]                           /* @4A4A 0078 ldrb r0, [r0]  */
L_4a4c:
    mov lr, r0                              /* @4A4C 8646 mov lr, r0  */
L_4a4e:
    cmp r0, r7                              /* @4A4E b842 cmp r0, r7  */
L_4a50:
    bls L_4a90                              /* @4A50 1ed9 bls #60 -> 0x4A90 */
L_4a52:
    mov r4, r7                              /* @4A52 3c46 mov r4, r7  */
L_4a54:
    movs r2, #0                             /* @4A54 0022 movs r2, #0  */
L_4a56:
    b L_4a80                                /* @4A56 13e0 b #38 -> 0x4A80 */
L_4a58:
    movs r0, #0                             /* @4A58 0020 movs r0, #0  */
L_4a5a:
    lsls r1, r4, #3                         /* @4A5A e100 lsls r1, r4, #3  */
L_4a5c:
    subs r1, r1, r4                         /* @4A5C 091b subs r1, r1, r4  */
L_4a5e:
    mov r12, r1                             /* @4A5E 8c46 mov r12, r1  */
L_4a60:
    mov r1, r12                             /* @4A60 6146 mov r1, r12  */
L_4a62:
    ldr r3, [pc, #248]                      /* @4A62 3e4b ldr r3, [pc, #248] literal 0x4B5C -> 0x200008EC */
L_4a64:
    adds r1, r1, r0                         /* @4A64 0918 adds r1, r1, r0  */
L_4a66:
    strb r2, [r3, r1]                       /* @4A66 5a54 strb r2, [r3, r1]  */
L_4a68:
    adds r1, r1, r3                         /* @4A68 c918 adds r1, r1, r3  */
L_4a6a:
    adds r1, #64                            /* @4A6A 4031 adds r1, #64  */
L_4a6c:
    ldrb r3, [r1, #8]                       /* @4A6C 0b7a ldrb r3, [r1, #8]  */
L_4a6e:
    lsrs r3, r3, #6                         /* @4A6E 9b09 lsrs r3, r3, #6  */
L_4a70:
    lsls r3, r3, #6                         /* @4A70 9b01 lsls r3, r3, #6  */
L_4a72:
    strb r3, [r1, #8]                       /* @4A72 0b72 strb r3, [r1, #8]  */
L_4a74:
    adds r0, r0, #1                         /* @4A74 401c adds r0, r0, #1  */
L_4a76:
    uxtb r0, r0                             /* @4A76 c0b2 uxtb r0, r0  */
L_4a78:
    cmp r0, #7                              /* @4A78 0728 cmp r0, #7  */
L_4a7a:
    blo L_4a60                              /* @4A7A f1d3 blo #-30 -> 0x4A60 */
L_4a7c:
    adds r4, r4, #1                         /* @4A7C 641c adds r4, r4, #1  */
L_4a7e:
    uxtb r4, r4                             /* @4A7E e4b2 uxtb r4, r4  */
L_4a80:
    cmp lr, r4                              /* @4A80 a645 cmp lr, r4  */
L_4a82:
    bhi L_4a58                              /* @4A82 e9d8 bhi #-46 -> 0x4A58 */
L_4a84:
    mov r0, lr                              /* @4A84 7046 mov r0, lr  */
L_4a86:
    lsls r1, r0, #3                         /* @4A86 c100 lsls r1, r0, #3  */
L_4a88:
    subs r1, r1, r0                         /* @4A88 091a subs r1, r1, r0  */
L_4a8a:
    ldr r0, [pc, #212]                      /* @4A8A 3548 ldr r0, [pc, #212] literal 0x4B60 -> 0x20000622 */
L_4a8c:
    strh r1, [r0]                           /* @4A8C 0180 strh r1, [r0]  */
L_4a8e:
    b L_4a98                                /* @4A8E 03e0 b #6 -> 0x4A98 */
L_4a90:
    lsls r0, r7, #3                         /* @4A90 f800 lsls r0, r7, #3  */
L_4a92:
    subs r1, r0, r7                         /* @4A92 c11b subs r1, r0, r7  */
L_4a94:
    ldr r0, [pc, #200]                      /* @4A94 3248 ldr r0, [pc, #200] literal 0x4B60 -> 0x20000622 */
L_4a96:
    strh r1, [r0]                           /* @4A96 0180 strh r1, [r0]  */
L_4a98:
    ldr r0, [pc, #188]                      /* @4A98 2f48 ldr r0, [pc, #188] literal 0x4B58 -> 0x20000008 */
L_4a9a:
    ldrb r0, [r0, #1]                       /* @4A9A 4078 ldrb r0, [r0, #1]  */
L_4a9c:
    mov lr, r0                              /* @4A9C 8646 mov lr, r0  */
L_4a9e:
    cmp r0, r5                              /* @4A9E a842 cmp r0, r5  */
L_4aa0:
    bls L_4af0                              /* @4AA0 26d9 bls #76 -> 0x4AF0 */
L_4aa2:
    mov r2, r5                              /* @4AA2 2a46 mov r2, r5  */
L_4aa4:
    b L_4ae0                                /* @4AA4 1ce0 b #56 -> 0x4AE0 */
L_4aa6:
    movs r0, #0                             /* @4AA6 0020 movs r0, #0  */
L_4aa8:
    lsls r1, r2, #3                         /* @4AA8 d100 lsls r1, r2, #3  */
L_4aaa:
    subs r1, r1, r2                         /* @4AAA 891a subs r1, r1, r2  */
L_4aac:
    mov r12, r1                             /* @4AAC 8c46 mov r12, r1  */
L_4aae:
    mov r1, r12                             /* @4AAE 6146 mov r1, r12  */
L_4ab0:
    adds r3, r1, r0                         /* @4AB0 0b18 adds r3, r1, r0  */
L_4ab2:
    ldr r1, [pc, #168]                      /* @4AB2 2a49 ldr r1, [pc, #168] literal 0x4B5C -> 0x200008EC */
L_4ab4:
    adds r3, r3, r1                         /* @4AB4 5b18 adds r3, r3, r1  */
L_4ab6:
    mov r1, r3                              /* @4AB6 1946 mov r1, r3  */
L_4ab8:
    adds r3, #64                            /* @4AB8 4033 adds r3, #64  */
L_4aba:
    ldrb r4, [r3, #8]                       /* @4ABA 1c7a ldrb r4, [r3, #8]  */
L_4abc:
    lsls r4, r4, #26                        /* @4ABC a406 lsls r4, r4, #26  */
L_4abe:
    lsrs r4, r4, #26                        /* @4ABE a40e lsrs r4, r4, #26  */
L_4ac0:
    strb r4, [r3, #8]                       /* @4AC0 1c72 strb r4, [r3, #8]  */
L_4ac2:
    movs r4, #0                             /* @4AC2 0024 movs r4, #0  */
L_4ac4:
    movs r3, #144                           /* @4AC4 9023 movs r3, #144  */
L_4ac6:
    strb r4, [r3, r1]                       /* @4AC6 5c54 strb r4, [r3, r1]  */
L_4ac8:
    mov r3, r1                              /* @4AC8 0b46 mov r3, r1  */
L_4aca:
    adds r3, #192                           /* @4ACA c033 adds r3, #192  */
L_4acc:
    ldrb r1, [r3, #24]                      /* @4ACC 197e ldrb r1, [r3, #24]  */
L_4ace:
    lsrs r4, r1, #4                         /* @4ACE 0c09 lsrs r4, r1, #4  */
L_4ad0:
    lsls r4, r4, #4                         /* @4AD0 2401 lsls r4, r4, #4  */
L_4ad2:
    strb r4, [r3, #24]                      /* @4AD2 1c76 strb r4, [r3, #24]  */
L_4ad4:
    adds r0, r0, #1                         /* @4AD4 401c adds r0, r0, #1  */
L_4ad6:
    uxtb r0, r0                             /* @4AD6 c0b2 uxtb r0, r0  */
L_4ad8:
    cmp r0, #7                              /* @4AD8 0728 cmp r0, #7  */
L_4ada:
    blo L_4aae                              /* @4ADA e8d3 blo #-48 -> 0x4AAE */
L_4adc:
    adds r2, r2, #1                         /* @4ADC 521c adds r2, r2, #1  */
L_4ade:
    uxtb r2, r2                             /* @4ADE d2b2 uxtb r2, r2  */
L_4ae0:
    cmp lr, r2                              /* @4AE0 9645 cmp lr, r2  */
L_4ae2:
    bhi L_4aa6                              /* @4AE2 e0d8 bhi #-64 -> 0x4AA6 */
L_4ae4:
    mov r0, lr                              /* @4AE4 7046 mov r0, lr  */
L_4ae6:
    lsls r1, r0, #3                         /* @4AE6 c100 lsls r1, r0, #3  */
L_4ae8:
    subs r1, r1, r0                         /* @4AE8 091a subs r1, r1, r0  */
L_4aea:
    ldr r0, [pc, #116]                      /* @4AEA 1d48 ldr r0, [pc, #116] literal 0x4B60 -> 0x20000622 */
L_4aec:
    strh r1, [r0, #2]                       /* @4AEC 4180 strh r1, [r0, #2]  */
L_4aee:
    b L_4af8                                /* @4AEE 03e0 b #6 -> 0x4AF8 */
L_4af0:
    lsls r0, r5, #3                         /* @4AF0 e800 lsls r0, r5, #3  */
L_4af2:
    subs r1, r0, r5                         /* @4AF2 411b subs r1, r0, r5  */
L_4af4:
    ldr r0, [pc, #104]                      /* @4AF4 1a48 ldr r0, [pc, #104] literal 0x4B60 -> 0x20000622 */
L_4af6:
    strh r1, [r0, #2]                       /* @4AF6 4180 strh r1, [r0, #2]  */
L_4af8:
    ldr r0, [pc, #92]                       /* @4AF8 1748 ldr r0, [pc, #92] literal 0x4B58 -> 0x20000008 */
L_4afa:
    ldrb r0, [r0, #2]                       /* @4AFA 8078 ldrb r0, [r0, #2]  */
L_4afc:
    mov lr, r0                              /* @4AFC 8646 mov lr, r0  */
L_4afe:
    cmp r0, r6                              /* @4AFE b042 cmp r0, r6  */
L_4b00:
    bls L_4b46                              /* @4B00 21d9 bls #66 -> 0x4B46 */
L_4b02:
    mov r1, r6                              /* @4B02 3146 mov r1, r6  */
L_4b04:
    b L_4b36                                /* @4B04 17e0 b #46 -> 0x4B36 */
L_4b06:
    movs r0, #0                             /* @4B06 0020 movs r0, #0  */
L_4b08:
    lsls r2, r1, #3                         /* @4B08 ca00 lsls r2, r1, #3  */
L_4b0a:
    subs r2, r2, r1                         /* @4B0A 521a subs r2, r2, r1  */
L_4b0c:
    mov r12, r2                             /* @4B0C 9446 mov r12, r2  */
L_4b0e:
    mov r2, r12                             /* @4B0E 6246 mov r2, r12  */
L_4b10:
    adds r3, r2, r0                         /* @4B10 1318 adds r3, r2, r0  */
L_4b12:
    ldr r2, [pc, #72]                       /* @4B12 124a ldr r2, [pc, #72] literal 0x4B5C -> 0x200008EC */
L_4b14:
    adds r2, r3, r2                         /* @4B14 9a18 adds r2, r3, r2  */
L_4b16:
    mov r3, r2                              /* @4B16 1346 mov r3, r2  */
L_4b18:
    adds r3, #192                           /* @4B18 c033 adds r3, #192  */
L_4b1a:
    ldrb r4, [r3, #24]                      /* @4B1A 1c7e ldrb r4, [r3, #24]  */
L_4b1c:
    lsls r4, r4, #28                        /* @4B1C 2407 lsls r4, r4, #28  */
L_4b1e:
    lsrs r4, r4, #28                        /* @4B1E 240f lsrs r4, r4, #28  */
L_4b20:
    strb r4, [r3, #24]                      /* @4B20 1c76 strb r4, [r3, #24]  */
L_4b22:
    movs r4, #0                             /* @4B22 0024 movs r4, #0  */
L_4b24:
    adds r2, #255                           /* @4B24 ff32 adds r2, #255  */
L_4b26:
    adds r2, #33                            /* @4B26 2132 adds r2, #33  */
L_4b28:
    strb r4, [r2]                           /* @4B28 1470 strb r4, [r2]  */
L_4b2a:
    adds r0, r0, #1                         /* @4B2A 401c adds r0, r0, #1  */
L_4b2c:
    uxtb r0, r0                             /* @4B2C c0b2 uxtb r0, r0  */
L_4b2e:
    cmp r0, #7                              /* @4B2E 0728 cmp r0, #7  */
L_4b30:
    blo L_4b0e                              /* @4B30 edd3 blo #-38 -> 0x4B0E */
L_4b32:
    adds r1, r1, #1                         /* @4B32 491c adds r1, r1, #1  */
L_4b34:
    uxtb r1, r1                             /* @4B34 c9b2 uxtb r1, r1  */
L_4b36:
    cmp lr, r1                              /* @4B36 8e45 cmp lr, r1  */
L_4b38:
    bhi L_4b06                              /* @4B38 e5d8 bhi #-54 -> 0x4B06 */
L_4b3a:
    mov r0, lr                              /* @4B3A 7046 mov r0, lr  */
L_4b3c:
    lsls r1, r0, #3                         /* @4B3C c100 lsls r1, r0, #3  */
L_4b3e:
    subs r1, r1, r0                         /* @4B3E 091a subs r1, r1, r0  */
L_4b40:
    ldr r0, [pc, #28]                       /* @4B40 0748 ldr r0, [pc, #28] literal 0x4B60 -> 0x20000622 */
L_4b42:
    strh r1, [r0, #4]                       /* @4B42 8180 strh r1, [r0, #4]  */
L_4b44:
    b L_4b4e                                /* @4B44 03e0 b #6 -> 0x4B4E */
L_4b46:
    lsls r0, r6, #3                         /* @4B46 f000 lsls r0, r6, #3  */
L_4b48:
    subs r1, r0, r6                         /* @4B48 811b subs r1, r0, r6  */
L_4b4a:
    ldr r0, [pc, #20]                       /* @4B4A 0548 ldr r0, [pc, #20] literal 0x4B60 -> 0x20000622 */
L_4b4c:
    strh r1, [r0, #4]                       /* @4B4C 8180 strh r1, [r0, #4]  */
L_4b4e:
    ldr r0, [pc, #8]                        /* @4B4E 0248 ldr r0, [pc, #8] literal 0x4B58 -> 0x20000008 */
L_4b50:
    strb r7, [r0]                           /* @4B50 0770 strb r7, [r0]  */
L_4b52:
    strb r5, [r0, #1]                       /* @4B52 4570 strb r5, [r0, #1]  */
L_4b54:
    strb r6, [r0, #2]                       /* @4B54 8670 strb r6, [r0, #2]  */
L_4b56:
    pop {r3, r4, r5, r6, r7, pc}            /* @4B56 f8bd pop {r3, r4, r5, r6, r7, pc}  */
L_4b58:
    .byte 0x08,0x00,0x00,0x20 /* @4B58 literal */
L_4b5c:
    .byte 0xec,0x08,0x00,0x20 /* @4B5C literal */
L_4b60:
    .byte 0x22,0x06,0x00,0x20 /* @4B60 literal */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @4B64 unclassified */

/* sub_4B70 */
L_4b70:
    push {r0, r1, r2, r4, r5, r6, r7, lr}   /* @4B70 f7b5 push {r0, r1, r2, r4, r5, r6, r7, lr}  */
L_4b72:
    mov lr, r0                              /* @4B72 8646 mov lr, r0  */
L_4b74:
    movs r2, #0                             /* @4B74 0022 movs r2, #0  */
L_4b76:
    b L_4bae                                /* @4B76 1ae0 b #52 -> 0x4BAE */
L_4b78:
    subs r0, #32                            /* @4B78 2038 subs r0, #32  */
L_4b7a:
    uxth r0, r0                             /* @4B7A 80b2 uxth r0, r0  */
L_4b7c:
    movs r1, #0                             /* @4B7C 0021 movs r1, #0  */
L_4b7e:
    movs r3, #14                            /* @4B7E 0e23 movs r3, #14  */
L_4b80:
    muls r3, r2, r3                         /* @4B80 5343 muls r3, r2, r3  */
L_4b82:
    lsrs r3, r3, #1                         /* @4B82 5b08 lsrs r3, r3, #1  */
L_4b84:
    mov r12, r3                             /* @4B84 9c46 mov r12, r3  */
L_4b86:
    ldr r7, [pc, #248]                      /* @4B86 3e4f ldr r7, [pc, #248] literal 0x4C80 -> 0x00006816 */
L_4b88:
    ldr r5, [pc, #248]                      /* @4B88 3e4d ldr r5, [pc, #248] literal 0x4C84 -> 0x200008EC */
L_4b8a:
    movs r3, #14                            /* @4B8A 0e23 movs r3, #14  */
L_4b8c:
    muls r0, r3, r0                         /* @4B8C 5843 muls r0, r3, r0  */
L_4b8e:
    adds r4, r0, r1                         /* @4B8E 4418 adds r4, r0, r1  */
L_4b90:
    mov r3, r12                             /* @4B90 6346 mov r3, r12  */
L_4b92:
    ldrb r6, [r7, r4]                       /* @4B92 3e5d ldrb r6, [r7, r4]  */
L_4b94:
    adds r3, r3, r1                         /* @4B94 5b18 adds r3, r3, r1  */
L_4b96:
    strb r6, [r5, r3]                       /* @4B96 ee54 strb r6, [r5, r3]  */
L_4b98:
    adds r4, r4, r7                         /* @4B98 e419 adds r4, r4, r7  */
L_4b9a:
    adds r3, r3, r5                         /* @4B9A 5b19 adds r3, r3, r5  */
L_4b9c:
    ldrb r4, [r4, #7]                       /* @4B9C e479 ldrb r4, [r4, #7]  */
L_4b9e:
    adds r3, #64                            /* @4B9E 4033 adds r3, #64  */
L_4ba0:
    strb r4, [r3, #8]                       /* @4BA0 1c72 strb r4, [r3, #8]  */
L_4ba2:
    adds r1, r1, #1                         /* @4BA2 491c adds r1, r1, #1  */
L_4ba4:
    uxtb r1, r1                             /* @4BA4 c9b2 uxtb r1, r1  */
L_4ba6:
    cmp r1, #7                              /* @4BA6 0729 cmp r1, #7  */
L_4ba8:
    blo L_4b8e                              /* @4BA8 f1d3 blo #-30 -> 0x4B8E */
L_4baa:
    adds r2, r2, #1                         /* @4BAA 521c adds r2, r2, #1  */
L_4bac:
    uxtb r2, r2                             /* @4BAC d2b2 uxtb r2, r2  */
L_4bae:
    mov r0, lr                              /* @4BAE 7046 mov r0, lr  */
L_4bb0:
    ldrb r0, [r0, r2]                       /* @4BB0 805c ldrb r0, [r0, r2]  */
L_4bb2:
    cmp r0, #0                              /* @4BB2 0028 cmp r0, #0  */
L_4bb4:
    bne L_4b78                              /* @4BB4 e0d1 bne #-64 -> 0x4B78 */
L_4bb6:
    movs r2, #0                             /* @4BB6 0022 movs r2, #0  */
L_4bb8:
    b L_4c18                                /* @4BB8 2ee0 b #92 -> 0x4C18 */
L_4bba:
    subs r0, #32                            /* @4BBA 2038 subs r0, #32  */
L_4bbc:
    uxth r1, r0                             /* @4BBC 81b2 uxth r1, r0  */
L_4bbe:
    movs r0, #0                             /* @4BBE 0020 movs r0, #0  */
L_4bc0:
    movs r3, #14                            /* @4BC0 0e23 movs r3, #14  */
L_4bc2:
    muls r3, r2, r3                         /* @4BC2 5343 muls r3, r2, r3  */
L_4bc4:
    lsrs r3, r3, #1                         /* @4BC4 5b08 lsrs r3, r3, #1  */
L_4bc6:
    mov lr, r3                              /* @4BC6 9e46 mov lr, r3  */
L_4bc8:
    movs r3, #14                            /* @4BC8 0e23 movs r3, #14  */
L_4bca:
    muls r1, r3, r1                         /* @4BCA 5943 muls r1, r3, r1  */
L_4bcc:
    mov r12, r1                             /* @4BCC 8c46 mov r12, r1  */
L_4bce:
    mov r1, lr                              /* @4BCE 7146 mov r1, lr  */
L_4bd0:
    adds r3, r1, r0                         /* @4BD0 0b18 adds r3, r1, r0  */
L_4bd2:
    ldr r1, [pc, #176]                      /* @4BD2 2c49 ldr r1, [pc, #176] literal 0x4C84 -> 0x200008EC */
L_4bd4:
    adds r1, r3, r1                         /* @4BD4 5918 adds r1, r3, r1  */
L_4bd6:
    mov r3, r1                              /* @4BD6 0b46 mov r3, r1  */
L_4bd8:
    adds r3, #64                            /* @4BD8 4033 adds r3, #64  */
L_4bda:
    ldrb r4, [r3, #8]                       /* @4BDA 1c7a ldrb r4, [r3, #8]  */
L_4bdc:
    lsls r6, r4, #26                        /* @4BDC a606 lsls r6, r4, #26  */
L_4bde:
    lsrs r6, r6, #26                        /* @4BDE b60e lsrs r6, r6, #26  */
L_4be0:
    mov r4, r12                             /* @4BE0 6446 mov r4, r12  */
L_4be2:
    ldr r5, [pc, #156]                      /* @4BE2 274d ldr r5, [pc, #156] literal 0x4C80 -> 0x00006816 */
L_4be4:
    adds r4, r4, r0                         /* @4BE4 2418 adds r4, r4, r0  */
L_4be6:
    ldrb r5, [r5, r4]                       /* @4BE6 2d5d ldrb r5, [r5, r4]  */
L_4be8:
    lsls r7, r5, #6                         /* @4BE8 af01 lsls r7, r5, #6  */
L_4bea:
    orrs r6, r7                             /* @4BEA 3e43 orrs r6, r7  */
L_4bec:
    strb r6, [r3, #8]                       /* @4BEC 1e72 strb r6, [r3, #8]  */
L_4bee:
    ldr r3, [pc, #144]                      /* @4BEE 244b ldr r3, [pc, #144] literal 0x4C80 -> 0x00006816 */
L_4bf0:
    lsrs r5, r5, #2                         /* @4BF0 ad08 lsrs r5, r5, #2  */
L_4bf2:
    adds r3, r4, r3                         /* @4BF2 e318 adds r3, r4, r3  */
L_4bf4:
    ldrb r3, [r3, #7]                       /* @4BF4 db79 ldrb r3, [r3, #7]  */
L_4bf6:
    lsls r4, r3, #6                         /* @4BF6 9c01 lsls r4, r3, #6  */
L_4bf8:
    orrs r5, r4                             /* @4BF8 2543 orrs r5, r4  */
L_4bfa:
    movs r4, #144                           /* @4BFA 9024 movs r4, #144  */
L_4bfc:
    strb r5, [r4, r1]                       /* @4BFC 6554 strb r5, [r4, r1]  */
L_4bfe:
    adds r1, #192                           /* @4BFE c031 adds r1, #192  */
L_4c00:
    ldrb r4, [r1, #24]                      /* @4C00 0c7e ldrb r4, [r1, #24]  */
L_4c02:
    lsrs r4, r4, #4                         /* @4C02 2409 lsrs r4, r4, #4  */
L_4c04:
    lsls r4, r4, #4                         /* @4C04 2401 lsls r4, r4, #4  */
L_4c06:
    lsrs r3, r3, #2                         /* @4C06 9b08 lsrs r3, r3, #2  */
L_4c08:
    orrs r4, r3                             /* @4C08 1c43 orrs r4, r3  */
L_4c0a:
    strb r4, [r1, #24]                      /* @4C0A 0c76 strb r4, [r1, #24]  */
L_4c0c:
    adds r0, r0, #1                         /* @4C0C 401c adds r0, r0, #1  */
L_4c0e:
    uxtb r0, r0                             /* @4C0E c0b2 uxtb r0, r0  */
L_4c10:
    cmp r0, #7                              /* @4C10 0728 cmp r0, #7  */
L_4c12:
    blo L_4bce                              /* @4C12 dcd3 blo #-72 -> 0x4BCE */
L_4c14:
    adds r2, r2, #1                         /* @4C14 521c adds r2, r2, #1  */
L_4c16:
    uxtb r2, r2                             /* @4C16 d2b2 uxtb r2, r2  */
L_4c18:
    ldr r0, [sp, #4]                        /* @4C18 0198 ldr r0, [sp, #4]  */
L_4c1a:
    ldrb r0, [r0, r2]                       /* @4C1A 805c ldrb r0, [r0, r2]  */
L_4c1c:
    cmp r0, #0                              /* @4C1C 0028 cmp r0, #0  */
L_4c1e:
    bne L_4bba                              /* @4C1E ccd1 bne #-104 -> 0x4BBA */
L_4c20:
    movs r1, #0                             /* @4C20 0021 movs r1, #0  */
L_4c22:
    b L_4c76                                /* @4C22 28e0 b #80 -> 0x4C76 */
L_4c24:
    subs r0, #32                            /* @4C24 2038 subs r0, #32  */
L_4c26:
    uxth r2, r0                             /* @4C26 82b2 uxth r2, r0  */
L_4c28:
    movs r0, #0                             /* @4C28 0020 movs r0, #0  */
L_4c2a:
    movs r3, #14                            /* @4C2A 0e23 movs r3, #14  */
L_4c2c:
    muls r3, r1, r3                         /* @4C2C 4b43 muls r3, r1, r3  */
L_4c2e:
    lsrs r3, r3, #1                         /* @4C2E 5b08 lsrs r3, r3, #1  */
L_4c30:
    mov lr, r3                              /* @4C30 9e46 mov lr, r3  */
L_4c32:
    movs r3, #14                            /* @4C32 0e23 movs r3, #14  */
L_4c34:
    muls r2, r3, r2                         /* @4C34 5a43 muls r2, r3, r2  */
L_4c36:
    mov r12, r2                             /* @4C36 9446 mov r12, r2  */
L_4c38:
    mov r2, lr                              /* @4C38 7246 mov r2, lr  */
L_4c3a:
    adds r3, r2, r0                         /* @4C3A 1318 adds r3, r2, r0  */
L_4c3c:
    ldr r2, [pc, #68]                       /* @4C3C 114a ldr r2, [pc, #68] literal 0x4C84 -> 0x200008EC */
L_4c3e:
    adds r2, r3, r2                         /* @4C3E 9a18 adds r2, r3, r2  */
L_4c40:
    mov r3, r2                              /* @4C40 1346 mov r3, r2  */
L_4c42:
    adds r3, #192                           /* @4C42 c033 adds r3, #192  */
L_4c44:
    ldrb r4, [r3, #24]                      /* @4C44 1c7e ldrb r4, [r3, #24]  */
L_4c46:
    lsls r6, r4, #28                        /* @4C46 2607 lsls r6, r4, #28  */
L_4c48:
    lsrs r6, r6, #28                        /* @4C48 360f lsrs r6, r6, #28  */
L_4c4a:
    mov r4, r12                             /* @4C4A 6446 mov r4, r12  */
L_4c4c:
    ldr r5, [pc, #48]                       /* @4C4C 0c4d ldr r5, [pc, #48] literal 0x4C80 -> 0x00006816 */
L_4c4e:
    adds r4, r4, r0                         /* @4C4E 2418 adds r4, r4, r0  */
L_4c50:
    ldrb r5, [r5, r4]                       /* @4C50 2d5d ldrb r5, [r5, r4]  */
L_4c52:
    lsls r7, r5, #4                         /* @4C52 2f01 lsls r7, r5, #4  */
L_4c54:
    orrs r6, r7                             /* @4C54 3e43 orrs r6, r7  */
L_4c56:
    strb r6, [r3, #24]                      /* @4C56 1e76 strb r6, [r3, #24]  */
L_4c58:
    ldr r3, [pc, #36]                       /* @4C58 094b ldr r3, [pc, #36] literal 0x4C80 -> 0x00006816 */
L_4c5a:
    lsrs r5, r5, #4                         /* @4C5A 2d09 lsrs r5, r5, #4  */
L_4c5c:
    adds r3, r4, r3                         /* @4C5C e318 adds r3, r4, r3  */
L_4c5e:
    ldrb r3, [r3, #7]                       /* @4C5E db79 ldrb r3, [r3, #7]  */
L_4c60:
    adds r2, #255                           /* @4C60 ff32 adds r2, #255  */
L_4c62:
    lsls r3, r3, #4                         /* @4C62 1b01 lsls r3, r3, #4  */
L_4c64:
    orrs r5, r3                             /* @4C64 1d43 orrs r5, r3  */
L_4c66:
    adds r2, #33                            /* @4C66 2132 adds r2, #33  */
L_4c68:
    strb r5, [r2]                           /* @4C68 1570 strb r5, [r2]  */
L_4c6a:
    adds r0, r0, #1                         /* @4C6A 401c adds r0, r0, #1  */
L_4c6c:
    uxtb r0, r0                             /* @4C6C c0b2 uxtb r0, r0  */
L_4c6e:
    cmp r0, #7                              /* @4C6E 0728 cmp r0, #7  */
L_4c70:
    blo L_4c38                              /* @4C70 e2d3 blo #-60 -> 0x4C38 */
L_4c72:
    adds r1, r1, #1                         /* @4C72 491c adds r1, r1, #1  */
L_4c74:
    uxtb r1, r1                             /* @4C74 c9b2 uxtb r1, r1  */
L_4c76:
    ldr r0, [sp, #8]                        /* @4C76 0298 ldr r0, [sp, #8]  */
L_4c78:
    ldrb r0, [r0, r1]                       /* @4C78 405c ldrb r0, [r0, r1]  */
L_4c7a:
    cmp r0, #0                              /* @4C7A 0028 cmp r0, #0  */
L_4c7c:
    bne L_4c24                              /* @4C7C d2d1 bne #-92 -> 0x4C24 */
L_4c7e:
    pop {r1, r2, r3, r4, r5, r6, r7, pc}    /* @4C7E febd pop {r1, r2, r3, r4, r5, r6, r7, pc}  */
L_4c80:
    .byte 0x16,0x68,0x00,0x00 /* @4C80 literal */
L_4c84:
    .byte 0xec,0x08,0x00,0x20 /* @4C84 literal */
    .byte 0x00,0x00,0x00,0x00 /* @4C88 unclassified */

/* sub_4C8C */
L_4c8c:
    push {r4, r5, r6, lr}                   /* @4C8C 70b5 push {r4, r5, r6, lr}  */
L_4c8e:
    sub sp, #40                             /* @4C8E 8ab0 sub sp, #40  */
L_4c90:
    movs r4, #0                             /* @4C90 0024 movs r4, #0  */
L_4c92:
    add r0, sp, #32                         /* @4C92 08a8 add r0, sp, #32  */
L_4c94:
    strb r4, [r0, #4]                       /* @4C94 0471 strb r4, [r0, #4]  */
L_4c96:
    movs r5, #1                             /* @4C96 0125 movs r5, #1  */
L_4c98:
    strb r5, [r0, #5]                       /* @4C98 4571 strb r5, [r0, #5]  */
L_4c9a:
    movs r1, #12                            /* @4C9A 0c21 movs r1, #12  */
L_4c9c:
    movs r0, #0                             /* @4C9C 0020 movs r0, #0  */
L_4c9e:
    bl L_1430                               /* @4C9E fcf7c7fb bl #-14450 -> sub_1430 */
L_4ca2:
    add r2, sp, #32                         /* @4CA2 08aa add r2, sp, #32  */
L_4ca4:
    movs r1, #12                            /* @4CA4 0c21 movs r1, #12  */
L_4ca6:
    movs r0, #0                             /* @4CA6 0020 movs r0, #0  */
L_4ca8:
    bl L_137c                               /* @4CA8 fcf768fb bl #-14640 -> sub_137C */
L_4cac:
    add r0, sp, #32                         /* @4CAC 08a8 add r0, sp, #32  */
L_4cae:
    strb r4, [r0]                           /* @4CAE 0470 strb r4, [r0]  */
L_4cb0:
    strb r4, [r0, #1]                       /* @4CB0 4470 strb r4, [r0, #1]  */
L_4cb2:
    strb r5, [r0, #2]                       /* @4CB2 8570 strb r5, [r0, #2]  */
L_4cb4:
    strb r4, [r0, #3]                       /* @4CB4 c470 strb r4, [r0, #3]  */
L_4cb6:
    strb r5, [r0, #4]                       /* @4CB6 0571 strb r5, [r0, #4]  */
L_4cb8:
    strb r5, [r0, #5]                       /* @4CB8 4571 strb r5, [r0, #5]  */
L_4cba:
    add r2, sp, #32                         /* @4CBA 08aa add r2, sp, #32  */
L_4cbc:
    movs r1, #0                             /* @4CBC 0021 movs r1, #0  */
L_4cbe:
    movs r0, #192                           /* @4CBE c020 movs r0, #192  */
L_4cc0:
    bl L_137c                               /* @4CC0 fcf75cfb bl #-14664 -> sub_137C */
L_4cc4:
    add r2, sp, #32                         /* @4CC4 08aa add r2, sp, #32  */
L_4cc6:
    movs r1, #1                             /* @4CC6 0121 movs r1, #1  */
L_4cc8:
    movs r0, #192                           /* @4CC8 c020 movs r0, #192  */
L_4cca:
    bl L_137c                               /* @4CCA fcf757fb bl #-14674 -> sub_137C */
L_4cce:
    movs r2, #1                             /* @4CCE 0122 movs r2, #1  */
L_4cd0:
    movs r1, #0                             /* @4CD0 0021 movs r1, #0  */
L_4cd2:
    movs r0, #192                           /* @4CD2 c020 movs r0, #192  */
L_4cd4:
    bl L_1408                               /* @4CD4 fcf798fb bl #-14544 -> sub_1408 */
L_4cd8:
    movs r2, #1                             /* @4CD8 0122 movs r2, #1  */
L_4cda:
    mov r1, r2                              /* @4CDA 1146 mov r1, r2  */
L_4cdc:
    movs r0, #192                           /* @4CDC c020 movs r0, #192  */
L_4cde:
    bl L_1408                               /* @4CDE fcf793fb bl #-14554 -> sub_1408 */
L_4ce2:
    bl L_1924                               /* @4CE2 fcf71ffe bl #-13250 -> sub_1924 */
L_4ce6:
    str r0, [sp, #20]                       /* @4CE6 0590 str r0, [sp, #20]  */
L_4ce8:
    ldr r0, [pc, #108]                      /* @4CE8 1b48 ldr r0, [pc, #108] literal 0x4D58 -> 0x00061A80 */
L_4cea:
    str r0, [sp, #24]                       /* @4CEA 0690 str r0, [sp, #24]  */
L_4cec:
    movs r1, #64                            /* @4CEC 4021 movs r1, #64  */
L_4cee:
    mov r0, sp                              /* @4CEE 6846 mov r0, sp  */
L_4cf0:
    strb r1, [r0, #28]                      /* @4CF0 0177 strb r1, [r0, #28]  */
L_4cf2:
    movs r1, #85                            /* @4CF2 5521 movs r1, #85  */
L_4cf4:
    strb r1, [r0, #29]                      /* @4CF4 4177 strb r1, [r0, #29]  */
L_4cf6:
    strb r4, [r0, #30]                      /* @4CF6 8477 strb r4, [r0, #30]  */
L_4cf8:
    ldr r6, [pc, #96]                       /* @4CF8 184e ldr r6, [pc, #96] literal 0x4D5C -> 0x40000400 */
L_4cfa:
    add r1, sp, #20                         /* @4CFA 05a9 add r1, sp, #20  */
L_4cfc:
    mov r0, r6                              /* @4CFC 3046 mov r0, r6  */
L_4cfe:
    bl L_1578                               /* @4CFE fcf73bfc bl #-14218 -> sub_1578 */
L_4d02:
    movs r1, #6                             /* @4D02 0621 movs r1, #6  */
L_4d04:
    mov r0, r6                              /* @4D04 3046 mov r0, r6  */
L_4d06:
    bl L_160e                               /* @4D06 fcf782fc bl #-14076 -> sub_160E */
L_4d0a:
    mov r0, sp                              /* @4D0A 6846 mov r0, sp  */
L_4d0c:
    strb r4, [r0]                           /* @4D0C 0470 strb r4, [r0]  */
L_4d0e:
    strb r4, [r0, #5]                       /* @4D0E 4471 strb r4, [r0, #5]  */
L_4d10:
    movs r1, #7                             /* @4D10 0721 movs r1, #7  */
L_4d12:
    strb r1, [r0, #3]                       /* @4D12 c170 strb r1, [r0, #3]  */
L_4d14:
    strb r5, [r0, #6]                       /* @4D14 8571 strb r5, [r0, #6]  */
L_4d16:
    strb r4, [r0, #4]                       /* @4D16 0471 strb r4, [r0, #4]  */
L_4d18:
    strb r4, [r0, #2]                       /* @4D18 8470 strb r4, [r0, #2]  */
L_4d1a:
    strb r4, [r0, #1]                       /* @4D1A 4470 strb r4, [r0, #1]  */
L_4d1c:
    str r4, [sp, #8]                        /* @4D1C 0294 str r4, [sp, #8]  */
L_4d1e:
    str r4, [sp, #12]                       /* @4D1E 0394 str r4, [sp, #12]  */
L_4d20:
    str r4, [sp, #16]                       /* @4D20 0494 str r4, [sp, #16]  */
L_4d22:
    mov r1, sp                              /* @4D22 6946 mov r1, sp  */
L_4d24:
    movs r0, #2                             /* @4D24 0220 movs r0, #2  */
L_4d26:
    bl L_0ba0                               /* @4D26 fbf73bff bl #-16778 -> sub_0BA0 */
L_4d2a:
    ldr r4, [pc, #52]                       /* @4D2A 0d4c ldr r4, [pc, #52] literal 0x4D60 -> 0x0000FE89 */
L_4d2c:
    movs r0, #2                             /* @4D2C 0220 movs r0, #2  */
L_4d2e:
    mov r1, r4                              /* @4D2E 2146 mov r1, r4  */
L_4d30:
    bl L_08ec                               /* @4D30 fbf7dcfd bl #-17480 -> sub_08EC */
L_4d34:
    mov r1, r4                              /* @4D34 2146 mov r1, r4  */
L_4d36:
    movs r0, #2                             /* @4D36 0220 movs r0, #2  */
L_4d38:
    bl L_0908                               /* @4D38 fbf7e6fd bl #-17460 -> sub_0908 */
L_4d3c:
    movs r1, #0                             /* @4D3C 0021 movs r1, #0  */
L_4d3e:
    movs r0, #2                             /* @4D3E 0220 movs r0, #2  */
L_4d40:
    bl L_08c0                               /* @4D40 fbf7befd bl #-17540 -> sub_08C0 */
L_4d44:
    movs r0, #2                             /* @4D44 0220 movs r0, #2  */
L_4d46:
    bl L_0b84                               /* @4D46 fbf71dff bl #-16838 -> sub_0B84 */
L_4d4a:
    movs r2, #1                             /* @4D4A 0122 movs r2, #1  */
L_4d4c:
    mov r1, r2                              /* @4D4C 1146 mov r1, r2  */
L_4d4e:
    movs r0, #16                            /* @4D4E 1020 movs r0, #16  */
L_4d50:
    bl L_0fec                               /* @4D50 fcf74cf9 bl #-15720 -> sub_0FEC */
L_4d54:
    add sp, #40                             /* @4D54 0ab0 add sp, #40  */
L_4d56:
    pop {r4, r5, r6, pc}                    /* @4D56 70bd pop {r4, r5, r6, pc}  */
L_4d58:
    .byte 0x80,0x1a,0x06,0x00 /* @4D58 literal */
L_4d5c:
    .byte 0x00,0x04,0x00,0x40 /* @4D5C literal */
L_4d60:
    .byte 0x89,0xfe,0x00,0x00 /* @4D60 literal */

/* sub_4D64 */
L_4d64:
    push {r4, lr}                           /* @4D64 10b5 push {r4, lr}  */
L_4d66:
    mov r4, r0                              /* @4D66 0446 mov r4, r0  */
L_4d68:
    adds r1, #176                           /* @4D68 b031 adds r1, #176  */
L_4d6a:
    uxtb r0, r1                             /* @4D6A c8b2 uxtb r0, r1  */
L_4d6c:
    bl L_4d84                               /* @4D6C 00f00af8 bl #20 -> sub_4D84 */
L_4d70:
    lsls r0, r4, #28                        /* @4D70 2007 lsls r0, r4, #28  */
L_4d72:
    lsrs r0, r0, #28                        /* @4D72 000f lsrs r0, r0, #28  */
L_4d74:
    adds r0, #12                            /* @4D74 0c30 adds r0, #12  */
L_4d76:
    bl L_4d84                               /* @4D76 00f005f8 bl #10 -> sub_4D84 */
L_4d7a:
    lsrs r0, r4, #4                         /* @4D7A 2009 lsrs r0, r4, #4  */
L_4d7c:
    adds r0, #17                            /* @4D7C 1130 adds r0, #17  */
L_4d7e:
    bl L_4d84                               /* @4D7E 00f001f8 bl #2 -> sub_4D84 */
L_4d82:
    pop {r4, pc}                            /* @4D82 10bd pop {r4, pc}  */

/* sub_4D84 */
L_4d84:
    push {r0, lr}                           /* @4D84 01b5 push {r0, lr}  */
L_4d86:
    movs r2, #1                             /* @4D86 0122 movs r2, #1  */
L_4d88:
    mov r1, sp                              /* @4D88 6946 mov r1, sp  */
L_4d8a:
    movs r0, #0                             /* @4D8A 0020 movs r0, #0  */
L_4d8c:
    bl L_14b8                               /* @4D8C fcf794fb bl #-14552 -> sub_14B8 */
L_4d90:
    pop {r3, pc}                            /* @4D90 08bd pop {r3, pc}  */
    .byte 0x00,0x00 /* @4D92 unclassified */

/* sub_4D94 */
L_4d94:
    push {r4, lr}                           /* @4D94 10b5 push {r4, lr}  */
L_4d96:
    adr r2, L_4db0                          /* @4D96 06a2 adr r2, #24 address 0x4DB0 -> 0x00004DB0 'Fail!!!' */
L_4d98:
    adr r1, L_4db8                          /* @4D98 07a1 adr r1, #28 address 0x4DB8 -> 0x00004DB8 ' Origin' */
L_4d9a:
    adr r0, L_4dc0                          /* @4D9A 09a0 adr r0, #36 address 0x4DC0 -> 0x00004DC0 'Back to' */
L_4d9c:
    bl L_4884                               /* @4D9C fff772fd bl #-1308 -> display_three_lines */
L_4da0:
    movs r0, #1                             /* @4DA0 0120 movs r0, #1  */
L_4da2:
    bl L_3f28                               /* @4DA2 fff7c1f8 bl #-3710 -> sub_3F28 */
L_4da6:
    movs r0, #125                           /* @4DA6 7d20 movs r0, #125  */
L_4da8:
    lsls r0, r0, #3                         /* @4DA8 c000 lsls r0, r0, #3  */
L_4daa:
    bl L_2de8                               /* @4DAA fef71df8 bl #-8134 -> delay_ms_candidate */
L_4dae:
    pop {r4, pc}                            /* @4DAE 10bd pop {r4, pc}  */
L_4db0:
    .byte 0x46,0x61,0x69,0x6c,0x21,0x21,0x21,0x00 /* @4DB0 unclassified */
L_4db8:
    .byte 0x20,0x4f,0x72,0x69,0x67,0x69,0x6e,0x00 /* @4DB8 unclassified */
L_4dc0:
    .byte 0x42,0x61,0x63,0x6b,0x20,0x74,0x6f,0x00 /* @4DC0 unclassified */

/* origin_return_candidate */
L_4dc8:
    push {r0, r4, r5, r6, r7, lr}           /* @4DC8 f1b5 push {r0, r4, r5, r6, r7, lr}  */
L_4dca:
    sub sp, #32                             /* @4DCA 88b0 sub sp, #32  */
L_4dcc:
    movs r0, #0                             /* @4DCC 0020 movs r0, #0  */
L_4dce:
    str r0, [sp, #28]                       /* @4DCE 0790 str r0, [sp, #28]  */
L_4dd0:
    str r0, [sp, #24]                       /* @4DD0 0690 str r0, [sp, #24]  */
L_4dd2:
    str r0, [sp, #20]                       /* @4DD2 0590 str r0, [sp, #20]  */
L_4dd4:
    str r0, [sp, #16]                       /* @4DD4 0490 str r0, [sp, #16]  */
L_4dd6:
    str r0, [sp, #12]                       /* @4DD6 0390 str r0, [sp, #12]  */
L_4dd8:
    str r0, [sp, #8]                        /* @4DD8 0290 str r0, [sp, #8]  */
L_4dda:
    str r0, [sp, #4]                        /* @4DDA 0190 str r0, [sp, #4]  */
L_4ddc:
    ldr r0, [pc, #508]                      /* @4DDC 7f48 ldr r0, [pc, #508] literal 0x4FDC -> 0x20000013 */
L_4dde:
    ldrb r0, [r0]                           /* @4DDE 0078 ldrb r0, [r0]  */
L_4de0:
    cmp r0, #1                              /* @4DE0 0128 cmp r0, #1  */
L_4de2:
    beq L_4e20                              /* @4DE2 1dd0 beq #58 -> 0x4E20 */
L_4de4:
    ldr r0, [pc, #504]                      /* @4DE4 7e48 ldr r0, [pc, #504] literal 0x4FE0 -> 0x2000072C */
L_4de6:
    ldrb r0, [r0, #9]                       /* @4DE6 407a ldrb r0, [r0, #9]  */
L_4de8:
    cmp r0, #0                              /* @4DE8 0028 cmp r0, #0  */
L_4dea:
    beq L_4ece                              /* @4DEA 70d0 beq #224 -> 0x4ECE */
L_4dec:
    ldr r6, [pc, #500]                      /* @4DEC 7d4e ldr r6, [pc, #500] literal 0x4FE4 -> 0x200005F6 */
L_4dee:
    ldrb r0, [r6]                           /* @4DEE 3078 ldrb r0, [r6]  */
L_4df0:
    cmp r0, #0                              /* @4DF0 0028 cmp r0, #0  */
L_4df2:
    beq L_4dea                              /* @4DF2 fad0 beq #-12 -> 0x4DEA */
L_4df4:
    ldrh r0, [r6, #2]                       /* @4DF4 7088 ldrh r0, [r6, #2]  */
L_4df6:
    ldr r1, [pc, #496]                      /* @4DF6 7c49 ldr r1, [pc, #496] literal 0x4FE8 -> 0x0000FFFF */
L_4df8:
    cmp r0, r1                              /* @4DF8 8842 cmp r0, r1  */
L_4dfa:
    beq L_4dea                              /* @4DFA f6d0 beq #-20 -> 0x4DEA */
L_4dfc:
    ldrh r1, [r6, #2]                       /* @4DFC 7188 ldrh r1, [r6, #2]  */
L_4dfe:
    ldr r4, [pc, #492]                      /* @4DFE 7b4c ldr r4, [pc, #492] literal 0x4FEC -> 0x20000680 */
L_4e00:
    ldr r0, [r4, #16]                       /* @4E00 2069 ldr r0, [r4, #16]  */
L_4e02:
    ldr r2, [r4, #20]                       /* @4E02 6269 ldr r2, [r4, #20]  */
L_4e04:
    subs r0, r1, r0                         /* @4E04 081a subs r0, r1, r0  */
L_4e06:
    sxth r0, r0                             /* @4E06 00b2 sxth r0, r0  */
L_4e08:
    str r0, [sp, #28]                       /* @4E08 0790 str r0, [sp, #28]  */
L_4e0a:
    ldr r0, [sp, #28]                       /* @4E0A 0798 ldr r0, [sp, #28]  */
L_4e0c:
    movs r5, #1                             /* @4E0C 0125 movs r5, #1  */
L_4e0e:
    cmp r0, #100                            /* @4E0E 6428 cmp r0, #100  */
L_4e10:
    ble L_4e4e                              /* @4E10 1ddd ble #58 -> 0x4E4E */
L_4e12:
    ldrb r0, [r6]                           /* @4E12 3078 ldrb r0, [r6]  */
L_4e14:
    cmp r0, #1                              /* @4E14 0128 cmp r0, #1  */
L_4e16:
    beq L_4e26                              /* @4E16 06d0 beq #12 -> 0x4E26 */
L_4e18:
    ldrb r0, [r6]                           /* @4E18 3078 ldrb r0, [r6]  */
L_4e1a:
    cmp r0, #2                              /* @4E1A 0228 cmp r0, #2  */
L_4e1c:
    beq L_4e40                              /* @4E1C 10d0 beq #32 -> 0x4E40 */
L_4e1e:
    b L_4e8e                                /* @4E1E 36e0 b #108 -> 0x4E8E */
L_4e20:
    movs r0, #0                             /* @4E20 0020 movs r0, #0  */
L_4e22:
    add sp, #36                             /* @4E22 09b0 add sp, #36  */
L_4e24:
    pop {r4, r5, r6, r7, pc}                /* @4E24 f0bd pop {r4, r5, r6, r7, pc}  */
L_4e26:
    ldrb r0, [r6, #6]                       /* @4E26 b079 ldrb r0, [r6, #6]  */
L_4e28:
    cmp r0, #0                              /* @4E28 0028 cmp r0, #0  */
L_4e2a:
    beq L_4e38                              /* @4E2A 05d0 beq #10 -> 0x4E38 */
L_4e2c:
    movs r1, #1                             /* @4E2C 0121 movs r1, #1  */
L_4e2e:
    ldr r0, [sp, #28]                       /* @4E2E 0798 ldr r0, [sp, #28]  */
L_4e30:
    lsls r1, r1, #16                        /* @4E30 0904 lsls r1, r1, #16  */
L_4e32:
    subs r0, r1, r0                         /* @4E32 081a subs r0, r1, r0  */
L_4e34:
    str r0, [sp, #16]                       /* @4E34 0490 str r0, [sp, #16]  */
L_4e36:
    b L_4e8e                                /* @4E36 2ae0 b #84 -> 0x4E8E */
L_4e38:
    ldr r0, [sp, #28]                       /* @4E38 0798 ldr r0, [sp, #28]  */
L_4e3a:
    str r0, [sp, #16]                       /* @4E3A 0490 str r0, [sp, #16]  */
L_4e3c:
    str r5, [sp, #20]                       /* @4E3C 0595 str r5, [sp, #20]  */
L_4e3e:
    b L_4e8e                                /* @4E3E 26e0 b #76 -> 0x4E8E */
L_4e40:
    ldrb r0, [r6, #6]                       /* @4E40 b079 ldrb r0, [r6, #6]  */
L_4e42:
    cmp r0, #0                              /* @4E42 0028 cmp r0, #0  */
L_4e44:
    bne L_4e48                              /* @4E44 00d1 bne #0 -> 0x4E48 */
L_4e46:
    str r5, [sp, #20]                       /* @4E46 0595 str r5, [sp, #20]  */
L_4e48:
    ldr r0, [sp, #28]                       /* @4E48 0798 ldr r0, [sp, #28]  */
L_4e4a:
    str r0, [sp, #16]                       /* @4E4A 0490 str r0, [sp, #16]  */
L_4e4c:
    b L_4e8e                                /* @4E4C 1fe0 b #62 -> 0x4E8E */
L_4e4e:
    ldr r0, [sp, #28]                       /* @4E4E 0798 ldr r0, [sp, #28]  */
L_4e50:
    movs r1, #100                           /* @4E50 6421 movs r1, #100  */
L_4e52:
    cmn r0, r1                              /* @4E52 c842 cmn r0, r1  */
L_4e54:
    bge L_4e8e                              /* @4E54 1bda bge #54 -> 0x4E8E */
L_4e56:
    ldrb r0, [r6]                           /* @4E56 3078 ldrb r0, [r6]  */
L_4e58:
    cmp r0, #1                              /* @4E58 0128 cmp r0, #1  */
L_4e5a:
    beq L_4e64                              /* @4E5A 03d0 beq #6 -> 0x4E64 */
L_4e5c:
    ldrb r0, [r6]                           /* @4E5C 3078 ldrb r0, [r6]  */
L_4e5e:
    cmp r0, #2                              /* @4E5E 0228 cmp r0, #2  */
L_4e60:
    beq L_4e80                              /* @4E60 0ed0 beq #28 -> 0x4E80 */
L_4e62:
    b L_4e8e                                /* @4E62 14e0 b #40 -> 0x4E8E */
L_4e64:
    ldrb r0, [r6, #6]                       /* @4E64 b079 ldrb r0, [r6, #6]  */
L_4e66:
    cmp r0, #0                              /* @4E66 0028 cmp r0, #0  */
L_4e68:
    beq L_4e72                              /* @4E68 03d0 beq #6 -> 0x4E72 */
L_4e6a:
    ldr r0, [sp, #28]                       /* @4E6A 0798 ldr r0, [sp, #28]  */
L_4e6c:
    rsbs r0, r0, #0                         /* @4E6C 4042 rsbs r0, r0, #0  */
L_4e6e:
    str r0, [sp, #16]                       /* @4E6E 0490 str r0, [sp, #16]  */
L_4e70:
    b L_4e8e                                /* @4E70 0de0 b #26 -> 0x4E8E */
L_4e72:
    ldr r1, [pc, #372]                      /* @4E72 5d49 ldr r1, [pc, #372] literal 0x4FE8 -> 0x0000FFFF */
L_4e74:
    ldr r0, [sp, #28]                       /* @4E74 0798 ldr r0, [sp, #28]  */
L_4e76:
    subs r1, r1, #1                         /* @4E76 491e subs r1, r1, #1  */
L_4e78:
    adds r0, r0, r1                         /* @4E78 4018 adds r0, r0, r1  */
L_4e7a:
    str r0, [sp, #16]                       /* @4E7A 0490 str r0, [sp, #16]  */
L_4e7c:
    str r5, [sp, #20]                       /* @4E7C 0595 str r5, [sp, #20]  */
L_4e7e:
    b L_4e8e                                /* @4E7E 06e0 b #12 -> 0x4E8E */
L_4e80:
    ldrb r0, [r6, #6]                       /* @4E80 b079 ldrb r0, [r6, #6]  */
L_4e82:
    cmp r0, #0                              /* @4E82 0028 cmp r0, #0  */
L_4e84:
    beq L_4e88                              /* @4E84 00d0 beq #0 -> 0x4E88 */
L_4e86:
    str r5, [sp, #20]                       /* @4E86 0595 str r5, [sp, #20]  */
L_4e88:
    ldr r0, [sp, #28]                       /* @4E88 0798 ldr r0, [sp, #28]  */
L_4e8a:
    rsbs r0, r0, #0                         /* @4E8A 4042 rsbs r0, r0, #0  */
L_4e8c:
    str r0, [sp, #16]                       /* @4E8C 0490 str r0, [sp, #16]  */
L_4e8e:
    ldr r0, [pc, #352]                      /* @4E8E 5848 ldr r0, [pc, #352] literal 0x4FF0 -> 0x200005E4 */
L_4e90:
    ldrh r0, [r0, #4]                       /* @4E90 8088 ldrh r0, [r0, #4]  */
L_4e92:
    ldr r1, [sp, #16]                       /* @4E92 0499 ldr r1, [sp, #16]  */
L_4e94:
    muls r0, r1, r0                         /* @4E94 4843 muls r0, r1, r0  */
L_4e96:
    ldr r1, [pc, #348]                      /* @4E96 5749 ldr r1, [pc, #348] literal 0x4FF4 -> 0x20000648 */
L_4e98:
    ldr r2, [r1, #12]                       /* @4E98 ca68 ldr r2, [r1, #12]  */
L_4e9a:
    ldr r1, [r1, #12]                       /* @4E9A c968 ldr r1, [r1, #12]  */
L_4e9c:
    asrs r2, r2, #1                         /* @4E9C 5210 asrs r2, r2, #1  */
L_4e9e:
    adds r0, r0, r2                         /* @4E9E 8018 adds r0, r0, r2  */
L_4ea0:
    bl L_013c                               /* @4EA0 fbf74cf9 bl #-19816 -> sdivmod32 */
L_4ea4:
    str r0, [sp, #16]                       /* @4EA4 0490 str r0, [sp, #16]  */
L_4ea6:
    ldr r0, [sp, #16]                       /* @4EA6 0498 ldr r0, [sp, #16]  */
L_4ea8:
    cmp r0, #0                              /* @4EA8 0028 cmp r0, #0  */
L_4eaa:
    ble L_4ece                              /* @4EAA 10dd ble #32 -> 0x4ECE */
L_4eac:
    ldr r0, [sp, #32]                       /* @4EAC 0898 ldr r0, [sp, #32]  */
L_4eae:
    cmp r0, #0                              /* @4EAE 0028 cmp r0, #0  */
L_4eb0:
    beq L_4ebe                              /* @4EB0 05d0 beq #10 -> 0x4EBE */
L_4eb2:
    bl L_34a4                               /* @4EB2 fef7f7fa bl #-6674 -> sub_34A4 */
L_4eb6:
    bl L_3b9c                               /* @4EB6 fef771fe bl #-4894 -> sub_3B9C */
L_4eba:
    bl L_5918                               /* @4EBA 00f02dfd bl #2650 -> sub_5918 */
L_4ebe:
    adr r2, L_4ff8                          /* @4EBE 4ea2 adr r2, #312 address 0x4FF8 -> 0x00004FF8 'Origin..' */
L_4ec0:
    adr r1, L_5004                          /* @4EC0 50a1 adr r1, #320 address 0x5004 -> 0x00005004 ' Back to' */
L_4ec2:
    adr r0, L_5010                          /* @4EC2 53a0 adr r0, #332 address 0x5010 -> 0x00005010 'Coming' */
L_4ec4:
    bl L_4884                               /* @4EC4 fff7defc bl #-1604 -> display_three_lines */
L_4ec8:
    movs r2, #25                            /* @4EC8 1922 movs r2, #25  */
L_4eca:
    lsls r2, r2, #5                         /* @4ECA 5201 lsls r2, r2, #5  */
L_4ecc:
    b L_4f3c                                /* @4ECC 36e0 b #108 -> 0x4F3C */
L_4ece:
    b L_4fd6                                /* @4ECE 82e0 b #260 -> 0x4FD6 */
L_4ed0:
    ldr r0, [sp, #20]                       /* @4ED0 0598 ldr r0, [sp, #20]  */
L_4ed2:
    cmp r0, #0                              /* @4ED2 0028 cmp r0, #0  */
L_4ed4:
    beq L_4ee6                              /* @4ED4 07d0 beq #14 -> 0x4EE6 */
L_4ed6:
    ldr r0, [r4]                            /* @4ED6 2068 ldr r0, [r4]  */
L_4ed8:
    ldr r1, [r4, #4]                        /* @4ED8 6168 ldr r1, [r4, #4]  */
L_4eda:
    adds r0, r0, #1                         /* @4EDA 401c adds r0, r0, #1  */
L_4edc:
    movs r3, #0                             /* @4EDC 0023 movs r3, #0  */
L_4ede:
    adcs r1, r3                             /* @4EDE 5941 adcs r1, r3  */
L_4ee0:
    str r0, [r4]                            /* @4EE0 2060 str r0, [r4]  */
L_4ee2:
    str r1, [r4, #4]                        /* @4EE2 6160 str r1, [r4, #4]  */
L_4ee4:
    b L_4ef6                                /* @4EE4 07e0 b #14 -> 0x4EF6 */
L_4ee6:
    ldr r0, [r4]                            /* @4EE6 2068 ldr r0, [r4]  */
L_4ee8:
    ldr r1, [r4, #4]                        /* @4EE8 6168 ldr r1, [r4, #4]  */
L_4eea:
    movs r3, #0                             /* @4EEA 0023 movs r3, #0  */
L_4eec:
    subs r0, r0, #1                         /* @4EEC 401e subs r0, r0, #1  */
L_4eee:
    mvns r3, r3                             /* @4EEE db43 mvns r3, r3  */
L_4ef0:
    adcs r1, r3                             /* @4EF0 5941 adcs r1, r3  */
L_4ef2:
    str r0, [r4]                            /* @4EF2 2060 str r0, [r4]  */
L_4ef4:
    str r1, [r4, #4]                        /* @4EF4 6160 str r1, [r4, #4]  */
L_4ef6:
    ldrh r0, [r6, #4]                       /* @4EF6 b088 ldrh r0, [r6, #4]  */
L_4ef8:
    movs r3, r0                             /* @4EF8 0300 movs r3, r0  */
L_4efa:
    bl L_15ec                               /* @4EFA fcf777fb bl #-14610 -> switch_u8_dispatch ; inline switch destinations 0x4F06,0x4F0C,0x4F12,0x4F1A,0x4F22,0x4F26 */
    .byte 0x05,0x04,0x07,0x0a,0x0e,0x12,0x14 /* @4EFE switch_table */
    .byte 0x00 /* @4F05 unclassified */
L_4f06:
    movs r0, #50                            /* @4F06 3220 movs r0, #50  */
L_4f08:
    str r0, [sp, #12]                       /* @4F08 0390 str r0, [sp, #12]  */
L_4f0a:
    b L_4f30                                /* @4F0A 11e0 b #34 -> 0x4F30 */
L_4f0c:
    movs r0, #100                           /* @4F0C 6420 movs r0, #100  */
L_4f0e:
    str r0, [sp, #12]                       /* @4F0E 0390 str r0, [sp, #12]  */
L_4f10:
    b L_4f30                                /* @4F10 0ee0 b #28 -> 0x4F30 */
L_4f12:
    movs r0, #255                           /* @4F12 ff20 movs r0, #255  */
L_4f14:
    adds r0, #45                            /* @4F14 2d30 adds r0, #45  */
L_4f16:
    str r0, [sp, #12]                       /* @4F16 0390 str r0, [sp, #12]  */
L_4f18:
    b L_4f30                                /* @4F18 0ae0 b #20 -> 0x4F30 */
L_4f1a:
    movs r0, #255                           /* @4F1A ff20 movs r0, #255  */
L_4f1c:
    adds r0, #245                           /* @4F1C f530 adds r0, #245  */
L_4f1e:
    str r0, [sp, #12]                       /* @4F1E 0390 str r0, [sp, #12]  */
L_4f20:
    b L_4f30                                /* @4F20 06e0 b #12 -> 0x4F30 */
L_4f22:
    str r2, [sp, #12]                       /* @4F22 0392 str r2, [sp, #12]  */
L_4f24:
    b L_4f30                                /* @4F24 04e0 b #8 -> 0x4F30 */
L_4f26:
    str r2, [sp, #12]                       /* @4F26 0392 str r2, [sp, #12]  */
L_4f28:
    b L_4f30                                /* @4F28 02e0 b #4 -> 0x4F30 */
L_4f2a:
    ldr r0, [sp, #12]                       /* @4F2A 0398 ldr r0, [sp, #12]  */
L_4f2c:
    subs r0, r0, #1                         /* @4F2C 401e subs r0, r0, #1  */
L_4f2e:
    str r0, [sp, #12]                       /* @4F2E 0390 str r0, [sp, #12]  */
L_4f30:
    ldr r0, [sp, #12]                       /* @4F30 0398 ldr r0, [sp, #12]  */
L_4f32:
    cmp r0, #0                              /* @4F32 0028 cmp r0, #0  */
L_4f34:
    bne L_4f2a                              /* @4F34 f9d1 bne #-14 -> 0x4F2A */
L_4f36:
    ldr r0, [sp, #16]                       /* @4F36 0498 ldr r0, [sp, #16]  */
L_4f38:
    subs r0, r0, #1                         /* @4F38 401e subs r0, r0, #1  */
L_4f3a:
    str r0, [sp, #16]                       /* @4F3A 0490 str r0, [sp, #16]  */
L_4f3c:
    ldr r0, [sp, #16]                       /* @4F3C 0498 ldr r0, [sp, #16]  */
L_4f3e:
    cmp r0, #0                              /* @4F3E 0028 cmp r0, #0  */
L_4f40:
    bne L_4ed0                              /* @4F40 c6d1 bne #-116 -> 0x4ED0 */
L_4f42:
    ldr r7, [pc, #212]                      /* @4F42 354f ldr r7, [pc, #212] literal 0x5018 -> 0x200005DA */
L_4f44:
    strb r5, [r7]                           /* @4F44 3d70 strb r5, [r7]  */
L_4f46:
    movs r5, #0                             /* @4F46 0025 movs r5, #0  */
L_4f48:
    str r5, [sp, #8]                        /* @4F48 0295 str r5, [sp, #8]  */
L_4f4a:
    ldrh r1, [r6, #2]                       /* @4F4A 7188 ldrh r1, [r6, #2]  */
L_4f4c:
    ldr r0, [r4, #16]                       /* @4F4C 2069 ldr r0, [r4, #16]  */
L_4f4e:
    ldr r2, [r4, #20]                       /* @4F4E 6269 ldr r2, [r4, #20]  */
L_4f50:
    subs r0, r1, r0                         /* @4F50 081a subs r0, r1, r0  */
L_4f52:
    sxth r0, r0                             /* @4F52 00b2 sxth r0, r0  */
L_4f54:
    str r0, [sp, #28]                       /* @4F54 0790 str r0, [sp, #28]  */
L_4f56:
    b L_4f8c                                /* @4F56 19e0 b #50 -> 0x4F8C */
L_4f58:
    ldr r0, [sp, #28]                       /* @4F58 0798 ldr r0, [sp, #28]  */
L_4f5a:
    str r0, [sp, #24]                       /* @4F5A 0690 str r0, [sp, #24]  */
L_4f5c:
    movs r0, #1                             /* @4F5C 0120 movs r0, #1  */
L_4f5e:
    bl L_2d98                               /* @4F5E fdf71bff bl #-8650 -> delay_100us_candidate */
L_4f62:
    ldr r0, [sp, #12]                       /* @4F62 0398 ldr r0, [sp, #12]  */
L_4f64:
    adds r0, r0, #1                         /* @4F64 401c adds r0, r0, #1  */
L_4f66:
    str r0, [sp, #12]                       /* @4F66 0390 str r0, [sp, #12]  */
L_4f68:
    ldrh r1, [r6, #2]                       /* @4F68 7188 ldrh r1, [r6, #2]  */
L_4f6a:
    ldr r0, [r4, #16]                       /* @4F6A 2069 ldr r0, [r4, #16]  */
L_4f6c:
    ldr r2, [r4, #20]                       /* @4F6C 6269 ldr r2, [r4, #20]  */
L_4f6e:
    subs r0, r1, r0                         /* @4F6E 081a subs r0, r1, r0  */
L_4f70:
    sxth r0, r0                             /* @4F70 00b2 sxth r0, r0  */
L_4f72:
    str r0, [sp, #28]                       /* @4F72 0790 str r0, [sp, #28]  */
L_4f74:
    ldr r1, [sp, #24]                       /* @4F74 0699 ldr r1, [sp, #24]  */
L_4f76:
    ldr r0, [sp, #28]                       /* @4F76 0798 ldr r0, [sp, #28]  */
L_4f78:
    subs r0, r0, r1                         /* @4F78 401a subs r0, r0, r1  */
L_4f7a:
    bl L_373a                               /* @4F7A fef7defb bl #-6212 -> abs32_candidate */
L_4f7e:
    cmp r0, #9                              /* @4F7E 0928 cmp r0, #9  */
L_4f80:
    bge L_4f8a                              /* @4F80 03da bge #6 -> 0x4F8A */
L_4f82:
    ldr r0, [sp, #8]                        /* @4F82 0298 ldr r0, [sp, #8]  */
L_4f84:
    adds r0, r0, #1                         /* @4F84 401c adds r0, r0, #1  */
L_4f86:
    str r0, [sp, #8]                        /* @4F86 0290 str r0, [sp, #8]  */
L_4f88:
    b L_4f8c                                /* @4F88 00e0 b #0 -> 0x4F8C */
L_4f8a:
    str r5, [sp, #8]                        /* @4F8A 0295 str r5, [sp, #8]  */
L_4f8c:
    ldr r0, [sp, #28]                       /* @4F8C 0798 ldr r0, [sp, #28]  */
L_4f8e:
    bl L_373a                               /* @4F8E fef7d4fb bl #-6232 -> abs32_candidate */
L_4f92:
    cmp r0, #18                             /* @4F92 1228 cmp r0, #18  */
L_4f94:
    ble L_4fa6                              /* @4F94 07dd ble #14 -> 0x4FA6 */
L_4f96:
    ldr r1, [pc, #132]                      /* @4F96 2149 ldr r1, [pc, #132] literal 0x501C -> 0x000186A0 */
L_4f98:
    ldr r0, [sp, #12]                       /* @4F98 0398 ldr r0, [sp, #12]  */
L_4f9a:
    cmp r0, r1                              /* @4F9A 8842 cmp r0, r1  */
L_4f9c:
    bge L_4fa6                              /* @4F9C 03da bge #6 -> 0x4FA6 */
L_4f9e:
    ldr r1, [pc, #128]                      /* @4F9E 2049 ldr r1, [pc, #128] literal 0x5020 -> 0x00001770 */
L_4fa0:
    ldr r0, [sp, #8]                        /* @4FA0 0298 ldr r0, [sp, #8]  */
L_4fa2:
    cmp r0, r1                              /* @4FA2 8842 cmp r0, r1  */
L_4fa4:
    blt L_4f58                              /* @4FA4 d8db blt #-80 -> 0x4F58 */
L_4fa6:
    ldr r0, [sp, #32]                       /* @4FA6 0898 ldr r0, [sp, #32]  */
L_4fa8:
    cmp r0, #0                              /* @4FA8 0028 cmp r0, #0  */
L_4faa:
    beq L_4fbc                              /* @4FAA 07d0 beq #14 -> 0x4FBC */
L_4fac:
    bl L_588c                               /* @4FAC 00f06efc bl #2268 -> sub_588C */
L_4fb0:
    bl L_3b70                               /* @4FB0 fef7defd bl #-5188 -> sub_3B70 */
L_4fb4:
    bl L_3488                               /* @4FB4 fef768fa bl #-6960 -> sub_3488 */
L_4fb8:
    bl L_342c                               /* @4FB8 fef738fa bl #-7056 -> sub_342C */
L_4fbc:
    ldr r0, [sp, #28]                       /* @4FBC 0798 ldr r0, [sp, #28]  */
L_4fbe:
    bl L_373a                               /* @4FBE fef7bcfb bl #-6280 -> abs32_candidate */
L_4fc2:
    cmp r0, #100                            /* @4FC2 6428 cmp r0, #100  */
L_4fc4:
    ble L_4fd0                              /* @4FC4 04dd ble #8 -> 0x4FD0 */
L_4fc6:
    strb r5, [r7]                           /* @4FC6 3d70 strb r5, [r7]  */
L_4fc8:
    bl L_4d94                               /* @4FC8 fff7e4fe bl #-568 -> sub_4D94 */
L_4fcc:
    movs r0, #0                             /* @4FCC 0020 movs r0, #0  */
L_4fce:
    b L_4e22                                /* @4FCE 28e7 b #-432 -> 0x4E22 */
L_4fd0:
    movs r0, #0                             /* @4FD0 0020 movs r0, #0  */
L_4fd2:
    bl L_3f28                               /* @4FD2 fef7a9ff bl #-4270 -> sub_3F28 */
L_4fd6:
    movs r0, #1                             /* @4FD6 0120 movs r0, #1  */
L_4fd8:
    b L_4e22                                /* @4FD8 23e7 b #-442 -> 0x4E22 */
    .byte 0x00,0x00 /* @4FDA unclassified */
L_4fdc:
    .byte 0x13,0x00,0x00,0x20 /* @4FDC literal */
L_4fe0:
    .byte 0x2c,0x07,0x00,0x20 /* @4FE0 literal */
L_4fe4:
    .byte 0xf6,0x05,0x00,0x20 /* @4FE4 literal */
L_4fe8:
    .byte 0xff,0xff,0x00,0x00 /* @4FE8 literal */
L_4fec:
    .byte 0x80,0x06,0x00,0x20 /* @4FEC literal */
L_4ff0:
    .byte 0xe4,0x05,0x00,0x20 /* @4FF0 literal */
L_4ff4:
    .byte 0x48,0x06,0x00,0x20 /* @4FF4 literal */
L_4ff8:
    .byte 0x4f,0x72,0x69,0x67,0x69,0x6e,0x2e,0x2e,0x00,0x00,0x00,0x00 /* @4FF8 unclassified */
L_5004:
    .byte 0x20,0x42,0x61,0x63,0x6b,0x20,0x74,0x6f,0x00,0x00,0x00,0x00 /* @5004 unclassified */
L_5010:
    .byte 0x43,0x6f,0x6d,0x69,0x6e,0x67,0x00,0x00 /* @5010 unclassified */
L_5018:
    .byte 0xda,0x05,0x00,0x20 /* @5018 literal */
L_501c:
    .byte 0xa0,0x86,0x01,0x00 /* @501C literal */
L_5020:
    .byte 0x70,0x17,0x00,0x00 /* @5020 literal */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @5024 unclassified */

/* sub_5030 */
L_5030:
    push {r4, lr}                           /* @5030 10b5 push {r4, lr}  */
L_5032:
    ldr r4, [pc, #36]                       /* @5032 094c ldr r4, [pc, #36] literal 0x5058 -> 0x20000620 */
L_5034:
    movs r0, #0                             /* @5034 0020 movs r0, #0  */
L_5036:
    strb r0, [r4]                           /* @5036 2070 strb r0, [r4]  */
L_5038:
    bl L_50f0                               /* @5038 00f05af8 bl #180 -> sub_50F0 */
L_503c:
    cmp r0, #0                              /* @503C 0028 cmp r0, #0  */
L_503e:
    bne L_504c                              /* @503E 05d1 bne #10 -> 0x504C */
L_5040:
    bl L_50f0                               /* @5040 00f056f8 bl #172 -> sub_50F0 */
L_5044:
    cmp r0, #0                              /* @5044 0028 cmp r0, #0  */
L_5046:
    bne L_504c                              /* @5046 01d1 bne #2 -> 0x504C */
L_5048:
    movs r1, #4                             /* @5048 0421 movs r1, #4  */
L_504a:
    strb r1, [r4]                           /* @504A 2170 strb r1, [r4]  */
L_504c:
    lsls r0, r0, #30                        /* @504C 8007 lsls r0, r0, #30  */
L_504e:
    bpl L_5054                              /* @504E 01d5 bpl #2 -> 0x5054 */
L_5050:
    movs r0, #3                             /* @5050 0320 movs r0, #3  */
L_5052:
    strb r0, [r4]                           /* @5052 2070 strb r0, [r4]  */
L_5054:
    ldrb r0, [r4]                           /* @5054 2078 ldrb r0, [r4]  */
L_5056:
    pop {r4, pc}                            /* @5056 10bd pop {r4, pc}  */
L_5058:
    .byte 0x20,0x06,0x00,0x20 /* @5058 literal */

/* sub_505C */
L_505c:
    push {r1, r2, r3, r4, r5, lr}           /* @505C 3eb5 push {r1, r2, r3, r4, r5, lr}  */
L_505e:
    movs r0, #0                             /* @505E 0020 movs r0, #0  */
L_5060:
    mov r1, sp                              /* @5060 6946 mov r1, sp  */
L_5062:
    strb r0, [r1, #4]                       /* @5062 0871 strb r0, [r1, #4]  */
L_5064:
    strb r0, [r1, #8]                       /* @5064 0872 strb r0, [r1, #8]  */
L_5066:
    movs r4, #1                             /* @5066 0124 movs r4, #1  */
L_5068:
    strb r4, [r1, #9]                       /* @5068 4c72 strb r4, [r1, #9]  */
L_506a:
    movs r1, #4                             /* @506A 0421 movs r1, #4  */
L_506c:
    bl L_1430                               /* @506C fcf7e0f9 bl #-15424 -> sub_1430 */
L_5070:
    add r2, sp, #4                          /* @5070 01aa add r2, sp, #4  */
L_5072:
    movs r1, #4                             /* @5072 0421 movs r1, #4  */
L_5074:
    movs r0, #0                             /* @5074 0020 movs r0, #0  */
L_5076:
    bl L_137c                               /* @5076 fcf781f9 bl #-15614 -> sub_137C */
L_507a:
    movs r2, #1                             /* @507A 0122 movs r2, #1  */
L_507c:
    movs r1, #4                             /* @507C 0421 movs r1, #4  */
L_507e:
    movs r0, #0                             /* @507E 0020 movs r0, #0  */
L_5080:
    bl L_1408                               /* @5080 fcf7c2f9 bl #-15484 -> sub_1408 */
L_5084:
    movs r1, #5                             /* @5084 0521 movs r1, #5  */
L_5086:
    movs r0, #0                             /* @5086 0020 movs r0, #0  */
L_5088:
    bl L_1430                               /* @5088 fcf7d2f9 bl #-15452 -> sub_1430 */
L_508c:
    add r2, sp, #4                          /* @508C 01aa add r2, sp, #4  */
L_508e:
    movs r1, #5                             /* @508E 0521 movs r1, #5  */
L_5090:
    movs r0, #0                             /* @5090 0020 movs r0, #0  */
L_5092:
    bl L_137c                               /* @5092 fcf773f9 bl #-15642 -> sub_137C */
L_5096:
    movs r2, #1                             /* @5096 0122 movs r2, #1  */
L_5098:
    movs r1, #5                             /* @5098 0521 movs r1, #5  */
L_509a:
    movs r0, #0                             /* @509A 0020 movs r0, #0  */
L_509c:
    bl L_1408                               /* @509C fcf7b4f9 bl #-15512 -> sub_1408 */
L_50a0:
    add r2, sp, #4                          /* @50A0 01aa add r2, sp, #4  */
L_50a2:
    movs r1, #7                             /* @50A2 0721 movs r1, #7  */
L_50a4:
    movs r0, #0                             /* @50A4 0020 movs r0, #0  */
L_50a6:
    bl L_137c                               /* @50A6 fcf769f9 bl #-15662 -> sub_137C */
L_50aa:
    movs r2, #1                             /* @50AA 0122 movs r2, #1  */
L_50ac:
    movs r1, #7                             /* @50AC 0721 movs r1, #7  */
L_50ae:
    movs r0, #0                             /* @50AE 0020 movs r0, #0  */
L_50b0:
    bl L_1408                               /* @50B0 fcf7aaf9 bl #-15532 -> sub_1408 */
L_50b4:
    mov r1, sp                              /* @50B4 6946 mov r1, sp  */
L_50b6:
    strb r4, [r1, #4]                       /* @50B6 0c71 strb r4, [r1, #4]  */
L_50b8:
    strb r4, [r1, #5]                       /* @50B8 4c71 strb r4, [r1, #5]  */
L_50ba:
    strb r4, [r1, #6]                       /* @50BA 8c71 strb r4, [r1, #6]  */
L_50bc:
    add r2, sp, #4                          /* @50BC 01aa add r2, sp, #4  */
L_50be:
    movs r1, #6                             /* @50BE 0621 movs r1, #6  */
L_50c0:
    movs r0, #0                             /* @50C0 0020 movs r0, #0  */
L_50c2:
    bl L_137c                               /* @50C2 fcf75bf9 bl #-15690 -> sub_137C */
L_50c6:
    movs r2, #1                             /* @50C6 0122 movs r2, #1  */
L_50c8:
    movs r1, #6                             /* @50C8 0621 movs r1, #6  */
L_50ca:
    movs r0, #0                             /* @50CA 0020 movs r0, #0  */
L_50cc:
    bl L_1408                               /* @50CC fcf79cf9 bl #-15560 -> sub_1408 */
L_50d0:
    movs r0, #16                            /* @50D0 1020 movs r0, #16  */
L_50d2:
    mov r1, sp                              /* @50D2 6946 mov r1, sp  */
L_50d4:
    strb r0, [r1]                           /* @50D4 0870 strb r0, [r1]  */
L_50d6:
    movs r0, #2                             /* @50D6 0220 movs r0, #2  */
L_50d8:
    strb r0, [r1, #1]                       /* @50D8 4870 strb r0, [r1, #1]  */
L_50da:
    movs r0, #4                             /* @50DA 0420 movs r0, #4  */
L_50dc:
    strb r0, [r1, #3]                       /* @50DC c870 strb r0, [r1, #3]  */
L_50de:
    movs r0, #8                             /* @50DE 0820 movs r0, #8  */
L_50e0:
    strb r0, [r1, #2]                       /* @50E0 8870 strb r0, [r1, #2]  */
L_50e2:
    ldr r0, [pc, #8]                        /* @50E2 0248 ldr r0, [pc, #8] literal 0x50EC -> 0x40000800 */
L_50e4:
    bl L_16c4                               /* @50E4 fcf7eefa bl #-14884 -> sub_16C4 */
L_50e8:
    pop {r1, r2, r3, r4, r5, pc}            /* @50E8 3ebd pop {r1, r2, r3, r4, r5, pc}  */
    .byte 0x00,0x00 /* @50EA unclassified */
L_50ec:
    .byte 0x00,0x08,0x00,0x40 /* @50EC literal */

/* sub_50F0 */
L_50f0:
    push {r4, r5, lr}                       /* @50F0 30b5 push {r4, r5, lr}  */
L_50f2:
    ldr r1, [pc, #228]                      /* @50F2 3949 ldr r1, [pc, #228] literal 0x51D8 -> 0x40000800 */
L_50f4:
    movs r5, #0                             /* @50F4 0025 movs r5, #0  */
L_50f6:
    str r5, [r1, #4]                        /* @50F6 4d60 str r5, [r1, #4]  */
L_50f8:
    movs r0, #0                             /* @50F8 0020 movs r0, #0  */
L_50fa:
    movs r4, #255                           /* @50FA ff24 movs r4, #255  */
L_50fc:
    adds r4, #145                           /* @50FC 9134 adds r4, #145  */
L_50fe:
    b L_510c                                /* @50FE 05e0 b #10 -> 0x510C */
L_5100:
    adds r0, r0, #1                         /* @5100 401c adds r0, r0, #1  */
L_5102:
    uxth r0, r0                             /* @5102 80b2 uxth r0, r0  */
L_5104:
    cmp r0, r4                              /* @5104 a042 cmp r0, r4  */
L_5106:
    bls L_510c                              /* @5106 01d9 bls #2 -> 0x510C */
L_5108:
    movs r0, #0                             /* @5108 0020 movs r0, #0  */
L_510a:
    pop {r4, r5, pc}                        /* @510A 30bd pop {r4, r5, pc}  */
L_510c:
    ldr r2, [r1, #8]                        /* @510C 8a68 ldr r2, [r1, #8]  */
L_510e:
    lsls r2, r2, #29                        /* @510E 5207 lsls r2, r2, #29  */
L_5110:
    bpl L_5100                              /* @5110 f6d5 bpl #-20 -> 0x5100 */
L_5112:
    movs r0, #131                           /* @5112 8320 movs r0, #131  */
L_5114:
    str r0, [r1, #12]                       /* @5114 c860 str r0, [r1, #12]  */
L_5116:
    movs r0, #0                             /* @5116 0020 movs r0, #0  */
L_5118:
    b L_5126                                /* @5118 05e0 b #10 -> 0x5126 */
L_511a:
    adds r0, r0, #1                         /* @511A 401c adds r0, r0, #1  */
L_511c:
    uxth r0, r0                             /* @511C 80b2 uxth r0, r0  */
L_511e:
    cmp r0, r4                              /* @511E a042 cmp r0, r4  */
L_5120:
    bls L_5126                              /* @5120 01d9 bls #2 -> 0x5126 */
L_5122:
    movs r0, #0                             /* @5122 0020 movs r0, #0  */
L_5124:
    pop {r4, r5, pc}                        /* @5124 30bd pop {r4, r5, pc}  */
L_5126:
    ldr r2, [r1, #8]                        /* @5126 8a68 ldr r2, [r1, #8]  */
L_5128:
    lsls r2, r2, #30                        /* @5128 9207 lsls r2, r2, #30  */
L_512a:
    bpl L_511a                              /* @512A f6d5 bpl #-20 -> 0x511A */
L_512c:
    ldr r0, [r1, #12]                       /* @512C c868 ldr r0, [r1, #12]  */
L_512e:
    movs r0, #0                             /* @512E 0020 movs r0, #0  */
L_5130:
    b L_513e                                /* @5130 05e0 b #10 -> 0x513E */
L_5132:
    adds r0, r0, #1                         /* @5132 401c adds r0, r0, #1  */
L_5134:
    uxth r0, r0                             /* @5134 80b2 uxth r0, r0  */
L_5136:
    cmp r0, r4                              /* @5136 a042 cmp r0, r4  */
L_5138:
    bls L_513e                              /* @5138 01d9 bls #2 -> 0x513E */
L_513a:
    movs r0, #0                             /* @513A 0020 movs r0, #0  */
L_513c:
    pop {r4, r5, pc}                        /* @513C 30bd pop {r4, r5, pc}  */
L_513e:
    ldr r2, [r1, #8]                        /* @513E 8a68 ldr r2, [r1, #8]  */
L_5140:
    lsls r2, r2, #29                        /* @5140 5207 lsls r2, r2, #29  */
L_5142:
    bpl L_5132                              /* @5142 f6d5 bpl #-20 -> 0x5132 */
L_5144:
    movs r0, #85                            /* @5144 5520 movs r0, #85  */
L_5146:
    str r0, [r1, #12]                       /* @5146 c860 str r0, [r1, #12]  */
L_5148:
    movs r0, #0                             /* @5148 0020 movs r0, #0  */
L_514a:
    b L_5158                                /* @514A 05e0 b #10 -> 0x5158 */
L_514c:
    adds r0, r0, #1                         /* @514C 401c adds r0, r0, #1  */
L_514e:
    uxth r0, r0                             /* @514E 80b2 uxth r0, r0  */
L_5150:
    cmp r0, r4                              /* @5150 a042 cmp r0, r4  */
L_5152:
    bls L_5158                              /* @5152 01d9 bls #2 -> 0x5158 */
L_5154:
    movs r0, #0                             /* @5154 0020 movs r0, #0  */
L_5156:
    pop {r4, r5, pc}                        /* @5156 30bd pop {r4, r5, pc}  */
L_5158:
    ldr r2, [r1, #8]                        /* @5158 8a68 ldr r2, [r1, #8]  */
L_515a:
    lsls r2, r2, #30                        /* @515A 9207 lsls r2, r2, #30  */
L_515c:
    bpl L_514c                              /* @515C f6d5 bpl #-20 -> 0x514C */
L_515e:
    ldr r0, [r1, #12]                       /* @515E c868 ldr r0, [r1, #12]  */
L_5160:
    lsls r0, r0, #24                        /* @5160 0006 lsls r0, r0, #24  */
L_5162:
    lsrs r2, r0, #16                        /* @5162 020c lsrs r2, r0, #16  */
L_5164:
    movs r3, #1                             /* @5164 0123 movs r3, #1  */
L_5166:
    str r3, [r1, #4]                        /* @5166 4b60 str r3, [r1, #4]  */
L_5168:
    str r5, [r1, #4]                        /* @5168 4d60 str r5, [r1, #4]  */
L_516a:
    movs r0, #0                             /* @516A 0020 movs r0, #0  */
L_516c:
    b L_517a                                /* @516C 05e0 b #10 -> 0x517A */
L_516e:
    adds r0, r0, #1                         /* @516E 401c adds r0, r0, #1  */
L_5170:
    uxth r0, r0                             /* @5170 80b2 uxth r0, r0  */
L_5172:
    cmp r0, r4                              /* @5172 a042 cmp r0, r4  */
L_5174:
    bls L_517a                              /* @5174 01d9 bls #2 -> 0x517A */
L_5176:
    movs r0, #0                             /* @5176 0020 movs r0, #0  */
L_5178:
    pop {r4, r5, pc}                        /* @5178 30bd pop {r4, r5, pc}  */
L_517a:
    ldr r5, [r1, #8]                        /* @517A 8d68 ldr r5, [r1, #8]  */
L_517c:
    lsls r5, r5, #29                        /* @517C 6d07 lsls r5, r5, #29  */
L_517e:
    bpl L_516e                              /* @517E f6d5 bpl #-20 -> 0x516E */
L_5180:
    movs r0, #132                           /* @5180 8420 movs r0, #132  */
L_5182:
    str r0, [r1, #12]                       /* @5182 c860 str r0, [r1, #12]  */
L_5184:
    movs r0, #0                             /* @5184 0020 movs r0, #0  */
L_5186:
    b L_5194                                /* @5186 05e0 b #10 -> 0x5194 */
L_5188:
    adds r0, r0, #1                         /* @5188 401c adds r0, r0, #1  */
L_518a:
    uxth r0, r0                             /* @518A 80b2 uxth r0, r0  */
L_518c:
    cmp r0, r4                              /* @518C a042 cmp r0, r4  */
L_518e:
    bls L_5194                              /* @518E 01d9 bls #2 -> 0x5194 */
L_5190:
    movs r0, #0                             /* @5190 0020 movs r0, #0  */
L_5192:
    pop {r4, r5, pc}                        /* @5192 30bd pop {r4, r5, pc}  */
L_5194:
    ldr r5, [r1, #8]                        /* @5194 8d68 ldr r5, [r1, #8]  */
L_5196:
    lsls r5, r5, #30                        /* @5196 ad07 lsls r5, r5, #30  */
L_5198:
    bpl L_5188                              /* @5198 f6d5 bpl #-20 -> 0x5188 */
L_519a:
    ldr r0, [r1, #12]                       /* @519A c868 ldr r0, [r1, #12]  */
L_519c:
    movs r0, #0                             /* @519C 0020 movs r0, #0  */
L_519e:
    b L_51ac                                /* @519E 05e0 b #10 -> 0x51AC */
L_51a0:
    adds r0, r0, #1                         /* @51A0 401c adds r0, r0, #1  */
L_51a2:
    uxth r0, r0                             /* @51A2 80b2 uxth r0, r0  */
L_51a4:
    cmp r0, r4                              /* @51A4 a042 cmp r0, r4  */
L_51a6:
    bls L_51ac                              /* @51A6 01d9 bls #2 -> 0x51AC */
L_51a8:
    movs r0, #0                             /* @51A8 0020 movs r0, #0  */
L_51aa:
    pop {r4, r5, pc}                        /* @51AA 30bd pop {r4, r5, pc}  */
L_51ac:
    ldr r5, [r1, #8]                        /* @51AC 8d68 ldr r5, [r1, #8]  */
L_51ae:
    lsls r5, r5, #29                        /* @51AE 6d07 lsls r5, r5, #29  */
L_51b0:
    bpl L_51a0                              /* @51B0 f6d5 bpl #-20 -> 0x51A0 */
L_51b2:
    movs r0, #160                           /* @51B2 a020 movs r0, #160  */
L_51b4:
    str r0, [r1, #12]                       /* @51B4 c860 str r0, [r1, #12]  */
L_51b6:
    movs r0, #0                             /* @51B6 0020 movs r0, #0  */
L_51b8:
    b L_51c6                                /* @51B8 05e0 b #10 -> 0x51C6 */
L_51ba:
    adds r0, r0, #1                         /* @51BA 401c adds r0, r0, #1  */
L_51bc:
    uxth r0, r0                             /* @51BC 80b2 uxth r0, r0  */
L_51be:
    cmp r0, r4                              /* @51BE a042 cmp r0, r4  */
L_51c0:
    bls L_51c6                              /* @51C0 01d9 bls #2 -> 0x51C6 */
L_51c2:
    movs r0, #0                             /* @51C2 0020 movs r0, #0  */
L_51c4:
    pop {r4, r5, pc}                        /* @51C4 30bd pop {r4, r5, pc}  */
L_51c6:
    ldr r5, [r1, #8]                        /* @51C6 8d68 ldr r5, [r1, #8]  */
L_51c8:
    lsls r5, r5, #30                        /* @51C8 ad07 lsls r5, r5, #30  */
L_51ca:
    bpl L_51ba                              /* @51CA f6d5 bpl #-20 -> 0x51BA */
L_51cc:
    ldr r0, [r1, #12]                       /* @51CC c868 ldr r0, [r1, #12]  */
L_51ce:
    uxtb r0, r0                             /* @51CE c0b2 uxtb r0, r0  */
L_51d0:
    orrs r0, r2                             /* @51D0 1043 orrs r0, r2  */
L_51d2:
    str r3, [r1, #4]                        /* @51D2 4b60 str r3, [r1, #4]  */
L_51d4:
    pop {r4, r5, pc}                        /* @51D4 30bd pop {r4, r5, pc}  */
    .byte 0x00,0x00 /* @51D6 unclassified */
L_51d8:
    .byte 0x00,0x08,0x00,0x40 /* @51D8 literal */

/* sub_51DC */
L_51dc:
    push {r4, r5, lr}                       /* @51DC 30b5 push {r4, r5, lr}  */
L_51de:
    ldr r1, [pc, #232]                      /* @51DE 3a49 ldr r1, [pc, #232] literal 0x52C8 -> 0x40000800 */
L_51e0:
    movs r5, #0                             /* @51E0 0025 movs r5, #0  */
L_51e2:
    str r5, [r1, #4]                        /* @51E2 4d60 str r5, [r1, #4]  */
L_51e4:
    movs r0, #0                             /* @51E4 0020 movs r0, #0  */
L_51e6:
    movs r2, #255                           /* @51E6 ff22 movs r2, #255  */
L_51e8:
    adds r2, #145                           /* @51E8 9132 adds r2, #145  */
L_51ea:
    b L_51f8                                /* @51EA 05e0 b #10 -> 0x51F8 */
L_51ec:
    adds r0, r0, #1                         /* @51EC 401c adds r0, r0, #1  */
L_51ee:
    uxth r0, r0                             /* @51EE 80b2 uxth r0, r0  */
L_51f0:
    cmp r0, r2                              /* @51F0 9042 cmp r0, r2  */
L_51f2:
    bls L_51f8                              /* @51F2 01d9 bls #2 -> 0x51F8 */
L_51f4:
    movs r0, #0                             /* @51F4 0020 movs r0, #0  */
L_51f6:
    pop {r4, r5, pc}                        /* @51F6 30bd pop {r4, r5, pc}  */
L_51f8:
    ldr r3, [r1, #8]                        /* @51F8 8b68 ldr r3, [r1, #8]  */
L_51fa:
    lsls r3, r3, #29                        /* @51FA 5b07 lsls r3, r3, #29  */
L_51fc:
    bpl L_51ec                              /* @51FC f6d5 bpl #-20 -> 0x51EC */
L_51fe:
    movs r0, #131                           /* @51FE 8320 movs r0, #131  */
L_5200:
    str r0, [r1, #12]                       /* @5200 c860 str r0, [r1, #12]  */
L_5202:
    movs r0, #0                             /* @5202 0020 movs r0, #0  */
L_5204:
    b L_5212                                /* @5204 05e0 b #10 -> 0x5212 */
L_5206:
    adds r0, r0, #1                         /* @5206 401c adds r0, r0, #1  */
L_5208:
    uxth r0, r0                             /* @5208 80b2 uxth r0, r0  */
L_520a:
    cmp r0, r2                              /* @520A 9042 cmp r0, r2  */
L_520c:
    bls L_5212                              /* @520C 01d9 bls #2 -> 0x5212 */
L_520e:
    movs r0, #0                             /* @520E 0020 movs r0, #0  */
L_5210:
    pop {r4, r5, pc}                        /* @5210 30bd pop {r4, r5, pc}  */
L_5212:
    ldr r3, [r1, #8]                        /* @5212 8b68 ldr r3, [r1, #8]  */
L_5214:
    lsls r3, r3, #30                        /* @5214 9b07 lsls r3, r3, #30  */
L_5216:
    bpl L_5206                              /* @5216 f6d5 bpl #-20 -> 0x5206 */
L_5218:
    ldr r0, [r1, #12]                       /* @5218 c868 ldr r0, [r1, #12]  */
L_521a:
    movs r0, #0                             /* @521A 0020 movs r0, #0  */
L_521c:
    b L_522a                                /* @521C 05e0 b #10 -> 0x522A */
L_521e:
    adds r0, r0, #1                         /* @521E 401c adds r0, r0, #1  */
L_5220:
    uxth r0, r0                             /* @5220 80b2 uxth r0, r0  */
L_5222:
    cmp r0, r2                              /* @5222 9042 cmp r0, r2  */
L_5224:
    bls L_522a                              /* @5224 01d9 bls #2 -> 0x522A */
L_5226:
    movs r0, #0                             /* @5226 0020 movs r0, #0  */
L_5228:
    pop {r4, r5, pc}                        /* @5228 30bd pop {r4, r5, pc}  */
L_522a:
    ldr r3, [r1, #8]                        /* @522A 8b68 ldr r3, [r1, #8]  */
L_522c:
    lsls r3, r3, #29                        /* @522C 5b07 lsls r3, r3, #29  */
L_522e:
    bpl L_521e                              /* @522E f6d5 bpl #-20 -> 0x521E */
L_5230:
    movs r0, #85                            /* @5230 5520 movs r0, #85  */
L_5232:
    str r0, [r1, #12]                       /* @5232 c860 str r0, [r1, #12]  */
L_5234:
    movs r0, #0                             /* @5234 0020 movs r0, #0  */
L_5236:
    b L_5244                                /* @5236 05e0 b #10 -> 0x5244 */
L_5238:
    adds r0, r0, #1                         /* @5238 401c adds r0, r0, #1  */
L_523a:
    uxth r0, r0                             /* @523A 80b2 uxth r0, r0  */
L_523c:
    cmp r0, r2                              /* @523C 9042 cmp r0, r2  */
L_523e:
    bls L_5244                              /* @523E 01d9 bls #2 -> 0x5244 */
L_5240:
    movs r0, #0                             /* @5240 0020 movs r0, #0  */
L_5242:
    pop {r4, r5, pc}                        /* @5242 30bd pop {r4, r5, pc}  */
L_5244:
    ldr r3, [r1, #8]                        /* @5244 8b68 ldr r3, [r1, #8]  */
L_5246:
    lsls r3, r3, #30                        /* @5246 9b07 lsls r3, r3, #30  */
L_5248:
    bpl L_5238                              /* @5248 f6d5 bpl #-20 -> 0x5238 */
L_524a:
    ldr r0, [r1, #12]                       /* @524A c868 ldr r0, [r1, #12]  */
L_524c:
    lsls r0, r0, #22                        /* @524C 8005 lsls r0, r0, #22  */
L_524e:
    lsrs r4, r0, #16                        /* @524E 040c lsrs r4, r0, #16  */
L_5250:
    movs r3, #1                             /* @5250 0123 movs r3, #1  */
L_5252:
    str r3, [r1, #4]                        /* @5252 4b60 str r3, [r1, #4]  */
L_5254:
    str r5, [r1, #4]                        /* @5254 4d60 str r5, [r1, #4]  */
L_5256:
    movs r0, #0                             /* @5256 0020 movs r0, #0  */
L_5258:
    b L_5266                                /* @5258 05e0 b #10 -> 0x5266 */
L_525a:
    adds r0, r0, #1                         /* @525A 401c adds r0, r0, #1  */
L_525c:
    uxth r0, r0                             /* @525C 80b2 uxth r0, r0  */
L_525e:
    cmp r0, r2                              /* @525E 9042 cmp r0, r2  */
L_5260:
    bls L_5266                              /* @5260 01d9 bls #2 -> 0x5266 */
L_5262:
    movs r0, #0                             /* @5262 0020 movs r0, #0  */
L_5264:
    pop {r4, r5, pc}                        /* @5264 30bd pop {r4, r5, pc}  */
L_5266:
    ldr r5, [r1, #8]                        /* @5266 8d68 ldr r5, [r1, #8]  */
L_5268:
    lsls r5, r5, #29                        /* @5268 6d07 lsls r5, r5, #29  */
L_526a:
    bpl L_525a                              /* @526A f6d5 bpl #-20 -> 0x525A */
L_526c:
    movs r0, #132                           /* @526C 8420 movs r0, #132  */
L_526e:
    str r0, [r1, #12]                       /* @526E c860 str r0, [r1, #12]  */
L_5270:
    movs r0, #0                             /* @5270 0020 movs r0, #0  */
L_5272:
    b L_5280                                /* @5272 05e0 b #10 -> 0x5280 */
L_5274:
    adds r0, r0, #1                         /* @5274 401c adds r0, r0, #1  */
L_5276:
    uxth r0, r0                             /* @5276 80b2 uxth r0, r0  */
L_5278:
    cmp r0, r2                              /* @5278 9042 cmp r0, r2  */
L_527a:
    bls L_5280                              /* @527A 01d9 bls #2 -> 0x5280 */
L_527c:
    movs r0, #0                             /* @527C 0020 movs r0, #0  */
L_527e:
    pop {r4, r5, pc}                        /* @527E 30bd pop {r4, r5, pc}  */
L_5280:
    ldr r5, [r1, #8]                        /* @5280 8d68 ldr r5, [r1, #8]  */
L_5282:
    lsls r5, r5, #30                        /* @5282 ad07 lsls r5, r5, #30  */
L_5284:
    bpl L_5274                              /* @5284 f6d5 bpl #-20 -> 0x5274 */
L_5286:
    ldr r0, [r1, #12]                       /* @5286 c868 ldr r0, [r1, #12]  */
L_5288:
    movs r0, #0                             /* @5288 0020 movs r0, #0  */
L_528a:
    b L_5298                                /* @528A 05e0 b #10 -> 0x5298 */
L_528c:
    adds r0, r0, #1                         /* @528C 401c adds r0, r0, #1  */
L_528e:
    uxth r0, r0                             /* @528E 80b2 uxth r0, r0  */
L_5290:
    cmp r0, r2                              /* @5290 9042 cmp r0, r2  */
L_5292:
    bls L_5298                              /* @5292 01d9 bls #2 -> 0x5298 */
L_5294:
    movs r0, #0                             /* @5294 0020 movs r0, #0  */
L_5296:
    pop {r4, r5, pc}                        /* @5296 30bd pop {r4, r5, pc}  */
L_5298:
    ldr r5, [r1, #8]                        /* @5298 8d68 ldr r5, [r1, #8]  */
L_529a:
    lsls r5, r5, #29                        /* @529A 6d07 lsls r5, r5, #29  */
L_529c:
    bpl L_528c                              /* @529C f6d5 bpl #-20 -> 0x528C */
L_529e:
    movs r0, #160                           /* @529E a020 movs r0, #160  */
L_52a0:
    str r0, [r1, #12]                       /* @52A0 c860 str r0, [r1, #12]  */
L_52a2:
    movs r0, #0                             /* @52A2 0020 movs r0, #0  */
L_52a4:
    b L_52b2                                /* @52A4 05e0 b #10 -> 0x52B2 */
L_52a6:
    adds r0, r0, #1                         /* @52A6 401c adds r0, r0, #1  */
L_52a8:
    uxth r0, r0                             /* @52A8 80b2 uxth r0, r0  */
L_52aa:
    cmp r0, r2                              /* @52AA 9042 cmp r0, r2  */
L_52ac:
    bls L_52b2                              /* @52AC 01d9 bls #2 -> 0x52B2 */
L_52ae:
    movs r0, #0                             /* @52AE 0020 movs r0, #0  */
L_52b0:
    pop {r4, r5, pc}                        /* @52B0 30bd pop {r4, r5, pc}  */
L_52b2:
    ldr r5, [r1, #8]                        /* @52B2 8d68 ldr r5, [r1, #8]  */
L_52b4:
    lsls r5, r5, #30                        /* @52B4 ad07 lsls r5, r5, #30  */
L_52b6:
    bpl L_52a6                              /* @52B6 f6d5 bpl #-20 -> 0x52A6 */
L_52b8:
    ldr r0, [r1, #12]                       /* @52B8 c868 ldr r0, [r1, #12]  */
L_52ba:
    lsls r0, r0, #16                        /* @52BA 0004 lsls r0, r0, #16  */
L_52bc:
    lsrs r0, r0, #18                        /* @52BC 800c lsrs r0, r0, #18  */
L_52be:
    adds r0, r0, r4                         /* @52BE 0019 adds r0, r0, r4  */
L_52c0:
    uxth r0, r0                             /* @52C0 80b2 uxth r0, r0  */
L_52c2:
    str r3, [r1, #4]                        /* @52C2 4b60 str r3, [r1, #4]  */
L_52c4:
    pop {r4, r5, pc}                        /* @52C4 30bd pop {r4, r5, pc}  */
    .byte 0x00,0x00 /* @52C6 unclassified */
L_52c8:
    .byte 0x00,0x08,0x00,0x40 /* @52C8 literal */

/* sub_52CC */
L_52cc:
    ldr r1, [pc, #128]                      /* @52CC 2049 ldr r1, [pc, #128] literal 0x5350 -> 0x2000072C */
L_52ce:
    ldrh r2, [r1]                           /* @52CE 0a88 ldrh r2, [r1]  */
L_52d0:
    ldr r0, [pc, #128]                      /* @52D0 2048 ldr r0, [pc, #128] literal 0x5354 -> 0x200006F4 */
L_52d2:
    strh r2, [r0]                           /* @52D2 0280 strh r2, [r0]  */
L_52d4:
    ldrh r2, [r1, #2]                       /* @52D4 4a88 ldrh r2, [r1, #2]  */
L_52d6:
    strh r2, [r0, #2]                       /* @52D6 4280 strh r2, [r0, #2]  */
L_52d8:
    ldrb r2, [r1, #4]                       /* @52D8 0a79 ldrb r2, [r1, #4]  */
L_52da:
    strb r2, [r0, #4]                       /* @52DA 0271 strb r2, [r0, #4]  */
L_52dc:
    ldrh r2, [r1, #6]                       /* @52DC ca88 ldrh r2, [r1, #6]  */
L_52de:
    strh r2, [r0, #6]                       /* @52DE c280 strh r2, [r0, #6]  */
L_52e0:
    ldrb r2, [r1, #8]                       /* @52E0 0a7a ldrb r2, [r1, #8]  */
L_52e2:
    strb r2, [r0, #8]                       /* @52E2 0272 strb r2, [r0, #8]  */
L_52e4:
    ldrb r2, [r1, #9]                       /* @52E4 4a7a ldrb r2, [r1, #9]  */
L_52e6:
    strb r2, [r0, #9]                       /* @52E6 4272 strb r2, [r0, #9]  */
L_52e8:
    ldrb r2, [r1, #10]                      /* @52E8 8a7a ldrb r2, [r1, #10]  */
L_52ea:
    strb r2, [r0, #10]                      /* @52EA 8272 strb r2, [r0, #10]  */
L_52ec:
    ldrb r2, [r1, #11]                      /* @52EC ca7a ldrb r2, [r1, #11]  */
L_52ee:
    strb r2, [r0, #11]                      /* @52EE c272 strb r2, [r0, #11]  */
L_52f0:
    ldrb r2, [r1, #12]                      /* @52F0 0a7b ldrb r2, [r1, #12]  */
L_52f2:
    strb r2, [r0, #12]                      /* @52F2 0273 strb r2, [r0, #12]  */
L_52f4:
    ldr r2, [r1, #16]                       /* @52F4 0a69 ldr r2, [r1, #16]  */
L_52f6:
    str r2, [r0, #16]                       /* @52F6 0261 str r2, [r0, #16]  */
L_52f8:
    ldrh r2, [r1, #20]                      /* @52F8 8a8a ldrh r2, [r1, #20]  */
L_52fa:
    strh r2, [r0, #20]                      /* @52FA 8282 strh r2, [r0, #20]  */
L_52fc:
    ldrh r2, [r1, #22]                      /* @52FC ca8a ldrh r2, [r1, #22]  */
L_52fe:
    strh r2, [r0, #22]                      /* @52FE c282 strh r2, [r0, #22]  */
L_5300:
    ldrh r2, [r1, #24]                      /* @5300 0a8b ldrh r2, [r1, #24]  */
L_5302:
    strh r2, [r0, #24]                      /* @5302 0283 strh r2, [r0, #24]  */
L_5304:
    ldrh r1, [r1, #26]                      /* @5304 498b ldrh r1, [r1, #26]  */
L_5306:
    strh r1, [r0, #26]                      /* @5306 4183 strh r1, [r0, #26]  */
L_5308:
    ldr r1, [pc, #76]                       /* @5308 1349 ldr r1, [pc, #76] literal 0x5358 -> 0x200005E4 */
L_530a:
    ldrh r2, [r1]                           /* @530A 0a88 ldrh r2, [r1]  */
L_530c:
    strh r2, [r0, #28]                      /* @530C 8283 strh r2, [r0, #28]  */
L_530e:
    ldrh r2, [r1, #2]                       /* @530E 4a88 ldrh r2, [r1, #2]  */
L_5310:
    strh r2, [r0, #30]                      /* @5310 c283 strh r2, [r0, #30]  */
L_5312:
    ldrh r1, [r1, #4]                       /* @5312 8988 ldrh r1, [r1, #4]  */
L_5314:
    strh r1, [r0, #32]                      /* @5314 0184 strh r1, [r0, #32]  */
L_5316:
    ldr r2, [pc, #68]                       /* @5316 114a ldr r2, [pc, #68] literal 0x535C -> 0x200005F6 */
L_5318:
    ldrb r3, [r2]                           /* @5318 1378 ldrb r3, [r2]  */
L_531a:
    mov r1, r0                              /* @531A 0146 mov r1, r0  */
L_531c:
    adds r1, #32                            /* @531C 2031 adds r1, #32  */
L_531e:
    strb r3, [r1, #2]                       /* @531E 8b70 strb r3, [r1, #2]  */
L_5320:
    ldrh r3, [r2, #2]                       /* @5320 5388 ldrh r3, [r2, #2]  */
L_5322:
    strh r3, [r0, #36]                      /* @5322 8384 strh r3, [r0, #36]  */
L_5324:
    ldrh r3, [r2, #4]                       /* @5324 9388 ldrh r3, [r2, #4]  */
L_5326:
    strh r3, [r0, #38]                      /* @5326 c384 strh r3, [r0, #38]  */
L_5328:
    ldrb r2, [r2, #6]                       /* @5328 9279 ldrb r2, [r2, #6]  */
L_532a:
    strb r2, [r1, #8]                       /* @532A 0a72 strb r2, [r1, #8]  */
L_532c:
    ldr r1, [pc, #48]                       /* @532C 0c49 ldr r1, [pc, #48] literal 0x5360 -> 0x200005EA */
L_532e:
    ldrh r2, [r1]                           /* @532E 0a88 ldrh r2, [r1]  */
L_5330:
    strh r2, [r0, #42]                      /* @5330 4285 strh r2, [r0, #42]  */
L_5332:
    ldrh r2, [r1, #2]                       /* @5332 4a88 ldrh r2, [r1, #2]  */
L_5334:
    strh r2, [r0, #44]                      /* @5334 8285 strh r2, [r0, #44]  */
L_5336:
    ldrh r1, [r1, #4]                       /* @5336 8988 ldrh r1, [r1, #4]  */
L_5338:
    strh r1, [r0, #46]                      /* @5338 c185 strh r1, [r0, #46]  */
L_533a:
    ldr r1, [pc, #40]                       /* @533A 0a49 ldr r1, [pc, #40] literal 0x5364 -> 0x200005F0 */
L_533c:
    ldrh r2, [r1]                           /* @533C 0a88 ldrh r2, [r1]  */
L_533e:
    strh r2, [r0, #48]                      /* @533E 0286 strh r2, [r0, #48]  */
L_5340:
    ldrh r2, [r1, #2]                       /* @5340 4a88 ldrh r2, [r1, #2]  */
L_5342:
    strh r2, [r0, #50]                      /* @5342 4286 strh r2, [r0, #50]  */
L_5344:
    ldrh r1, [r1, #4]                       /* @5344 8988 ldrh r1, [r1, #4]  */
L_5346:
    strh r1, [r0, #52]                      /* @5346 8186 strh r1, [r0, #52]  */
L_5348:
    movs r1, #1                             /* @5348 0121 movs r1, #1  */
L_534a:
    strh r1, [r0, #54]                      /* @534A c186 strh r1, [r0, #54]  */
L_534c:
    bx lr                                   /* @534C 7047 bx lr  */
    .byte 0x00,0x00 /* @534E unclassified */
L_5350:
    .byte 0x2c,0x07,0x00,0x20 /* @5350 literal */
L_5354:
    .byte 0xf4,0x06,0x00,0x20 /* @5354 literal */
L_5358:
    .byte 0xe4,0x05,0x00,0x20 /* @5358 literal */
L_535c:
    .byte 0xf6,0x05,0x00,0x20 /* @535C literal */
L_5360:
    .byte 0xea,0x05,0x00,0x20 /* @5360 literal */
L_5364:
    .byte 0xf0,0x05,0x00,0x20 /* @5364 literal */

/* sub_5368 */
L_5368:
    push {r4, lr}                           /* @5368 10b5 push {r4, lr}  */
L_536a:
    bl L_4660                               /* @536A fff779f9 bl #-3342 -> sub_4660 */
L_536e:
    ldr r0, [pc, #124]                      /* @536E 1f48 ldr r0, [pc, #124] literal 0x53EC -> 0x20000004 */
L_5370:
    ldr r0, [r0]                            /* @5370 0068 ldr r0, [r0]  */
L_5372:
    ldrh r0, [r0, #54]                      /* @5372 c08e ldrh r0, [r0, #54]  */
L_5374:
    cmp r0, #1                              /* @5374 0128 cmp r0, #1  */
L_5376:
    beq L_53e2                              /* @5376 34d0 beq #104 -> 0x53E2 */
L_5378:
    ldr r0, [pc, #120]                      /* @5378 1e48 ldr r0, [pc, #120] literal 0x53F4 -> 0x200005EA */
L_537a:
    ldr r1, [pc, #116]                      /* @537A 1d49 ldr r1, [pc, #116] literal 0x53F0 -> 0x00000666 */
L_537c:
    strh r1, [r0]                           /* @537C 0180 strh r1, [r0]  */
L_537e:
    movs r2, #1                             /* @537E 0122 movs r2, #1  */
L_5380:
    strh r2, [r0, #2]                       /* @5380 4280 strh r2, [r0, #2]  */
L_5382:
    strh r1, [r0, #4]                       /* @5382 8180 strh r1, [r0, #4]  */
L_5384:
    ldr r0, [pc, #112]                      /* @5384 1c48 ldr r0, [pc, #112] literal 0x53F8 -> 0x2000072C */
L_5386:
    movs r1, #16                            /* @5386 1021 movs r1, #16  */
L_5388:
    strh r1, [r0]                           /* @5388 0180 strh r1, [r0]  */
L_538a:
    movs r1, #0                             /* @538A 0021 movs r1, #0  */
L_538c:
    strh r1, [r0, #2]                       /* @538C 4180 strh r1, [r0, #2]  */
L_538e:
    strb r1, [r0, #4]                       /* @538E 0171 strb r1, [r0, #4]  */
L_5390:
    ldr r3, [pc, #104]                      /* @5390 1a4b ldr r3, [pc, #104] literal 0x53FC -> 0x00000CCC */
L_5392:
    strh r3, [r0, #6]                       /* @5392 c380 strh r3, [r0, #6]  */
L_5394:
    strb r1, [r0, #8]                       /* @5394 0172 strb r1, [r0, #8]  */
L_5396:
    strb r2, [r0, #9]                       /* @5396 4272 strb r2, [r0, #9]  */
L_5398:
    strb r1, [r0, #10]                      /* @5398 8172 strb r1, [r0, #10]  */
L_539a:
    strb r1, [r0, #11]                      /* @539A c172 strb r1, [r0, #11]  */
L_539c:
    strb r2, [r0, #12]                      /* @539C 0273 strb r2, [r0, #12]  */
L_539e:
    str r1, [r0, #16]                       /* @539E 0161 str r1, [r0, #16]  */
L_53a0:
    movs r3, #224                           /* @53A0 e023 movs r3, #224  */
L_53a2:
    strh r3, [r0, #20]                      /* @53A2 8382 strh r3, [r0, #20]  */
L_53a4:
    ldr r4, [pc, #88]                       /* @53A4 164c ldr r4, [pc, #88] literal 0x5400 -> 0x0000FFFF */
L_53a6:
    strh r4, [r0, #22]                      /* @53A6 c482 strh r4, [r0, #22]  */
L_53a8:
    strh r1, [r0, #24]                      /* @53A8 0183 strh r1, [r0, #24]  */
L_53aa:
    movs r3, #135                           /* @53AA 8723 movs r3, #135  */
L_53ac:
    lsls r3, r3, #3                         /* @53AC db00 lsls r3, r3, #3  */
L_53ae:
    strh r3, [r0, #26]                      /* @53AE 4383 strh r3, [r0, #26]  */
L_53b0:
    movs r3, #255                           /* @53B0 ff23 movs r3, #255  */
L_53b2:
    ldr r0, [pc, #80]                       /* @53B2 1448 ldr r0, [pc, #80] literal 0x5404 -> 0x200005F0 */
L_53b4:
    adds r3, #31                            /* @53B4 1f33 adds r3, #31  */
L_53b6:
    strh r3, [r0]                           /* @53B6 0380 strh r3, [r0]  */
L_53b8:
    movs r3, #2                             /* @53B8 0223 movs r3, #2  */
L_53ba:
    strh r3, [r0, #2]                       /* @53BA 4380 strh r3, [r0, #2]  */
L_53bc:
    strh r2, [r0, #4]                       /* @53BC 8280 strh r2, [r0, #4]  */
L_53be:
    ldr r0, [pc, #72]                       /* @53BE 1248 ldr r0, [pc, #72] literal 0x5408 -> 0x200005F6 */
L_53c0:
    strb r1, [r0]                           /* @53C0 0170 strb r1, [r0]  */
L_53c2:
    strh r4, [r0, #2]                       /* @53C2 4480 strh r4, [r0, #2]  */
L_53c4:
    strh r3, [r0, #4]                       /* @53C4 8380 strh r3, [r0, #4]  */
L_53c6:
    strb r1, [r0, #6]                       /* @53C6 8171 strb r1, [r0, #6]  */
L_53c8:
    ldr r0, [pc, #64]                       /* @53C8 1048 ldr r0, [pc, #64] literal 0x540C -> 0x200005E4 */
L_53ca:
    movs r1, #25                            /* @53CA 1921 movs r1, #25  */
L_53cc:
    strh r1, [r0, #4]                       /* @53CC 8180 strh r1, [r0, #4]  */
L_53ce:
    movs r1, #75                            /* @53CE 4b21 movs r1, #75  */
L_53d0:
    lsls r1, r1, #3                         /* @53D0 c900 lsls r1, r1, #3  */
L_53d2:
    strh r1, [r0, #2]                       /* @53D2 4180 strh r1, [r0, #2]  */
L_53d4:
    lsls r1, r1, #1                         /* @53D4 4900 lsls r1, r1, #1  */
L_53d6:
    strh r1, [r0]                           /* @53D6 0180 strh r1, [r0]  */
L_53d8:
    ldr r0, [pc, #52]                       /* @53D8 0d48 ldr r0, [pc, #52] literal 0x5410 -> 0x20000648 */
L_53da:
    strh r2, [r0, #8]                       /* @53DA 0281 strh r2, [r0, #8]  */
L_53dc:
    bl L_52cc                               /* @53DC fff776ff bl #-276 -> sub_52CC */
L_53e0:
    pop {r4, pc}                            /* @53E0 10bd pop {r4, pc}  */
L_53e2:
    bl L_3978                               /* @53E2 fef7c9fa bl #-6766 -> sub_3978 */
L_53e6:
    bl L_52cc                               /* @53E6 fff771ff bl #-286 -> sub_52CC */
L_53ea:
    pop {r4, pc}                            /* @53EA 10bd pop {r4, pc}  */
L_53ec:
    .byte 0x04,0x00,0x00,0x20 /* @53EC literal */
L_53f0:
    .byte 0x66,0x06,0x00,0x00 /* @53F0 literal */
L_53f4:
    .byte 0xea,0x05,0x00,0x20 /* @53F4 literal */
L_53f8:
    .byte 0x2c,0x07,0x00,0x20 /* @53F8 literal */
L_53fc:
    .byte 0xcc,0x0c,0x00,0x00 /* @53FC literal */
L_5400:
    .byte 0xff,0xff,0x00,0x00 /* @5400 literal */
L_5404:
    .byte 0xf0,0x05,0x00,0x20 /* @5404 literal */
L_5408:
    .byte 0xf6,0x05,0x00,0x20 /* @5408 literal */
L_540c:
    .byte 0xe4,0x05,0x00,0x20 /* @540C literal */
L_5410:
    .byte 0x48,0x06,0x00,0x20 /* @5410 literal */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @5414 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @5424 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @5434 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @5444 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @5454 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @5464 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @5474 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @5484 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @5494 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @54A4 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @54B4 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @54C4 unclassified */

/* calibration_work_candidate */
L_54cc:
    push {r3, r4, r5, r6, r7, lr}           /* @54CC f8b5 push {r3, r4, r5, r6, r7, lr}  */
L_54ce:
    movs r4, #0                             /* @54CE 0024 movs r4, #0  */
L_54d0:
    movs r6, #0                             /* @54D0 0026 movs r6, #0  */
L_54d2:
    bl L_5874                               /* @54D2 00f0cff9 bl #926 -> sub_5874 */
L_54d6:
    adr r2, L_55e0                          /* @54D6 42a2 adr r2, #264 address 0x55E0 -> 0x000055E0 */
L_54d8:
    adr r1, L_55e4                          /* @54D8 42a1 adr r1, #264 address 0x55E4 -> 0x000055E4 'Cal...' */
L_54da:
    adr r0, L_55e0                          /* @54DA 41a0 adr r0, #260 address 0x55E0 -> 0x000055E0 */
L_54dc:
    bl L_4884                               /* @54DC fff7d2f9 bl #-3164 -> display_three_lines */
L_54e0:
    bl L_3cac                               /* @54E0 fef7e4fb bl #-6200 -> sub_3CAC */
L_54e4:
    bl L_34a4                               /* @54E4 fdf7deff bl #-8260 -> sub_34A4 */
L_54e8:
    ldr r1, [pc, #256]                      /* @54E8 4049 ldr r1, [pc, #256] literal 0x55EC -> 0x00000BB8 */
L_54ea:
    movs r0, #0                             /* @54EA 0020 movs r0, #0  */
L_54ec:
    bl L_3470                               /* @54EC fdf7c0ff bl #-8320 -> sub_3470 */
L_54f0:
    bl L_3b9c                               /* @54F0 fef754fb bl #-6488 -> sub_3B9C */
L_54f4:
    movs r0, #75                            /* @54F4 4b20 movs r0, #75  */
L_54f6:
    lsls r0, r0, #4                         /* @54F6 0001 lsls r0, r0, #4  */
L_54f8:
    bl L_2de8                               /* @54F8 fdf776fc bl #-10004 -> delay_ms_candidate */
L_54fc:
    ldr r1, [pc, #240]                      /* @54FC 3c49 ldr r1, [pc, #240] literal 0x55F0 -> 0x20000600 */
L_54fe:
    movs r0, #0                             /* @54FE 0020 movs r0, #0  */
L_5500:
    str r0, [r1]                            /* @5500 0860 str r0, [r1]  */
L_5502:
    ldr r7, [pc, #240]                      /* @5502 3c4f ldr r7, [pc, #240] literal 0x55F4 -> 0x20000748 */
L_5504:
    b L_551a                                /* @5504 09e0 b #18 -> 0x551A */
L_5506:
    movs r0, #102                           /* @5506 6620 movs r0, #102  */
L_5508:
    bl L_5a38                               /* @5508 00f096fa bl #1324 -> sub_5A38 */
L_550c:
    lsls r1, r4, #1                         /* @550C 6100 lsls r1, r4, #1  */
L_550e:
    strh r0, [r7, r1]                       /* @550E 7852 strh r0, [r7, r1]  */
L_5510:
    bl L_3534                               /* @5510 fef710f8 bl #-8160 -> sub_3534 */
L_5514:
    adds r4, r4, #1                         /* @5514 641c adds r4, r4, #1  */
L_5516:
    cmp r4, #200                            /* @5516 c82c cmp r4, #200  */
L_5518:
    beq L_5552                              /* @5518 1bd0 beq #54 -> 0x5552 */
L_551a:
    cmp r4, #200                            /* @551A c82c cmp r4, #200  */
L_551c:
    blt L_5506                              /* @551C f3db blt #-26 -> 0x5506 */
L_551e:
    cmp r6, #0                              /* @551E 002e cmp r6, #0  */
L_5520:
    beq L_5588                              /* @5520 32d0 beq #100 -> 0x5588 */
L_5522:
    movs r5, #200                           /* @5522 c825 movs r5, #200  */
L_5524:
    bl L_34c0                               /* @5524 fdf7ccff bl #-8296 -> sub_34C0 */
L_5528:
    movs r0, #102                           /* @5528 6620 movs r0, #102  */
L_552a:
    bl L_5a38                               /* @552A 00f085fa bl #1290 -> sub_5A38 */
L_552e:
    mov r7, r0                              /* @552E 0746 mov r7, r0  */
L_5530:
    subs r0, r5, #1                         /* @5530 681e subs r0, r5, #1  */
L_5532:
    lsls r0, r0, #1                         /* @5532 4000 lsls r0, r0, #1  */
L_5534:
    ldr r1, [pc, #188]                      /* @5534 2f49 ldr r1, [pc, #188] literal 0x55F4 -> 0x20000748 */
L_5536:
    str r0, [sp]                            /* @5536 0090 str r0, [sp]  */
L_5538:
    ldrh r4, [r1, r0]                       /* @5538 0c5a ldrh r4, [r1, r0]  */
L_553a:
    subs r0, r4, r7                         /* @553A e01b subs r0, r4, r7  */
L_553c:
    bl L_373a                               /* @553C fef7fdf8 bl #-7686 -> abs32_candidate */
L_5540:
    movs r1, #1                             /* @5540 0121 movs r1, #1  */
L_5542:
    lsls r1, r1, #13                        /* @5542 4903 lsls r1, r1, #13  */
L_5544:
    cmp r0, r1                              /* @5544 8842 cmp r0, r1  */
L_5546:
    ble L_555c                              /* @5546 09dd ble #18 -> 0x555C */
L_5548:
    cmp r7, r4                              /* @5548 a742 cmp r7, r4  */
L_554a:
    bge L_5556                              /* @554A 04da bge #8 -> 0x5556 */
L_554c:
    lsls r0, r1, #1                         /* @554C 4800 lsls r0, r1, #1  */
L_554e:
    adds r7, r7, r0                         /* @554E 3f18 adds r7, r7, r0  */
L_5550:
    b L_555c                                /* @5550 04e0 b #8 -> 0x555C */
L_5552:
    movs r6, #1                             /* @5552 0126 movs r6, #1  */
L_5554:
    b L_551e                                /* @5554 e3e7 b #-58 -> 0x551E */
L_5556:
    movs r0, #1                             /* @5556 0120 movs r0, #1  */
L_5558:
    lsls r0, r0, #14                        /* @5558 8003 lsls r0, r0, #14  */
L_555a:
    adds r4, r4, r0                         /* @555A 2418 adds r4, r4, r0  */
L_555c:
    adds r0, r7, r4                         /* @555C 3819 adds r0, r7, r4  */
L_555e:
    lsrs r1, r0, #31                        /* @555E c10f lsrs r1, r0, #31  */
L_5560:
    adds r0, r1, r0                         /* @5560 0818 adds r0, r1, r0  */
L_5562:
    asrs r1, r0, #1                         /* @5562 4110 asrs r1, r0, #1  */
L_5564:
    ldr r0, [pc, #144]                      /* @5564 2448 ldr r0, [pc, #144] literal 0x55F8 -> 0x00003FFF */
L_5566:
    cmp r1, r0                              /* @5566 8142 cmp r1, r0  */
L_5568:
    ble L_5570                              /* @5568 02dd ble #4 -> 0x5570 */
L_556a:
    adds r0, r0, #1                         /* @556A 401c adds r0, r0, #1  */
L_556c:
    subs r1, r1, r0                         /* @556C 091a subs r1, r1, r0  */
L_556e:
    b L_557a                                /* @556E 04e0 b #8 -> 0x557A */
L_5570:
    cmp r1, #0                              /* @5570 0029 cmp r1, #0  */
L_5572:
    bge L_557a                              /* @5572 02da bge #4 -> 0x557A */
L_5574:
    movs r0, #1                             /* @5574 0120 movs r0, #1  */
L_5576:
    lsls r0, r0, #14                        /* @5576 8003 lsls r0, r0, #14  */
L_5578:
    adds r1, r1, r0                         /* @5578 0918 adds r1, r1, r0  */
L_557a:
    ldr r2, [pc, #120]                      /* @557A 1e4a ldr r2, [pc, #120] literal 0x55F4 -> 0x20000748 */
L_557c:
    ldr r0, [sp]                            /* @557C 0098 ldr r0, [sp]  */
L_557e:
    strh r1, [r2, r0]                       /* @557E 1152 strh r1, [r2, r0]  */
L_5580:
    subs r5, r5, #1                         /* @5580 6d1e subs r5, r5, #1  */
L_5582:
    cmp r5, #0                              /* @5582 002d cmp r5, #0  */
L_5584:
    beq L_55d2                              /* @5584 25d0 beq #74 -> 0x55D2 */
L_5586:
    bgt L_5524                              /* @5586 cddc bgt #-102 -> 0x5524 */
L_5588:
    ldr r0, [pc, #100]                      /* @5588 1948 ldr r0, [pc, #100] literal 0x55F0 -> 0x20000600 */
L_558a:
    movs r4, #0                             /* @558A 0024 movs r4, #0  */
L_558c:
    str r4, [r0]                            /* @558C 0460 str r4, [r0]  */
L_558e:
    ldr r5, [pc, #108]                      /* @558E 1b4d ldr r5, [pc, #108] literal 0x55FC -> 0x40005800 */
L_5590:
    cmp r6, #0                              /* @5590 002e cmp r6, #0  */
L_5592:
    beq L_55c0                              /* @5592 15d0 beq #42 -> 0x55C0 */
L_5594:
    ldr r1, [pc, #84]                       /* @5594 1549 ldr r1, [pc, #84] literal 0x55EC -> 0x00000BB8 */
L_5596:
    movs r0, #0                             /* @5596 0020 movs r0, #0  */
L_5598:
    bl L_3470                               /* @5598 fdf76aff bl #-8492 -> sub_3470 */
L_559c:
    movs r0, #75                            /* @559C 4b20 movs r0, #75  */
L_559e:
    lsls r0, r0, #4                         /* @559E 0001 lsls r0, r0, #4  */
L_55a0:
    bl L_2de8                               /* @55A0 fdf722fc bl #-10172 -> delay_ms_candidate */
L_55a4:
    bl L_3b70                               /* @55A4 fef7e4fa bl #-6712 -> sub_3B70 */
L_55a8:
    bl L_279c                               /* @55A8 fdf7f8f8 bl #-11792 -> sub_279C */
L_55ac:
    ldr r0, [r5, #12]                       /* @55AC e868 ldr r0, [r5, #12]  */
L_55ae:
    lsls r0, r0, #31                        /* @55AE c007 lsls r0, r0, #31  */
L_55b0:
    beq L_55c0                              /* @55B0 06d0 beq #12 -> 0x55C0 */
L_55b2:
    ldr r0, [pc, #76]                       /* @55B2 1348 ldr r0, [pc, #76] literal 0x5600 -> 0x20000648 */
L_55b4:
    strb r4, [r0]                           /* @55B4 0470 strb r4, [r0]  */
L_55b6:
    ldr r0, [pc, #76]                       /* @55B6 1348 ldr r0, [pc, #76] literal 0x5604 -> 0x20000358 */
L_55b8:
    bl L_4574                               /* @55B8 fef7dcff bl #-4168 -> sub_4574 */
L_55bc:
    ldr r0, [pc, #72]                       /* @55BC 1248 ldr r0, [pc, #72] literal 0x5608 -> 0x20000628 */
L_55be:
    strb r4, [r0, #2]                       /* @55BE 8470 strb r4, [r0, #2]  */
L_55c0:
    movs r0, #0                             /* @55C0 0020 movs r0, #0  */
L_55c2:
    bl L_3f28                               /* @55C2 fef7b1fc bl #-5790 -> sub_3F28 */
L_55c6:
    ldr r0, [r5, #12]                       /* @55C6 e868 ldr r0, [r5, #12]  */
L_55c8:
    lsls r0, r0, #31                        /* @55C8 c007 lsls r0, r0, #31  */
L_55ca:
    beq L_55d6                              /* @55CA 04d0 beq #8 -> 0x55D6 */
L_55cc:
    bl L_58a0                               /* @55CC 00f068f9 bl #720 -> sub_58A0 */
L_55d0:
    b L_55da                                /* @55D0 03e0 b #6 -> 0x55DA */
L_55d2:
    movs r6, #1                             /* @55D2 0126 movs r6, #1  */
L_55d4:
    b L_5588                                /* @55D4 d8e7 b #-80 -> 0x5588 */
L_55d6:
    bl L_342c                               /* @55D6 fdf729ff bl #-8622 -> sub_342C */
L_55da:
    mov r0, r6                              /* @55DA 3046 mov r0, r6  */
L_55dc:
    pop {r3, r4, r5, r6, r7, pc}            /* @55DC f8bd pop {r3, r4, r5, r6, r7, pc}  */
    .byte 0x00,0x00 /* @55DE unclassified */
L_55e0:
    .byte 0x00,0x00,0x00,0x00 /* @55E0 unclassified */
L_55e4:
    .byte 0x43,0x61,0x6c,0x2e,0x2e,0x2e,0x00,0x00 /* @55E4 unclassified */
L_55ec:
    .byte 0xb8,0x0b,0x00,0x00 /* @55EC literal */
L_55f0:
    .byte 0x00,0x06,0x00,0x20 /* @55F0 literal */
L_55f4:
    .byte 0x48,0x07,0x00,0x20 /* @55F4 literal */
L_55f8:
    .byte 0xff,0x3f,0x00,0x00 /* @55F8 literal */
L_55fc:
    .byte 0x00,0x58,0x00,0x40 /* @55FC literal */
L_5600:
    .byte 0x48,0x06,0x00,0x20 /* @5600 literal */
L_5604:
    .byte 0x58,0x03,0x00,0x20 /* @5604 literal */
L_5608:
    .byte 0x28,0x06,0x00,0x20 /* @5608 literal */

/* sub_560C */
L_560c:
    push {r4, r5, r6, lr}                   /* @560C 70b5 push {r4, r5, r6, lr}  */
L_560e:
    bl L_239c                               /* @560E fcf7c5fe bl #-12918 -> sub_239C */
L_5612:
    movs r0, #3                             /* @5612 0320 movs r0, #3  */
L_5614:
    bl L_2de8                               /* @5614 fdf7e8fb bl #-10288 -> delay_ms_candidate */
L_5618:
    ldr r4, [pc, #52]                       /* @5618 0d4c ldr r4, [pc, #52] literal 0x5650 -> 0x200006E0 */
L_561a:
    ldr r5, [pc, #56]                       /* @561A 0e4d ldr r5, [pc, #56] literal 0x5654 -> 0x0000FFFF */
L_561c:
    b L_5634                                /* @561C 0ae0 b #20 -> 0x5634 */
L_561e:
    ldrh r0, [r4, #2]                       /* @561E 6088 ldrh r0, [r4, #2]  */
L_5620:
    cmp r0, r5                              /* @5620 a842 cmp r0, r5  */
L_5622:
    bne L_5634                              /* @5622 07d1 bne #14 -> 0x5634 */
L_5624:
    adr r2, L_5658                          /* @5624 0ca2 adr r2, #48 address 0x5658 -> 0x00005658 'Power!' */
L_5626:
    adr r1, L_5660                          /* @5626 0ea1 adr r1, #56 address 0x5660 -> 0x00005660 ' v+' */
L_5628:
    adr r0, L_5664                          /* @5628 0ea0 adr r0, #56 address 0x5664 -> 0x00005664 'Waiting' */
L_562a:
    bl L_4884                               /* @562A fff72bf9 bl #-3498 -> display_three_lines */
L_562e:
    ldrb r0, [r4]                           /* @562E 2078 ldrb r0, [r4]  */
L_5630:
    cmp r0, #0                              /* @5630 0028 cmp r0, #0  */
L_5632:
    beq L_562e                              /* @5632 fcd0 beq #-8 -> 0x562E */
L_5634:
    ldrb r0, [r4]                           /* @5634 2078 ldrb r0, [r4]  */
L_5636:
    cmp r0, #0                              /* @5636 0028 cmp r0, #0  */
L_5638:
    beq L_561e                              /* @5638 f1d0 beq #-30 -> 0x561E */
L_563a:
    ldr r0, [r4, #12]                       /* @563A e068 ldr r0, [r4, #12]  */
L_563c:
    cmp r0, #35                             /* @563C 2328 cmp r0, #35  */
L_563e:
    ble L_564a                              /* @563E 04dd ble #8 -> 0x564A */
L_5640:
    ldr r0, [r4, #16]                       /* @5640 2069 ldr r0, [r4, #16]  */
L_5642:
    cmp r0, #35                             /* @5642 2328 cmp r0, #35  */
L_5644:
    ble L_564a                              /* @5644 01dd ble #2 -> 0x564A */
L_5646:
    movs r0, #1                             /* @5646 0120 movs r0, #1  */
L_5648:
    pop {r4, r5, r6, pc}                    /* @5648 70bd pop {r4, r5, r6, pc}  */
L_564a:
    movs r0, #0                             /* @564A 0020 movs r0, #0  */
L_564c:
    pop {r4, r5, r6, pc}                    /* @564C 70bd pop {r4, r5, r6, pc}  */
    .byte 0x00,0x00 /* @564E unclassified */
L_5650:
    .byte 0xe0,0x06,0x00,0x20 /* @5650 literal */
L_5654:
    .byte 0xff,0xff,0x00,0x00 /* @5654 literal */
L_5658:
    .byte 0x50,0x6f,0x77,0x65,0x72,0x21,0x00,0x00 /* @5658 unclassified */
L_5660:
    .byte 0x20,0x76,0x2b,0x00 /* @5660 unclassified */
L_5664:
    .byte 0x57,0x61,0x69,0x74,0x69,0x6e,0x67,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @5664 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @5674 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @5684 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @5694 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @56A4 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @56B4 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @56C4 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @56D4 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @56E4 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @56F4 unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @5704 unclassified */

/* sub_570C */
L_570c:
    push {r4, r5, r6, lr}                   /* @570C 70b5 push {r4, r5, r6, lr}  */
L_570e:
    mov r5, r0                              /* @570E 0546 mov r5, r0  */
L_5710:
    movs r0, #0                             /* @5710 0020 movs r0, #0  */
L_5712:
    bl L_3f28                               /* @5712 fef709fc bl #-6126 -> sub_3F28 */
L_5716:
    bl L_3cac                               /* @5716 fef7c9fa bl #-6766 -> sub_3CAC */
L_571a:
    movs r1, #1                             /* @571A 0121 movs r1, #1  */
L_571c:
    cmp r5, #2                              /* @571C 022d cmp r5, #2  */
L_571e:
    beq L_5724                              /* @571E 01d0 beq #2 -> 0x5724 */
L_5720:
    ldr r0, [pc, #180]                      /* @5720 2d48 ldr r0, [pc, #180] literal 0x57D8 -> 0x20000604 */
L_5722:
    strb r1, [r0, #3]                       /* @5722 c170 strb r1, [r0, #3]  */
L_5724:
    movs r4, #0                             /* @5724 0024 movs r4, #0  */
L_5726:
    ldr r0, [pc, #180]                      /* @5726 2d48 ldr r0, [pc, #180] literal 0x57DC -> 0x200006C0 */
L_5728:
    cmp r5, #0                              /* @5728 002d cmp r5, #0  */
L_572a:
    beq L_57c8                              /* @572A 4dd0 beq #154 -> 0x57C8 */
L_572c:
    strb r1, [r0]                           /* @572C 0170 strb r1, [r0]  */
L_572e:
    bl L_5030                               /* @572E fff77ffc bl #-1794 -> sub_5030 */
L_5732:
    movs r6, #75                            /* @5732 4b26 movs r6, #75  */
L_5734:
    lsls r6, r6, #4                         /* @5734 3601 lsls r6, r6, #4  */
L_5736:
    ldr r5, [pc, #168]                      /* @5736 2a4d ldr r5, [pc, #168] literal 0x57E0 -> 0x20000628 */
L_5738:
    cmp r0, #0                              /* @5738 0028 cmp r0, #0  */
L_573a:
    beq L_57a0                              /* @573A 31d0 beq #98 -> 0x57A0 */
L_573c:
    ldr r0, [pc, #164]                      /* @573C 2948 ldr r0, [pc, #164] literal 0x57E4 -> 0x20000620 */
L_573e:
    ldrb r1, [r0]                           /* @573E 0178 ldrb r1, [r0]  */
L_5740:
    cmp r1, #1                              /* @5740 0129 cmp r1, #1  */
L_5742:
    beq L_5770                              /* @5742 15d0 beq #42 -> 0x5770 */
L_5744:
    cmp r1, #2                              /* @5744 0229 cmp r1, #2  */
L_5746:
    beq L_577c                              /* @5746 19d0 beq #50 -> 0x577C */
L_5748:
    cmp r1, #3                              /* @5748 0329 cmp r1, #3  */
L_574a:
    beq L_5788                              /* @574A 1dd0 beq #58 -> 0x5788 */
L_574c:
    cmp r1, #4                              /* @574C 0429 cmp r1, #4  */
L_574e:
    beq L_5794                              /* @574E 21d0 beq #66 -> 0x5794 */
L_5750:
    adr r2, L_57e8                          /* @5750 25a2 adr r2, #148 address 0x57E8 -> 0x000057E8 'Enter..' */
L_5752:
    adr r1, L_57f0                          /* @5752 27a1 adr r1, #156 address 0x57F0 -> 0x000057F0 ' Error!' */
L_5754:
    adr r0, L_57f8                          /* @5754 28a0 adr r0, #160 address 0x57F8 -> 0x000057F8 'Encoder' */
L_5756:
    bl L_4884                               /* @5756 fff795f8 bl #-3798 -> display_three_lines */
L_575a:
    mov r0, r6                              /* @575A 3046 mov r0, r6  */
L_575c:
    bl L_2de8                               /* @575C fdf744fb bl #-10616 -> delay_ms_candidate */
L_5760:
    str r4, [r5, #8]                        /* @5760 ac60 str r4, [r5, #8]  */
L_5762:
    ldr r0, [pc, #156]                      /* @5762 2748 ldr r0, [pc, #156] literal 0x5800 -> 0x20000560 */
L_5764:
    bl L_4574                               /* @5764 fef706ff bl #-4596 -> sub_4574 */
L_5768:
    bl L_4568                               /* @5768 fef7fefe bl #-4612 -> sub_4568 */
L_576c:
    movs r0, #3                             /* @576C 0320 movs r0, #3  */
L_576e:
    pop {r4, r5, r6, pc}                    /* @576E 70bd pop {r4, r5, r6, pc}  */
L_5770:
    adr r2, L_57e8                          /* @5770 1da2 adr r2, #116 address 0x57E8 -> 0x000057E8 'Enter..' */
L_5772:
    adr r1, L_5804                          /* @5772 24a1 adr r1, #144 address 0x5804 -> 0x00005804 ' Strong!' */
L_5774:
    adr r0, L_5810                          /* @5774 26a0 adr r0, #152 address 0x5810 -> 0x00005810 'Magnet' */
L_5776:
    bl L_4884                               /* @5776 fff785f8 bl #-3830 -> display_three_lines */
L_577a:
    b L_575a                                /* @577A eee7 b #-36 -> 0x575A */
L_577c:
    adr r2, L_57e8                          /* @577C 1aa2 adr r2, #104 address 0x57E8 -> 0x000057E8 'Enter..' */
L_577e:
    adr r1, L_5818                          /* @577E 26a1 adr r1, #152 address 0x5818 -> 0x00005818 ' Weak!' */
L_5780:
    adr r0, L_5810                          /* @5780 23a0 adr r0, #140 address 0x5810 -> 0x00005810 'Magnet' */
L_5782:
    bl L_4884                               /* @5782 fff77ff8 bl #-3842 -> display_three_lines */
L_5786:
    b L_575a                                /* @5786 e8e7 b #-48 -> 0x575A */
L_5788:
    adr r2, L_57e8                          /* @5788 17a2 adr r2, #92 address 0x57E8 -> 0x000057E8 'Enter..' */
L_578a:
    adr r1, L_5820                          /* @578A 25a1 adr r1, #148 address 0x5820 -> 0x00005820 ' Loss!' */
L_578c:
    adr r0, L_5810                          /* @578C 20a0 adr r0, #128 address 0x5810 -> 0x00005810 'Magnet' */
L_578e:
    bl L_4884                               /* @578E fff779f8 bl #-3854 -> display_three_lines */
L_5792:
    b L_575a                                /* @5792 e2e7 b #-60 -> 0x575A */
L_5794:
    adr r2, L_57e8                          /* @5794 14a2 adr r2, #80 address 0x57E8 -> 0x000057E8 'Enter..' */
L_5796:
    adr r1, L_57f0                          /* @5796 16a1 adr r1, #88 address 0x57F0 -> 0x000057F0 ' Error!' */
L_5798:
    adr r0, L_57f8                          /* @5798 17a0 adr r0, #92 address 0x57F8 -> 0x000057F8 'Encoder' */
L_579a:
    bl L_4884                               /* @579A fff773f8 bl #-3866 -> display_three_lines */
L_579e:
    b L_575a                                /* @579E dce7 b #-72 -> 0x575A */
L_57a0:
    bl L_267c                               /* @57A0 fcf76cff bl #-12584 -> load_calibration_knots */
L_57a4:
    cmp r0, #0                              /* @57A4 0028 cmp r0, #0  */
L_57a6:
    bne L_57ce                              /* @57A6 12d1 bne #36 -> 0x57CE */
L_57a8:
    adr r2, L_580c                          /* @57A8 18a2 adr r2, #96 address 0x580C -> 0x0000580C */
L_57aa:
    adr r1, L_5828                          /* @57AA 1fa1 adr r1, #124 address 0x5828 -> 0x00005828 'Not Cal' */
L_57ac:
    adr r0, L_580c                          /* @57AC 17a0 adr r0, #92 address 0x580C -> 0x0000580C */
L_57ae:
    bl L_4884                               /* @57AE fff769f8 bl #-3886 -> display_three_lines */
L_57b2:
    mov r0, r6                              /* @57B2 3046 mov r0, r6  */
L_57b4:
    bl L_2de8                               /* @57B4 fdf718fb bl #-10704 -> delay_ms_candidate */
L_57b8:
    str r4, [r5, #8]                        /* @57B8 ac60 str r4, [r5, #8]  */
L_57ba:
    ldr r0, [pc, #116]                      /* @57BA 1d48 ldr r0, [pc, #116] literal 0x5830 -> 0x200004FC */
L_57bc:
    bl L_4574                               /* @57BC fef7dafe bl #-4684 -> sub_4574 */
L_57c0:
    bl L_4568                               /* @57C0 fef7d2fe bl #-4700 -> sub_4568 */
L_57c4:
    movs r0, #2                             /* @57C4 0220 movs r0, #2  */
L_57c6:
    pop {r4, r5, r6, pc}                    /* @57C6 70bd pop {r4, r5, r6, pc}  */
L_57c8:
    ldr r1, [pc, #104]                      /* @57C8 1a49 ldr r1, [pc, #104] literal 0x5834 -> 0x20000648 */
L_57ca:
    str r4, [r1, #36]                       /* @57CA 4c62 str r4, [r1, #36]  */
L_57cc:
    strb r4, [r0]                           /* @57CC 0470 strb r4, [r0]  */
L_57ce:
    ldr r0, [pc, #104]                      /* @57CE 1a48 ldr r0, [pc, #104] literal 0x5838 -> 0x20000358 */
L_57d0:
    bl L_4574                               /* @57D0 fef7d0fe bl #-4704 -> sub_4574 */
L_57d4:
    movs r0, #0                             /* @57D4 0020 movs r0, #0  */
L_57d6:
    pop {r4, r5, r6, pc}                    /* @57D6 70bd pop {r4, r5, r6, pc}  */
L_57d8:
    .byte 0x04,0x06,0x00,0x20 /* @57D8 literal */
L_57dc:
    .byte 0xc0,0x06,0x00,0x20 /* @57DC literal */
L_57e0:
    .byte 0x28,0x06,0x00,0x20 /* @57E0 literal */
L_57e4:
    .byte 0x20,0x06,0x00,0x20 /* @57E4 literal */
L_57e8:
    .byte 0x45,0x6e,0x74,0x65,0x72,0x2e,0x2e,0x00 /* @57E8 unclassified */
L_57f0:
    .byte 0x20,0x45,0x72,0x72,0x6f,0x72,0x21,0x00 /* @57F0 unclassified */
L_57f8:
    .byte 0x45,0x6e,0x63,0x6f,0x64,0x65,0x72,0x00 /* @57F8 unclassified */
L_5800:
    .byte 0x60,0x05,0x00,0x20 /* @5800 literal */
L_5804:
    .byte 0x20,0x53,0x74,0x72,0x6f,0x6e,0x67,0x21 /* @5804 unclassified */
L_580c:
    .byte 0x00,0x00,0x00,0x00 /* @580C unclassified */
L_5810:
    .byte 0x4d,0x61,0x67,0x6e,0x65,0x74,0x00,0x00 /* @5810 unclassified */
L_5818:
    .byte 0x20,0x57,0x65,0x61,0x6b,0x21,0x00,0x00 /* @5818 unclassified */
L_5820:
    .byte 0x20,0x4c,0x6f,0x73,0x73,0x21,0x00,0x00 /* @5820 unclassified */
L_5828:
    .byte 0x4e,0x6f,0x74,0x20,0x43,0x61,0x6c,0x00 /* @5828 unclassified */
L_5830:
    .byte 0xfc,0x04,0x00,0x20 /* @5830 literal */
L_5834:
    .byte 0x48,0x06,0x00,0x20 /* @5834 literal */
L_5838:
    .byte 0x58,0x03,0x00,0x20 /* @5838 literal */

/* sub_583C */
L_583c:
    ldr r0, [pc, #44]                       /* @583C 0b48 ldr r0, [pc, #44] literal 0x586C -> 0x40003080 */
L_583e:
    movs r1, #0                             /* @583E 0021 movs r1, #0  */
L_5840:
    str r1, [r0, #8]                        /* @5840 8160 str r1, [r0, #8]  */
L_5842:
    str r1, [r0, #12]                       /* @5842 c160 str r1, [r0, #12]  */
L_5844:
    ldr r1, [pc, #40]                       /* @5844 0a49 ldr r1, [pc, #40] literal 0x5870 -> 0x2000072C */
L_5846:
    ldrb r2, [r1, #4]                       /* @5846 0a79 ldrb r2, [r1, #4]  */
L_5848:
    movs r3, #64                            /* @5848 4023 movs r3, #64  */
L_584a:
    movs r1, #16                            /* @584A 1021 movs r1, #16  */
L_584c:
    cmp r2, #0                              /* @584C 002a cmp r2, #0  */
L_584e:
    beq L_585e                              /* @584E 06d0 beq #12 -> 0x585E */
L_5850:
    ldr r2, [r0, #8]                        /* @5850 8268 ldr r2, [r0, #8]  */
L_5852:
    orrs r2, r3                             /* @5852 1a43 orrs r2, r3  */
L_5854:
    str r2, [r0, #8]                        /* @5854 8260 str r2, [r0, #8]  */
L_5856:
    ldr r2, [r0, #12]                       /* @5856 c268 ldr r2, [r0, #12]  */
L_5858:
    orrs r2, r1                             /* @5858 0a43 orrs r2, r1  */
L_585a:
    str r2, [r0, #12]                       /* @585A c260 str r2, [r0, #12]  */
L_585c:
    bx lr                                   /* @585C 7047 bx lr  */
L_585e:
    ldr r2, [r0, #8]                        /* @585E 8268 ldr r2, [r0, #8]  */
L_5860:
    orrs r2, r1                             /* @5860 0a43 orrs r2, r1  */
L_5862:
    str r2, [r0, #8]                        /* @5862 8260 str r2, [r0, #8]  */
L_5864:
    ldr r1, [r0, #12]                       /* @5864 c168 ldr r1, [r0, #12]  */
L_5866:
    orrs r1, r3                             /* @5866 1943 orrs r1, r3  */
L_5868:
    str r1, [r0, #12]                       /* @5868 c160 str r1, [r0, #12]  */
L_586a:
    bx lr                                   /* @586A 7047 bx lr  */
L_586c:
    .byte 0x80,0x30,0x00,0x40 /* @586C literal */
L_5870:
    .byte 0x2c,0x07,0x00,0x20 /* @5870 literal */

/* sub_5874 */
L_5874:
    push {r4, lr}                           /* @5874 10b5 push {r4, lr}  */
L_5876:
    bl L_3b70                               /* @5876 fef77bf9 bl #-7434 -> sub_3B70 */
L_587a:
    bl L_3dd4                               /* @587A fef7abfa bl #-6826 -> sub_3DD4 */
L_587e:
    bl L_588c                               /* @587E 00f005f8 bl #10 -> sub_588C */
L_5882:
    pop {r4, pc}                            /* @5882 10bd pop {r4, pc}  */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @5884 unclassified */

/* sub_588C */
L_588c:
    ldr r0, [pc, #12]                       /* @588C 0348 ldr r0, [pc, #12] literal 0x589C -> 0x40005800 */
L_588e:
    ldr r1, [r0, #12]                       /* @588E c168 ldr r1, [r0, #12]  */
L_5890:
    asrs r2, r0, #20                        /* @5890 0215 asrs r2, r0, #20  */
L_5892:
    bics r1, r2                             /* @5892 9143 bics r1, r2  */
L_5894:
    str r1, [r0, #12]                       /* @5894 c160 str r1, [r0, #12]  */
L_5896:
    movs r1, #254                           /* @5896 fe21 movs r1, #254  */
L_5898:
    str r1, [r0, #20]                       /* @5898 4161 str r1, [r0, #20]  */
L_589a:
    bx lr                                   /* @589A 7047 bx lr  */
L_589c:
    .byte 0x00,0x58,0x00,0x40 /* @589C literal */

/* sub_58A0 */
L_58a0:
    push {r4, lr}                           /* @58A0 10b5 push {r4, lr}  */
L_58a2:
    bl L_2090                               /* @58A2 fcf7f5fb bl #-14358 -> sub_2090 */
L_58a6:
    ldr r1, [pc, #72]                       /* @58A6 1249 ldr r1, [pc, #72] literal 0x58F0 -> 0x2000072C */
L_58a8:
    ldrb r0, [r1, #9]                       /* @58A8 487a ldrb r0, [r1, #9]  */
L_58aa:
    cmp r0, #2                              /* @58AA 0228 cmp r0, #2  */
L_58ac:
    beq L_58b4                              /* @58AC 02d0 beq #4 -> 0x58B4 */
L_58ae:
    bl L_3e54                               /* @58AE fef7d1fa bl #-6750 -> sub_3E54 */
L_58b2:
    b L_58e4                                /* @58B2 17e0 b #46 -> 0x58E4 */
L_58b4:
    ldr r0, [pc, #60]                       /* @58B4 0f48 ldr r0, [pc, #60] literal 0x58F4 -> 0x20000013 */
L_58b6:
    ldrb r0, [r0]                           /* @58B6 0078 ldrb r0, [r0]  */
L_58b8:
    cmp r0, #1                              /* @58B8 0128 cmp r0, #1  */
L_58ba:
    bne L_58e4                              /* @58BA 13d1 bne #38 -> 0x58E4 */
L_58bc:
    ldrh r2, [r1, #22]                      /* @58BC ca8a ldrh r2, [r1, #22]  */
L_58be:
    ldr r0, [pc, #56]                       /* @58BE 0e48 ldr r0, [pc, #56] literal 0x58F8 -> 0x0000FFFF */
L_58c0:
    cmp r2, r0                              /* @58C0 8242 cmp r2, r0  */
L_58c2:
    beq L_58e4                              /* @58C2 0fd0 beq #30 -> 0x58E4 */
L_58c4:
    ldr r2, [pc, #52]                       /* @58C4 0d4a ldr r2, [pc, #52] literal 0x58FC -> 0x2000061C */
L_58c6:
    movs r0, #0                             /* @58C6 0020 movs r0, #0  */
L_58c8:
    str r0, [r2]                            /* @58C8 1060 str r0, [r2]  */
L_58ca:
    ldr r0, [pc, #52]                       /* @58CA 0d48 ldr r0, [pc, #52] literal 0x5900 -> 0x20000610 */
L_58cc:
    movs r3, #0                             /* @58CC 0023 movs r3, #0  */
L_58ce:
    str r3, [r0]                            /* @58CE 0360 str r3, [r0]  */
L_58d0:
    str r3, [r0, #4]                        /* @58D0 4360 str r3, [r0, #4]  */
L_58d2:
    ldrh r0, [r1, #22]                      /* @58D2 c88a ldrh r0, [r1, #22]  */
L_58d4:
    ldr r2, [pc, #44]                       /* @58D4 0b4a ldr r2, [pc, #44] literal 0x5904 -> 0x20000608 */
L_58d6:
    lsrs r0, r0, #15                        /* @58D6 c00b lsrs r0, r0, #15  */
L_58d8:
    strb r0, [r2]                           /* @58D8 1070 strb r0, [r2]  */
L_58da:
    ldrh r0, [r1, #22]                      /* @58DA c88a ldrh r0, [r1, #22]  */
L_58dc:
    ldr r1, [pc, #40]                       /* @58DC 0a49 ldr r1, [pc, #40] literal 0x5908 -> 0x20000618 */
L_58de:
    lsls r0, r0, #17                        /* @58DE 4004 lsls r0, r0, #17  */
L_58e0:
    lsrs r0, r0, #17                        /* @58E0 400c lsrs r0, r0, #17  */
L_58e2:
    strh r0, [r1]                           /* @58E2 0880 strh r0, [r1]  */
L_58e4:
    bl L_3040                               /* @58E4 fdf7acfb bl #-10408 -> sub_3040 */
L_58e8:
    bl L_0d3c                               /* @58E8 fbf728fa bl #-19376 -> sub_0D3C */
L_58ec:
    pop {r4, pc}                            /* @58EC 10bd pop {r4, pc}  */
    .byte 0x00,0x00 /* @58EE unclassified */
L_58f0:
    .byte 0x2c,0x07,0x00,0x20 /* @58F0 literal */
L_58f4:
    .byte 0x13,0x00,0x00,0x20 /* @58F4 literal */
L_58f8:
    .byte 0xff,0xff,0x00,0x00 /* @58F8 literal */
L_58fc:
    .byte 0x1c,0x06,0x00,0x20 /* @58FC literal */
L_5900:
    .byte 0x10,0x06,0x00,0x20 /* @5900 literal */
L_5904:
    .byte 0x08,0x06,0x00,0x20 /* @5904 literal */
L_5908:
    .byte 0x18,0x06,0x00,0x20 /* @5908 literal */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @590C unclassified */

/* sub_5918 */
L_5918:
    ldr r0, [pc, #12]                       /* @5918 0348 ldr r0, [pc, #12] literal 0x5928 -> 0x40005800 */
L_591a:
    movs r1, #254                           /* @591A fe21 movs r1, #254  */
L_591c:
    str r1, [r0, #20]                       /* @591C 4161 str r1, [r0, #20]  */
L_591e:
    ldr r1, [r0, #12]                       /* @591E c168 ldr r1, [r0, #12]  */
L_5920:
    asrs r2, r0, #20                        /* @5920 0215 asrs r2, r0, #20  */
L_5922:
    orrs r1, r2                             /* @5922 1143 orrs r1, r2  */
L_5924:
    str r1, [r0, #12]                       /* @5924 c160 str r1, [r0, #12]  */
L_5926:
    bx lr                                   /* @5926 7047 bx lr  */
L_5928:
    .byte 0x00,0x58,0x00,0x40 /* @5928 literal */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @592C unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @593C unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @594C unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @595C unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @596C unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @597C unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @598C unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @599C unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @59AC unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @59BC unclassified */

/* sub_59C8 */
L_59c8:
    push {r3, r4, r5, r6, r7, lr}           /* @59C8 f8b5 push {r3, r4, r5, r6, r7, lr}  */
L_59ca:
    ldr r4, [pc, #104]                      /* @59CA 1a4c ldr r4, [pc, #104] literal 0x5A34 -> 0x00000BB8 */
L_59cc:
    movs r0, #0                             /* @59CC 0020 movs r0, #0  */
L_59ce:
    mov r1, r4                              /* @59CE 2146 mov r1, r4  */
L_59d0:
    bl L_3470                               /* @59D0 fdf74efd bl #-9572 -> sub_3470 */
L_59d4:
    movs r5, #75                            /* @59D4 4b25 movs r5, #75  */
L_59d6:
    lsls r5, r5, #3                         /* @59D6 ed00 lsls r5, r5, #3  */
L_59d8:
    mov r0, r5                              /* @59D8 2846 mov r0, r5  */
L_59da:
    bl L_2de8                               /* @59DA fdf705fa bl #-11254 -> delay_ms_candidate */
L_59de:
    movs r0, #102                           /* @59DE 6620 movs r0, #102  */
L_59e0:
    bl L_5a38                               /* @59E0 00f02af8 bl #84 -> sub_5A38 */
L_59e4:
    mov r6, r0                              /* @59E4 0646 mov r6, r0  */
L_59e6:
    mov r1, r4                              /* @59E6 2146 mov r1, r4  */
L_59e8:
    movs r0, #128                           /* @59E8 8020 movs r0, #128  */
L_59ea:
    bl L_3470                               /* @59EA fdf741fd bl #-9598 -> sub_3470 */
L_59ee:
    movs r0, #120                           /* @59EE 7820 movs r0, #120  */
L_59f0:
    bl L_2de8                               /* @59F0 fdf7faf9 bl #-11276 -> delay_ms_candidate */
L_59f4:
    movs r0, #255                           /* @59F4 ff20 movs r0, #255  */
L_59f6:
    mov r1, r4                              /* @59F6 2146 mov r1, r4  */
L_59f8:
    adds r0, #1                             /* @59F8 0130 adds r0, #1  */
L_59fa:
    bl L_3470                               /* @59FA fdf739fd bl #-9614 -> sub_3470 */
L_59fe:
    mov r0, r5                              /* @59FE 2846 mov r0, r5  */
L_5a00:
    bl L_2de8                               /* @5A00 fdf7f2f9 bl #-11292 -> delay_ms_candidate */
L_5a04:
    movs r0, #102                           /* @5A04 6620 movs r0, #102  */
L_5a06:
    bl L_5a38                               /* @5A06 00f017f8 bl #46 -> sub_5A38 */
L_5a0a:
    mov r7, r0                              /* @5A0A 0746 mov r7, r0  */
L_5a0c:
    mov r1, r4                              /* @5A0C 2146 mov r1, r4  */
L_5a0e:
    movs r0, #128                           /* @5A0E 8020 movs r0, #128  */
L_5a10:
    bl L_3470                               /* @5A10 fdf72efd bl #-9636 -> sub_3470 */
L_5a14:
    movs r0, #120                           /* @5A14 7820 movs r0, #120  */
L_5a16:
    bl L_2de8                               /* @5A16 fdf7e7f9 bl #-11314 -> delay_ms_candidate */
L_5a1a:
    mov r1, r4                              /* @5A1A 2146 mov r1, r4  */
L_5a1c:
    movs r0, #0                             /* @5A1C 0020 movs r0, #0  */
L_5a1e:
    bl L_3470                               /* @5A1E fdf727fd bl #-9650 -> sub_3470 */
L_5a22:
    mov r0, r5                              /* @5A22 2846 mov r0, r5  */
L_5a24:
    bl L_2de8                               /* @5A24 fdf7e0f9 bl #-11328 -> delay_ms_candidate */
L_5a28:
    bl L_342c                               /* @5A28 fdf700fd bl #-9728 -> sub_342C */
L_5a2c:
    subs r0, r7, r6                         /* @5A2C b81b subs r0, r7, r6  */
L_5a2e:
    sxth r0, r0                             /* @5A2E 00b2 sxth r0, r0  */
L_5a30:
    pop {r3, r4, r5, r6, r7, pc}            /* @5A30 f8bd pop {r3, r4, r5, r6, r7, pc}  */
    .byte 0x00,0x00 /* @5A32 unclassified */
L_5a34:
    .byte 0xb8,0x0b,0x00,0x00 /* @5A34 literal */

/* sub_5A38 */
L_5a38:
    push {r0, r4, r5, r6, r7, lr}           /* @5A38 f1b5 push {r0, r4, r5, r6, r7, lr}  */
L_5a3a:
    sub sp, #8                              /* @5A3A 82b0 sub sp, #8  */
L_5a3c:
    movs r5, #0                             /* @5A3C 0025 movs r5, #0  */
L_5a3e:
    movs r7, #0                             /* @5A3E 0027 movs r7, #0  */
L_5a40:
    movs r0, #0                             /* @5A40 0020 movs r0, #0  */
L_5a42:
    str r0, [sp, #4]                        /* @5A42 0190 str r0, [sp, #4]  */
L_5a44:
    str r0, [sp]                            /* @5A44 0090 str r0, [sp]  */
L_5a46:
    movs r6, #0                             /* @5A46 0026 movs r6, #0  */
L_5a48:
    b L_5a94                                /* @5A48 24e0 b #72 -> 0x5A94 */
L_5a4a:
    bl L_51dc                               /* @5A4A fff7c7fb bl #-2162 -> sub_51DC */
L_5a4e:
    mov r4, r0                              /* @5A4E 0446 mov r4, r0  */
L_5a50:
    cmp r6, #0                              /* @5A50 002e cmp r6, #0  */
L_5a52:
    bne L_5a5a                              /* @5A52 02d1 bne #4 -> 0x5A5A */
L_5a54:
    str r4, [sp, #4]                        /* @5A54 0194 str r4, [sp, #4]  */
L_5a56:
    mov r7, r4                              /* @5A56 2746 mov r7, r4  */
L_5a58:
    mov r5, r4                              /* @5A58 2546 mov r5, r4  */
L_5a5a:
    subs r0, r5, r4                         /* @5A5A 281b subs r0, r5, r4  */
L_5a5c:
    bl L_373a                               /* @5A5C fdf76dfe bl #-8998 -> abs32_candidate */
L_5a60:
    movs r1, #1                             /* @5A60 0121 movs r1, #1  */
L_5a62:
    lsls r1, r1, #13                        /* @5A62 4903 lsls r1, r1, #13  */
L_5a64:
    cmp r0, r1                              /* @5A64 8842 cmp r0, r1  */
L_5a66:
    ble L_5a78                              /* @5A66 07dd ble #14 -> 0x5A78 */
L_5a68:
    cmp r5, r4                              /* @5A68 a542 cmp r5, r4  */
L_5a6a:
    ble L_5a72                              /* @5A6A 02dd ble #4 -> 0x5A72 */
L_5a6c:
    lsls r0, r1, #1                         /* @5A6C 4800 lsls r0, r1, #1  */
L_5a6e:
    adds r4, r4, r0                         /* @5A6E 2418 adds r4, r4, r0  */
L_5a70:
    b L_5a78                                /* @5A70 02e0 b #4 -> 0x5A78 */
L_5a72:
    movs r0, #1                             /* @5A72 0120 movs r0, #1  */
L_5a74:
    lsls r0, r0, #14                        /* @5A74 8003 lsls r0, r0, #14  */
L_5a76:
    subs r4, r4, r0                         /* @5A76 241a subs r4, r4, r0  */
L_5a78:
    mov r5, r4                              /* @5A78 2546 mov r5, r4  */
L_5a7a:
    ldr r0, [sp, #4]                        /* @5A7A 0198 ldr r0, [sp, #4]  */
L_5a7c:
    cmp r4, r0                              /* @5A7C 8442 cmp r4, r0  */
L_5a7e:
    ble L_5a84                              /* @5A7E 01dd ble #2 -> 0x5A84 */
L_5a80:
    str r4, [sp, #4]                        /* @5A80 0194 str r4, [sp, #4]  */
L_5a82:
    b L_5a8a                                /* @5A82 02e0 b #4 -> 0x5A8A */
L_5a84:
    cmp r4, r7                              /* @5A84 bc42 cmp r4, r7  */
L_5a86:
    bge L_5a8a                              /* @5A86 00da bge #0 -> 0x5A8A */
L_5a88:
    mov r7, r4                              /* @5A88 2746 mov r7, r4  */
L_5a8a:
    ldr r0, [sp]                            /* @5A8A 0098 ldr r0, [sp]  */
L_5a8c:
    adds r0, r0, r4                         /* @5A8C 0019 adds r0, r0, r4  */
L_5a8e:
    str r0, [sp]                            /* @5A8E 0090 str r0, [sp]  */
L_5a90:
    adds r6, r6, #1                         /* @5A90 761c adds r6, r6, #1  */
L_5a92:
    uxth r6, r6                             /* @5A92 b6b2 uxth r6, r6  */
L_5a94:
    ldr r0, [sp, #8]                        /* @5A94 0298 ldr r0, [sp, #8]  */
L_5a96:
    cmp r6, r0                              /* @5A96 8642 cmp r6, r0  */
L_5a98:
    blo L_5a4a                              /* @5A98 d7d3 blo #-82 -> 0x5A4A */
L_5a9a:
    ldr r0, [sp]                            /* @5A9A 0098 ldr r0, [sp]  */
L_5a9c:
    subs r1, r0, r7                         /* @5A9C c11b subs r1, r0, r7  */
L_5a9e:
    ldr r0, [sp, #4]                        /* @5A9E 0198 ldr r0, [sp, #4]  */
L_5aa0:
    subs r0, r1, r0                         /* @5AA0 081a subs r0, r1, r0  */
L_5aa2:
    ldr r1, [sp, #8]                        /* @5AA2 0299 ldr r1, [sp, #8]  */
L_5aa4:
    subs r1, r1, #2                         /* @5AA4 891e subs r1, r1, #2  */
L_5aa6:
    bl L_013c                               /* @5AA6 faf749fb bl #-22894 -> sdivmod32 */
L_5aaa:
    cmp r0, #0                              /* @5AAA 0028 cmp r0, #0  */
L_5aac:
    bge L_5ab8                              /* @5AAC 04da bge #8 -> 0x5AB8 */
L_5aae:
    lsls r1, r0, #1                         /* @5AAE 4100 lsls r1, r0, #1  */
L_5ab0:
    movs r0, #1                             /* @5AB0 0120 movs r0, #1  */
L_5ab2:
    lsls r0, r0, #14                        /* @5AB2 8003 lsls r0, r0, #14  */
L_5ab4:
    adds r0, r1, r0                         /* @5AB4 0818 adds r0, r1, r0  */
L_5ab6:
    b L_5ac2                                /* @5AB6 04e0 b #8 -> 0x5AC2 */
L_5ab8:
    ldr r1, [pc, #12]                       /* @5AB8 0349 ldr r1, [pc, #12] literal 0x5AC8 -> 0x00003FFF */
L_5aba:
    cmp r0, r1                              /* @5ABA 8842 cmp r0, r1  */
L_5abc:
    ble L_5ac2                              /* @5ABC 01dd ble #2 -> 0x5AC2 */
L_5abe:
    adds r1, r1, #1                         /* @5ABE 491c adds r1, r1, #1  */
L_5ac0:
    subs r0, r0, r1                         /* @5AC0 401a subs r0, r0, r1  */
L_5ac2:
    uxth r0, r0                             /* @5AC2 80b2 uxth r0, r0  */
L_5ac4:
    pop {r1, r2, r3, r4, r5, r6, r7, pc}    /* @5AC4 febd pop {r1, r2, r3, r4, r5, r6, r7, pc}  */
    .byte 0x00,0x00 /* @5AC6 unclassified */
L_5ac8:
    .byte 0xff,0x3f,0x00,0x00 /* @5AC8 literal */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @5ACC unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @5ADC unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @5AEC unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @5AFC unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @5B0C unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @5B1C unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @5B2C unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @5B3C unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @5B4C unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @5B5C unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @5B6C unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @5B7C unclassified */

/* sub_5B84 */
L_5b84:
    push {r2, r3, r4, lr}                   /* @5B84 1cb5 push {r2, r3, r4, lr}  */
L_5b86:
    movs r1, #7                             /* @5B86 0721 movs r1, #7  */
L_5b88:
    mov r0, sp                              /* @5B88 6846 mov r0, sp  */
L_5b8a:
    bl L_2d84                               /* @5B8A fdf7fbf8 bl #-11786 -> zero_bytes */
L_5b8e:
    movs r0, #0                             /* @5B8E 0020 movs r0, #0  */
L_5b90:
    mov r1, sp                              /* @5B90 6946 mov r1, sp  */
L_5b92:
    strb r0, [r1]                           /* @5B92 0870 strb r0, [r1]  */
L_5b94:
    strb r0, [r1, #5]                       /* @5B94 4871 strb r0, [r1, #5]  */
L_5b96:
    movs r0, #6                             /* @5B96 0620 movs r0, #6  */
L_5b98:
    strb r0, [r1, #3]                       /* @5B98 c870 strb r0, [r1, #3]  */
L_5b9a:
    movs r0, #1                             /* @5B9A 0120 movs r0, #1  */
L_5b9c:
    strb r0, [r1, #6]                       /* @5B9C 8871 strb r0, [r1, #6]  */
L_5b9e:
    strb r0, [r1, #4]                       /* @5B9E 0871 strb r0, [r1, #4]  */
L_5ba0:
    strb r0, [r1, #2]                       /* @5BA0 8870 strb r0, [r1, #2]  */
L_5ba2:
    strb r0, [r1, #1]                       /* @5BA2 4870 strb r0, [r1, #1]  */
L_5ba4:
    mov r0, sp                              /* @5BA4 6846 mov r0, sp  */
L_5ba6:
    bl L_2010                               /* @5BA6 fcf733fa bl #-15258 -> sub_2010 */
L_5baa:
    ldr r4, [pc, #36]                       /* @5BAA 094c ldr r4, [pc, #36] literal 0x5BD0 -> 0x0000FFB6 */
L_5bac:
    mov r0, r4                              /* @5BAC 2046 mov r0, r4  */
L_5bae:
    bl L_1fcc                               /* @5BAE fcf70dfa bl #-15334 -> sub_1FCC */
L_5bb2:
    mov r0, r4                              /* @5BB2 2046 mov r0, r4  */
L_5bb4:
    bl L_1fe4                               /* @5BB4 fcf716fa bl #-15316 -> sub_1FE4 */
L_5bb8:
    movs r0, #0                             /* @5BB8 0020 movs r0, #0  */
L_5bba:
    bl L_1fb8                               /* @5BBA fcf7fdf9 bl #-15366 -> sub_1FB8 */
L_5bbe:
    movs r2, #1                             /* @5BBE 0122 movs r2, #1  */
L_5bc0:
    movs r1, #2                             /* @5BC0 0221 movs r1, #2  */
L_5bc2:
    movs r0, #5                             /* @5BC2 0520 movs r0, #5  */
L_5bc4:
    bl L_0fec                               /* @5BC4 fbf712fa bl #-19420 -> sub_0FEC */
L_5bc8:
    bl L_1ffc                               /* @5BC8 fcf718fa bl #-15312 -> sub_1FFC */
L_5bcc:
    pop {r2, r3, r4, pc}                    /* @5BCC 1cbd pop {r2, r3, r4, pc}  */
    .byte 0x00,0x00 /* @5BCE unclassified */
L_5bd0:
    .byte 0xb6,0xff,0x00,0x00 /* @5BD0 literal */

/* sub_5BD4 */
L_5bd4:
    push {r0, r1, r2, r3, r4, lr}           /* @5BD4 1fb5 push {r0, r1, r2, r3, r4, lr}  */
L_5bd6:
    ldr r4, [pc, #108]                      /* @5BD6 1b4c ldr r4, [pc, #108] literal 0x5C44 -> 0x2000072C */
L_5bd8:
    ldr r0, [r4, #16]                       /* @5BD8 2069 ldr r0, [r4, #16]  */
L_5bda:
    cmp r0, #0                              /* @5BDA 0028 cmp r0, #0  */
L_5bdc:
    beq L_5c3c                              /* @5BDC 2ed0 beq #92 -> 0x5C3C */
L_5bde:
    movs r1, #16                            /* @5BDE 1021 movs r1, #16  */
L_5be0:
    mov r0, sp                              /* @5BE0 6846 mov r0, sp  */
L_5be2:
    bl L_2d84                               /* @5BE2 fdf7cff8 bl #-11874 -> zero_bytes */
L_5be6:
    movs r0, #64                            /* @5BE6 4020 movs r0, #64  */
L_5be8:
    mov r1, sp                              /* @5BE8 6946 mov r1, sp  */
L_5bea:
    strb r0, [r1]                           /* @5BEA 0870 strb r0, [r1]  */
L_5bec:
    movs r0, #0                             /* @5BEC 0020 movs r0, #0  */
L_5bee:
    strh r0, [r1, #2]                       /* @5BEE 4880 strh r0, [r1, #2]  */
L_5bf0:
    strb r0, [r1, #1]                       /* @5BF0 4870 strb r0, [r1, #1]  */
L_5bf2:
    movs r0, #1                             /* @5BF2 0120 movs r0, #1  */
L_5bf4:
    lsls r0, r0, #9                         /* @5BF4 4002 lsls r0, r0, #9  */
L_5bf6:
    strh r0, [r1, #4]                       /* @5BF6 8880 strh r0, [r1, #4]  */
L_5bf8:
    bl L_1924                               /* @5BF8 fbf794fe bl #-17112 -> sub_1924 */
L_5bfc:
    str r0, [sp, #8]                        /* @5BFC 0290 str r0, [sp, #8]  */
L_5bfe:
    ldr r0, [r4, #16]                       /* @5BFE 2069 ldr r0, [r4, #16]  */
L_5c00:
    str r0, [sp, #12]                       /* @5C00 0390 str r0, [sp, #12]  */
L_5c02:
    movs r0, #1                             /* @5C02 0120 movs r0, #1  */
L_5c04:
    mov r1, sp                              /* @5C04 6946 mov r1, sp  */
L_5c06:
    lsls r0, r0, #30                        /* @5C06 8007 lsls r0, r0, #30  */
L_5c08:
    bl L_21f0                               /* @5C08 fcf7f2fa bl #-14876 -> sub_21F0 */
L_5c0c:
    ldr r0, [pc, #56]                       /* @5C0C 0e48 ldr r0, [pc, #56] literal 0x5C48 -> 0x40020C00 */
L_5c0e:
    ldr r1, [r0, #52]                       /* @5C0E 416b ldr r1, [r0, #52]  */
L_5c10:
    lsrs r1, r1, #3                         /* @5C10 c908 lsrs r1, r1, #3  */
L_5c12:
    lsls r1, r1, #3                         /* @5C12 c900 lsls r1, r1, #3  */
L_5c14:
    adds r1, r1, #2                         /* @5C14 891c adds r1, r1, #2  */
L_5c16:
    str r1, [r0, #52]                       /* @5C16 4163 str r1, [r0, #52]  */
L_5c18:
    ldr r1, [r0, #56]                       /* @5C18 816b ldr r1, [r0, #56]  */
L_5c1a:
    lsrs r1, r1, #3                         /* @5C1A c908 lsrs r1, r1, #3  */
L_5c1c:
    lsls r1, r1, #3                         /* @5C1C c900 lsls r1, r1, #3  */
L_5c1e:
    str r1, [r0, #56]                       /* @5C1E 8163 str r1, [r0, #56]  */
L_5c20:
    ldr r0, [pc, #40]                       /* @5C20 0a48 ldr r0, [pc, #40] literal 0x5C4C -> 0x40020D00 */
L_5c22:
    ldr r1, [r0]                            /* @5C22 0168 ldr r1, [r0]  */
L_5c24:
    movs r2, #1                             /* @5C24 0122 movs r2, #1  */
L_5c26:
    lsls r2, r2, #14                        /* @5C26 9203 lsls r2, r2, #14  */
L_5c28:
    orrs r1, r2                             /* @5C28 1143 orrs r1, r2  */
L_5c2a:
    str r1, [r0]                            /* @5C2A 0160 str r1, [r0]  */
L_5c2c:
    bl L_0d3c                               /* @5C2C fbf786f8 bl #-20212 -> sub_0D3C */
L_5c30:
    movs r2, #1                             /* @5C30 0122 movs r2, #1  */
L_5c32:
    mov r1, r2                              /* @5C32 1146 mov r1, r2  */
L_5c34:
    movs r0, #6                             /* @5C34 0620 movs r0, #6  */
L_5c36:
    bl L_0fec                               /* @5C36 fbf7d9f9 bl #-19534 -> sub_0FEC */
L_5c3a:
    pop {r0, r1, r2, r3, r4, pc}            /* @5C3A 1fbd pop {r0, r1, r2, r3, r4, pc}  */
L_5c3c:
    bl L_5c50                               /* @5C3C 00f008f8 bl #16 -> sub_5C50 */
L_5c40:
    pop {r0, r1, r2, r3, r4, pc}            /* @5C40 1fbd pop {r0, r1, r2, r3, r4, pc}  */
    .byte 0x00,0x00 /* @5C42 unclassified */
L_5c44:
    .byte 0x2c,0x07,0x00,0x20 /* @5C44 literal */
L_5c48:
    .byte 0x00,0x0c,0x02,0x40 /* @5C48 literal */
L_5c4c:
    .byte 0x00,0x0d,0x02,0x40 /* @5C4C literal */

/* sub_5C50 */
L_5c50:
    push {lr}                               /* @5C50 00b5 push {lr}  */
L_5c52:
    movs r0, #1                             /* @5C52 0120 movs r0, #1  */
L_5c54:
    lsls r0, r0, #30                        /* @5C54 8007 lsls r0, r0, #30  */
L_5c56:
    ldr r2, [r0, #4]                        /* @5C56 4268 ldr r2, [r0, #4]  */
L_5c58:
    movs r1, #2                             /* @5C58 0221 movs r1, #2  */
L_5c5a:
    bics r2, r1                             /* @5C5A 8a43 bics r2, r1  */
L_5c5c:
    str r2, [r0, #4]                        /* @5C5C 4260 str r2, [r0, #4]  */
L_5c5e:
    ldr r2, [r0, #4]                        /* @5C5E 4268 ldr r2, [r0, #4]  */
L_5c60:
    lsrs r2, r2, #1                         /* @5C60 5208 lsrs r2, r2, #1  */
L_5c62:
    lsls r2, r2, #1                         /* @5C62 5200 lsls r2, r2, #1  */
L_5c64:
    str r2, [r0, #4]                        /* @5C64 4260 str r2, [r0, #4]  */
L_5c66:
    ldr r2, [r0, #20]                       /* @5C66 4269 ldr r2, [r0, #20]  */
L_5c68:
    bics r2, r1                             /* @5C68 8a43 bics r2, r1  */
L_5c6a:
    str r2, [r0, #20]                       /* @5C6A 4261 str r2, [r0, #20]  */
L_5c6c:
    ldr r1, [r0, #20]                       /* @5C6C 4169 ldr r1, [r0, #20]  */
L_5c6e:
    lsrs r1, r1, #1                         /* @5C6E 4908 lsrs r1, r1, #1  */
L_5c70:
    lsls r1, r1, #1                         /* @5C70 4900 lsls r1, r1, #1  */
L_5c72:
    str r1, [r0, #20]                       /* @5C72 4161 str r1, [r0, #20]  */
L_5c74:
    bl L_5ce8                               /* @5C74 00f038f8 bl #112 -> sub_5CE8 */
L_5c78:
    pop {pc}                                /* @5C78 00bd pop {pc}  */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @5C7A unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 /* @5C8A unclassified */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00 /* @5C9A unclassified */

/* sub_5CA0 */
L_5ca0:
    push {r2, r3, r4, lr}                   /* @5CA0 1cb5 push {r2, r3, r4, lr}  */
L_5ca2:
    ldr r0, [pc, #60]                       /* @5CA2 0f48 ldr r0, [pc, #60] literal 0x5CE0 -> 0x40002000 */
L_5ca4:
    ldr r1, [pc, #52]                       /* @5CA4 0d49 ldr r1, [pc, #52] literal 0x5CDC -> 0x00005A5A */
L_5ca6:
    str r1, [r0, #8]                        /* @5CA6 8160 str r1, [r0, #8]  */
L_5ca8:
    ldr r1, [pc, #56]                       /* @5CA8 0e49 ldr r1, [pc, #56] literal 0x5CE4 -> 0x0000A5A5 */
L_5caa:
    str r1, [r0, #8]                        /* @5CAA 8160 str r1, [r0, #8]  */
L_5cac:
    ldr r1, [r0, #4]                        /* @5CAC 4168 ldr r1, [r0, #4]  */
L_5cae:
    asrs r2, r0, #22                        /* @5CAE 8215 asrs r2, r0, #22  */
L_5cb0:
    orrs r1, r2                             /* @5CB0 1143 orrs r1, r2  */
L_5cb2:
    str r1, [r0, #4]                        /* @5CB2 4160 str r1, [r0, #4]  */
L_5cb4:
    movs r0, #1                             /* @5CB4 0120 movs r0, #1  */
L_5cb6:
    mov r1, sp                              /* @5CB6 6946 mov r1, sp  */
L_5cb8:
    strb r0, [r1]                           /* @5CB8 0870 strb r0, [r1]  */
L_5cba:
    strb r0, [r1, #1]                       /* @5CBA 4870 strb r0, [r1, #1]  */
L_5cbc:
    movs r0, #0                             /* @5CBC 0020 movs r0, #0  */
L_5cbe:
    strb r0, [r1, #2]                       /* @5CBE 8870 strb r0, [r1, #2]  */
L_5cc0:
    strb r0, [r1, #3]                       /* @5CC0 c870 strb r0, [r1, #3]  */
L_5cc2:
    strb r0, [r1, #4]                       /* @5CC2 0871 strb r0, [r1, #4]  */
L_5cc4:
    strb r0, [r1, #5]                       /* @5CC4 4871 strb r0, [r1, #5]  */
L_5cc6:
    mov r2, sp                              /* @5CC6 6a46 mov r2, sp  */
L_5cc8:
    movs r1, #13                            /* @5CC8 0d21 movs r1, #13  */
L_5cca:
    bl L_137c                               /* @5CCA fbf757fb bl #-18770 -> sub_137C */
L_5cce:
    mov r2, sp                              /* @5CCE 6a46 mov r2, sp  */
L_5cd0:
    movs r1, #14                            /* @5CD0 0e21 movs r1, #14  */
L_5cd2:
    movs r0, #0                             /* @5CD2 0020 movs r0, #0  */
L_5cd4:
    bl L_137c                               /* @5CD4 fbf752fb bl #-18780 -> sub_137C */
L_5cd8:
    pop {r2, r3, r4, pc}                    /* @5CD8 1cbd pop {r2, r3, r4, pc}  */
    .byte 0x00,0x00 /* @5CDA unclassified */
L_5cdc:
    .byte 0x5a,0x5a,0x00,0x00 /* @5CDC literal */
L_5ce0:
    .byte 0x00,0x20,0x00,0x40 /* @5CE0 literal */
L_5ce4:
    .byte 0xa5,0xa5,0x00,0x00 /* @5CE4 literal */

/* sub_5CE8 */
L_5ce8:
    ldr r2, [pc, #20]                       /* @5CE8 054a ldr r2, [pc, #20] literal 0x5D00 -> 0x200006A8 */
L_5cea:
    movs r1, #0                             /* @5CEA 0021 movs r1, #0  */
L_5cec:
    strb r1, [r2]                           /* @5CEC 1170 strb r1, [r2]  */
L_5cee:
    strb r1, [r2, #1]                       /* @5CEE 5170 strb r1, [r2, #1]  */
L_5cf0:
    movs r0, #0                             /* @5CF0 0020 movs r0, #0  */
L_5cf2:
    adds r3, r2, r0                         /* @5CF2 1318 adds r3, r2, r0  */
L_5cf4:
    strb r1, [r3, #3]                       /* @5CF4 d970 strb r1, [r3, #3]  */
L_5cf6:
    adds r0, r0, #1                         /* @5CF6 401c adds r0, r0, #1  */
L_5cf8:
    uxth r0, r0                             /* @5CF8 80b2 uxth r0, r0  */
L_5cfa:
    cmp r0, #9                              /* @5CFA 0928 cmp r0, #9  */
L_5cfc:
    blo L_5cf2                              /* @5CFC f9d3 blo #-14 -> 0x5CF2 */
L_5cfe:
    bx lr                                   /* @5CFE 7047 bx lr  */
L_5d00:
    .byte 0xa8,0x06,0x00,0x20 /* @5D00 literal */
    .byte 0x00,0x00,0x00,0x00 /* @5D04 unclassified */

/* sub_5D08 */
L_5d08:
    push {r0, r1, r2, r3, r4, r5, r6, r7, lr} /* @5D08 ffb5 push {r0, r1, r2, r3, r4, r5, r6, r7, lr}  */
L_5d0a:
    sub sp, #4                              /* @5D0A 81b0 sub sp, #4  */
L_5d0c:
    ldr r4, [sp, #40]                       /* @5D0C 0a9c ldr r4, [sp, #40]  */
L_5d0e:
    mov r6, r2                              /* @5D0E 1646 mov r6, r2  */
L_5d10:
    movs r1, #0                             /* @5D10 0021 movs r1, #0  */
L_5d12:
    str r1, [sp]                            /* @5D12 0091 str r1, [sp]  */
L_5d14:
    movs r5, #0                             /* @5D14 0025 movs r5, #0  */
L_5d16:
    mov r7, sp                              /* @5D16 6f46 mov r7, sp  */
L_5d18:
    b L_5d28                                /* @5D18 06e0 b #12 -> 0x5D28 */
L_5d1a:
    movs r1, #10                            /* @5D1A 0a21 movs r1, #10  */
L_5d1c:
    bl L_013c                               /* @5D1C faf70efa bl #-23524 -> sdivmod32 */
L_5d20:
    adds r1, #48                            /* @5D20 3031 adds r1, #48  */
L_5d22:
    strb r1, [r7, r5]                       /* @5D22 7955 strb r1, [r7, r5]  */
L_5d24:
    adds r5, r5, #1                         /* @5D24 6d1c adds r5, r5, #1  */
L_5d26:
    uxtb r5, r5                             /* @5D26 edb2 uxtb r5, r5  */
L_5d28:
    cmp r5, #0                              /* @5D28 002d cmp r5, #0  */
L_5d2a:
    beq L_5d1a                              /* @5D2A f6d0 beq #-20 -> 0x5D1A */
L_5d2c:
    cmp r0, #0                              /* @5D2C 0028 cmp r0, #0  */
L_5d2e:
    bgt L_5d1a                              /* @5D2E f4dc bgt #-24 -> 0x5D1A */
L_5d30:
    ldrb r0, [r4]                           /* @5D30 2078 ldrb r0, [r4]  */
L_5d32:
    cmp r0, #45                             /* @5D32 2d28 cmp r0, #45  */
L_5d34:
    beq L_5d3a                              /* @5D34 01d0 beq #2 -> 0x5D3A */
L_5d36:
    movs r0, #0                             /* @5D36 0020 movs r0, #0  */
L_5d38:
    b L_5d4c                                /* @5D38 08e0 b #16 -> 0x5D4C */
L_5d3a:
    movs r0, #1                             /* @5D3A 0120 movs r0, #1  */
L_5d3c:
    b L_5d4c                                /* @5D3C 06e0 b #12 -> 0x5D4C */
L_5d3e:
    subs r5, r5, #1                         /* @5D3E 6d1e subs r5, r5, #1  */
L_5d40:
    mov r1, r0                              /* @5D40 0146 mov r1, r0  */
L_5d42:
    uxtb r5, r5                             /* @5D42 edb2 uxtb r5, r5  */
L_5d44:
    ldrb r2, [r7, r5]                       /* @5D44 7a5d ldrb r2, [r7, r5]  */
L_5d46:
    adds r0, r0, #1                         /* @5D46 401c adds r0, r0, #1  */
L_5d48:
    uxtb r0, r0                             /* @5D48 c0b2 uxtb r0, r0  */
L_5d4a:
    strb r2, [r4, r1]                       /* @5D4A 6254 strb r2, [r4, r1]  */
L_5d4c:
    cmp r5, #0                              /* @5D4C 002d cmp r5, #0  */
L_5d4e:
    bne L_5d3e                              /* @5D4E f6d1 bne #-20 -> 0x5D3E */
L_5d50:
    cmp r6, #4                              /* @5D50 042e cmp r6, #4  */
L_5d52:
    beq L_5d82                              /* @5D52 16d0 beq #44 -> 0x5D82 */
L_5d54:
    movs r2, #46                            /* @5D54 2e22 movs r2, #46  */
L_5d56:
    adds r1, r0, #1                         /* @5D56 411c adds r1, r0, #1  */
L_5d58:
    uxtb r5, r1                             /* @5D58 cdb2 uxtb r5, r1  */
L_5d5a:
    strb r2, [r4, r0]                       /* @5D5A 2254 strb r2, [r4, r0]  */
L_5d5c:
    cmp r6, #3                              /* @5D5C 032e cmp r6, #3  */
L_5d5e:
    bne L_5d72                              /* @5D5E 08d1 bne #16 -> 0x5D72 */
L_5d60:
    movs r1, #10                            /* @5D60 0a21 movs r1, #10  */
L_5d62:
    ldr r0, [sp, #8]                        /* @5D62 0298 ldr r0, [sp, #8]  */
L_5d64:
    bl L_013c                               /* @5D64 faf7eaf9 bl #-23596 -> sdivmod32 */
L_5d68:
    adds r0, #48                            /* @5D68 3030 adds r0, #48  */
L_5d6a:
    mov r1, r5                              /* @5D6A 2946 mov r1, r5  */
L_5d6c:
    adds r5, r5, #1                         /* @5D6C 6d1c adds r5, r5, #1  */
L_5d6e:
    uxtb r5, r5                             /* @5D6E edb2 uxtb r5, r5  */
L_5d70:
    strb r0, [r4, r1]                       /* @5D70 6054 strb r0, [r4, r1]  */
L_5d72:
    movs r1, #10                            /* @5D72 0a21 movs r1, #10  */
L_5d74:
    ldr r0, [sp, #8]                        /* @5D74 0298 ldr r0, [sp, #8]  */
L_5d76:
    bl L_013c                               /* @5D76 faf7e1f9 bl #-23614 -> sdivmod32 */
L_5d7a:
    adds r1, #48                            /* @5D7A 3031 adds r1, #48  */
L_5d7c:
    adds r0, r5, #1                         /* @5D7C 681c adds r0, r5, #1  */
L_5d7e:
    uxtb r0, r0                             /* @5D7E c0b2 uxtb r0, r0  */
L_5d80:
    strb r1, [r4, r5]                       /* @5D80 6155 strb r1, [r4, r5]  */
L_5d82:
    cmp r6, #1                              /* @5D82 012e cmp r6, #1  */
L_5d84:
    bne L_5d90                              /* @5D84 04d1 bne #8 -> 0x5D90 */
L_5d86:
    movs r2, #75                            /* @5D86 4b22 movs r2, #75  */
L_5d88:
    mov r1, r0                              /* @5D88 0146 mov r1, r0  */
L_5d8a:
    adds r0, r0, #1                         /* @5D8A 401c adds r0, r0, #1  */
L_5d8c:
    uxtb r0, r0                             /* @5D8C c0b2 uxtb r0, r0  */
L_5d8e:
    strb r2, [r4, r1]                       /* @5D8E 6254 strb r2, [r4, r1]  */
L_5d90:
    ldr r1, [sp, #16]                       /* @5D90 0499 ldr r1, [sp, #16]  */
L_5d92:
    cmp r1, #0                              /* @5D92 0029 cmp r1, #0  */
L_5d94:
    beq L_5dc2                              /* @5D94 15d0 beq #42 -> 0x5DC2 */
L_5d96:
    cmp r1, #1                              /* @5D96 0129 cmp r1, #1  */
L_5d98:
    beq L_5dc8                              /* @5D98 16d0 beq #44 -> 0x5DC8 */
L_5d9a:
    cmp r1, #2                              /* @5D9A 0229 cmp r1, #2  */
L_5d9c:
    bne L_5dbe                              /* @5D9C 0fd1 bne #30 -> 0x5DBE */
L_5d9e:
    movs r2, #99                            /* @5D9E 6322 movs r2, #99  */
L_5da0:
    adds r1, r0, #1                         /* @5DA0 411c adds r1, r0, #1  */
L_5da2:
    uxtb r1, r1                             /* @5DA2 c9b2 uxtb r1, r1  */
L_5da4:
    strb r2, [r4, r0]                       /* @5DA4 2254 strb r2, [r4, r0]  */
L_5da6:
    ldrb r0, [r4]                           /* @5DA6 2078 ldrb r0, [r4]  */
L_5da8:
    cmp r0, #45                             /* @5DA8 2d28 cmp r0, #45  */
L_5daa:
    beq L_5db0                              /* @5DAA 01d0 beq #2 -> 0x5DB0 */
L_5dac:
    cmp r1, #7                              /* @5DAC 0729 cmp r1, #7  */
L_5dae:
    bhs L_5dba                              /* @5DAE 04d2 bhs #8 -> 0x5DBA */
L_5db0:
    movs r2, #108                           /* @5DB0 6c22 movs r2, #108  */
L_5db2:
    mov r0, r1                              /* @5DB2 0846 mov r0, r1  */
L_5db4:
    adds r1, r1, #1                         /* @5DB4 491c adds r1, r1, #1  */
L_5db6:
    uxtb r1, r1                             /* @5DB6 c9b2 uxtb r1, r1  */
L_5db8:
    strb r2, [r4, r0]                       /* @5DB8 2254 strb r2, [r4, r0]  */
L_5dba:
    movs r0, #107                           /* @5DBA 6b20 movs r0, #107  */
L_5dbc:
    strb r0, [r4, r1]                       /* @5DBC 6054 strb r0, [r4, r1]  */
L_5dbe:
    add sp, #20                             /* @5DBE 05b0 add sp, #20  */
L_5dc0:
    pop {r4, r5, r6, r7, pc}                /* @5DC0 f0bd pop {r4, r5, r6, r7, pc}  */
L_5dc2:
    movs r1, #63                            /* @5DC2 3f21 movs r1, #63  */
L_5dc4:
    strb r1, [r4, r0]                       /* @5DC4 2154 strb r1, [r4, r0]  */
L_5dc6:
    b L_5dbe                                /* @5DC6 fae7 b #-12 -> 0x5DBE */
L_5dc8:
    movs r2, #101                           /* @5DC8 6522 movs r2, #101  */
L_5dca:
    adds r1, r0, #1                         /* @5DCA 411c adds r1, r0, #1  */
L_5dcc:
    uxtb r1, r1                             /* @5DCC c9b2 uxtb r1, r1  */
L_5dce:
    strb r2, [r4, r0]                       /* @5DCE 2254 strb r2, [r4, r0]  */
L_5dd0:
    movs r2, #114                           /* @5DD0 7222 movs r2, #114  */
L_5dd2:
    adds r0, r1, #1                         /* @5DD2 481c adds r0, r1, #1  */
L_5dd4:
    uxtb r0, r0                             /* @5DD4 c0b2 uxtb r0, r0  */
L_5dd6:
    strb r2, [r4, r1]                       /* @5DD6 6254 strb r2, [r4, r1]  */
L_5dd8:
    strb r2, [r4, r0]                       /* @5DD8 2254 strb r2, [r4, r0]  */
L_5dda:
    b L_5dbe                                /* @5DDA f0e7 b #-32 -> 0x5DBE */
    .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3e,0x31,0x30,0x30 /* @5DDC unclassified */
    .byte 0x30,0x4b,0x3f,0x00,0x00,0x3c,0x2d,0x31,0x30,0x30,0x30,0x4b,0x3f,0x00,0x3e,0x31 /* @5DEC unclassified */
    .byte 0x30,0x30,0x30,0x4b,0x65,0x72,0x72,0x3e,0x39,0x39,0x4b,0x63,0x6c,0x6b,0x00,0x00 /* @5DFC unclassified */
    .byte 0x3c,0x2d,0x39,0x39,0x4b,0x63,0x6c,0x6b,0x00,0x00 /* @5E0C unclassified */
