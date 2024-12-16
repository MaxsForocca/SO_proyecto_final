// src/io/interrupts.c
#include "interrupts.h"

void register_interrupt(uint8_t interrupt_number, void (*handler)()) {
    // Código para registrar el manejador en la tabla IDT
    // (Esto depende de cómo estés configurando la IDT en tu kernel)
}

void enable_interrupts() {
    __asm__ __volatile__("sti");  // Habilitar interrupciones
}

void disable_interrupts() {
    __asm__ __volatile__("cli");  // Deshabilitar interrupciones
}
