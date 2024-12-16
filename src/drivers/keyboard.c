#include "../io/interrupts.h"
#include "../io/ports.h"
#include "../drivers/vga.h"
#include "../history/history.h"  // Si estas definiciones están aquí
#include "keyboard.h"


void keyboard_interrupt_handler() {
    uint8_t scan_code = inb(0x60);  // Leer código de escaneo

    if (scan_code == 0x48 && scroll_position > 0) {  // Flecha arriba
        scroll_position--;
        update_screen();
    } else if (scan_code == 0x50 && scroll_position < history_count - VGA_HEIGHT) {  // Flecha abajo
        scroll_position++;
        update_screen();
    }

    // Enviar End of Interrupt (EOI)
    outb(0x20, 0x20);
}

