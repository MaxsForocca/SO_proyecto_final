#include "../io/interrupts.h"
#include "../io/ports.h"
#include "../drivers/vga.h"
#include "../history/history.h"

// Estado del desplazamiento
int scroll_position = 0;

void keyboard_interrupt_handler() {
    uint8_t scan_code = inb(0x60);  // Leer código de escaneo

    switch (scan_code) {
        case 0x48:  // Flecha arriba
            if (scroll_position > 0) {
                scroll_position--;  // Subir en el historial
                update_screen();
            }
            break;
        case 0x50:  // Flecha abajo
            if (scroll_position < history_count - VGA_HEIGHT) {
                scroll_position++;  // Bajar en el historial
                update_screen();
            }
            break;
        default:
            break;
    }

    // Enviar End of Interrupt (EOI)
    outb(0x20, 0x20);
}
