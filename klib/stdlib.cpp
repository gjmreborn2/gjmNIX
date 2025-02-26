#include <klib/stdlib.h>

__attribute__((__noreturn__)) void abort() {
    // printf("kernel: panic: abort()\n");
    asm volatile("hlt");

    while(1) {}
    __builtin_unreachable();
}
