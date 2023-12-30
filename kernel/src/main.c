#include <stddef.h>
#include <stdbool.h>
#include <limine.h>
#include "kprint/kprint.h"
#include "x86/gdt/gdt.h"

LIMINE_BASE_REVISION(1)

struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

struct limine_bootloader_info_request bootloader_info_request = {
    .id = LIMINE_BOOTLOADER_INFO_REQUEST,
    .revision = 0
};

static void halt(void) {
    kprintf("Kernel halting...");
    asm("cli");
    while (1) asm("hlt");
}

void _start(void) {
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        halt();
    }

    if (framebuffer_request.response == NULL
     || framebuffer_request.response->framebuffer_count < 1) {
        halt();
    }

    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];
    kprint_fb_init(framebuffer);
    kprintf("Finn booted by %s v%s\n\n", bootloader_info_request.response->name, bootloader_info_request.response->version);

    gdt_init();

    halt();
}
