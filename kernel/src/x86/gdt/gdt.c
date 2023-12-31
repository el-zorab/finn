#include <stdint.h>
#include "x86/gdt/gdt.h"

static enum GDT_ACCESS_BYTE {
    GDT_ACCESSED = 0x1,
    GDT_RW = 0x2,
    GDT_EXECUTABLE = 0x8,
    GDT_CODE_OR_DATA_SEGMENT = 0x10,
    GDT_USER_DPL = 0x60,
    GDT_PRESENT = 0x80,
    GDT_GENERAL_ACCESS_BYTE = GDT_ACCESSED | GDT_RW | GDT_CODE_OR_DATA_SEGMENT | GDT_PRESENT
};

static enum GDT_FLAGS {
    GDT_FLAG_LONG_MODE = 0x20,
    GDT_FLAG_PAGE_GRANULARITY = 0x80,
    GDT_GENERAL_FLAGS = GDT_FLAG_LONG_MODE | GDT_FLAG_PAGE_GRANULARITY
};

static struct gdt_descriptor {
    uint16_t limit0;
    uint16_t base0;
    uint8_t base1;
    uint8_t access;
    uint8_t limit1_flags;
    uint8_t base2;
};

static struct gdtr {
    uint16_t size;
    uint64_t offset;
} __attribute__((packed));

static struct gdt {
    struct gdt_descriptor gdt_descriptors[5];
};

static struct gdtr gdtr;
static struct gdt gdt;

static void gdt_add_null_entry() {
    struct gdt_descriptor *entry = &gdt.gdt_descriptors[0];

    entry->limit0 = 0;
    entry->base0 = 0;
    entry->base1 = 0;
    entry->access = 0;
    entry->limit1_flags = 0;
    entry->base2 = 0;
}

static void gdt_add_entry(uint8_t access) {
    static uint8_t entry_counter = 1;
    struct gdt_descriptor *entry = &gdt.gdt_descriptors[entry_counter++];
    
    entry->limit0 = 0xffff;
    entry->base0 = 0;
    entry->base1 = 0;
    entry->access = access;
    entry->limit1_flags = GDT_GENERAL_FLAGS | 0xf;
    entry->base2 = 0;
}

extern void gdt_reload(uint64_t gdtr_addr, uint16_t code_offset, uint16_t data_offset);

void gdt_init() {
    gdt_add_null_entry();

    // kernel code
    gdt_add_entry(GDT_GENERAL_ACCESS_BYTE | GDT_EXECUTABLE);

    // kernel data
    gdt_add_entry(GDT_GENERAL_ACCESS_BYTE);

    // user code
    gdt_add_entry(GDT_GENERAL_ACCESS_BYTE | GDT_EXECUTABLE | GDT_USER_DPL);

    // user data
    gdt_add_entry(GDT_GENERAL_ACCESS_BYTE | GDT_USER_DPL);

    gdtr.size = sizeof(gdt) - 1;
    gdtr.offset = (uint64_t) &gdt;

    gdt_reload((uint64_t) &gdtr, GDT_KERNEL_CODE, GDT_KERNEL_DATA);
}
