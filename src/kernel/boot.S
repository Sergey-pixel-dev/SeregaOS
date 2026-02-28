.section ".text.boot"

.global _start

_start:
    // Read core ID from MPIDR_EL1, park all cores except core 0
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbz     x1, core0
    b       halt

core0:
    // Set up stack below kernel load address (0x80000)
    ldr     x1, =_start
    mov     sp, x1

    // Zero out BSS
    ldr     x1, =__bss_start
    ldr     x2, =__bss_end
    b       2f
1:
    str     xzr, [x1], #8
2:
    cmp     x1, x2
    b.lo    1b

    // Jump to C entry point
    bl      kernel_main

halt:
    wfe
    b       halt
