.section .text._start, "ax", %progbits
.global _start
.type _start, %function

.equ Cpu_Mode_USR, 0x10
.equ Cpu_Mode_IRQ, 0x12
.equ Cpu_Mode_SVC, 0x13

.equ Cpu_Stack_USR, 0x03007F00
.equ Cpu_Stack_IRQ, 0x03007FA0
.equ Cpu_Stack_SVC, 0x03007FE0

.arm
_start:
    b start
    .space 0xBC

start:
    mov r0, #(Cpu_Mode_IRQ | 0x80 | 0x40)
    msr CPSR_c, r0
    ldr r13, =Cpu_Stack_IRQ

    mov r0, #(Cpu_Mode_SVC | 0x80 | 0x40)
    msr CPSR_c, r0
    ldr r13, =Cpu_Stack_SVC

    ldr r0, =__data_lma
    ldr r1, =__data_start__
    ldr r3, =__bss_start__
    cmp r0, r1
    beq 1f
0:
    cmp r1, r3
    ldrcc r2, [r0], #4
    strcc r2, [r1], #4
    bcc 0b
1:
    ldr r1, =__bss_end__
    mov r2, #0
2:
    cmp r3, r1
    strcc r2, [r3], #4
    bcc 2b

    mov r0, #(Cpu_Mode_SVC | 0x40)
    msr CPSR_c, r0

    mov r0, #(Cpu_Mode_USR | 0x80 | 0x40)
    msr CPSR_c, r0
    ldr sp, =Cpu_Stack_USR

    b C_Entry

final_return_loop:
    b final_return_loop
