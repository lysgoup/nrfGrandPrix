.section .text
.global map

map:
    // int map(int x, int in_min, int in_max, int out_min, int out_max)
    // Arguments in registers: x (r0), in_min (r1), in_max (r2), out_min (r3)

    push {r4-r7}
    mov r4, lr
    push {r4}
    ldr r4, [sp, #20]
    sub r5, r0, r1
    sub r6, r4, r3
    sub r7, r2, r1
    mul r5, r5, r6
    udiv r5, r5, r7
    add r0, r5, r3
    pop {r4}
    mov lr, r4
    pop {r4-r7}
    bx lr
