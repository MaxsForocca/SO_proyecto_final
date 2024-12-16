#include "vga.h"

volatile uint16_t* vga_buffer = (uint16_t*)0xB8000;  // Definición de vga_buffer
int cursor_x = 0;  // Posición horizontal global
int cursor_y = 0;  // Posición vertical global

void scroll_up() {
    // Desplazar todas las líneas hacia arriba
    for (int y = 1; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            vga_buffer[(y - 1) * VGA_WIDTH + x] = vga_buffer[y * VGA_WIDTH + x];
        }
    }

    // Limpiar la última línea
    for (int x = 0; x < VGA_WIDTH; x++) {
        vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = (uint16_t)(' ' | (0x0F << 8));
    }

    // Ajustar el cursor a la última línea
    cursor_y = VGA_HEIGHT - 1;
    cursor_x = 0;
}
