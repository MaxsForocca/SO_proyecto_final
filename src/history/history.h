#ifndef HISTORY_H
#define HISTORY_H

#include <stddef.h>

#define HISTORY_SIZE 1000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25  // Definir altura estándar de pantalla VGA

extern char history[HISTORY_SIZE][VGA_WIDTH];
extern int history_count;
extern int scroll_position;

void add_to_history(const char* line);
void update_screen();

#endif
