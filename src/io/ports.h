#ifndef PORTS_H
#define PORTS_H

#include <stdint.h>

static inline uint8_t inb(uint16_t port) {
    uint8_t value;
    __asm__ __volatile__("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

#endif // PORTS_H

