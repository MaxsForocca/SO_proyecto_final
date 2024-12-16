#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

// Estados de los procesos
#define PROCESS_READY      0
#define PROCESS_RUNNING    1
#define PROCESS_TERMINATED 2
#define PROCESS_WAITING    3  // Nuevo estado para procesos en espera

#define DEFAULT_QUANTUM    5  // Tiempo máximo de ejecución para cada proceso

// Declaración del tipo de proceso
typedef struct process {
    uint64_t pid;                  // ID único del proceso
    uint64_t state;                // Estado del proceso
    uint64_t quantum_remaining;    // Quantum actual del proceso
    uint64_t priority;             // Prioridad del proceso
    uint64_t* stack_pointer;       // Puntero a la pila del proceso
    void (*entry_point)();         // Función asociada al proceso
    struct process* next;          // Apunta al siguiente proceso (lista circular)
} process_t;

// Declaración de variables externas
extern process_t *current_process;

// Funciones relacionadas con los procesos
void create_process(void (*entry_point)(), uint64_t priority);
void delete_process(process_t *process);  // Nueva función para eliminar procesos
void init_scheduler();
void scheduler();

#endif
