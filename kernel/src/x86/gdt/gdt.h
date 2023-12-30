#pragma once

enum GDT_SELECTORS {
    GDT_KERNEL_CODE = 0x8,
    GDT_KERNEL_DATA = 0x10,
    GDT_USER_CODE   = 0x18,
    GDT_USER_DATA   = 0x20
};

void gdt_init();
