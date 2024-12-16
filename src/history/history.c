#include "../drivers/vga.h"
#include "../history/history.h"
#include "../util/string.h"


char history[HISTORY_SIZE][VGA_WIDTH];
int history_count = 0;

void add_to_history(const char* line) {
    if (history_count < HISTORY_SIZE) {
        strncpy(history[history_count], line, VGA_WIDTH);
        history_count++;
    } else {
        for (int i = 1; i < HISTORY_SIZE; i++) {
            strncpy(history[i - 1], history[i], VGA_WIDTH);
        }
        strncpy(history[HISTORY_SIZE - 1], line, VGA_WIDTH);
    }
}

void update_screen() {
    for (int y = 0; y < VGA_HEIGHT; y++) {
        int history_line = scroll_position + y;
        if (history_line < history_count) {
            for (int x = 0; x < VGA_WIDTH; x++) {
                vga_buffer[y * VGA_WIDTH + x] = history[history_line][x] | (0x0F << 8);
            }
        } else {
            for (int x = 0; x < VGA_WIDTH; x++) {
                vga_buffer[y * VGA_WIDTH + x] = ' ' | (0x0F << 8);
            }
        }
    }
}
