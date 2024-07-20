#pragma once

#include <stdint.h>

inline void lidt(void *base, uint16_t size) {
    struct {
    } __attribute__((packed)) IDTR = { size, base };

    asm ("lidt %0" : : "m"(IDTR));
}