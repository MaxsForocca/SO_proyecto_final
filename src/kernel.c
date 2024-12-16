#include "print/printf/printf.h"
#include "bootloader/bootservices.h"
#include "memory/memory.h"
#include "process/process.h"
#include "resource/resource.h"
#include "kernel.h"
#include <stdint.h>

// Recurso compartido
resource_t resource1 = { .id = 1, .is_in_use = 0, .owner = NULL };

// Proceso de prueba 1
void test_process_1() {
    printf("Process 1 (Batch) is requesting resource 1...\n");
    use_resource(current_process, &resource1);

    for (int i = 0; i < 5; i++) {  // Simular trabajo
        printf("Process 1: iteration %d\n", i);
    }

    release_resource(&resource1);
    printf("Process 1 has finished execution.\n");
    current_process->state = PROCESS_TERMINATED;
}

// Proceso de prueba 2
void test_process_2() {
    printf("Process 2 (Interactive) is requesting resource 1...\n");
    use_resource(current_process, &resource1);

    for (int i = 0; i < 3; i++) {  // Simular trabajo
        printf("Process 2: iteration %d\n", i);
    }

    release_resource(&resource1);
    printf("Process 2 has finished execution.\n");
    current_process->state = PROCESS_TERMINATED;
}

// Proceso de prueba 3
void test_process_3() {
    printf("Process 3 (High Priority) is running...\n");

    for (int i = 0; i < 2; i++) {  // Simular trabajo más corto
        printf("Process 3: iteration %d\n", i);
    }

    printf("Process 3 has finished execution.\n");
    current_process->state = PROCESS_TERMINATED;
}

// Proceso idle
void idle_process() {
    printf("Idle process is running...\n");
    while (1) {
        __asm__("hlt");  // Detener la CPU para ahorrar energía
    }
}

void _start() {
    printf("Loading kernel, bootloader=%s, version=%s\n",
        get_bootloader_name(),
        get_bootloader_version()
    );

    // Inicialización de memoria
    printf("Initializing memory...\n");
    init_memory();

    // Inicialización del scheduler
    printf("Initializing scheduler...\n");
    init_scheduler();

    // Crear proceso idle
    create_process(idle_process, 0);  // Prioridad más baja

    // Crear procesos de prueba
    create_process(test_process_1, 1);  // Prioridad baja
    create_process(test_process_2, 2);  // Prioridad media
    create_process(test_process_3, 3);  // Prioridad alta

    // Ejecutar el scheduler
    while (1) {
        scheduler();
    }

    hlt();  // Detener la CPU si el loop se rompe
}
