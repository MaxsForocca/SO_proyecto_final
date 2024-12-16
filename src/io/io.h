#ifndef IO_H
#define IO_H

#include <stdint.h>

// Función inline para escribir un byte en un puerto
static inline void outb(uint16_t port, uint8_t value) {
    __asm__ __volatile__("outb %0, %1" : : "a"(value), "Nd"(port));
}
void delay(unsigned long long ticks) {
    volatile unsigned long long i = 0;
    while (i < ticks) {
        i++;  // Incremento para consumir tiempo
    }
}
#endif // IO_H
