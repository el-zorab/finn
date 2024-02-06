.altmacro

.section .text

.macro isr_err_stub vector
isr_stub_\vector:
    push $\vector
    jmp isr_wrapper
.endm

.macro isr_no_err_stub vector
isr_stub_\vector:
    push $0
    push $\vector
    jmp isr_wrapper
.endm

isr_wrapper:
    push %rax
    push %rbx
    push %rcx
    push %rdx
    push %rdi
    push %rsi
    push %rbp

    push %r8
    push %r9
    push %r10
    push %r11
    push %r12
    push %r13
    push %r14
    push %r15

    mov %cr0, %rax
    push %rax
    mov %cr2, %rax
    push %rax
    mov %cr3, %rax
    push %rax
    mov %cr4, %rax
    push %rax

    mov %ds, %ax
    push %rax

    cld

    mov %rsp, %rdi
    call isr_handler

    pop %rax
    mov %ax, %ds

    pop %rax
    mov %rax, %cr4
    pop %rax
    mov %rax, %cr3
    pop %rax
    mov %rax, %cr2
    pop %rax
    mov %rax, %cr0

    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %r11
    pop %r10
    pop %r9
    pop %r8

    pop %rbp
    pop %rsi
    pop %rdi
    pop %rdx
    pop %rcx
    pop %rbx
    pop %rax

    add $0x10, %rsp

    iretq

isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_err_stub    21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_err_stub    29
isr_err_stub    30
isr_no_err_stub 31

.set i, 32
.rept 224
    isr_no_err_stub %i
    .set i, i+1
.endr

.macro isr_get_addr vector
    .quad isr_stub_\vector
.endm

.section .data

.global isr_stubs
.align 8
isr_stubs:
.set i, 0
.rept 256
    isr_get_addr %i
    .set i, i+1
.endr
