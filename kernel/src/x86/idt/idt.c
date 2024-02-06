#include <stdint.h>
#include "x86/idt/idt.h"
#include "x86/gdt/gdt.h"

static struct idt_descriptor {
    uint16_t offset0;
    uint16_t seg_selector;
    uint8_t ist;
    uint8_t attr;
    uint16_t offset1;
    uint32_t offset2;
    uint32_t reserved;
};

static const uint8_t IDT_INTERRUPT_GATE = 0x8e;

static struct idtr {
    uint16_t size;
    uint64_t offset;
} __attribute__((packed));

static struct idtr idtr;
__attribute__((aligned(0x10)))
static struct idt_descriptor idt[256];

extern uint64_t *isr_stubs[];

static void idt_set_entry(uint8_t vector, uint64_t offset, uint8_t attr) {
    struct idt_descriptor *entry = &idt[vector];

    entry->offset0 = offset & 0xffff;
    entry->seg_selector = GDT_KERNEL_CODE;
    entry->ist = 0;
    entry->attr = attr;
    entry->offset1 = (offset >> 16) & 0xffff;
    entry->offset2 = (offset >> 32) & 0xffffffff;
    entry->reserved = 0;
}

void idt_init() {
    for (uint16_t i = 0; i < 256; i++) {
        idt_set_entry(i, (uint64_t) isr_stubs[i], IDT_INTERRUPT_GATE);
    }

    idtr.size = sizeof(idt) - 1;
    idtr.offset = (uint64_t) &idt[0];

    asm volatile("lidt %0; sti" : : "m" (idtr));
}
