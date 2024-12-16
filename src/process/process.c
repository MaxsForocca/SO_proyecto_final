#include "process.h"
#include "../memory/memory.h"
#include "../print/printf/printf.h"

#define STACK_SIZE 0x1000  // 4 KB de pila por proceso

process_t *process_list = NULL;  // Lista circular de procesos
process_t *current_process = NULL;  // Proceso actual
uint64_t pid_counter = 1;

void init_scheduler() {
    process_list = NULL;
    current_process = NULL;
    pid_counter = 1;
}

void create_process(void (*entry_point)(), uint64_t priority) {
    process_t *new_process = (process_t*)malloc(sizeof(process_t));

    if (!new_process) {
        printf("Failed to allocate memory for process\n");
        return;
    }

    // Inicializa el PCB
    new_process->pid = pid_counter++;
    new_process->state = PROCESS_READY;
    new_process->priority = priority;
    new_process->quantum_remaining = DEFAULT_QUANTUM;
    new_process->entry_point = entry_point;

    // Asignar y configurar la pila
    uint64_t *stack = (uint64_t*)malloc(STACK_SIZE);
    if (!stack) {
        printf("Failed to allocate stack for process\n");
        free(new_process);
        return;
    }
    new_process->stack_pointer = stack + (STACK_SIZE / sizeof(uint64_t));  // Pila crece hacia abajo

    // Añadir a la lista circular de procesos
    if (!process_list) {
        process_list = new_process;
        new_process->next = new_process;  // Apuntarse a sí mismo
    } else {
        process_t *temp = process_list;
        while (temp->next != process_list) {
            temp = temp->next;
        }
        temp->next = new_process;
        new_process->next = process_list;  // Cierra la lista circular
    }

    printf("Created process PID=%lu, Priority=%lu\n", new_process->pid, new_process->priority);
}


void delete_process(process_t *process) {
    if (!process) return;

    // Liberar la pila asignada al proceso
    if (process->stack_pointer) {
        free(process->stack_pointer - (STACK_SIZE / sizeof(uint64_t)));
    }

    // Liberar el PCB
    free(process);
}


void scheduler() {
    if (!process_list) {
        printf("No processes to schedule.\n");
        return;
    }

    while (1) {
        // Verificar si todos los procesos están terminados
        process_t* temp = process_list;
        int all_terminated = 1;
        do {
            if (temp->state != PROCESS_TERMINATED) {
                all_terminated = 0;
                break;
            }
            temp = temp->next;
        } while (temp != process_list);

        if (all_terminated) {
            printf("All processes terminated. Halting system.\n");
            while (1) __asm__("hlt");
        }

        // Continuar con el Round Robin
        if (!current_process) {
            current_process = process_list;
        } else {
            current_process = current_process->next ? current_process->next : process_list;
        }

        if (current_process->state == PROCESS_READY) {
            printf("Switching to process PID=%lu, Priority=%lu\n", current_process->pid, current_process->priority);
            current_process->state = PROCESS_RUNNING;

            current_process->entry_point();

            if (current_process->state != PROCESS_TERMINATED) {
                current_process->state = PROCESS_READY;
            }
            break;
        }
    }
}

