#include "interrupts.h"

#define MAX_INTERRUPTS 256

static void (*interrupt_handlers[MAX_INTERRUPTS])();  // Arreglo de manejadores

void register_interrupt(uint16_t interrupt_number, void (*handler)()) {
    if (interrupt_number >= MAX_INTERRUPTS) return;  // Validar límite de interrupciones
    interrupt_handlers[interrupt_number] = handler;
}

void trigger_interrupt(uint16_t interrupt_number) {
    if (interrupt_number >= MAX_INTERRUPTS) return;  // Validar límite de interrupciones
    if (interrupt_handlers[interrupt_number]) {
        interrupt_handlers[interrupt_number]();
    }
}

void enable_interrupts() {
    __asm__ __volatile__("sti");  // Habilitar interrupciones
}

void disable_interrupts() {
    __asm__ __volatile__("cli");  // Deshabilitar interrupciones
}
