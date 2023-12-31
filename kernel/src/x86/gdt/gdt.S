.section .text

.global gdt_reload
gdt_reload:
    lgdt (%rdi)

    push %rsi
    lea reload_seg_regs(%rip), %rax
    push %rax
    lretq

reload_seg_regs:
    mov %dx, %ds
    mov %dx, %es
    mov %dx, %fs
    mov %dx, %gs
    mov %dx, %ss
    ret
