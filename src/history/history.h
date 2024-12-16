#ifndef HISTORY_H
#define HISTORY_H

#include <stddef.h>

#define HISTORY_SIZE 1000
#define VGA_WIDTH 80

extern char history[HISTORY_SIZE][VGA_WIDTH];
extern int history_count;
extern int scroll_position;  // Variable externa para el desplazamiento


void add_to_history(const char* line);
void update_screen();

#endif
