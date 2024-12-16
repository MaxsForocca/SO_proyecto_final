#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>

#define MAX_INTERRUPTS 256

void register_interrupt(uint16_t interrupt_number, void (*handler)());
void trigger_interrupt(uint16_t interrupt_number);
void enable_interrupts();
void disable_interrupts();

#endif

