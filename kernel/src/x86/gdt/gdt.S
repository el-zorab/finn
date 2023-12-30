.section .text

.global gdt_reload
gdt_reload:
    lgdt (%rdi)

    push %rsi
    lea reload_seg_regs(%rip), %rax
    push %rax
    lretq

reload_seg_regs:
    mov %dx, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    ret
