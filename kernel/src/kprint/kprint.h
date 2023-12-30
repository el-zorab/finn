#pragma once

#include <limine.h>
#include <stdarg.h>

void kprint_fb_init(struct limine_framebuffer *framebuffer);
int kprintf(const char *format, ...);
