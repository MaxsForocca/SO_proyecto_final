#include "process.h"
#include "../print/printf/printf.h"
#include <stdint.h>

#define STACK_SIZE 0x1000  // 4 KB de pila por proceso

process_t* process_list = NULL;  // Lista circular de procesos
process_t* current_process = NULL;  // Proceso actual
uint64_t pid_counter = 1;

void init_scheduler() {
    process_list = NULL;
    current_process = NULL;
    pid_counter = 1;
}

void create_process(void (*entry_point)(), uint64_t priority) {
    process_t* new_process = (process_t*)malloc(sizeof(process_t));
    if (!new_process) {
        printf("Failed to allocate memory for process\n");
        return;
    }

    new_process->pid = pid_counter++;
    new_process->state = PROCESS_READY;  // Estado inicial
    new_process->priority = priority;    // Prioridad del proceso
    new_process->entry_point = entry_point;
    new_process->next = NULL;

    // Insertar el nuevo proceso en la lista circular
    if (!process_list) {
        process_list = new_process;
        new_process->next = new_process;  // Se apunta a sí mismo
    } else {
        process_t* temp = process_list;
        while (temp->next != process_list)
            temp = temp->next;
        temp->next = new_process;
        new_process->next = process_list;
    }

    printf("Created process PID=%lu, Priority=%lu\n", new_process->pid, new_process->priority);
}

// Nueva función para encontrar el proceso con la prioridad más alta
process_t* find_highest_priority_process() {
    process_t* selected = NULL;
    process_t* temp = process_list;

    do {
        if (temp->state == PROCESS_READY) {
            if (!selected || temp->priority > selected->priority) {
                selected = temp;
            }
        }
        temp = temp->next;
    } while (temp != process_list);

    return selected;
}

void scheduler() {
    process_t* next_process = find_highest_priority_process();

    if (next_process) {
        current_process = next_process;
        printf("Switching to process PID=%lu, Priority=%lu\n", current_process->pid, current_process->priority);
        current_process->state = PROCESS_RUNNING;

        current_process->entry_point();

        if (current_process->state != PROCESS_TERMINATED) {
            current_process->state = PROCESS_READY;
        }
    } else {
        printf("No processes ready. Switching to idle process.\n");
        current_process = process_list;  // Asumimos que el proceso idle es el primero
        current_process->state = PROCESS_RUNNING;
        current_process->entry_point();
    }
}
