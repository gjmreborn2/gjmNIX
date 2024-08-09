#pragma once

#include <stdint.h>
#include <stdbool.h>

#define internal static

inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

inline uint8_t inb(uint16_t port) {
    uint8_t val;
    __asm__ volatile ("inb %w1, %b0" : "=a" (val) : "Nd" (port) : "memory");
    return val;
}

inline bool are_interrupts_enabled() {
    uint32_t eflags;
    asm volatile ( "pushf\n\t" "pop %0" : "=g" (eflags) );
    return eflags & (1 << 9);
}

inline uint64_t rdtsc() {
    uint64_t val;
    asm volatile ("rdtsc" : "=A"(val));
    return val;
}

inline uint32_t read_cr0() {
    uint32_t val;
    asm volatile ("mov %%cr0, %0" : "=r"(val));
    return val;
}

inline void wrmsr(uint32_t msr_id, uint64_t msr_value) {
    // msr_value is placed in eax:edx
    asm volatile ("wrmsr" : : "c" (msr_id), "A" (msr_value));
}

inline uint64_t rdmsr(uint32_t msr_id) {
    uint64_t msr_val;
    asm volatile ("rdmsr" : "=A" (msr_val) : "c" (msr_id));
    return msr_val;
}