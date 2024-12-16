// src/io/interrupts.h
#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>

// Función para registrar un manejador de interrupciones
void register_interrupt(uint8_t interrupt_number, void (*handler)());

// Función para habilitar interrupciones
void enable_interrupts();

// Función para deshabilitar interrupciones
void disable_interrupts();

#endif
