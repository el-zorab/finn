#include "lib/flanterm/flanterm.h"
#include "lib/flanterm/backends/fb.h"
#include "lib/printf/printf.h"
#include "kprint/kprint.h"

static struct flanterm_context *ft_ctx;

void putchar_(char c) {
    flanterm_putchar(ft_ctx, c);
}

void kprint_fb_init(struct limine_framebuffer *framebuffer) {
    ft_ctx = flanterm_fb_simple_init(framebuffer->address, framebuffer->width, framebuffer->height, framebuffer->pitch);
}

static int kvprintf(const char *format, va_list arg) {
    int ret = vprintf_(format, arg);
    if (ft_ctx->autoflush) {
        ft_ctx->double_buffer_flush(ft_ctx);
    }
    return ret;
}

int kprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int ret = kvprintf(format, args);
    va_end(args);
    return ret;
}