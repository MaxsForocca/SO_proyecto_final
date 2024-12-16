#ifndef VGA_H
#define VGA_H

#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

extern volatile uint16_t* vga_buffer;
extern int cursor_x;  // Posición del cursor en X
extern int cursor_y;  // Posición del cursor en Y

void scroll_up();

#endif
