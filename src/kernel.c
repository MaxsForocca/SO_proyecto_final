#include "print/printf/printf.h"
#include "bootloader/bootservices.h"
#include "memory/memory.h"
#include "history/history.h"       // Manejo del historial y desplazamiento.
#include "process/process.h"       // Scheduler y procesos.
#include "resource/resource.h"     // Manejo de recursos.
#include "io/interrupts.h"         // Manejo de interrupciones.
#include "io/io.h"                 // Entrada/Salida.
#include "kernel.h"
#include <stdint.h>

void test_process_1() {
    /*printf("Proceso 1 (Recurso Exclusivo) solicitando recurso ...\n");
    //delay(100000000);  // Retardo de aproximadamente 3 segundos

    for (int i = 0; i < 5; i++) {
        printf("Proceso 1: iteración %d\n", i);
        //delay(100000000);  // Retardo antes de cada iteración
    }

    printf("Proceso 1 ha finalizado su ejecución.\n");
    current_process->state = PROCESS_TERMINATED;*/
}

void test_process_2() {
    /*
     printf("Proceso 2 (Recurso Exclusivo) solicitando recurso ...\n");
    //delay(100000000);  // Retardo de aproximadamente 3 segundos

    for (int i = 0; i < 5; i++) {
        printf("Proceso 2: iteración %d\n", i);
        //delay(100000000);  // Retardo antes de cada iteración
    }

    printf("Proceso 2 ha finalizado su ejecución.\n");
    current_process->state = PROCESS_TERMINATED;*/
}

void test_process_3() {  /*
     printf("Proceso 3 (Recurso Exclusivo) solicitando recurso ...\n");
    //delay(100000000);  // Retardo de aproximadamente 3 segundos

    for (int i = 0; i < 5; i++) {
        printf("Proceso 3: iteración %d\n", i);
        //delay(100000000);  // Retardo antes de cada iteración
    }

    printf("Proceso 3 ha finalizado su ejecución.\n");
    current_process->state = PROCESS_TERMINATED;*/
}

void test_process_4() {   /*
    printf("Proceso 4 (Recurso Exclusivo) solicitando recurso ...\n");
    //delay(100000000);  // Retardo de aproximadamente 3 segundos

    for (int i = 0; i < 5; i++) {
        printf("Proceso 4: iteración %d\n", i);
        //delay(100000000);  // Retardo antes de cada iteración
    }

    printf("Proceso 4 ha finalizado su ejecución.\n");
    current_process->state = PROCESS_TERMINATED; */
}



// Recursos compartidos y exclusivos
resource_t resource1;
resource_t resource2;
resource_t resource3;

// Cola de espera para recursos
process_t* waiting_queue = NULL;

// Contador simulado para interrupciones
int interrupt_counter = 0;

// Función para manejar la cola de espera
void add_to_waiting_queue(process_t* process) {
    process->state = PROCESS_WAITING;

    if (!waiting_queue) {
        waiting_queue = process;
        process->next = process;
    } else {
        process_t* temp = waiting_queue;
        while (temp->next != waiting_queue) {
            temp = temp->next;
        }
        temp->next = process;
        process->next = waiting_queue;
    }
}

void process_waiting_queue(resource_t* resource) {
    if (!waiting_queue) return;

    process_t* temp = waiting_queue;
    process_t* prev = NULL;

    do {
        if (temp->state == PROCESS_WAITING && !resource->is_in_use) {
            printf("Recurso %d asignado al Proceso PID=%lu\n", resource->id, temp->pid);
            resource->is_in_use = 1;
            resource->owner = temp;
            temp->state = PROCESS_READY;

            // Remover proceso de la cola
            if (prev) {
                prev->next = temp->next;
            } else {
                waiting_queue = temp->next == temp ? NULL : temp->next;
            }
            break;
        }

        prev = temp;
        temp = temp->next;
    } while (temp != waiting_queue);
}

// Proceso idle
void idle_process() {
    printf("El proceso idle está ejecutándose...\n");
    while (1) {
        __asm__("hlt");  // Detiene la CPU para ahorrar energía
    }
}

// Función para simular una interrupción
void simulate_interrupt() {
    printf("\n[Interrupción Simulada] ¡Interrupción activada!\n");
    interrupt_counter++;

    // Desbloquear procesos interactivos en espera
    process_t* temp = process_list;
    do {
        if (temp->state == PROCESS_WAITING) {
            temp->state = PROCESS_READY;
            printf("Proceso PID=%lu ahora está LISTO (despertado por interrupción).\n", temp->pid);
        }
        temp = temp->next;
    } while (temp != process_list);
}

// Función principal
void _start() {
    printf("Cargando el kernel, bootloader=%s, versión=%s\n",
           get_bootloader_name(),
           get_bootloader_version());

    // Inicialización de memoria
    printf("Inicializando la memoria...\n");
    init_memory();
    //delay(100000000);

    // Inicialización del scheduler
    printf("Inicializando el scheduler...\n");
    init_scheduler();

    // Inicialización de recursos
    printf("Inicializando recursos...\n");
    init_resource(&resource1, 1, 0); // Recurso exclusivo
    init_resource(&resource2, 2, 1); // Recurso compartido
    init_resource(&resource3, 3, 0); // Recurso exclusivo

    // Crear proceso idle
    create_process(idle_process, 0);  // Prioridad más baja

    // Crear procesos de prueba
    create_process(test_process_1, 1);  // Recurso exclusivo
    create_process(test_process_2, 2);  // Recurso compartido
    create_process(test_process_3, 3);  // Simulado con interrupciones
    create_process(test_process_4, 1);  // Otro recurso exclusivo

    // Ejecutar el scheduler
    while (1) {
        scheduler();

        // Simular interrupciones periódicamente
        if (interrupt_counter < 2) {
            simulate_interrupt();
        }
    }

    hlt();  // Detener la CPU si el loop se rompe
}
