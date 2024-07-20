__attribute__((__noreturn__)) void abort() {
    // TODO: Add kernel panic kernel: panic: abort()\n
    asm volatile("hlt");
}