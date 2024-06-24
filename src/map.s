.section .text
.global map

map:
    // Function signature: int map(int x, int in_min, int in_max, int out_min, int out_max)
    // Arguments in registers: x (r0), in_min (r1), in_max (r2), out_min (r3)
    // Argument in stack: out_max

    // Save r4 and lr to the stack
    push {r4}

    // Save lr to the stack manually
    mov r4, lr
    push {r4}

    // Load out_max from the stack (8 bytes offset from the top of the stack)
    ldr r4, [sp, #8]

    // Calculate (x - in_min) and save to r5
    sub r5, r0, r1

    // Calculate (out_max - out_min) and save to r6
    sub r6, r4, r3

    // Calculate (in_max - in_min) and save to r7
    sub r7, r2, r1

    // Multiply r5 by r6
    mul r5, r5, r6

    // Divide r5 by r7 (assuming r7 is not zero, need to handle this case in real code)
    udiv r5, r5, r7

    // Add out_min to the result
    add r0, r5, r3

    // Restore lr from the stack manually
    pop {r4}
    mov lr, r4

    // Restore r4 from the stack
    pop {r4}

    // Return the result (in r0)
    bx lr
