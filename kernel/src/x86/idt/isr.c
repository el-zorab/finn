#include <stdint.h>
#include "kprint/kprint.h"

static char *exception_names[] = {
    "Division Error", "Debug", "Non-maskable interrupt", "Breakpoint",
    "Overflow", "Bound Range Exceeded", "Invalid Opcode", "Device Not Available",
    "Double Fault", "Coprocessor Segment Overrun", "Invalid TSS", "Segment Not Present",
    "Stack-Segment Fault", "General Protection Fault", "Page Fault", "Reserved",
    "x87 Floating-Point Exception", "Alignment Check", "Machine Check", "SIMD Floating-Point Exception",
    "Virtualization Exception", "Control Protection Exception", "Reserved", "Reserved",
    "Reserved", "Reserved", "Reserved", "Reserved",
    "Hypervisor Injection Exception", "VMM Communication Exception", "Security Exception", "Reserved"
};

static struct isr_stack_frame {
    uint64_t ds;
    uint64_t cr4, cr3, cr2, cr0;
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rbp, rsi, rdi, rdx, rcx, rbx, rax;
    uint64_t vector, error_code;
    uint64_t rip, cs, rflags, rsp, ss;
};

static void print_stack_frame(struct isr_stack_frame *frame) {
    kprintf("Stack frame:\n");
    kprintf("rax=0x%016llx  rbx=0x%016llx     rcx=0x%016llx  rdx=0x%016llx\n", frame->rax, frame->rbx, frame->rcx, frame->rdx);
    kprintf("rdi=0x%016llx  rsi=0x%016llx     rbp=0x%016llx  rsp=0x%016llx\n", frame->rdi, frame->rsi, frame->rbp, frame->rsp);
    kprintf("r8 =0x%016llx  r9 =0x%016llx     r10=0x%016llx  r11=0x%016llx\n", frame->r8, frame->r9, frame->r10, frame->r11);
    kprintf("r12=0x%016llx  r13=0x%016llx     r14=0x%016llx  r15=0x%016llx\n", frame->r12, frame->r13, frame->r14, frame->r15);
    kprintf("cr0=0x%016llx  cr2=0x%016llx     cr3=0x%016llx  cr4=0x%016llx\n", frame->cr0, frame->cr2, frame->cr3, frame->cr4);    
    kprintf("rip=0x%016llx  cs=0x%04x    ds=0x%04x     ss=0x%04x               rflags=0x%016llx\n", frame->rip, frame->cs, frame->ds, frame->ss, frame->rflags);
}

__attribute__((noreturn)) void isr_handler(struct isr_stack_frame *frame) {
    // TODO panic

    if (frame->vector < 32) {
        kprintf("\nEXCEPTION %s (0x%02x) ERROR CODE 0x%016llx\n\n", exception_names[frame->vector], frame->vector, frame->error_code);
    } else {
        kprintf("\nUNHANDLED INTERRUPT VECTOR 0x%02x\n\n", frame->vector);
    }

    print_stack_frame(frame);

    asm volatile("cli; hlt");
    while (1);
}
