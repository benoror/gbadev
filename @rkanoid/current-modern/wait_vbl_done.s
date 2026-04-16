.section .text.wait_vbl_done, "ax", %progbits
.global wait_vbl_done
.type wait_vbl_done, %function

.arm
wait_vbl_done:
    mov r0, #0x04000000
    add r0, r0, #6
scanline_wait:
    ldrh r1, [r0]
    cmp r1, #160
    bne scanline_wait
    bx lr
