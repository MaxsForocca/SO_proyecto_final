#include "../drivers/vga.h"
#include "history.h"
#include "../util/string.h"


char history[HISTORY_SIZE][VGA_WIDTH];
int history_count = 0;
int scroll_position = 0;

void add_to_history(const char* line) {
    if (history_count < HISTORY_SIZE) {
        strncpy(history[history_count], line, VGA_WIDTH);
        history[history_count][VGA_WIDTH - 1] = '\0';  // Asegurar el null-terminator.
        history_count++;
    }
}

void update_screen() {
    for (int y = 0; y < VGA_HEIGHT; y++) {
        int history_line = scroll_position + y;
        if (history_line < history_count) {
            for (int x = 0; x < VGA_WIDTH; x++) {
                vga_buffer[y * VGA_WIDTH + x] = history[history_line][x] | (0x0F << 8);  // Fondo negro, texto blanco.
            }
        }
    }
}
